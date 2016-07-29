/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_personality_photoframe.c
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

static ITUBackground* SetPersonalityPhotoFrameBackground;
static ITUBackground* SetCloseLcdTimeMsgBackground;
static ITUBackground* SetSaveLcdTimeMsgBackground;
static ITULayer* SetPersonalityLayer;				//个性设置界面
static ITUText* SetCloseLcd2Text;
static ITUText* SetLcdSave2Text;
static ITURadioBox* SetSaveLcdTimeMsg1RadioBox;
static ITURadioBox* SetSaveLcdTimeMsg2RadioBox;
static ITURadioBox* SetSaveLcdTimeMsg3RadioBox;
static ITURadioBox* SetSaveLcdTimeMsg4RadioBox;
static ITURadioBox* SetSaveLcdTimeMsg5RadioBox;
static ITURadioBox* SetSaveLcdTimeMsg6RadioBox;
static ITURadioBox* SetCloseLcdTimeMsg1RadioBox;
static ITURadioBox* SetCloseLcdTimeMsg2RadioBox;
static ITURadioBox* SetCloseLcdTimeMsg3RadioBox;
static ITURadioBox* SetCloseLcdTimeMsg4RadioBox;

static EPHOTO_TIME g_time;
static EPHOTO_PARAM g_param;

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

		SetSaveLcdTimeMsg1RadioBox = ituSceneFindWidget(&theScene, "SetSaveLcdTimeMsg1RadioBox");
		assert(SetSaveLcdTimeMsg1RadioBox);

		SetSaveLcdTimeMsg2RadioBox = ituSceneFindWidget(&theScene, "SetSaveLcdTimeMsg2RadioBox");
		assert(SetSaveLcdTimeMsg2RadioBox);

		SetSaveLcdTimeMsg3RadioBox = ituSceneFindWidget(&theScene, "SetSaveLcdTimeMsg3RadioBox");
		assert(SetSaveLcdTimeMsg3RadioBox);

		SetSaveLcdTimeMsg4RadioBox = ituSceneFindWidget(&theScene, "SetSaveLcdTimeMsg4RadioBox");
		assert(SetSaveLcdTimeMsg4RadioBox);

		SetSaveLcdTimeMsg5RadioBox = ituSceneFindWidget(&theScene, "SetSaveLcdTimeMsg5RadioBox");
		assert(SetSaveLcdTimeMsg5RadioBox);

		SetSaveLcdTimeMsg6RadioBox = ituSceneFindWidget(&theScene, "SetSaveLcdTimeMsg6RadioBox");
		assert(SetSaveLcdTimeMsg6RadioBox); 

		SetCloseLcdTimeMsg1RadioBox = ituSceneFindWidget(&theScene, "SetCloseLcdTimeMsg1RadioBox");
		assert(SetCloseLcdTimeMsg1RadioBox);

		SetCloseLcdTimeMsg2RadioBox = ituSceneFindWidget(&theScene, "SetCloseLcdTimeMsg2RadioBox");
		assert(SetCloseLcdTimeMsg2RadioBox);

		SetCloseLcdTimeMsg3RadioBox = ituSceneFindWidget(&theScene, "SetCloseLcdTimeMsg3RadioBox");
		assert(SetCloseLcdTimeMsg3RadioBox);

		SetCloseLcdTimeMsg4RadioBox = ituSceneFindWidget(&theScene, "SetCloseLcdTimeMsg4RadioBox");
		assert(SetCloseLcdTimeMsg4RadioBox); 
	}

	memset(&g_param, 0, sizeof(EPHOTO_PARAM));
	memcpy(&g_param, (PEPHOTO_PARAM)storage_get_screenparam(), sizeof(EPHOTO_PARAM));

	printf("g_param.used %d, g_param.intime %d,  g_param.holdtime %d\n", g_param.used, g_param.intime, g_param.holdtime);

	if (g_param.used)
	{
		memset(temp, 0, sizeof(temp));
		timeflag = storage_get_screen_intime();
		if (timeflag >= 60)
		{
			sprintf(temp, "%d", timeflag / 60);
			strcat(temp, get_str(SID_Time_Min));
		}
		else
		{
			sprintf(temp, "%d", timeflag);
			strcat(temp, get_str(SID_Time_Sec));
		}
		ituTextSetString(SetLcdSave2Text, temp);
	}
	else
	{
		ituTextSetString(SetLcdSave2Text, get_str(SID_Time_Never));
	}
	memset(temp, 0, sizeof(temp));
	timeflag = storage_get_closelcd_time();
	sprintf(temp, "%d", timeflag/60);
	strcat(temp, get_str(SID_Time_Min));
	ituTextSetString(SetCloseLcd2Text, temp);
	
	ituWidgetSetVisible(SetCloseLcdTimeMsgBackground, false);
	ituWidgetSetVisible(SetSaveLcdTimeMsgBackground, false);
	ituWidgetSetVisible(SetPersonalityPhotoFrameBackground, true);

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
	switch (g_param.holdtime)
	{
	case EPHOTO_TIME_5:
		ituRadioBoxSetChecked(SetCloseLcdTimeMsg1RadioBox, true);
		break;

	case EPHOTO_TIME_10:
		ituRadioBoxSetChecked(SetCloseLcdTimeMsg2RadioBox, true);
		break;

	case EPHOTO_TIME_30:
		ituRadioBoxSetChecked(SetCloseLcdTimeMsg3RadioBox, true);
		break;

	case EPHOTO_TIME_60:
		ituRadioBoxSetChecked(SetCloseLcdTimeMsg4RadioBox, true);
		break;

	default:
		ituRadioBoxSetChecked(SetCloseLcdTimeMsg1RadioBox, true);
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
	if (g_param.used)
	{
		switch (g_param.intime)
		{
		case EPHOTO_TIME_HALF:
			ituRadioBoxSetChecked(SetSaveLcdTimeMsg1RadioBox, true);
			break;

		case EPHOTO_TIME_1:
			ituRadioBoxSetChecked(SetSaveLcdTimeMsg2RadioBox, true);
			break;

		case EPHOTO_TIME_3:
			ituRadioBoxSetChecked(SetSaveLcdTimeMsg3RadioBox, true);
			break;

		case EPHOTO_TIME_5:
			ituRadioBoxSetChecked(SetSaveLcdTimeMsg4RadioBox, true);
			break;

		case EPHOTO_TIME_10:
			ituRadioBoxSetChecked(SetSaveLcdTimeMsg5RadioBox, true);
			break;

		default:
			ituRadioBoxSetChecked(SetSaveLcdTimeMsg1RadioBox, true);
			break;
		}
	}
	else
	{
		ituRadioBoxSetChecked(SetSaveLcdTimeMsg6RadioBox, true);
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
	uint8 temp[40];
	int16 timeflag = 0;

	if (ituRadioBoxIsChecked(SetCloseLcdTimeMsg1RadioBox))
	{
		g_param.holdtime = EPHOTO_TIME_5;
	}
	else if (ituRadioBoxIsChecked(SetCloseLcdTimeMsg2RadioBox))
	{
		g_param.holdtime = EPHOTO_TIME_10;
	}
	else if (ituRadioBoxIsChecked(SetCloseLcdTimeMsg3RadioBox))
	{
		g_param.holdtime = EPHOTO_TIME_30;
	}
	else if (ituRadioBoxIsChecked(SetCloseLcdTimeMsg4RadioBox))
	{
		g_param.holdtime = EPHOTO_TIME_60;
	}
	
	ret = storage_set_screenparam(&g_param);
	if (ret)
	{
		//InitScreenTimer();
	}

	memset(temp, 0, sizeof(temp));
	timeflag = storage_get_closelcd_time();
	sprintf(temp, "%d", timeflag/60);
	strcat(temp, get_str(SID_Time_Min));
	ituTextSetString(SetCloseLcd2Text, temp);

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
	uint8 temp[40];
	int16 timeflag = 0;

	if (ituRadioBoxIsChecked(SetSaveLcdTimeMsg1RadioBox))
	{
		g_param.used = TRUE;
		g_param.intime = EPHOTO_TIME_HALF;
	}
	else if (ituRadioBoxIsChecked(SetSaveLcdTimeMsg2RadioBox))
	{
		g_param.used = TRUE;
		g_param.intime = EPHOTO_TIME_1;
	}
	else if (ituRadioBoxIsChecked(SetSaveLcdTimeMsg3RadioBox))
	{
		g_param.used = TRUE;
		g_param.intime = EPHOTO_TIME_3;
	}
	else if (ituRadioBoxIsChecked(SetSaveLcdTimeMsg4RadioBox))
	{
		g_param.used = TRUE;
		g_param.intime = EPHOTO_TIME_5;
	}
	else if (ituRadioBoxIsChecked(SetSaveLcdTimeMsg5RadioBox))
	{
		g_param.used = TRUE;
		g_param.intime = EPHOTO_TIME_10;
	}
	else if (ituRadioBoxIsChecked(SetSaveLcdTimeMsg6RadioBox))
	{
		g_param.used = FALSE;
	}

	ret = storage_set_screenparam(&g_param);
	if (ret)
	{
		//InitScreenTimer();
	}

	if (g_param.used)
	{
		memset(temp, 0, sizeof(temp));
		timeflag = storage_get_screen_intime();
		if (timeflag >= 60)
		{
			sprintf(temp, "%d", timeflag / 60);
			strcat(temp, get_str(SID_Time_Min));
		}
		else
		{
			sprintf(temp, "%d", timeflag);
			strcat(temp, get_str(SID_Time_Sec));
		}
		ituTextSetString(SetLcdSave2Text, temp);
	}
	else
	{
		ituTextSetString(SetLcdSave2Text, get_str(SID_Time_Never));
	}
	
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

void SetPersonalityPhotoFrameLayerReset(void)
{
	SetPersonalityPhotoFrameBackground = NULL;
}
