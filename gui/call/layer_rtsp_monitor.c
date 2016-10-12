/*************************************************
Copyright (C),  2006-2016, Aurine
File name:		layer_rtsp_monitor.c
Author:     	caogw
Version:    	
Date: 			2016-08-25
Description:
History:
1. Date:
Author:
Modification:
*************************************************/
#include "gui_include.h"

/*****************常量定义***********************/
#define MAX_PRESET_NUM			10					// 最大预置点个数

/*****************变量定义***********************/
static ITULayer* MainLayer = NULL;
static ITULayer* RTSPMonitorLayer = NULL;
static ITUBackground* RTSPMonitorBackground = NULL;
static ITUBackground* RTSPMonitorMSGHitBackground = NULL;
static ITUBackground* RTSPMonitorMSGErrBackground = NULL;
static ITUBackground* RTSPMonitorMSGHitGrayBackground = NULL;
static ITUText* RTSPMonitorTimeText = NULL;
static ITUButton* RTSPMonitorShowBottomButton = NULL;
static ITUBackgroundButton* RTSPMonitorBottomBackgroundButton = NULL;
static ITURadioBox* RTSPMonitorBottomRadioBox[MAX_PRESET_NUM] = { NULL };

/*****************常量定义***********************/
static MONITOR_STATE_E g_InterState = MONITOR_END;
static int16 g_RemainTime = 0;
static uint32 g_MonitorLastTick = 0;				// 实时更新的tick
static uint32 g_ErrHintTxtID = 0;					// 错误提示文字ID
static uint8 g_ErrHintTicks = 0;					// 错误提示计时
static uint8 g_StartMonitorTick = 0;				// 开始监视tick
static PRtspDeviceList g_RtspList = NULL;
static uint8 g_Index = 0;
static uint8 g_BtnEvent = 0;						// 预置点按键事件

typedef enum
{
	RTSPMonitorExitEvent = 0x00,
	RTSPMonitorPresetEvent0,
	RTSPMonitorPresetEvent1,
	RTSPMonitorPresetEvent2,
	RTSPMonitorPresetEvent3,
	RTSPMonitorPresetEvent4,
	RTSPMonitorPresetEvent5,
	RTSPMonitorPresetEvent6,
	RTSPMonitorPresetEvent7,
	RTSPMonitorPresetEvent8,
	RTSPMonitorPresetEvent9,
	RTSPMonitorMaxEvent,
}RTSPMonitorButtonEvent;

