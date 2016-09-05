/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_netparam.c
Author:     	zxc
Version:
Date: 		2016-07-10
Description: 设置网络参数界面
*************************************************/
#include "../layer_set.h"

static ITUText* SetNetParamTitleText = NULL;
static ITUCalendar* SetNetParamContainer = NULL;
static ITUCalendar* SetNetParamHostListContainer = NULL;
static ITUCalendar* SetManageIPContainer = NULL;
static ITUCalendar* SetServerIPContainer = NULL;
static ITULayer* SetProjectLayer = NULL;
static ITUText* SetHostIP2Text = NULL;
static ITUText* SetHostGateWay2Text = NULL;
static ITUText* SetHostNetMask2Text = NULL;
static ITUText* SetCenterServerIP2Text = NULL;
static ITUText* SetRtspIP2Text = NULL;
static ITUText* SetManageIP12Text = NULL;
static ITUText* SetManageIP22Text = NULL;
static ITUText* SetManageIP32Text = NULL;
static ITUTextBox* SetNumKeyBordTextBox = NULL;
static ITURadioBox* MsgFailHintSuccess1RadioBox = NULL;
static ITURadioBox* MsgFailHintSuccess0RadioBox = NULL;

static uint32 g_ip[IP_MAX];
static IP_TYPE g_ipType = IP_MAX;
static uint32 g_ip_host[CENTER_IPADDR] = { 0 };

