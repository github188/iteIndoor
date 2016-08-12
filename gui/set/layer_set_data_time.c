/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_menu.c
Author:     	zxc
Version:
Date: 		2016-06-13
Description:
History:
1. Date:
Author:
Modification:
*************************************************/
#include "gui_include.h"

static ITUBackground* SetDataTimeBackground;
static ITUBackground* SetDataMsgBackground;
static ITUBackground* SetTimeMsgBackground;
static ITULayer* SetMenuLayer;
static ITUWheel* SetDataYearWheel;
static ITUWheel* SetDataMonthWheel;
static ITUWheel* SetDataDayWheel;
static ITUText* SetData2Text;
static ITUText* SetTime2Text;
static ITUWheel* SetTimeHourWheel;
static ITUWheel* SetTimeMinWheel;

static DATE_TIME g_time;

/*************************************************
Function:		SetDataTimeOnEnter
Description: 	进入时间和日期设置界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetDataTimeOnEnter(ITUWidget* widget, char* param)
{
	char str[20];

	if (!SetDataTimeBackground)
	{
		SetDataTimeBackground = ituSceneFindWidget(&theScene, "SetDataTimeBackground");
		assert(SetDataTimeBackground);

		SetDataMsgBackground = ituSceneFindWidget(&theScene, "SetDataMsgBackground");
		assert(SetDataMsgBackground);

		SetTimeMsgBackground = ituSceneFindWidget(&theScene, "SetTimeMsgBackground");
		assert(SetTimeMsgBackground);

		SetMenuLayer = ituSceneFindWidget(&theScene, "SetMenuLayer");
		assert(SetMenuLayer);

		SetDataYearWheel = ituSceneFindWidget(&theScene, "SetDataYearWheel");
		assert(SetDataYearWheel);

		SetDataMonthWheel = ituSceneFindWidget(&theScene, "SetDataMonthWheel");
		assert(SetDataMonthWheel);

		SetDataDayWheel = ituSceneFindWidget(&theScene, "SetDataDayWheel");
		assert(SetDataDayWheel);

		SetData2Text = ituSceneFindWidget(&theScene, "SetData2Text");
		assert(SetData2Text);

		SetTime2Text = ituSceneFindWidget(&theScene, "SetTime2Text");
		assert(SetTime2Text);

		SetTimeHourWheel = ituSceneFindWidget(&theScene, "SetTimeHourWheel");
		assert(SetTimeHourWheel);

		SetTimeMinWheel = ituSceneFindWidget(&theScene, "SetTimeMinWheel");
		assert(SetTimeMinWheel);
	}

	get_timer(&g_time);

	printf("%4d-%02d-%02d %02d:%02d:%02d\n", (g_time.year), (g_time.month), g_time.day, g_time.hour, g_time.min, g_time.sec);

	memset(str, 0, sizeof(str));
	sprintf(str, "%4d-%02d-%02d", g_time.year, g_time.month, g_time.day);
	ituTextSetString(SetData2Text, str);
	memset(str, 0, sizeof(str));
	sprintf(str, "%02d:%02d", g_time.hour, g_time.min);
	ituTextSetString(SetTime2Text, str);

	ituWidgetSetVisible(SetDataMsgBackground, false); 
	ituWidgetSetVisible(SetTimeMsgBackground, false);
	ituWidgetSetVisible(SetDataTimeBackground, true);
	
	return true;
}

/*************************************************
Function:		SetDataButtonOnMouseUp
Description: 	设置日期按钮按下抬起执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetDataButtonOnMouseUp(ITUWidget* widget, char* param)
{
	ituWheelGoto(SetDataDayWheel, g_time.day - 1);
	ituWheelGoto(SetDataMonthWheel, g_time.month - 1);
	ituWheelGoto(SetDataYearWheel, g_time.year - 2000);

	ituWidgetDisable(SetDataTimeBackground);
	ituWidgetSetVisible(SetDataMsgBackground, true);
	
	return true;
}

/*************************************************
Function:		SetDataMsgSetOKButtonOnMouseUp
Description: 	设置日期确认按钮按下抬起执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetDataMsgSetOKButtonOnMouseUp(ITUWidget* widget, char* param)
{
	g_time.day = SetDataDayWheel->focusIndex + 1;
	g_time.month = SetDataMonthWheel->focusIndex + 1;
	g_time.year = SetDataYearWheel->focusIndex + 2000;

	set_rtc_timer(&g_time);

	char str[20];
	memset(str, 0, sizeof(str));
	sprintf(str, "%4d-%02d-%02d", g_time.year, g_time.month, g_time.day);
	ituTextSetString(SetData2Text, str);

	ituWidgetSetVisible(SetDataMsgBackground, false);
	ituWidgetEnable(SetDataTimeBackground);

	return true;
}

/*************************************************
Function:		SetTimeButtonOnMouseUp
Description: 	设置时间按钮按下抬起执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetTimeButtonOnMouseUp(ITUWidget* widget, char* param)
{
	ituWheelGoto(SetTimeHourWheel, g_time.hour);
	ituWheelGoto(SetTimeMinWheel, g_time.min);

	ituWidgetDisable(SetDataTimeBackground);
	ituWidgetSetVisible(SetTimeMsgBackground, true);

	return true;
}

/*************************************************
Function:		SetTimeMsgOKButtonOnMouseUp
Description: 	设置时间确认按钮按下抬起执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetTimeMsgOKButtonOnMouseUp(ITUWidget* widget, char* param)
{
	g_time.hour = SetTimeHourWheel->focusIndex;
	g_time.min = SetTimeMinWheel->focusIndex;

	set_rtc_timer(&g_time);

	char str[20];
	memset(str, 0, sizeof(str));
	sprintf(str, "%02d:%02d", g_time.hour, g_time.min);
	ituTextSetString(SetTime2Text, str);

	ituWidgetSetVisible(SetTimeMsgBackground, false);
	ituWidgetEnable(SetDataTimeBackground);

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
void SetDataTimeLayerOnReturn(void)
{
	if (ituWidgetIsVisible(SetDataMsgBackground))
	{
		ituWidgetSetVisible(SetDataMsgBackground, false);
		ituWidgetEnable(SetDataTimeBackground);
		return;
	}
	else if (ituWidgetIsVisible(SetTimeMsgBackground))
	{
		ituWidgetSetVisible(SetTimeMsgBackground, false);
		ituWidgetEnable(SetDataTimeBackground);
		return;
	}
	else if (!ituWidgetIsVisible(SetMenuLayer))
	{
		ituLayerGoto(SetMenuLayer);
		return;
	}
}

void SetDataTimeReset(void)
{
	SetDataTimeBackground = NULL;
}