/*************************************************
Function:		DrawStringHint
Description: 	画状态提示
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void DrawStringHint(void)
{
	// 画时间提示: time
	if (g_RemainTime)
	{
		char Time[10] = { 0 };
		sprintf(Time, "%d", g_RemainTime);
		ituTextSetString(RTSPMonitorTimeText, Time);
	}
}

/*************************************************
Function:		RTSPMonitorSearchFail
Description: 	监视失败
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void RTSPMonitorSearchFail(void)
{
	g_ErrHintTxtID = SID_Inter_SearchFailed;
	g_InterState = MONITOR_END;
	g_ErrHintTicks = 3;
}

/*************************************************
Function:		RTSPMonitorLayerOnLeave
Description: 	销毁处理函数
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
bool RTSPMonitorLayerOnLeave(ITUWidget* widget, char* param)
{
	return true;
}

/*************************************************
Function:		RTSPMonitorLayerOnTimer
Description: 	定时器
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool RTSPMonitorLayerOnTimer(ITUWidget* widget, char* param)
{
	uint32 tick = SDL_GetTicks();
	uint32 diff = tick - g_MonitorLastTick;
	if (diff >= 1000)
	{
		g_MonitorLastTick = tick;

		if (g_ErrHintTicks)
		{
			g_ErrHintTicks--;
			if (2 == g_ErrHintTicks)
			{
				ituWidgetSetVisible(RTSPMonitorMSGErrBackground, true);
				ituWidgetSetVisible(RTSPMonitorMSGHitBackground, false);
				ituWidgetSetVisible(RTSPMonitorMSGHitGrayBackground, true);
			}
			else if (0 == g_ErrHintTicks)
			{
				if (g_InterState == MONITOR_END)
				{
					ituLayerGoto(MainLayer);
				}
				g_ErrHintTxtID = 0;
			}
		}

		// 开始监视
		if (g_StartMonitorTick)
		{
			g_StartMonitorTick--;
			if (0 == g_StartMonitorTick)
			{
				if (false == rtsp_monitor_start(g_Index, DEVICE_TYPE_IPCAMERA))
				{
					RTSPMonitorSearchFail();
				}
			}
		}
	}

	return true;
}

/*************************************************
Function:		RTSPMonitorState
Description: 	监视回调执行函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool RTSPMonitorState(ITUWidget* widget, char* param)
{
	uint32 temp = 0;
	PINTER_CALLBACK pmonitorbak_data = (PINTER_CALLBACK)param;
	
	if (NULL == pmonitorbak_data)
	{
		return false;
	}
	
	if (MONITOR_TEMP == pmonitorbak_data->InterState)
	{
		pmonitorbak_data->InterState = MONITOR_END;
	}

	switch (pmonitorbak_data->InterState)
	{
		case MONITOR_SEARCH:
			g_InterState = MONITOR_SEARCH;
			break;

		case MONITOR_REQUEST:
			break;

		case MONITOR_MONITORING:
			g_InterState = MONITOR_MONITORING;
			ituWidgetSetVisible(RTSPMonitorMSGHitGrayBackground, false);
			break;

		case MONITOR_END:
			g_InterState = MONITOR_END;
			dprintf("atoi(pmonitorbak_data->Buf)......: %d\n", atoi(pmonitorbak_data->Buf));
			switch (atoi(pmonitorbak_data->Buf))
			{
				case MONITOR_SEARCH_ERR:			// 搜索设备失败
					g_ErrHintTxtID = SID_Inter_SearchFailed;
					g_ErrHintTicks = 3;				// 显示提示的时间
					break;

				case MONITOR_REQUEST_TIMEOUT:		// 连接失败
					g_ErrHintTxtID = SID_Inter_ConnFailed;
					g_ErrHintTicks = 3;				// 显示提示的时间
					break;

				default:
					ituLayerGoto(MainLayer);
					return true;
			}
			break;

		case MONITOR_TIMER:
			temp = atoi(pmonitorbak_data->Buf);
			g_RemainTime = temp;
			DrawStringHint();
			break;

		default:
			break;
	}

	return true;
}

/*************************************************
Function:		RTSPMonitorLayerShowBottomButtonOnMouseUp
Description: 	显示预置点函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool RTSPMonitorLayerShowBottomButtonOnMouseUp(ITUWidget* widget, char* param)
{
	if (true == ituWidgetIsVisible(RTSPMonitorBottomBackgroundButton))
	{
		ituWidgetSetVisible(RTSPMonitorBottomBackgroundButton, false);
	}
	else
	{
		ituWidgetSetVisible(RTSPMonitorBottomBackgroundButton, true);
	}

	return true;
}

/*************************************************
Function:		RTSPMonitorLayerButtonOnMouseUp
Description: 	右边按键弹起函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool RTSPMonitorLayerButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 btn_event = atoi(param);

	dprintf("btn_event: %d\n", btn_event);
	switch (btn_event)
	{
		case RTSPMonitorExitEvent:
			if (MONITOR_END == g_InterState)
			{
				ituLayerGoto(MainLayer);
			}
			else
			{
				rtsp_monitor_stop();
			}
			break;

		case  RTSPMonitorPresetEvent0:
		case  RTSPMonitorPresetEvent1:
		case  RTSPMonitorPresetEvent2:
		case  RTSPMonitorPresetEvent3:
		case  RTSPMonitorPresetEvent4:
		case  RTSPMonitorPresetEvent5:
		case  RTSPMonitorPresetEvent6:
		case  RTSPMonitorPresetEvent7:
		case  RTSPMonitorPresetEvent8:
		case  RTSPMonitorPresetEvent9:
			if (g_BtnEvent)
			{
				if (g_BtnEvent != btn_event)
				{
					ituRadioBoxSetChecked(RTSPMonitorBottomRadioBox[g_BtnEvent - 1], false);
				}
			}
			g_BtnEvent = btn_event;
			preset_control_ptz(btn_event);
			break;

		default:
			break;
	}
	return true;
}

/*************************************************
Function:		RTSPMonitorLayerOnEnter
Description:	监视初始化处理函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool RTSPMonitorLayerOnEnter(ITUWidget* widget, char* param)
{
	g_MonitorLastTick = SDL_GetTicks();
	g_StartMonitorTick = 1;
	ituTextSetString(RTSPMonitorTimeText, NULL);
	ituWidgetSetVisible(RTSPMonitorMSGErrBackground, false);
	ituWidgetSetVisible(RTSPMonitorMSGHitBackground, true);
	ituWidgetSetVisible(RTSPMonitorMSGHitGrayBackground, true);
	//if (g_RtspList->Devinfo[g_Index].CanControlPTZ)
	if (0)
	{
		ituWidgetSetVisible(RTSPMonitorShowBottomButton, true);
	}
	else
	{
		ituWidgetSetVisible(RTSPMonitorShowBottomButton, false);
	}
	ituWidgetSetVisible(RTSPMonitorBottomBackgroundButton, false);
	
	return true;
}

/*************************************************
Function:		InitMonitorLayer
Description: 	监视页面初始化
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void InitMonitorLayer(void)
{
	if (!RTSPMonitorLayer)
	{
		uint8 i;
		char callname[50];

		MainLayer = ituSceneFindWidget(&theScene, "mainLayer");
		assert(MainLayer);

		RTSPMonitorLayer = ituSceneFindWidget(&theScene, "RTSPMonitorLayer");
		assert(RTSPMonitorLayer);

		RTSPMonitorBackground = ituSceneFindWidget(&theScene, "RTSPMonitorBackground");
		assert(RTSPMonitorBackground);

		RTSPMonitorMSGHitBackground = ituSceneFindWidget(&theScene, "RTSPMonitorMSGHitBackground");
		assert(RTSPMonitorMSGHitBackground);

		RTSPMonitorMSGErrBackground = ituSceneFindWidget(&theScene, "RTSPMonitorMSGErrBackground");
		assert(RTSPMonitorMSGErrBackground);

		RTSPMonitorMSGHitGrayBackground = ituSceneFindWidget(&theScene, "RTSPMonitorMSGHitGrayBackground");
		assert(RTSPMonitorMSGHitGrayBackground);

		RTSPMonitorTimeText = ituSceneFindWidget(&theScene, "RTSPMonitorTimeText");
		assert(RTSPMonitorTimeText);

		RTSPMonitorShowBottomButton = ituSceneFindWidget(&theScene, "RTSPMonitorShowBottomButton");
		assert(RTSPMonitorShowBottomButton);

		RTSPMonitorBottomBackgroundButton = ituSceneFindWidget(&theScene, "RTSPMonitorBottomBackgroundButton");
		assert(RTSPMonitorBottomBackgroundButton);

		for (i = 0; i < MAX_PRESET_NUM; i++)
		{
			memset(callname, 0, sizeof(callname));
			sprintf(callname, "%s%d", "RTSPMonitorBottomRadioBox", i);
			RTSPMonitorBottomRadioBox[i] = ituSceneFindWidget(&theScene, callname);
			assert(RTSPMonitorBottomRadioBox[i]);
		}
	}
}

/*************************************************
Function:		RTSPMonitorWin
Description: 	监视界面
Input:
1.DevType		设备类型
2.index			索引
utput:			无
Return:			无
Others:			无
*************************************************/
void RTSPMonitorWin(PRtspDeviceList list, uint8 index)
{
	g_RtspList = list;
	if (NULL == g_RtspList)
	{
		dprintf("g_RtspList is NULL\n");
		return;
	}
	g_Index = index;
	g_RemainTime = 0;			
	g_ErrHintTxtID = 0;	
	g_ErrHintTicks = 0;
	g_BtnEvent = 0;
	g_InterState = MONITOR_SEARCH;
	InitMonitorLayer();
	ituLayerGoto(RTSPMonitorLayer);
}

/*************************************************
Function:		RTSPMonitorReset
Description: 	监视界面初始化函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
void RTSPMonitorReset(void)
{
	RTSPMonitorLayer = NULL;
}

