/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_volume.c
Author:     	zxc
Version:
Date: 		2016-06-17
Description:
History:
1. Date:
Author:
Modification:
*************************************************/
#include "gui_include.h"

static ITUBackground* SetVolumeBackground;
static ITUBackground* SetVolumeMsgBackground;
static ITULayer* SetMenuLayer;				
static ITUText* SetKeypadTone2Text;
static ITUSprite* SetKeypadToneSprite; 
static ITUText* SetVolumeMsgTitleText;
static ITUProgressBar* SetVolumeMsgProgressBar;
static ITUTrackBar* SetVolumeMsgTrackBar;

static uint8 g_index[3];

static char g_set_volume_size_page = 0;		//0：设置通话音量大小消息框页面   1：设置振铃音量大小消息框页面
static int g_talk_volume_size_flag = 0;			
static int g_ring_volume_size_flag = 0;			

/*************************************************
Function:		SetVolumeOnEnter
Description: 	进入音量设置界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetVolumeOnEnter(ITUWidget* widget, char* param)
{
	if (!SetVolumeBackground)
	{
		SetVolumeBackground = ituSceneFindWidget(&theScene, "SetVolumeBackground");
		assert(SetVolumeBackground);

		SetVolumeMsgBackground = ituSceneFindWidget(&theScene, "SetVolumeMsgBackground");
		assert(SetVolumeMsgBackground);

		SetMenuLayer = ituSceneFindWidget(&theScene, "SetMenuLayer");
		assert(SetMenuLayer);

		SetKeypadTone2Text = ituSceneFindWidget(&theScene, "SetKeypadTone2Text");
		assert(SetKeypadTone2Text);

		SetKeypadToneSprite = ituSceneFindWidget(&theScene, "SetKeypadToneSprite");
		assert(SetKeypadToneSprite);

		SetVolumeMsgTitleText = ituSceneFindWidget(&theScene, "SetVolumeMsgTitleText");
		assert(SetVolumeMsgTitleText); 

		SetVolumeMsgProgressBar = ituSceneFindWidget(&theScene, "SetVolumeMsgProgressBar");
		assert(SetVolumeMsgProgressBar); 

		SetVolumeMsgTrackBar = ituSceneFindWidget(&theScene, "SetVolumeMsgTrackBar");
		assert(SetVolumeMsgTrackBar);
	}

	g_index[0] = storage_get_ringvolume();
	g_index[1] = storage_get_talkvolume();
	g_index[2] = storage_get_keykeep();

	if (0 == g_index[2])
	{
		ituTextSetString(SetKeypadTone2Text, get_str(SID_Set_KeyTone_OFF));
		ituSpriteGoto(SetKeypadToneSprite, 0);
	}
	else
	{
		ituTextSetString(SetKeypadTone2Text, get_str(SID_Set_KeyTone_ON));
		ituSpriteGoto(SetKeypadToneSprite, 1);
	}

	return true;
}

/*************************************************
Function:		SetTalkVolumeButtonOnMouseUp
Description: 	通话音量设置按钮按下抬起执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetTalkVolumeButtonOnMouseUp(ITUWidget* widget, char* param)
{
	ituTextSetString(SetVolumeMsgTitleText, get_str(SID_Set_Talk_Volume));
	g_set_volume_size_page = 0;
	ituProgressBarSetValue(SetVolumeMsgProgressBar, g_index[1]);
	ituTrackBarSetValue(SetVolumeMsgTrackBar, g_index[1]);
	ituWidgetDisable(SetVolumeBackground);
	ituWidgetSetVisible(SetVolumeMsgBackground, true);

	return true;
}

/*************************************************
Function:		SetRingingVolumeButtonOnMouseUp
Description: 	振铃音量设置按钮按下抬起执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetRingingVolumeButtonOnMouseUp(ITUWidget* widget, char* param)
{
	ituTextSetString(SetVolumeMsgTitleText, get_str(SID_Set_Ring_Volume));
	g_set_volume_size_page = 1; 
	ituProgressBarSetValue(SetVolumeMsgProgressBar, g_index[0]);
	ituTrackBarSetValue(SetVolumeMsgTrackBar, g_index[0]);
	ituWidgetDisable(SetVolumeBackground);
	ituWidgetSetVisible(SetVolumeMsgBackground, true);

	return true;
}

/*************************************************
Function:		SetKeypadToneButtonOnMouseUp
Description: 	启用按键音设置按钮按下抬起执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetKeypadToneButtonOnMouseUp(ITUWidget* widget, char* param)
{
	if (0 == g_index[2])
	{
		g_index[2] = 1;
		ituTextSetString(SetKeypadTone2Text, get_str(SID_Set_KeyTone_ON));
		ituSpriteGoto(SetKeypadToneSprite, 1);
	}
	else if (1 == g_index[2])
	{
		g_index[2] = 0;
		ituTextSetString(SetKeypadTone2Text, get_str(SID_Set_KeyTone_OFF));
		ituSpriteGoto(SetKeypadToneSprite, 0);
	}
	storage_set_volume(g_index[0], g_index[1], g_index[2]);

	return true;
}

/*************************************************
Function:		SetVolumeMsgProgressBarOnChanged
Description: 	音量调节函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetVolumeMsgProgressBarOnChanged(ITUWidget* widget, char* param)
{
	if (0 == g_set_volume_size_page)
	{
		g_talk_volume_size_flag = atoi(param);
	}
	else if (1 == g_set_volume_size_page)
	{
		g_ring_volume_size_flag = atoi(param);
	}

	return true;
}

/*************************************************
Function:		SetVolumeMSGTureButtonOnMouseUp
Description: 	确认按钮按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetVolumeMSGTureButtonOnMouseUp(ITUWidget* widget, char* param)
{
	if (0 == g_set_volume_size_page)
	{
		g_index[1] = g_talk_volume_size_flag;
	}
	else if (1 == g_set_volume_size_page)
	{
		g_index[0] = g_ring_volume_size_flag;
	}

	storage_set_volume(g_index[0], g_index[1], g_index[2]);

	if (ituWidgetIsVisible(SetVolumeMsgBackground))
	{
		ituWidgetSetVisible(SetVolumeMsgBackground, false);
		ituWidgetEnable(SetVolumeBackground);
	}

	return true;
}

/*************************************************
Function:		SetVolumeLayerOnReturn
Description: 	底部退出键按下执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
void SetVolumeLayerOnReturn(void)
{
	if (ituWidgetIsVisible(SetVolumeMsgBackground))
	{
		ituWidgetSetVisible(SetVolumeMsgBackground, false);
		ituWidgetEnable(SetVolumeBackground);
		return;
	}
	else if (!ituWidgetIsVisible(SetMenuLayer))
	{
		ituLayerGoto(SetMenuLayer);
		return;
	}
}

void SetVolumeReset(void)
{
	SetVolumeBackground = NULL;
}
