/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_ringtone.c
Author:     	zxc
Version:
Date: 		2016-06-15
Description:
History:
1. Date:
Author:
Modification:
*************************************************/
#include "gui_include.h"

static ITUBackground* SetRingtoneBackground;
static ITUBackground* SetRingtonePromptMsgBackground;
static ITULayer* SetMenuLayer;
static ITUSprite* SetRingtonePromptMsg1Sprite;
static ITUSprite* SetRingtonePromptMsg2Sprite;
static ITUSprite* SetRingtonePromptMsg3Sprite;
static ITUSprite* SetRingtonePromptMsg4Sprite;
static ITUText* SetAlarmRingtone2Text;
static ITUText* SetHitRingtone2Text;
static ITUText* SetLylyRingtone2Text;
static ITUText* SetRingtonePromptMsgTitleText;
static ITUText* SetRingtoneTitleText;
static ITUText* SetCallRingStair2Text;
static ITUText* SetCallRingArea2Text;
static ITUText* SetCallRingDoor2Text;
static ITUText* SetCallRingCenter2Text;
static ITUText* SetCallRingRoom2Text;
static ITUText* SetCallRingFenji2Text;
static ITUCalendar* SetCallRingtoneListContainer;
static ITUCalendar* SetRingtoneListContainer;
static ITUBackground* SetCallRingMsgBackground;
static ITUSprite* SetCallRingMsg1Sprite;
static ITUSprite* SetCallRingMsg2Sprite;
static ITUSprite* SetCallRingMsg3Sprite;
static ITUSprite* SetCallRingMsg4Sprite;
static ITUSprite* SetCallRingMsg5Sprite;
static ITUSprite* SetCallRingMsg6Sprite;
static ITUSprite* SetCallRingMsg7Sprite;
static ITUSprite* SetCallRingMsg8Sprite;
static ITUText* SetCallRingMsgTitleText;
static ITUCoverFlow* SetCallRingMsgCoverFlow;
static ITUCoverFlow* SetCallRingtoneListCoverFlow;

