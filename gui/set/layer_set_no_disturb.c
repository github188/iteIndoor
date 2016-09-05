/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_no_disturb.c
Author:     	zxc
Version:
Date: 		2016-06-18
Description:免打扰设置界面
History:
1. Date:
Author:
Modification:
*************************************************/
#include "layer_set.h"

static ITUBackground* SetNoDisturbTimeBackground = NULL;
static ITUBackground* SetNoDisturbBackground = NULL;
static ITULayer* SetMenuLayer = NULL;
static ITUSprite* SetEnableNoDisturSprite = NULL;
static ITUText* SetNoDisturTimeBtn1Text = NULL;
static ITUText* SetNoDisturTimeBtn2Text = NULL;
static ITUCalendar* SetNoDisturTimeBtnContainer = NULL;
static ITURadioBox* SetNoDisturbTime0RadioBox = NULL;
static ITURadioBox* SetNoDisturbTime1RadioBox = NULL;
static ITURadioBox* SetNoDisturbTime2RadioBox = NULL;
static ITURadioBox* SetNoDisturbTime3RadioBox = NULL;
static ITURadioBox* SetNoDisturbTime4RadioBox = NULL;
static ITURadioBox* SetNoDisturbTime5RadioBox = NULL;

static uint8 g_enable_no_disturb = FALSE;						//是否启用免打扰
static uint8 g_no_disturb_time = NOFACE_TIME_30;				//免打扰时长

