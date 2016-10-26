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
#define MAX_RTSP_NUM  			IPC_DEVICE_NUM		// 最大IPC数

/*****************变量定义***********************/
static ITULayer* MainLayer = NULL;
static ITULayer* RTSPSearchLayer = NULL;
static ITUBackground* RTSPSearchRightBackground = NULL;
static ITURadioBox* RTSPSearchRightHomeRadioBox = NULL;
static ITURadioBox* RTSPSearchRightPublicRadioBox = NULL;
static ITUBackground* RTSPSearchBottomBackground = NULL;
static ITUCoverFlow* RTSPSearchListCoverFlow = NULL;
static ITUContainer* RTSPSearchListContainer[LIST_ITEM_COUNT] = { NULL };
static ITUButton* RTSPSearchListButton[LIST_ITEM_COUNT] = { NULL };
static ITUSprite* RTSPSearchListSprite[LIST_ITEM_COUNT] = { NULL };
static ITUText* RTSPSearchListDevNameText[LIST_ITEM_COUNT] = { NULL };
static ITUText* RTSPSearchListOnLineText[LIST_ITEM_COUNT] = { NULL };
static ITUBackground* RTSPSearchMSGHitGrayBackground = NULL;
static ITUContainer* RTSPSearchBottomSearchContainer = NULL;
static ITUBackground* RTSPSearchBackground = NULL;
static ITUContainer* RTSPSearchBottomNullContainer0 = NULL;
static ITUButton* RTSPSearchBottomNullButton[4] = { NULL };
static ITUButton* RTSPSearchRightNullButton[2] = { NULL };

static ITUButton* g_RTSPSearchListButton[MAX_RTSP_NUM] = { NULL };
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
Function:		ShowRTSPNoneWin
Description: 	显示空的rtsp列表，分机搜索时显示为空
Input:			
	1.count		空列表条数
