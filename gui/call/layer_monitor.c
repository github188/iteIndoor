﻿/*************************************************
Copyright (C),  2006-2016, Aurine
File name:		layer_Monitor.c
Author:     	caogw
Version:    	
Date: 			2016-08-15
Description:
History:
1. Date:
Author:
Modification:
*************************************************/
#include "gui_include.h"

/*****************常量定义***********************/

/*****************变量定义***********************/
static ITULayer* MainLayer = NULL;
static ITULayer* MonitorLayer = NULL;
static ITUContainer* MonitorRightAnswerContainer = NULL;
static ITUContainer* MonitorRightHandUpContainer = NULL;
static ITUContainer* MonitorRightLockContainer = NULL;
static ITUContainer* MonitorRightSnapContainer = NULL;
static ITUBackground* MonitorBottomBackground = NULL;
static ITUBackground* MonitorHitBackground = NULL;
static ITUText* MonitorTimeText = NULL;
static ITUText* MonitorCallNoText = NULL;
static ITUSprite* MonitorHitSprite = NULL;
static ITUSprite* MonitorSoundSprite = NULL;
static ITUContainer* MonitorShowButtomContainer = NULL;

/*****************常量定义***********************/
static DEVICE_TYPE_E g_DevType;
static int32 g_index = -1;
static char g_DevNo[50];
static MONITOR_STATE_E g_InterState = MONITOR_END;
static int16 g_RemainTime = 0;
static uint8 g_SetVolume = FALSE;					// 是否处于设置音量状态
static uint8 g_mute = FALSE;						// 是否静音状态
static uint8 g_volume = 4;
static uint8 g_volumeticks = 0;
static uint8 g_MSGTicks = 0;
static uint32 g_MonitorLastTick = 0;					// 实时更新的tick
static uint32 g_ErrHintTxtID = 0;					// 错误提示文字ID
static uint8 g_ErrHintTicks = 0;					// 错误提示计时
static uint8 g_StartMonitorTick = 0;				// 开始监视tick

typedef enum
{
	MonitorAnswerEvent = 0x00,
	MonitorHandUpEvent,
	MonitorLockEvent,
	MonitorSnapEvent,
	MonitorExitEvent,
	MonitorMaxEvent,
}MonitorButtonEvent;

// 被叫提示图标类型
typedef enum
{
	MonitorLockMSGIcon = 0x00,
	MonitorSnapMSGIcon,
	MonitorMaxMSGIcon,
}MonitorMSGIconType;

// 是否静音图标类型
typedef enum
{
	MonitorNoMuteIcon = 0x00,
	MonitorMuteIcon,
	MonitorMaxIcon,
}MonitorSoundIcon;

