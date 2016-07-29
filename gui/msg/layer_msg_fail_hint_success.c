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
	
	ituWidgetShow(g_GotoLayer, ITU_EFFECT_NONE, 0);

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
		ituLayerGoto(g_GotoLayer);
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
*************************************************/
void ShowMsgFailHintSuccessLayer(uint8 spriteflag, uint32 id)
{
	if (!MsgFailHintSuccessLayer)
	{
		MsgFailHintSuccessLayer = ituSceneFindWidget(&theScene, "MsgFailHintSuccessLayer");
		assert(MsgFailHintSuccessLayer);

		MsgFailHintSuccessSprite = ituSceneFindWidget(&theScene, "MsgFailHintSuccessSprite");
		assert(MsgFailHintSuccessSprite);

		MsgFailHintSuccessText = ituSceneFindWidget(&theScene, "MsgFailHintSuccessText");
		assert(MsgFailHintSuccessText);
	}

	ituSpriteGoto(MsgFailHintSuccessSprite, spriteflag);
	ituTextSetString(MsgFailHintSuccessText, get_str(id));

	if (!ituWidgetIsVisible(MsgFailHintSuccessLayer))
	{
		ituLayerGoto(MsgFailHintSuccessLayer);
	}
}