Output:			无
Return:			无
Others:			无
*************************************************/
static void ShowRTSPNoneWin(uint8 count)
{
	char tmp[50];
	uint32 ypos = 0;
	uint8 i, result = 0;
	ITUButton* CloneChildRTSPSearchListButton;
	ITUText* CloneChildRTSPSearchListDevNameText;
	ITUText* CloneChildRTSPSearchListOnLineText;
	ITUSprite* CloneChildRTSPSearchListSprite;
	ITUContainer* CloneRTSPSearchListContainer;
	ITUContainer* OldRTSPSearchListContainer;

	// 删除已经Clone的List
	for (i = LIST_ITEM_COUNT; i < count; i++)
	{
		if (NULL != g_RTSPSearchListButton[i])
		{
			g_RTSPSearchListButton[i] = NULL;
			OldRTSPSearchListContainer = NULL;
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "RTSPSearchListContainer", i);
			OldRTSPSearchListContainer = (ITUContainer*)ituSceneFindWidget(&theScene, tmp);
			itcTreeRemove(OldRTSPSearchListContainer);						// 删除已有的
			ituWidgetExit(OldRTSPSearchListContainer);
			dprintf("Remove RTSPSearchListContainer List!!!\n");
		}
	}
	// 默认显示从第一条开始显示
	ituCoverFlowGoto(RTSPSearchListCoverFlow, 0);

	for (i = 0; i < count; i++)
	{
		CloneChildRTSPSearchListButton = NULL;
		CloneChildRTSPSearchListDevNameText = NULL;
		CloneChildRTSPSearchListOnLineText = NULL;
		CloneChildRTSPSearchListSprite = NULL;
		CloneRTSPSearchListContainer = NULL;

		if (i < LIST_ITEM_COUNT)
		{
			CloneChildRTSPSearchListButton = RTSPSearchListButton[i];
			CloneChildRTSPSearchListDevNameText = RTSPSearchListDevNameText[i];
			CloneChildRTSPSearchListOnLineText = RTSPSearchListOnLineText[i];
			CloneChildRTSPSearchListSprite = RTSPSearchListSprite[i];

			// 模板初始化
			ituWidgetDisable(CloneChildRTSPSearchListButton);
			ituWidgetSetVisible(CloneChildRTSPSearchListSprite, false);
			ituTextSetString(CloneChildRTSPSearchListDevNameText, "");
			ituTextSetString(CloneChildRTSPSearchListOnLineText, "");
		}
		else
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "RTSPSearchListContainer", i);
			result = ituWidgetClone(RTSPSearchListContainer[1], &CloneRTSPSearchListContainer);
			if (result)
			{
				ituWidgetSetName(CloneRTSPSearchListContainer, tmp);
				ituWidgetSetX(CloneRTSPSearchListContainer, 0);
				if (i < MAX_PAGE_NUM)
				{
					ypos = i * 62;
				}
				else
				{
					ypos = 0;
				}
				ituWidgetSetY(CloneRTSPSearchListContainer, ypos);

				memset(tmp, 0, sizeof(tmp));
				CloneChildRTSPSearchListButton = itcTreeGetChildAt(CloneRTSPSearchListContainer, 0);
				sprintf(tmp, "%s%d", "RTSPSearchListButton", i);
				ituWidgetSetName(CloneChildRTSPSearchListButton, tmp);

				memset(tmp, 0, sizeof(tmp));
				CloneChildRTSPSearchListDevNameText = itcTreeGetChildAt(CloneRTSPSearchListContainer, 1);
				sprintf(tmp, "%s%d", "RTSPSearchListDevNameText", i);
				ituWidgetSetName(CloneChildRTSPSearchListDevNameText, tmp);

				memset(tmp, 0, sizeof(tmp));
				CloneChildRTSPSearchListOnLineText = itcTreeGetChildAt(CloneRTSPSearchListContainer, 2);
				sprintf(tmp, "%s%d", "RTSPSearchListOnLineText", i);
				ituWidgetSetName(CloneChildRTSPSearchListOnLineText, tmp);

				memset(tmp, 0, sizeof(tmp));
				CloneChildRTSPSearchListSprite = itcTreeGetChildAt(CloneRTSPSearchListContainer, 3);
				sprintf(tmp, "%s%d", "RTSPSearchListSprite", i);
				ituWidgetSetName(CloneChildRTSPSearchListSprite, tmp);

				ituWidgetAdd(RTSPSearchListCoverFlow, CloneRTSPSearchListContainer);
			}
			ituCoverFlowUpdate((ITUWidget*)RTSPSearchListCoverFlow, ITU_EVENT_LAYOUT, 0, 0, 0);
		}
		g_RTSPSearchListButton[i] = CloneChildRTSPSearchListButton;
	}
}

