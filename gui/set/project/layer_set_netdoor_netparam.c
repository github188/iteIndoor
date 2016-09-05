/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_netdoor_netparam.c
Author:     	zxc
Version:    	
Date: 		2016-07-24
Description: 网络门前机网络参数设置
History:
1. Date:
Author:
Modification:
*************************************************/
#include "../layer_set.h"

/************************常量定义************************/
#define NETDOOR_LOCAL_IP		0x0A640101 			// 10.100.1.1
#define NETDOOR_LOCAL_GATEIP	0x0A6401FE 			// 10.100.1.254

static ITUText* SetNetDoorNetParamIP2Text = NULL;
static ITUText* SetNetDoorNetParamMask2Text = NULL;
static ITUText* SetNetDoorNetParamGateWay2Text = NULL;
static ITUText* SetNetDoorNetParamMac2Text = NULL;
static ITUText* SetNumKeyBordTextBox = NULL;
static ITULayer* SetNetDoorLayer = NULL;
static ITURadioBox* MsgFailHintSuccess0RadioBox = NULL;
static ITURadioBox* MsgFailHintSuccess1RadioBox = NULL;

static int g_devIndex = 1;		//	1：门前机1；   2：门前机2
static FULL_DEVICE_NO g_netdoorno;
static NET_PARAM g_ndparam;
static NET_PARAM g_old_ndparam;
static int g_nd_ip_flag = 0;	//设置IP类型
static uint8 HWaddr[20] = { 0 };
static uint8 readmacflag = FALSE;					// 设置读MAC时退出该界面时恢复原来的IP值标志位	
static uint8 g_msg_true_flag = FALSE;				// TRUE:保存参数消息框按下，FALSE：获取MAC地址


/*************************************************
Function:			KeyBordGotoSetNetDoorParam
Description: 		键盘界面进入网络门前机
Input:
1.win:   		当前处理的窗口
2.wParam:
Output:
Return:
Others:
*************************************************/
static void KeyBordGotoSetNetDoorParam()
{
	//char tmp[50];
	uint32 ip_data = 0;
	ITUText* IPtext[3] = { SetNetDoorNetParamIP2Text, SetNetDoorNetParamMask2Text, SetNetDoorNetParamGateWay2Text };
	char* IP_data = ituTextGetString(SetNumKeyBordTextBox);
	int ret = IPIsCorrect(IP_data);
	if (FALSE == ret)
	{
		ShowMsgFailHintSuccessLayer(0, SID_Set_Prj_IP_Address_Err, 0);
	}
	else
	{
		uint32 data = IPtoUlong(IP_data);
		switch (g_nd_ip_flag)
		{
		case 0:
			g_ndparam.IP = data;
			break;

		case 1:
			g_ndparam.SubNet = data;
			break;

		case 2:
			g_ndparam.DefaultGateway = data;
			break;
		}
		//memset(tmp, 0, sizeof(tmp));
		//change_ip_to_str(data, tmp);
		ituTextSetString(IPtext[g_nd_ip_flag], IP_data);
	}
}