static uint8 g_index[MAX_RING_ID];					// 声音ID(梯口 区口 门口 中心 住户 分机 信息 预警)
static LYLY_PARAM *g_param = NULL;
static uint8 g_page = 0;
static uint8 g_index_hit_flag = 0;		//记录预警提示音、信息提示音消息框按下的值
static uint8 g_index_ring_flag = 0;		//记录来电铃声消息框按下的值

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
	uint8 buf[40];

	if (!SetRingtoneBackground)
	{
		SetRingtoneBackground = ituSceneFindWidget(&theScene, "SetRingtoneBackground");
		assert(SetRingtoneBackground);

		SetRingtonePromptMsgBackground = ituSceneFindWidget(&theScene, "SetRingtonePromptMsgBackground");
		assert(SetRingtonePromptMsgBackground); 

		SetCallRingMsgBackground = ituSceneFindWidget(&theScene, "SetCallRingMsgBackground");
		assert(SetCallRingMsgBackground);

		SetCallRingtoneListContainer = ituSceneFindWidget(&theScene, "SetCallRingtoneListContainer");
		assert(SetCallRingtoneListContainer); 

		SetRingtoneListContainer = ituSceneFindWidget(&theScene, "SetRingtoneListContainer");
		assert(SetRingtoneListContainer);

		SetMenuLayer = ituSceneFindWidget(&theScene, "SetMenuLayer");
		assert(SetMenuLayer);

		SetRingtonePromptMsg1Sprite = ituSceneFindWidget(&theScene, "SetRingtonePromptMsg1Sprite");
		assert(SetRingtonePromptMsg1Sprite);

		SetRingtonePromptMsg2Sprite = ituSceneFindWidget(&theScene, "SetRingtonePromptMsg2Sprite");
		assert(SetRingtonePromptMsg2Sprite);

		SetRingtonePromptMsg3Sprite = ituSceneFindWidget(&theScene, "SetRingtonePromptMsg3Sprite");
		assert(SetRingtonePromptMsg3Sprite);

		SetRingtonePromptMsg4Sprite = ituSceneFindWidget(&theScene, "SetRingtonePromptMsg4Sprite");
		assert(SetRingtonePromptMsg4Sprite);

		SetAlarmRingtone2Text = ituSceneFindWidget(&theScene, "SetAlarmRingtone2Text");
		assert(SetAlarmRingtone2Text);

		SetHitRingtone2Text = ituSceneFindWidget(&theScene, "SetHitRingtone2Text");
		assert(SetHitRingtone2Text);

		SetLylyRingtone2Text = ituSceneFindWidget(&theScene, "SetLylyRingtone2Text");
		assert(SetLylyRingtone2Text);

		SetRingtonePromptMsgTitleText = ituSceneFindWidget(&theScene, "SetRingtonePromptMsgTitleText");
		assert(SetRingtonePromptMsgTitleText); 

		SetRingtoneTitleText = ituSceneFindWidget(&theScene, "SetRingtoneTitleText");
		assert(SetRingtoneTitleText);

		SetCallRingStair2Text = ituSceneFindWidget(&theScene, "SetCallRingStair2Text");
		assert(SetCallRingStair2Text);

		SetCallRingArea2Text = ituSceneFindWidget(&theScene, "SetCallRingArea2Text");
		assert(SetCallRingArea2Text);

		SetCallRingDoor2Text = ituSceneFindWidget(&theScene, "SetCallRingDoor2Text");
		assert(SetCallRingDoor2Text);

		SetCallRingCenter2Text = ituSceneFindWidget(&theScene, "SetCallRingCenter2Text");
		assert(SetCallRingCenter2Text);

		SetCallRingRoom2Text = ituSceneFindWidget(&theScene, "SetCallRingRoom2Text");
		assert(SetCallRingRoom2Text);

		SetCallRingFenji2Text = ituSceneFindWidget(&theScene, "SetCallRingFenji2Text");
		assert(SetCallRingFenji2Text);

		SetCallRingMsg1Sprite = ituSceneFindWidget(&theScene, "SetCallRingMsg1Sprite");
		assert(SetCallRingMsg1Sprite);

		SetCallRingMsg2Sprite = ituSceneFindWidget(&theScene, "SetCallRingMsg2Sprite");
		assert(SetCallRingMsg2Sprite);

		SetCallRingMsg3Sprite = ituSceneFindWidget(&theScene, "SetCallRingMsg3Sprite");
		assert(SetCallRingMsg3Sprite);

		SetCallRingMsg4Sprite = ituSceneFindWidget(&theScene, "SetCallRingMsg4Sprite");
		assert(SetCallRingMsg4Sprite);

		SetCallRingMsg5Sprite = ituSceneFindWidget(&theScene, "SetCallRingMsg5Sprite");
		assert(SetCallRingMsg5Sprite);

		SetCallRingMsg6Sprite = ituSceneFindWidget(&theScene, "SetCallRingMsg6Sprite");
		assert(SetCallRingMsg6Sprite);

		SetCallRingMsg7Sprite = ituSceneFindWidget(&theScene, "SetCallRingMsg7Sprite");
		assert(SetCallRingMsg7Sprite);

		SetCallRingMsg8Sprite = ituSceneFindWidget(&theScene, "SetCallRingMsg8Sprite");
		assert(SetCallRingMsg8Sprite); 

		SetCallRingMsgTitleText = ituSceneFindWidget(&theScene, "SetCallRingMsgTitleText");
		assert(SetCallRingMsgTitleText); 

		SetCallRingMsgCoverFlow = ituSceneFindWidget(&theScene, "SetCallRingMsgCoverFlow");
		assert(SetCallRingMsgCoverFlow); 

		SetCallRingtoneListCoverFlow = ituSceneFindWidget(&theScene, "SetCallRingtoneListCoverFlow");
		assert(SetCallRingtoneListCoverFlow);
	}

	for (i = 0; i < MAX_RING_TYPE; i++)
	{
		g_index[i] = storage_get_ring_id(i);
		printf("g_index[i] = %d\n", g_index[i]);
	}

	memset(tmp, 0, sizeof(tmp));
	strcpy(tmp, get_str(SID_Set_Ring_Hit));
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d", g_index[7] + 1);
	strcat(tmp, buf);
	ituTextSetString(SetAlarmRingtone2Text, tmp);
	memset(tmp, 0, sizeof(tmp));
	strcpy(tmp, get_str(SID_Set_Ring_Hit));
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d", g_index[6] + 1);
	strcat(tmp, buf);
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
	ituWidgetSetVisible(SetRingtoneBackground, true);
	ituWidgetSetVisible(SetCallRingtoneListContainer, false);

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
	uint8 buf[40];
	ITUText* text[6] = { SetCallRingStair2Text, SetCallRingArea2Text, SetCallRingDoor2Text,
						 SetCallRingCenter2Text, SetCallRingRoom2Text, SetCallRingFenji2Text };

	for (i = 0; i < 6; i++)
	{
		memset(tmp, 0, sizeof(tmp));
		strcpy(tmp, get_str(SID_Set_Ring_Ring));
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "%d", g_index[i] + 1);
		strcat(tmp, buf);
		ituTextSetString(text[i], tmp);
	}
	
	ituCoverFlowGoto(SetCallRingtoneListCoverFlow, 0);
	ituTextSetString(SetRingtoneTitleText, get_str(SID_Set_Ring_CallRing));
	ituWidgetSetVisible(SetRingtoneListContainer, false);
	ituWidgetSetVisible(SetCallRingtoneListContainer, true);
	
	return true;
}

