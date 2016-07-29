/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_link.c
Author:     	zxc
Version:
Date: 		2016-06-24
Description:
History:
1. Date:
Author:
Modification:
*************************************************/
#include "gui_include.h"

static ITUSprite* SetAFLinkTikouChefangSprite;
static ITUSprite* SetAFLinkDoorChefangSprite;
static ITUSprite* SetAFLinkBafangquDoorRingSprite; 
static ITUSprite* SetLinkEnableLylySprite;
static ITUSprite* SetCallLinkWaichuSprite;
static ITUSprite* SetCallLinkYejianSprite;
static ITUSprite* SetCallLinkNoFaceSprite;
static ITUSprite* SetJDLinkWaichuSprite;
static ITUSprite* SetJDLinkZaijiaSprite;
static ITUText* SetLinkLylyMode1Text;
static ITUText* SetLinkLylyMode2Text;
static ITUText* SetCallLinkWaichu1Text;
static ITUText* SetCallLinkWaichu2Text;
static ITUText* SetCallLinkYejian1Text;
static ITUText* SetCallLinkYejian2Text;
static ITUText* SetCallLinkNoFace1Text;
static ITUText* SetCallLinkNoFace2Text;
static ITUText* SetJDLinkWaichuMode1Text;
static ITUText* SetJDLinkWaichuMode2Text;
static ITUText* SetJDLinkZaijiaMode1Text;
static ITUText* SetJDLinkZaijiaMode2Text;
static ITUText* SetLinkTitleText;
static ITUCalendar* SetLinkJDListContainer;
static ITUCalendar* SetLinkAFListContainer;
static ITUCalendar* SetLinkContainer;
static ITUCalendar* SetLinkCallListContainer;
static ITUBackground* SetLinkLylyModeBackground;
static ITUBackground* SetLinkBackground;
static ITUBackground* SetLinkJDModeMsgBackground;
static ITULayer* SetMenuLayer;
static ITUCalendar* SetLinkLylyModeContainer;
static ITUCalendar* SetCallLinkWaichuContainer;
static ITUCalendar* SetCallLinkYejianContainer;
static ITUCalendar* SetCallLinkNoFaceContainer;
static ITUCalendar* SetJDLinkWaichuModeContainer;
static ITUCalendar* SetJDLinkZaijiaModeContainer;
static ITURadioBox* SetLinkLylyModeMsg0RadioBox;
static ITURadioBox* SetLinkLylyModeMsg1RadioBox;
static ITURadioBox* SetLinkLylyModeMsg2RadioBox;
static ITURadioBox* SetLinkJDModeMsgHKRadioBox;
static ITURadioBox* SetLinkJDModeMsgJCRadioBox;
static ITURadioBox* SetLinkJDModeMsgYJRadioBox;
static ITURadioBox* SetLinkJDModeMsgJDRadioBox;
static ITURadioBox* SetLinkJDModeMsgPURadioBox;

static uint8 g_link;
static uint8 g_scenlink[2];
static LYLY_PARAM *g_lyly_param = NULL;
static int g_JD_mode_select_index = 0;