/*************************************************
Function:		InitRightButton
Description: 	初始化右边按键
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void InitRightButton(void)
{
	ituWidgetSetVisible(MonitorRightAnswerContainer, true);
	ituWidgetSetVisible(MonitorRightHandUpContainer, false);
	ituWidgetSetVisible(MonitorRightLockContainer, true);
	ituWidgetSetVisible(MonitorRightSnapContainer, true);
}

/*************************************************
Function:		SetMonitorInfo
Description: 	设置监视信息
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void SetMonitorInfo(void)
{
	memset(g_DevNo, 0, sizeof(g_DevNo));
	g_InterState = MONITOR_REQUEST;
	g_RemainTime = 0;
	g_SetVolume = FALSE;
	g_mute = FALSE;
	g_volume = storage_get_ringvolume();
	g_volumeticks = 0;
	g_MSGTicks = 0;
	g_ErrHintTxtID = 0;
	g_ErrHintTicks = 0;
}

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
	char time[10] = { 0 };
	char temp[50] = { 0 };
	char DevDesc[50] = { 0 };
	char NoText[100] = { 0 };
	uint32 StrId = 0;

	switch (g_InterState)
	{
		case MONITOR_SEARCH:
			StrId = SID_Inter_Search;
			break;

		case MONITOR_REQUEST:
			StrId = SID_Inter_Connecting;
			break;

		case MONITOR_MONITORING:
			StrId = SID_Inter_Monitor;
			break;

		case MONITOR_TALKING:
			StrId = SID_Inter_Talking;
			break;

		case MONITOR_END:
			StrId = 0;
			break;

		default:
			break;
	}
	// 画设备描述
	if (strlen(g_DevNo) > 0)
	{
		get_dev_description(g_DevType, g_DevNo, DevDesc, 50); // 获得设备描述
	}

	if (0 == strlen(DevDesc))
	{
		if (g_ErrHintTxtID)
		{
			ituTextSetString(MonitorCallNoText, get_str(g_ErrHintTxtID));
		}
		else if (StrId)
		{
			ituTextSetString(MonitorCallNoText, get_str(StrId));
		}
	}
	else
	{
		sprintf(temp, "[%s]", DevDesc);
		if (g_ErrHintTxtID)
		{
			sprintf(NoText, "%s %s", temp, get_str(g_ErrHintTxtID));
			ituTextSetString(MonitorCallNoText, NoText);
		}
		else if (StrId)
		{
			sprintf(NoText, "%s %s", temp, get_str(StrId));
			ituTextSetString(MonitorCallNoText, NoText);
		}
		else
		{
			ituTextSetString(MonitorCallNoText, temp);
		}
	}

	// 画时间提示: time
	if (g_RemainTime)
	{
		sprintf(time, "%d", g_RemainTime);
		ituTextSetString(MonitorTimeText, time);
	}
}

/*************************************************
Function:		MonitorAnswerCall
Description: 	接听时改变右键
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void MonitorAnswerCall(void)
{
	// 接听
	uint32 ret = monitor_talk();
	if (ret == FALSE)
	{
		return;
	}
	ituWidgetSetVisible(MonitorRightHandUpContainer, true);
	ituWidgetSetVisible(MonitorRightAnswerContainer, false);
}

/*************************************************
Function:		ChangeVolume
Description: 	调节音量
Input:			无
Output:			无
Return:			无
Others:
*************************************************/
static void ChangeVolume(void)
{
	g_volumeticks = 4;
	// 音量为0时，自动为静音
	if (g_volume == 0)
	{
		g_mute = TRUE;
	}

	if (g_mute == TRUE)
	{
		// 设置静音音量
		media_set_talk_volume(g_DevType, 0);
	}
	else
	{
		// 设置volume音量
		media_set_talk_volume(g_DevType, g_volume);
	}
}

