/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_msg_fail_hint_success.c
Author:     	zxc
Version:
Date: 		2016-06-15
Description:
Modification:消息框提示错误、警告、成功页面
*************************************************/
#include "../layer_set.h"

static ITULayer* g_GotoLayer = NULL;						
static ITUSprite* MsgFailHintSuccessSprite = NULL;
static ITUText* MsgFailHintSuccessText = NULL;
static ITULayer* MsgFailHintSuccessLayer = NULL;
static ITUCalendar* MsgFailHintSuccess0Calendar = NULL;
static ITUCalendar* MsgFailHintSuccess1Calendar = NULL;
static ITURadioBox* MsgFailHintSuccess0RadioBox = NULL;
static ITURadioBox* MsgFailHintSuccess1RadioBox = NULL;
static ITULayer* SetBottonReturnLayer = NULL;

#if 0
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
#endif

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
	old_layer：进入时的界面
*************************************************/
void ShowMsgFailHintSuccessLayer(HIT_ICON_TYPE spriteflag, uint32 id, char* old_layer)
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

		SetBottonReturnLayer = ituSceneFindWidget(&theScene, "SetBottonReturnLayer");
		assert(SetBottonReturnLayer);
	}

	ituSpriteGoto(MsgFailHintSuccessSprite, spriteflag);
	ituTextSetString(MsgFailHintSuccessText, get_str(id));
	if (HIT_SPRITE_TO_WARNNING == spriteflag)
	{
		ituWidgetSetVisible(MsgFailHintSuccess0Calendar, false);
		ituWidgetSetVisible(MsgFailHintSuccess1Calendar, true);
	}
	else
	{
		ituWidgetSetVisible(MsgFailHintSuccess1Calendar, false);
		ituWidgetSetVisible(MsgFailHintSuccess0Calendar, true);
	}

	ituRadioBoxSetChecked(MsgFailHintSuccess0RadioBox, false);
	ituRadioBoxSetChecked(MsgFailHintSuccess1RadioBox, false);

	g_GotoLayer = ituSceneFindWidget(&theScene, old_layer);
	assert(g_GotoLayer);

	ituWidgetDisable(g_GotoLayer);
	ituWidgetShow(MsgFailHintSuccessLayer, ITU_EFFECT_NONE, 0);	// 提示框界面使用show出，不是goto
	SetBottonReturnOnReload(MsgFailHintSuccessLayer, NULL);
	ituWidgetShow(SetBottonReturnLayer, ITU_EFFECT_NONE, 0);
}
