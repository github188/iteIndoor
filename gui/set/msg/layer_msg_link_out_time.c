/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_msg_link_out_time.c
Author:     	zxc
Version:
Date: 		2016-06-26
Description:
Modification:连接超时提示
*************************************************/
#include "../layer_set.h"

static ITULayer* MsgLinkOutTimeLayer = NULL;

static uint32 g_TimerClock = 0;

/*************************************************
Function:		MsgLinkOutTimeOnTimer
Description: 	定时器函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool MsgLinkOutTimeOnTimer(ITUWidget* widget, char* param)
{
	uint32 tick = SDL_GetTicks();
	uint32 diff = 0;

	if (tick >= g_TimerClock)
	{
		diff = tick - g_TimerClock;
	}
	else
	{
		diff = 0xFFFFFFFF - g_TimerClock + tick;
	}

	if (diff >= 2000)
	{
		g_TimerClock = tick;

		ituWidgetHide(MsgLinkOutTimeLayer, ITU_EFFECT_NONE, 0);
	}

	return true;
}

/*************************************************
Function:		MsgLinkOutTimeLayerOnLeave
Description: 	退出处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool MsgLinkOutTimeLayerOnLeave(ITUWidget* widget, char* param)
{
	if (!MsgLinkOutTimeLayer)
	{
		MsgLinkOutTimeLayer = ituSceneFindWidget(&theScene, "MsgLinkOutTimeLayer");
		assert(MsgLinkOutTimeLayer);
	}

	if (ituWidgetIsVisible(MsgLinkOutTimeLayer))	//解决提示连接超时的小消息框还没消失就按下界面跳到另一个界面，导致小消息框不能消息卡机问题
	{
		ituWidgetHide(MsgLinkOutTimeLayer, ITU_EFFECT_NONE, 0);
	}

	return true;
}

/*************************************************
Function:		MsgLinkOutTimeOnShow
Description: 	
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
void MsgLinkOutTimeOnShow()
{
	if (!MsgLinkOutTimeLayer)
	{
		MsgLinkOutTimeLayer = ituSceneFindWidget(&theScene, "MsgLinkOutTimeLayer");
		assert(MsgLinkOutTimeLayer);
	}
	
	g_TimerClock = SDL_GetTicks();
	ituWidgetShow(MsgLinkOutTimeLayer, ITU_EFFECT_NONE, 0);
}
