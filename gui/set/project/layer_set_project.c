/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_extmodule.c
Author:     	zxc
Version:
Date: 		2016-07-11
Description:	�豸��Ž���
*************************************************/
#include "gui_include.h"

static ITUCoverFlow* SetProjectCoverFlow = NULL;
static ITULayer* SetPasswordLayer = NULL;

/*************************************************
Function:		SetProjectOnEnter
Description: 	���빤�����ý����ʼ������
Input:		��
Output:		��
Return:		TRUE �� FALSE ��
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
Description: 	������������
Input:		��
Output:		��
Return:		TRUE �� FALSE ��
Others:
*************************************************/
bool SetPrjProjectPwdButtonOnMouseUp(ITUWidget* widget, char* param)
{
	UpdataPassType(2);
	ituLayerGoto(SetPasswordLayer);

	return true;
}

