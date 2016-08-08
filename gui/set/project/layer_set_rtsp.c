/*************************************************
Copyright (C),  2006-2016, Aurine
File name:		layer_set_rtsp.c
Author:     	zxc
Version:    	
Date: 			2016-07-19
Description:
History:
1. Date:
Author:
Modification:
*************************************************/
#include "gui_include.h"

static ITUCoverFlow* SetRtspCoverFlow = NULL;
static ITUCoverFlow* SetRtspParamCoverFlow = NULL;
static ITUText* SetCamera2Text[MAX_HOME_NUM] = { NULL };
static ITUText* SetCameraEnable2Text = NULL;
static ITUText* CameraDevName2Text = NULL;
static ITUText* CameraIP2Text = NULL;
static ITUText* CameraPort2Text = NULL;
static ITUText* CameraVideo2Text = NULL;
static ITUText* CameraFactoryName2Text = NULL;
static ITUText* CameraUserName2Text = NULL;
static ITUText* CameraPwd2Text = NULL;
static ITUText* CameraFactoryName1Text = NULL;
static ITUText* CameraUserName1Text = NULL;
static ITUText* CameraPwd1Text = NULL;
static ITUSprite* SetCameraEnableSprite = NULL;
static ITULayer* SetProjectLayer = NULL;
//static ITULayer* SetNumKeyBordLayer = NULL;
static ITUTextBox* SetNumKeyBordTextBox = NULL;
static ITUCalendar* CameraDevNameContainer = NULL;
static ITURadioBox* SetRtspFactoryMsg0RadioBox = NULL;
static ITURadioBox* SetRtspFactoryMsg1RadioBox = NULL;
static ITUBackground* SetRtspFactoryMsgBackground = NULL;
static ITUBackground* SetRtspBackground = NULL;

static uint32 g_camera_index = -1;							//设置第几个摄像头
static PMonitorDeviceList g_list;
static HOMEDEVICE g_homedev;
static int g_camera_param_index = -1;						//设置参数界面按下第几个


