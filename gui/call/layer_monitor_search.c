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
#define MAX_PAGE_NUM					6					// 每页最大条数
#define MAX_MONITOR_NUM  				8					// 最大管理员机数
#define MAX_MONITOR_PAGE_NUM  			2					// 管理员机最大页数

/*****************变量定义***********************/
static ITULayer* MainLayer = NULL;
static ITULayer* MonitorSearchLayer = NULL;
static ITUBackground* MonitorSearchRightBackground = NULL;
static ITURadioBox* MonitorSearchRightStairRadioBox = NULL;
static ITUBackground* MonitorSearchBottomBackground = NULL;
static ITUCoverFlow* MonitorSearchListCoverFlow = NULL;
static ITUBackground* MonitorSearchListBackgroundPage[MAX_MONITOR_PAGE_NUM] = { NULL };
static ITUContainer* MonitorSearchListContainer[MAX_MONITOR_NUM] = { NULL };
static ITUSprite* MonitorSearchListSprite[MAX_MONITOR_NUM] = { NULL };
static ITUText* MonitorSearchListDevTypeText[MAX_MONITOR_NUM] = { NULL };
static ITUBackground* MonitorSearchMSGHitGrayBackground = NULL;
static ITUAnimation* MonitorSearchMSGHitAnimation = NULL;

/*****************常量定义***********************/
static PMONITORLISTINFO  g_MonitorList = NULL;
static DEVICE_TYPE_E g_DevType = DEVICE_TYPE_NONE;
static uint32 g_MonitorSearchLastTick = 0;					// 实时更新的tick
static uint8 g_ShowLoadTick = 0;

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
Function:		SetMonitorShowInit
Description: 	显示页面初始化
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void SetMonitorShowInit(void)
{
	uint8 i;

	for (i = 0; i < MAX_MONITOR_PAGE_NUM; i++)
	{
		ituWidgetSetVisible(MonitorSearchListBackgroundPage[i], true);
	}

	for (i = 0; i < MAX_MONITOR_NUM; i++)
	{
		ituWidgetSetVisible(MonitorSearchListContainer[i], true);
		ituWidgetSetVisible(MonitorSearchListSprite[i], true);
	}

	// 初始化默认为首页
	// 解决MonitorSearchListCoverFlow第一页是-1的情况
#if 1
	if (MonitorSearchListCoverFlow->frame == MonitorSearchListCoverFlow->totalframe)
	{
		ituCoverFlowGoto(MonitorSearchListCoverFlow, -1);
	}
	else
	{
		ituCoverFlowGoto(MonitorSearchListCoverFlow, 0);
	}
#endif
}

