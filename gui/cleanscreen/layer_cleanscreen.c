/*********************************************************
Copyright (C), 2015-2020, Aurine
File name:	layer_cleanscreen.c
Author:		WuZ
Version:   	1.0
Date:		2016-06-017
Description: 清洁屏幕操作函数
History:
1. Date:
Author:
Modification:
2. ...
*********************************************************/


#include "layer_cleanscreen.h"

//itu
static ITULayer*	mainLayer;
static ITUText*		cleanScreenTimeText;

// status
static uint8_t		gCleanScreenTimeoutCount;	//超时时间
static uint32_t		gCleanScreenLastTimeTick;	//用来记录定时器上个时刻的时间


bool cleanScreenLayerOnEnter(ITUWidget* widget, char* param)
{
	//TODO:在进入这个界面时候，启动定时器。
	printf("33333333333333333333333333");

	if (!cleanScreenTimeText)
	{
		cleanScreenTimeText = ituSceneFindWidget(&theScene, "cleanScreenTimeText");
		assert(cleanScreenTimeText);
	}
	gCleanScreenLastTimeTick = SDL_GetTicks();		//开启定时器前要先获取一次当前时间以便对比
	gCleanScreenTimeoutCount = CLEANSCREEN_TIMEOUT;
	ituTextSetString(cleanScreenTimeText, "10");

	return true;
}


bool cleanScreenLayerOnLeave(ITUWidget* widget, char* param)
{
	//TODO:在离开这个界面时候，关闭定时器。
	printf("4444444444444444444444444");

	return true;
}


bool cleanScreenTimeoutOnTimer(ITUWidget* widget, char* param)
{
	if (gCleanScreenTimeoutCount > 0)
	{
		uint32_t duration;
		uint32_t curtime = SDL_GetTicks();

		if (curtime >= gCleanScreenLastTimeTick)
		{
			duration = curtime - gCleanScreenLastTimeTick;
		}
		else
		{
			duration = 0xFFFFFFFF - gCleanScreenLastTimeTick + curtime;
		}
		
		if (duration >= 1000)		//时间差是以毫秒为单位
		{
			char buf[4] = { 0 };

			gCleanScreenTimeoutCount -= (uint32_t)duration / 1000;
			gCleanScreenLastTimeTick = curtime;

			if ((0 <= ((int)gCleanScreenTimeoutCount)) && (((int)gCleanScreenTimeoutCount) < CLEANSCREEN_TIMEOUT))
			{
				sprintf(buf, "%d", (int)gCleanScreenTimeoutCount);
				ituTextSetString(cleanScreenTimeText, buf);
			}
		}
		if (gCleanScreenTimeoutCount <= 0)
		{
			if (!mainLayer)
			{
				mainLayer = ituSceneFindWidget(&theScene, "mainLayer");
				assert(mainLayer);
			}
			ituLayerGoto(mainLayer);
		}
	}

	return true;
}

void cleanScreenReset(void)
{
	mainLayer = NULL;
	cleanScreenTimeText = NULL;
}