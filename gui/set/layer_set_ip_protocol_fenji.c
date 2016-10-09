/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_ip_protocol.c
Author:     	zxc
Version:
Date: 		2016-07-28
Description: 分机IP协议转换器设置
History:
1. Date:
Author:
Modification:
*************************************************/
#include "layer_set.h"

#define MAX_SHOW_FENJI_NUM			9					// UI最大支持显示几个分机

static ITUText* SetIpProtocolBindingFenji2Text = NULL;
static ITUText* SetIpProtocolBindNumFenji2Text = NULL;
static ITUText* SetIpProtocolIPFenji2Text = NULL;
static ITUCoverFlow* SetIpProtocolFenjiCoverFlow = NULL;
static ITUCalendar* SetIpProtocolFenjiListContainer[3] = { NULL };
static ITUText* SetIpProtocolFJInfoFenji1Text[MAX_SHOW_FENJI_NUM] = { NULL };
static ITUText* SetIpProtocolFJInfoFenji2Text[MAX_SHOW_FENJI_NUM] = { NULL };
static ITUCalendar* SetIpProtocolFJInfoFenjiContainer[MAX_SHOW_FENJI_NUM] = { NULL };
static ITUTextBox* SetNumKeyBordTextBox = NULL;

static uint32 g_extcode = 0;					// 与IP模块捆绑的验证码
static uint32 g_bindstatus; 					// 0 : 未绑定; 1 :已绑定 
static IPMODULE_INFO ipmoduleinfo;
static IPAD_EXTENSION_LIST g_IpadList;
static uint32 g_ip = 0;
static uint8 buttonflag = 3;

/*************************************************
Function:		show_win_bind
Description: 	显示绑定的具体信息
Input:		无
Output:		无
Return:	
Others:
*************************************************/
static void show_win_bind()
{
	uint8 tmp[50];

	g_bindstatus = get_ipmodule_bindstate();
	g_ip = get_ipmodule_addr();

	if (g_bindstatus)
	{
		ituTextSetString(SetIpProtocolBindingFenji2Text, get_str(SID_Set_Bounded));
	}
	else
	{
		ituTextSetString(SetIpProtocolBindingFenji2Text, get_str(SID_Set_Unbounded));
	}

	memset(tmp, 0, sizeof(tmp));
	g_extcode = get_ipmodule_bindcode();
	sprintf(tmp, "%d", g_extcode);
	ituTextSetString(SetIpProtocolBindNumFenji2Text, tmp);

	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp, "%s", UlongtoIP(g_ip));
	ituTextSetString(SetIpProtocolIPFenji2Text, tmp);
}

/*************************************************
Function:		show_win_fenji
Description: 	显示分机的具体信息
Input:		无
Output:		无
Return:
Others:
*************************************************/
static void show_win_fenji()
{
	uint8 j = 0;
	char pExtIp[20] = { 0 };
	uint8 fenji_index[8] = { 0 };
	uint32 fenji_ip[8] = { 0 };
	uint8 fenji_mun = 0;
	char devtype[50], devno[5];

	// 获取IPAD分机信息
	memset(fenji_index, 0, sizeof(fenji_index));
	for (j = 0; j < 8; j++)
	{
		if (storage_get_subdev_ip(j))
		{
			fenji_index[fenji_mun] = j;
			fenji_ip[fenji_mun] = storage_get_subdev_ip(j);
			fenji_mun++;
		}
	}

	if (fenji_mun > 0)
	{
		for (j = 0; j < fenji_mun; j++)
		{
			memset(devno, 0, sizeof(devno));
			memset(devtype, 0, sizeof(devtype));
			sprintf(devno, "%d", fenji_index[j]);
			get_dev_description(DEVICE_TYPE_FENJI_NET, devno, devtype, 50);

			memset(pExtIp, 0, sizeof(pExtIp));
			sprintf(pExtIp, "%s", UlongtoIP(fenji_ip[j]));

			ituTextSetString(SetIpProtocolFJInfoFenji1Text[j], devtype);
			ituTextSetString(SetIpProtocolFJInfoFenji2Text[j], pExtIp);
		}
	}
	
	if (fenji_mun > 4)
	{
		ituWidgetSetVisible(SetIpProtocolFenjiListContainer[2], true);
	}
	else
	{
		ituWidgetSetVisible(SetIpProtocolFenjiListContainer[2], false);
	}
	for (j = 0; j < fenji_mun; j++)
	{
		ituWidgetSetVisible(SetIpProtocolFJInfoFenjiContainer[j], true);
	}
	for (j = fenji_mun; j < MAX_SHOW_FENJI_NUM; j++)
	{
		ituWidgetSetVisible(SetIpProtocolFJInfoFenjiContainer[j], false);
	}
}

