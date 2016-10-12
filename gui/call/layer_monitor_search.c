/*************************************************
Copyright (C),  2006-2016, Aurine
File name:		layer_monitor_search.c
Author:     	caogw
Version:    	
Date: 			2016-07-20
Description:
History:
1. Date:
Author:
Modification:
*************************************************/
#include "gui_include.h"

/*****************常量定义***********************/
#define MAX_MONITOR_COUNT		AREA_DEVICE_NUM		// 最大监视列表数

/*****************变量定义***********************/
static ITULayer* MainLayer = NULL;
static ITULayer* MonitorSearchLayer = NULL;
static ITUBackground* MonitorSearchRightBackground = NULL;
static ITURadioBox* MonitorSearchRightStairRadioBox = NULL;
static ITUBackground* MonitorSearchBottomBackground = NULL;
static ITUCoverFlow* MonitorSearchListCoverFlow = NULL;
static ITUContainer* MonitorSearchListContainer[LIST_ITEM_COUNT] = { NULL };
static ITUButton* MonitorSearchListButton[LIST_ITEM_COUNT] = { NULL };
static ITUSprite* MonitorSearchListSprite[LIST_ITEM_COUNT] = { NULL };
static ITUText* MonitorSearchListDevTypeText[LIST_ITEM_COUNT] = { NULL };
static ITUBackground* MonitorSearchMSGHitGrayBackground = NULL;
static ITUButton* MonitorSearchRightNullButton0 = NULL;
static ITUButton* MonitorSearchBottomNullButton[3] = { NULL };

static ITUButton* g_MonitorSearchListButton[MAX_MONITOR_COUNT] = { NULL };
/*****************常量定义***********************/
static PMONITORLISTINFO  g_MonitorList = NULL;
static DEVICE_TYPE_E g_DevType = DEVICE_TYPE_NONE;
static uint32 g_MonitorSearchLastTick = 0;					// 实时更新的tick
static uint8 g_MonitorSearchCallBak = true;

typedef enum
{
	MonitorStairEvent = 0x00,
	MonitorDoorEvent,
	MonitorAreaEvent,
	MonitorSearchEvent,
	MonitorExitEvent,
	MonitorMaxEvent,
}MonitorButtonEvent;

// 监视图标类型
typedef enum
{
	MonitorStairIcon = 0x00,
	MonitorDoorIcon,
	MonitorAreaIcon,
	MonitorMaxIcon,
}MonitorIconType;

