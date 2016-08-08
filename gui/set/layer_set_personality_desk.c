/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_personality_desk.c
Author:     	zxc
Version:
Date: 		2016-07-31
Description:	设置桌面背景
*************************************************/
#include "layer_set.h"

#define MAX_DESK_INDEX			8

static ITUBackground* SetDeskPicBackground = NULL;
static ITUBackground* SetPersonalityPreviewBackground = NULL;
static ITURadioBox* SetPersonalityDeskRadioBox[MAX_DESK_INDEX] = { NULL };
static ITUIcon* SetPersonalityDeskIcon[MAX_DESK_INDEX] = { NULL };
static ITUIcon* SetPersonalityPreviewDownIcon = NULL;
static ITUSprite* SetPersonalityPreviewUpSprite = NULL;

static uint8 g_desk_index = 0;

/*************************************************
Function:		show_win
Description: 	
Input:		无
Output:		无
Return:		
Others:
*************************************************/
static void show_win()
{
	ituRadioBoxSetChecked(SetPersonalityDeskRadioBox[g_desk_index], true);

}

/*************************************************
Function:		SetPersonalityDeskOnEnter
Description: 	进入桌面设置界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetPersonalityDeskOnEnter(ITUWidget* widget, char* param)
{
	if (!SetDeskPicBackground)
	{
		char tmp[50];
		uint8 i = 0;

		for (i = 0; i < MAX_DESK_INDEX; i++)
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "SetPersonalityDeskIcon", i);
			SetPersonalityDeskIcon[i] = ituSceneFindWidget(&theScene, tmp);
			assert(SetPersonalityDeskIcon[i]);

			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "SetPersonalityDeskRadioBox", i);
			SetPersonalityDeskRadioBox[i] = ituSceneFindWidget(&theScene, tmp);
			assert(SetPersonalityDeskRadioBox[i]);
		}

		SetDeskPicBackground = ituSceneFindWidget(&theScene, "SetDeskPicBackground");
		assert(SetDeskPicBackground);

		SetPersonalityPreviewBackground = ituSceneFindWidget(&theScene, "SetPersonalityPreviewBackground");
		assert(SetPersonalityPreviewBackground);

		SetPersonalityPreviewUpSprite = ituSceneFindWidget(&theScene, "SetPersonalityPreviewUpSprite");
		assert(SetPersonalityPreviewUpSprite);
	}

	g_desk_index = storage_get_desk();

	show_win();

	ituWidgetSetVisible(SetPersonalityPreviewBackground, false);
	ituWidgetSetVisible(SetDeskPicBackground, true);

	return true;
}

/*************************************************
Function:		SetPersonalityDeskRadioBoxOnMouseUp
Description: 	桌面图片按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetPersonalityDeskRadioBoxOnMouseUp(ITUWidget* widget, char* param)
{
	int index = atoi(param);
	g_desk_index = (uint8)index;

	return true;
}

/*************************************************
Function:		RightPreviewButtonOnMouseUp
Description: 	预览按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool RightPreviewButtonOnMouseUp(ITUWidget* widget, char* param)
{

	
	ituSpriteGoto(SetPersonalityPreviewUpSprite, 0);
	ituWidgetSetVisible(SetDeskPicBackground, false);
	ituWidgetSetVisible(SetPersonalityPreviewBackground, true);

	return true;
}

/*************************************************
Function:		RightSetDeskButtonOnMouseUp
Description: 	设为桌面背景按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool RightSetDeskButtonOnMouseUp(ITUWidget* widget, char* param)
{
	storage_set_desk(TRUE, g_desk_index);

	return true;
}

/*************************************************
Function:		SetPersonalityPreviewButtonOnMouseUp
Description: 	退出预览按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetPersonalityPreviewButtonOnMouseUp(ITUWidget* widget, char* param)
{
	ituWidgetSetVisible(SetPersonalityPreviewBackground, false);
	ituWidgetSetVisible(SetDeskPicBackground, true);

	return true;
}
