/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_jd_scence_param.c
Author:     	zxc
Version:
Date: 		2016-08-29
Description: 家电情景参数设置
*************************************************/
#include "../layer_set.h"

#define SET_JD_SCENE_NAME_ADDR_LIST_MAX			28					// 显示的列表最大条数

typedef enum
{
	JD_SCENCE_NAME_PAGE = 0,
	JD_SCENCE_TYPE_PAGE = 1,
	JD_SCENCE_PAGE_MAX
}JD_SCENCE_PAGE_STATE;

static ITUText* SetJdScenceParamTitleText = NULL;
static ITUSprite* SetJdScenceParamEnableSprite = NULL;
static ITUText* SetJdScenceParamList1Text[3] = { NULL };
static ITUText* SetJdScenceParamList2Text[3] = { NULL };
static ITUText* SetJdScenceParamListContainer[3] = { NULL };
static ITULayer* SetJdScenceParamLayer = NULL; 
static ITURadioBox* SetNumKeyBordTextBox = NULL;
static ITULayer* SetJdScenceParamBackground = NULL;
static ITULayer* SetJdSceneNameMsgBackground = NULL;
static ITUText* SetJdSceneNameMsgAddressBtnText = NULL;
static ITUText* SetJdSceneNameMsgTypeBtnText = NULL;
static ITURadioBox* SetJdSceneNameMsgAddressRadioBox = NULL;
static ITUBackground* SetJdSceneAddrMsgBackground = NULL;
static ITUText* SetJdSceneAddrMsgTitleText = NULL;
static ITUCoverFlow* SetJdSceneAddrMsgListCoverFlow = NULL;
static ITUText* SetJdSceneAddrMsgText[SET_JD_SCENE_NAME_ADDR_LIST_MAX] = { NULL };
static ITUSprite* SetJdSceneAddrMsgSprite[SET_JD_SCENE_NAME_ADDR_LIST_MAX] = { NULL };
static ITUText* SetJdSceneAddrMsgListContainer[SET_JD_SCENE_NAME_ADDR_LIST_MAX] = { NULL };
static ITULayer* SetJdScenceLayer = NULL;

static JD_SCENE_INFO g_sceneinfo;
static PJD_SCENE_INFO_LIST g_SceneList = NULL;
static int g_selectIndex = 0;
static JD_SCENCE_PAGE_STATE g_page = JD_SCENCE_PAGE_MAX;
static uint32 g_text_new[2] = { 0, 0 };

