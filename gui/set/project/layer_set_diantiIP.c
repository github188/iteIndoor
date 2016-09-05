/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_diantiIP.c
Author:     	zxc
Version:
Date: 		2016-07-12
Description:	设置电梯控制器参数
*************************************************/
#include "../layer_set.h"

static ITUText* SetDiantiParamIP2Text = NULL;
static ITUTextBox* SetNumKeyBordTextBox = NULL;

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

		SetNumKeyBordTextBox = ituSceneFindWidget(&theScene, "SetNumKeyBordTextBox");
		assert(SetNumKeyBordTextBox); 
	}

	if (strcmp(param, "SetProjectLayer") == 0)
	{
		g_dianti_ip = storage_get_netparam_bytype(DIANTI_IPADDR);

		sprintf(tmp, "%s", UlongtoIP(g_dianti_ip));
		ituTextSetString(SetDiantiParamIP2Text, tmp);
	}
	else if (strcmp(param, "SetNumKeyBordLayer") == 0)
	{
		char* IP_data = ituTextGetString(SetNumKeyBordTextBox);
		int ret = IPIsCorrect(IP_data);
		if (FALSE == ret)
		{
			ShowMsgFailHintSuccessLayer(0, SID_Set_Prj_IP_Address_Err, 0);
		}
		else
		{
			uint32 data = IPtoUlong(IP_data);
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
	sprintf(tmp, "%s", UlongtoIP(g_dianti_ip));
	KeybordLayerOnShow(NULL, PASS_TYPE_MAX, 15, EXPRESS_CHAR, SPOT_BTN, tmp);

	return true;
}