/*************************************************
Function:		save_param
Description: 	保存
Input:
1.win
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void save_rtsp_param()
{
	HOMEDEVICE dev;

	if (NULL == g_list || NULL == g_list->Homedev)
	{
		ShowMsgFailHintSuccessLayer(0, SID_Set_Data_Save_Err, 0);
		return;
	}

	memcpy(&dev, &g_list->Homedev[g_camera_index], sizeof(HOMEDEVICE));
	memcpy(&g_list->Homedev[g_camera_index], &g_homedev, sizeof(HOMEDEVICE));

	dprintf("***********************************************\n");
	dprintf("g_index %x\n", g_camera_index);
	dprintf("g_homedev.DeviceName %s\n", g_homedev.DeviceName);
	dprintf("g_homedev.DeviceIP %x\n", g_homedev.DeviceIP);
	dprintf("g_homedev.DevPort %x\n", g_homedev.DevPort);
	dprintf("g_homedev.UserName %s\n", g_homedev.UserName);
	dprintf("g_homedev.Password %s\n", g_homedev.Password);
	dprintf("***********************************************\n");

	if (ECHO_STORAGE_OK == storage_save_monitordev(HOME_CAMERA, g_list))
	{
	}
	else
	{
		memcpy(&g_list->Homedev[g_camera_index], &dev, sizeof(HOMEDEVICE));
		memcpy(&g_homedev, &dev, sizeof(HOMEDEVICE));
		ShowMsgFailHintSuccessLayer(0, SID_Set_Data_Save_Err, 0);
	}
	return;
}

/*************************************************
Function:		SetRtspOnEnter
Description: 	监视设置界面初始化处理函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool SetRtspOnEnter(ITUWidget* widget, char* param)
{
	uint8 i = 0;

	if (!SetRtspCoverFlow)
	{
		char textname[50];

		SetRtspCoverFlow = ituSceneFindWidget(&theScene, "SetRtspCoverFlow");
		assert(SetRtspCoverFlow);

		SetRtspParamCoverFlow = ituSceneFindWidget(&theScene, "SetRtspParamCoverFlow");
		assert(SetRtspParamCoverFlow);

		SetCameraEnable2Text = ituSceneFindWidget(&theScene, "SetCameraEnable2Text");
		assert(SetCameraEnable2Text);

		CameraDevName2Text = ituSceneFindWidget(&theScene, "CameraDevName2Text");
		assert(CameraDevName2Text);

		CameraIP2Text = ituSceneFindWidget(&theScene, "CameraIP2Text");
		assert(CameraIP2Text);

		CameraPort2Text = ituSceneFindWidget(&theScene, "CameraPort2Text");
		assert(CameraPort2Text);

		CameraVideo2Text = ituSceneFindWidget(&theScene, "CameraVideo2Text");
		assert(CameraVideo2Text);

		CameraFactoryName2Text = ituSceneFindWidget(&theScene, "CameraFactoryName2Text");
		assert(CameraFactoryName2Text);

		CameraUserName2Text = ituSceneFindWidget(&theScene, "CameraUserName2Text");
		assert(CameraUserName2Text);

		CameraPwd2Text = ituSceneFindWidget(&theScene, "CameraPwd2Text");
		assert(CameraPwd2Text);

		CameraFactoryName1Text = ituSceneFindWidget(&theScene, "CameraFactoryName1Text");
		assert(CameraFactoryName1Text);

		CameraUserName1Text = ituSceneFindWidget(&theScene, "CameraUserName1Text");
		assert(CameraUserName1Text);

		CameraPwd1Text = ituSceneFindWidget(&theScene, "CameraPwd1Text");
		assert(CameraPwd1Text); 

		SetCameraEnableSprite = ituSceneFindWidget(&theScene, "SetCameraEnableSprite");
		assert(SetCameraEnableSprite);

		SetProjectLayer = ituSceneFindWidget(&theScene, "SetProjectLayer");
		assert(SetProjectLayer);

		//SetNumKeyBordLayer = ituSceneFindWidget(&theScene, "SetNumKeyBordLayer");
		//assert(SetNumKeyBordLayer); 

		SetNumKeyBordTextBox = ituSceneFindWidget(&theScene, "SetNumKeyBordTextBox");
		assert(SetNumKeyBordTextBox);

		CameraDevNameContainer = ituSceneFindWidget(&theScene, "CameraDevNameContainer");
		assert(CameraDevNameContainer);

		SetRtspFactoryMsg0RadioBox = ituSceneFindWidget(&theScene, "SetRtspFactoryMsg0RadioBox");
		assert(SetRtspFactoryMsg0RadioBox);

		SetRtspFactoryMsg1RadioBox = ituSceneFindWidget(&theScene, "SetRtspFactoryMsg1RadioBox");
		assert(SetRtspFactoryMsg1RadioBox);

		SetRtspFactoryMsgBackground = ituSceneFindWidget(&theScene, "SetRtspFactoryMsgBackground");
		assert(SetRtspFactoryMsgBackground);

		SetRtspBackground = ituSceneFindWidget(&theScene, "SetRtspBackground");
		assert(SetRtspBackground);

		for (i = 0; i < MAX_HOME_NUM; i++)
		{
			memset(textname, 0, sizeof(textname));
			sprintf(textname, "%s%d", "SetCamera2Text", i + 1);
			SetCamera2Text[i] = ituSceneFindWidget(&theScene, textname);
			assert(SetCamera2Text[i]);
		}
	}

	if (strcmp(param, "SetProjectLayer") == 0)
	{
		storage_free_monitordev_memory(&g_list);
		storage_get_monitordev(&g_list);
		if (NULL == g_list)
		{
			return false;
		}
		else
		{
			for (i = 0; i < MAX_HOME_NUM; i++)
			{
				int used = 0;
				if (i < g_list->Homenum)
				{
					used = g_list->Homedev[i].EnableOpen;
				}
				ituTextSetString(SetCamera2Text[i], get_str(SID_Set_UnUsed - used));
			}
		}

		ituCoverFlowGoto(SetRtspCoverFlow, 0);
		ituWidgetDisable(CameraDevNameContainer);
		ituWidgetSetVisible(SetRtspParamCoverFlow, false);
		ituWidgetSetVisible(SetRtspFactoryMsgBackground, false);
		ituWidgetSetVisible(SetRtspBackground, true);
		ituWidgetSetVisible(SetRtspCoverFlow, true);
	}
	else if (strcmp(param, "SetNumKeyBordLayer") == 0)
	{
		char* TextBox_data = ituTextGetString(SetNumKeyBordTextBox);
		uint8 ret = FALSE;
		int count_textbox = 0;

		switch (g_camera_param_index)
		{
		case 2:
			ret = check_ip_to_true(TextBox_data);
			if (FALSE == ret)
			{
				ShowMsgFailHintSuccessLayer(0, SID_Set_Prj_IP_Address_Err, 0);
			}
			else
			{
				uint32 ip_data = 0;
				ip_data = ipaddr_addr(TextBox_data);
				uint32 data = ntohl(ip_data);
				g_homedev.DeviceIP = data;

				ituTextSetString(CameraIP2Text, TextBox_data);
			}
			break;

		case 3:
			count_textbox = TextBox_data ? strlen(TextBox_data) : 0;
			if (0 == count_textbox)
			{
				ShowMsgFailHintSuccessLayer(0, SID_Set_Rtsp_Port_Null, 0);
				return false;
			}
			g_homedev.DevPort = atoi(TextBox_data);

			char tmp[10] = { 0 };
			sprintf(tmp, "%d", g_homedev.DevPort);
			ituTextSetString(CameraPort2Text, tmp);
			//ituTextSetString(CameraPort2Text, TextBox_data);
			
			break;

		case 4:
			count_textbox = TextBox_data ? strlen(TextBox_data) : 0;
			if (0 == count_textbox)
			{
				ShowMsgFailHintSuccessLayer(0, SID_Set_Rtsp_Channel_Null, 0);
				return false;
			}
			g_homedev.ChannelNumber = atoi(TextBox_data);

			char text_tmp[10] = {0};
			sprintf(text_tmp, "%d", g_homedev.ChannelNumber);
			ituTextSetString(CameraVideo2Text, text_tmp);
			//ituTextSetString(CameraVideo2Text, TextBox_data);
			break;

		case 6:
			strcpy(g_homedev.UserName, TextBox_data);

			ituTextSetString(CameraUserName2Text, TextBox_data);
			
			break;

		case 7:
			strcpy(g_homedev.Password, TextBox_data);

			ituTextSetString(CameraPwd2Text, TextBox_data);
			break;
		}
		save_rtsp_param();
	}
	
	return true;
}

/*************************************************
Function:		ShowSetCameraParamList
Description: 	室内摄像头设置显示函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void ShowSetCameraParamList()
{
	char text_tmp[50] = { 0 };
	char tmp[32] = { 0 };

	// 设备名称
	memset(text_tmp, 0, sizeof(text_tmp));
	strcpy(text_tmp, get_str(SID_Set_Rtsp_Indoor_Camera));
	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp, "%d", g_camera_index + 1);
	strcat(text_tmp, tmp);

	memset(&g_homedev, 0, sizeof(HOMEDEVICE));
	if (0 == g_list->Homenum)
	{
		storage_free_monitordev_memory(&g_list);
		storage_get_monitordev(&g_list);
	}

#if 0
	if (g_camera_index < g_list->Homenum && g_list->Homedev[g_camera_index].EnableOpen)
	{
		memcpy(&g_homedev, &g_list->Homedev[g_camera_index], sizeof(HOMEDEVICE));
	}
	else
	{
		g_homedev.EnableOpen = 0;
		g_homedev.DeviceIP = 0;
		g_homedev.DevPort = 0;
		g_homedev.ChannelNumber = 0;
		g_homedev.CanControlPTZ = 0;
		#ifdef _NEW_SELF_IPC_
		memset(g_homedev.FactoryName, 0, sizeof(g_homedev.FactoryName));
		#else
		sprintf(g_homedev.FactoryName, "%s", "SANLI");
		#endif
		// 固定
		sprintf(g_homedev.DeviceName, "%s", text_tmp);
	}
#else
	if (g_camera_index < g_list->Homenum)
	{
		memcpy(&g_homedev, &g_list->Homedev[g_camera_index], sizeof(HOMEDEVICE));
	}

	// 固定
	sprintf(g_homedev.DeviceName, "%s", text_tmp);
#endif

	memset(text_tmp, 0, sizeof(text_tmp));
	if (g_homedev.EnableOpen)
	{
		ituSpriteGoto(SetCameraEnableSprite, 1);
		sprintf(text_tmp, "%s", get_str(SID_Set_Rtsp_Camera_Used));
	}
	else
	{
		ituSpriteGoto(SetCameraEnableSprite, 0);
		sprintf(text_tmp, "%s", get_str(SID_Set_Rtsp_Camera_Unused));
	}
	ituTextSetString(SetCameraEnable2Text, text_tmp);

	memset((char *)text_tmp, 0, sizeof(text_tmp));
	strcat(text_tmp, g_homedev.DeviceName);
	ituTextSetString(CameraDevName2Text, text_tmp);

	memset((char *)text_tmp, 0, sizeof(text_tmp));
	sprintf(text_tmp, "%s", UlongtoIP(g_homedev.DeviceIP));
	ituTextSetString(CameraIP2Text, text_tmp);

	memset((char *)text_tmp, 0, sizeof(text_tmp));
	sprintf(text_tmp, "%d", g_homedev.DevPort);
	ituTextSetString(CameraPort2Text, text_tmp);

	memset((char *)text_tmp, 0, sizeof(text_tmp));
	sprintf(text_tmp, "%d", g_homedev.ChannelNumber);
	ituTextSetString(CameraVideo2Text, text_tmp);

	memset((char *)text_tmp, 0, sizeof(text_tmp));
	sprintf(text_tmp, "%s", g_homedev.FactoryName);
	ituTextSetString(CameraFactoryName2Text, text_tmp);

	memset((char *)text_tmp, 0, sizeof(text_tmp));
	sprintf(text_tmp, "%s", g_homedev.UserName);
	ituTextSetString(CameraUserName2Text, text_tmp);

	memset((char *)text_tmp, 0, sizeof(text_tmp));
	sprintf(text_tmp, "%s", g_homedev.Password);
	ituTextSetString(CameraPwd2Text, text_tmp);
}

/*************************************************
Function:		SetCameraButtonOnMouseUp
Description: 	室内摄像头按下
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetCameraButtonOnMouseUp(ITUWidget* widget, char* param)
{
	g_camera_index = atoi(param);

	ShowSetCameraParamList();

	ituCoverFlowGoto(SetRtspParamCoverFlow, 0);
	ituWidgetSetVisible(SetRtspCoverFlow, false);
	ituWidgetSetVisible(SetRtspParamCoverFlow, true);

	return true;
}

/*************************************************
Function:		SetRtspParamListButtonOnMouseUp
Description: 	室内摄像头参数设置界面按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetRtspParamListButtonOnMouseUp(ITUWidget* widget, char* param)
{
	char text_tmp[50] = { 0 };
	g_camera_param_index = atoi(param);

	memset(text_tmp, 0, sizeof(text_tmp));

	switch (g_camera_param_index)
	{
	case 0:
		if (g_homedev.EnableOpen)
		{
			g_homedev.EnableOpen = 0;
			ituSpriteGoto(SetCameraEnableSprite, 0);
			sprintf(text_tmp, "%s", get_str(SID_Set_Rtsp_Camera_Unused));
		}
		else
		{
			g_homedev.EnableOpen = 1;
			ituSpriteGoto(SetCameraEnableSprite, 1);
			sprintf(text_tmp, "%s", get_str(SID_Set_Rtsp_Camera_Used));
		}

		save_rtsp_param();
		ituTextSetString(SetCameraEnable2Text, text_tmp);
		break;

	case 1:
		break;

	case 2:
		sprintf(text_tmp, "%s", UlongtoIP(g_homedev.DeviceIP));
		KeybordLayerOnShow(NULL, PASS_TYPE_MAX, 15, EXPRESS_CHAR, SPOT_BTN, text_tmp);
		break;

	case 3:
		sprintf(text_tmp, "%d", g_homedev.DevPort);
		KeybordLayerOnShow(NULL, PASS_TYPE_MAX, 4, EXPRESS_CHAR, CANCEL_BTN, text_tmp);
		break;

	case 4:
		sprintf(text_tmp, "%d", g_homedev.ChannelNumber);
		KeybordLayerOnShow(NULL, PASS_TYPE_MAX, 3, EXPRESS_CHAR, CANCEL_BTN, text_tmp);
		break;

	case 5:
		if (strcmp(g_homedev.FactoryName, "HIKVISION") == 0)
		{
			ituRadioBoxSetChecked(SetRtspFactoryMsg0RadioBox, true);
		}
		else if (strcmp(g_homedev.FactoryName, "SelfIPC") == 0)
		{
			ituRadioBoxSetChecked(SetRtspFactoryMsg1RadioBox, true);
		}
		ituWidgetDisable(SetRtspBackground);
		ituWidgetSetVisible(SetRtspFactoryMsgBackground, true);
		break;

	case 6:
		sprintf(text_tmp, "%s", g_homedev.UserName);
		KeybordLayerOnShow(NULL, PASS_TYPE_MAX, 12, EXPRESS_CHAR, CANCEL_BTN, text_tmp);
		break;

	case 7:
		sprintf(text_tmp, "%s", g_homedev.Password);
		KeybordLayerOnShow(NULL, PASS_TYPE_MAX, 12, EXPRESS_CHAR, CANCEL_BTN, text_tmp);
		break;
	}

	return true;
}

/*************************************************
Function:		SetRtspFactoryMsgRadioBoxOnMouseUp
Description: 	厂商设置界面按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetRtspFactoryMsgRadioBoxOnMouseUp(ITUWidget* widget, char* param)
{
	char text_tmp[12] = { 0 };
	int index = atoi(param);

	memset(g_homedev.FactoryName, 0, sizeof(g_homedev.FactoryName));
	
	switch (index)
	{
	case 0:
		strcpy(g_homedev.FactoryName, "HIKVISION");
		break;

	case 1:
		strcpy(g_homedev.FactoryName, "SelfIPC");
		break;
	}

	memset(text_tmp, 0, sizeof(text_tmp));
	sprintf(text_tmp, "%s", g_homedev.FactoryName);
	ituTextSetString(CameraFactoryName2Text, text_tmp);

	save_rtsp_param();

	ituWidgetSetVisible(SetRtspFactoryMsgBackground, false);
	ituWidgetEnable(SetRtspBackground);

	return true;
}

/*************************************************
Function:		SetRtspLayerOnReturn
Description: 	底部退出键按下执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
void SetRtspLayerOnReturn(void)
{
	if (ituWidgetIsVisible(SetRtspFactoryMsgBackground))
	{
		ituWidgetSetVisible(SetRtspFactoryMsgBackground, false);
		ituWidgetEnable(SetRtspBackground);
		return;
	}
	else if (ituWidgetIsVisible(SetRtspParamCoverFlow))
	{
		ituWidgetSetVisible(SetRtspParamCoverFlow, false);
		ituWidgetSetVisible(SetRtspCoverFlow, true);
		return;
	}
	else if (ituWidgetIsVisible(SetRtspCoverFlow))
	{
		if (!ituWidgetIsVisible(SetProjectLayer))
		{
			ituLayerGoto(SetProjectLayer);
			return;
		}
	}
}

/*************************************************
Function:		SetRtspReset
Description: 	
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
void SetRtspReset(void)
{
	SetRtspCoverFlow = NULL;
}
