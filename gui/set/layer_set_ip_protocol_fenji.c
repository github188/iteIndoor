/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_ip_protocol.c
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
	change_ip_to_str(g_ip, tmp);
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
	uint32 extenNo = 0;
	char pExtIp[20] = { 0 };
	uint8 tmp[50] = { 0 };
	uint32 cOnlineID = 0;

	// 获取IPAD分机信息
	get_ipad_extension(&g_IpadList);

	for (j = 0; j < g_IpadList.count; j++)
	{
		extenNo = g_IpadList.ipadData[j].devno;
		memset(pExtIp, 0, sizeof(pExtIp));
		change_ip_to_str(g_IpadList.ipadData[j].ipAddr, pExtIp);
		if (g_IpadList.ipadData[j].state == 1)
		{
			cOnlineID = get_str(SID_Set_Online);
		}
		else
		{
			cOnlineID = get_str(SID_Set_Offline);
		}

		memset(tmp, 0, sizeof(tmp));
		if (g_bindstatus)
		{
			sprintf(tmp, "%s:%d (%s)", get_str(SID_Set_ExtenNo), extenNo, cOnlineID);
		}
		else
		{
			sprintf(tmp, "%s:%d", get_str(SID_Set_ExtenNo), extenNo);
		}
		ituTextSetString(SetIpProtocolFJInfoFenji1Text[j], tmp);
		ituTextSetString(SetIpProtocolFJInfoFenji2Text[j], pExtIp);
	}

	
	if (g_IpadList.count > 4)
	{
		ituWidgetSetVisible(SetIpProtocolFenjiListContainer[2], true);
	}
	else
	{
		ituWidgetSetVisible(SetIpProtocolFenjiListContainer[2], false);
	}
	for (j = 0; j < g_IpadList.count; j++)
	{
		ituWidgetSetVisible(SetIpProtocolFJInfoFenjiContainer[j], true);
	}
	for (j = g_IpadList.count; j < MAX_SHOW_FENJI_NUM; j++)
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
	char tmp[50];
	uint32 ip_data = 0;

	char* text_data = ituTextGetString(SetNumKeyBordTextBox);
	dprintf("%s\n", text_data);
	if (0 == buttonflag)
	{
		g_extcode = atoi(text_data);
		ituTextSetString(SetIpProtocolBindNumFenji2Text, text_data);
	}
	else
	{
		uint8 ret = check_ip_to_true(text_data);
		if (FALSE == ret)
		{
			ShowMsgFailHintSuccessLayer(0, SID_Set_Prj_IP_Address_Err, 0);
		}
		else
		{
			ip_data = ipaddr_addr(text_data);
			g_ip = ntohl(ip_data);

			memset(tmp, 0, sizeof(tmp));
			change_ip_to_str(g_ip, tmp);
			ituTextSetString(SetIpProtocolIPFenji2Text, tmp);
		}
	}		
}

/*************************************************
Function:		boolSetIpProtocolFenjiOnEnter
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
	KeybordLayerOnShow(NULL, PASS_TYPE_MAX, 6, EXPRESS_CHAR, CANCEL_BTN, tmp);

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
	change_ip_to_str(g_ip, tmp);
	KeybordLayerOnShow(NULL, PASS_TYPE_MAX, 15, EXPRESS_CHAR, SPOT_BTN, tmp);

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
	uint8 ret = 0;

	set_ipmodule_bindcode(g_extcode);
	set_ipmodule_addr(g_ip);
	set_ipmodule_bindstate(0);
	ret = ipmodule_request_bind();
	if (ret)
	{
		//show_msg(hDlg, IDC_MSG_CTRL, MSG_WARNING, SID_Set_Bind_OK);
	}
	else
	{
		//show_msg(hDlg, IDC_MSG_CTRL, MSG_ERROR, SID_Set_Bind_Fail);
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



	return true;
}
