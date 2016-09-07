/*********************************************************
Copyright (C), 2015-2020, Aurine
File name:	layer_cleanscreen.c
Author:		WuZ
Version:   	1.0
Date:		2016-06-017
Description: �����Ļ��������
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
static uint8_t		gCleanScreenTimeoutCount;	//��ʱʱ��
static uint32_t		gCleanScreenLastTimeTick;	//������¼��ʱ���ϸ�ʱ�̵�ʱ��


bool cleanScreenLayerOnEnter(ITUWidget* widget, char* param)
{
	//TODO:�ڽ����������ʱ��������ʱ����
	printf("33333333333333333333333333");

	if (!cleanScreenTimeText)
	{
		cleanScreenTimeText = ituSceneFindWidget(&theScene, "cleanScreenTimeText");
		assert(cleanScreenTimeText);
	}
	gCleanScreenLastTimeTick = SDL_GetTicks();		//������ʱ��ǰҪ�Ȼ�ȡһ�ε�ǰʱ���Ա�Ա�
	gCleanScreenTimeoutCount = CLEANSCREEN_TIMEOUT;
	ituTextSetString(cleanScreenTimeText, "10");

	return true;
}


bool cleanScreenLayerOnLeave(ITUWidget* widget, char* param)
{
	//TODO:���뿪�������ʱ�򣬹رն�ʱ����
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
		
		if (duration >= 1000)		//ʱ������Ժ���Ϊ��λ
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