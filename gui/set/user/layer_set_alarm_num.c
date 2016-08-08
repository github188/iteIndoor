/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_alarm_num.c
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

static ITUText* SetAlarmNum11Text = NULL;
static ITUText* SetAlarmNum12Text;
static ITUText* SetAlarmNum13Text;
static ITUText* SetAlarmNum21Text;
static ITUText* SetAlarmNum22Text;
static ITUText* SetAlarmNum23Text;
//static ITULayer* SetNumKeyBordLayer;
static ITUTextBox* SetNumKeyBordTextBox = NULL;

static uint8 g_button_flag = 0;

uint8 msg1[TELNUM_LEN + 1];
uint8 msg2[TELNUM_LEN + 1];

/*************************************************
Function:		SetAlarmNumOnEnter
Description: 	安防号码设置界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetAlarmNumOnEnter(ITUWidget* widget, char* param)
{
	if (!SetAlarmNum11Text)
	{
		SetAlarmNum11Text = ituSceneFindWidget(&theScene, "SetAlarmNum11Text");
		assert(SetAlarmNum11Text);

		SetAlarmNum12Text = ituSceneFindWidget(&theScene, "SetAlarmNum12Text");
		assert(SetAlarmNum12Text);

		SetAlarmNum13Text = ituSceneFindWidget(&theScene, "SetAlarmNum13Text");
		assert(SetAlarmNum13Text);

		SetAlarmNum21Text = ituSceneFindWidget(&theScene, "SetAlarmNum21Text");
		assert(SetAlarmNum21Text);

		SetAlarmNum22Text = ituSceneFindWidget(&theScene, "SetAlarmNum22Text");
		assert(SetAlarmNum22Text);

		SetAlarmNum23Text = ituSceneFindWidget(&theScene, "SetAlarmNum23Text");
		assert(SetAlarmNum23Text);

		//SetNumKeyBordLayer = ituSceneFindWidget(&theScene, "SetNumKeyBordLayer");
		//assert(SetNumKeyBordLayer);	

		SetNumKeyBordTextBox = ituSceneFindWidget(&theScene, "SetNumKeyBordTextBox");
		assert(SetNumKeyBordTextBox);
	}

	if (strcmp(param, "SetUserLayer") == 0)
	{
		memset(msg1, 0, TELNUM_LEN + 1);
		memset(msg2, 0, TELNUM_LEN + 1);
		storage_get_gsm_num(msg1, msg2);
	}
	else if (strcmp(param, "SetNumKeyBordLayer") == 0)
	{
		char* TextBox_data = ituTextGetString(SetNumKeyBordTextBox);

		if (1 == g_button_flag)
		{
			memset(msg1, 0, TELNUM_LEN + 1);
			strcpy(msg1, TextBox_data);
		}
		else if (2 == g_button_flag)
		{
			memset(msg2, 0, TELNUM_LEN + 1);
			strcpy(msg2, TextBox_data);
		}

		storage_set_gsm_num(msg1, msg2);
	}

	if (0 == strlen(msg1))
	{
		ituWidgetSetVisible(SetAlarmNum11Text, false);
		ituWidgetSetVisible(SetAlarmNum12Text, false);
		ituWidgetSetVisible(SetAlarmNum13Text, true);
	}
	else
	{
		ituTextSetString(SetAlarmNum12Text, msg1);
		ituWidgetSetVisible(SetAlarmNum11Text, true);
		ituWidgetSetVisible(SetAlarmNum12Text, true);
		ituWidgetSetVisible(SetAlarmNum13Text, false);
	}

	if (0 == strlen(msg2))
	{
		ituWidgetSetVisible(SetAlarmNum21Text, false);
		ituWidgetSetVisible(SetAlarmNum22Text, false);
		ituWidgetSetVisible(SetAlarmNum23Text, true);
	}
	else
	{
		ituTextSetString(SetAlarmNum22Text, msg2);
		ituWidgetSetVisible(SetAlarmNum21Text, true);
		ituWidgetSetVisible(SetAlarmNum22Text, true);
		ituWidgetSetVisible(SetAlarmNum23Text, false);
	}

	return true;
}

/*************************************************
Function:		SetAlarmNumButtonOnMouseUp
Description: 	安防号码设置按钮按下抬起执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetAlarmNumButtonOnMouseUp(ITUWidget* widget, char* param)
{
	int index = atoi(param);
	uint8 tmp[TELNUM_LEN + 1];

	g_button_flag = (uint8)index;

	if (1 == g_button_flag)
	{
		strcpy(tmp, msg1);
	}
	else
	{
		strcpy(tmp, msg2);
	}

	KeybordLayerOnShow(NULL, PASS_TYPE_MAX, TELNUM_LEN, EXPRESS_CHAR, CANCEL_BTN, tmp);

	return true;
}

void SetAlarmNumeReset(void)
{
	SetAlarmNum11Text = NULL;
}
