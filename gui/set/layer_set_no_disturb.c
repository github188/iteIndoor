/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_no_disturb.c
Author:     	zxc
Version:
Date: 		2016-06-18
Description:
History:
1. Date:
Author:
Modification:
*************************************************/
#include "gui_include.h"

static ITUBackground* SetNoDisturbTimeBackground;
static ITUBackground* SetNoDisturbBackground;
static ITULayer* SetMenuLayer;
static ITUSprite* SetEnableNoDisturSprite;
static ITUText* SetNoDisturTimeBtn1Text;
static ITUText* SetNoDisturTimeBtn2Text; 
static ITUCalendar* SetNoDisturTimeBtnContainer; 
static ITURadioBox* SetNoDisturbTime0RadioBox;
static ITURadioBox* SetNoDisturbTime1RadioBox;
static ITURadioBox* SetNoDisturbTime2RadioBox;
static ITURadioBox* SetNoDisturbTime3RadioBox;
static ITURadioBox* SetNoDisturbTime4RadioBox;
static ITURadioBox* SetNoDisturbTime5RadioBox;

static ITULayer* SetNumKeyBordLayer;

static uint8 g_enable_no_disturb = FALSE;						//是否启用免打扰
static NOFACE_TIME g_no_disturb_time = NOFACE_TIME_30;		//免打扰时长

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
	char timetmp[80];
	char tmp[40];
	uint16 NofaceTime;

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

		SetNumKeyBordLayer = ituSceneFindWidget(&theScene, "SetNumKeyBordLayer");
		assert(SetNumKeyBordLayer);
	}

	g_enable_no_disturb = storage_get_noface();			//存储获取是否启用免打扰
	g_no_disturb_time = storage_get_noface_index();		//存储获取免打扰时长
	NofaceTime = storage_get_noface_time();

	printf("g_enable_no_disturb = %d, g_no_disturb_time=%d, NofaceTime=%d\n", g_enable_no_disturb, g_no_disturb_time, NofaceTime);
	
	if ((g_no_disturb_time > NOFACE_TIME_30) && (g_no_disturb_time < NOFACE_TIME_MAX))
	{
		memset(tmp, 0, sizeof(tmp));
		memset(timetmp, 0, sizeof(timetmp));
		sprintf(tmp, "%d", NofaceTime / 3600);
		strcpy(timetmp, tmp);
		memset(tmp, 0, sizeof(tmp));
		strcpy(tmp, get_str(SID_Time_Hour));
		strcat(timetmp, tmp);
	}
	else
	{
		memset(tmp, 0, sizeof(tmp));
		memset(timetmp, 0, sizeof(timetmp));
		sprintf(tmp, "%d", NofaceTime / 60);
		strcpy(timetmp, tmp);
		memset(tmp, 0, sizeof(tmp));
		strcpy(tmp, get_str(SID_Time_Min));
		strcat(timetmp, tmp);
	}
	ituTextSetString(SetNoDisturTimeBtn2Text, timetmp);

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
		//init_distrub();
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
		//init_distrub();
	}

	char timetmp[80];
	char tmp[40];
	uint16 NofaceTime; NofaceTime = storage_get_noface_time();
	if ((g_no_disturb_time > NOFACE_TIME_30) && (g_no_disturb_time < NOFACE_TIME_MAX))
	{
		memset(tmp, 0, sizeof(tmp));
		memset(timetmp, 0, sizeof(timetmp));
		sprintf(tmp, "%d", NofaceTime / 3600);
		strcpy(timetmp, tmp);
		memset(tmp, 0, sizeof(tmp));
		strcpy(tmp, get_str(SID_Time_Hour));
		strcat(timetmp, tmp);
	}
	else
	{
		memset(tmp, 0, sizeof(tmp));
		memset(timetmp, 0, sizeof(timetmp));
		sprintf(tmp, "%d", NofaceTime / 60);
		strcpy(timetmp, tmp);
		memset(tmp, 0, sizeof(tmp));
		strcpy(tmp, get_str(SID_Time_Min));
		strcat(timetmp, tmp);
	}
	ituTextSetString(SetNoDisturTimeBtn2Text, timetmp);

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

void SetNoDisturReset(void)
{
	SetNoDisturbBackground = NULL;
}
