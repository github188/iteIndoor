/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_diantiIP.c
Author:     	zxc
Version:
Date: 		2016-07-12
Description:	
*************************************************/
#include "../../gui_include.h"

static ITUText* SetDiantiParamIP2Text;
//static ITULayer* SetNumKeyBordLayer;
static ITUTextBox* SetNumKeyBordTextBox;
static ITUBackground* SetDiantiParamMsgErrBackground;
static ITUBackground* SetDiantiParamBackground;

static uint32 g_dianti_ip = 0;

/*************************************************
Function:		SetDiantiParamOnEnter
Description: 	进入电梯设置界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetDiantiParamOnEnter(ITUWidget* widget, char* param)
{
	char tmp[40] = { 0 };

	if (!SetDiantiParamIP2Text)
	{
		SetDiantiParamIP2Text = ituSceneFindWidget(&theScene, "SetDiantiParamIP2Text");
		assert(SetDiantiParamIP2Text);

		//SetNumKeyBordLayer = ituSceneFindWidget(&theScene, "SetNumKeyBordLayer");
		//assert(SetNumKeyBordLayer); 

		SetNumKeyBordTextBox = ituSceneFindWidget(&theScene, "SetNumKeyBordTextBox");
		assert(SetNumKeyBordTextBox); 

		SetDiantiParamMsgErrBackground = ituSceneFindWidget(&theScene, "SetDiantiParamMsgErrBackground");
		assert(SetDiantiParamMsgErrBackground);

		SetDiantiParamBackground = ituSceneFindWidget(&theScene, "SetDiantiParamBackground");
		assert(SetDiantiParamBackground);
	}

	if (strcmp(param, "SetProjectLayer") == 0)
	{
		g_dianti_ip = storage_get_netparam_bytype(DIANTI_IPADDR);

		change_ip_to_str(g_dianti_ip, tmp);
		ituTextSetString(SetDiantiParamIP2Text, tmp);

		ituWidgetSetVisible(SetDiantiParamMsgErrBackground, false);
		ituWidgetSetVisible(SetDiantiParamBackground, true);
	}
	else
	{
		char* IP_data = ituTextGetString(SetNumKeyBordTextBox);
		uint8 ret = check_ip_to_true(IP_data);
		if (FALSE == ret)
		{
			ituWidgetDisable(SetDiantiParamBackground);
			ituWidgetSetVisible(SetDiantiParamMsgErrBackground, true);
		}
		else
		{
			uint32 ip_data = 0;
			ip_data = ipaddr_addr(IP_data);
			uint32 data = ntohl(ip_data);
			if (data != g_dianti_ip)
			{
				storage_set_netparam(0, DIANTI_IPADDR, data);

				ituTextSetString(SetDiantiParamIP2Text, IP_data);
				g_dianti_ip = data;
			}
			
		}
	}
	
	return true;
}

/*************************************************
Function:		SetDiantiParamIPButtonOnMouseUp
Description: 	按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetDiantiParamIPButtonOnMouseUp(ITUWidget* widget, char* param)
{
	char tmp[40];
	memset(tmp, 0, sizeof(tmp));
	change_ip_to_str(g_dianti_ip, tmp);
	KeybordLayerOnShow(NULL, PASS_TYPE_MAX, 15, EXPRESS_CHAR, SPOT_BTN, tmp);

	return true;
}

void SetDiantiParamLayerReset(void)
{
	SetDiantiParamIP2Text = NULL;
}
