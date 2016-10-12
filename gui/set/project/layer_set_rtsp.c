/*************************************************
Copyright (C),  2006-2016, Aurine
File name:		layer_set_rtsp.c
Author:     	zxc
Version:    	
Date: 			2016-07-19
Description: 监控设置界面
*************************************************/
#include "../layer_set.h"

static ITUCoverFlow* SetRtspCoverFlow = NULL;
static ITUCoverFlow* SetRtspParamCoverFlow = NULL;
static ITUText* SetCamera2Text[MAX_HOME_NUM] = { NULL };
static ITUSprite* SetCameraEnableSprite = NULL;
static ITULayer* SetProjectLayer = NULL;
static ITUTextBox* SetNumKeyBordTextBox = NULL;
static ITUCalendar* CameraDevNameContainer = NULL;
static ITURadioBox* SetRtspFactoryMsg0RadioBox = NULL;
static ITURadioBox* SetRtspFactoryMsg1RadioBox = NULL;
static ITUBackground* SetRtspFactoryMsgBackground = NULL;
static ITUBackground* SetRtspBackground = NULL;
static ITUText* SetRtspParamList[9] = { NULL };
static ITUSprite* SetPTZEnableSprite = NULL;
static ITUText* SetPTZEnable1Text = NULL;

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
		ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_ERROR, SID_Set_Data_Save_Err, "SetRtspLayer");
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
		ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_ERROR, SID_Set_Data_Save_Err, "SetRtspLayer");
	}
	return;
}

