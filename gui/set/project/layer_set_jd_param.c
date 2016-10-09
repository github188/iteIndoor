/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_jd_param.c
Author:     	zxc
Version:    	
Date: 		2016-08-22
Description: 设置家电参数界面
History:
1. Date:
Author:
Modification:
*************************************************/
#include "../layer_set.h"

#define SET_JD_DEV_NAME_ADDR_LIST_MAX			60					// 显示的列表最大条数

typedef enum
{
	JD_DEV_ADDR_PAGE = 0,			// 设备位置界面
	JD_DEV_TYPE_PAGE,				// 设备类型界面
	JD_DEV_LIGHT_ID_PAGE,			// 设置灯光ID界面
	JD_PAGE_MAX,
}JD_PAGE_STATE;

static ITUText* SetJdParamTitleText = NULL;
static ITUText* SetJdParamDeviceName2Text = NULL;
static ITUText* SetJdParamAddress2Text = NULL;
static ITUText* SetJdParamAdjust2Text = NULL;
static ITUContainer* SetJdParamAdjustContainer = NULL;
static ITUContainer* SetJdParamIRLearnContainer = NULL;
static ITULayer* SetJDLayer = NULL;
static ITULayer* SetJdParamLayer = NULL;
static ITUText* SetJdParamEnable2Text = NULL;
static ITUSprite* SetJdParamEnableSprite = NULL;
static ITUSprite* SetJdParamAdjustSprite = NULL;
static ITUText* SetJdParam1Text[4] = { NULL };
static ITUText* SetJdParam2Text[4] = { NULL };
static ITUContainer* SetJdParamContainer[4] = { NULL };
static ITUTextBox* SetNumKeyBordTextBox = NULL;
static ITUBackground* SetJdParamBackground = NULL;
static ITUBackground* SetJdDeviceNameMsgBackground = NULL;
static ITUText* SetJdDeviceNameMsgAddressBtnText = NULL;
static ITUText* SetJdDeviceNameMsgTypeBtnText = NULL;
static ITURadioBox* SetJdDeviceNameMsgAddressRadioBox = NULL;
static ITUBackground* SetJdDeviNameAddrMsgBackground = NULL;
static ITUText* SetJdDeviNameAddrMsgTitleText = NULL;
static ITUText* SetJdDeviNameAddrMsgText[60] = { NULL };
static ITUSprite* SetJdDeviNameAddrMsgSprite[60] = { NULL };
static ITUContainer* SetJdDeviNameAddrMsgListContainer[60] = { NULL };
static ITUCoverFlow* SetJdDeviNameAddrMsgListCoverFlow = NULL;

static uint16 g_Index = 0;	//设置第几个
static PJD_DEV_LIST_INFO g_devlist = NULL;
static AU_JD_DEV_TYPE g_devtype;
static JD_DEV_INFO g_devinfo;
static uint32 g_device_name_old[3] = { 0, 0, 0 };
static JD_PAGE_STATE g_page = JD_PAGE_MAX;