/*************************************************
Function:		KeyBordGotoSetIpProtocol
Description: 	
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void KeyBordGotoSetIpProtocol()
{
	uint32 ip_data = 0;
	char tmp[50];

	char* text_data = ituTextGetString(SetNumKeyBordTextBox);
	if (0 == buttonflag)
	{
		g_extcode = atoi(text_data);
		ituTextSetString(SetIpProtocolBindNumFenji2Text, text_data);
	}
	else
	{
		int ret = IPIsCorrect(text_data);
		if (FALSE == ret)
		{
			ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_ERROR, SID_Set_Prj_IP_Address_Err, "SetIpProtocolFenjiLayer");
		}
		else
		{
			g_ip = IPtoUlong(text_data);

			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s", UlongtoIP(g_ip));
			ituTextSetString(SetIpProtocolIPFenji2Text, tmp);
		}
	}		
}

/*************************************************
Function:		SetIpProtocolFenjiOnEnter
Description: 	
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetIpProtocolFenjiOnEnter(ITUWidget* widget, char* param)
{
	if (!SetIpProtocolBindingFenji2Text)
	{
		uint8 i;
		char tmp[50] = { 0 };

		SetIpProtocolBindingFenji2Text = ituSceneFindWidget(&theScene, "SetIpProtocolBindingFenji2Text");
		assert(SetIpProtocolBindingFenji2Text);

		SetIpProtocolBindNumFenji2Text = ituSceneFindWidget(&theScene, "SetIpProtocolBindNumFenji2Text");
		assert(SetIpProtocolBindNumFenji2Text);

		SetIpProtocolIPFenji2Text = ituSceneFindWidget(&theScene, "SetIpProtocolIPFenji2Text");
		assert(SetIpProtocolIPFenji2Text);

		SetIpProtocolFenjiCoverFlow = ituSceneFindWidget(&theScene, "SetIpProtocolFenjiCoverFlow");
		assert(SetIpProtocolFenjiCoverFlow);

		SetNumKeyBordTextBox = ituSceneFindWidget(&theScene, "SetNumKeyBordTextBox");
		assert(SetNumKeyBordTextBox);

		for (i = 0; i < 3; i++)
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "SetIpProtocolFenjiListContainer", i);
			SetIpProtocolFenjiListContainer[i] = ituSceneFindWidget(&theScene, tmp);
			assert(SetIpProtocolFenjiListContainer[i]);
		}

		for (i = 0; i < MAX_SHOW_FENJI_NUM; i++)
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "SetIpProtocolFJInfoFenji1Text", i);
			SetIpProtocolFJInfoFenji1Text[i] = ituSceneFindWidget(&theScene, tmp);
			assert(SetIpProtocolFJInfoFenji1Text[i]);

			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "SetIpProtocolFJInfoFenji2Text", i);
			SetIpProtocolFJInfoFenji2Text[i] = ituSceneFindWidget(&theScene, tmp);
			assert(SetIpProtocolFJInfoFenji2Text[i]);

			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "SetIpProtocolFJInfoFenjiContainer", i);
			SetIpProtocolFJInfoFenjiContainer[i] = ituSceneFindWidget(&theScene, tmp);
			assert(SetIpProtocolFJInfoFenjiContainer[i]);
		}
	}

	if (strcmp(param, "SetNumKeyBordLayer") == 0)
	{
		KeyBordGotoSetIpProtocol();
	}
	else
	{
		g_extcode = 0;
		g_ip = 0;
		show_win_bind();
		show_win_fenji();
		ituCoverFlowGoto(SetIpProtocolFenjiCoverFlow, 0);
	}

	return true;
}

/*************************************************
Function:		SetIpProtocolBindNumFenjiButtonOnMouseUp
Description: 	绑定码钮按下抬起执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetIpProtocolBindNumFenjiButtonOnMouseUp(ITUWidget* widget, char* param)
{
	char tmp[50];

	buttonflag = 0;
	
	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp, "%d", g_extcode);
	KeybordLayerOnShow(NULL, PASS_TYPE_MAX, 6, EXPRESS_CHAR, CANCEL_BTN, tmp, "SetIpProtocolFenjiLayer");

	return true;
}

/*************************************************
Function:		SetIpProtocolIPFenjiButtonOnMouseUp
Description: 	IP协议地址钮按下抬起执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetIpProtocolIPFenjiButtonOnMouseUp(ITUWidget* widget, char* param)
{
	char tmp[50];

	buttonflag = 1;

	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp, "%s", UlongtoIP(g_ip));
	KeybordLayerOnShow(NULL, PASS_TYPE_MAX, 15, EXPRESS_CHAR, SPOT_BTN, tmp, "SetIpProtocolFenjiLayer");

	return true;
}

/*************************************************
Function:		SetIpProtocolBingFenjiButtonOnMouseUp
Description: 	绑定钮按下抬起执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetIpProtocolBingFenjiButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint32 ret = 0;

	set_ipmodule_bindcode(g_extcode);
	set_ipmodule_addr(g_ip);
	set_ipmodule_bindstate(0);
	ret = ipmodule_request_bind();
	if (ret)
	{
		ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_OK, SID_Set_Bind_OK, "SetIpProtocolFenjiLayer");
	}
	else
	{
		ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_ERROR, SID_Set_Bind_Fail, "SetIpProtocolFenjiLayer");
	}

	return true;
}

/*************************************************
Function:		SetIpProtocolGetFJFenjiButtonOnMouseUp
Description: 	获取分机钮按下抬起执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetIpProtocolGetFJFenjiButtonOnMouseUp(ITUWidget* widget, char* param)
{
	get_fenji_list();
	show_win_fenji();

	return true;
}
