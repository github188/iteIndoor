/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_user.c
Author:     	zxc
Version:
Date: 		2016-07-06
Description: 用户设置界面
*************************************************/
#include "../layer_set.h"

static ITUCoverFlow* SetUserCoverFlow = NULL;
static ITUText* SetYujingTime2Text = NULL;
static ITURadioBox* SetYujingTimeMsgRadioBox[5] = { NULL };
static ITUBackground* SetYujingTimeMsgBackground = NULL;
static ITUBackground* SetUserBackground = NULL;
static ITULayer* SetMenuLayer = NULL;
//static ITULayer* SetAlarmAreaListLayer;

static uint8 g_time_alarm_param[3] = { 0 };

/*************************************************
Function:		ShowSetYujingTime2Text
Description: 	显示退出预警时间
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void ShowSetYujingTime2Text(uint8 timeflag)
{
	uint8 tmp[20];
	uint8 timetmp[40];
	uint16 exit_time = 0;

	exit_time = get_time_for_timetype(OUT_SOUND_TYPE, g_time_alarm_param[1]);

	if ((timeflag > 0) && (timeflag < 5))
	{
		memset(tmp, 0, sizeof(tmp));
		memset(timetmp, 0, sizeof(timetmp));
		sprintf(tmp, "%d", exit_time / 60);
		strcpy(timetmp, tmp);
		memset(tmp, 0, sizeof(tmp));
		strcpy(tmp, get_str(SID_Time_Min));
		strcat(timetmp, tmp);
	}
	else
	{
		memset(tmp, 0, sizeof(tmp));
		memset(timetmp, 0, sizeof(timetmp));
		sprintf(tmp, "%d", exit_time);
		strcpy(timetmp, tmp);
		memset(tmp, 0, sizeof(tmp));
		strcpy(tmp, get_str(SID_Time_Sec));
		strcat(timetmp, tmp);
	}
	ituTextSetString(SetYujingTime2Text, timetmp);
}

/*************************************************
Function:		SetUserOnEnter
Description: 	进入用户设置界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetUserOnEnter(ITUWidget* widget, char* param)
{
	if (!SetUserBackground)
	{
		SetUserBackground = ituSceneFindWidget(&theScene, "SetUserBackground");
		assert(SetUserBackground);

		SetYujingTimeMsgBackground = ituSceneFindWidget(&theScene, "SetYujingTimeMsgBackground");
		assert(SetYujingTimeMsgBackground);

		SetUserCoverFlow = ituSceneFindWidget(&theScene, "SetUserCoverFlow");
		assert(SetUserCoverFlow);

		SetYujingTime2Text = ituSceneFindWidget(&theScene, "SetYujingTime2Text");
		assert(SetYujingTime2Text);

		SetMenuLayer = ituSceneFindWidget(&theScene, "SetMenuLayer");
		assert(SetMenuLayer); 

		//SetAlarmAreaListLayer = ituSceneFindWidget(&theScene, "SetAlarmAreaListLayer");
		//assert(SetAlarmAreaListLayer);
	}

	if (strcmp(param, "SetNumKeyBordLayer") == 0)
	{
		ituCoverFlowGoto(SetUserCoverFlow, 0);
	}

	storage_get_time_param(g_time_alarm_param);

	ShowSetYujingTime2Text(g_time_alarm_param[1]);
	
	ituWidgetSetVisible(SetYujingTimeMsgBackground, false);
	ituWidgetSetVisible(SetUserBackground, true);
	if (!ituWidgetIsEnabled(SetUserBackground))
	{
		ituWidgetEnable(SetUserBackground);
	}

	return true;
}

/*************************************************
Function:		SetYujingTimeButtonOnMouseUp
Description: 	退出预警时间按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetYujingTimeButtonOnMouseUp(ITUWidget* widget, char* param)
{
	if (!SetYujingTimeMsgRadioBox[0])
	{
		uint8 i = 0;
		uint8 tmp[50] = { 0 };

		for (i = 0; i < 5; i++)
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d%s", "SetYujingTimeMsg", i, "RadioBox");
			SetYujingTimeMsgRadioBox[i] = ituSceneFindWidget(&theScene, tmp);
			assert(SetYujingTimeMsgRadioBox[i]);
		}
	}

	ituRadioBoxSetChecked(SetYujingTimeMsgRadioBox[g_time_alarm_param[1]], true);

	ituWidgetDisable(SetUserBackground);
	ituWidgetSetVisible(SetYujingTimeMsgBackground, true);
	
	return true;
}

/*************************************************
Function:		SetYujingTimeMsgRadioBoxOnMouseUp
Description: 	退出预警时间列表按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetYujingTimeMsgRadioBoxOnMouseUp(ITUWidget* widget, char* param)
{
	int index = atoi(param);

	g_time_alarm_param[1] = (uint8)index;

	storage_set_time_param(g_time_alarm_param);

	ShowSetYujingTime2Text(g_time_alarm_param[1]);

	ituWidgetSetVisible(SetYujingTimeMsgBackground, false);
	ituWidgetEnable(SetUserBackground);

	return true;
}

/*************************************************
Function:		SetGeliAndJufangButtonOnMouseUp
Description: 	局防有效、安防隔离按下处理
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetGeliAndJufangButtonOnMouseUp(ITUWidget* widget, char* param)
{
	int index = atoi(param);

	SetAlarmAreaList((uint8)index);

	return true;
}

/*************************************************
Function:		SetUserLayerOnReturn
Description: 	底部退出键按下执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
void SetUserLayerOnReturn(void)
{
	if (ituWidgetIsVisible(SetYujingTimeMsgBackground))
	{
		ituWidgetSetVisible(SetYujingTimeMsgBackground, false);
		ituWidgetEnable(SetUserBackground);
		return;
	}
	else if (!ituWidgetIsVisible(SetMenuLayer))
	{
		ituLayerGoto(SetMenuLayer);
		return;
	}
}
