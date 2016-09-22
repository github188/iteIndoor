/*************************************************
Copyright (C),  2006-2016, Aurine
File name:		layer_rtsp_search.c
Author:     	caogw
Version:    	
Date: 			2016-08-24
Description:
History:
1. Date:
Author:
Modification:
*************************************************/
#include "gui_include.h"

/*****************常量定义***********************/
#define MAX_PAGE_NUM			6					// 每页最大条数
#define MAX_RTSP_NUM  			24					// 最大管理员机数
#define MAX_RTSP_PAGE_NUM  		4					// 管理员机最大页数

/*****************变量定义***********************/
static ITULayer* MainLayer = NULL;
static ITULayer* RTSPSearchLayer = NULL;
static ITUBackground* RTSPSearchRightBackground = NULL;
static ITURadioBox* RTSPSearchRightHomeRadioBox = NULL;
static ITURadioBox* RTSPSearchRightPublicRadioBox = NULL;
static ITUBackground* RTSPSearchBottomBackground = NULL;
static ITUCoverFlow* RTSPSearchListCoverFlow = NULL;
static ITUBackground* RTSPSearchListBackgroundPage[MAX_RTSP_PAGE_NUM] = { NULL };
static ITUContainer* RTSPSearchListContainer[MAX_RTSP_NUM] = { NULL };
static ITUButton* RTSPSearchListButton[MAX_RTSP_NUM] = { NULL };
static ITUSprite* RTSPSearchListSprite[MAX_RTSP_NUM] = { NULL };
static ITUText* RTSPSearchListDevNameText[MAX_RTSP_NUM] = { NULL };
static ITUText* RTSPSearchListOnLineText[MAX_RTSP_NUM] = { NULL };
static ITUBackground* RTSPSearchMSGHitGrayBackground = NULL;
static ITUAnimation* RTSPSearchMSGHitAnimation = NULL;
static ITUContainer* RTSPSearchBottomSearchContainer = NULL;
static ITUBackground* RTSPSearchBackground = NULL;
static ITUContainer* RTSPSearchBottomNullContainer0 = NULL;
static ITUButton* RTSPSearchBottomNullButton[4] = { NULL };
static ITUButton* RTSPSearchRightNullButton[2] = { NULL };

/*****************常量定义***********************/
static PRtspDeviceList  g_RTSPList = NULL;
static uint8  g_Type = 0;							// 0 家居监视 1 社区监视
static uint32 g_RTSPSearchLastTick = 0;				// 实时更新的tick
static uint8 g_ShowLoadTick = 0;
static uint8 g_FenJiLoadTick = 0;					// 家居分机向主机同步

typedef enum
{
	RTSPHomeEvent = 0x00,
	RTSPPublicEvent,
	RTSPSearchEvent,
	RTSPExitEvent,
	RTSPMaxEvent,
}RTSPButtonEvent;

// 监视图标类型
typedef enum
{
	RTSPHomeIcon = 0x00,
	RTSPPublicIcon,
	RTSPMaxIcon,
}RTSPIconType;

