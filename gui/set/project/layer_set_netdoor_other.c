/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_netdoor.c
Author:     	zxc
Version:    	
Date: 		2016-07-28
Description:
History:
1. Date:
Author:
Modification:
*************************************************/
#include "gui_include.h"

static ITUSprite* SetNetDoorRemoteMoniterSprite = NULL;
static ITULayer* SetNetDoorLayer = NULL;
static ITURadioBox* MsgFailHintSuccess1RadioBox = NULL;

static uint8 g_value[10];
static uint8 g_old_value[10];

/*************************************************
Function:			OnNetDoorDeal
Description: 		处理网络门前机
Input:
1.win:   		当前处理的窗口
2.wParam:
Output:
Return:
Others:
*************************************************/
static void OnNetDoorDeal(uint32 wParam)
{
	uint32 ret = 0;

	switch (wParam)
	{
	case CMD_GATEWAY_GET_OTHERS:
		ret = logic_get_netdoor_other_settings(g_value);
		if (NETDOOR_ECHO_SUCESS == ret)
		{
			strcpy(g_old_value, g_value);
			if (g_value[1] > 1)	//目前就只有设置远程监视
			{
				g_value[1] = 0;
			}
			ituSpriteGoto(SetNetDoorRemoteMoniterSprite, g_value[1]);
		}
		else
		{
			ituSpriteGoto(SetNetDoorRemoteMoniterSprite, 0);
			MsgLinkOutTimeOnShow();
		}
		break;

	case CMD_GATEWAY_SET_OTHERS:
		ret = logic_set_netdoor_other_settings(g_value);
		if (NETDOOR_ECHO_SUCESS == ret)
		{
			ShowMsgFailHintSuccessLayer(2, SID_Msg_Save_OK, 0);
		}
		else
		{
			ShowMsgFailHintSuccessLayer(0, SID_Msg_Save_Err, 0);
		}
		break;
	}
}

/*************************************************
Function:		SetNetDoorOnEnter
Description: 	设置网络门前机界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetNetDoorOtherOnEnter(ITUWidget* widget, char* param)
{
	if (!SetNetDoorRemoteMoniterSprite)
	{
		SetNetDoorRemoteMoniterSprite = ituSceneFindWidget(&theScene, "SetNetDoorRemoteMoniterSprite");
		assert(SetNetDoorRemoteMoniterSprite);

		SetNetDoorLayer = ituSceneFindWidget(&theScene, "SetNetDoorLayer");
		assert(SetNetDoorLayer);

		MsgFailHintSuccess1RadioBox = ituSceneFindWidget(&theScene, "MsgFailHintSuccess1RadioBox");
		assert(MsgFailHintSuccess1RadioBox);
	}

	if (strcmp(param, "SetNetDoorLayer") == 0)
	{
		memset(g_value, 0, sizeof(g_value));
		memset(g_old_value, 0, sizeof(g_old_value));
		OnNetDoorDeal(CMD_GATEWAY_GET_OTHERS);
	}
	else if (strcmp(param, "MsgFailHintSuccessLayer") == 0)
	{
		if (ituRadioBoxIsChecked(MsgFailHintSuccess1RadioBox))	//确认键
		{
			OnNetDoorDeal(CMD_GATEWAY_SET_OTHERS);
		}
		else
		{
			ituLayerGoto(SetNetDoorLayer);
		}
	}

	return true;
}

/*************************************************
Function:		SetNetDoorRemoteMoniterButtonOnMouseUp
Description: 	远程监视按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetNetDoorRemoteMoniterButtonOnMouseUp(ITUWidget* widget, char* param)
{
	if (1 == g_value[1])	
	{
		g_value[1] = 0;
	}
	else
	{
		g_value[1] = 1;
	}
	ituSpriteGoto(SetNetDoorRemoteMoniterSprite, g_value[1]);
	
	return true;
}

/*************************************************
Function:		SetNetDoorOtherLayerOnReturn
Description: 	底部返回按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
void SetNetDoorOtherLayerOnReturn()
{	
	if (!ituWidgetIsVisible(SetNetDoorLayer))
	{
		//if (strcmp(g_value, g_old_value) == 0)
		if (g_value[1] == g_old_value[1])
		{
			ituLayerGoto(SetNetDoorLayer);
		}
		else
		{
			ShowMsgFailHintSuccessLayer(1, SID_Msg_Param_Save, 1);
		}
		
		return;
	}
}