/*************************************************
Function:		ShowSetJdScenceParamEnter
Description: 	界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void ShowSetJdScenceParamEnter()
{
	char tmp[50];
	uint8 i = 0;
	uint16 addr2 = 0;
	uint32 TextID[JD_SCENE_MAX] = { SID_Jd_Mode_Huike, SID_Jd_Mode_Jiucan, SID_Jd_Mode_Yejian, SID_Jd_Mode_Jiedian, SID_Jd_Mode_Putong };

	// 标题
	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp, "%s%d", get_str(SID_Jd_Mode_Self), g_selectIndex + 1);
	ituTextSetString(SetJdScenceParamTitleText, tmp);

	if (g_selectIndex >= 5)
	{
		// 启用
		ituSetColor(&((ITUWidget*)SetJdScenceParamList1Text[0])->color, 255, 255, 255, 255);
		ituSetColor(&((ITUWidget*)SetJdScenceParamList2Text[0])->color, 255, 255, 255, 255);
		ituWidgetEnable(SetJdScenceParamListContainer[0]);
		if (g_sceneinfo.IsUsed)
		{
			ituSpriteGoto(SetJdScenceParamEnableSprite, 1);
			ituTextSetString(SetJdScenceParamList2Text[0], get_str(SID_Set_Used));

			for (i = 1; i < 3; i++)
			{
				ituSetColor(&((ITUWidget*)SetJdScenceParamList1Text[i])->color, 255, 255, 255, 255);
				ituSetColor(&((ITUWidget*)SetJdScenceParamList2Text[i])->color, 255, 255, 255, 255);
				ituWidgetEnable(SetJdScenceParamListContainer[i]);
			}
		}
		else
		{
			ituSpriteGoto(SetJdScenceParamEnableSprite, 0);
			ituTextSetString(SetJdScenceParamList2Text[0], get_str(SID_Set_UnUsed));
			for (i = 1; i < 3; i++)
			{
				ituSetColor(&((ITUWidget*)SetJdScenceParamList1Text[i])->color, 255, 128, 138, 135);//冷灰
				ituSetColor(&((ITUWidget*)SetJdScenceParamList2Text[i])->color, 255, 192, 192, 192);//灰色
				ituWidgetDisable(SetJdScenceParamListContainer[i]);
			}
		}

		// 名称
		memset(tmp, 0, sizeof(tmp));
		#ifdef _IP_MODULE_JD_
		if (JD_FACTORY_ACBUS == storage_get_extmode(EXT_MODE_JD_FACTORY))
		{
			if (get_ipmodule())
			{
				if (strlen(g_sceneinfo.SceneName) == 0)
				{
					strcpy(tmp, get_str(SID_Jd_None));
				}
			}
			else
			{
				//if (0 == g_sceneinfo.IsUsed || g_sceneinfo.TextIDName == 0 || g_sceneinfo.TextIDPos == 0 ||
				//	g_sceneinfo.TextIDPos == SID_Jd_None || g_sceneinfo.TextIDName == SID_Jd_None)
				if (g_sceneinfo.TextIDName == 0 || g_sceneinfo.TextIDPos == 0 ||
					g_sceneinfo.TextIDPos == SID_Jd_None || g_sceneinfo.TextIDName == SID_Jd_None)
				{
					strcpy(tmp, get_str(SID_Jd_None));
				}
				else
				{	
					strcpy(tmp, get_str(g_sceneinfo.TextIDPos));
					strcat(tmp, get_str(g_sceneinfo.TextIDName));
				}
			}
		}
		else
		#endif	
		{
			#ifdef _USERDEFINE_JDNAME_
			if (strlen(g_sceneinfo.SceneName) == 0)
			{
				strcpy(tmp, get_str(SID_Jd_None));
			}
			#else
			//if (0 == g_sceneinfo.IsUsed || g_sceneinfo.TextIDName == 0 || g_sceneinfo.TextIDPos == 0 ||
			//	g_sceneinfo.TextIDPos == SID_Jd_None || g_sceneinfo.TextIDName == SID_Jd_None)
			if (g_sceneinfo.TextIDName == 0 || g_sceneinfo.TextIDPos == 0 ||
				g_sceneinfo.TextIDPos == SID_Jd_None || g_sceneinfo.TextIDName == SID_Jd_None)
			{
				strcpy(tmp, get_str(SID_Jd_None));
			}
			else
			{
				strcpy(tmp, get_str(g_sceneinfo.TextIDPos));
				strcat(tmp, get_str(g_sceneinfo.TextIDName));
			}
			#endif
		}
		ituTextSetString(SetJdScenceParamList2Text[1], tmp);

		//地址
		memset(tmp, 0, sizeof(tmp));
		addr2 = g_sceneinfo.Address;
		addr2 &= 0xff;
		sprintf(tmp, "%d", addr2);
		ituTextSetString(SetJdScenceParamList2Text[2], tmp);

	}
	else
	{
		ituSpriteGoto(SetJdScenceParamEnableSprite, 2);
		ituTextSetString(SetJdScenceParamList2Text[0], get_str(SID_Set_Used));
		ituTextSetString(SetJdScenceParamList2Text[1], get_str(TextID[g_selectIndex]));

		addr2 = g_sceneinfo.Address;
		addr2 &= 0xff;
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%d", addr2);
		ituTextSetString(SetJdScenceParamList2Text[2], tmp);

		for (i = 0; i < 3; i++)
		{
			ituSetColor(&((ITUWidget*)SetJdScenceParamList1Text[i])->color, 255, 128, 138, 135);//冷灰
			ituSetColor(&((ITUWidget*)SetJdScenceParamList2Text[i])->color, 255, 192, 192, 192);//灰色
			ituWidgetDisable(SetJdScenceParamListContainer[i]);
		}
	}
}

/*************************************************
Function:		KeyBordGotoSetJDScenceParamOnEnterShow
Description: 	键盘设置进入界面显示函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void KeyBordGotoSetJDScenceParamOnEnterShow()
{
	uint16 address = 0;
	char tmp[50];

	if (!SetNumKeyBordTextBox)
	{
		SetNumKeyBordTextBox = ituSceneFindWidget(&theScene, "SetNumKeyBordTextBox");
		assert(SetNumKeyBordTextBox);
	}
	char * get_keybord_text = ituTextGetString(SetNumKeyBordTextBox);

	address = (uint16)atoi(get_keybord_text);

	if ((address > 254) && (address < 16))
	{
		ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_ERROR, SID_Jd_Set_Adr_Max_Err, "SetJdScenceParamLayer");
	}
	else
	{
		g_sceneinfo.Address = address;

		address &= 0xff;
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%d", address);
		ituTextSetString(SetJdScenceParamList2Text[2], tmp);
	}
}

/*************************************************
Function:		SetJdScenceParamOnEnter
Description: 	界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJdScenceParamOnEnter(ITUWidget* widget, char* param)
{
	if (!SetJdScenceParamTitleText)
	{
		SetJdScenceParamTitleText = ituSceneFindWidget(&theScene, "SetJdScenceParamTitleText");
		assert(SetJdScenceParamTitleText);

		SetJdScenceParamEnableSprite = ituSceneFindWidget(&theScene, "SetJdScenceParamEnableSprite");
		assert(SetJdScenceParamEnableSprite);

		SetJdScenceParamList1Text[0] = ituSceneFindWidget(&theScene, "SetJdScenceParamEnable1Text");
		assert(SetJdScenceParamList1Text[0]);

		SetJdScenceParamList1Text[1] = ituSceneFindWidget(&theScene, "SetJdScenceParamName1Text");
		assert(SetJdScenceParamList1Text[1]);

		SetJdScenceParamList1Text[2] = ituSceneFindWidget(&theScene, "SetJdScenceParamAddress1Text");
		assert(SetJdScenceParamList1Text[2]);

		SetJdScenceParamList2Text[0] = ituSceneFindWidget(&theScene, "SetJdScenceParamEnable2Text");
		assert(SetJdScenceParamList2Text[0]);

		SetJdScenceParamList2Text[1] = ituSceneFindWidget(&theScene, "SetJdScenceParamName2Text");
		assert(SetJdScenceParamList2Text[1]);

		SetJdScenceParamList2Text[2] = ituSceneFindWidget(&theScene, "SetJdScenceParamAddress2Text");
		assert(SetJdScenceParamList2Text[2]);

		SetJdScenceParamListContainer[0] = ituSceneFindWidget(&theScene, "SetJdScenceParamEnableContainer");
		assert(SetJdScenceParamListContainer[0]);

		SetJdScenceParamListContainer[1] = ituSceneFindWidget(&theScene, "SetJdScenceParamNameContainer");
		assert(SetJdScenceParamListContainer[1]);
		
		SetJdScenceParamListContainer[2] = ituSceneFindWidget(&theScene, "SetJdScenceParamAddressContainer");
		assert(SetJdScenceParamListContainer[2]);

		SetJdScenceParamBackground = ituSceneFindWidget(&theScene, "SetJdScenceParamBackground");
		assert(SetJdScenceParamBackground);

		SetJdSceneNameMsgBackground = ituSceneFindWidget(&theScene, "SetJdSceneNameMsgBackground");
		assert(SetJdSceneNameMsgBackground);

		SetJdSceneAddrMsgBackground = ituSceneFindWidget(&theScene, "SetJdSceneAddrMsgBackground");
		assert(SetJdSceneAddrMsgBackground);

		SetJdScenceLayer = ituSceneFindWidget(&theScene, "SetJdScenceLayer");
		assert(SetJdScenceLayer);
	}

	if (strcmp(param, "SetJdScenceLayer") == 0)
	{
		memset(&g_sceneinfo, 0, sizeof(JD_SCENE_INFO));
		memcpy(&g_sceneinfo, &g_SceneList->pjd_scene_info[g_selectIndex], sizeof(JD_SCENE_INFO));

		ShowSetJdScenceParamEnter();

		ituWidgetSetVisible(SetJdSceneNameMsgBackground, false);
		ituWidgetSetVisible(SetJdSceneAddrMsgBackground, false);
		ituWidgetSetVisible(SetJdScenceParamBackground, true);
		if (!ituWidgetIsEnabled(SetJdScenceParamBackground))
		{
			ituWidgetEnable(SetJdScenceParamBackground);
		}
		if (!ituWidgetIsEnabled(SetJdSceneNameMsgBackground))
		{
			ituWidgetEnable(SetJdSceneNameMsgBackground);
		}
	}
	else if (strcmp(param, "SetNumKeyBordLayer") == 0)
	{
		KeyBordGotoSetJDScenceParamOnEnterShow();
	}

	return true;
}

/*************************************************
Function:		SetJdScenceParamEnableButtonOnMouseUp
Description: 	是否启用按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJdScenceParamEnableButtonOnMouseUp(ITUWidget* widget, char* param)
{
	if (g_sceneinfo.IsUsed)
	{
		g_sceneinfo.IsUsed = 0;
	}
	else
	{
		g_sceneinfo.IsUsed = 1;
	}
	ShowSetJdScenceParamEnter();

	return true;
}

/*************************************************
Function:		SetJdScenceParamNameButtonOnMouseUp
Description: 	名称按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJdScenceParamNameButtonOnMouseUp(ITUWidget* widget, char* param)
{
	if (!SetJdSceneNameMsgAddressBtnText)
	{
		SetJdSceneNameMsgAddressBtnText = ituSceneFindWidget(&theScene, "SetJdSceneNameMsgAddressBtnText");
		assert(SetJdSceneNameMsgAddressBtnText);

		SetJdSceneNameMsgTypeBtnText = ituSceneFindWidget(&theScene, "SetJdSceneNameMsgTypeBtnText");
		assert(SetJdSceneNameMsgTypeBtnText);

		SetJdSceneNameMsgAddressRadioBox = ituSceneFindWidget(&theScene, "SetJdSceneNameMsgAddressRadioBox");
		assert(SetJdSceneNameMsgAddressRadioBox);
	}

	g_text_new[0] = g_sceneinfo.TextIDPos;
	g_text_new[1] = g_sceneinfo.TextIDName;

	if (0 == g_sceneinfo.TextIDPos)
	{
		ituTextSetString(SetJdSceneNameMsgAddressBtnText, get_str(SID_Jd_None));
	}
	else
	{
		ituTextSetString(SetJdSceneNameMsgAddressBtnText, get_str(g_sceneinfo.TextIDPos));
	}

	if (0 == g_sceneinfo.TextIDName)
	{
		ituTextSetString(SetJdSceneNameMsgTypeBtnText, get_str(SID_Jd_None));
	}
	else
	{
		ituTextSetString(SetJdSceneNameMsgTypeBtnText, get_str(g_sceneinfo.TextIDName));
	}

	ituRadioBoxSetChecked(SetJdSceneNameMsgAddressRadioBox, true);
	ituWidgetDisable(SetJdScenceParamBackground);
	ituWidgetSetVisible(SetJdSceneNameMsgBackground, true);

	return true;
}

/*************************************************
Function:		SetJdScenceParamAddressButtonOnMouseUp
Description: 	地址按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJdScenceParamAddressButtonOnMouseUp(ITUWidget* widget, char* param)
{
	char tmp[50];
	uint16 addr1, addr2;

	memset(tmp, 0, sizeof(tmp));
	addr2 = g_sceneinfo.Address;
	addr2 &= 0xff;
	sprintf(tmp, "%d", addr2);

	KeybordLayerOnShow(NULL, PASS_TYPE_MAX, 3, EXPRESS_CHAR, CANCEL_BTN, tmp, "SetJdScenceParamLayer");

	return true;
}

/*************************************************
Function:		SetJdSceneNameMsgTureButtonOnMouseUp
Description: 	确认键
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJdSceneNameMsgTureButtonOnMouseUp(ITUWidget* widget, char* param)
{
	if (g_text_new[1] == 0 || g_text_new[1] == SID_Jd_None
		|| g_text_new[0] == 0 || g_text_new[0] == SID_Jd_None)
	{
		g_sceneinfo.TextIDPos = SID_Jd_None;
		g_sceneinfo.TextIDName = SID_Jd_None;
	}
	else
	{
		g_sceneinfo.TextIDPos = g_text_new[0];
		g_sceneinfo.TextIDName = g_text_new[1];
	}

	ShowSetJdScenceParamEnter();
	ituWidgetSetVisible(SetJdSceneNameMsgBackground, false);
	ituWidgetEnable(SetJdScenceParamBackground);

	return true;
}

/*************************************************
Function:		SetJdSceneNameMsgFalseButtonOnMouseUp
Description: 	取消键
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJdSceneNameMsgFalseButtonOnMouseUp(ITUWidget* widget, char* param)
{
	switch (g_page)
	{
	case JD_SCENCE_NAME_PAGE:
	case JD_SCENCE_TYPE_PAGE:
		g_page = JD_SCENCE_PAGE_MAX;
		ituWidgetSetVisible(SetJdSceneAddrMsgBackground, false);
		ituWidgetEnable(SetJdSceneNameMsgBackground);
		break;

	default:
		g_text_new[0] = 0;
		g_text_new[1] = 0;
		ituWidgetSetVisible(SetJdSceneNameMsgBackground, false);
		ituWidgetEnable(SetJdScenceParamBackground);
		break;
	}

	return true;
}

/*************************************************
Function:		Show_JD_Scence_Addr_Page
Description: 	显示情景名字界面
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void Show_JD_Scence_Addr_Page()
{
	uint32 i = 0;

	g_page = JD_SCENCE_NAME_PAGE;
	ituTextSetString(SetJdSceneAddrMsgTitleText, get_str(SID_Jd_Mode_Set_Name1));

	for (i = 0; i < SET_JD_SCENE_NAME_ADDR_LIST_MAX; i++)
	{
		dprintf("i = %d\n", i);
		if (0 == i)
		{
			ituTextSetString(SetJdSceneAddrMsgText[i], get_str(SID_Jd_None));
		}
		else
		{
			ituTextSetString(SetJdSceneAddrMsgText[i], get_str(SID_Jd_Mode_Out + i - 1));
		}
		ituWidgetSetVisible(SetJdSceneAddrMsgListContainer[i], true);
	}

	if (g_text_new[0] == 0 || g_text_new[0] == SID_Jd_None)
	{
		ituSpriteGoto(SetJdSceneAddrMsgSprite[0], 1);
	}
	else
	{
		ituSpriteGoto(SetJdSceneAddrMsgSprite[0], 0);
	}

	for (i = SID_Jd_Mode_Out; i < (SID_Jd_Mode_Warm + 1); i++)
	{
		if (g_text_new[0] == i)
		{
			ituSpriteGoto(SetJdSceneAddrMsgSprite[i - SID_Jd_Mode_Out + 1], 1);
		}
		else
		{
			ituSpriteGoto(SetJdSceneAddrMsgSprite[i - SID_Jd_Mode_Out + 1], 0);
		}
	}
}

/*************************************************
Function:		Show_JD_Scence_Type_Page
Description: 	显示情景类型界面
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void Show_JD_Scence_Type_Page()
{
	uint32 i = 0;
	uint32 text_num = SID_Jd_Mode_Changjing - SID_Jd_Mode_Mode + 2;

	g_page = JD_SCENCE_TYPE_PAGE;
	ituTextSetString(SetJdSceneAddrMsgTitleText, get_str(SID_Jd_Mode_Set_Name2));

	for (i = 0; i < text_num; i++)
	{
		if (0 == i)
		{
			ituTextSetString(SetJdSceneAddrMsgText[i], get_str(SID_Jd_None));
		}
		else
		{
			ituTextSetString(SetJdSceneAddrMsgText[i], get_str(SID_Jd_Mode_Mode + i - 1));
		}
		ituWidgetSetVisible(SetJdSceneAddrMsgListContainer[i], true);
	}
	for (i = text_num; i < SET_JD_SCENE_NAME_ADDR_LIST_MAX; i++)
	{
		ituWidgetSetVisible(SetJdSceneAddrMsgListContainer[i], false);
	}
	
	if (g_text_new[1] == 0 || g_text_new[1] == SID_Jd_None)
	{
		ituSpriteGoto(SetJdSceneAddrMsgSprite[0], 1);
	}
	else
	{
		ituSpriteGoto(SetJdSceneAddrMsgSprite[0], 0);
	}

	for (i = SID_Jd_Mode_Mode; i < (SID_Jd_Mode_Changjing + 1); i++)
	{
		if (g_text_new[1] == i)
		{
			ituSpriteGoto(SetJdSceneAddrMsgSprite[i - SID_Jd_Mode_Mode + 1], 1);
		}
		else
		{
			ituSpriteGoto(SetJdSceneAddrMsgSprite[i - SID_Jd_Mode_Mode + 1], 0);
		}
	}
}

/*************************************************
Function:		SetJdSceneNameMsgButtonOnMouseUp
Description: 	页面选择切换
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJdSceneNameMsgButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 i;
	char tmp[60];

	if (!SetJdSceneAddrMsgTitleText)
	{
		SetJdSceneAddrMsgTitleText = ituSceneFindWidget(&theScene, "SetJdSceneAddrMsgTitleText");
		assert(SetJdSceneAddrMsgTitleText);

		SetJdSceneAddrMsgListCoverFlow = ituSceneFindWidget(&theScene, "SetJdSceneAddrMsgListCoverFlow");
		assert(SetJdSceneAddrMsgListCoverFlow);

		for (i = 0; i < SET_JD_SCENE_NAME_ADDR_LIST_MAX; i++)
		{
			dprintf("i = %d\n", i);
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "SetJdSceneAddrMsgText", i);
			SetJdSceneAddrMsgText[i] = ituSceneFindWidget(&theScene, tmp);
			assert(SetJdSceneAddrMsgText[i]);

			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "SetJdSceneAddrMsgSprite", i);
			SetJdSceneAddrMsgSprite[i] = ituSceneFindWidget(&theScene, tmp);
			assert(SetJdSceneAddrMsgSprite[i]);

			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "SetJdSceneAddrMsgListContainer", i);
			SetJdSceneAddrMsgListContainer[i] = ituSceneFindWidget(&theScene, tmp);
			assert(SetJdSceneAddrMsgListContainer[i]);
		}
	}
	int index = atoi(param);

	switch (index)
	{
	case 0:
		Show_JD_Scence_Addr_Page();
		break;

	case 1:
		Show_JD_Scence_Type_Page();
		break;
	}

	ituCoverFlowGoto(SetJdSceneAddrMsgListCoverFlow, 0);
	ituWidgetDisable(SetJdSceneNameMsgBackground);
	ituWidgetSetVisible(SetJdSceneAddrMsgBackground, true);

	return true;
}

/*************************************************
Function:		JDSceneAddrPageBtn
Description: 	情景名字列表按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void JDSceneAddrPageBtn(int index)
{
	if (0 == index)
	{
		g_text_new[0] = SID_Jd_None;
	}
	else
	{
		g_text_new[0] = SID_Jd_Mode_Out + index - 1;
	}
	ituTextSetString(SetJdSceneNameMsgAddressBtnText, get_str(g_text_new[0]));

	g_page = JD_SCENCE_PAGE_MAX;
	ituWidgetSetVisible(SetJdSceneAddrMsgBackground, false);
	ituWidgetEnable(SetJdSceneNameMsgBackground);
}

/*************************************************
Function:		JDSceneTypePageBtn
Description: 	情景类型按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void JDSceneTypePageBtn(int index)
{
	if (0 == index)
	{
		g_text_new[1] = SID_Jd_None;
	}
	else
	{
		g_text_new[1] = SID_Jd_Mode_Mode + index - 1;
	}
	ituTextSetString(SetJdSceneNameMsgTypeBtnText, get_str(g_text_new[1]));

	g_page = JD_SCENCE_PAGE_MAX;
	ituWidgetSetVisible(SetJdSceneAddrMsgBackground, false);
	ituWidgetEnable(SetJdSceneNameMsgBackground);
}

/*************************************************
Function:		SetJdSceneAddrMsgButtonOnMouseUp
Description: 	情景名字、情景类型列表按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJdSceneAddrMsgButtonOnMouseUp(ITUWidget* widget, char* param)
{
	int index = atoi(param);

	switch (g_page)
	{
	case JD_SCENCE_NAME_PAGE:
		JDSceneAddrPageBtn(index);
		break;

	case JD_SCENCE_TYPE_PAGE:
		JDSceneTypePageBtn(index);
		break;
	}

	return true;
}

/*************************************************
Function:			SetJDScene
Description: 		家电情景设置参数主窗口
Input:
1.win:   		当前处理的窗口
Output:
Return:
Others:
*************************************************/
uint32 SetAurinJDSceneList(uint8 Index, PJD_SCENE_INFO_LIST sencelist)
{
	if (NULL == sencelist)
	{
		return FALSE;
	}

	g_selectIndex = Index;
	g_SceneList = sencelist;

	if (!SetJdScenceParamLayer)
	{
		SetJdScenceParamLayer = ituSceneFindWidget(&theScene, "SetJdScenceParamLayer");
		assert(SetJdScenceParamLayer);
	}
	ituLayerGoto(SetJdScenceParamLayer);

	return TRUE;
}

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
	uint8 ret = ECHO_STORAGE_OK;

	if (g_selectIndex < 5)
	{
		return TRUE;
	}

	g_sceneinfo.SceneID = g_selectIndex;
	if (1 == g_sceneinfo.IsUsed)
	{
		if (g_text_new[0] == 0 || g_text_new[0] == SID_Jd_None ||
			g_text_new[1] == 0 || g_text_new[1] == SID_Jd_None)
		{
			ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_ERROR, SID_Jd_Set_Scene_Noname, "SetJdScenceParamLayer");
			return FALSE;
		}
	}
	if (g_sceneinfo.IsUsed && g_sceneinfo.Address < 16)
	{
		ret = ECH0_STORAGE_ERR_ADDR;
	}
	else
	{
		ret = storage_add_aurine_scenemode(&g_sceneinfo);
	}
	switch (ret)
	{
	case ECHO_STORAGE_SAME_NAME:
		ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_ERROR, SID_Jd_Set_Name_Err, "SetJdScenceParamLayer");
		return FALSE;

	case ECHO_STORAGE_SAME_ADDR:
		ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_ERROR, SID_Jd_Set_Adr_Err, "SetJdScenceParamLayer");
		return FALSE;

	case ECHO_STORAGE_ERR:
		ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_ERROR, SID_Msg_Save_Err, "SetJdScenceParamLayer");
		return FALSE;

	case ECH0_STORAGE_ERR_ADDR:
		ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_ERROR, SID_Jd_Set_Adr_Max_Err, "SetJdScenceParamLayer");
		return FALSE;

	case ECHO_STORAGE_OK:
		memcpy(&g_SceneList->pjd_scene_info[g_selectIndex], &g_sceneinfo, sizeof(JD_SCENE_INFO));
		return TRUE;
	}
	return FALSE;
}

/*************************************************
Function:		SetJDSceneParamLayerOnReturn
Description: 	底部退出键按下执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
void SetJDSceneParamLayerOnReturn(void)
{
	int ret = FALSE;

	if ((ituWidgetIsVisible(SetJdSceneNameMsgBackground)) || (ituWidgetIsVisible(SetJdSceneAddrMsgBackground)))
	{
		SetJdSceneNameMsgFalseButtonOnMouseUp(NULL, NULL);
		return;
	}
	else if (ituWidgetIsVisible(SetJdScenceParamBackground))
	{
		ret = save_param();
		if (TRUE == ret)
		{
			ituLayerGoto(SetJdScenceLayer);
		}
		return;
	}
}