/*************************************************
Function:		SetRTSPShowInit
Description: 	显示页面初始化
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void SetRTSPShowInit(void)
{
	uint8 i;

	for (i = 0; i < MAX_RTSP_PAGE_NUM; i++)
	{
		ituWidgetSetVisible(RTSPSearchListBackgroundPage[i], true);
	}

	for (i = 0; i < MAX_RTSP_NUM; i++)
	{
		ituWidgetSetVisible(RTSPSearchListContainer[i], true);
		ituWidgetDisable(RTSPSearchListButton[i]);
		ituTextSetString(RTSPSearchListDevNameText[i], "");
		ituWidgetSetVisible(RTSPSearchListOnLineText[i], true);
		ituTextSetString(RTSPSearchListOnLineText[i], "");
		ituWidgetSetVisible(RTSPSearchListSprite[i], false);
	}

	// 初始化默认为首页
	// 解决RTSPSearchListCoverFlow第一页是-1的情况
	if (RTSPSearchListCoverFlow->frame == RTSPSearchListCoverFlow->totalframe)
	{
		ituCoverFlowGoto(RTSPSearchListCoverFlow, -1);
	}
	else
	{
		ituCoverFlowGoto(RTSPSearchListCoverFlow, 0);
	}
}

/*************************************************
Function:		SetRTSPShowNum
Description: 	显示监视列表具体条数
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void SetRTSPShowNum(uint8 max)
{
	uint8 i, count = 0;
	uint8 pagenum = 0, rest = 0;

	// 隐藏多余页
	if (max <= MAX_PAGE_NUM)
	{
		pagenum = 1;
	}
	else
	{
		pagenum = max / MAX_PAGE_NUM;
		rest = max % MAX_PAGE_NUM;
		if (rest > 0)
		{
			pagenum++;
		}
	}

	for (i = MAX_RTSP_PAGE_NUM; i > pagenum; i--)
	{
		ituWidgetSetVisible(RTSPSearchListBackgroundPage[i - 1], false);
	}

#if 0
	// 隐藏多余行
	count = pagenum * MAX_PAGE_NUM;
	if (count > MAX_RTSP_NUM)
	{
		count = MAX_RTSP_NUM;
	}
#endif

	for (i = 0; i < max; i++)
	{
		ituWidgetEnable(RTSPSearchListButton[i]);
	}

	printf("SetRTSPShowNum..:count %d\n", count);
}

/*************************************************
Function:		ShowRtspWin
Description: 	显示rtsp列表
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void ShowRtspWin(uint8 type)
{
	uint8 i, max = 0;
	uint32 strid = 0;
	RTSPIconType icontype = RTSPHomeIcon;

	SetRTSPShowInit();
	g_RTSPList = rtsp_monitor_get_devlist(type);
	if (NULL == g_RTSPList)
	{
		return;
	}

	if (g_RTSPList && g_RTSPList->DevNum)
	{
		max = g_RTSPList->DevNum;
		if (max > MAX_RTSP_NUM)
		{
			max = MAX_RTSP_NUM;
		}
		// 图标类型
		if (0 == type)
		{
			icontype = RTSPHomeIcon;
		}
		else 
		{
			icontype = RTSPPublicIcon;
		}

		for (i = 0; i < max; i++)
		{
			ituWidgetSetVisible(RTSPSearchListSprite[i], true);
			ituSpriteGoto(RTSPSearchListSprite[i], icontype);
			ituTextSetString(RTSPSearchListDevNameText[i], g_RTSPList->Devinfo[i].DeviceName);
			#ifdef _NEW_SELF_IPC_
			if (0 == strcmp(g_RTSPList->Devinfo[i].FactoryName, "SelfIPC")
				|| 0 == strcmp(g_RTSPList->Devinfo[i].FactoryName, "SELFIPC"))
			{
				if (0 == type)
				{
					uint8 index = storage_get_index(i);
					if (get_ipc_state(index))
					{
						strid = SID_Set_Online;
					}
					else
					{
						strid = SID_Set_Offline;
					}
				}
				else
				{
					dprintf("g_RTSPList->Devinfo[%d].DeviceIP...: %x\n", i, g_RTSPList->Devinfo[i].DeviceIP);
					if (get_commdev_state(g_RTSPList->Devinfo[i].DeviceIP))
					{
						strid = SID_Set_Online;
					}
					else
					{
						strid = SID_Set_Offline;
					}
				}
				ituTextSetString(RTSPSearchListOnLineText[i], get_str(strid));
			}
			else
			#endif
			{
				ituWidgetSetVisible(RTSPSearchListOnLineText[i], false);
			}
		}
		SetRTSPShowNum(max);
	}
	else
	{
		SetRTSPShowNum(0);
	}
}

/*************************************************
Function:		InitRTSPSWin
Description: 	搜索RTSP页面初始化
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void InitRTSPSWin(void)
{
	g_FenJiLoadTick = 0;
	g_ShowLoadTick = 0;
	ituWidgetSetVisible(RTSPSearchMSGHitGrayBackground, false);
	if (0 == g_Type)
	{
		ituRadioBoxSetChecked(RTSPSearchRightPublicRadioBox, false);
		ituRadioBoxSetChecked(RTSPSearchRightHomeRadioBox, true);
		ituWidgetSetVisible(RTSPSearchBottomSearchContainer, false);
		ituWidgetSetVisible(RTSPSearchBottomNullContainer0, true);
		ituWidgetDisable(RTSPSearchBottomNullButton[0]);
		if (is_main_DeviceNo())
		{
			ShowRtspWin(0);
		}
		else
		{
			SetRTSPShowNum(0);
			g_FenJiLoadTick = 3;
			ituWidgetDisable(RTSPSearchBackground);
			ituWidgetSetVisible(RTSPSearchMSGHitGrayBackground, true);
		}
	}
	else
	{
		ituRadioBoxSetChecked(RTSPSearchRightHomeRadioBox, false);
		ituRadioBoxSetChecked(RTSPSearchRightPublicRadioBox, true);
		ituWidgetSetVisible(RTSPSearchBottomSearchContainer, true);
		ituWidgetSetVisible(RTSPSearchBottomNullContainer0, false);
		SetRTSPShowNum(0);
		g_ShowLoadTick = 1;
		ituWidgetDisable(RTSPSearchBackground);
		ituWidgetSetVisible(RTSPSearchMSGHitGrayBackground, true);
	}
	g_RTSPSearchLastTick = SDL_GetTicks();
}

/*************************************************
Function:		RTSPSearchDestroyProc
Description: 	销毁处理函数
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void RTSPSearchDestroyProc(void)
{
	rtsp_monitor_list_free(); 
}

/*************************************************
Function:		RTSPSearchLayerOnTimer
Description: 	定时器
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool RTSPSearchLayerOnTimer(ITUWidget* widget, char* param)
{
	uint32 tick = SDL_GetTicks();
	uint32 diff = tick - g_RTSPSearchLastTick;
	if (diff >= 1000)
	{
		g_RTSPSearchLastTick = tick;
		// 搜索列表
		if (g_ShowLoadTick)
		{
			g_ShowLoadTick--;
			if (0 == g_ShowLoadTick)
			{
				rtsp_monitor_sync_devlist();
			}
		}
		// 分机同步
		if (g_FenJiLoadTick)
		{
			g_FenJiLoadTick--;
			if (2 == g_FenJiLoadTick)
			{
				fenji_sync_ipc_list();
			}
			else if (0 == g_FenJiLoadTick)
			{
				ituWidgetEnable(RTSPSearchBackground);
				ituWidgetSetVisible(RTSPSearchMSGHitGrayBackground, false);
				ShowRtspWin(0);
			}
		}
	}

	return true;
}

/*************************************************
Function:		RTSPSearchListState
Description: 	获取监视列表执行函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool RTSPSearchListState(ITUWidget* widget, char* param)
{
	PINTER_CALLBACK prtspbak_data = (PINTER_CALLBACK)param;

	dprintf("prtspbak_data->InterState.......%d\n", prtspbak_data->InterState);
	if (prtspbak_data->InterState == MONITOR_GETLIST)
	{
		ituWidgetEnable(RTSPSearchBackground);
		ituWidgetSetVisible(RTSPSearchMSGHitGrayBackground, false);
		ShowRtspWin(1);
	}

	return true;
}

/*************************************************
Function:		RTSPSearchListButtonOnMouseUp
Description: 	监视列表按下执行函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool RTSPSearchListButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 index = atoi(param);

	if (g_RTSPList && g_RTSPList->Devinfo)
	{
		RTSPMonitorWin(g_RTSPList, index);
	}

	return true;
}

/*************************************************
Function:		RTSPSearchLayerButtonOnMouseUp
Description: 	监视搜索按键弹起函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool RTSPSearchLayerButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 btn_event = atoi(param);

	switch (btn_event)
	{
		case RTSPHomeEvent:
			ituWidgetSetVisible(RTSPSearchBottomSearchContainer, false);
			if (is_main_DeviceNo())
			{
				g_Type = 0;
				ShowRtspWin(0);
			}
			else
			{
				g_FenJiLoadTick = 3;
				ituWidgetDisable(RTSPSearchBackground);
				ituWidgetSetVisible(RTSPSearchMSGHitGrayBackground, true);
			}
			break;

		case RTSPPublicEvent:
			ituWidgetSetVisible(RTSPSearchBottomSearchContainer, true);
			g_Type = 1;
			g_ShowLoadTick = 1;
			ituWidgetDisable(RTSPSearchBackground);
			ituWidgetSetVisible(RTSPSearchMSGHitGrayBackground, true);
			break;


		case RTSPSearchEvent:
			g_ShowLoadTick = 1;
			ituWidgetDisable(RTSPSearchBackground);
			ituWidgetSetVisible(RTSPSearchMSGHitGrayBackground, true);
			break;

		case RTSPExitEvent:
			RTSPSearchDestroyProc();
			ituLayerGoto(MainLayer);
			break;

		default:
			break;
	}
	return true;
}

/*************************************************
Function:		RTSPSearchLayerOnEnter
Description: 	监视搜索初始化处理函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool RTSPSearchLayerOnEnter(ITUWidget* widget, char* param)
{
	uint8 i;

	InitRTSPSWin();

	for (i = 0; i < 2; i++)
	{
		ituWidgetDisable(RTSPSearchRightNullButton[i]);
	}
	for (i = 1; i < 4; i++)
	{
		ituWidgetDisable(RTSPSearchBottomNullButton[i]);
	}
	
	return true;
}

/*************************************************
Function:		RTSPSearchLayerStart
Description: 	进入RTSP搜索函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool RTSPSearchLayerStart(ITUWidget* widget, char* param)
{
	uint8 index = atoi(param);
	int32 ret = ui_show_win_arbitration(SYS_OPER_MONITOR);
	if (ret == TRUE)
	{
		if (!RTSPSearchLayer)
		{
			uint8 i;
			char callname[50];

			MainLayer = ituSceneFindWidget(&theScene, "mainLayer");
			assert(MainLayer);

			RTSPSearchLayer = ituSceneFindWidget(&theScene, "RTSPSearchLayer");
			assert(RTSPSearchLayer);

			RTSPSearchRightBackground = ituSceneFindWidget(&theScene, "RTSPSearchRightBackground");
			assert(RTSPSearchRightBackground);

			RTSPSearchRightHomeRadioBox = ituSceneFindWidget(&theScene, "RTSPSearchRightHomeRadioBox");
			assert(RTSPSearchRightHomeRadioBox);

			RTSPSearchRightPublicRadioBox = ituSceneFindWidget(&theScene, "RTSPSearchRightPublicRadioBox");
			assert(RTSPSearchRightPublicRadioBox);

			RTSPSearchBottomBackground = ituSceneFindWidget(&theScene, "RTSPSearchBottomBackground");
			assert(RTSPSearchBottomBackground);

			RTSPSearchListCoverFlow = ituSceneFindWidget(&theScene, "RTSPSearchListCoverFlow");
			assert(RTSPSearchListCoverFlow);

			RTSPSearchMSGHitGrayBackground = ituSceneFindWidget(&theScene, "RTSPSearchMSGHitGrayBackground");
			assert(RTSPSearchMSGHitGrayBackground);

			RTSPSearchMSGHitAnimation = ituSceneFindWidget(&theScene, "RTSPSearchMSGHitAnimation");
			assert(RTSPSearchMSGHitAnimation);

			RTSPSearchBottomSearchContainer = ituSceneFindWidget(&theScene, "RTSPSearchBottomSearchContainer");
			assert(RTSPSearchBottomSearchContainer);

			RTSPSearchBackground = ituSceneFindWidget(&theScene, "RTSPSearchBackground");
			assert(RTSPSearchBackground);

			RTSPSearchBottomNullContainer0 = ituSceneFindWidget(&theScene, "RTSPSearchBottomNullContainer0");
			assert(RTSPSearchBottomNullContainer0);

			for (i = 0; i < MAX_RTSP_PAGE_NUM; i++)
			{
				memset(callname, 0, sizeof(callname));
				sprintf(callname, "%s%d", "RTSPSearchListBackgroundPage", i);
				RTSPSearchListBackgroundPage[i] = ituSceneFindWidget(&theScene, callname);
				assert(RTSPSearchListBackgroundPage[i]);
			}

			for (i = 0; i < MAX_RTSP_NUM; i++)
			{
				memset(callname, 0, sizeof(callname));
				sprintf(callname, "%s%d", "RTSPSearchListContainer", i);
				RTSPSearchListContainer[i] = ituSceneFindWidget(&theScene, callname);
				assert(RTSPSearchListContainer[i]);

				memset(callname, 0, sizeof(callname));
				sprintf(callname, "%s%d", "RTSPSearchListButton", i);
				RTSPSearchListButton[i] = ituSceneFindWidget(&theScene, callname);
				assert(RTSPSearchListButton[i]);

				memset(callname, 0, sizeof(callname));
				sprintf(callname, "%s%d", "RTSPSearchListDevNameText", i);
				RTSPSearchListDevNameText[i] = ituSceneFindWidget(&theScene, callname);
				assert(RTSPSearchListDevNameText[i]);

				memset(callname, 0, sizeof(callname));
				sprintf(callname, "%s%d", "RTSPSearchListOnLineText", i);
				RTSPSearchListOnLineText[i] = ituSceneFindWidget(&theScene, callname);
				assert(RTSPSearchListOnLineText[i]);

				memset(callname, 0, sizeof(callname));
				sprintf(callname, "%s%d", "RTSPSearchListSprite", i);
				RTSPSearchListSprite[i] = ituSceneFindWidget(&theScene, callname);
				assert(RTSPSearchListSprite[i]);
			}
			
			for (i = 0; i < 2; i++)
			{
				memset(callname, 0, sizeof(callname));
				sprintf(callname, "%s%d", "RTSPSearchRightNullButton", i);
				RTSPSearchRightNullButton[i] = ituSceneFindWidget(&theScene, callname);
				assert(RTSPSearchRightNullButton[i]);
			}

			for (i = 0; i < 4; i++)
			{
				memset(callname, 0, sizeof(callname));
				sprintf(callname, "%s%d", "RTSPSearchBottomNullButton", i);
				RTSPSearchBottomNullButton[i] = ituSceneFindWidget(&theScene, callname);
				assert(RTSPSearchBottomNullButton[i]);
			}
		}
		g_Type = index;
		ituLayerGoto(RTSPSearchLayer);
	}

	return true;
}

/*************************************************
Function:		RTSPSearchReset
Description: 	监视搜索界面初始化函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
void RTSPSearchReset(void)
{
	RTSPSearchLayer = NULL;
}

