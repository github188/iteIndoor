/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_personality_photoframe.c
Author:     	zxc
Version:
Date: 		2016-06-13
Description:	电子相框，设置关屏屏保时间
History:
1. Date:
Author:
Modification:
*************************************************/
#include "layer_set.h"

static ITUBackground* SetPersonalityPhotoFrameBackground = NULL;
static ITUBackground* SetCloseLcdTimeMsgBackground = NULL;
static ITUBackground* SetSaveLcdTimeMsgBackground = NULL;
static ITULayer* SetPersonalityLayer = NULL;				//个性设置界面
static ITUText* SetCloseLcd2Text = NULL;
static ITUText* SetLcdSave2Text = NULL;
static ITURadioBox* SetSaveLcdTimeMsgRadioBox[6] = { NULL };
static ITURadioBox* SetCloseLcdTimeMsgRadioBox[4] = { NULL };

static EPHOTO_TIME g_time;
static EPHOTO_PARAM g_param;

/*************************************************
Function:		SaveCloseLcdTimeText
Description: 	时间text显示
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void SaveCloseLcdTimeText(uint8 save_lcd, uint8 close_lcd)
{
	uint8 temp[40] = { 0 };
	int16 timeflag = 0;

	if (TRUE == save_lcd)
	{
		if (g_param.used)
		{
			memset(temp, 0, sizeof(temp));
			timeflag = storage_get_screen_intime();
			if (timeflag >= 60)
			{
				sprintf(temp, "%d%s", timeflag / 60, get_str(SID_Time_Min));
			}
			else
			{
				sprintf(temp, "%d%s", timeflag, get_str(SID_Time_Sec));
			}
			ituTextSetString(SetLcdSave2Text, temp);
		}
		else
		{
			ituTextSetString(SetLcdSave2Text, get_str(SID_Time_Never));
		}
	}
	
	if (TRUE == close_lcd)
	{
		memset(temp, 0, sizeof(temp));
		timeflag = storage_get_closelcd_time();
		sprintf(temp, "%d%s", timeflag / 60, get_str(SID_Time_Min));
		ituTextSetString(SetCloseLcd2Text, temp);
	}
}

/*************************************************
Function:		SetPersonalityPhotoFrameOnEnter
Description: 	进入电子相框界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetPersonalityPhotoFrameOnEnter(ITUWidget* widget, char* param)
{
	uint8 temp[40];
	int16 timeflag = 0;

	if (!SetPersonalityPhotoFrameBackground)
	{
		SetPersonalityPhotoFrameBackground = ituSceneFindWidget(&theScene, "SetPersonalityPhotoFrameBackground");
		assert(SetPersonalityPhotoFrameBackground);

		SetCloseLcdTimeMsgBackground = ituSceneFindWidget(&theScene, "SetCloseLcdTimeMsgBackground");
		assert(SetCloseLcdTimeMsgBackground);

		SetSaveLcdTimeMsgBackground = ituSceneFindWidget(&theScene, "SetSaveLcdTimeMsgBackground");
		assert(SetSaveLcdTimeMsgBackground);

		SetPersonalityLayer = ituSceneFindWidget(&theScene, "SetPersonalityLayer");
		assert(SetPersonalityLayer);

		SetCloseLcd2Text = ituSceneFindWidget(&theScene, "SetCloseLcd2Text");
		assert(SetCloseLcd2Text);

		SetLcdSave2Text = ituSceneFindWidget(&theScene, "SetLcdSave2Text");
		assert(SetLcdSave2Text);
	}

	memset(&g_param, 0, sizeof(EPHOTO_PARAM));
	memcpy(&g_param, (PEPHOTO_PARAM)storage_get_screenparam(), sizeof(EPHOTO_PARAM));

	SaveCloseLcdTimeText(TRUE, TRUE);
	
	ituWidgetSetVisible(SetCloseLcdTimeMsgBackground, false);
	ituWidgetSetVisible(SetSaveLcdTimeMsgBackground, false);
	ituWidgetSetVisible(SetPersonalityPhotoFrameBackground, true);
	if (!ituWidgetIsEnabled(SetPersonalityPhotoFrameBackground))
	{
		ituWidgetEnable(SetPersonalityPhotoFrameBackground);
	}

	return true;
}

/*************************************************
Function:		SetCloseLcdButtonOnMouseUp
Description: 	关屏时间设置按钮按下抬起执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetCloseLcdButtonOnMouseUp(ITUWidget* widget, char* param)
{
	if (!SetCloseLcdTimeMsgRadioBox[0])
	{
		char tmp[50];
		uint8 i = 0;

		for (i = 0; i < 4; i++)
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d%s", "SetCloseLcdTimeMsg", i+1, "RadioBox");
			SetCloseLcdTimeMsgRadioBox[i] = ituSceneFindWidget(&theScene, tmp);
			assert(SetCloseLcdTimeMsgRadioBox[i]);
		}
	}

	switch (g_param.holdtime)
	{
	case EPHOTO_TIME_5:
		ituRadioBoxSetChecked(SetCloseLcdTimeMsgRadioBox[0], true);
		break;

	case EPHOTO_TIME_10:
		ituRadioBoxSetChecked(SetCloseLcdTimeMsgRadioBox[1], true);
		break;

	case EPHOTO_TIME_30:
		ituRadioBoxSetChecked(SetCloseLcdTimeMsgRadioBox[2], true);
		break;

	case EPHOTO_TIME_60:
		ituRadioBoxSetChecked(SetCloseLcdTimeMsgRadioBox[3], true);
		break;

	default:
		ituRadioBoxSetChecked(SetCloseLcdTimeMsgRadioBox[0], true);
		break;
	}

	ituWidgetDisable(SetPersonalityPhotoFrameBackground);
	ituWidgetSetVisible(SetCloseLcdTimeMsgBackground, true);

	return true;
}

/*************************************************
Function:		SetLcdSaveButtonOnMouseUp
Description: 	进入屏保时间设置按钮按下抬起执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetLcdSaveButtonOnMouseUp(ITUWidget* widget, char* param)
{
	if (!SetSaveLcdTimeMsgRadioBox[0])
	{
		char tmp[50];
		uint8 i = 0;

		for (i = 0; i < 6; i++)
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d%s", "SetSaveLcdTimeMsg", i + 1, "RadioBox");
			SetSaveLcdTimeMsgRadioBox[i] = ituSceneFindWidget(&theScene, tmp);
			assert(SetSaveLcdTimeMsgRadioBox[i]);
		}
	}

	if (g_param.used)
	{
		switch (g_param.intime)
		{
		case EPHOTO_TIME_HALF:
			ituRadioBoxSetChecked(SetSaveLcdTimeMsgRadioBox[0], true);
			break;

		case EPHOTO_TIME_1:
			ituRadioBoxSetChecked(SetSaveLcdTimeMsgRadioBox[1], true);
			break;

		case EPHOTO_TIME_3:
			ituRadioBoxSetChecked(SetSaveLcdTimeMsgRadioBox[2], true);
			break;

		case EPHOTO_TIME_5:
			ituRadioBoxSetChecked(SetSaveLcdTimeMsgRadioBox[3], true);
			break;

		case EPHOTO_TIME_10:
			ituRadioBoxSetChecked(SetSaveLcdTimeMsgRadioBox[4], true);
			break;

		default:
			ituRadioBoxSetChecked(SetSaveLcdTimeMsgRadioBox[0], true);
			break;
		}
	}
	else
	{
		ituRadioBoxSetChecked(SetSaveLcdTimeMsgRadioBox[5], true);
	}

	ituWidgetDisable(SetPersonalityPhotoFrameBackground);
	ituWidgetSetVisible(SetSaveLcdTimeMsgBackground, true);

	return true;
}

/*************************************************
Function:		SetCloseLcdTimeMsgRadioBoxOnMouseUp
Description: 	关屏时间列表按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetCloseLcdTimeMsgRadioBoxOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 ret = TRUE;
	int index = atoi(param);

	switch (index)
	{
	case 0:
		g_param.holdtime = EPHOTO_TIME_5;
		break;

	case 1:
		g_param.holdtime = EPHOTO_TIME_10;
		break;

	case 2:
		g_param.holdtime = EPHOTO_TIME_30;
		break;

	case 3:
		g_param.holdtime = EPHOTO_TIME_60;
		break;

	default:
		g_param.holdtime = EPHOTO_TIME_5;
		break;
	}
	
	ret = storage_set_screenparam(&g_param);
	if (ret)
	{
		//InitScreenTimer();
	}

	SaveCloseLcdTimeText(FALSE, TRUE);

	ituWidgetSetVisible(SetCloseLcdTimeMsgBackground, false);
	ituWidgetEnable(SetPersonalityPhotoFrameBackground);
	
	return true;
}

/*************************************************
Function:		SetSaveLcdTimeMsgRadioBoxOnMouseUp
Description: 	屏保时间列表按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetSaveLcdTimeMsgRadioBoxOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 ret;
	int index = atoi(param);

	switch (index)
	{
	case 0:
		g_param.used = TRUE;
		g_param.intime = EPHOTO_TIME_HALF;
		break;

	case 1:
		g_param.used = TRUE;
		g_param.intime = EPHOTO_TIME_1;
		break;

	case 2:
		g_param.used = TRUE;
		g_param.intime = EPHOTO_TIME_3;
		break;

	case 3:
		g_param.used = TRUE;
		g_param.intime = EPHOTO_TIME_5;
		break;

	case 4:
		g_param.used = TRUE;
		g_param.intime = EPHOTO_TIME_10;
		break;

	case 5:
	default:
		g_param.used = FALSE;
		break;
	}

	ret = storage_set_screenparam(&g_param);
	if (ret)
	{
		//InitScreenTimer();
	}

	SaveCloseLcdTimeText(TRUE, FALSE);
	
	ituWidgetSetVisible(SetSaveLcdTimeMsgBackground, false);
	ituWidgetEnable(SetPersonalityPhotoFrameBackground);

	return true;
}

/*************************************************
Function:		SetPersonalityPhotoFrameLayerOnReturn
Description: 	底部退出键按下执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
void SetPersonalityPhotoFrameLayerOnReturn(void)
{
	if (ituWidgetIsVisible(SetCloseLcdTimeMsgBackground))
	{
		ituWidgetSetVisible(SetCloseLcdTimeMsgBackground, false);
		ituWidgetEnable(SetPersonalityPhotoFrameBackground);
		return;
	}
	else if (ituWidgetIsVisible(SetSaveLcdTimeMsgBackground))
	{
		ituWidgetSetVisible(SetSaveLcdTimeMsgBackground, false);
		ituWidgetEnable(SetPersonalityPhotoFrameBackground);
		return;
	}
	else if (!ituWidgetIsVisible(SetPersonalityLayer))
	{
		ituLayerGoto(SetPersonalityLayer);
		return;
	}
}
