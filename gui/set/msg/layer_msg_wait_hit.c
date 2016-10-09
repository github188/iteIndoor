/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_msg_wait_hit.c
Author:     	zxc
Version:
Date: 		2016-09-26
Description:
Modification:等待提示框
*************************************************/
#include "../layer_set.h"

static ITULayer* MsgWaitHitLayer = NULL; 
static ITULayer* g_GotoLayer = NULL;
static ITULayer* SetBottonReturnLayer = NULL;

static uint32 g_TimerClock = 0;
static uint8 g_open_timer = FALSE;

/*************************************************
Function:		MsgWaitHitLayerOnHide
Description: 	退出处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
void MsgWaitHitLayerOnHide()
{
	ituWidgetHide(MsgWaitHitLayer, ITU_EFFECT_NONE, 0);

	SetBottonReturnOnReload(g_GotoLayer, NULL);

	if (g_GotoLayer)
	{
		if (!ituWidgetIsEnabled(g_GotoLayer))
		{
			ituWidgetEnable(g_GotoLayer);
		}
	}
}

/*************************************************
Function:		MsgLinkOutTimeOnShow
Description: 	
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
void MsgWaitHitLayerOnShow(char* old_layer)
{
	if (!MsgWaitHitLayer)
	{
		MsgWaitHitLayer = ituSceneFindWidget(&theScene, "MsgWaitHitLayer");
		assert(MsgWaitHitLayer);

		SetBottonReturnLayer = ituSceneFindWidget(&theScene, "SetBottonReturnLayer");
		assert(SetBottonReturnLayer);
	}

	g_GotoLayer = ituSceneFindWidget(&theScene, old_layer);
	assert(g_GotoLayer);

	g_TimerClock = SDL_GetTicks();
	ituWidgetDisable(g_GotoLayer);
	ituWidgetShow(MsgWaitHitLayer, ITU_EFFECT_NONE, 0);	// 提示框界面使用show出，不是goto
	SetBottonReturnOnReload(MsgWaitHitLayer, NULL);
	ituWidgetShow(SetBottonReturnLayer, ITU_EFFECT_NONE, 0);
}

/*************************************************
Function:		MsgLinkOutTimeOnTimer
Description: 	定时器函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool MsgWaitHitLayerOnTimer(ITUWidget* widget, char* param)
{
	uint32 diff = 0;
	uint32 tick = SDL_GetTicks();

	if (tick >= g_TimerClock)
	{
		diff = tick - g_TimerClock;
	}
	else
	{
		diff = 0xFFFFFFFF - g_TimerClock + tick;
	}

	if (diff >= 3000)
	{
		g_TimerClock = tick;

		MsgWaitHitLayerOnHide();
		return true;
	}

	return false;
}
