/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_msg_fail_hint_success.c
Author:     	zxc
Version:
Date: 		2016-06-15
Description:
Modification:消息框提示错误、警告、成功页面
*************************************************/
#include "gui_include.h"

static ITULayer* g_GotoLayer = NULL;						
static ITUSprite* MsgFailHintSuccessSprite = NULL;
static ITUText* MsgFailHintSuccessText = NULL;
static ITULayer* MsgFailHintSuccessLayer = NULL;
static ITUCalendar* MsgFailHintSuccess0Calendar = NULL;
static ITUCalendar* MsgFailHintSuccess1Calendar = NULL;
static ITURadioBox* MsgFailHintSuccess0RadioBox = NULL;
static ITURadioBox* MsgFailHintSuccess1RadioBox = NULL;

/*************************************************
Function:		MsgFailHintSuccessOnEnter
Description: 	消息框提示错误、警告、成功初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool MsgFailHintSuccessOnEnter(ITUWidget* widget, char* param)
{
	g_GotoLayer = ituSceneFindWidget(&theScene, param);
	assert(g_GotoLayer);
	
	ituRadioBoxSetChecked(MsgFailHintSuccess0RadioBox, false);
	ituRadioBoxSetChecked(MsgFailHintSuccess1RadioBox, false);

	ituWidgetShow(g_GotoLayer, ITU_EFFECT_NONE, 0);
	ituWidgetDisable(g_GotoLayer);

	return true;
}

/*************************************************
Function:		SetBottonReturnButtonOnPress
Description: 	确认键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool MsgFailHintSuccessButtonOnMouseUp(ITUWidget* widget, char* param)
{
	if (ituWidgetIsVisible(MsgFailHintSuccessLayer))
	{
		ituWidgetEnable(g_GotoLayer);
		ituLayerGoto(g_GotoLayer);
	}
	return true;
}

/*************************************************
Function:		MsgFailHintSuccessLayerOnLeave
Description: 	
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool MsgFailHintSuccessLayerOnLeave(ITUWidget* widget, char* param)
{
	if (!ituWidgetIsEnabled(g_GotoLayer))
	{
		ituWidgetEnable(g_GotoLayer);
	}
	return true;
}

/*************************************************
Function:		ShowMsgFailHintSuccessLayer
Description: 	调用消息框处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
param:
	spriteflag: 图标显示第几个	0：“X”	1：“！”	2：“√”
	id：		显示字的ID
	calendarflag:	显示一个按键还是两个按键	0:确认；	1：确认，取消
*************************************************/
void ShowMsgFailHintSuccessLayer(uint8 spriteflag, uint32 id, uint8 calendarflag)
{
	if (!MsgFailHintSuccessLayer)
	{
		MsgFailHintSuccessLayer = ituSceneFindWidget(&theScene, "MsgFailHintSuccessLayer");
		assert(MsgFailHintSuccessLayer);

		MsgFailHintSuccessSprite = ituSceneFindWidget(&theScene, "MsgFailHintSuccessSprite");
		assert(MsgFailHintSuccessSprite);

		MsgFailHintSuccessText = ituSceneFindWidget(&theScene, "MsgFailHintSuccessText");
		assert(MsgFailHintSuccessText);

		MsgFailHintSuccess0Calendar = ituSceneFindWidget(&theScene, "MsgFailHintSuccess0Calendar");
		assert(MsgFailHintSuccess0Calendar);

		MsgFailHintSuccess1Calendar = ituSceneFindWidget(&theScene, "MsgFailHintSuccess1Calendar");
		assert(MsgFailHintSuccess1Calendar);

		MsgFailHintSuccess0RadioBox = ituSceneFindWidget(&theScene, "MsgFailHintSuccess0RadioBox");
		assert(MsgFailHintSuccess0RadioBox);

		MsgFailHintSuccess1RadioBox = ituSceneFindWidget(&theScene, "MsgFailHintSuccess1RadioBox");
		assert(MsgFailHintSuccess1RadioBox);
	}

	ituSpriteGoto(MsgFailHintSuccessSprite, spriteflag);
	ituTextSetString(MsgFailHintSuccessText, get_str(id));
	if (0 == calendarflag)
	{
		ituWidgetSetVisible(MsgFailHintSuccess1Calendar, false);
		ituWidgetSetVisible(MsgFailHintSuccess0Calendar, true);
	}
	else
	{
		ituWidgetSetVisible(MsgFailHintSuccess0Calendar, false);
		ituWidgetSetVisible(MsgFailHintSuccess1Calendar, true);
	}

	if (!ituWidgetIsVisible(MsgFailHintSuccessLayer))
	{
		ituLayerGoto(MsgFailHintSuccessLayer);
	}
}