/*************************************************
Function:		SetNoDisturbOnEnter
Description: 	进入免打扰设置界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool ShowListTime()
{
	uint32 NofaceTime = 0;
	char timetmp[80];

	NofaceTime = storage_get_noface_time();
	memset(timetmp, 0, sizeof(timetmp));

	if ((g_no_disturb_time > NOFACE_TIME_30) && (g_no_disturb_time < NOFACE_TIME_MAX))
	{
		sprintf(timetmp, "%d%s", NofaceTime / 3600, get_str(SID_Time_Hour));
	}
	else
	{
		sprintf(timetmp, "%d%s", NofaceTime / 60, get_str(SID_Time_Min));
	}
	ituTextSetString(SetNoDisturTimeBtn2Text, timetmp);
}

/*************************************************
Function:		SetNoDisturbOnEnter
Description: 	进入免打扰设置界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetNoDisturbOnEnter(ITUWidget* widget, char* param)
{
	if (!SetNoDisturbBackground)
	{
		SetNoDisturbBackground = ituSceneFindWidget(&theScene, "SetNoDisturbBackground");
		assert(SetNoDisturbBackground);

		SetNoDisturbTimeBackground = ituSceneFindWidget(&theScene, "SetNoDisturbTimeBackground");
		assert(SetNoDisturbTimeBackground);

		SetMenuLayer = ituSceneFindWidget(&theScene, "SetMenuLayer");
		assert(SetMenuLayer);

		SetEnableNoDisturSprite = ituSceneFindWidget(&theScene, "SetEnableNoDisturSprite");
		assert(SetEnableNoDisturSprite); 

		SetNoDisturTimeBtn1Text = ituSceneFindWidget(&theScene, "SetNoDisturTimeBtn1Text");
		assert(SetNoDisturTimeBtn1Text);

		SetNoDisturTimeBtn2Text = ituSceneFindWidget(&theScene, "SetNoDisturTimeBtn2Text");
		assert(SetNoDisturTimeBtn2Text); 

		SetNoDisturTimeBtnContainer = ituSceneFindWidget(&theScene, "SetNoDisturTimeBtnContainer");
		assert(SetNoDisturTimeBtnContainer);

		SetNoDisturbTime0RadioBox = ituSceneFindWidget(&theScene, "SetNoDisturbTime0RadioBox");
		assert(SetNoDisturbTime0RadioBox);

		SetNoDisturbTime1RadioBox = ituSceneFindWidget(&theScene, "SetNoDisturbTime1RadioBox");
		assert(SetNoDisturbTime1RadioBox);

		SetNoDisturbTime2RadioBox = ituSceneFindWidget(&theScene, "SetNoDisturbTime2RadioBox");
		assert(SetNoDisturbTime2RadioBox);

		SetNoDisturbTime3RadioBox = ituSceneFindWidget(&theScene, "SetNoDisturbTime3RadioBox");
		assert(SetNoDisturbTime3RadioBox); 

		SetNoDisturbTime4RadioBox = ituSceneFindWidget(&theScene, "SetNoDisturbTime4RadioBox");
		assert(SetNoDisturbTime4RadioBox);

		SetNoDisturbTime5RadioBox = ituSceneFindWidget(&theScene, "SetNoDisturbTime5RadioBox");
		assert(SetNoDisturbTime5RadioBox);
	}

	g_enable_no_disturb = storage_get_noface();			//存储获取是否启用免打扰
	g_no_disturb_time = storage_get_noface_index();		//存储获取免打扰时长
	ShowListTime();

	if (g_enable_no_disturb)
	{
		ituSpriteGoto(SetEnableNoDisturSprite, 1);
		ituSetColor(&((ITUWidget*)SetNoDisturTimeBtn1Text)->color, 255, 255, 255, 255);
		ituSetColor(&((ITUWidget*)SetNoDisturTimeBtn2Text)->color, 255, 255, 255, 255);
	}
	else
	{
		ituSpriteGoto(SetEnableNoDisturSprite, 0);
		ituSetColor(&((ITUWidget*)SetNoDisturTimeBtn1Text)->color, 255, 128, 138, 135);//冷灰
		ituSetColor(&((ITUWidget*)SetNoDisturTimeBtn2Text)->color, 255, 192, 192, 192);//灰色
		ituWidgetDisable(SetNoDisturTimeBtnContainer);
	}

	ituWidgetSetVisible(SetNoDisturbTimeBackground, false);
	ituWidgetSetVisible(SetNoDisturbBackground, true);
	if (!ituWidgetIsEnabled(SetNoDisturbBackground))
	{
		ituWidgetEnable(SetNoDisturbBackground);
	}
	
	return true;
}

/*************************************************
Function:		SetEnableNoDisturButtonOnMouseUp
Description: 	启用免打扰按钮按下抬起执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetEnableNoDisturButtonOnMouseUp(ITUWidget* widget, char* param)
{
	if (g_enable_no_disturb)
	{
		g_enable_no_disturb = FALSE;
		ituSpriteGoto(SetEnableNoDisturSprite, 0);
		ituSetColor(&((ITUWidget*)SetNoDisturTimeBtn1Text)->color, 255, 128, 138, 135);
		ituSetColor(&((ITUWidget*)SetNoDisturTimeBtn2Text)->color, 255, 192, 192, 192);
		ituWidgetDisable(SetNoDisturTimeBtnContainer);
	}
	else
	{
		g_enable_no_disturb = TRUE;
		ituSpriteGoto(SetEnableNoDisturSprite, 1);
		ituSetColor(&((ITUWidget*)SetNoDisturTimeBtn1Text)->color, 255, 255, 255, 255);
		ituSetColor(&((ITUWidget*)SetNoDisturTimeBtn2Text)->color, 255, 255, 255, 255);
		ituWidgetEnable(SetNoDisturTimeBtnContainer);
	}

	uint8 ret = TRUE;
	ret = storage_set_noface(g_enable_no_disturb, g_no_disturb_time);
	if (ret)
	{
		start_disturb_timer();
	}
	
	return true;
}

/*************************************************
Function:		SetNoDisturTimeBtnButtonOnMouseUp
Description: 	免打扰时间设置按钮按下抬起执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetNoDisturTimeBtnButtonOnMouseUp(ITUWidget* widget, char* param)
{
	if (g_enable_no_disturb)
	{
		switch (g_no_disturb_time)
		{
		case NOFACE_TIME_30:
			ituRadioBoxSetChecked(SetNoDisturbTime0RadioBox, true);
			break;

		case NOFACE_TIME_60:
			ituRadioBoxSetChecked(SetNoDisturbTime1RadioBox, true);
			break;

		case NOFACE_TIME_120:
			ituRadioBoxSetChecked(SetNoDisturbTime2RadioBox, true);
			break;

		case NOFACE_TIME_180:
			ituRadioBoxSetChecked(SetNoDisturbTime3RadioBox, true);
			break;

		case NOFACE_TIME_300:
			ituRadioBoxSetChecked(SetNoDisturbTime4RadioBox, true);
			break;

		case NOFACE_TIME_480:
			ituRadioBoxSetChecked(SetNoDisturbTime5RadioBox, true);
			break;

		default:
			ituRadioBoxSetChecked(SetNoDisturbTime0RadioBox, true);
			break;
		}
		
		ituWidgetDisable(SetNoDisturbBackground);
		ituWidgetSetVisible(SetNoDisturbTimeBackground, true);
	}

	return true;
}

/*************************************************
Function:		SetNoDisturTimeBtnButtonOnMouseUp
Description: 	免打扰时间设置按钮按下抬起执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetNoDisturbTimeRadioBoxOnMouseUp(ITUWidget* widget, char* param)
{
	if (ituRadioBoxIsChecked(SetNoDisturbTime0RadioBox))
	{
		g_no_disturb_time = NOFACE_TIME_30;
	}
	else if (ituRadioBoxIsChecked(SetNoDisturbTime1RadioBox))
	{
		g_no_disturb_time = NOFACE_TIME_60;
	}
	else if (ituRadioBoxIsChecked(SetNoDisturbTime2RadioBox))
	{
		g_no_disturb_time = NOFACE_TIME_120;
	}
	else if (ituRadioBoxIsChecked(SetNoDisturbTime3RadioBox))
	{
		g_no_disturb_time = NOFACE_TIME_180;
	}
	else if (ituRadioBoxIsChecked(SetNoDisturbTime4RadioBox))
	{
		g_no_disturb_time = NOFACE_TIME_300;
	}
	else if (ituRadioBoxIsChecked(SetNoDisturbTime5RadioBox))
	{
		g_no_disturb_time = NOFACE_TIME_480;
	}
	else
	{
		g_no_disturb_time = NOFACE_TIME_30;
	}

	uint8 ret = TRUE;
	ret = storage_set_noface(g_enable_no_disturb, g_no_disturb_time);
	if (ret)
	{
		start_disturb_timer();
	}

	ShowListTime();

	ituWidgetSetVisible(SetNoDisturbTimeBackground, false);
	ituWidgetEnable(SetNoDisturbBackground);

	return true;
}

/*************************************************
Function:		SetNoDisturLayerOnReturn
Description: 	底部退出键按下执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
void SetNoDisturLayerOnReturn(void)
{
	if (ituWidgetIsVisible(SetNoDisturbTimeBackground))
	{
		ituWidgetSetVisible(SetNoDisturbTimeBackground, false);
		ituWidgetEnable(SetNoDisturbBackground);
		return;
	}
	else if (!ituWidgetIsVisible(SetMenuLayer))
	{
		ituLayerGoto(SetMenuLayer);
		return;
	}
}
