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

static int g_page = 0;
static uint8 g_index[MAX_RING_ID];					// 声音ID(梯口 区口 门口 中心 住户 分机 信息 预警)
static LYLY_PARAM *g_param = NULL;
static uint8 g_alarm_time_param[3] = { 0 };				//  g_alarm_time_param[2]:是否启用预警提示音
static uint8 g_index_flag = 0;						//缓存设置的值

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
	ituTextSetString(SetLylyRingtone2Text, get_str(SID_Set_Ring_Lyly_default_tip + g_param->default_tip));

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
	//sys_start_play_audio(SYS_MEDIA_MUSIC, (char *)storage_get_ring_by_id(value), FALSE, storage_get_ringvolume(), NULL, NULL);

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
		//sys_start_play_audio(SYS_MEDIA_MUSIC, (char *)storage_get_yj_path_byID(value - 1), FALSE, storage_get_ringvolume(), NULL, NULL);
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
		//sys_stop_play_audio(SYS_MEDIA_MUSIC);

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

	//sys_start_play_audio(SYS_MEDIA_MUSIC, (char *)storage_get_msg_hit_byID(value), FALSE, storage_get_ringvolume(), NULL, NULL);

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
	//sys_stop_play_audio(SYS_MEDIA_MUSIC);

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
	//sys_stop_play_audio(SYS_MEDIA_MUSIC);
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
Function:		SetLylyRingtoneButtonOnMouseUp
Description: 	留影留言提示音
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetLylyRingtoneButtonOnMouseUp(ITUWidget* widget, char* param)
{


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
		ituWidgetSetVisible(SetRingtonePromptMsgBackground, false);
		ituWidgetEnable(SetRingtoneBackground);
		return;
	}
	else if (ituWidgetIsVisible(SetCallRingMsgBackground))
	{
		ituWidgetSetVisible(SetCallRingMsgBackground, false);
		ituWidgetEnable(SetRingtoneBackground);
		return;
	}
	else if (ituWidgetIsVisible(SetAlarmRingtoneMsgBackground))
	{
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
