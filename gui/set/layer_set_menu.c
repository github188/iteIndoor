/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_menu.c
Author:     	zxc
Version:    	
Date: 		2016-06-13
Description: 设置首页面
History:
1. Date:
Author:
Modification:
*************************************************/
#include "layer_set.h"

static ITUButton* SetMenuDoorPwdButton = NULL;
static ITUButton* SetMenuSecurityPwdButton = NULL;
static ITUButton* SetMenuUserButton = NULL;
static ITUButton* SetMenuProjectButton = NULL;
static ITUButton* SetMenuIPModuleButton = NULL;
static ITULayer* SetPasswordLayer = NULL;
static ITULayer* SetUserLayer = NULL;
static ITULayer* SetProjectLayer = NULL;
static ITULayer* SetIpProtocolHostLayer = NULL;
static ITULayer* SetIpProtocolFenjiLayer = NULL;
static ITUCoverFlow* SetMenuCoverFlow = NULL;

/*************************************************
Function:		SetMenuOnEnter
Description: 	设置界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetMenuOnEnter(ITUWidget* widget, char* param)
{
	if (!SetMenuDoorPwdButton)
	{
		SetMenuDoorPwdButton = ituSceneFindWidget(&theScene, "SetMenuDoorPwdButton");
		assert(SetMenuDoorPwdButton);

		SetPasswordLayer = ituSceneFindWidget(&theScene, "SetPasswordLayer");
		assert(SetPasswordLayer); 

		SetMenuSecurityPwdButton = ituSceneFindWidget(&theScene, "SetMenuSecurityPwdButton");
		assert(SetMenuSecurityPwdButton);

		SetMenuUserButton = ituSceneFindWidget(&theScene, "SetMenuUserButton");
		assert(SetMenuUserButton); 

		SetUserLayer = ituSceneFindWidget(&theScene, "SetUserLayer");
		assert(SetUserLayer); 

		SetMenuProjectButton = ituSceneFindWidget(&theScene, "SetMenuProjectButton");
		assert(SetMenuProjectButton); 

		SetProjectLayer = ituSceneFindWidget(&theScene, "SetProjectLayer");
		assert(SetProjectLayer); 

		SetMenuCoverFlow = ituSceneFindWidget(&theScene, "SetMenuCoverFlow");
		assert(SetMenuCoverFlow);

		SetIpProtocolHostLayer = ituSceneFindWidget(&theScene, "SetIpProtocolHostLayer");
		assert(SetIpProtocolHostLayer);

		SetIpProtocolFenjiLayer = ituSceneFindWidget(&theScene, "SetIpProtocolFenjiLayer");
		assert(SetIpProtocolFenjiLayer);

		SetMenuIPModuleButton = ituSceneFindWidget(&theScene, "SetMenuIPModuleButton");
		assert(SetMenuIPModuleButton);
	}

	if (strcmp(param, "mainLayer") == 0)
	{
		ituCoverFlowGoto(SetMenuCoverFlow, 0);
	}

	return true;
}

/*************************************************
Function:		SetMenuDoorPwdButtonOnMouseUp
Description: 	设置界面按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetMenuPwdButtonOnMouseUp(ITUWidget* widget, char* param)
{
	ITUButton* buttonflag = (ITUButton*)widget;

	if (SetMenuDoorPwdButton == buttonflag)
	{
		UpdataPassType(0);
		KeybordLayerOnShow(SetPasswordLayer, PASS_TYPE_USER, 6, CIPHERTEXT, CANCEL_BTN, NULL, "SetMenuLayer");
	}
	else if (SetMenuSecurityPwdButton == buttonflag)
	{
		UpdataPassType(1);
		KeybordLayerOnShow(SetPasswordLayer, PASS_TYPE_USER, 6, CIPHERTEXT, CANCEL_BTN, NULL, "SetMenuLayer");
	}
	else if (SetMenuUserButton == buttonflag)
	{
		KeybordLayerOnShow(SetUserLayer, PASS_TYPE_USER, 6, CIPHERTEXT, CANCEL_BTN, NULL, "SetMenuLayer");
	}
	else if (SetMenuProjectButton == buttonflag)
	{
		KeybordLayerOnShow(SetProjectLayer, PASS_TYPE_ADMIN, 6, CIPHERTEXT, CANCEL_BTN, NULL, "SetMenuLayer");
	}
	else if (SetMenuIPModuleButton == buttonflag)
	{
		if (is_main_DeviceNo())
		{
			if (!ituWidgetIsVisible(SetIpProtocolHostLayer))
			{
				ituLayerGoto(SetIpProtocolHostLayer);
			}
		}
		else
		{
			if (!ituWidgetIsVisible(SetIpProtocolFenjiLayer))
			{
				ituLayerGoto(SetIpProtocolFenjiLayer);
			}
		}
	}

	return true;
}