/*************************************************
Function:		SetNetDoorNetParamContainerOnShowData
Description: 	更新门前机网络参数设置界面数据
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void SetNetDoorNetParamOnShowData()
{
	char tmp[50];

	memset(tmp, 0, sizeof(tmp));
	change_ip_to_str(g_ndparam.IP, tmp);
	ituTextSetString(SetNetDoorNetParamIP2Text, tmp);

	memset(tmp, 0, sizeof(tmp));
	change_ip_to_str(g_ndparam.SubNet, tmp);
	ituTextSetString(SetNetDoorNetParamMask2Text, tmp);

	memset(tmp, 0, sizeof(tmp));
	change_ip_to_str(g_ndparam.DefaultGateway, tmp);
	ituTextSetString(SetNetDoorNetParamGateWay2Text, tmp);

	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp, "%02X-%02X-%02X-%02X-%02X-%02X",
		HWaddr[0], HWaddr[1], HWaddr[2], HWaddr[3], HWaddr[4], HWaddr[5]);
	ituTextSetString(SetNetDoorNetParamMac2Text, tmp);
}

/*************************************************
Function:		set_default_ip
Description: 	设置默认IP与网络门前机同一网段
Input: 		无
Output:		无
Return:		无
Others:
*************************************************/
static void set_default_ip(void)
{
	net_set_bind_ip(NETDOOR_LOCAL_IP, DEFAULT_SUBNET, NETDOOR_LOCAL_GATEIP);
	readmacflag = TRUE;
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
static void OnNetDoorDealNetParam(uint32 wParam)
{
	uint32 ret = 0;

	PNET_PARAM pnet_param = storage_get_netparam();
	switch (wParam)
	{
	case CMD_GATEWAY_GET_NET_PARAM:
		memset(&g_ndparam, 0, sizeof(g_ndparam));
		ret = logic_get_netdevice_netparam(&g_ndparam);

		memset(&g_old_ndparam, 0, sizeof(NET_PARAM));
		memcpy(&g_old_ndparam, &g_ndparam, sizeof(NET_PARAM));

		if (NETDOOR_ECHO_SUCESS == ret)
		{
			SetNetDoorNetParamOnShowData();
		}
		else
		{
			MsgLinkOutTimeOnShow();
		}
		break;

	case CMD_GATEWAY_SET_NET_PARAM:
		//设置网络门前机设备编号以及规则
		ret = logic_set_netdoor_devnoinfo(&g_netdoorno, NULL);
		// 将中心机等参数从本机中取重新设置门前机
		memcpy((char*)&g_ndparam + 12, (char*)pnet_param + 12, sizeof(NET_PARAM)-12);
		if (ENGLISH == storage_get_language())
		{
			g_ndparam.DNS1 = 0xEE;				// 语言,英语
		}
		g_ndparam.DNS2 = 0xCF;					// 视频格式,CIF格式

		//设置网络门前机网络参数
		ret |= logic_set_netdoor_netparam(&g_ndparam);
		if (NETDOOR_ECHO_SUCESS == ret)
		{
			net_set_local_param();
			ShowMsgFailHintSuccessLayer(2, SID_Msg_Save_OK, 0);
		}
		else
		{
			ShowMsgFailHintSuccessLayer(0, SID_Msg_Save_Err, 0);
		}
		storage_set_netdoor_ip(1, (g_devIndex - 1), g_ndparam.IP);
		break;

	case CMD_GATEWAY_GET_MAC:
		set_default_ip();
		DelayMs_nops(100);
		ret = logic_get_netdoor_mac(g_devIndex, HWaddr);
		if (NETDOOR_ECHO_SUCESS == ret)
		{
			memcpy(&g_ndparam, storage_get_netparam(), sizeof(NET_PARAM));
			SetNetDoorNetParamOnShowData();
		}
		else
		{
			MsgLinkOutTimeOnShow();
			if (readmacflag)
			{
				net_set_local_param();
				readmacflag = FALSE;
			}
		}
		break;
	}
}

/*************************************************
Function:		SetNetDoorNetParamOnEnter
Description: 	设置网络门前机网络参数界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetNetDoorNetParamOnEnter(ITUWidget* widget, char* param)
{
	if (!SetNetDoorNetParamIP2Text)
	{
		SetNetDoorNetParamIP2Text = ituSceneFindWidget(&theScene, "SetNetDoorNetParamIP2Text");
		assert(SetNetDoorNetParamIP2Text);

		SetNetDoorNetParamMask2Text = ituSceneFindWidget(&theScene, "SetNetDoorNetParamMask2Text");
		assert(SetNetDoorNetParamMask2Text);

		SetNetDoorNetParamGateWay2Text = ituSceneFindWidget(&theScene, "SetNetDoorNetParamGateWay2Text");
		assert(SetNetDoorNetParamGateWay2Text);

		SetNetDoorNetParamMac2Text = ituSceneFindWidget(&theScene, "SetNetDoorNetParamMac2Text");
		assert(SetNetDoorNetParamMac2Text);

		SetNumKeyBordTextBox = ituSceneFindWidget(&theScene, "SetNumKeyBordTextBox");
		assert(SetNumKeyBordTextBox);

		SetNetDoorLayer = ituSceneFindWidget(&theScene, "SetNetDoorLayer");
		assert(SetNetDoorLayer);

		MsgFailHintSuccess0RadioBox = ituSceneFindWidget(&theScene, "MsgFailHintSuccess0RadioBox");
		assert(MsgFailHintSuccess0RadioBox);

		MsgFailHintSuccess1RadioBox = ituSceneFindWidget(&theScene, "MsgFailHintSuccess1RadioBox");
		assert(MsgFailHintSuccess1RadioBox);
	}

	if (strcmp(param, "SetNumKeyBordLayer") == 0)
	{
		KeyBordGotoSetNetDoorParam();
	}
	else if (strcmp(param, "MsgFailHintSuccessLayer") == 0)
	{
		if (ituRadioBoxIsChecked(MsgFailHintSuccess1RadioBox))	//确认键
		{
			if (TRUE == g_msg_true_flag)
			{
				OnNetDoorDealNetParam(CMD_GATEWAY_SET_NET_PARAM);
				//g_msg_true_flag = FALSE;
			}
			else
			{
				OnNetDoorDealNetParam(CMD_GATEWAY_GET_MAC);
			}
		}
		else
		{
			if (TRUE == g_msg_true_flag)
			{
				g_msg_true_flag = FALSE;
				ituLayerGoto(SetNetDoorLayer);
			}
		}
	}
	else //if (strcmp(param, "SetNetDoorLayer") == 0)
	{
		memset(HWaddr, 0, sizeof(HWaddr));
		memset(&g_ndparam, 0, sizeof(NET_PARAM));
		memcpy(HWaddr, storage_get_mac(DOOR1_MAC + g_devIndex - 1), 6);
		readmacflag = FALSE;
		g_msg_true_flag = FALSE;

		SetNetDoorNetParamOnShowData();
		OnNetDoorDealNetParam(CMD_GATEWAY_GET_NET_PARAM);
	}
	
	return true;
}

/*************************************************
Function:		SetNetDoorNetParamButtonOnMouseUp
Description: 	设置门前IP
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetNetDoorNetParamButtonOnMouseUp(ITUWidget* widget, char* param)
{
	char tmp[40];
	uint32 ip_flag = 0;
	int index = atoi(param);

	if (3 == index)		//设置MAC
	{
		ShowMsgFailHintSuccessLayer(1, SID_Net_Door_Get_MAC, 1);
		return true;
	}

	g_nd_ip_flag = index;

	switch (g_nd_ip_flag)
	{
	case 0:
		ip_flag = g_ndparam.IP;
		break;

	case 1:
		ip_flag = g_ndparam.SubNet;
		break;

	case 2:
		ip_flag = g_ndparam.DefaultGateway;
		break;
	}

	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp, "%s", UlongtoIP(ip_flag));
	KeybordLayerOnShow(NULL, PASS_TYPE_MAX, 15, EXPRESS_CHAR, SPOT_BTN, tmp);

	return true;
}

/*************************************************
Function:		SetNetDoorNetParamLayerOnLeave
Description: 	设置门前IP退出处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetNetDoorNetParamLayerOnLeave(ITUWidget* widget, char* param)
{
	if (readmacflag)
	{
		net_set_local_param();
	}
	readmacflag = FALSE;

	MsgLinkOutTimeLayerOnLeave(widget, param);

	return true;
}

/*************************************************
Function:		set_netdoorparamfrm_Page
Description: 	设置显示页面
Input:
1.pageIndex	索引页1-网络门前机1, 2-网络门前机2
Output:		无
Return:		无
Others:
*************************************************/
void set_netdoorparam_Page(int pageIndex)
{
	char DevText[20];
	PFULL_DEVICE_NO pdoorno = storage_get_devparam();

	g_devIndex = pageIndex;
	g_netdoorno = *pdoorno;
	strncpy(DevText, g_netdoorno.DeviceNoStr, g_netdoorno.DevNoLen - 1);
	switch (g_devIndex)
	{
	case 0:
		DevText[g_netdoorno.DevNoLen - 1] = '0';
		break;

	case 1:
		DevText[g_netdoorno.DevNoLen - 1] = '8';
		break;

	case 2:
		DevText[g_netdoorno.DevNoLen - 1] = '9';
		break;
	}
	DevText[g_netdoorno.DevNoLen] = 0;
	strcpy(g_netdoorno.DeviceNoStr, DevText);
}

/*************************************************
Function:		SetNetDoorLayerOnReturn
Description: 	底部返回按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
void SetNetDoorNetparamLayerOnReturn()
{	
	if (!ituWidgetIsVisible(SetNetDoorLayer))
	{
		if((g_ndparam.IP != g_old_ndparam.IP) || (g_ndparam.SubNet != g_old_ndparam.SubNet) || (g_ndparam.DefaultGateway != g_old_ndparam.DefaultGateway))
		{
			g_msg_true_flag = TRUE;
			ShowMsgFailHintSuccessLayer(1, SID_Msg_Param_Save, 1);
		}
		else
		{
			ituLayerGoto(SetNetDoorLayer);
		}
		
		return;
	}
}
