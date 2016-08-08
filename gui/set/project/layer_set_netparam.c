/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_netparam.c
Author:     	zxc
Version:
Date: 		2016-07-10
Description:
*************************************************/
#include "gui_include.h"

static ITUText* SetNetParamTitleText;
static ITUCalendar* SetNetParamContainer;
static ITUCalendar* SetNetParamHostListContainer;
static ITUCalendar* SetManageIPContainer;
static ITUCalendar* SetServerIPContainer;
static ITULayer* SetProjectLayer;
static ITUText* SetHostIP2Text;
static ITUText* SetHostGateWay2Text;
static ITUText* SetHostNetMask2Text;
static ITUText* SetCenterServerIP2Text;
static ITUText* SetRtspIP2Text;
static ITUText* SetManageIP12Text;
static ITUText* SetManageIP22Text;
static ITUText* SetManageIP32Text;
static ITUTextBox* SetNumKeyBordTextBox;

static uint32 g_ip[IP_MAX];
static IP_TYPE g_ipType = IP_MAX;

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
	uint8 ret = check_ip_to_true(IP_data);
	if (FALSE == ret)
	{
		ShowMsgFailHintSuccessLayer(0, SID_Set_Prj_IP_Address_Err, 0);
	}
	else
	{
		uint32 ip_data = 0;
		ip_data = ipaddr_addr(IP_data);
		uint32 data = ntohl(ip_data);
		if (data != g_ip[g_ipType])
		{
			if (HOST_IPADDR != g_ipType)
			{
				storage_set_netparam(0, g_ipType, data);
			}
			else
			{
				// 存入注册表
				storage_set_netparam(1, HOST_IPADDR, data);
				//net_set_local_param(storage_get_netparam());
			}
			ituTextSetString(IPtext[g_ipType], IP_data);
			g_ip[g_ipType] = data;
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
	}

	if (strcmp(param, "SetNumKeyBordLayer") == 0)
	{
		KeyBordGotoSetNetParam();
	}
	else if (strcmp(param, "SetProjectLayer") == 0)
	{
		for (i = 0; i < IP_MAX; i++)
		{
			g_ip[i] = storage_get_netparam_bytype(HOST_IPADDR + i);
		}

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
		change_ip_to_str(g_ip[MANAGER1_IPADDR + i], tmp);
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
		ituWidgetSetVisible(SetNetParamHostListContainer, false);
		ituWidgetSetVisible(SetNetParamContainer, true);
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

void SetNetParamReset(void)
{
	SetNetParamTitleText = NULL;
}