/*************************************************
Function:		ShowRTSPWin
Description: 	显示rtsp列表
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void ShowRTSPWin(uint8 type)
{
	char tmp[50];
	uint32 strid = 0, ypos = 0;
	uint8 i, max = 0, count = 0, result = 0;
	RTSPIconType icontype = RTSPHomeIcon;
	ITUButton* CloneChildRTSPSearchListButton;
	ITUText* CloneChildRTSPSearchListDevNameText;
	ITUText* CloneChildRTSPSearchListOnLineText;
	ITUSprite* CloneChildRTSPSearchListSprite;
	ITUContainer* CloneRTSPSearchListContainer;
	ITUContainer* OldRTSPSearchListContainer;

	// 删除已经Clone的List
	for (i = LIST_ITEM_COUNT; i < MAX_RTSP_NUM; i++)
	{
		if (NULL != g_RTSPSearchListButton[i])
		{
			g_RTSPSearchListButton[i] = NULL;
			OldRTSPSearchListContainer = NULL;
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "RTSPSearchListContainer", i);
			OldRTSPSearchListContainer = (ITUContainer*)ituSceneFindWidget(&theScene, tmp);
			itcTreeRemove(OldRTSPSearchListContainer);						// 删除已有的
			ituWidgetExit(OldRTSPSearchListContainer);
			dprintf("Remove RTSPSearchListContainer List!!!\n");
		}
	}

	g_RTSPList = rtsp_monitor_get_devlist(type);
	if (NULL == g_RTSPList)
	{
		return;
	}
	// 默认显示从第一条开始显示
	ituCoverFlowGoto(RTSPSearchListCoverFlow, 0);

	max = g_RTSPList->DevNum;
	if (max > MAX_RTSP_NUM)
	{
		max = MAX_RTSP_NUM;
	}
	if (max <= MAX_PAGE_NUM)
	{
		count = MAX_PAGE_NUM;
	}
	else
	{
		count = max;
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

	for (i = 0; i < count; i++)
	{
		CloneChildRTSPSearchListButton = NULL;
		CloneChildRTSPSearchListDevNameText = NULL;
		CloneChildRTSPSearchListOnLineText = NULL;
		CloneChildRTSPSearchListSprite = NULL;
		CloneRTSPSearchListContainer = NULL;

		if (i < LIST_ITEM_COUNT)
		{
			CloneChildRTSPSearchListButton = RTSPSearchListButton[i];
			CloneChildRTSPSearchListDevNameText = RTSPSearchListDevNameText[i];
			CloneChildRTSPSearchListOnLineText = RTSPSearchListOnLineText[i];
			CloneChildRTSPSearchListSprite = RTSPSearchListSprite[i];

			// 模板初始化
			ituWidgetEnable(CloneChildRTSPSearchListButton);
			ituWidgetSetVisible(CloneChildRTSPSearchListSprite, true);
			ituTextSetString(CloneChildRTSPSearchListDevNameText, "");
			ituTextSetString(CloneChildRTSPSearchListOnLineText, "");
		}
		else
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "RTSPSearchListContainer", i);
			result = ituWidgetClone(RTSPSearchListContainer[1], &CloneRTSPSearchListContainer);
			if (result)
			{
				ituWidgetSetName(CloneRTSPSearchListContainer, tmp);
				ituWidgetSetX(CloneRTSPSearchListContainer, 0);
				if (i < MAX_PAGE_NUM)
				{
					ypos = i * 62;
				}
				else
				{
					ypos = 0;
				}
				ituWidgetSetY(CloneRTSPSearchListContainer, ypos);

				memset(tmp, 0, sizeof(tmp));
				CloneChildRTSPSearchListButton = itcTreeGetChildAt(CloneRTSPSearchListContainer, 0);
				sprintf(tmp, "%s%d", "RTSPSearchListButton", i);
				ituWidgetSetName(CloneChildRTSPSearchListButton, tmp);

				memset(tmp, 0, sizeof(tmp));
				CloneChildRTSPSearchListDevNameText = itcTreeGetChildAt(CloneRTSPSearchListContainer, 1);
				sprintf(tmp, "%s%d", "RTSPSearchListDevNameText", i);
				ituWidgetSetName(CloneChildRTSPSearchListDevNameText, tmp);

				memset(tmp, 0, sizeof(tmp));
				CloneChildRTSPSearchListOnLineText = itcTreeGetChildAt(CloneRTSPSearchListContainer, 2);
				sprintf(tmp, "%s%d", "RTSPSearchListOnLineText", i);
				ituWidgetSetName(CloneChildRTSPSearchListOnLineText, tmp);

				memset(tmp, 0, sizeof(tmp));
				CloneChildRTSPSearchListSprite = itcTreeGetChildAt(CloneRTSPSearchListContainer, 3);
				sprintf(tmp, "%s%d", "RTSPSearchListSprite", i);
				ituWidgetSetName(CloneChildRTSPSearchListSprite, tmp);

				ituWidgetAdd(RTSPSearchListCoverFlow, CloneRTSPSearchListContainer);
			}
			ituCoverFlowUpdate((ITUWidget*)RTSPSearchListCoverFlow, ITU_EVENT_LAYOUT, 0, 0, 0);
		}
		g_RTSPSearchListButton[i] = CloneChildRTSPSearchListButton;

		if (i < max)
		{
			ituSpriteGoto(CloneChildRTSPSearchListSprite, icontype);
			ituTextSetString(CloneChildRTSPSearchListDevNameText, g_RTSPList->Devinfo[i].DeviceName);
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
					if (get_commdev_state(g_RTSPList->Devinfo[i].DeviceIP))
					{
						strid = SID_Set_Online;
					}
					else
					{
						strid = SID_Set_Offline;
					}
				}
				ituTextSetString(CloneChildRTSPSearchListOnLineText, get_str(strid));
			}
			else
			#endif
			{
				ituTextSetString(CloneChildRTSPSearchListOnLineText, "");
			}
		}
		else
		{
			ituWidgetDisable(CloneChildRTSPSearchListButton);
			ituWidgetSetVisible(CloneChildRTSPSearchListSprite, false);
			ituTextSetString(CloneChildRTSPSearchListDevNameText, "");
			ituTextSetString(CloneChildRTSPSearchListOnLineText, "");
		}
	}
}

/*************************************************
Function:		InitRTSPWin
Description: 	搜索RTSP页面初始化
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void InitRTSPWin(void)
{
	ituWidgetSetVisible(RTSPSearchMSGHitGrayBackground, false);
	if (0 == g_Type)
	{
		ituRadioBoxSetChecked(RTSPSearchRightPublicRadioBox, false);
		ituRadioBoxSetChecked(RTSPSearchRightHomeRadioBox, true);
		ituWidgetSetVisible(RTSPSearchBottomSearchContainer, false);
		ituWidgetSetVisible(RTSPSearchBottomNullContainer0, true);
		ituWidgetDisable(RTSPSearchBottomNullButton[0]);
		ShowRTSPWin(0);
		#if 0
		if (is_main_DeviceNo())
		{
			ShowRTSPWin(0);
		}
		else
		{
			g_FenJiLoadTick = 3;
			ituWidgetDisable(RTSPSearchBackground);
			ituWidgetSetVisible(RTSPSearchMSGHitGrayBackground, true);
			ShowRTSPNoneWin(MAX_PAGE_NUM);
		}
		#endif
	}
	else
	{
		ituRadioBoxSetChecked(RTSPSearchRightHomeRadioBox, false);
		ituRadioBoxSetChecked(RTSPSearchRightPublicRadioBox, true);
		ituWidgetSetVisible(RTSPSearchBottomSearchContainer, true);
		ituWidgetSetVisible(RTSPSearchBottomNullContainer0, false);
		ShowRTSPWin(1);
	}
	g_RTSPSearchLastTick = SDL_GetTicks();
}

/*************************************************
Function:		RTSPSearchLayerOnLeave
Description: 	销毁处理函数
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
bool RTSPSearchLayerOnLeave(ITUWidget* widget, char* param)
{
	rtsp_monitor_list_free();

	return true;
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
			if (4 == g_ShowLoadTick)
			{
				rtsp_monitor_sync_devlist();
			}
			else if (0 == g_ShowLoadTick)
			{
				// 超时自动停止搜索
				ituWidgetEnable(RTSPSearchBackground);
				ituWidgetSetVisible(RTSPSearchMSGHitGrayBackground, false);
			}
		}
		#if 0
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
				ShowRTSPWin(0);
			}
		}
		#endif
		return true;
	}

	return false;
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
		g_ShowLoadTick = 0;
		ituWidgetEnable(RTSPSearchBackground);
		ituWidgetSetVisible(RTSPSearchMSGHitGrayBackground, false);
		ShowRTSPWin(1);
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
	uint8 i;

	for (i = 0; i < g_RTSPList->DevNum; i++)
	{
		if (g_RTSPSearchListButton[i] == (ITUButton*)widget)
		{
			RTSPMonitorWin(g_RTSPList, i);
			break;
		}
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
			g_Type = 0;
			InitRTSPWin();
			break;

		case RTSPPublicEvent:
			g_Type = 1;
			InitRTSPWin();
			break;

		case RTSPSearchEvent:
			g_ShowLoadTick = 5;
			ituWidgetDisable(RTSPSearchBackground);
			ituWidgetSetVisible(RTSPSearchMSGHitGrayBackground, true);
			break;

		case RTSPExitEvent:
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

	g_FenJiLoadTick = 0;
	g_ShowLoadTick = 0;
	InitRTSPWin();
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

			RTSPSearchBottomSearchContainer = ituSceneFindWidget(&theScene, "RTSPSearchBottomSearchContainer");
			assert(RTSPSearchBottomSearchContainer);

			RTSPSearchBackground = ituSceneFindWidget(&theScene, "RTSPSearchBackground");
			assert(RTSPSearchBackground);

			RTSPSearchBottomNullContainer0 = ituSceneFindWidget(&theScene, "RTSPSearchBottomNullContainer0");
			assert(RTSPSearchBottomNullContainer0);

			for (i = 0; i < LIST_ITEM_COUNT; i++)
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