/*************************************************
Function:		SetRingPageShow
Description: 	设置显示第几个
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void SetRingPageShow(uint8 index)
{
	switch (index)
	{
	case 0:
		ituSpriteGoto(SetCallRingMsg1Sprite, 1);
		ituSpriteGoto(SetCallRingMsg2Sprite, 0);
		ituSpriteGoto(SetCallRingMsg3Sprite, 0);
		ituSpriteGoto(SetCallRingMsg4Sprite, 0);
		ituSpriteGoto(SetCallRingMsg5Sprite, 0);
		ituSpriteGoto(SetCallRingMsg6Sprite, 0);
		ituSpriteGoto(SetCallRingMsg7Sprite, 0);
		ituSpriteGoto(SetCallRingMsg8Sprite, 0);
		break;

	case 1:
		ituSpriteGoto(SetCallRingMsg1Sprite, 0);
		ituSpriteGoto(SetCallRingMsg2Sprite, 1);
		ituSpriteGoto(SetCallRingMsg3Sprite, 0);
		ituSpriteGoto(SetCallRingMsg4Sprite, 0);
		ituSpriteGoto(SetCallRingMsg5Sprite, 0);
		ituSpriteGoto(SetCallRingMsg6Sprite, 0);
		ituSpriteGoto(SetCallRingMsg7Sprite, 0);
		ituSpriteGoto(SetCallRingMsg8Sprite, 0);
		break;

	case 2:
		ituSpriteGoto(SetCallRingMsg1Sprite, 0);
		ituSpriteGoto(SetCallRingMsg2Sprite, 0);
		ituSpriteGoto(SetCallRingMsg3Sprite, 1);
		ituSpriteGoto(SetCallRingMsg4Sprite, 0);
		ituSpriteGoto(SetCallRingMsg5Sprite, 0);
		ituSpriteGoto(SetCallRingMsg6Sprite, 0);
		ituSpriteGoto(SetCallRingMsg7Sprite, 0);
		ituSpriteGoto(SetCallRingMsg8Sprite, 0);
		break;

	case 3:
		ituSpriteGoto(SetCallRingMsg1Sprite, 0);
		ituSpriteGoto(SetCallRingMsg2Sprite, 0);
		ituSpriteGoto(SetCallRingMsg3Sprite, 0);
		ituSpriteGoto(SetCallRingMsg4Sprite, 1);
		ituSpriteGoto(SetCallRingMsg5Sprite, 0);
		ituSpriteGoto(SetCallRingMsg6Sprite, 0);
		ituSpriteGoto(SetCallRingMsg7Sprite, 0);
		ituSpriteGoto(SetCallRingMsg8Sprite, 0);
		break;

	case 4:
		ituSpriteGoto(SetCallRingMsg1Sprite, 0);
		ituSpriteGoto(SetCallRingMsg2Sprite, 0);
		ituSpriteGoto(SetCallRingMsg3Sprite, 0);
		ituSpriteGoto(SetCallRingMsg4Sprite, 0);
		ituSpriteGoto(SetCallRingMsg5Sprite, 1);
		ituSpriteGoto(SetCallRingMsg6Sprite, 0);
		ituSpriteGoto(SetCallRingMsg7Sprite, 0);
		ituSpriteGoto(SetCallRingMsg8Sprite, 0);
		break;

	case 5:
		ituSpriteGoto(SetCallRingMsg1Sprite, 0);
		ituSpriteGoto(SetCallRingMsg2Sprite, 0);
		ituSpriteGoto(SetCallRingMsg3Sprite, 0);
		ituSpriteGoto(SetCallRingMsg4Sprite, 0);
		ituSpriteGoto(SetCallRingMsg5Sprite, 0);
		ituSpriteGoto(SetCallRingMsg6Sprite, 1);
		ituSpriteGoto(SetCallRingMsg7Sprite, 0);
		ituSpriteGoto(SetCallRingMsg8Sprite, 0);
		break;

	case 6:
		ituSpriteGoto(SetCallRingMsg1Sprite, 0);
		ituSpriteGoto(SetCallRingMsg2Sprite, 0);
		ituSpriteGoto(SetCallRingMsg3Sprite, 0);
		ituSpriteGoto(SetCallRingMsg4Sprite, 0);
		ituSpriteGoto(SetCallRingMsg5Sprite, 0);
		ituSpriteGoto(SetCallRingMsg6Sprite, 0);
		ituSpriteGoto(SetCallRingMsg7Sprite, 1);
		ituSpriteGoto(SetCallRingMsg8Sprite, 0);
		break;

	case 7:
		ituSpriteGoto(SetCallRingMsg1Sprite, 0);
		ituSpriteGoto(SetCallRingMsg2Sprite, 0);
		ituSpriteGoto(SetCallRingMsg3Sprite, 0);
		ituSpriteGoto(SetCallRingMsg4Sprite, 0);
		ituSpriteGoto(SetCallRingMsg5Sprite, 0);
		ituSpriteGoto(SetCallRingMsg6Sprite, 0);
		ituSpriteGoto(SetCallRingMsg7Sprite, 0);
		ituSpriteGoto(SetCallRingMsg8Sprite, 1);
		break;

	default:
		break;
	}
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
	int value = atoi(param);

	g_page = value;
	SetRingPageShow(g_index[g_page]);
	g_index_ring_flag = g_index[g_page];

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
	ITUSprite* sprite[8] = { SetCallRingMsg1Sprite, SetCallRingMsg2Sprite, SetCallRingMsg3Sprite, SetCallRingMsg4Sprite,
							 SetCallRingMsg5Sprite, SetCallRingMsg6Sprite, SetCallRingMsg7Sprite, SetCallRingMsg8Sprite };
	int value = atoi(param);

	ituSpriteGoto(sprite[g_index_ring_flag], 0);
	g_index_ring_flag = (uint8)value;
	ituSpriteGoto(sprite[g_index_ring_flag], 1);
	//sys_start_play_audio(SYS_MEDIA_MUSIC, (char *)storage_get_ring_by_id(value), FALSE, storage_get_ringvolume(), NULL, NULL);

	return true;
}

/*************************************************
Function:		SetCallRingMsgYesButtonOnMouseUp
Description: 	来电铃声消息框确认 按钮
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetCallRingMsgYesButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 tmp[40];
	uint8 buf[40];
	ITUText* text[6] = { SetCallRingStair2Text, SetCallRingArea2Text, SetCallRingDoor2Text,
						 SetCallRingCenter2Text, SetCallRingRoom2Text, SetCallRingFenji2Text };

	g_index[g_page] = g_index_ring_flag;

	//sys_stop_play_audio(SYS_MEDIA_MUSIC);
	storage_set_ring_id(g_page, g_index[g_page]);

	memset(tmp, 0, sizeof(tmp));
	strcpy(tmp, get_str(SID_Set_Ring_Ring));
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d", g_index[g_page] + 1);
	strcat(tmp, buf);
	ituTextSetString(text[g_page], tmp);

	ituWidgetSetVisible(SetCallRingMsgBackground, false);
	ituWidgetEnable(SetRingtoneBackground);

	return true;
}

/*************************************************
Function:		SetCallRingMsgNoButtonOnMouseUp
Description: 	来电铃声消息框取消 按钮
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetCallRingMsgNoButtonOnMouseUp(ITUWidget* widget, char* param)
{
	//sys_stop_play_audio(SYS_MEDIA_MUSIC);

	ituWidgetSetVisible(SetCallRingMsgBackground, false);
	ituWidgetEnable(SetRingtoneBackground);

	return true;
}

/*************************************************
Function:		SetHitPageShow
Description: 	设置显示第几个
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void SetHitPageShow(uint8 index)
{
	switch (index)
	{
	case 0:
		ituSpriteGoto(SetRingtonePromptMsg1Sprite, 1);
		ituSpriteGoto(SetRingtonePromptMsg2Sprite, 0);
		ituSpriteGoto(SetRingtonePromptMsg3Sprite, 0);
		ituSpriteGoto(SetRingtonePromptMsg4Sprite, 0);
		break;

	case 1:
		ituSpriteGoto(SetRingtonePromptMsg1Sprite, 0);
		ituSpriteGoto(SetRingtonePromptMsg2Sprite, 1);
		ituSpriteGoto(SetRingtonePromptMsg3Sprite, 0);
		ituSpriteGoto(SetRingtonePromptMsg4Sprite, 0);
		break;

	case 2:
		ituSpriteGoto(SetRingtonePromptMsg1Sprite, 0);
		ituSpriteGoto(SetRingtonePromptMsg2Sprite, 0);
		ituSpriteGoto(SetRingtonePromptMsg3Sprite, 1);
		ituSpriteGoto(SetRingtonePromptMsg4Sprite, 0);
		break;

	case 3:
		ituSpriteGoto(SetRingtonePromptMsg1Sprite, 0);
		ituSpriteGoto(SetRingtonePromptMsg2Sprite, 0);
		ituSpriteGoto(SetRingtonePromptMsg3Sprite, 0);
		ituSpriteGoto(SetRingtonePromptMsg4Sprite, 1);
		break;

	default:
		break;
	}
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
	ituTextSetString(SetRingtonePromptMsgTitleText, get_str(SID_Set_Ring_Alarm));
	g_page = 7;
	SetHitPageShow(g_index[g_page]);
	g_index_hit_flag = g_index[g_page];

	ituWidgetDisable(SetRingtoneBackground);
	ituWidgetSetVisible(SetRingtonePromptMsgBackground, true);

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
	ituTextSetString(SetRingtonePromptMsgTitleText, get_str(SID_Set_Ring_Info));
	g_page = 6;
	SetHitPageShow(g_index[g_page]);
	g_index_hit_flag = g_index[g_page];

	ituWidgetDisable(SetRingtoneBackground);
	ituWidgetSetVisible(SetRingtonePromptMsgBackground, true);

	return true;
}

/*************************************************
Function:		SetRingtonePromptMsgButtonOnMouseUp
Description: 	信息提示音、预警提示音消息框按钮
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetRingtonePromptMsgButtonOnMouseUp(ITUWidget* widget, char* param)
{
	ITUSprite* sprite[4] = { SetRingtonePromptMsg1Sprite, SetRingtonePromptMsg2Sprite, SetRingtonePromptMsg3Sprite, SetRingtonePromptMsg4Sprite };
	int value = atoi(param);

	ituSpriteGoto(sprite[g_index_hit_flag], 0);
	g_index_hit_flag = (uint8)value;
	ituSpriteGoto(sprite[g_index_hit_flag], 1);

	if (6 == g_page)
	{
		//sys_start_play_audio(SYS_MEDIA_MUSIC, (char *)storage_get_msg_hit_byID(value), FALSE, storage_get_ringvolume(), NULL, NULL);
	}
	else if (7 == g_page)
	{
		//sys_start_play_audio(SYS_MEDIA_MUSIC, (char *)storage_get_yj_path_byID(value), FALSE, storage_get_ringvolume(), NULL, NULL);
	}

	return true;
}

/*************************************************
Function:		SetRingtonePromptMsgTureButtonOnMouseUp
Description: 	信息提示音、预警提示音消息框确认 按钮
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetRingtonePromptMsgTureButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 tmp[40];
	uint8 buf[40];

	g_index[g_page] = g_index_hit_flag;

	//sys_stop_play_audio(SYS_MEDIA_MUSIC);
	storage_set_ring_id(g_page, g_index[g_page]);

	memset(tmp, 0, sizeof(tmp));
	strcpy(tmp, get_str(SID_Set_Ring_Hit));
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d", g_index[g_page] + 1);
	strcat(tmp, buf);
	if (6 == g_page)
	{
		ituTextSetString(SetHitRingtone2Text, tmp);
	}
	else if (7 == g_page)
	{
		ituTextSetString(SetAlarmRingtone2Text, tmp);
	}

	ituWidgetSetVisible(SetRingtonePromptMsgBackground, false);
	ituWidgetEnable(SetRingtoneBackground);
	
	return true;
}

/*************************************************
Function:		SetRingtonePromptMsgFalseButtonOnMouseUp
Description: 	信息提示音、预警提示音消息框取消 按钮
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetRingtonePromptMsgFalseButtonOnMouseUp(ITUWidget* widget, char* param)
{
	//sys_stop_play_audio(SYS_MEDIA_MUSIC);

	ituWidgetSetVisible(SetRingtonePromptMsgBackground, false);
	ituWidgetEnable(SetRingtoneBackground);

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
	if (ituWidgetIsVisible(SetCallRingMsgBackground))
	{
		ituWidgetSetVisible(SetCallRingMsgBackground, false);
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

void SetRingToneLayerReset(void)
{
	SetRingtoneBackground = NULL;
}