/*************************************************
Function:		KeyBordGotoRtspParam
Description: 	键盘进入本页面
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void ShowRtspEnable()
{
	uint8 i;
	for (i = 0; i < MAX_HOME_NUM; i++)
	{
		int used = 0;
		if (i < g_list->Homenum)
		{
			used = g_list->Homedev[i].EnableOpen;
		}
		ituTextSetString(SetCamera2Text[i], get_str(SID_Set_UnUsed + used));
	}
}

/*************************************************
Function:		ShowSetCameraParamList
Description: 	室内摄像头设置列表显示函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void ShowSetCameraParamList(int index)
{
	char text_tmp[50];
	memset(text_tmp, 0, sizeof(text_tmp));

	switch (index)
	{
	case 0:
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
		break;

	case 1:
		if (strcmp(g_homedev.FactoryName, "HIKVISION") == 0)
		{
			ituSetColor(&((ITUWidget*)SetPTZEnable1Text)->color, 255, 255, 255, 255);
			ituSetColor(&((ITUWidget*)SetRtspParamList[1])->color, 255, 255, 255, 255);
		}
		else
		{
			ituSetColor(&((ITUWidget*)SetPTZEnable1Text)->color, 255, 128, 138, 135);//冷灰
			ituSetColor(&((ITUWidget*)SetRtspParamList[1])->color, 255, 192, 192, 192);//灰色
		}

		if (g_homedev.CanControlPTZ)
		{
			ituSpriteGoto(SetPTZEnableSprite, 1);
			sprintf(text_tmp, "%s", get_str(SID_Set_Rtsp_PTZ_Used));
		}
		else
		{
			ituSpriteGoto(SetPTZEnableSprite, 0);
			sprintf(text_tmp, "%s", get_str(SID_Set_Rtsp_PTZ_Unused));
		}
		break;

	case 2:
		strcat(text_tmp, g_homedev.DeviceName);
		break;

	case 3:
		sprintf(text_tmp, "%s", UlongtoIP(g_homedev.DeviceIP));
		break;

	case 4:
		sprintf(text_tmp, "%d", g_homedev.DevPort);
		break;

	case 5:
		sprintf(text_tmp, "%d", g_homedev.ChannelNumber);
		break;

	case 6:
		sprintf(text_tmp, "%s", g_homedev.FactoryName);
		break;

	case 7:
		sprintf(text_tmp, "%s", g_homedev.UserName);
		break;

	case 8:
		sprintf(text_tmp, "%s", g_homedev.Password);
		break;

	default:
		break;
	}

	ituTextSetString(SetRtspParamList[index], text_tmp);
}

/*************************************************
Function:		KeyBordGotoRtspParam
Description: 	键盘进入本页面
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void KeyBordGotoRtspParam()
{
	char* TextBox_data = ituTextGetString(SetNumKeyBordTextBox);
	int ret = FALSE;
	int count_textbox = 0;

	switch (g_camera_param_index)
	{
	case 3:
		ret = IPIsCorrect(TextBox_data);
		if (FALSE == ret)
		{
			ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_ERROR, SID_Set_Prj_IP_Address_Err, "SetRtspLayer");
		}
		else
		{
			uint32 data = IPtoUlong(TextBox_data);
			g_homedev.DeviceIP = data;
		}
		break;

	case 4:
		count_textbox = TextBox_data ? strlen(TextBox_data) : 0;
		if (0 == count_textbox)
		{
			ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_ERROR, SID_Set_Rtsp_Port_Null, "SetRtspLayer");
			return false;
		}
		g_homedev.DevPort = atoi(TextBox_data);
		break;

	case 5:
		count_textbox = TextBox_data ? strlen(TextBox_data) : 0;
		if (0 == count_textbox)
		{
			ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_ERROR, SID_Set_Rtsp_Channel_Null, "SetRtspLayer");
			return false;
		}
		g_homedev.ChannelNumber = atoi(TextBox_data);
		break;

	case 7:
		strcpy(g_homedev.UserName, TextBox_data);
		break;

	case 8:
		strcpy(g_homedev.Password, TextBox_data);
		break;
	}
	save_rtsp_param();
	ShowSetCameraParamList(g_camera_param_index);
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

		SetProjectLayer = ituSceneFindWidget(&theScene, "SetProjectLayer");
		assert(SetProjectLayer);

		SetNumKeyBordTextBox = ituSceneFindWidget(&theScene, "SetNumKeyBordTextBox");
		assert(SetNumKeyBordTextBox);

		SetRtspFactoryMsg0RadioBox = ituSceneFindWidget(&theScene, "SetRtspFactoryMsg0RadioBox");
		assert(SetRtspFactoryMsg0RadioBox);

		SetRtspFactoryMsg1RadioBox = ituSceneFindWidget(&theScene, "SetRtspFactoryMsg1RadioBox");
		assert(SetRtspFactoryMsg1RadioBox);

		SetRtspFactoryMsgBackground = ituSceneFindWidget(&theScene, "SetRtspFactoryMsgBackground");
		assert(SetRtspFactoryMsgBackground);

		SetRtspBackground = ituSceneFindWidget(&theScene, "SetRtspBackground");
		assert(SetRtspBackground);

		SetCameraEnableSprite = ituSceneFindWidget(&theScene, "SetCameraEnableSprite");
		assert(SetCameraEnableSprite);

		SetRtspParamList[0] = ituSceneFindWidget(&theScene, "SetCameraEnable2Text");
		assert(SetRtspParamList[0]);

		SetRtspParamList[1] = ituSceneFindWidget(&theScene, "SetPTZEnable2Text");
		assert(SetRtspParamList[1]);

		SetRtspParamList[2] = ituSceneFindWidget(&theScene, "CameraDevName2Text");
		assert(SetRtspParamList[2]);

		SetRtspParamList[3] = ituSceneFindWidget(&theScene, "CameraIP2Text");
		assert(SetRtspParamList[3]);

		SetRtspParamList[4] = ituSceneFindWidget(&theScene, "CameraPort2Text");
		assert(SetRtspParamList[4]);

		SetRtspParamList[5] = ituSceneFindWidget(&theScene, "CameraVideo2Text");
		assert(SetRtspParamList[5]);

		SetRtspParamList[6] = ituSceneFindWidget(&theScene, "CameraFactoryName2Text");
		assert(SetRtspParamList[6]);

		SetRtspParamList[7] = ituSceneFindWidget(&theScene, "CameraUserName2Text");
		assert(SetRtspParamList[7]);

		SetRtspParamList[8] = ituSceneFindWidget(&theScene, "CameraPwd2Text");
		assert(SetRtspParamList[8]);

		for (i = 0; i < MAX_HOME_NUM; i++)
		{
			memset(textname, 0, sizeof(textname));
			sprintf(textname, "%s%d", "SetCamera2Text", i + 1);
			SetCamera2Text[i] = ituSceneFindWidget(&theScene, textname);
			assert(SetCamera2Text[i]);
		}

		CameraDevNameContainer = ituSceneFindWidget(&theScene, "CameraDevNameContainer");
		assert(CameraDevNameContainer);

		SetPTZEnableSprite = ituSceneFindWidget(&theScene, "SetPTZEnableSprite");
		assert(SetPTZEnableSprite);

		SetPTZEnable1Text = ituSceneFindWidget(&theScene, "SetPTZEnable1Text");
		assert(SetPTZEnable1Text);

		ituWidgetDisable(CameraDevNameContainer);
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
			ShowRtspEnable();
		}

		ituCoverFlowGoto(SetRtspCoverFlow, 0);
		ituWidgetSetVisible(SetRtspParamCoverFlow, false);
		ituWidgetSetVisible(SetRtspFactoryMsgBackground, false);
		ituWidgetSetVisible(SetRtspBackground, true);
		ituWidgetSetVisible(SetRtspCoverFlow, true);
	}
	else if (strcmp(param, "SetNumKeyBordLayer") == 0)
	{
		KeyBordGotoRtspParam();
	}
	
	return true;
}

/*************************************************
Function:		ShowSetCameraParam
Description: 	室内摄像头设置显示函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void ShowSetCameraParam()
{
	char text_tmp[50] = { 0 };
	char tmp[32] = { 0 };
	int i = 0;

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

#if 0 // 926没启用后，数据都清空保存，3066是没启用也可以设置保存，现在做的跟3066一样
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

	for (i = 0; i < 9; i++)
	{
		ShowSetCameraParamList(i);
	}
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

	ShowSetCameraParam();

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
		}
		else
		{
			g_homedev.EnableOpen = 1;
		}

		save_rtsp_param();
		ShowSetCameraParamList(0);
		break;

	case 1:
		if (strcmp(g_homedev.FactoryName, "HIKVISION") == 0)
		{
			if (g_homedev.CanControlPTZ)
			{
				g_homedev.CanControlPTZ = 0;
			}
			else
			{
				g_homedev.CanControlPTZ = 1;
			}

			save_rtsp_param();
			ShowSetCameraParamList(1);
		}
		break;

	case 2:
		break;

	case 3:
		sprintf(text_tmp, "%s", UlongtoIP(g_homedev.DeviceIP));
		KeybordLayerOnShow(NULL, PASS_TYPE_MAX, 15, EXPRESS_CHAR, SPOT_BTN, text_tmp, "SetRtspLayer");
		break;

	case 4:
		sprintf(text_tmp, "%d", g_homedev.DevPort);
		KeybordLayerOnShow(NULL, PASS_TYPE_MAX, 4, EXPRESS_CHAR, CANCEL_BTN, text_tmp, "SetRtspLayer");
		break;

	case 5:
		sprintf(text_tmp, "%d", g_homedev.ChannelNumber);
		KeybordLayerOnShow(NULL, PASS_TYPE_MAX, 3, EXPRESS_CHAR, CANCEL_BTN, text_tmp, "SetRtspLayer");
		break;

	case 6:
		if (strcmp(g_homedev.FactoryName, "HIKVISION") == 0)
		{
			ituRadioBoxSetChecked(SetRtspFactoryMsg0RadioBox, true);
		}
		else if (strcmp(g_homedev.FactoryName, "SelfIPC") == 0)
		{
			ituRadioBoxSetChecked(SetRtspFactoryMsg1RadioBox, true);
		}
		else
		{
			ituRadioBoxSetChecked(SetRtspFactoryMsg0RadioBox, false);
			ituRadioBoxSetChecked(SetRtspFactoryMsg1RadioBox, false);
		}
		ituWidgetDisable(SetRtspBackground);
		ituWidgetSetVisible(SetRtspFactoryMsgBackground, true);
		break;

	case 7:
		sprintf(text_tmp, "%s", g_homedev.UserName);
		KeybordLayerOnShow(NULL, PASS_TYPE_MAX, 12, EXPRESS_CHAR, CANCEL_BTN, text_tmp, "SetRtspLayer");
		break;

	case 8:
		sprintf(text_tmp, "%s", g_homedev.Password);
		KeybordLayerOnShow(NULL, PASS_TYPE_MAX, 12, EXPRESS_CHAR, CANCEL_BTN, text_tmp, "SetRtspLayer");
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
	int index = atoi(param);

	memset(g_homedev.FactoryName, 0, sizeof(g_homedev.FactoryName));
	
	switch (index)
	{
	case 0:
		strcpy(g_homedev.FactoryName, "HIKVISION");
		break;

	case 1:
		strcpy(g_homedev.FactoryName, "SelfIPC");
		g_homedev.CanControlPTZ = 0;
		break;
	}

	save_rtsp_param();
	ShowSetCameraParamList(1);
	ShowSetCameraParamList(6);

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
		ShowRtspEnable();
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