/*************************************************
Function:		SetMonitorLockAndSnap
Description: 	设置监视开锁或抓拍
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void SetMonitorLockAndSnap(MonitorButtonEvent event)
{
	int32 ret = FALSE;
	if (event == MonitorLockEvent)
	{
		if (g_InterState == MONITOR_MONITORING || g_InterState == MONITOR_TALKING)
		{
			ret = monitor_unlock();
			if (ret == TRUE)
			{
				dprintf("monitor lock ok!\n");
			}
			else
			{
				// 提示开锁失败
				dprintf("monitor lock fail!\n");
			}
		}
	}
	else
	{
		//ret = monitor_video_snap();
		if (ret == TRUE)
		{
			// 抓拍成功提示
			dprintf("monitor snap ok!\n");
		}
		else
		{
			// 抓拍失败提示
			dprintf("monitor snap fail!\n");
		}
	}
	ituWidgetSetVisible(MonitorHitBackground, true);
	ituSpriteGoto(MonitorHitSprite, (event - MonitorLockEvent));
	g_MSGTicks = 3;
}

/*************************************************
Function:		MonitorDestroyProc
Description: 	销毁处理函数
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void MonitorDestroyProc(void)
{
}

/*************************************************
Function:		monitor_search_fail
Description: 	监视失败
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void monitor_search_fail(void)
{
	g_ErrHintTxtID = SID_Inter_SearchFailed;
	g_InterState = MONITOR_END;
	g_ErrHintTicks = 3;
	DrawStringHint();
}

/*************************************************
Function:		MonitorLayerOnTimer
Description: 	定时器
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool MonitorLayerOnTimer(ITUWidget* widget, char* param)
{
	uint32 tick = SDL_GetTicks();
	uint32 diff = tick - g_MonitorLastTick;
	if (diff >= 1000)
	{
		g_MonitorLastTick = tick;

		if (g_SetVolume == TRUE)
		{
			g_volumeticks--;
			if (0 == g_volumeticks)
			{
				g_SetVolume = FALSE;
				// 保存铃声或通话音量
				storage_set_volume(storage_get_ringvolume(), g_volume, storage_get_keykeep());
				ituWidgetSetVisible(MonitorBottomBackground, false);
			}
		}
		
		if (g_MSGTicks)
		{
			g_MSGTicks--;
			if (0 == g_MSGTicks)
			{
				ituWidgetSetVisible(MonitorHitBackground, false);
			}
		}

		if (g_ErrHintTicks)
		{
			g_ErrHintTicks--;
			if (0 == g_ErrHintTicks)
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
				if (monitor_start(g_DevType, g_index) == FALSE)
				{
					monitor_search_fail();
				}
			}
		}
	}

	return true;
}

/*************************************************
Function:		MonitorState
Description: 	监视回调执行函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool MonitorState(ITUWidget* widget, char* param)
{
	uint32 temp = 0;
	PINTER_CALLBACK pmonitorbak_data = (PINTER_CALLBACK)param;

	dprintf("pmonitorbak_data->InterState......:%d\n", pmonitorbak_data->InterState);
	switch (pmonitorbak_data->InterState)
	{
		case MONITOR_SEARCH:
			g_InterState = MONITOR_SEARCH;
			break;

		case MONITOR_REQUEST:
			{
				char DevStr[5] = { 0 };
				int32 index = atoi(pmonitorbak_data->Buf);

				// 获取设备号
				memset(g_DevNo, 0, sizeof(g_DevNo));
				sprintf(DevStr, "%d", index);
				switch (g_DevType)
				{
					case DEVICE_TYPE_STAIR:
					{
						get_stair_fullno(g_DevNo, DevStr[0]);	// 获得梯号
						break;
					}

					case DEVICE_TYPE_AREA:
						strcpy(g_DevNo, DevStr);
						break;

					case DEVICE_TYPE_DOOR_NET:
					case DEVICE_TYPE_DOOR_PHONE:
						strcpy(g_DevNo, DevStr);
						break;

					default:
						break;
				}
				g_InterState = MONITOR_REQUEST;
			}
			break;

		case MONITOR_MONITORING:
			if (atoi(pmonitorbak_data->Buf) == MONITOR_BUSY)
			{
				g_ErrHintTxtID = SID_Inter_busy;
				g_ErrHintTicks = 3;
			}
			else
			{
				g_InterState = MONITOR_MONITORING;
			}
			break;

		case MONITOR_TALKING:
			ituTextSetString(MonitorTimeText, NULL);
			ituWidgetSetVisible(MonitorShowButtomContainer, true);
			g_volume = storage_get_talkvolume();
			g_InterState = MONITOR_TALKING;
			break;

		case MONITOR_END:
			g_InterState = MONITOR_END;
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

				case MONITOR_BUSY:					// 对方忙
					g_ErrHintTxtID = SID_Inter_busy;
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
			break;

		default:
			break;
	}
	DrawStringHint();

	return true;
}

/*************************************************
Function:		MonitorButtomSoundOnChanged
Description: 	音量按下执行函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool MonitorButtomSoundOnChanged(ITUWidget* widget, char* param)
{
	uint8 volume = atoi(param);

	g_volume = volume;
	ChangeVolume();
	dprintf("volume..........%d\n", volume);

	return true;
}

/*************************************************
Function:		MonitorButtomSoundButtonOnMouseUp
Description: 	是否静音按下执行函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool MonitorButtomSoundButtonOnMouseUp(ITUWidget* widget, char* param)
{
	if (MonitorSoundSprite->frame == MonitorNoMuteIcon)
	{
		ituSpriteGoto(MonitorSoundSprite, MonitorMuteIcon);
	}
	else
	{
		ituSpriteGoto(MonitorSoundSprite, MonitorNoMuteIcon);
	}

	g_mute = !g_mute;
	ChangeVolume();

	return true;
}

/*************************************************
Function:		MonitorShowButtomBackgroundOnMouseUp
Description: 	是否显示音量调节函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool MonitorShowButtomBackgroundOnMouseUp(ITUWidget* widget, char* param)
{
	if (!g_SetVolume)
	{
		g_volumeticks = 4;
		// 通话时才可调音量
		if (g_InterState == MONITOR_TALKING)
		{
			g_SetVolume = TRUE;
			// 显示音量初始化
			g_volume = storage_get_talkvolume();
		}
		ituWidgetSetVisible(MonitorHitBackground, false);
		ituWidgetSetVisible(MonitorBottomBackground, true);
	}
	printf(" MonitorShowButtomBackgroundOnMouseUp.........\n");

	return true;
}

/*************************************************
Function:		MonitorLayerButtonOnMouseUp
Description: 	右边按键弹起函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool MonitorLayerButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 btn_event = atoi(param);

	dprintf("btn_event: %d\n", btn_event);
	switch (btn_event)
	{
		case MonitorAnswerEvent:
			MonitorAnswerCall();
			break;

		case MonitorLockEvent:
		case MonitorSnapEvent:
			if (g_SetVolume)
			{
				g_volumeticks = 0;
				g_SetVolume = FALSE;
				storage_set_volume(storage_get_ringvolume(), g_volume, storage_get_keykeep());
				ituWidgetSetVisible(MonitorBottomBackground, false);
			}
			SetMonitorLockAndSnap(btn_event);
			break;

		case MonitorHandUpEvent:
		case MonitorExitEvent:
			ui_monitor_stop();
			break;

		default:
			break;
	}
	return true;
}

/*************************************************
Function:		MonitorLayerOnEnter
Description:	监视初始化处理函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool MonitorLayerOnEnter(ITUWidget* widget, char* param)
{
	g_MonitorLastTick = SDL_GetTicks();
	InitRightButton();
	ituWidgetSetVisible(MonitorHitBackground, false);
	ituWidgetSetVisible(MonitorBottomBackground, false);
	ituWidgetSetVisible(MonitorShowButtomContainer, false);
	ituTextSetString(MonitorTimeText, NULL);
	ituTextSetString(MonitorCallNoText, NULL);
	SetMonitorInfo();
	DrawStringHint();
	g_StartMonitorTick = 1;

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
	if (!MonitorLayer)
	{
		MainLayer = ituSceneFindWidget(&theScene, "mainLayer");
		assert(MainLayer);

		MonitorLayer = ituSceneFindWidget(&theScene, "MonitorLayer");
		assert(MonitorLayer);

		MonitorRightAnswerContainer = ituSceneFindWidget(&theScene, "MonitorRightAnswerContainer");
		assert(MonitorRightAnswerContainer);

		MonitorRightHandUpContainer = ituSceneFindWidget(&theScene, "MonitorRightHandUpContainer");
		assert(MonitorRightHandUpContainer);

		MonitorRightLockContainer = ituSceneFindWidget(&theScene, "MonitorRightLockContainer");
		assert(MonitorRightLockContainer);

		MonitorRightSnapContainer = ituSceneFindWidget(&theScene, "MonitorRightSnapContainer");
		assert(MonitorRightSnapContainer);

		MonitorBottomBackground = ituSceneFindWidget(&theScene, "MonitorBottomBackground");
		assert(MonitorBottomBackground);

		MonitorHitBackground = ituSceneFindWidget(&theScene, "MonitorHitBackground");
		assert(MonitorHitBackground);

		MonitorTimeText = ituSceneFindWidget(&theScene, "MonitorTimeText");
		assert(MonitorTimeText);

		MonitorCallNoText = ituSceneFindWidget(&theScene, "MonitorCallNoText");
		assert(MonitorCallNoText);

		MonitorHitSprite = ituSceneFindWidget(&theScene, "MonitorHitSprite");
		assert(MonitorHitSprite);

		MonitorSoundSprite = ituSceneFindWidget(&theScene, "MonitorSoundSprite");
		assert(MonitorSoundSprite);
		
		MonitorShowButtomContainer = ituSceneFindWidget(&theScene, "MonitorShowButtomContainer");
		assert(MonitorShowButtomContainer);
	}
}

/*************************************************
Function:		MonitorWin
Description: 	监视界面
Input:
1.DevType		设备类型
2.index			索引
utput:			无
Return:			无
Others:			无
*************************************************/
void MonitorWin(DEVICE_TYPE_E DevType, uint8 index)
{
	int32 ret = ui_show_win_arbitration(SYS_OPER_MONITOR);
	if (ret == TRUE)
	{
		g_index = index;
		g_DevType = DevType;
		InitMonitorLayer();
		ituLayerGoto(MonitorLayer);
	}
}

/*************************************************
Function:		MonitorReset
Description: 	监视界面初始化函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
void MonitorReset(void)
{
	MonitorLayer = NULL;
}

