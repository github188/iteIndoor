/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_menu.c
Author:     	zxc
Version:    	
Date: 		2016-06-13
Description:
History:
1. Date:
Author:
Modification:
*************************************************/
#include "gui_include.h"

//static ITULayer* SetNumKeyBordLayer;
static ITUButton* SetMenuDoorPwdButton;
static ITUButton* SetMenuSecurityPwdButton;
static ITULayer* SetPasswordLayer;
//static ITUBackground* SetMenuBackground;
static ITUButton* SetMenuUserButton;
static ITULayer* SetUserLayer;
static ITUButton* SetMenuProjectButton;
static ITULayer* SetProjectLayer;
static ITUCoverFlow* SetMenuCoverFlow;


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

		//SetNumKeyBordLayer = ituSceneFindWidget(&theScene, "SetNumKeyBordLayer");
		//assert(SetNumKeyBordLayer);

		SetPasswordLayer = ituSceneFindWidget(&theScene, "SetPasswordLayer");
		assert(SetPasswordLayer); 

		SetMenuSecurityPwdButton = ituSceneFindWidget(&theScene, "SetMenuSecurityPwdButton");
		assert(SetMenuSecurityPwdButton);

		//SetMenuBackground = ituSceneFindWidget(&theScene, "SetMenuBackground");
		//assert(SetMenuBackground); 

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
		KeybordLayerOnShow(SetPasswordLayer, PASS_TYPE_USER, 6, 33, 0, NULL);
	}
	else if (SetMenuSecurityPwdButton == buttonflag)
	{
		UpdataPassType(1);
		KeybordLayerOnShow(SetPasswordLayer, PASS_TYPE_USER, 6, 33, 0, NULL);
	}
	else if (SetMenuUserButton == buttonflag)
	{
		KeybordLayerOnShow(SetUserLayer, PASS_TYPE_USER, 6, 33, 0, NULL);
	}
	else if (SetMenuProjectButton == buttonflag)
	{
		KeybordLayerOnShow(SetProjectLayer, PASS_TYPE_ADMIN, 6, 33, 0, NULL);
	}

	return true;
}

void SetMenuReset(void)
{
	SetMenuDoorPwdButton = NULL;
}
