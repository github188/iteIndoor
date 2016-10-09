/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_netdoor_card.c
Author:     	zxc
Version:    	
Date: 		2016-07-27
Description:	网络门前机卡管理
History:
1. Date:
Author:
Modification:
*************************************************/
#include "../layer_set.h"

static ITULayer* SetNetDoorLayer = NULL;
static ITURadioBox* MsgFailHintSuccess1RadioBox = NULL;

static uint8 g_OperType = 0;						// 1 增加卡 2 删除卡

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
	case CMD_NETDOOR_MNG_CARD:
		if (NETDOOR_ECHO_SUCESS == logic_mng_netdoor_card(g_OperType, NULL))
		{
			if (2 == g_OperType)
			{
				ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_OK, SID_Msg_Operat_OK, "SetNetDoorLayer");
			}
		}
		else
		{
			ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_ERROR, SID_Msg_Operat_Err, "SetNetDoorCardLayer");
		}
		break;
	}
}


/*************************************************
Function:		SetNetDoorCardOnEnter
Description: 	设置网络门前机卡界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetNetDoorCardOnEnter(ITUWidget* widget, char* param)
{
	if (!SetNetDoorLayer)
	{
		SetNetDoorLayer = ituSceneFindWidget(&theScene, "SetNetDoorLayer");
		assert(SetNetDoorLayer);

		MsgFailHintSuccess1RadioBox = ituSceneFindWidget(&theScene, "MsgFailHintSuccess1RadioBox");
		assert(MsgFailHintSuccess1RadioBox);
	}

	if (strcmp(param, "MsgFailHintSuccessLayer") == 0)
	{
		if (ituRadioBoxIsChecked(MsgFailHintSuccess1RadioBox))	//确认键
		{
			OnNetDoorDeal(CMD_NETDOOR_MNG_CARD);
		}
	}

	return true;
}

/*************************************************
Function:		SetNetDoorCardButtonOnMouseUp
Description: 	网络门前机卡选择界面按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetNetDoorCardButtonOnMouseUp(ITUWidget* widget, char* param)
{
	int index = atoi(param);

	g_OperType = (uint8)index;

	if (1 == g_OperType)
	{
		OnNetDoorDeal(CMD_NETDOOR_MNG_CARD);
	}
	else
	{
		ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_WARNNING, SID_Msg_Operat_Err, "SetNetDoorCardLayer");
	}
	
	return true;
}
