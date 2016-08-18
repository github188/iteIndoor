/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_project.c
Author:     	zxc
Version:
Date: 		2016-07-11
Description:	�������ý���
*************************************************/
#include "../layer_set.h"

static ITUCoverFlow* SetProjectCoverFlow = NULL;
static ITULayer* SetPasswordLayer = NULL;
static ITURadioBox* MsgFailHintSuccess1RadioBox = NULL;
static ITULayer* SetProjectLayer = NULL;

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

		MsgFailHintSuccess1RadioBox = ituSceneFindWidget(&theScene, "MsgFailHintSuccess1RadioBox");
		assert(MsgFailHintSuccess1RadioBox);

		SetProjectLayer = ituSceneFindWidget(&theScene, "SetProjectLayer");
		assert(SetProjectLayer);
	}

	if (strcmp(param, "SetNumKeyBordLayer") == 0)
	{
		ituCoverFlowGoto(SetProjectCoverFlow, 0);
		if (!ituWidgetIsEnabled(SetProjectLayer))
		{
			ituWidgetEnable(SetProjectLayer);
		}
	}
	else if (strcmp(param, "MsgFailHintSuccessLayer") == 0)
	{
		if (ituRadioBoxIsChecked(MsgFailHintSuccess1RadioBox))	//ȷ�ϼ�
		{
			storage_recover_factory();
			hw_stop_feet_dog();
		}
	}

	return true;
}

/*************************************************
Function:		SetPrjBackButtonOnMouseUp
Description: 	�ָ���������
Input:		��
Output:		��
Return:		TRUE �� FALSE ��
Others:
*************************************************/
bool SetPrjBackButtonOnMouseUp(ITUWidget* widget, char* param)
{
	ShowMsgFailHintSuccessLayer(1, SID_Msg_Return_FactorySet, 1);

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


