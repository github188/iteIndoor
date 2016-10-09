/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_netdoor_lock.c
Author:     	zxc
Version:    	
Date: 		2016-07-27
Description: 网络门前机锁类型设置
History:
1. Date:
Author:
Modification:
*************************************************/
#include "../layer_set.h"

static ITUText* SetNetDoorLockType2Text = NULL;
static ITUText* SetNetDoorLockTime2Text = NULL;
static ITUCalendar* SetNetDoorLockBackground = NULL;
static ITUCalendar* SetNetDoorLockTypeMsgBackground = NULL;
static ITULayer* SetNetDoorLayer = NULL;
static ITURadioBox* SetNetDoorLockTypeMsgRadioBox[2] = { NULL };
static ITUTextBox* SetNumKeyBordTextBox = NULL;
static ITURadioBox* MsgFailHintSuccess1RadioBox = NULL;

static uint8 g_state[2];				// 0:常开  1:常闭
static int32 g_locktime[2];				// 时长

/*************************************************
Function:		SetNetDoorNetLockOnShowData
Description: 	更新锁类型参数设置界面数据
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void SetNetDoorNetLockOnShowData()
{
	char tmp[50];

	ituTextSetString(SetNetDoorLockType2Text, get_str(SID_Bj_Tantou_Close + g_state[0]));

	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp, "%d%s", g_locktime[0], get_str(SID_Time_Sec));
	ituTextSetString(SetNetDoorLockTime2Text, tmp);
}

/*************************************************
Function:		KeyBordGotoSetNetDoorLock
Description: 	开锁时长参数设置界面数据
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void KeyBordGotoSetNetDoorLock()
{
	char tmp[50];
	char* text_data = ituTextGetString(SetNumKeyBordTextBox);
	int time = atoi(text_data);

	g_locktime[0] = time;
	if (g_locktime[0] > 60)
	{
		g_locktime[0] = 60;
	}

	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp, "%d%s", g_locktime[0], get_str(SID_Time_Sec));
	ituTextSetString(SetNetDoorLockTime2Text, tmp);
}

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
	case CMD_NETDOOR_GET_LOCK:
		if (NETDOOR_ECHO_SUCESS == logic_get_netdevice_lockinfo(&g_state[0], &g_locktime[0]))
		{ 
			SetNetDoorNetLockOnShowData();
		}
		else
		{
			MsgLinkOutTimeOnShow();
		}
		g_state[1] = g_state[0];
		g_locktime[1] = g_locktime[0];
		break;

	case CMD_NETDOOR_SET_LOCK:
		if (NETDOOR_ECHO_SUCESS == logic_set_netdevice_lockinfo(g_state[0], g_locktime[0]))
		{
			ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_OK, SID_Msg_Save_OK, "SetNetDoorLockLayer");
		}
		else
		{
			ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_ERROR, SID_Msg_Save_Err, "SetNetDoorLockLayer");
		}
		break;
	}
}

/*************************************************
Function:		SetNetDoorLockOnEnter
Description: 	设置网络门前机锁类型界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetNetDoorLockOnEnter(ITUWidget* widget, char* param)
{
	if (!SetNetDoorLockType2Text)
	{
		char tmp[50];
		uint8 i = 0;

		SetNetDoorLockType2Text = ituSceneFindWidget(&theScene, "SetNetDoorLockType2Text");
		assert(SetNetDoorLockType2Text);

		SetNetDoorLockTime2Text = ituSceneFindWidget(&theScene, "SetNetDoorLockTime2Text");
		assert(SetNetDoorLockTime2Text);

		SetNetDoorLockBackground = ituSceneFindWidget(&theScene, "SetNetDoorLockBackground");
		assert(SetNetDoorLockBackground);

		SetNetDoorLockTypeMsgBackground = ituSceneFindWidget(&theScene, "SetNetDoorLockTypeMsgBackground");
		assert(SetNetDoorLockTypeMsgBackground);

		SetNetDoorLayer = ituSceneFindWidget(&theScene, "SetNetDoorLayer");
		assert(SetNetDoorLayer);

		SetNumKeyBordTextBox = ituSceneFindWidget(&theScene, "SetNumKeyBordTextBox");
		assert(SetNumKeyBordTextBox);

		MsgFailHintSuccess1RadioBox = ituSceneFindWidget(&theScene, "MsgFailHintSuccess1RadioBox");
		assert(MsgFailHintSuccess1RadioBox);

		for (i = 0; i < 2; i++)
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d%s", "SetNetDoorLockTypeMsg", i, "RadioBox");
			SetNetDoorLockTypeMsgRadioBox[i] = ituSceneFindWidget(&theScene, tmp);
			assert(SetNetDoorLockTypeMsgRadioBox[i]);
		}
	}

	
	if (strcmp(param, "SetNumKeyBordLayer") == 0)
	{
		KeyBordGotoSetNetDoorLock();
	}
	else if (strcmp(param, "MsgFailHintSuccessLayer") == 0)
	{
		if (ituRadioBoxIsChecked(MsgFailHintSuccess1RadioBox))	//确认键
		{
			OnNetDoorDeal(CMD_NETDOOR_SET_LOCK);
		}
		else
		{
			ituLayerGoto(SetNetDoorLayer);
		}
	}
	else //if (strcmp(param, "SetNetDoorLayer") == 0)
	{
		g_locktime[0] = 1;
		g_state[0] = 0;
		SetNetDoorNetLockOnShowData();
		ituWidgetSetVisible(SetNetDoorLockTypeMsgBackground, false);
		ituWidgetSetVisible(SetNetDoorLockBackground, true);

		OnNetDoorDeal(CMD_NETDOOR_GET_LOCK);
		
	}

	return true;
}

/*************************************************
Function:		SetNetDoorLockTypeButtonOnMouseUp
Description: 	锁类型按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetNetDoorLockTypeButtonOnMouseUp(ITUWidget* widget, char* param)
{
	ituRadioBoxSetChecked(SetNetDoorLockTypeMsgRadioBox[g_state[0]], true);

	ituWidgetDisable(SetNetDoorLockBackground);
	ituWidgetSetVisible(SetNetDoorLockTypeMsgBackground, true);
	
	return true;
}

/*************************************************
Function:		SetNetDoorLockTimeButtonOnMouseUp
Description: 	开锁时长按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetNetDoorLockTimeButtonOnMouseUp(ITUWidget* widget, char* param)
{
	char tmp[50];
	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp, "%d", g_locktime[0]);
	KeybordLayerOnShow(NULL, PASS_TYPE_MAX, 2, EXPRESS_CHAR, CANCEL_BTN, tmp, "SetNetDoorLockLayer");

	return true;
}

/*************************************************
Function:		SetNetDoorLockTypeMsgRadioBoxOnMouseUp
Description: 	锁类型列表按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetNetDoorLockTypeMsgRadioBoxOnMouseUp(ITUWidget* widget, char* param)
{
	int index = atoi(param);
	g_state[0] = (uint8)index;

	ituTextSetString(SetNetDoorLockType2Text, get_str(SID_Bj_Tantou_Close + g_state[0]));

	ituWidgetSetVisible(SetNetDoorLockTypeMsgBackground, false);
	ituWidgetEnable(SetNetDoorLockBackground);

	return true;
}

/*************************************************
Function:		SetNetDoorLockLayerOnReturn
Description: 	底部返回按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
void SetNetDoorLockLayerOnReturn()
{	
	if (ituWidgetIsVisible(SetNetDoorLockTypeMsgBackground))
	{
		ituWidgetSetVisible(SetNetDoorLockTypeMsgBackground, false);
		ituWidgetEnable(SetNetDoorLockBackground);
	}
	else if (!ituWidgetIsVisible(SetNetDoorLayer))
	{
		if ((g_state[1] != g_state[0]) || (g_locktime[1] != g_locktime[0]))
		{
			ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_WARNNING, SID_Net_Door_Save_Reboot_Door, "SetNetDoorLockLayer");
		}
		else
		{
			ituLayerGoto(SetNetDoorLayer);
		}
		return;
	}
}