/*************************************************
Function:		SetLinkOnEnter
Description: 	进入联动设置界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetLinkOnEnter(ITUWidget* widget, char* param)
{
	if (!SetAFLinkTikouChefangSprite)
	{
		SetAFLinkTikouChefangSprite = ituSceneFindWidget(&theScene, "SetAFLinkTikouChefangSprite");
		assert(SetAFLinkTikouChefangSprite);

		SetAFLinkDoorChefangSprite = ituSceneFindWidget(&theScene, "SetAFLinkDoorChefangSprite");
		assert(SetAFLinkDoorChefangSprite);

		SetAFLinkBafangquDoorRingSprite = ituSceneFindWidget(&theScene, "SetAFLinkBafangquDoorRingSprite");
		assert(SetAFLinkBafangquDoorRingSprite);

		SetLinkEnableLylySprite = ituSceneFindWidget(&theScene, "SetLinkEnableLylySprite");
		assert(SetLinkEnableLylySprite);

		SetCallLinkWaichuSprite = ituSceneFindWidget(&theScene, "SetCallLinkWaichuSprite");
		assert(SetCallLinkWaichuSprite);

		SetCallLinkYejianSprite = ituSceneFindWidget(&theScene, "SetCallLinkYejianSprite");
		assert(SetCallLinkYejianSprite);

		SetCallLinkNoFaceSprite = ituSceneFindWidget(&theScene, "SetCallLinkNoFaceSprite");
		assert(SetCallLinkNoFaceSprite);

		SetJDLinkWaichuSprite = ituSceneFindWidget(&theScene, "SetJDLinkWaichuSprite");
		assert(SetJDLinkWaichuSprite);

		SetJDLinkZaijiaSprite = ituSceneFindWidget(&theScene, "SetJDLinkZaijiaSprite");
		assert(SetJDLinkZaijiaSprite);

		SetLinkLylyMode1Text = ituSceneFindWidget(&theScene, "SetLinkLylyMode1Text");
		assert(SetLinkLylyMode1Text);

		SetLinkLylyMode2Text = ituSceneFindWidget(&theScene, "SetLinkLylyMode2Text");
		assert(SetLinkLylyMode2Text);

		SetCallLinkWaichu1Text = ituSceneFindWidget(&theScene, "SetCallLinkWaichu1Text");
		assert(SetCallLinkWaichu1Text); 

		SetCallLinkWaichu2Text = ituSceneFindWidget(&theScene, "SetCallLinkWaichu2Text");
		assert(SetCallLinkWaichu2Text);

		SetCallLinkYejian1Text = ituSceneFindWidget(&theScene, "SetCallLinkYejian1Text");
		assert(SetCallLinkYejian1Text);

		SetCallLinkYejian2Text = ituSceneFindWidget(&theScene, "SetCallLinkYejian2Text");
		assert(SetCallLinkYejian2Text);

		SetCallLinkNoFace1Text = ituSceneFindWidget(&theScene, "SetCallLinkNoFace1Text");
		assert(SetCallLinkNoFace1Text);

		SetCallLinkNoFace2Text = ituSceneFindWidget(&theScene, "SetCallLinkNoFace2Text");
		assert(SetCallLinkNoFace2Text);

		SetJDLinkWaichuMode1Text = ituSceneFindWidget(&theScene, "SetJDLinkWaichuMode1Text");
		assert(SetJDLinkWaichuMode1Text);

		SetJDLinkWaichuMode2Text = ituSceneFindWidget(&theScene, "SetJDLinkWaichuMode2Text");
		assert(SetJDLinkWaichuMode2Text);

		SetJDLinkZaijiaMode1Text = ituSceneFindWidget(&theScene, "SetJDLinkZaijiaMode1Text");
		assert(SetJDLinkZaijiaMode1Text);

		SetJDLinkZaijiaMode2Text = ituSceneFindWidget(&theScene, "SetJDLinkZaijiaMode2Text");
		assert(SetJDLinkZaijiaMode2Text);

		SetLinkTitleText = ituSceneFindWidget(&theScene, "SetLinkTitleText");
		assert(SetLinkTitleText);

		SetLinkJDListContainer = ituSceneFindWidget(&theScene, "SetLinkJDListContainer");
		assert(SetLinkJDListContainer);

		SetLinkAFListContainer = ituSceneFindWidget(&theScene, "SetLinkAFListContainer");
		assert(SetLinkAFListContainer);

		SetLinkContainer = ituSceneFindWidget(&theScene, "SetLinkContainer");
		assert(SetLinkContainer);

		SetLinkCallListContainer = ituSceneFindWidget(&theScene, "SetLinkCallListContainer");
		assert(SetLinkCallListContainer);

		SetLinkLylyModeBackground = ituSceneFindWidget(&theScene, "SetLinkLylyModeBackground");
		assert(SetLinkLylyModeBackground);

		SetLinkBackground = ituSceneFindWidget(&theScene, "SetLinkBackground");
		assert(SetLinkBackground);

		SetLinkJDModeMsgBackground = ituSceneFindWidget(&theScene, "SetLinkJDModeMsgBackground");
		assert(SetLinkJDModeMsgBackground); 

		SetMenuLayer = ituSceneFindWidget(&theScene, "SetMenuLayer");
		assert(SetMenuLayer);

		SetLinkLylyModeContainer = ituSceneFindWidget(&theScene, "SetLinkLylyModeContainer");
		assert(SetLinkLylyModeContainer);

		SetCallLinkWaichuContainer = ituSceneFindWidget(&theScene, "SetCallLinkWaichuContainer");
		assert(SetCallLinkWaichuContainer);

		SetCallLinkYejianContainer = ituSceneFindWidget(&theScene, "SetCallLinkYejianContainer");
		assert(SetCallLinkYejianContainer);

		SetCallLinkNoFaceContainer = ituSceneFindWidget(&theScene, "SetCallLinkNoFaceContainer");
		assert(SetCallLinkNoFaceContainer); 

		SetLinkLylyModeMsg0RadioBox = ituSceneFindWidget(&theScene, "SetLinkLylyModeMsg0RadioBox");
		assert(SetLinkLylyModeMsg0RadioBox);

		SetLinkLylyModeMsg1RadioBox = ituSceneFindWidget(&theScene, "SetLinkLylyModeMsg1RadioBox");
		assert(SetLinkLylyModeMsg1RadioBox);

		SetLinkLylyModeMsg2RadioBox = ituSceneFindWidget(&theScene, "SetLinkLylyModeMsg2RadioBox");
		assert(SetLinkLylyModeMsg2RadioBox); 

		SetJDLinkWaichuModeContainer = ituSceneFindWidget(&theScene, "SetJDLinkWaichuModeContainer");
		assert(SetJDLinkWaichuModeContainer); 

		SetJDLinkZaijiaModeContainer = ituSceneFindWidget(&theScene, "SetJDLinkZaijiaModeContainer");
		assert(SetJDLinkZaijiaModeContainer);

		SetLinkJDModeMsgHKRadioBox = ituSceneFindWidget(&theScene, "SetLinkJDModeMsgHKRadioBox");
		assert(SetLinkJDModeMsgHKRadioBox);

		SetLinkJDModeMsgJCRadioBox = ituSceneFindWidget(&theScene, "SetLinkJDModeMsgJCRadioBox");
		assert(SetLinkJDModeMsgJCRadioBox);

		SetLinkJDModeMsgYJRadioBox = ituSceneFindWidget(&theScene, "SetLinkJDModeMsgYJRadioBox");
		assert(SetLinkJDModeMsgYJRadioBox);

		SetLinkJDModeMsgJDRadioBox = ituSceneFindWidget(&theScene, "SetLinkJDModeMsgJDRadioBox");
		assert(SetLinkJDModeMsgJDRadioBox);

		SetLinkJDModeMsgPURadioBox = ituSceneFindWidget(&theScene, "SetLinkJDModeMsgPURadioBox");
		assert(SetLinkJDModeMsgPURadioBox);
	}

	g_link = storage_get_link_param();
	storage_get_link_scene(g_scenlink);
	g_lyly_param = (PLYLY_PARAM)malloc(sizeof(LYLY_PARAM));
	if (NULL == g_lyly_param)
	{
		return FALSE;
	}
	memcpy(g_lyly_param, (PLYLY_PARAM)storage_get_lyly_param(), sizeof(LYLY_PARAM));

	ituTextSetString(SetLinkTitleText, get_str(SID_Set_Link_LinkSet));
	ituWidgetSetVisible(SetLinkJDModeMsgBackground, false);
	ituWidgetSetVisible(SetLinkLylyModeBackground, false);
	ituWidgetSetVisible(SetLinkBackground, true);
	ituWidgetSetVisible(SetLinkJDListContainer, false);
	ituWidgetSetVisible(SetLinkAFListContainer, false); 
	ituWidgetSetVisible(SetLinkCallListContainer, false);

	return true;
}

/*************************************************
Function:		SetLinkAFButtonOnMouseUp
Description: 	安防联动按钮按下抬起执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetLinkAFButtonOnMouseUp(ITUWidget* widget, char* param)
{
	ITUSprite* AFSprite[3] = { SetAFLinkTikouChefangSprite, SetAFLinkDoorChefangSprite, SetAFLinkBafangquDoorRingSprite };
	uint8 i;

	for (i = 0; i < 3; i++)
	{
		if (1 == ((g_link >> i) & 0x01))
		{
			ituSpriteGoto(AFSprite[i], 1);
		}
		else
		{
			ituSpriteGoto(AFSprite[i], 0);
		}
	}

	ituTextSetString(SetLinkTitleText, get_str(SID_Set_Link_AfLink));
	ituWidgetSetVisible(SetLinkContainer, false);
	ituWidgetSetVisible(SetLinkAFListContainer, true);
	return true;
}

/*************************************************
Function:		SetAFLinkDealSaveButtonOnMouseUp
Description: 	
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:	
*************************************************/
bool SetAFLinkDealSaveButtonOnMouseUp(ITUWidget* widget, char* param)
{
	int index = atoi(param);
	ITUSprite* AFSprite[3] = { SetAFLinkTikouChefangSprite, SetAFLinkDoorChefangSprite, SetAFLinkBafangquDoorRingSprite };

	if (1 == ((g_link >> index) & 0x01))
	{
		SET_ONE_VALUE(g_link, 0, index);
		ituSpriteGoto(AFSprite[index], 0);
	}
	else
	{
		SET_ONE_VALUE(g_link, 1, index);
		ituSpriteGoto(AFSprite[index], 1);
	}
	storage_set_link_param(g_link);

	return true;
}

