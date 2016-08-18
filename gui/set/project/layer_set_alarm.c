/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_alarm.c
Author:     	zxc
Version:
Date: 		2016-07-09
Description: 安防设置界面
*************************************************/
#include "../layer_set.h"

static ITUText* SetAlarmTime2Text = NULL;
static ITURadioBox* SetAlarmTimeMsgRadioBox[5] = { NULL };
static ITUBackground* SetAlarmBackground = NULL;
static ITUBackground* SetAlarmTimeMsgBackground = NULL;
static ITULayer* SetProjectLayer = NULL;

static uint8 g_time_alarm_param[3] = { 0 };

/*************************************************
Function:		ShowSetYujingTime2Text
Description: 	显示退出预警时间
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void ShowSetAlarmTime2Text(uint8 timeflag)
{
	uint8 tmp[20];
	uint8 timetmp[40];
	uint16 exit_time = 0;

	exit_time = get_time_for_timetype(ALARM_SOUND_TYPE, g_time_alarm_param[0]);

	memset(tmp, 0, sizeof(tmp));
	memset(timetmp, 0, sizeof(timetmp));
	sprintf(tmp, "%d", exit_time / 60);
	strcpy(timetmp, tmp);
	memset(tmp, 0, sizeof(tmp));
	strcpy(tmp, get_str(SID_Time_Min));
	strcat(timetmp, tmp);
	
	ituTextSetString(SetAlarmTime2Text, timetmp);
}

/*************************************************
Function:		SetAlarmOnEnter
Description: 	进入用户设置界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetAlarmOnEnter(ITUWidget* widget, char* param)
{
	if (!SetAlarmTime2Text)
	{
		SetAlarmTime2Text = ituSceneFindWidget(&theScene, "SetAlarmTime2Text");
		assert(SetAlarmTime2Text);

		SetAlarmBackground = ituSceneFindWidget(&theScene, "SetAlarmBackground");
		assert(SetAlarmBackground);

		SetAlarmTimeMsgBackground = ituSceneFindWidget(&theScene, "SetAlarmTimeMsgBackground");
		assert(SetAlarmTimeMsgBackground);

		SetProjectLayer = ituSceneFindWidget(&theScene, "SetProjectLayer");
		assert(SetProjectLayer);
	}

	storage_get_time_param(g_time_alarm_param);

	ShowSetAlarmTime2Text(g_time_alarm_param[0]);

	ituWidgetSetVisible(SetAlarmTimeMsgBackground, false);
	ituWidgetSetVisible(SetAlarmBackground, true);
	if (!ituWidgetIsEnabled(SetAlarmBackground))
	{
		ituWidgetEnable(SetAlarmBackground);
	}

	return true;
}

/*************************************************
Function:		SetAlarmTimeButtonOnMouseUp
Description: 	报警时间按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetAlarmTimeButtonOnMouseUp(ITUWidget* widget, char* param)
{
	if (!SetAlarmTimeMsgRadioBox[0])
	{
		uint8 textname[50];
		uint8 i = 0;
		for (i = 0; i < 5; i++)
		{
			memset(textname, 0, sizeof(textname));
			sprintf(textname, "%s%d%s", "SetAlarmTimeMsg", i, "RadioBox");
			SetAlarmTimeMsgRadioBox[i] = ituSceneFindWidget(&theScene, textname);
			assert(SetAlarmTimeMsgRadioBox[i]);
		}
	}

	if ((g_time_alarm_param[0] < 5) || (g_time_alarm_param[0] >= 0))
	{
		ituRadioBoxSetChecked(SetAlarmTimeMsgRadioBox[g_time_alarm_param[0]], true);
	}
	else
	{
		ituRadioBoxSetChecked(SetAlarmTimeMsgRadioBox[0], true);
	}

	ituWidgetDisable(SetAlarmBackground);
	ituWidgetSetVisible(SetAlarmTimeMsgBackground, true);

	return true;
}

/*************************************************
Function:		SetAlarmTimeMsgRadioBoxOnMouseUp
Description: 	报警时间列表按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetAlarmTimeMsgRadioBoxOnMouseUp(ITUWidget* widget, char* param)
{
	int index = atoi(param);

	g_time_alarm_param[0] = (uint8)index;

	storage_set_time_param(g_time_alarm_param);

	ShowSetAlarmTime2Text(g_time_alarm_param[0]);

	ituWidgetSetVisible(SetAlarmTimeMsgBackground, false);
	ituWidgetEnable(SetAlarmBackground);

	return true;
}

/*************************************************
Function:		SetAlarmParamButtonOnMouseUp
Description: 	安防属性按下
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetAlarmParamButtonOnMouseUp(ITUWidget* widget, char* param)
{
	SetAlarmAreaList(3);

	return true;
}


/*************************************************
Function:		SetAlarmLayerOnReturn
Description: 	底部退出键按下执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
void SetAlarmLayerOnReturn(void)
{
	if (ituWidgetIsVisible(SetAlarmTimeMsgBackground))
	{
		ituWidgetSetVisible(SetAlarmTimeMsgBackground, false);
		ituWidgetEnable(SetAlarmBackground);
		return;
	}
	else if (!ituWidgetIsVisible(SetProjectLayer))
	{
		ituLayerGoto(SetProjectLayer);
		return;
	}
}