/*************************************************
Function:		SetMonitorShowNum
Description: 	显示监视列表具体条数
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void SetMonitorShowNum(uint8 max)
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

	for (i = MAX_MONITOR_PAGE_NUM; i > pagenum; i--)
	{
		ituWidgetSetVisible(MonitorSearchListBackgroundPage[i - 1], false);
	}

	// 隐藏多余行(目前分机只支持8条)
	count = pagenum * MAX_PAGE_NUM;
	if (count > MAX_MONITOR_NUM)
	{
		count = MAX_MONITOR_NUM;
	}

	for (i = count; i > max; i--)
	{
		ituWidgetSetVisible(MonitorSearchListContainer[i - 1], false);
	}
	printf("SetMonitorShowNum..:count %d\n", count);
	printf("SetMonitorShowNum..:max %d\n", max);
}

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
	uint8 i, max = 0;
	char devno[20], devtype[50];
	MonitorIconType icontype = MonitorStairIcon;

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
	SetMonitorShowInit();
	g_MonitorList = storage_get_monitorlist(g_DevType);
	if (NULL == g_MonitorList)
	{
		return;
	}

	if (g_MonitorList && g_MonitorList->MonitorCount > 0)
	{
		max = g_MonitorList->MonitorCount;
		if (max > MAX_MONITOR_NUM)
		{
			max = MAX_MONITOR_NUM;
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

		for (i = 0; i < max; i++)
		{
			memset(devtype, 0, sizeof(devtype));
			fill_devno_by_index(g_MonitorList->pMonitorInfo[i].DeviceType, g_MonitorList->pMonitorInfo[i].index, devno);
			get_dev_description(g_MonitorList->pMonitorInfo[i].DeviceType, devno, devtype, 50);
			ituTextSetString(MonitorSearchListDevTypeText[i], devtype);
			ituSpriteGoto(MonitorSearchListSprite[i], icontype);
		}
		SetMonitorShowNum(max);
	}
	else
	{
		SetMonitorShowNum(0);
	}
}

/*************************************************
Function:		MonitorSearchDestroyProc
Description: 	销毁处理函数
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void MonitorSearchDestroyProc(void)
{
	free_monitorlist_memory(&g_MonitorList);
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
	uint32 diff = tick - g_MonitorSearchLastTick;
	if (diff >= 1000)
	{
		g_MonitorSearchLastTick = tick;
		// 搜索列表
		if (g_ShowLoadTick)
		{
			g_ShowLoadTick--;
			if (0 == g_ShowLoadTick)
			{
				storage_clear_monitorlist(g_DevType);
				monitorlist_sync_devlist(g_DevType);
			}
		}
	}

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
		//ituAnimationStop(MonitorSearchMSGHitAnimation);
		ShowMonitorWin();
		ituWidgetSetVisible(MonitorSearchMSGHitGrayBackground, false);
	}
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
	uint8 index = atoi(param);

	if (g_MonitorList && g_MonitorList->MonitorCount > 0)
	{
		g_DevType = g_MonitorList->pMonitorInfo[index].DeviceType;
		printf("00focusIndex : %d\n", MonitorSearchListCoverFlow->focusIndex);
		MonitorWin(g_DevType, index);		// 点击直接开始监视
		printf("11focusIndex : %d\n", MonitorSearchListCoverFlow->focusIndex);
	}
	printf("MonitorSearchList g_DevType......:%x\n", g_DevType);
	printf("MonitorSearchList index......:%d\n", index);
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
			//ituAnimationPlay(MonitorSearchMSGHitAnimation);
			g_ShowLoadTick = 1;
			ituWidgetSetVisible(MonitorSearchMSGHitGrayBackground, true);
			break;

		case MonitorExitEvent:
			MonitorSearchDestroyProc();
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
	if (!MonitorSearchLayer)
	{
		uint8 i;
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

		MonitorSearchMSGHitAnimation = ituSceneFindWidget(&theScene, "MonitorSearchMSGHitAnimation");
		assert(MonitorSearchMSGHitAnimation);

		for (i = 0; i < MAX_MONITOR_PAGE_NUM; i++)
		{
			memset(callname, 0, sizeof(callname));
			sprintf(callname, "%s%d", "MonitorSearchListBackgroundPage", i);
			MonitorSearchListBackgroundPage[i] = ituSceneFindWidget(&theScene, callname);
			assert(MonitorSearchListBackgroundPage[i]);
		}
		
		for (i = 0; i < MAX_MONITOR_NUM; i++)
		{
			memset(callname, 0, sizeof(callname));
			sprintf(callname, "%s%d", "MonitorSearchListContainer", i);
			MonitorSearchListContainer[i] = ituSceneFindWidget(&theScene, callname);
			assert(MonitorSearchListContainer[i]);

			memset(callname, 0, sizeof(callname));
			sprintf(callname, "%s%d", "MonitorSearchListDevTypeText", i);
			MonitorSearchListDevTypeText[i] = ituSceneFindWidget(&theScene, callname);
			assert(MonitorSearchListDevTypeText[i]);
			 
			memset(callname, 0, sizeof(callname));
			sprintf(callname, "%s%d", "MonitorSearchListSprite", i);
			MonitorSearchListSprite[i] = ituSceneFindWidget(&theScene, callname);
			assert(MonitorSearchListSprite[i]);
		}
	}
	g_ShowLoadTick = 0;
	g_DevType = DEVICE_TYPE_STAIR;
	ShowMonitorWin();
	ituRadioBoxSetChecked(MonitorSearchRightStairRadioBox, true);
	ituWidgetSetVisible(MonitorSearchMSGHitGrayBackground, false);
	g_MonitorSearchLastTick = SDL_GetTicks();

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