/*************************************************
Function:		SetLinkCallButtonOnMouseUp
Description: 	对讲联动按钮按下抬起执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetLinkCallButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 i;
	ITUText* enable_text1[4] = { SetLinkLylyMode1Text, SetCallLinkWaichu1Text, SetCallLinkYejian1Text, SetCallLinkNoFace1Text };
	ITUText* enable_text2[4] = { SetLinkLylyMode2Text, SetCallLinkWaichu2Text, SetCallLinkYejian2Text, SetCallLinkNoFace2Text };
	ITUCalendar* enable_Container[4] = { SetLinkLylyModeContainer, SetCallLinkWaichuContainer, SetCallLinkYejianContainer, SetCallLinkNoFaceContainer };
	ITUSprite* CallSprite[3] = { SetCallLinkWaichuSprite, SetCallLinkYejianSprite, SetCallLinkNoFaceSprite };

	if (g_lyly_param->Enable)
	{
		ituSpriteGoto(SetLinkEnableLylySprite, 1);
		for (i = 0; i < 4; i++)
		{
			ituSetColor(&((ITUWidget*)enable_text1[i])->color, 255, 255, 255, 255);
			ituSetColor(&((ITUWidget*)enable_text2[i])->color, 255, 255, 255, 255);
			ituWidgetEnable(enable_Container[i]);
		}

		for (i = 0; i < 3; i++)
		{
			if (g_lyly_param->Link[i])
			{
				ituSpriteGoto(CallSprite[i], 1);
			}
			else
			{
				ituSpriteGoto(CallSprite[i], 0);
			}
		}
	}
	else
	{
		ituSpriteGoto(SetLinkEnableLylySprite, 0);
		for (i = 0; i < 4; i++)
		{
			ituSetColor(&((ITUWidget*)enable_text1[i])->color, 255, 128, 138, 135);//冷灰
			ituSetColor(&((ITUWidget*)enable_text2[i])->color, 255, 192, 192, 192);//灰色
			ituWidgetDisable(enable_Container[i]);
		}

		for (i = 0; i < 3; i++)
		{
			if (g_lyly_param->Link[i])
			{
				ituSpriteGoto(CallSprite[i], 2);
			}
			else
			{
				ituSpriteGoto(CallSprite[i], 0);
			}
		}
	}

	ituTextSetString(SetLinkLylyMode2Text, get_str(SID_Set_Line_Lyly_Mode_Audio + g_lyly_param->Mode));
	
	ituTextSetString(SetLinkTitleText, get_str(SID_Set_Link_CallLink));
	ituWidgetSetVisible(SetLinkContainer, false);
	ituWidgetSetVisible(SetLinkCallListContainer, true);
	return true;
}

/*************************************************
Function:		SetLinkEnableLylyButtonOnMouseUp
Description: 	启用留影留言功能按钮按下抬起执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetLinkEnableLylyButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 i;
	ITUText* enable_text1[4] = { SetLinkLylyMode1Text, SetCallLinkWaichu1Text, SetCallLinkYejian1Text, SetCallLinkNoFace1Text };
	ITUText* enable_text2[4] = { SetLinkLylyMode2Text, SetCallLinkWaichu2Text, SetCallLinkYejian2Text, SetCallLinkNoFace2Text };
	ITUCalendar* enable_Container[4] = { SetLinkLylyModeContainer, SetCallLinkWaichuContainer, SetCallLinkYejianContainer, SetCallLinkNoFaceContainer };
	ITUSprite* CallSprite[3] = { SetCallLinkWaichuSprite, SetCallLinkYejianSprite, SetCallLinkNoFaceSprite };

	if (g_lyly_param->Enable)
	{
		g_lyly_param->Enable = FALSE;

		ituSpriteGoto(SetLinkEnableLylySprite, 0);
		for (i = 0; i < 4; i++)
		{
			ituSetColor(&((ITUWidget*)enable_text1[i])->color, 255, 128, 138, 135);//冷灰
			ituSetColor(&((ITUWidget*)enable_text2[i])->color, 255, 192, 192, 192);//灰色
			ituWidgetDisable(enable_Container[i]);
		}

		for (i = 0; i < 3; i++)
		{
			if (g_lyly_param->Link[i])
			{
				ituSpriteGoto(CallSprite[i], 2);
			}
			else
			{
				ituSpriteGoto(CallSprite[i], 0);
			}
		}
	}
	else
	{
		g_lyly_param->Enable = TRUE;
		ituSpriteGoto(SetLinkEnableLylySprite, 1);
		for (i = 0; i < 4; i++)
		{
			ituSetColor(&((ITUWidget*)enable_text1[i])->color, 255, 255, 255, 255);
			ituSetColor(&((ITUWidget*)enable_text2[i])->color, 255, 255, 255, 255);
			ituWidgetEnable(enable_Container[i]);
		}

		for (i = 0; i < 3; i++)
		{
			if (g_lyly_param->Link[i])
			{
				ituSpriteGoto(CallSprite[i], 1);
			}
			else
			{
				ituSpriteGoto(CallSprite[i], 0);
			}
		}
	}

	uint8 ret = FALSE;
	ret = storage_set_lyly_param(g_lyly_param);
	if (ret)
	{
		//sys_sync_hint_state();
	}

	return true;
}

/*************************************************
Function:		SetCallLinkLylyModeButtonOnMouseUp
Description: 	留影留言模式选择按钮按下抬起执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetCallLinkLylyModeButtonOnMouseUp(ITUWidget* widget, char* param)
{
	switch (g_lyly_param->Mode)
	{
	case LYLY_TYPE_AUDIO:
		ituRadioBoxSetChecked(SetLinkLylyModeMsg0RadioBox, true);
		break;

	case LYLY_TYPE_PIC_AUDIO:
		ituRadioBoxSetChecked(SetLinkLylyModeMsg1RadioBox, true);
		break;

	case LYLY_TYPE_VIDEO:
		ituRadioBoxSetChecked(SetLinkLylyModeMsg2RadioBox, true);
		break;

	default:
		ituRadioBoxSetChecked(SetLinkLylyModeMsg0RadioBox, true);
		break;
	}

	ituWidgetDisable(SetLinkBackground);
	ituWidgetSetVisible(SetLinkLylyModeBackground, true);
	
	return true;
}

/*************************************************
Function:		SetLinkLylyModeMsgRadioBoxOnMouseUp
Description: 	留影留言模式设置
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetLinkLylyModeMsgRadioBoxOnMouseUp(ITUWidget* widget, char* param)
{
	if (ituRadioBoxIsChecked(SetLinkLylyModeMsg0RadioBox))
	{
		g_lyly_param->Mode = LYLY_TYPE_AUDIO;
	}
	else if (ituRadioBoxIsChecked(SetLinkLylyModeMsg1RadioBox))
	{
		g_lyly_param->Mode = LYLY_TYPE_PIC_AUDIO;
	}
	else if (ituRadioBoxIsChecked(SetLinkLylyModeMsg2RadioBox))
	{
		g_lyly_param->Mode = LYLY_TYPE_VIDEO;
	}
	
	uint8 ret = FALSE;
	ret = storage_set_lyly_param(g_lyly_param);
	if (ret)
	{
		//sys_sync_hint_state();
	}

	ituTextSetString(SetLinkLylyMode2Text, get_str(SID_Set_Line_Lyly_Mode_Audio + g_lyly_param->Mode));
	ituWidgetSetVisible(SetLinkLylyModeBackground, false);
	ituWidgetEnable(SetLinkBackground);

	return true;
}

/*************************************************
Function:		SetLinkLylyModeMsgExitButtonOnMouseUp
Description: 	留影留言模式设置取消键
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetLinkLylyModeMsgExitButtonOnMouseUp(ITUWidget* widget, char* param)
{
	ituWidgetSetVisible(SetLinkLylyModeBackground, false);
	ituWidgetEnable(SetLinkBackground);
	
	return true;
}

/*************************************************
Function:		SetCallLinkDealSaveButtonOnMouseUp
Description: 	留影留言联动选择按钮按下抬起执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetCallLinkDealSaveButtonOnMouseUp(ITUWidget* widget, char* param)
{
	int index = atoi(param);
	ITUSprite* CallSprite[3] = { SetCallLinkWaichuSprite, SetCallLinkYejianSprite, SetCallLinkNoFaceSprite };

	if (g_lyly_param->Link[index])
	{
		g_lyly_param->Link[index] = FALSE;
		ituSpriteGoto(CallSprite[index], 0);
	}
	else
	{
		g_lyly_param->Link[index] = TRUE;
		ituSpriteGoto(CallSprite[index], 1);
	}

	uint8 ret = FALSE;
	ret = storage_set_lyly_param(g_lyly_param);
	if (ret)
	{
		//sys_sync_hint_state();
	}

	return true;
}

/*************************************************
Function:		SetLinkJDButtonOnMouseUp
Description: 	家电联动按钮按下抬起执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetLinkJDButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 i;
	ITUSprite* JDSprite[2] = { SetJDLinkWaichuSprite, SetJDLinkZaijiaSprite };
	ITUCalendar* JDCalendar[2] = { SetJDLinkWaichuModeContainer, SetJDLinkZaijiaModeContainer };
	ITUText* enable_text1[2] = { SetJDLinkWaichuMode1Text, SetJDLinkZaijiaMode1Text };
	ITUText* enable_text2[2] = { SetJDLinkWaichuMode2Text, SetJDLinkZaijiaMode2Text };


	for (i = 3; i < 5; i++)
	{
		if (1 == ((g_link >> i) & 0x01))
		{
			ituSpriteGoto(JDSprite[i-3], 1);
			ituWidgetEnable(JDCalendar[i-3]);
			ituSetColor(&((ITUWidget*)enable_text1[i-3])->color, 255, 255, 255, 255);
			ituSetColor(&((ITUWidget*)enable_text2[i-3])->color, 255, 255, 255, 255);
		}
		else
		{
			ituSpriteGoto(JDSprite[i-3], 0);
			ituWidgetDisable(JDCalendar[i-3]);
			ituSetColor(&((ITUWidget*)enable_text1[i-3])->color, 255, 128, 138, 135);//冷灰
			ituSetColor(&((ITUWidget*)enable_text2[i-3])->color, 255, 192, 192, 192);//灰色
		}
	}
	
	ituTextSetString(SetJDLinkWaichuMode2Text, get_str(SID_Set_Line_Jd_Mode_Huike + g_scenlink[0]));
	ituTextSetString(SetJDLinkZaijiaMode2Text, get_str(SID_Set_Line_Jd_Mode_Huike + g_scenlink[1]));

	ituTextSetString(SetLinkTitleText, get_str(SID_Set_Link_JDLink));
	ituWidgetSetVisible(SetLinkContainer, false);
	ituWidgetSetVisible(SetLinkJDListContainer, true);
	return true;
}

/*************************************************
Function:		SetJDLinkDealSaveButtonOnMouseUp
Description: 	家电联动情景模式启用按键
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJDLinkDealSaveButtonOnMouseUp(ITUWidget* widget, char* param)
{
	int index = atoi(param);
	ITUSprite* JDSprite[2] = { SetJDLinkWaichuSprite, SetJDLinkZaijiaSprite };
	ITUCalendar* JDCalendar[2] = { SetJDLinkWaichuModeContainer, SetJDLinkZaijiaModeContainer };
	ITUText* enable_text1[2] = { SetJDLinkWaichuMode1Text, SetJDLinkZaijiaMode1Text };
	ITUText* enable_text2[2] = { SetJDLinkWaichuMode2Text, SetJDLinkZaijiaMode2Text };

	if (1 == ((g_link >> index) & 0x01))
	{
		SET_ONE_VALUE(g_link, 0, index);
		ituSpriteGoto(JDSprite[index-3], 0);
		ituWidgetDisable(JDCalendar[index - 3]);
		ituSetColor(&((ITUWidget*)enable_text1[index - 3])->color, 255, 128, 138, 135);//冷灰
		ituSetColor(&((ITUWidget*)enable_text2[index - 3])->color, 255, 192, 192, 192);//灰色
	}
	else
	{
		SET_ONE_VALUE(g_link, 1, index);
		ituSpriteGoto(JDSprite[index-3], 1);
		ituWidgetEnable(JDCalendar[index - 3]);
		ituSetColor(&((ITUWidget*)enable_text1[index - 3])->color, 255, 255, 255, 255);
		ituSetColor(&((ITUWidget*)enable_text2[index - 3])->color, 255, 255, 255, 255);
	}

	storage_set_link_param(g_link);

	return true;
}

/*************************************************
Function:		SetJDLinkModeSelectButtonOnMouseUp
Description: 	家电联动模式选择按钮按下抬起执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJDLinkModeSelectButtonOnMouseUp(ITUWidget* widget, char* param)
{
	g_JD_mode_select_index = atoi(param);

	switch (g_scenlink[g_JD_mode_select_index])
	{
	case JD_SCENE_RECEPTION:
		ituRadioBoxSetChecked(SetLinkJDModeMsgHKRadioBox, true);
		break;

	case JD_SCENE_MEALS:
		ituRadioBoxSetChecked(SetLinkJDModeMsgJCRadioBox, true);
		break;

	case JD_SCENE_NIGHT:
		ituRadioBoxSetChecked(SetLinkJDModeMsgYJRadioBox, true);
		break;


	case JD_SCENE_POWERSAVING:
		ituRadioBoxSetChecked(SetLinkJDModeMsgJDRadioBox, true);
		break;

	case JD_SCENE_GENERAL:
		ituRadioBoxSetChecked(SetLinkJDModeMsgPURadioBox, true);
		break;

	default:
		break;
	}

	ituWidgetDisable(SetLinkBackground);
	ituWidgetSetVisible(SetLinkJDModeMsgBackground, true);
	
	return true;
}

/*************************************************
Function:		SetLinkJDModeMsgRadioBoxOnMouseUp
Description: 	家电联动模式选择消息框按钮按下抬起执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetLinkJDModeMsgRadioBoxOnMouseUp(ITUWidget* widget, char* param)
{
	if (ituRadioBoxIsChecked(SetLinkJDModeMsgHKRadioBox))
	{
		g_scenlink[g_JD_mode_select_index] = JD_SCENE_RECEPTION;
	}
	else if (ituRadioBoxIsChecked(SetLinkJDModeMsgJCRadioBox))
	{
		g_scenlink[g_JD_mode_select_index] = JD_SCENE_MEALS;
	}
	else if (ituRadioBoxIsChecked(SetLinkJDModeMsgYJRadioBox))
	{
		g_scenlink[g_JD_mode_select_index] = JD_SCENE_NIGHT;
	}
	else if (ituRadioBoxIsChecked(SetLinkJDModeMsgJDRadioBox))
	{
		g_scenlink[g_JD_mode_select_index] = JD_SCENE_POWERSAVING;
	}
	else if (ituRadioBoxIsChecked(SetLinkJDModeMsgPURadioBox))
	{
		g_scenlink[g_JD_mode_select_index] = JD_SCENE_GENERAL;
	}

	storage_set_link_scene(g_scenlink); // 保存情景模式

	if (0 == g_JD_mode_select_index)
	{
		ituTextSetString(SetJDLinkWaichuMode2Text, get_str(SID_Set_Line_Jd_Mode_Huike + g_scenlink[0]));
	}
	else if (1 == g_JD_mode_select_index)
	{
		ituTextSetString(SetJDLinkZaijiaMode2Text, get_str(SID_Set_Line_Jd_Mode_Huike + g_scenlink[1]));
	}
	else
	{
		return false;
	}
	
	ituWidgetSetVisible(SetLinkJDModeMsgBackground, false);
	ituWidgetEnable(SetLinkBackground);

	return true;
}

/*************************************************
Function:		SetLinkJDModeMsgExitButtonOnMouseUp
Description: 	家电联动模式选择消息框取消按钮按下抬起执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetLinkJDModeMsgExitButtonOnMouseUp(ITUWidget* widget, char* param)
{
	ituWidgetSetVisible(SetLinkJDModeMsgBackground, false);
	ituWidgetEnable(SetLinkBackground);
	return true;
}

/*************************************************
Function:		SetNoDisturLayerOnReturn
Description: 	底部退出键按下执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
void SetLinkLayerOnReturn(void)
{
	if (ituWidgetIsVisible(SetLinkJDModeMsgBackground))
	{
		ituWidgetSetVisible(SetLinkJDModeMsgBackground, false);
		//ituWidgetSetVisible(SetLinkBackground, true);
		ituWidgetEnable(SetLinkBackground);
		return;
	}
	else if (ituWidgetIsVisible(SetLinkLylyModeBackground))
	{
		ituWidgetSetVisible(SetLinkLylyModeBackground, false);
		//ituWidgetSetVisible(SetLinkBackground, true);
		ituWidgetEnable(SetLinkBackground);
		return;
	}
	else if (ituWidgetIsVisible(SetLinkAFListContainer))
	{
		ituTextSetString(SetLinkTitleText, get_str(SID_Set_Link_LinkSet));
		ituWidgetSetVisible(SetLinkAFListContainer, false);
		ituWidgetSetVisible(SetLinkContainer, true);
		return;
	}
	else if (ituWidgetIsVisible(SetLinkCallListContainer))
	{
		ituTextSetString(SetLinkTitleText, get_str(SID_Set_Link_LinkSet));
		ituWidgetSetVisible(SetLinkCallListContainer, false);
		ituWidgetSetVisible(SetLinkContainer, true);
		return;
	}
	else if (ituWidgetIsVisible(SetLinkJDListContainer))
	{
		ituTextSetString(SetLinkTitleText, get_str(SID_Set_Link_LinkSet));
		ituWidgetSetVisible(SetLinkJDListContainer, false);
		ituWidgetSetVisible(SetLinkContainer, true);
		return;
	}
	else if (!ituWidgetIsVisible(SetMenuLayer))
	{
		ituLayerGoto(SetMenuLayer);
		return;
	}
}

void SetLinkReset(void)
{
	SetAFLinkTikouChefangSprite = NULL;
}
