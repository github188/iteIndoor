/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_ringtone.c
Author:     	zxc
Version:
Date: 		2016-06-15
Description:	铃声设置
History:
1. Date:
Author:
Modification:
*************************************************/
#include "layer_set.h"

#define PLAY_VOLUME			4

typedef enum
{
	MEDIA_STATE_STOP = 0x00,
	MEDIA_STATE_PLAY,
	MEDIA_STATE_REC,
	MEDIA_STATE_SUSPEND,
}MEDIA_STATE;

static ITUBackground* SetRingtoneBackground = NULL;
static ITUBackground* SetRingtonePromptMsgBackground = NULL;
static ITUBackground* SetCallRingMsgBackground = NULL;
static ITUBackground* SetAlarmRingtoneMsgBackground = NULL;
static ITUBackground* SetLylyRingRecMsgBackground = NULL;
static ITUCalendar* SetCallRingtoneListContainer = NULL;
static ITUCalendar* SetRingtoneListContainer = NULL;
static ITULayer* SetMenuLayer = NULL;
static ITUText* SetRingtoneTitleText = NULL;
static ITUText* SetAlarmRingtone2Text = NULL;
static ITUText* SetHitRingtone2Text = NULL;
static ITUText* SetLylyRingtone2Text = NULL;
static ITUCoverFlow* SetCallRingtoneListCoverFlow = NULL;
static ITUText* SetCallRingtoneList[6] = { NULL };
static ITUCoverFlow* SetCallRingMsgCoverFlow = NULL;
static ITUText* SetCallRingMsgTitleText = NULL;
static ITUSprite* SetCallRingMsgSprite[8] = { NULL };
static ITUSprite* SetRingtonePromptMsgSprite[4] = { NULL };
static ITUSprite* SetAlarmRingtoneMsgListSprite[5] = { NULL };

static ITUSprite* SetLylyRingRecMsg0Sprite = NULL;
static ITUSprite* SetLylyRingRecMsg1Sprite = NULL;
static ITUText* SetLylyRingRecMsgTime0Text = NULL;
static ITUText* SetLylyRingRecMsgTime1Text = NULL;
static ITUIcon* SetLylyRingRecMsgStartDisableIcon = NULL;
static ITUIcon* SetLylyRingRecMsgPayDisableIcon = NULL;
static ITUIcon* SetLylyRingRecMsgStopDisableIcon = NULL;
static ITUButton* SetLylyRingRecMsgStartButton = NULL;
static ITUButton* SetLylyRingRecMsgPauseButton = NULL;
static ITUButton* SetLylyRingRecMsgPayButton = NULL;
static ITUButton* SetLylyRingRecMsgStopButton = NULL;

static int g_page = 0;
static uint8 g_index[MAX_RING_ID];								// 声音ID(梯口 区口 门口 中心 住户 分机 信息 预警)
static LYLY_PARAM *g_param = NULL;
static uint8 g_alarm_time_param[3] = { 0 };						//  g_alarm_time_param[2]:是否启用预警提示音
static uint8 g_index_flag = 0;									// 缓存铃声设置的值
static uint8 g_rec_tip_param = 0;								// 缓存留影留言提示音的值
static MEDIA_STATE g_rec_media_state = MEDIA_STATE_STOP;		// 录制音媒体转态
static int g_lyly_rec_proc_time = 0;							// 定时器时间，播放，录制进度时间
static uint8 g_timer_run_flag = FALSE;							// 定时器计时标志
static uint32 g_last_time_tick;									// 用来记录定时器上个时刻的时间