/*************************************************
Function:		save_param
Description: 	保存
Input:
1.hDlg
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void save_param(void)
{
	uint8 i;

	if (g_ipType > HOST_GATEWAY)
	{
		storage_set_netparam(1, g_ipType, g_ip[g_ipType]);
	}
	else
	{
		for (i = 0; i < CENTER_IPADDR; i++)
		{
			storage_set_netparam(1, HOST_IPADDR+i, g_ip[i]);	
		}
		// 存入注册表
		net_set_local_param();
	}
}

/*************************************************
Function:		SetNetParamOnEnter
Description: 	进入网络设置界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void KeyBordGotoSetNetParam()
{
	ITUText* IPtext[IP_MAX] = { SetHostIP2Text, SetHostNetMask2Text, SetHostGateWay2Text, SetCenterServerIP2Text, 
								SetManageIP12Text, SetManageIP22Text, SetManageIP32Text, 0, 0, SetRtspIP2Text };
	char* IP_data = ituTextGetString(SetNumKeyBordTextBox);
	int ret = IPIsCorrect(IP_data);
	if (FALSE == ret)
	{
		ShowMsgFailHintSuccessLayer(0, SID_Set_Prj_IP_Address_Err, 0);
	}
	else
	{
		uint32 data = IPtoUlong(IP_data);

		if (data != g_ip[g_ipType])
		{
			if (g_ipType > HOST_GATEWAY)
			{
				g_ip[g_ipType] = data;
				save_param();
			}
			else
			{
				g_ip_host[g_ipType] = data;
			}
			ituTextSetString(IPtext[g_ipType], IP_data);
		}
	}
}

/*************************************************
Function:		SetNetParamOnEnter
Description: 	进入网络设置界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetNetParamOnEnter(ITUWidget* widget, char* param)
{
	uint8 i;

	if (!SetNetParamTitleText)
	{
		SetNetParamTitleText = ituSceneFindWidget(&theScene, "SetNetParamTitleText");
		assert(SetNetParamTitleText);

		SetNetParamContainer = ituSceneFindWidget(&theScene, "SetNetParamContainer");
		assert(SetNetParamContainer);

		SetNetParamHostListContainer = ituSceneFindWidget(&theScene, "SetNetParamHostListContainer");
		assert(SetNetParamHostListContainer);

		SetManageIPContainer = ituSceneFindWidget(&theScene, "SetManageIPContainer");
		assert(SetManageIPContainer);

		SetServerIPContainer = ituSceneFindWidget(&theScene, "SetServerIPContainer");
		assert(SetServerIPContainer);

		SetProjectLayer = ituSceneFindWidget(&theScene, "SetProjectLayer");
		assert(SetProjectLayer);

		SetHostIP2Text = ituSceneFindWidget(&theScene, "SetHostIP2Text");
		assert(SetHostIP2Text);

		SetHostGateWay2Text = ituSceneFindWidget(&theScene, "SetHostGateWay2Text");
		assert(SetHostGateWay2Text);

		SetHostNetMask2Text = ituSceneFindWidget(&theScene, "SetHostNetMask2Text");
		assert(SetHostNetMask2Text);

		SetCenterServerIP2Text = ituSceneFindWidget(&theScene, "SetCenterServerIP2Text");
		assert(SetCenterServerIP2Text);

		SetRtspIP2Text = ituSceneFindWidget(&theScene, "SetRtspIP2Text");
		assert(SetRtspIP2Text);

		SetManageIP12Text = ituSceneFindWidget(&theScene, "SetManageIP12Text");
		assert(SetManageIP12Text);

		SetManageIP22Text = ituSceneFindWidget(&theScene, "SetManageIP22Text");
		assert(SetManageIP22Text);

		SetManageIP32Text = ituSceneFindWidget(&theScene, "SetManageIP32Text");
		assert(SetManageIP32Text); 

		SetNumKeyBordTextBox = ituSceneFindWidget(&theScene, "SetNumKeyBordTextBox");
		assert(SetNumKeyBordTextBox); 

		MsgFailHintSuccess1RadioBox = ituSceneFindWidget(&theScene, "MsgFailHintSuccess1RadioBox");
		assert(MsgFailHintSuccess1RadioBox);

		MsgFailHintSuccess0RadioBox = ituSceneFindWidget(&theScene, "MsgFailHintSuccess0RadioBox");
		assert(MsgFailHintSuccess0RadioBox);
	}

	if (strcmp(param, "SetNumKeyBordLayer") == 0)
	{
		KeyBordGotoSetNetParam();
	}
	else if (strcmp(param, "MsgFailHintSuccessLayer") == 0)
	{
		if (ituRadioBoxIsChecked(MsgFailHintSuccess1RadioBox))	//确认键
		{
			memcpy(g_ip, g_ip_host, sizeof(g_ip_host));
			save_param();

			ituWidgetSetVisible(SetNetParamHostListContainer, false);
			ituWidgetSetVisible(SetNetParamContainer, true);
		}
		if (ituRadioBoxIsChecked(MsgFailHintSuccess0RadioBox))	//确认键
		{
			ituWidgetSetVisible(SetNetParamHostListContainer, false);
			ituWidgetSetVisible(SetNetParamContainer, true);
		}
	}
	else if (strcmp(param, "SetProjectLayer") == 0)
	{
		memset(g_ip, 0, sizeof(g_ip));
		memset(g_ip_host, 0, sizeof(g_ip_host));

		for (i = 0; i < IP_MAX; i++)
		{
			g_ip[i] = storage_get_netparam_bytype(HOST_IPADDR + i);
		}
		memcpy(g_ip_host, g_ip, sizeof(g_ip_host));

		ituWidgetSetVisible(SetNetParamHostListContainer, false);
		ituWidgetSetVisible(SetServerIPContainer, false);
		ituWidgetSetVisible(SetManageIPContainer, false);
		ituWidgetSetVisible(SetNetParamContainer, true);
	}


	return true;
}

/*************************************************
Function:		SetNetParamHostButtonOnMouseUp
Description: 	本机参数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetNetParamHostButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 i;
	char tmp[30];
	ITUText* text[3] = { SetHostIP2Text, SetHostNetMask2Text, SetHostGateWay2Text };
	
	for (i = 0; i < 3; i++)
	{
		memset(tmp, 0, sizeof(tmp));
		change_ip_to_str(g_ip[HOST_IPADDR + i], tmp);
		ituTextSetString(text[i], tmp);
	}
	
	ituWidgetSetVisible(SetNetParamContainer, false);
	ituWidgetSetVisible(SetNetParamHostListContainer, true);

	return true;
}

/*************************************************
Function:		SetNetParamSeverButtonOnMouseUp
Description: 	服务器参数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetNetParamSeverButtonOnMouseUp(ITUWidget* widget, char* param)
{
	char tmp[30];
		
	memset(tmp, 0, sizeof(tmp));
	change_ip_to_str(g_ip[CENTER_IPADDR], tmp);
	ituTextSetString(SetCenterServerIP2Text, tmp);
	memset(tmp, 0, sizeof(tmp));
	change_ip_to_str(g_ip[RTSP_IPADDR], tmp);
	ituTextSetString(SetRtspIP2Text, tmp);
	
	ituWidgetSetVisible(SetNetParamContainer, false);
	ituWidgetSetVisible(SetServerIPContainer, true);

	return true;
}

/*************************************************
Function:		SetNetParamManageButtonOnMouseUp
Description: 	管理员机参数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetNetParamManageButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 i;
	char tmp[30];
	ITUText* text[3] = { SetManageIP12Text, SetManageIP22Text, SetManageIP32Text };

	for (i = 0; i < 3; i++)
	{
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s", UlongtoIP(g_ip[MANAGER1_IPADDR + i]));
		ituTextSetString(text[i], tmp);
	}

	ituWidgetSetVisible(SetNetParamContainer, false);
	ituWidgetSetVisible(SetManageIPContainer, true);

	return true;
}

/*************************************************
Function:		SetAllNetParamButtonOnMouseUp
Description: 	设置IP
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetAllNetParamButtonOnMouseUp(ITUWidget* widget, char* param)
{
	char tmp[40];
	int index = atoi(param);
	g_ipType = (IP_TYPE)index;
	
	memset(tmp, 0, sizeof(tmp));
	change_ip_to_str(g_ip[index], tmp);

	KeybordLayerOnShow(NULL, PASS_TYPE_MAX, 15, EXPRESS_CHAR, SPOT_BTN, tmp);

	return true;
}

/*************************************************
Function:		SetNetParamLayerOnReturn
Description: 	底部退出键按下执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
void SetNetParamLayerOnReturn(void)
{
	if (ituWidgetIsVisible(SetNetParamContainer))
	{
		if (!ituWidgetIsVisible(SetProjectLayer))
		{
			ituLayerGoto(SetProjectLayer);
		}
		return;
	}
	else if (ituWidgetIsVisible(SetNetParamHostListContainer))
	{
		if (0 != strncasecmp(g_ip, g_ip_host, sizeof(g_ip_host)))
		{
			ShowMsgFailHintSuccessLayer(1, SID_Msg_Param_Suer_Save, 1);
		}
		else
		{
			ituWidgetSetVisible(SetNetParamHostListContainer, false);
			ituWidgetSetVisible(SetNetParamContainer, true);
		}
		
		return;
	}
	else if (ituWidgetIsVisible(SetServerIPContainer))
	{
		ituWidgetSetVisible(SetServerIPContainer, false);
		ituWidgetSetVisible(SetNetParamContainer, true);
		return;
	}
	else if (ituWidgetIsVisible(SetManageIPContainer))
	{
		ituWidgetSetVisible(SetManageIPContainer, false);
		ituWidgetSetVisible(SetNetParamContainer, true);
		return;
	}
}