/*************************************************
Function:		ShowCenterWin
Description: 	显示监视列表
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void ShowMonitorWin(void)
{
	uint32 ypos = 0;
	uint8 i, max = 0, count = 0, result = 0;
	char devno[20], devtype[50], tmp[50];
	MonitorIconType icontype = MonitorStairIcon;
	ITUButton* CloneChildMonitorSearchListButton;
	ITUText* CloneChildMonitorSearchListDevTypeText;
	ITUSprite* CloneChildMonitorSearchListSprite;
	ITUContainer* CloneMonitorSearchListContainer;
	ITUContainer* OldMonitorSearchListContainer;

	if (g_MonitorList)
	{
		if (g_MonitorList->pMonitorInfo)
		{
			free(g_MonitorList->pMonitorInfo);
			g_MonitorList->pMonitorInfo = NULL;
		}
		free(g_MonitorList);
		g_MonitorList = NULL;
	}

	// 删除已经Clone的List
	for (i = LIST_ITEM_COUNT; i < MAX_MONITOR_COUNT; i++)
	{
		if (NULL != g_MonitorSearchListButton[i])
		{
			g_MonitorSearchListButton[i] = NULL;
			OldMonitorSearchListContainer = NULL;
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "MonitorSearchListContainer", i);
			OldMonitorSearchListContainer = (ITUContainer*)ituSceneFindWidget(&theScene, tmp);
			itcTreeRemove(OldMonitorSearchListContainer);						// 删除已有的
			ituWidgetExit(OldMonitorSearchListContainer);
			dprintf("Remove MonitorSearchListContainer List!!!\n");
		}
	}
	
	g_MonitorList = storage_get_monitorlist(g_DevType);
	if (NULL == g_MonitorList)
	{
		return;
	}
	// 默认显示从第一条开始显示
	ituCoverFlowGoto(MonitorSearchListCoverFlow, 0);

	max = g_MonitorList->MonitorCount;
	if (max > MAX_MONITOR_COUNT)
	{
		max = MAX_MONITOR_COUNT;
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
	if (g_DevType == DEVICE_TYPE_STAIR)
	{
		icontype = MonitorStairIcon;
	}
	else if (g_DevType == DEVICE_TYPE_DOOR_NET)
	{
		icontype = MonitorDoorIcon;
	}
	else if (g_DevType == DEVICE_TYPE_AREA)
	{
		icontype = MonitorAreaIcon;
	}

	for (i = 0; i < count; i++)
	{
		CloneChildMonitorSearchListButton = NULL;
		CloneChildMonitorSearchListDevTypeText = NULL;
		CloneChildMonitorSearchListSprite = NULL;
		CloneMonitorSearchListContainer = NULL;

		if (i < LIST_ITEM_COUNT)
		{
			CloneChildMonitorSearchListButton = MonitorSearchListButton[i];
			CloneChildMonitorSearchListDevTypeText = MonitorSearchListDevTypeText[i];
			CloneChildMonitorSearchListSprite = MonitorSearchListSprite[i];

			// 模板初始化
			ituWidgetEnable(CloneChildMonitorSearchListButton);
			ituWidgetSetVisible(CloneChildMonitorSearchListSprite, true);
			ituTextSetString(CloneChildMonitorSearchListDevTypeText, "");
		}
		else
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "MonitorSearchListContainer", i);
			result = ituWidgetClone(MonitorSearchListContainer[1], &CloneMonitorSearchListContainer);
			if (result)
			{
				ituWidgetSetName(CloneMonitorSearchListContainer, tmp);
				ituWidgetSetX(CloneMonitorSearchListContainer, 0);
				if (i < MAX_PAGE_NUM)
				{
					ypos = i * 62;
				}
				else
				{
					ypos = 0;
				}
				ituWidgetSetY(CloneMonitorSearchListContainer, ypos);

				memset(tmp, 0, sizeof(tmp));
				CloneChildMonitorSearchListButton = itcTreeGetChildAt(CloneMonitorSearchListContainer, 0);
				sprintf(tmp, "%s%d", "MonitorSearchListButton", i);
				ituWidgetSetName(CloneChildMonitorSearchListButton, tmp);

				memset(tmp, 0, sizeof(tmp));
				CloneChildMonitorSearchListDevTypeText = itcTreeGetChildAt(CloneMonitorSearchListContainer, 1);
				sprintf(tmp, "%s%d", "MonitorSearchListDevTypeText", i);
				ituWidgetSetName(CloneChildMonitorSearchListDevTypeText, tmp);

				memset(tmp, 0, sizeof(tmp));
				CloneChildMonitorSearchListSprite = itcTreeGetChildAt(CloneMonitorSearchListContainer, 2);
				sprintf(tmp, "%s%d", "MonitorSearchListSprite", i);
				ituWidgetSetName(CloneChildMonitorSearchListSprite, tmp);

				ituWidgetAdd(MonitorSearchListCoverFlow, CloneMonitorSearchListContainer);
			}
			ituCoverFlowUpdate((ITUWidget*)MonitorSearchListCoverFlow, ITU_EVENT_LAYOUT, 0, 0, 0);
		}

		g_MonitorSearchListButton[i] = CloneChildMonitorSearchListButton;
		if (i < max)
		{
			memset(devtype, 0, sizeof(devtype));
			fill_devno_by_index(g_MonitorList->pMonitorInfo[i].DeviceType, g_MonitorList->pMonitorInfo[i].index, devno);
			get_dev_description(g_MonitorList->pMonitorInfo[i].DeviceType, devno, devtype, 50);
			ituTextSetString(CloneChildMonitorSearchListDevTypeText, devtype);
			ituSpriteGoto(CloneChildMonitorSearchListSprite, icontype);
		}
		else
		{
			ituWidgetDisable(CloneChildMonitorSearchListButton);
			ituWidgetSetVisible(CloneChildMonitorSearchListSprite, false);
			ituTextSetString(CloneChildMonitorSearchListDevTypeText, "");
		}
	}
}

/*************************************************
Function:		MonitorSearchLayerOnLeave
Description: 	销毁处理函数
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
bool MonitorSearchLayerOnLeave(ITUWidget* widget, char* param)
{
	free_monitorlist_memory(&g_MonitorList);

	return true;
}

/*************************************************
Function:		MonitorSearchLayerOnTimer
Description: 	定时器
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool MonitorSearchLayerOnTimer(ITUWidget* widget, char* param)
{	
	uint32 tick = SDL_GetTicks();
	if (g_MonitorSearchCallBak == false)
	{
		if ((tick - g_MonitorSearchLastTick) >= 5000)
		{
			g_MonitorSearchLastTick = tick;
			g_MonitorSearchCallBak = true;
			
			// 搜索列表
			ShowMonitorWin();
			ituWidgetSetVisible(MonitorSearchMSGHitGrayBackground, false);
		}
	}
	return true;
}

/*************************************************
Function:		MonitorSearchListState
Description: 	获取监视列表执行函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool MonitorSearchListState(ITUWidget* widget, char* param)
{
	PINTER_CALLBACK pmonitorbak_data = (PINTER_CALLBACK)param;

	printf("pmonitorbak_data->InterState.......%d\n", pmonitorbak_data->InterState);
	if (pmonitorbak_data->InterState == MONITOR_GETLIST)
	{
		g_MonitorSearchCallBak = true;
		ShowMonitorWin();
		ituWidgetSetVisible(MonitorSearchMSGHitGrayBackground, false);
	}

	return true;
}

/*************************************************
Function:		MonitorSearchListButtonOnMouseUp
Description: 	监视列表按下执行函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool MonitorSearchListButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 i;

	for (i = 0; i < g_MonitorList->MonitorCount; i++)
	{
		if (g_MonitorSearchListButton[i] == (ITUButton*)widget)
		{
			g_DevType = g_MonitorList->pMonitorInfo[i].DeviceType;
			MonitorWin(g_DevType, i, g_MonitorList->MonitorCount);		// 点击直接开始监视
			break;
		}
	}

	return true;
}

/*************************************************
Function:		MonitorSearchMSGButtonOnMouseUp
Description: 	消息框按键按下执行函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool MonitorSearchMSGButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 index = atoi(param);
	return true;
}

/*************************************************
Function:		MonitorSearchLayerButtonOnMouseUp
Description: 	监视搜索按键弹起函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool MonitorSearchLayerButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 btn_event = atoi(param);

	switch (btn_event)
	{
		case MonitorStairEvent:
			g_DevType = DEVICE_TYPE_STAIR;
			ShowMonitorWin();
			break;

		case MonitorDoorEvent:
			g_DevType = DEVICE_TYPE_DOOR_NET;
			ShowMonitorWin();
			break;

		case MonitorAreaEvent:
			g_DevType = DEVICE_TYPE_AREA;
			ShowMonitorWin();
			break;

		case MonitorSearchEvent:
			g_MonitorSearchLastTick = SDL_GetTicks();
			g_MonitorSearchCallBak = false;
			storage_clear_monitorlist(g_DevType);
			monitorlist_sync_devlist(g_DevType);
			ituWidgetSetVisible(MonitorSearchMSGHitGrayBackground, true);
			break;

		case MonitorExitEvent:
			ituLayerGoto(MainLayer);
			break;

		default:
			break;
	}
	return true;
}

/*************************************************
Function:		MonitorSearchLayerOnEnter
Description: 	监视搜索初始化处理函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool MonitorSearchLayerOnEnter(ITUWidget* widget, char* param)
{
	uint8 i;

	if (!MonitorSearchLayer)
	{
		char callname[50];

		MainLayer = ituSceneFindWidget(&theScene, "mainLayer");
		assert(MainLayer);

		MonitorSearchLayer = ituSceneFindWidget(&theScene, "MonitorSearchLayer");
		assert(MonitorSearchLayer);

		MonitorSearchRightBackground = ituSceneFindWidget(&theScene, "MonitorSearchRightBackground");
		assert(MonitorSearchRightBackground);

		MonitorSearchRightStairRadioBox = ituSceneFindWidget(&theScene, "MonitorSearchRightStairRadioBox");
		assert(MonitorSearchRightStairRadioBox);
		
		MonitorSearchBottomBackground = ituSceneFindWidget(&theScene, "MonitorSearchBottomBackground");
		assert(MonitorSearchBottomBackground);
		
		MonitorSearchListCoverFlow = ituSceneFindWidget(&theScene, "MonitorSearchListCoverFlow");
		assert(MonitorSearchListCoverFlow);

		MonitorSearchMSGHitGrayBackground = ituSceneFindWidget(&theScene, "MonitorSearchMSGHitGrayBackground");
		assert(MonitorSearchMSGHitGrayBackground);

		MonitorSearchRightNullButton0 = ituSceneFindWidget(&theScene, "MonitorSearchRightNullButton0");
		assert(MonitorSearchRightNullButton0);

		for (i = 0; i < LIST_ITEM_COUNT; i++)
		{
			memset(callname, 0, sizeof(callname));
			sprintf(callname, "%s%d", "MonitorSearchListContainer", i);
			MonitorSearchListContainer[i] = ituSceneFindWidget(&theScene, callname);
			assert(MonitorSearchListContainer[i]);

			memset(callname, 0, sizeof(callname));
			sprintf(callname, "%s%d", "MonitorSearchListButton", i);
			MonitorSearchListButton[i] = ituSceneFindWidget(&theScene, callname);
			assert(MonitorSearchListButton[i]);
		
			memset(callname, 0, sizeof(callname));
			sprintf(callname, "%s%d", "MonitorSearchListDevTypeText", i);
			MonitorSearchListDevTypeText[i] = ituSceneFindWidget(&theScene, callname);
			assert(MonitorSearchListDevTypeText[i]);
			 
			memset(callname, 0, sizeof(callname));
			sprintf(callname, "%s%d", "MonitorSearchListSprite", i);
			MonitorSearchListSprite[i] = ituSceneFindWidget(&theScene, callname);
			assert(MonitorSearchListSprite[i]);
		}

		for (i = 0; i < 3; i++)
		{
			memset(callname, 0, sizeof(callname));
			sprintf(callname, "%s%d", "MonitorSearchBottomNullButton", i);
			MonitorSearchBottomNullButton[i] = ituSceneFindWidget(&theScene, callname);
			assert(MonitorSearchBottomNullButton[i]);
		}
	}
	g_DevType = DEVICE_TYPE_STAIR;
	g_MonitorSearchCallBak = true;
	ShowMonitorWin();
	ituRadioBoxSetChecked(MonitorSearchRightStairRadioBox, true);
	ituWidgetSetVisible(MonitorSearchMSGHitGrayBackground, false);
	ituWidgetDisable(MonitorSearchRightNullButton0);
	for (i = 0; i < 3; i++)
	{
		ituWidgetDisable(MonitorSearchBottomNullButton[i]);
	}
	
	return true;
}

/*************************************************
Function:		MonitorSearchReset
Description: 	监视搜索界面初始化函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
void MonitorSearchReset(void)
{
	MonitorSearchLayer = NULL;
}