/*************************************************
Function:		SetRingtoneOnEnter
Description: 	声音设置界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetRingtoneOnEnter(ITUWidget* widget, char* param)
{
	uint8 i;
	uint8 tmp[40];

	if (!SetRingtoneBackground)
	{
		SetRingtoneBackground = ituSceneFindWidget(&theScene, "SetRingtoneBackground");
		assert(SetRingtoneBackground);

		SetRingtonePromptMsgBackground = ituSceneFindWidget(&theScene, "SetRingtonePromptMsgBackground");
		assert(SetRingtonePromptMsgBackground); 

		SetCallRingMsgBackground = ituSceneFindWidget(&theScene, "SetCallRingMsgBackground");
		assert(SetCallRingMsgBackground);

		SetAlarmRingtoneMsgBackground = ituSceneFindWidget(&theScene, "SetAlarmRingtoneMsgBackground");
		assert(SetAlarmRingtoneMsgBackground);

		SetLylyRingRecMsgBackground = ituSceneFindWidget(&theScene, "SetLylyRingRecMsgBackground");
		assert(SetLylyRingRecMsgBackground);

		SetCallRingtoneListContainer = ituSceneFindWidget(&theScene, "SetCallRingtoneListContainer");
		assert(SetCallRingtoneListContainer); 

		SetRingtoneListContainer = ituSceneFindWidget(&theScene, "SetRingtoneListContainer");
		assert(SetRingtoneListContainer);

		SetMenuLayer = ituSceneFindWidget(&theScene, "SetMenuLayer");
		assert(SetMenuLayer);

		SetRingtoneTitleText = ituSceneFindWidget(&theScene, "SetRingtoneTitleText");
		assert(SetRingtoneTitleText);

		SetAlarmRingtone2Text = ituSceneFindWidget(&theScene, "SetAlarmRingtone2Text");
		assert(SetAlarmRingtone2Text);

		SetHitRingtone2Text = ituSceneFindWidget(&theScene, "SetHitRingtone2Text");
		assert(SetHitRingtone2Text);

		SetLylyRingtone2Text = ituSceneFindWidget(&theScene, "SetLylyRingtone2Text");
		assert(SetLylyRingtone2Text);
	}

	if (strcmp(param, "SetMenuLayer") == 0)
	{
		for (i = 0; i < MAX_RING_TYPE; i++)
		{
			g_index[i] = storage_get_ring_id(i);
		}
		storage_get_time_param(g_alarm_time_param);

		memset(tmp, 0, sizeof(tmp));
		if (g_alarm_time_param[2])
		{
			sprintf(tmp, "%s%d", get_str(SID_Set_Ring_Hit), g_index[7] + 1);
		}
		else
		{
			strcpy(tmp, get_str(SID_Set_Null));
		}
		ituTextSetString(SetAlarmRingtone2Text, tmp);
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", get_str(SID_Set_Ring_Hit), g_index[6] + 1);
		ituTextSetString(SetHitRingtone2Text, tmp);

		g_param = (PLYLY_PARAM)malloc(sizeof(LYLY_PARAM));
		if (NULL == g_param)
		{
			return FALSE;
		}
		memcpy(g_param, (PLYLY_PARAM)storage_get_lyly_param(), sizeof(LYLY_PARAM));
		g_rec_tip_param = g_param->default_tip;
		ituTextSetString(SetLylyRingtone2Text, get_str(SID_Set_Ring_Lyly_default_tip + g_rec_tip_param));

		ituTextSetString(SetRingtoneTitleText, get_str(SID_Set_Ring_RingSet));

		ituWidgetSetVisible(SetRingtonePromptMsgBackground, false);
		ituWidgetSetVisible(SetCallRingMsgBackground, false);
		ituWidgetSetVisible(SetAlarmRingtoneMsgBackground, false);
		ituWidgetSetVisible(SetLylyRingRecMsgBackground, false);
		ituWidgetSetVisible(SetRingtoneBackground, true);
		ituWidgetSetVisible(SetCallRingtoneListContainer, false);
		if (!ituWidgetIsEnabled(SetRingtoneBackground))
		{
			ituWidgetEnable(SetRingtoneBackground);
		}

		g_last_time_tick = SDL_GetTicks();		//开启定时器前要先获取一次当前时间以便对比
	}

	return true;
}

/*************************************************
Function:		SetCallRingtoneButtonOnMouseUp
Description: 	来电铃声按钮
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetCallRingtoneButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 i;
	uint8 tmp[40];

	if (!SetCallRingtoneListCoverFlow)
	{
		SetCallRingtoneListCoverFlow = ituSceneFindWidget(&theScene, "SetCallRingtoneListCoverFlow");
		assert(SetCallRingtoneListCoverFlow);

		SetCallRingtoneList[0] = ituSceneFindWidget(&theScene, "SetCallRingStair2Text");
		assert(SetCallRingtoneList[0]);

		SetCallRingtoneList[1] = ituSceneFindWidget(&theScene, "SetCallRingArea2Text");
		assert(SetCallRingtoneList[1]);

		SetCallRingtoneList[2] = ituSceneFindWidget(&theScene, "SetCallRingDoor2Text");
		assert(SetCallRingtoneList[2]);

		SetCallRingtoneList[3] = ituSceneFindWidget(&theScene, "SetCallRingCenter2Text");
		assert(SetCallRingtoneList[3]);

		SetCallRingtoneList[4] = ituSceneFindWidget(&theScene, "SetCallRingRoom2Text");
		assert(SetCallRingtoneList[4]);

		SetCallRingtoneList[5] = ituSceneFindWidget(&theScene, "SetCallRingFenji2Text");
		assert(SetCallRingtoneList[5]);
	}

	for (i = 0; i < 6; i++)
	{
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", get_str(SID_Set_Ring_Ring), g_index[i] + 1);
		ituTextSetString(SetCallRingtoneList[i], tmp);
	}
	
	ituCoverFlowGoto(SetCallRingtoneListCoverFlow, 0);
	ituTextSetString(SetRingtoneTitleText, get_str(SID_Set_Ring_CallRing));
	ituWidgetSetVisible(SetRingtoneListContainer, false);
	ituWidgetSetVisible(SetCallRingtoneListContainer, true);

	return true;
}

/*************************************************
Function:		SetCallRingtoneListButtonOnMouseUp
Description: 	来电铃声列表按钮
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetCallRingtoneListButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 i = 0;

	if (!SetCallRingMsgCoverFlow)
	{
		char textname[50];

		SetCallRingMsgCoverFlow = ituSceneFindWidget(&theScene, "SetCallRingMsgCoverFlow");
		assert(SetCallRingMsgCoverFlow);

		SetCallRingMsgTitleText = ituSceneFindWidget(&theScene, "SetCallRingMsgTitleText");
		assert(SetCallRingMsgTitleText);

		for (i = 0; i < 8; i++)
		{
			memset(textname, 0, sizeof(textname));
			sprintf(textname, "%s%d%s", "SetCallRingMsg", i + 1, "Sprite");
			SetCallRingMsgSprite[i] = ituSceneFindWidget(&theScene, textname);
			assert(SetCallRingMsgSprite[i]);
		}
	}

	g_page = atoi(param);
	for (i = 0; i < 8; i++)
	{
		if (i != g_index[g_page])
		{
			ituSpriteGoto(SetCallRingMsgSprite[i], 0);
		}
		else
		{
			ituSpriteGoto(SetCallRingMsgSprite[i], 1);
		}
	}

	g_index_flag = g_index[g_page];

	ituCoverFlowGoto(SetCallRingMsgCoverFlow, 0);
	ituTextSetString(SetCallRingMsgTitleText, get_str(SID_Set_Ring_Stair + g_page));
	ituWidgetDisable(SetRingtoneBackground);
	ituWidgetSetVisible(SetCallRingMsgBackground, true);

	return true;
}

/*************************************************
Function:		SetCallRingMsgButtonOnMouseUp
Description: 	来电铃声按钮
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetCallRingMsgButtonOnMouseUp(ITUWidget* widget, char* param)
{
	int value = atoi(param);

	ituSpriteGoto(SetCallRingMsgSprite[g_index_flag], 0);
	g_index_flag = (uint8)value;
	ituSpriteGoto(SetCallRingMsgSprite[g_index_flag], 1);
	sys_start_play_audio(SYS_MEDIA_MUSIC, (char *)storage_get_ring_by_id(value), FALSE, storage_get_ringvolume(), NULL, NULL);

	return true;
}

/*************************************************
Function:		SetAlarmRingtoneButtonOnMouseUp
Description: 	预警提示音按钮
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetAlarmRingtoneButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 i = 0;
	if (!SetAlarmRingtoneMsgListSprite[0])
	{
		char textname[50];

		for (i = 0; i < 5; i++)
		{
			memset(textname, 0, sizeof(textname));
			sprintf(textname, "%s%d%s", "SetAlarmRingtoneMsgList", i, "Sprite");
			SetAlarmRingtoneMsgListSprite[i] = ituSceneFindWidget(&theScene, textname);
			assert(SetAlarmRingtoneMsgListSprite[i]);
		}
	}

	g_page = 7;

	if (g_alarm_time_param[2])
	{
		for (i = 0; i < 5; i++)
		{
			if (i != g_index[g_page] + 1)
			{
				ituSpriteGoto(SetAlarmRingtoneMsgListSprite[i], 0);
			}
			else
			{
				ituSpriteGoto(SetAlarmRingtoneMsgListSprite[i], 1);
			}
		}
		g_index_flag = g_index[g_page] + 1;
	}
	else
	{
		ituSpriteGoto(SetAlarmRingtoneMsgListSprite[0], 1);
		for (i = 1; i < 5; i++)
		{
			ituSpriteGoto(SetAlarmRingtoneMsgListSprite[i], 0);
		}
		g_index_flag = 0;
	}

	ituWidgetDisable(SetRingtoneBackground);
	ituWidgetSetVisible(SetAlarmRingtoneMsgBackground, true);

	return true;
}

/*************************************************
Function:		SetAlarmRingtoneMsgListButtonOnMouseUp
Description: 	预警提示音消息框按钮
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetAlarmRingtoneMsgListButtonOnMouseUp(ITUWidget* widget, char* param)
{
	int value = atoi(param);

	ituSpriteGoto(SetAlarmRingtoneMsgListSprite[g_index_flag], 0);
	g_index_flag = (uint8)value;
	ituSpriteGoto(SetAlarmRingtoneMsgListSprite[g_index_flag], 1);

	if (0 != g_index_flag)
	{
		sys_start_play_audio(SYS_MEDIA_MUSIC, (char *)storage_get_yj_path_byID(value - 1), FALSE, storage_get_ringvolume(), NULL, NULL);
	}

	return true;
}

/*************************************************
Function:		SetAlarmRingtoneMsgOkButtonOnMouseUp
Description: 	报警提示音消息框确认 按钮
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetAlarmRingtoneMsgOkButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 tmp[40];
	memset(tmp, 0, sizeof(tmp));

	if (0 == g_index_flag)
	{
		g_alarm_time_param[2] = 0;

		strcpy(tmp, get_str(SID_Set_Null));
	}
	else
	{
		g_alarm_time_param[2] = 1;
		g_index[g_page] = g_index_flag - 1;

		storage_set_ring_id(g_page, g_index[g_page]);
		sys_stop_play_audio(SYS_MEDIA_MUSIC);

		sprintf(tmp, "%s%d", get_str(SID_Set_Ring_Hit), g_index[g_page] + 1);
	}
	g_index_flag = 0;
	storage_set_time_param(g_alarm_time_param);

	ituTextSetString(SetAlarmRingtone2Text, tmp);

	ituWidgetSetVisible(SetAlarmRingtoneMsgBackground, false);
	ituWidgetEnable(SetRingtoneBackground);

	return true;
}

/*************************************************
Function:		SetHitRingtoneButtonOnMouseUp
Description: 	信息提示音按钮
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetHitRingtoneButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 i = 0;
	if (!SetRingtonePromptMsgSprite[0])
	{
		char textname[50];

		for (i = 0; i < 4; i++)
		{
			memset(textname, 0, sizeof(textname));
			sprintf(textname, "%s%d%s", "SetRingtonePromptMsg", i + 1, "Sprite");
			SetRingtonePromptMsgSprite[i] = ituSceneFindWidget(&theScene, textname);
			assert(SetRingtonePromptMsgSprite[i]);
		}
	}

	g_page = 6;

	for (i = 0; i < 4; i++)
	{
		if (i != g_index[g_page])
		{
			ituSpriteGoto(SetRingtonePromptMsgSprite[i], 0);
		}
		else
		{
			ituSpriteGoto(SetRingtonePromptMsgSprite[i], 1);
		}
	}

	g_index_flag = g_index[g_page];

	ituWidgetDisable(SetRingtoneBackground);
	ituWidgetSetVisible(SetRingtonePromptMsgBackground, true);

	return true;
}

/*************************************************
Function:		SetRingtonePromptMsgButtonOnMouseUp
Description: 	信息提示音消息框按钮
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetRingtonePromptMsgButtonOnMouseUp(ITUWidget* widget, char* param)
{
	int value = atoi(param);

	ituSpriteGoto(SetRingtonePromptMsgSprite[g_index_flag], 0);
	g_index_flag = (uint8)value;
	ituSpriteGoto(SetRingtonePromptMsgSprite[g_index_flag], 1);

	sys_start_play_audio(SYS_MEDIA_MUSIC, (char *)storage_get_msg_hit_byID(value), FALSE, storage_get_ringvolume(), NULL, NULL);

	return true;
}

/*************************************************
Function:		SetRingtonePromptMsgTureButtonOnMouseUp
Description: 	消息框确认 按钮
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetRingtonePromptMsgTureButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 tmp[40];

	g_index[g_page] = g_index_flag;
	g_index_flag = 0;

	storage_set_ring_id(g_page, g_index[g_page]);
	sys_stop_play_audio(SYS_MEDIA_MUSIC);

	memset(tmp, 0, sizeof(tmp));

	if (6 == g_page)
	{
		sprintf(tmp, "%s%d", get_str(SID_Set_Ring_Hit), g_index[g_page] + 1);
		ituTextSetString(SetHitRingtone2Text, tmp);
		ituWidgetSetVisible(SetRingtonePromptMsgBackground, false);
	}
	else
	{
		sprintf(tmp, "%s%d", get_str(SID_Set_Ring_Ring), g_index[g_page] + 1);
		ituTextSetString(SetCallRingtoneList[g_page], tmp);
		ituWidgetSetVisible(SetCallRingMsgBackground, false);
	}	
	ituWidgetEnable(SetRingtoneBackground);

	return true;
}

/*************************************************
Function:		SetRingtonePromptMsgFalseButtonOnMouseUp
Description: 	消息框取消 按钮
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetRingtonePromptMsgFalseButtonOnMouseUp(ITUWidget* widget, char* param)
{
	sys_stop_play_audio(SYS_MEDIA_MUSIC);
	g_index_flag = 0;

	switch (g_page)
	{
	case 6:
		ituWidgetSetVisible(SetRingtonePromptMsgBackground, false);
		ituWidgetEnable(SetRingtoneBackground);
		break;

	case 7:
		ituWidgetSetVisible(SetAlarmRingtoneMsgBackground, false);
		ituWidgetEnable(SetRingtoneBackground);
		break;

	default:
		ituWidgetSetVisible(SetCallRingMsgBackground, false);
		ituWidgetEnable(SetRingtoneBackground);
		break;
	}

	return true;
}

/*************************************************
Function:		ShowSetLylyRingRecButtom
Description: 	留影留言提示音录、播、暂、停按键显示
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void ShowSetLylyRingRecButtom(MEDIA_STATE state)
{
	if (!SetLylyRingRecMsgStartButton)
	{
		SetLylyRingRecMsgStartButton = ituSceneFindWidget(&theScene, "SetLylyRingRecMsgStartButton");
		assert(SetLylyRingRecMsgStartButton);

		SetLylyRingRecMsgPauseButton = ituSceneFindWidget(&theScene, "SetLylyRingRecMsgPauseButton");
		assert(SetLylyRingRecMsgPauseButton);

		SetLylyRingRecMsgPayButton = ituSceneFindWidget(&theScene, "SetLylyRingRecMsgPayButton");
		assert(SetLylyRingRecMsgPayButton);

		SetLylyRingRecMsgStopButton = ituSceneFindWidget(&theScene, "SetLylyRingRecMsgStopButton");
		assert(SetLylyRingRecMsgStopButton);

		SetLylyRingRecMsgStartDisableIcon = ituSceneFindWidget(&theScene, "SetLylyRingRecMsgStartDisableIcon");
		assert(SetLylyRingRecMsgStartDisableIcon);

		SetLylyRingRecMsgPayDisableIcon = ituSceneFindWidget(&theScene, "SetLylyRingRecMsgPayDisableIcon");
		assert(SetLylyRingRecMsgPayDisableIcon);

		SetLylyRingRecMsgStopDisableIcon = ituSceneFindWidget(&theScene, "SetLylyRingRecMsgStopDisableIcon");
		assert(SetLylyRingRecMsgStopDisableIcon);
	}

	switch (state)
	{
	case MEDIA_STATE_STOP:
		if (1 == g_rec_tip_param)	//自己录制
		{
			ituWidgetSetVisible(SetLylyRingRecMsgStartDisableIcon, false);
			ituWidgetSetVisible(SetLylyRingRecMsgStartButton, true);
		}
		else
		{
			ituWidgetSetVisible(SetLylyRingRecMsgStartButton, false);
			ituWidgetSetVisible(SetLylyRingRecMsgStartDisableIcon, true);
		}
		ituWidgetSetVisible(SetLylyRingRecMsgPauseButton, false);
		ituWidgetSetVisible(SetLylyRingRecMsgStopButton, false);
		ituWidgetSetVisible(SetLylyRingRecMsgPayDisableIcon, false);
		ituWidgetSetVisible(SetLylyRingRecMsgPayButton, true);
		ituWidgetSetVisible(SetLylyRingRecMsgStopDisableIcon, true);
		break;

	case MEDIA_STATE_PLAY:
		ituWidgetSetVisible(SetLylyRingRecMsgPayButton, false);
		ituWidgetSetVisible(SetLylyRingRecMsgStopDisableIcon, false);
		ituWidgetSetVisible(SetLylyRingRecMsgStartButton, false);
		ituWidgetSetVisible(SetLylyRingRecMsgPayDisableIcon, false);
		ituWidgetSetVisible(SetLylyRingRecMsgStartDisableIcon, true);
		ituWidgetSetVisible(SetLylyRingRecMsgPauseButton, true);
		ituWidgetSetVisible(SetLylyRingRecMsgStopButton, true);
		break;

	case MEDIA_STATE_REC:
		ituWidgetSetVisible(SetLylyRingRecMsgStartDisableIcon, true);
		ituWidgetSetVisible(SetLylyRingRecMsgPayDisableIcon, true);
		ituWidgetSetVisible(SetLylyRingRecMsgStopButton, true);
		ituWidgetSetVisible(SetLylyRingRecMsgPayButton, false);
		ituWidgetSetVisible(SetLylyRingRecMsgStopDisableIcon, false);
		ituWidgetSetVisible(SetLylyRingRecMsgStartButton, false);
		ituWidgetSetVisible(SetLylyRingRecMsgPauseButton, false);
		break;

	case MEDIA_STATE_SUSPEND:
		ituWidgetSetVisible(SetLylyRingRecMsgStopDisableIcon, false);
		ituWidgetSetVisible(SetLylyRingRecMsgStartButton, false);
		ituWidgetSetVisible(SetLylyRingRecMsgPauseButton, false);
		ituWidgetSetVisible(SetLylyRingRecMsgPayDisableIcon, false);
		ituWidgetSetVisible(SetLylyRingRecMsgStartDisableIcon, true);
		ituWidgetSetVisible(SetLylyRingRecMsgPayButton, true);
		ituWidgetSetVisible(SetLylyRingRecMsgStopButton, true);
		break;
	}
}

/*************************************************
Function:		ShowSetLylyRingRecTime
Description: 	留影留言提示音进度时间和总时间
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void ShowSetLylyRingRecTime(uint8 flag, int all_time, int proc_time)
{
	char tmp[50];

	if (flag)
	{
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%02d", "00:", all_time);
		ituTextSetString(SetLylyRingRecMsgTime0Text, tmp);
	}

	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp, "%s%02d", "00:", proc_time);
	ituTextSetString(SetLylyRingRecMsgTime1Text, tmp);
}

/*************************************************
Function:		SetLylyRingtoneButtonOnMouseUp
Description: 	留影留言提示音录制界面
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetLylyRingtoneButtonOnMouseUp(ITUWidget* widget, char* param)
{
	int time = 0;
	char tmp[50];

	if (!SetLylyRingRecMsgTime0Text)
	{
		SetLylyRingRecMsgTime0Text = ituSceneFindWidget(&theScene, "SetLylyRingRecMsgTime0Text");
		assert(SetLylyRingRecMsgTime0Text);

		SetLylyRingRecMsgTime1Text = ituSceneFindWidget(&theScene, "SetLylyRingRecMsgTime1Text");
		assert(SetLylyRingRecMsgTime1Text);

		SetLylyRingRecMsg0Sprite = ituSceneFindWidget(&theScene, "SetLylyRingRecMsg0Sprite");
		assert(SetLylyRingRecMsg0Sprite);

		SetLylyRingRecMsg1Sprite = ituSceneFindWidget(&theScene, "SetLylyRingRecMsg1Sprite");
		assert(SetLylyRingRecMsg1Sprite);
	}

	g_rec_media_state = MEDIA_STATE_STOP;

	if (1 == g_rec_tip_param)	//自己录制
	{
		ituSpriteGoto(SetLylyRingRecMsg1Sprite, 1);
		ituSpriteGoto(SetLylyRingRecMsg0Sprite, 0);

		time = media_get_wav_time(LYLY_HIT_RE_FILE_PATH);
	}
	else                    //默认提示音
	{
		ituSpriteGoto(SetLylyRingRecMsg0Sprite, 1);
		ituSpriteGoto(SetLylyRingRecMsg1Sprite, 0);

		time = media_get_wav_time(storage_get_lyly_tip_path_bytip());
	}

	ShowSetLylyRingRecTime(TRUE, time, 0);

	ShowSetLylyRingRecButtom(g_rec_media_state);
	ituWidgetDisable(SetRingtoneBackground);
	ituWidgetSetVisible(SetLylyRingRecMsgBackground, true);
	
	return true;
}

/*************************************************
Function:		lyrecordDestroyProc
Description: 	销毁处理函数
Input:
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static uint32 lyrecordDestroyProc()
{
	if (g_rec_media_state != MEDIA_STATE_STOP)
	{
		switch (g_rec_media_state)
		{
		case MEDIA_STATE_PLAY:
		case MEDIA_STATE_SUSPEND:
			sys_stop_play_audio(SYS_MEDIA_MUSIC);
			break;

		case MEDIA_STATE_REC:
			sys_stop_family_record();
			break;
		}
		g_rec_media_state = MEDIA_STATE_STOP;
	}
	return TRUE;
}

/*************************************************
Function:		SetLylyRingRecMsgNoButtonOnMouseUp
Description: 	留影留言提示音录制界面取消键
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetLylyRingRecMsgNoButtonOnMouseUp(ITUWidget* widget, char* param)
{
	//lyrecordDestroyProc();
	if (MEDIA_STATE_STOP == g_rec_media_state)
	{
		ituWidgetSetVisible(SetLylyRingRecMsgBackground, false);
		ituWidgetEnable(SetRingtoneBackground);
	}

	return true;
}

/*************************************************
Function:		SetLylyRingRecMsgYesButtonOnMouseUp
Description: 	留影留言提示音录制界面确认键
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetLylyRingRecMsgYesButtonOnMouseUp(ITUWidget* widget, char* param)
{
	if (MEDIA_STATE_STOP == g_rec_media_state)
	{
		// add by caogw 2016-06-20为解决选择录制提示音模式时，但无录制文件，则留影留言会死机问题
		if ((is_fileexist(LYLY_HIT_RE_FILE_PATH) == TRUE && g_rec_tip_param == 1) || g_rec_tip_param == 0)
		{
			storage_set_lyly_tip(g_rec_tip_param);
			
			ituTextSetString(SetLylyRingtone2Text, get_str(SID_Set_Ring_Lyly_default_tip + g_rec_tip_param));
			ituWidgetSetVisible(SetLylyRingRecMsgBackground, false);
			ituWidgetEnable(SetRingtoneBackground);
		}
		else
		{
			ShowMsgFailHintSuccessLayer(0, SID_Set_Ring_Tips_Fail, 0);
		}
	}
	return true;
}

/*************************************************
Function:		SetLylyRingRecMsgButtonOnMouseUp
Description: 	留影留言提示音录制界面选择按键
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetLylyRingRecMsgButtonOnMouseUp(ITUWidget* widget, char* param)
{
	int index = atoi(param);
	int time = 0;

	if (1 == index)
	{
		g_rec_tip_param = 1;
		ituSpriteGoto(SetLylyRingRecMsg1Sprite, 1);
		ituSpriteGoto(SetLylyRingRecMsg0Sprite, 0);

		time = media_get_wav_time(LYLY_HIT_RE_FILE_PATH);
		dprintf("time = %d\n", time);
	}
	else
	{
		g_rec_tip_param = 0;
		ituSpriteGoto(SetLylyRingRecMsg1Sprite, 0);
		ituSpriteGoto(SetLylyRingRecMsg0Sprite, 1);

		time = media_get_wav_time(storage_get_lyly_tip_path_bytip());
	}
	ShowSetLylyRingRecTime(TRUE, time, 0);
	ShowSetLylyRingRecButtom(g_rec_media_state);
	
	return true;
}

/*************************************************
Function:		MediaRecord_Callback
Description: 	进度回调函数
Input:
1.param1
2.param2
3.state
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static uint32 MediaRecord_Callback(int32 param1, int32 param2, int32 state)
{
	if (FALSE == state)
	{
		sys_stop_play_audio(SYS_MEDIA_MUSIC);
		g_rec_media_state = MEDIA_STATE_STOP;
		ShowSetLylyRingRecButtom(g_rec_media_state);
		ShowSetLylyRingRecTime(FALSE, 0, 0);
		g_timer_run_flag = FALSE;
		g_lyly_rec_proc_time = 0;
		return TRUE;
	}
#if 0
	if (g_rec_media_state == MEDIA_STATE_REC)
	{
		if (100 == param2)
		{
			sys_stop_play_audio(SYS_MEDIA_MUSIC);
			g_rec_media_state = MEDIA_STATE_STOP;
			ShowSetLylyRingRecButtom(g_rec_media_state);

			ShowSetLylyRingRecTime(TRUE, g_lyly_rec_proc_time, 0);
			g_timer_run_flag = FALSE;
			g_lyly_rec_proc_time = 0;
		}
	}
#endif
	return TRUE;
}

/*************************************************
Function:		MediaPlay_Callback
Description: 	进度回调函数
Input:
1.param1
2.param2
3.state
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static uint32 MediaPlay_Callback(int32 param1, int32 param2, int32 state)
{
	if (FALSE == state)
	{
		sys_stop_play_audio(SYS_MEDIA_MUSIC);
		g_rec_media_state = MEDIA_STATE_STOP;
		ShowSetLylyRingRecButtom(g_rec_media_state);
		ShowSetLylyRingRecTime(FALSE, 0, 0);
		g_timer_run_flag = FALSE;
		g_lyly_rec_proc_time = 0;
		return TRUE;
	}

	if (g_rec_media_state == MEDIA_STATE_PLAY)
	{
		if (100 == param2)
		{
			sys_stop_play_audio(SYS_MEDIA_MUSIC);
			g_rec_media_state = MEDIA_STATE_STOP;

			ShowSetLylyRingRecButtom(g_rec_media_state);

			ShowSetLylyRingRecTime(FALSE, 0, 0);
			g_timer_run_flag = FALSE;
			g_lyly_rec_proc_time = 0;
		}		
	}
	return TRUE;
}

/*************************************************
Function:		SetLylyRingRecMsgRecStateButtonOnMouseUp
Description: 	留影留言提示音录制界面录、播、暂、停按键按下
Input:		无	2:录、1:播、3:暂、0:停
Output:		无
Return:		TRUE 是 FALSE 否
Others:	
*************************************************/
bool SetLylyRingRecMsgRecStateButtonOnMouseUp(ITUWidget* widget, char* param)
{
	int32 ret = 0;
	int index = atoi(param);

	switch (index)
	{
	case 0:				//停止按钮
		if (MEDIA_STATE_REC == g_rec_media_state)
		{
			ShowSetLylyRingRecTime(TRUE, g_lyly_rec_proc_time, 0);
		}
		else
		{
			ShowSetLylyRingRecTime(FALSE, 0, 0);
		}
		lyrecordDestroyProc();
		g_timer_run_flag = FALSE;
		g_lyly_rec_proc_time = 0;
		ShowSetLylyRingRecButtom(g_rec_media_state);
		break;

	case 1:				//播放按钮
		switch (g_rec_media_state)
		{
		case MEDIA_STATE_STOP:
			if (0 == g_rec_tip_param)
			{
				ret = sys_start_play_audio(SYS_MEDIA_MUSIC, storage_get_lyly_tip_path_bytip(), FALSE, PLAY_VOLUME, MediaPlay_Callback, NULL);
			}
			else
			{
				ret = sys_start_play_audio(SYS_MEDIA_MUSIC, LYLY_HIT_RE_FILE_PATH, FALSE, PLAY_VOLUME, MediaPlay_Callback, NULL);
			}
			if (SYS_MEDIA_ECHO_OK == ret)
			{
				g_rec_media_state = MEDIA_STATE_PLAY;
				g_timer_run_flag = TRUE;	// 定时器开始计时
				ShowSetLylyRingRecButtom(g_rec_media_state);
			}
			break;

		case MEDIA_STATE_SUSPEND:
			ret = sys_continue_play_file();		//继续播放函数
			if (SYS_MEDIA_ECHO_OK == ret)				
			{
				g_rec_media_state = MEDIA_STATE_PLAY;
				g_timer_run_flag = TRUE;	// 定时器开始计时
				ShowSetLylyRingRecButtom(g_rec_media_state);
			}
			break;
		}
		break;

	case 2:				//录制按钮
		if (MEDIA_STATE_STOP == g_rec_media_state && 1 == g_rec_tip_param)
		{
			#if 0
			uint8 fileLen = 0;
			char filename[50] = { 0 };
			sprintf(filename, "%s", LYLY_HIT_RE_FILE_PATH);
			fileLen = strlen(filename);
			filename[fileLen - 4] = 0;		// 把文件后缀名.WAV 去除
			#endif
			ret = sys_start_family_record(LYLY_HIT_RE_FILE_PATH, MediaRecord_Callback, NULL);
			if (SYS_MEDIA_ECHO_OK == ret)
			{
				g_rec_media_state = MEDIA_STATE_REC;
				g_timer_run_flag = TRUE;	// 定时器开始计时
				ShowSetLylyRingRecButtom(g_rec_media_state);
			}
		}
		break;

	case 3:				//暂停按钮
		if (MEDIA_STATE_PLAY == g_rec_media_state)
		{
			ret = sys_pause_play_file();
			if (SYS_MEDIA_ECHO_OK == ret)				
			{
				g_rec_media_state = MEDIA_STATE_SUSPEND;
				g_timer_run_flag = FALSE;
				ShowSetLylyRingRecButtom(g_rec_media_state);
			}
		}
		break;

	default:
		break;
	}

	return true;
}

