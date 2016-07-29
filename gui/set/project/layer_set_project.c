/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_extmodule.c
Author:     	zxc
Version:
Date: 		2016-07-11
Description:	设备编号界面
*************************************************/
#include "gui_include.h"

static ITUCoverFlow* SetProjectCoverFlow = NULL;
static ITULayer* SetPasswordLayer = NULL;

/*************************************************
Function:		SetProjectOnEnter
Description: 	进入工程设置界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetProjectOnEnter(ITUWidget* widget, char* param)
{
	if (!SetProjectCoverFlow)
	{
		SetProjectCoverFlow = ituSceneFindWidget(&theScene, "SetProjectCoverFlow");
		assert(SetProjectCoverFlow); 

		SetPasswordLayer = ituSceneFindWidget(&theScene, "SetPasswordLayer");
		assert(SetPasswordLayer);
	}

	if (strcmp(param, "SetNumKeyBordLayer") == 0)
	{
		ituCoverFlowGoto(SetProjectCoverFlow, 0);
	}

	return true;
}

/*************************************************
Function:		SetPrjProjectPwdButtonOnMouseUp
Description: 	工程密码设置
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetPrjProjectPwdButtonOnMouseUp(ITUWidget* widget, char* param)
{
	UpdataPassType(2);
	ituLayerGoto(SetPasswordLayer);

	return true;
}

