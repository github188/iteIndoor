/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_alarm_remote.c
Author:     	zxc
Version:
Date: 		2016-07-06
Description:
*************************************************/
#include "gui_include.h"

#define SET_ONE_VALUE(val,state,pos) ((0==state)?(val&=(~(1<<pos))):(val|=(1<<pos)))

static ITUSprite* SetAlarmRemoteHomeSprite;
static ITUSprite* SetAlarmRemoteOutSprite;
static ITUSprite* SetAlarmRemoteNightSprite;
static ITUText* SetAlarmRemoteHome2Text;
static ITUText* SetAlarmRemoteOut2Text;
static ITUText* SetAlarmRemoteNight2Text;

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
	ITUSprite* sprite[3] = { SetAlarmRemoteHomeSprite, SetAlarmRemoteOutSprite, SetAlarmRemoteNightSprite };
	ITUText* text[3] = { SetAlarmRemoteHome2Text, SetAlarmRemoteOut2Text, SetAlarmRemoteNight2Text };
	uint16 text_id[3] = { SID_Bj_Remote_Home_Used, SID_Bj_Remote_Out_Used, SID_Bj_Remote_Night_Used };
	uint16 text_id1[3] = { SID_Bj_Remote_Home_Unused, SID_Bj_Remote_Out_Unused, SID_Bj_Remote_Night_Unused };

	g_remote = storage_get_remote_set();

	for (i = 0; i < 3; i++)
	{
		if (1 == (0x01 & (g_remote >> i)))
		{
			ituSpriteGoto(sprite[i], 1);
			ituTextSetString(text[i], get_str(text_id[i]));
		}
		else
		{
			ituSpriteGoto(sprite[i], 0);
			ituTextSetString(text[i], get_str(text_id1[i]));
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
	if (!SetAlarmRemoteHomeSprite)
	{
		SetAlarmRemoteHomeSprite = ituSceneFindWidget(&theScene, "SetAlarmRemoteHomeSprite");
		assert(SetAlarmRemoteHomeSprite);

		SetAlarmRemoteOutSprite = ituSceneFindWidget(&theScene, "SetAlarmRemoteOutSprite");
		assert(SetAlarmRemoteOutSprite);

		SetAlarmRemoteNightSprite = ituSceneFindWidget(&theScene, "SetAlarmRemoteNightSprite");
		assert(SetAlarmRemoteNightSprite);

		SetAlarmRemoteHome2Text = ituSceneFindWidget(&theScene, "SetAlarmRemoteHome2Text");
		assert(SetAlarmRemoteHome2Text);

		SetAlarmRemoteOut2Text = ituSceneFindWidget(&theScene, "SetAlarmRemoteOut2Text");
		assert(SetAlarmRemoteOut2Text);

		SetAlarmRemoteNight2Text = ituSceneFindWidget(&theScene, "SetAlarmRemoteNight2Text");
		assert(SetAlarmRemoteNight2Text);
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
	ITUSprite* sprite[3] = { SetAlarmRemoteHomeSprite, SetAlarmRemoteOutSprite, SetAlarmRemoteNightSprite };
	ITUText* text[3] = { SetAlarmRemoteHome2Text, SetAlarmRemoteOut2Text, SetAlarmRemoteNight2Text };
	uint16 text_id[3] = { SID_Bj_Remote_Home_Used, SID_Bj_Remote_Out_Used, SID_Bj_Remote_Night_Used };
	uint16 text_id1[3] = { SID_Bj_Remote_Home_Unused, SID_Bj_Remote_Out_Unused, SID_Bj_Remote_Night_Unused };

	if (1 == ((g_remote >> (index)) & 0x01))
	{
		SET_ONE_VALUE(g_remote, 0, index);
		
		ituSpriteGoto(sprite[index], 0);
		ituTextSetString(text[index], get_str(text_id1[index]));
	}
	else
	{
		SET_ONE_VALUE(g_remote, 1, index);

		ituSpriteGoto(sprite[index], 1);
		ituTextSetString(text[index], get_str(text_id[index]));
	}
	storage_set_remote_set(g_remote);

	return true;
}

void SetAlarmRemoteLayerReset(void)
{
	SetAlarmRemoteHomeSprite = NULL;
}