/*************************************************
Function:		SetRingLylyRecTipTimeoutOnTimer
Description: 	安防界面定时器函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetRingLylyRecTipTimeoutOnTimer(ITUWidget* widget, char* param)
{
	uint32 duration;
	uint32 curtime = SDL_GetTicks();

	if (curtime >= g_last_time_tick)
	{
		duration = curtime - g_last_time_tick;
	}
	else
	{
		duration = 0xFFFFFFFF - g_last_time_tick + curtime;
	}

	if (duration >= 1000)		//时间差是以毫秒为单位
	{
		g_last_time_tick = curtime;

		if (g_timer_run_flag)
		{
			g_lyly_rec_proc_time++;

			if (g_lyly_rec_proc_time >= 30)
			{
				ShowSetLylyRingRecTime(TRUE, g_lyly_rec_proc_time, 0);
				g_lyly_rec_proc_time = 0;
				g_timer_run_flag = FALSE;
				lyrecordDestroyProc();
				ShowSetLylyRingRecButtom(g_rec_media_state);
			}
			ShowSetLylyRingRecTime(FALSE, 0, g_lyly_rec_proc_time);
		}
	}

	return true;
}

/*************************************************
Function:		SetRingtoneOnLeave
Description: 	声音设置界面退出处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetRingtoneOnLeave(ITUWidget* widget, char* param)
{
	lyrecordDestroyProc();

	if (g_param)
	{
		free(g_param);
		g_param = NULL;
	}

	return true;
}

void SetRingtoneOnReturn(void)
{
	if (ituWidgetIsVisible(SetRingtonePromptMsgBackground))
	{
		sys_stop_play_audio(SYS_MEDIA_MUSIC);
		ituWidgetSetVisible(SetRingtonePromptMsgBackground, false);
		ituWidgetEnable(SetRingtoneBackground);
		return;
	}
	else if (ituWidgetIsVisible(SetLylyRingRecMsgBackground))
	{
		SetLylyRingRecMsgNoButtonOnMouseUp(NULL, NULL);
		return;
	}
	else if (ituWidgetIsVisible(SetCallRingMsgBackground))
	{
		sys_stop_play_audio(SYS_MEDIA_MUSIC);
		ituWidgetSetVisible(SetCallRingMsgBackground, false);
		ituWidgetEnable(SetRingtoneBackground);
		return;
	}
	else if (ituWidgetIsVisible(SetAlarmRingtoneMsgBackground))
	{
		sys_stop_play_audio(SYS_MEDIA_MUSIC);
		ituWidgetSetVisible(SetAlarmRingtoneMsgBackground, false);
		ituWidgetEnable(SetRingtoneBackground);
		return;
	}
	else if (ituWidgetIsVisible(SetCallRingtoneListContainer))
	{
		ituTextSetString(SetRingtoneTitleText, get_str(SID_Set_Ring_RingSet));
		ituWidgetSetVisible(SetCallRingtoneListContainer, false);
		ituWidgetSetVisible(SetRingtoneListContainer, true);
		return;
	}
	else if (!ituWidgetIsVisible(SetMenuLayer))
	{
		ituLayerGoto(SetMenuLayer);
		return;
	}
}