/*************************************************
Function:		save_param
Description: 	保存
Input:
1.win
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static int save_param()
{
	ECHO_STORAGE ret = ECHO_STORAGE_ERR;
	g_devinfo.Index = g_Index;
	g_devinfo.JdType = g_devtype;

#ifdef _IP_MODULE_JD_
	if (get_ipmodule() && JD_FACTORY_ACBUS == storage_get_extmode(EXT_MODE_JD_FACTORY))
	{
		if (strlen(g_devinfo.Name) == 0)
		{
			ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_ERROR, SID_Jd_Set_Dev_Noname, "SetJdParamLayer");
			return FALSE;
		}
	}
	else
#endif
	{
		if (1 == g_devinfo.IsUsed)
		{
			#ifdef _USERDEFINE_JDNAME_
			if (strlen(g_devinfo.Name) == 0)
			#else
			if (g_devinfo.TextIDPos == 0 || g_devinfo.TextIDPos == SID_Jd_None ||
				g_devinfo.TextIDName == 0 || g_devinfo.TextIDName == SID_Jd_None)
			#endif
			{
				ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_ERROR, SID_Jd_Set_Dev_Noname, "SetJdParamLayer");
				return FALSE;
			}
		}
	}

	ret = storage_add_jddev(&g_devinfo);
	switch (ret)
	{
	case ECHO_STORAGE_SAME_NAME:
		ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_ERROR, SID_Jd_Set_Name_Err, "SetJdParamLayer");
		return FALSE;

	case ECHO_STORAGE_SAME_ADDR:
		ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_ERROR, SID_Jd_Set_Adr_Err, "SetJdParamLayer");
		return FALSE;

	case ECHO_STORAGE_ERR:
		ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_ERROR, SID_Msg_Save_Err, "SetJdParamLayer");
		return FALSE;

	case ECHO_STORAGE_OK:
		memcpy(&g_devlist->pjd_dev_info[g_Index], &g_devinfo, sizeof(JD_DEV_INFO));
		return TRUE;
	}
	return FALSE;
}

/*************************************************
Function:		SetJDParamOnEnter
Description: 	界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void ShowSetJDParamEnter()
{
	char tmp[50];
	uint8 i = 0;
	uint16 addr1, addr2;
	uint32 text_id[5] = { SID_Jd_Lamp, SID_Jd_ChuangLian, SID_Jd_KongTiao, SID_Jd_Dianyuan, SID_Jd_Meiqi };

	// 标题
	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp, "%s%d", get_str(text_id[g_devtype]), g_Index + 1);
	ituTextSetString(SetJdParamTitleText, tmp);

	// 启用
	if (g_devinfo.IsUsed)
	{
		ituSpriteGoto(SetJdParamEnableSprite, 1);
		ituTextSetString(SetJdParamEnable2Text, get_str(SID_Set_Used));
		for (i = 0; i < 4; i++)
		{
			ituSetColor(&((ITUWidget*)SetJdParam1Text[i])->color, 255, 255, 255, 255);
			ituSetColor(&((ITUWidget*)SetJdParam2Text[i])->color, 255, 255, 255, 255);
			ituWidgetEnable(SetJdParamContainer[i]);
		}
	}
	else
	{
		ituSpriteGoto(SetJdParamEnableSprite, 0);
		ituTextSetString(SetJdParamEnable2Text, get_str(SID_Set_UnUsed));
		for (i = 0; i < 4; i++)
		{
			ituSetColor(&((ITUWidget*)SetJdParam1Text[i])->color, 255, 128, 138, 135);//冷灰
			ituSetColor(&((ITUWidget*)SetJdParam2Text[i])->color, 255, 192, 192, 192);//灰色
			ituWidgetDisable(SetJdParamContainer[i]);
		}
	}

	// 名称
	memset(tmp, 0, sizeof(tmp));
#ifdef _USERDEFINE_JDNAME_
	if (strlen(g_devinfo.Name) == 0)
	{
		strcpy(tmp, get_str(SID_Jd_None));
	}
#else
	//if (0 == g_devinfo.IsUsed || g_devinfo.TextIDName == 0 || g_devinfo.TextIDPos == 0 ||
	//	g_devinfo.TextIDPos == SID_Jd_None || g_devinfo.TextIDName == SID_Jd_None)
	if (g_devinfo.TextIDName == 0 || g_devinfo.TextIDPos == 0 ||
		g_devinfo.TextIDPos == SID_Jd_None || g_devinfo.TextIDName == SID_Jd_None)
	{
		strcpy(tmp, get_str(SID_Jd_None));
	}
#endif		
	else
	{
#ifdef _USERDEFINE_JDNAME_					// 使用自定义设备英文名称
		strcpy(tmp, g_devinfo.Name);
#else										// 使用预定设备名称
		strcpy(tmp, get_str(g_devinfo.TextIDPos));
		strcat(tmp, get_str(g_devinfo.TextIDName));
#endif		
	}
	ituTextSetString(SetJdParamDeviceName2Text, tmp);

	// 地址
	memset(tmp, 0, sizeof(tmp));
	addr1 = g_devinfo.Address;
	addr1 = (addr1 >> 8);
	addr2 = g_devinfo.Address;
	addr2 &= 0xff;
	sprintf(tmp, "%d", addr2);
	ituTextSetString(SetJdParamAddress2Text, tmp);

	// 是否可调
	memset(tmp, 0, sizeof(tmp));
	if (JD_DEV_LIGHT == g_devtype)
	{
		if (0 == g_devinfo.IsTune)
		{
			strcpy(tmp, get_str(SID_Jd_Dev_Set_UnAdjust));
			ituSpriteGoto(SetJdParamAdjustSprite, 0);
		}
		else
		{
			strcpy(tmp, get_str(SID_Jd_Dev_Set_Adjust));
			if (g_devinfo.IsUsed)
			{
				ituSpriteGoto(SetJdParamAdjustSprite, 1);
			}
			else
			{
				ituSpriteGoto(SetJdParamAdjustSprite, 2);
			}
		}
	}

	if (JD_DEV_KONGTIAO == g_devtype)
	{
		g_devinfo.IsTune = 1;
		strcpy(tmp, get_str(SID_Jd_Dev_Set_Adjust));
		if (g_devinfo.IsUsed)
		{
			ituSpriteGoto(SetJdParamAdjustSprite, 1);
		}
		else
		{
			ituSpriteGoto(SetJdParamAdjustSprite, 2);
		}
	}

	ituTextSetString(SetJdParamAdjust2Text, tmp);
	if ((JD_DEV_LIGHT == g_devtype) || (JD_DEV_KONGTIAO == g_devtype))
	{
		ituWidgetSetVisible(SetJdParamAdjustContainer, true);
	}
	else
	{
		ituWidgetSetVisible(SetJdParamAdjustContainer, false);
	}

	// 空调学习
	if (JD_DEV_KONGTIAO == g_devtype)
	{
		ituWidgetSetVisible(SetJdParamIRLearnContainer, true);
	}
	else
	{
		ituWidgetSetVisible(SetJdParamIRLearnContainer, false);
	}
}

/*************************************************
Function:		KeyBordGotoSetJDParamOnEnterShow
Description: 	键盘设置进入界面显示函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void KeyBordGotoSetJDParamOnEnterShow()
{
	uint16 address = 0;
	char tmp[50];
	char * get_keybord_text = ituTextGetString(SetNumKeyBordTextBox);
	
	address = (uint16)atoi(get_keybord_text);

	if (address > 254)
	{
		ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_ERROR, SID_Jd_Set_Adr_Max_Err, "SetJdParamLayer");
	}
	else
	{
		g_devinfo.Address = address;

		address &= 0xff;
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%d", address);
		ituTextSetString(SetJdParamAddress2Text, tmp);
	}
}

/*************************************************
Function:		SetJDParamOnEnter
Description: 	界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJDParamOnEnter(ITUWidget* widget, char* param)
{
	if (!SetJdParamTitleText)
	{
		SetJdParamTitleText = ituSceneFindWidget(&theScene, "SetJdParamTitleText");
		assert(SetJdParamTitleText);

		SetJdParamDeviceName2Text = ituSceneFindWidget(&theScene, "SetJdParamDeviceName2Text");
		assert(SetJdParamDeviceName2Text);

		SetJdParamAddress2Text = ituSceneFindWidget(&theScene, "SetJdParamAddress2Text");
		assert(SetJdParamAddress2Text);

		SetJdParamAdjust2Text = ituSceneFindWidget(&theScene, "SetJdParamAdjust2Text");
		assert(SetJdParamAdjust2Text);

		SetJdParamBackground = ituSceneFindWidget(&theScene, "SetJdParamBackground");
		assert(SetJdParamBackground);

		SetJdDeviceNameMsgBackground = ituSceneFindWidget(&theScene, "SetJdDeviceNameMsgBackground");
		assert(SetJdDeviceNameMsgBackground);

		SetJdParamAdjustContainer = ituSceneFindWidget(&theScene, "SetJdParamAdjustContainer");
		assert(SetJdParamAdjustContainer);

		SetJdParamIRLearnContainer = ituSceneFindWidget(&theScene, "SetJdParamIRLearnContainer");
		assert(SetJdParamIRLearnContainer);

		SetJDLayer = ituSceneFindWidget(&theScene, "SetJDLayer");
		assert(SetJDLayer);

		SetJdParamEnableSprite = ituSceneFindWidget(&theScene, "SetJdParamEnableSprite");
		assert(SetJdParamEnableSprite);

		SetJdParamAdjustSprite = ituSceneFindWidget(&theScene, "SetJdParamAdjustSprite");
		assert(SetJdParamAdjustSprite);

		SetJdParam1Text[0] = ituSceneFindWidget(&theScene, "SetJdParamDeviceName1Text");
		assert(SetJdParam1Text[0]);

		SetJdParam1Text[1] = ituSceneFindWidget(&theScene, "SetJdParamAddress1Text");
		assert(SetJdParam1Text[1]);

		SetJdParam1Text[2] = ituSceneFindWidget(&theScene, "SetJdParamAdjust1Text");
		assert(SetJdParam1Text[2]);

		SetJdParam1Text[3] = ituSceneFindWidget(&theScene, "SetJdParamIRLearn1Text");
		assert(SetJdParam1Text[3]);

		SetJdParam2Text[0] = SetJdParamDeviceName2Text;
		SetJdParam2Text[1] = SetJdParamAddress2Text;
		SetJdParam2Text[2] = SetJdParamAdjust2Text;
		SetJdParam2Text[3] = ituSceneFindWidget(&theScene, "SetJdParamIRLearn2Text");
		assert(SetJdParam2Text[3]);

		SetJdParamContainer[0] = ituSceneFindWidget(&theScene, "SetJdParamDeviceNameContainer");
		assert(SetJdParamContainer[0]);

		SetJdParamContainer[1] = ituSceneFindWidget(&theScene, "SetJdParamAddressContainer");
		assert(SetJdParamContainer[1]);

		SetJdParamContainer[2] = SetJdParamAdjustContainer;
		SetJdParamContainer[3] = SetJdParamIRLearnContainer;

		SetNumKeyBordTextBox = ituSceneFindWidget(&theScene, "SetNumKeyBordTextBox");
		assert(SetNumKeyBordTextBox);

		SetJdDeviNameAddrMsgBackground = ituSceneFindWidget(&theScene, "SetJdDeviNameAddrMsgBackground");
		assert(SetJdDeviNameAddrMsgBackground);

		SetJdParamEnable2Text = ituSceneFindWidget(&theScene, "SetJdParamEnable2Text");
		assert(SetJdParamEnable2Text);
	}

	if (strcmp(param, "SetJDLayer") == 0)
	{
		memset(&g_devinfo, 0, sizeof(JD_DEV_INFO));
		memcpy(&g_devinfo, &g_devlist->pjd_dev_info[g_Index], sizeof(JD_DEV_INFO));

		ShowSetJDParamEnter();

		ituWidgetSetVisible(SetJdDeviceNameMsgBackground, false);
		ituWidgetSetVisible(SetJdDeviNameAddrMsgBackground, false);
		ituWidgetSetVisible(SetJdParamBackground, true);
		if (!ituWidgetIsEnabled(SetJdParamBackground))
		{
			ituWidgetEnable(SetJdParamBackground);
		}
		if (!ituWidgetIsEnabled(SetJdDeviceNameMsgBackground))
		{
			ituWidgetEnable(SetJdDeviceNameMsgBackground);
		}
	}
	else if (strcmp(param, "SetNumKeyBordLayer") == 0)
	{
		KeyBordGotoSetJDParamOnEnterShow();
	}

	return true;
}

/*************************************************
Function:		SetJdParamEnableButtonOnMouseUp
Description: 	是否启用按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJdParamEnableButtonOnMouseUp(ITUWidget* widget, char* param)
{
	if (g_devinfo.IsUsed)
	{
		g_devinfo.IsUsed = 0;
	}
	else
	{
		g_devinfo.IsUsed = 1;
	}
	ShowSetJDParamEnter();

	return true;
}

/*************************************************
Function:		SetJdParamDeviceNameButtonOnMouseUp
Description: 	设备名称按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJdParamDeviceNameButtonOnMouseUp(ITUWidget* widget, char* param)
{
	if (!SetJdDeviceNameMsgAddressBtnText)
	{
		SetJdDeviceNameMsgAddressBtnText = ituSceneFindWidget(&theScene, "SetJdDeviceNameMsgAddressBtnText");
		assert(SetJdDeviceNameMsgAddressBtnText);

		SetJdDeviceNameMsgTypeBtnText = ituSceneFindWidget(&theScene, "SetJdDeviceNameMsgTypeBtnText");
		assert(SetJdDeviceNameMsgTypeBtnText);

		SetJdDeviceNameMsgAddressRadioBox = ituSceneFindWidget(&theScene, "SetJdDeviceNameMsgAddressRadioBox");
		assert(SetJdDeviceNameMsgAddressRadioBox);
	}

	g_device_name_old[0] = g_devinfo.TextIDPos;
	g_device_name_old[1] = g_devinfo.TextIDName;
	g_device_name_old[2] = g_devinfo.TextID;

	if (0 == g_devinfo.TextIDPos)
	{
		ituTextSetString(SetJdDeviceNameMsgAddressBtnText, get_str(SID_Jd_None));
	}
	else
	{
		ituTextSetString(SetJdDeviceNameMsgAddressBtnText, get_str(g_devinfo.TextIDPos));
	}

	if (0 == g_devinfo.TextIDName)
	{
		ituTextSetString(SetJdDeviceNameMsgTypeBtnText, get_str(SID_Jd_None));
	}
	else
	{
		ituTextSetString(SetJdDeviceNameMsgTypeBtnText, get_str(g_devinfo.TextIDName));
	}
	
	ituRadioBoxSetChecked(SetJdDeviceNameMsgAddressRadioBox, true);
	ituWidgetDisable(SetJdParamBackground);
	ituWidgetSetVisible(SetJdDeviceNameMsgBackground, true);

	return true;
}

/*************************************************
Function:		SetJdDeviceNameMsgTureButtonOnMouseUp
Description: 	确认键
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJdDeviceNameMsgTureButtonOnMouseUp(ITUWidget* widget, char* param)
{
	if (g_device_name_old[1] == 0 || g_device_name_old[1] == SID_Jd_None
		|| g_device_name_old[0] == 0 || g_device_name_old[0] == SID_Jd_None)
	{
		g_devinfo.TextIDPos = SID_Jd_None;
		g_devinfo.TextIDName = SID_Jd_None;
		g_devinfo.TextID = SID_Jd_None;
	}
	else
	{
		g_devinfo.TextIDPos = g_device_name_old[0];
		g_devinfo.TextIDName = g_device_name_old[1];
		g_devinfo.TextID = g_device_name_old[2];
	}

	ShowSetJDParamEnter();
	ituWidgetSetVisible(SetJdDeviceNameMsgBackground, false);
	ituWidgetEnable(SetJdParamBackground);

	return true;
}

/*************************************************
Function:		Show_JD_Dev_Addr_Page
Description: 	显示设备位置界面
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void Show_JD_Dev_Addr_Page()
{
	uint32 i = 0;

	g_page = JD_DEV_ADDR_PAGE;
	ituTextSetString(SetJdDeviNameAddrMsgTitleText, get_str(SID_Jd_Set_Dev_Name1));

	for (i = 0; i < SET_JD_DEV_NAME_ADDR_LIST_MAX; i++)
	{
		if (0 == i)
		{
			ituTextSetString(SetJdDeviNameAddrMsgText[i], get_str(SID_Jd_None));
		}
		else
		{
			ituTextSetString(SetJdDeviNameAddrMsgText[i], get_str(SID_Jd_LouShang + i - 1));
		}
		ituWidgetSetVisible(SetJdDeviNameAddrMsgListContainer[i], true);
	}

	if (g_device_name_old[0] == 0 || g_device_name_old[0] == SID_Jd_None)
	{
		ituSpriteGoto(SetJdDeviNameAddrMsgSprite[0], 1);
	}
	else
	{
		ituSpriteGoto(SetJdDeviNameAddrMsgSprite[0], 0);
	}

	for (i = SID_Jd_LouShang; i < (SID_Jd_DiXiaShi + 1); i++)
	{
		if (g_device_name_old[0] == i)
		{
			ituSpriteGoto(SetJdDeviNameAddrMsgSprite[i - SID_Jd_LouShang + 1], 1);
		}
		else
		{
			ituSpriteGoto(SetJdDeviNameAddrMsgSprite[i - SID_Jd_LouShang + 1], 0);
		}
	}
}

/*************************************************
Function:		Show_JD_Dev_Type_Page
Description: 	显示设备类型界面
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void Show_JD_Dev_Type_Page()
{
	uint32 text_start[5] = { SID_Jd_Deng, SID_Jd_ChuangLian, SID_Jd_KongTiaoJi, SID_Jd_ChaZuo1, SID_Jd_Meiqi };
	uint32 text_end[5] = { SID_Jd_ZhuanshiDEng, SID_Jd_DianDongShaLian, SID_Jd_LenQiJi, SID_Jd_ChaZuo5, SID_Jd_Meiqi };
	uint32 text = 0;
	uint32 i = 0;
	uint32 text_num = text_end[g_devtype] - text_start[g_devtype] + 2;

	g_page = JD_DEV_TYPE_PAGE;
	ituTextSetString(SetJdDeviNameAddrMsgTitleText, get_str(SID_Jd_Set_Dev_Name2));

	for (i = 0; i < text_num; i++)
	{
		if (0 == i)
		{
			ituTextSetString(SetJdDeviNameAddrMsgText[i], get_str(SID_Jd_None));
		}
		else
		{
			ituTextSetString(SetJdDeviNameAddrMsgText[i], get_str(text_start[g_devtype] + i - 1));
		}
		ituWidgetSetVisible(SetJdDeviNameAddrMsgListContainer[i], true);
	}
	for (i = text_num; i < SET_JD_DEV_NAME_ADDR_LIST_MAX; i++)
	{
		ituWidgetSetVisible(SetJdDeviNameAddrMsgListContainer[i], false);
	}

	if (g_devtype != JD_DEV_LIGHT)
	{
		text = g_device_name_old[1];
	}
	else
	{
		text = g_device_name_old[2];
	}

	if (text == 0 || text == SID_Jd_None)
	{
		ituSpriteGoto(SetJdDeviNameAddrMsgSprite[0], 1);
	}
	else
	{
		ituSpriteGoto(SetJdDeviNameAddrMsgSprite[0], 0);
	}

	for (i = text_start[g_devtype]; i < (text_end[g_devtype] + 1); i++)
	{
		if (text == i)
		{
			ituSpriteGoto(SetJdDeviNameAddrMsgSprite[i - text_start[g_devtype] + 1], 1);
		}
		else
		{
			ituSpriteGoto(SetJdDeviNameAddrMsgSprite[i - text_start[g_devtype] + 1], 0);
		}
	}
}

/*************************************************
Function:		SetJdDeviceNameMsgFalseButtonOnMouseUp
Description: 	取消键
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJdDeviceNameMsgFalseButtonOnMouseUp(ITUWidget* widget, char* param)
{
	switch (g_page)
	{
	case JD_DEV_ADDR_PAGE:
	case JD_DEV_TYPE_PAGE:
		g_page = JD_DEV_MAX;
		ituWidgetSetVisible(SetJdDeviNameAddrMsgBackground, false);
		ituWidgetEnable(SetJdDeviceNameMsgBackground);
		break;

	case JD_DEV_LIGHT_ID_PAGE:
		Show_JD_Dev_Type_Page();
		break;

	default:
		g_device_name_old[0] = 0;
		g_device_name_old[1] = 0;
		g_device_name_old[2] = 0;
		ituWidgetSetVisible(SetJdDeviceNameMsgBackground, false);
		ituWidgetEnable(SetJdParamBackground);
		break;
	}

	return true;
}

/*************************************************
Function:		SetJDDevNameSelectPageButtonOnMouseUp
Description: 	页面选择切换
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJDDevNameSelectPageButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 i;
	char tmp[60];

	if (!SetJdDeviNameAddrMsgTitleText)
	{
		SetJdDeviNameAddrMsgTitleText = ituSceneFindWidget(&theScene, "SetJdDeviNameAddrMsgTitleText");
		assert(SetJdDeviNameAddrMsgTitleText);

		SetJdDeviNameAddrMsgListCoverFlow = ituSceneFindWidget(&theScene, "SetJdDeviNameAddrMsgListCoverFlow");
		assert(SetJdDeviNameAddrMsgListCoverFlow);

		for (i = 0; i < 60; i++)
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "SetJdDeviNameAddrMsgText", i);
			SetJdDeviNameAddrMsgText[i] = ituSceneFindWidget(&theScene, tmp);
			assert(SetJdDeviNameAddrMsgText[i]);

			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "SetJdDeviNameAddrMsgSprite", i);
			SetJdDeviNameAddrMsgSprite[i] = ituSceneFindWidget(&theScene, tmp);
			assert(SetJdDeviNameAddrMsgSprite[i]);

			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "SetJdDeviNameAddrMsgListContainer", i);
			SetJdDeviNameAddrMsgListContainer[i] = ituSceneFindWidget(&theScene, tmp);
			assert(SetJdDeviNameAddrMsgListContainer[i]);
		}
	}
	int index = atoi(param);

	switch (index)
	{
	case 0:
		Show_JD_Dev_Addr_Page();
		break;

	case 1:
		Show_JD_Dev_Type_Page();
		break;
	}

	ituCoverFlowGoto(SetJdDeviNameAddrMsgListCoverFlow, 0);
	ituWidgetDisable(SetJdDeviceNameMsgBackground);
	ituWidgetSetVisible(SetJdDeviNameAddrMsgBackground, true);

	return true;
}

/*************************************************
Function:		show_show_light_idpage
Description: 	显示的灯光信息
Input:		无
Output:		无
Return:		TRUE 成功 FALSE 失败
Others:
*************************************************/
static void show_light_id(int index)
{
	uint32 text_start[SID_Jd_ZhuanshiDEng - SID_Jd_Deng + 2] = { SID_Jd_Deng1, SID_Jd_TiaoGuangDeng1, SID_Jd_BaiChiDeng1, SID_Jd_GuangGuan1, SID_Jd_DiaoDeng1, SID_Jd_HuaDeng1, SID_Jd_BiDeng1, SID_Jd_SheDeng1, \
		SID_Jd_TaiDeng1, SID_Jd_LouDiDeng1, SID_Jd_ZhaoMingDeng1, SID_Jd_ShiYingDeng1, SID_Jd_ShiYingDeng1, SID_Jd_YeDeng1, SID_Jd_ZhuanshiDEng1 };

	uint32 i = 0;
	uint32 text_num = 10;
	//uint8 flag = 0;

	g_page = JD_DEV_LIGHT_ID_PAGE;

	for (i = 0; i < text_num; i++)
	{
		ituTextSetString(SetJdDeviNameAddrMsgText[i], get_str(text_start[index] + i));
		ituWidgetSetVisible(SetJdDeviNameAddrMsgListContainer[i], true);
	}
	for (i = text_num; i < SET_JD_DEV_NAME_ADDR_LIST_MAX; i++)
	{
		ituWidgetSetVisible(SetJdDeviNameAddrMsgListContainer[i], false);
	}
	
	for (i = text_start[index]; i < (text_start[index] + 10); i++)
	{
		if (g_device_name_old[1] == i)
		{
			ituSpriteGoto(SetJdDeviNameAddrMsgSprite[i - text_start[index]], 1);
			//flag = 1;
		}
		else
		{
			ituSpriteGoto(SetJdDeviNameAddrMsgSprite[i - text_start[index]], 0);
		}
	}
#if 0
	if ((0 == flag) || (0 == g_device_name_old[1]) || (g_device_name_old[1] == SID_Jd_None))
	{
		ituSpriteGoto(SetJdDeviNameAddrMsgSprite[0], 1);
	}
#endif
	ituCoverFlowGoto(SetJdDeviNameAddrMsgListCoverFlow, 0);
}

