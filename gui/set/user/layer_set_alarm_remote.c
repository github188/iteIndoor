/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_alarm_remote.c
Author:     	zxc
Version:
Date: 		2016-07-06
Description: 安防远程控制界面
*************************************************/
#include "../layer_set.h"

static ITUSprite* SetAlarmRemoteSprite[3] = { NULL };
static ITUText* SetAlarmRemote2Text[3] = { NULL };

static uint8 g_remote;

/*************************************************
Function:		SetAlarmRemoteOnEnterShow
Description: 	进入远程控制设置界面显示函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void SetAlarmRemoteOnEnterShow()
{
	uint8 i = 0;
	uint16 text_id[3] = { SID_Bj_Remote_Home_Used, SID_Bj_Remote_Out_Used, SID_Bj_Remote_Night_Used };
	uint16 text_id1[3] = { SID_Bj_Remote_Home_Unused, SID_Bj_Remote_Out_Unused, SID_Bj_Remote_Night_Unused };

	g_remote = storage_get_remote_set();

	for (i = 0; i < 3; i++)
	{
		if (1 == (0x01 & (g_remote >> i)))
		{
			ituSpriteGoto(SetAlarmRemoteSprite[i], 1);
			ituTextSetString(SetAlarmRemote2Text[i], get_str(text_id[i]));
		}
		else
		{
			ituSpriteGoto(SetAlarmRemoteSprite[i], 0);
			ituTextSetString(SetAlarmRemote2Text[i], get_str(text_id1[i]));
		}
	}
}

/*************************************************
Function:		SetAlarmRemoteOnEnter
Description: 	进入远程控制设置界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetAlarmRemoteOnEnter(ITUWidget* widget, char* param)
{
	if (!SetAlarmRemoteSprite[0])
	{
		SetAlarmRemoteSprite[0] = ituSceneFindWidget(&theScene, "SetAlarmRemoteHomeSprite");
		assert(SetAlarmRemoteSprite[0]);

		SetAlarmRemoteSprite[1] = ituSceneFindWidget(&theScene, "SetAlarmRemoteOutSprite");
		assert(SetAlarmRemoteSprite[1]);

		SetAlarmRemoteSprite[2] = ituSceneFindWidget(&theScene, "SetAlarmRemoteNightSprite");
		assert(SetAlarmRemoteSprite[2]);

		SetAlarmRemote2Text[0] = ituSceneFindWidget(&theScene, "SetAlarmRemoteHome2Text");
		assert(SetAlarmRemote2Text[0]);

		SetAlarmRemote2Text[1] = ituSceneFindWidget(&theScene, "SetAlarmRemoteOut2Text");
		assert(SetAlarmRemote2Text[1]);

		SetAlarmRemote2Text[2] = ituSceneFindWidget(&theScene, "SetAlarmRemoteNight2Text");
		assert(SetAlarmRemote2Text[2]);
	}

	SetAlarmRemoteOnEnterShow();

	return true;
}

/*************************************************
Function:		SetAlarmRemoteHomeButtonOnMouseUp
Description: 	远程控制按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetAlarmRemoteHomeButtonOnMouseUp(ITUWidget* widget, char* param)
{
	int index = atoi(param);
	uint16 text_id[3] = { SID_Bj_Remote_Home_Used, SID_Bj_Remote_Out_Used, SID_Bj_Remote_Night_Used };
	uint16 text_id1[3] = { SID_Bj_Remote_Home_Unused, SID_Bj_Remote_Out_Unused, SID_Bj_Remote_Night_Unused };

	if (1 == ((g_remote >> (index)) & 0x01))
	{
		SET_ONE_VALUE(g_remote, 0, index);
		
		ituSpriteGoto(SetAlarmRemoteSprite[index], 0);
		ituTextSetString(SetAlarmRemote2Text[index], get_str(text_id1[index]));
	}
	else
	{
		SET_ONE_VALUE(g_remote, 1, index);

		ituSpriteGoto(SetAlarmRemoteSprite[index], 1);
		ituTextSetString(SetAlarmRemote2Text[index], get_str(text_id[index]));
	}
	storage_set_remote_set(g_remote);

	return true;
}
