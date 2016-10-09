/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_netdoor.c
Author:     	zxc
Version:    	
Date: 		2016-07-24
Description: 网络门前机设置界面
History:
1. Date:
Author:
Modification:
*************************************************/
#include "../layer_set.h"

static ITUText* SetNetDoorTitleText = NULL;
static ITUText* SetNetDoorNum2Text[2] = { NULL };
static ITUCalendar* SetNetDoorNumContainer = NULL; 
static ITUCalendar* SetNetDoorParamContainer = NULL;
static ITULayer* SetProjectLayer = NULL;

//static uint32 g_TimerClock = 0;
//static uint8 g_open_timer = FALSE;
static int g_door_index = 0;

/*************************************************
Function:			EnterNetDoorDeal
Description: 		工程设置界面进入网络门前机
Input:
1.win:   		当前处理的窗口
2.wParam:
Output:
Return:
Others:
*************************************************/
static void EnterNetDoorDeal()
{
	uint32 i, data = 0;
	// 判断网络门前机是否启用
	for (i = 0; i < 2; i++)
	{
		data = storage_get_extmode(EXT_MODE_NETDOOR1 + i);
		ituTextSetString(SetNetDoorNum2Text[i], get_str(SID_Set_Prj_ExtDoor1_Unused + data));
	}

	ituTextSetString(SetNetDoorTitleText, get_str(SID_Net_Door));

	ituWidgetSetVisible(SetNetDoorParamContainer, false); 
	ituWidgetSetVisible(SetNetDoorNumContainer, true);
}

/*************************************************
Function:			EnterNetDoorDeal
Description: 		进入网络门前机参数列表
Input:
1.win:   		当前处理的窗口
2.wParam:
Output:
Return:
Others:
*************************************************/
static void EnterNetDoorParamListDeal(int num)
{
	if (FALSE == storage_get_extmode(EXT_MODE_ALARM + num))
	{
		if (1 == num)
		{
			ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_ERROR, SID_Set_Prj_ExtDoor1_Unused, "SetNetDoorLayer");
		}
		else
		{
			ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_ERROR, SID_Set_Prj_ExtDoor2_Unused, "SetNetDoorLayer");
		}
		return;
	}
	
	// 判断网络门前机是否连接
	if (NETDOOR_ECHO_SUCESS != logic_netdoor_pub_ini(num))
	{
		MsgLinkOutTimeOnShow();			//显示连接超时
	}

	MsgWaitHitLayerOnHide();
}

/*************************************************
Function:		SetNetDoorOnEnter
Description: 	设置网络门前机界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetNetDoorOnEnter(ITUWidget* widget, char* param)
{
	if (!SetNetDoorTitleText)
	{
		uint8 i;
		char tmp[50];

		SetNetDoorTitleText = ituSceneFindWidget(&theScene, "SetNetDoorTitleText");
		assert(SetNetDoorTitleText);

		SetNetDoorNumContainer = ituSceneFindWidget(&theScene, "SetNetDoorNumContainer");
		assert(SetNetDoorNumContainer);

		SetNetDoorParamContainer = ituSceneFindWidget(&theScene, "SetNetDoorParamContainer");
		assert(SetNetDoorParamContainer);

		SetProjectLayer = ituSceneFindWidget(&theScene, "SetProjectLayer");
		assert(SetProjectLayer);

		for (i = 0; i < 2; i++)
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d%s", "SetNetDoorNum", i+1, "2Text");
			SetNetDoorNum2Text[i] = ituSceneFindWidget(&theScene, tmp);
			assert(SetNetDoorNum2Text[i]);
		}
	}

	if (strcmp(param, "SetProjectLayer") == 0)
	{
		//g_TimerClock = SDL_GetTicks();

		EnterNetDoorDeal();
	}

	return true;
}

/*************************************************
Function:		SetNetDoorNumButtonOnMouseUp
Description: 	网络门前机选择界面按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetNetDoorNumButtonOnMouseUp(ITUWidget* widget, char* param)
{
	int index = atoi(param);
	g_door_index = index;

	set_netdoorparam_Page(index);

	ituTextSetString(SetNetDoorTitleText, get_str(SID_Net_Door_First + index - 1));

	ituWidgetSetVisible(SetNetDoorNumContainer, false);
	ituWidgetSetVisible(SetNetDoorParamContainer, true);

	MsgWaitHitLayerOnShow("SetNetDoorLayer");
	//g_open_timer = TRUE;
	EnterNetDoorParamListDeal(g_door_index);
	
	return true;
}

#if 0
/*************************************************
Function:		MsgLinkOutTimeOnTimer
Description: 	定时器函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetNetDoorLayerOnTimer(ITUWidget* widget, char* param)
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

	dprintf("diff = %d\n", diff);

	if (diff >= 500)
	{
		g_TimerClock = tick;

		if (g_open_timer == TRUE)
		{
			g_open_timer = FALSE;
			
		}
	}

	return true;
}
#endif

/*************************************************
Function:		SetNetDoorLayerOnReturn
Description: 	底部返回按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
void SetNetDoorLayerOnReturn()
{	
	if (ituWidgetIsVisible(SetNetDoorParamContainer))
	{
		ituTextSetString(SetNetDoorTitleText, get_str(SID_Net_Door));
		ituWidgetSetVisible(SetNetDoorParamContainer, false);
		ituWidgetSetVisible(SetNetDoorNumContainer, true);
		return;
	}
	else if (ituWidgetIsVisible(SetNetDoorNumContainer))
	{
		if (!ituWidgetIsVisible(SetProjectLayer))
		{
			ituLayerGoto(SetProjectLayer);
			return;
		}
	}
}