/*************************************************
Function:		JDDevAddrPageBtn
Description: 	设备位置按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void JDDevAddrPageBtn(int index)
{
	if (0 == index)
	{
		g_device_name_old[0] = SID_Jd_None;
		ituTextSetString(SetJdDeviceNameMsgAddressBtnText, get_str(SID_Jd_None));
	}
	else
	{
		g_device_name_old[0] = SID_Jd_LouShang + index - 1;
		ituTextSetString(SetJdDeviceNameMsgAddressBtnText, get_str(g_device_name_old[0]));
	}
	g_page = JD_PAGE_MAX;
	ituWidgetSetVisible(SetJdDeviNameAddrMsgBackground, false);
	ituWidgetEnable(SetJdDeviceNameMsgBackground);
}

/*************************************************
Function:		JDDevTypePageBtn
Description: 	设备类型按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void JDDevTypePageBtn(int index)
{
	uint32 text_start[5] = { SID_Jd_Deng, SID_Jd_ChuangLian, SID_Jd_KongTiaoJi, SID_Jd_ChaZuo1, SID_Jd_Meiqi };
	if (0 == index)
	{
		if (g_devtype != JD_DEV_LIGHT)
		{
			g_device_name_old[1] = SID_Jd_None;
		}
		else
		{
			g_device_name_old[1] = SID_Jd_None;
			g_device_name_old[2] = SID_Jd_None;
		}
		ituTextSetString(SetJdDeviceNameMsgTypeBtnText, get_str(SID_Jd_None));
	}
	else
	{
		if (g_devtype != JD_DEV_LIGHT)
		{
			g_device_name_old[1] = text_start[g_devtype] + index - 1;
			ituTextSetString(SetJdDeviceNameMsgTypeBtnText, get_str(g_device_name_old[1]));
		}
		else
		{
			g_device_name_old[2] = text_start[g_devtype] + index - 1;
		}
	}

	if (g_devtype == JD_DEV_LIGHT  && index != 0)
	{
		show_light_id(index - 1);
	}
	else
	{
		g_page = JD_PAGE_MAX;
		ituWidgetSetVisible(SetJdDeviNameAddrMsgBackground, false);
		ituWidgetEnable(SetJdDeviceNameMsgBackground);
	}
}

/*************************************************
Function:		JDDevLightIDPageBtn
Description: 	灯光类型按下
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void JDDevLightIDPageBtn(int index)
{
	uint32 text_start[SID_Jd_ZhuanshiDEng - SID_Jd_Deng + 2] = { SID_Jd_Deng1, SID_Jd_TiaoGuangDeng1, SID_Jd_BaiChiDeng1, SID_Jd_GuangGuan1, SID_Jd_DiaoDeng1, SID_Jd_HuaDeng1, SID_Jd_BiDeng1, SID_Jd_SheDeng1, \
		SID_Jd_TaiDeng1, SID_Jd_LouDiDeng1, SID_Jd_ZhaoMingDeng1, SID_Jd_ShiYingDeng1, SID_Jd_ShiYingDeng1, SID_Jd_YeDeng1, SID_Jd_ZhuanshiDEng1 };
	
	uint32 id = g_device_name_old[2] - SID_Jd_Deng;

	g_device_name_old[1] = text_start[id] + index;
	ituTextSetString(SetJdDeviceNameMsgTypeBtnText, get_str(g_device_name_old[1]));

	g_page = JD_PAGE_MAX;
	ituWidgetSetVisible(SetJdDeviNameAddrMsgBackground, false);
	ituWidgetEnable(SetJdDeviceNameMsgBackground);
}

/*************************************************
Function:		SetJdDeviNameAddrMsgButtonOnMouseUp
Description: 	设备位置、设备类型列表按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJdDeviNameAddrMsgButtonOnMouseUp(ITUWidget* widget, char* param)
{
	int index = atoi(param);

	switch (g_page)
	{
	case JD_DEV_ADDR_PAGE:
		JDDevAddrPageBtn(index);
		break;

	case JD_DEV_TYPE_PAGE:
		JDDevTypePageBtn(index);
		break;

	case JD_DEV_LIGHT_ID_PAGE:
		JDDevLightIDPageBtn(index);
		break;
	}

	return true;
}

/*************************************************
Function:		SetJdParamAddressButtonOnMouseUp
Description: 	地址按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJdParamAddressButtonOnMouseUp(ITUWidget* widget, char* param)
{
	char tmp[50];
	uint16 addr1, addr2;

	addr1 = g_devinfo.Address;
	addr1 = (addr1 >> 8);
	addr2 = g_devinfo.Address;
	addr2 &= 0xff;
	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp, "%d", addr2);

	KeybordLayerOnShow(NULL, PASS_TYPE_MAX, 3, EXPRESS_CHAR, CANCEL_BTN, tmp, "SetJdParamLayer");

	return true;
}

/*************************************************
Function:		SetJdParamAdjustButtonOnMouseUp
Description: 	是否可调按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJdParamAdjustButtonOnMouseUp(ITUWidget* widget, char* param)
{
	char tmp[50];
	memset(tmp, 0, sizeof(tmp));

	if (JD_DEV_LIGHT == g_devtype)
	{
		if (0 == g_devinfo.IsTune)
		{
			g_devinfo.IsTune = 1;
			strcpy(tmp, get_str(SID_Jd_Dev_Set_Adjust));
			ituSpriteGoto(SetJdParamAdjustSprite, 1);
		}
		else
		{
			g_devinfo.IsTune = 0;
			strcpy(tmp, get_str(SID_Jd_Dev_Set_UnAdjust));
			ituSpriteGoto(SetJdParamAdjustSprite, 0);
		}
		ituTextSetString(SetJdParamAdjust2Text, tmp);
	}
	
	return true;
}

/*************************************************
Function:		SetJdParamIRLearnButtonOnMouseUp
Description: 	红外学习按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJdParamIRLearnButtonOnMouseUp(ITUWidget* widget, char* param)
{
	if (JD_DEV_KONGTIAO == g_devtype)
	{
		SetJDLearnIRParam(g_devinfo);
	}
	
	return true;
}

/*************************************************
Function:			SetJDParam
Description: 		家电内容
Input:
1.hDlg:   		当前处理的窗口
Output:
Return:
Others:
*************************************************/
uint32 SetJDParam(PJD_DEV_LIST_INFO devlist, AU_JD_DEV_TYPE devtype, int index)
{
	if (!SetJdParamLayer)
	{
		SetJdParamLayer = ituSceneFindWidget(&theScene, "SetJdParamLayer");
		assert(SetJdParamLayer);
	}
	if (NULL == devlist)
	{
		return FALSE;
	}

	g_Index = index;
	g_devlist = devlist;
	g_devtype = devtype;

	ituLayerGoto(SetJdParamLayer);

	return TRUE;
}

/*************************************************
Function:		SetJDParamLayerOnReturn
Description: 	底部退出键按下执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
void SetJDParamLayerOnReturn(void)
{
	int ret = FALSE;

	if ((ituWidgetIsVisible(SetJdDeviNameAddrMsgBackground)) || (ituWidgetIsVisible(SetJdDeviceNameMsgBackground)))
	{
		SetJdDeviceNameMsgFalseButtonOnMouseUp(NULL, NULL);
		return;
	}
	else if (ituWidgetIsVisible(SetJdParamBackground))
	{
		ret = save_param();
		if (TRUE == ret)
		{
			ituLayerGoto(SetJDLayer);
		}
		return;
	}
}
