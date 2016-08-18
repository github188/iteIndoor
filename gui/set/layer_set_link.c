/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_link.c
Author:     	zxc
Version:
Date: 		2016-06-24
Description: 联动设置
History:
1. Date:
Author:
Modification:
*************************************************/
#include "layer_set.h"

static ITUCalendar* SetLinkContainer = NULL;
static ITUCalendar* SetLinkAFListContainer = NULL;
static ITUCalendar* SetLinkCallListContainer = NULL;
static ITUCalendar* SetLinkJDListContainer = NULL;
static ITUText* SetLinkTitleText = NULL;
static ITUBackground* SetLinkLylyModeBackground = NULL;
static ITUBackground* SetLinkBackground = NULL;
static ITUBackground* SetLinkJDModeMsgBackground = NULL;
static ITULayer* SetMenuLayer = NULL;
static ITUSprite* SetAFLinkSprite[3] = { NULL };
static ITUText* SetLinkCallContainer1Text[4] = { NULL };
static ITUText* SetLinkCallContainer2Text[4] = { NULL };
static ITUCalendar* SetLinkCallContainerEnable[4] = { NULL };
static ITUSprite* SetLinkCallSprite[3] = { NULL };
static ITUSprite* SetLinkEnableLylySprite = NULL;
static ITURadioBox* SetLinkLylyModeMsgRadioBox[3] = { NULL };
static ITUSprite* SetJDLinkSprite[2] = { NULL };
static ITUCalendar* SetJDLinkContainer[2] = { NULL };
static ITUText* SetJDLink1Text[2] = { NULL };
static ITUText* SetJDLink2Text[2] = { NULL };
static ITURadioBox* SetLinkJDModeMsgRadioBox[5] = { NULL };

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
	if (!SetLinkContainer)
	{
		SetLinkContainer = ituSceneFindWidget(&theScene, "SetLinkContainer");
		assert(SetLinkContainer);

		SetLinkAFListContainer = ituSceneFindWidget(&theScene, "SetLinkAFListContainer");
		assert(SetLinkAFListContainer);

		SetLinkCallListContainer = ituSceneFindWidget(&theScene, "SetLinkCallListContainer");
		assert(SetLinkCallListContainer);

		SetLinkJDListContainer = ituSceneFindWidget(&theScene, "SetLinkJDListContainer");
		assert(SetLinkJDListContainer);

		SetLinkTitleText = ituSceneFindWidget(&theScene, "SetLinkTitleText");
		assert(SetLinkTitleText);

		SetLinkLylyModeBackground = ituSceneFindWidget(&theScene, "SetLinkLylyModeBackground");
		assert(SetLinkLylyModeBackground);

		SetLinkBackground = ituSceneFindWidget(&theScene, "SetLinkBackground");
		assert(SetLinkBackground);

		SetLinkJDModeMsgBackground = ituSceneFindWidget(&theScene, "SetLinkJDModeMsgBackground");
		assert(SetLinkJDModeMsgBackground);

		SetMenuLayer = ituSceneFindWidget(&theScene, "SetMenuLayer");
		assert(SetMenuLayer);
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
	if (!ituWidgetIsEnabled(SetLinkBackground))
	{
		ituWidgetEnable(SetLinkBackground);
	}

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
	uint8 i = 0;

	if (!SetAFLinkSprite[0])
	{
		SetAFLinkSprite[0] = ituSceneFindWidget(&theScene, "SetAFLinkTikouChefangSprite");
		assert(SetAFLinkSprite[0]);

		SetAFLinkSprite[1] = ituSceneFindWidget(&theScene, "SetAFLinkDoorChefangSprite");
		assert(SetAFLinkSprite[1]);

		SetAFLinkSprite[2] = ituSceneFindWidget(&theScene, "SetAFLinkBafangquDoorRingSprite");
		assert(SetAFLinkSprite[2]);
	}

	for (i = 0; i < 3; i++)
	{
		if (1 == ((g_link >> i) & 0x01))
		{
			ituSpriteGoto(SetAFLinkSprite[i], 1);
		}
		else
		{
			ituSpriteGoto(SetAFLinkSprite[i], 0);
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

	if (1 == ((g_link >> index) & 0x01))
	{
		SET_ONE_VALUE(g_link, 0, index);
		ituSpriteGoto(SetAFLinkSprite[index], 0);
	}
	else
	{
		SET_ONE_VALUE(g_link, 1, index);
		ituSpriteGoto(SetAFLinkSprite[index], 1);
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
	uint8 i = 0;

	if (!SetLinkCallContainer1Text[0])
	{
		SetLinkCallContainer1Text[0] = ituSceneFindWidget(&theScene, "SetLinkLylyMode1Text");
		assert(SetLinkCallContainer1Text[0]);

		SetLinkCallContainer1Text[1] = ituSceneFindWidget(&theScene, "SetCallLinkWaichu1Text");
		assert(SetLinkCallContainer1Text[1]);

		SetLinkCallContainer1Text[2] = ituSceneFindWidget(&theScene, "SetCallLinkYejian1Text");
		assert(SetLinkCallContainer1Text[2]);

		SetLinkCallContainer1Text[3] = ituSceneFindWidget(&theScene, "SetCallLinkNoFace1Text");
		assert(SetLinkCallContainer1Text[3]);

		SetLinkCallContainer2Text[0] = ituSceneFindWidget(&theScene, "SetLinkLylyMode2Text");
		assert(SetLinkCallContainer2Text[0]);

		SetLinkCallContainer2Text[1] = ituSceneFindWidget(&theScene, "SetCallLinkWaichu2Text");
		assert(SetLinkCallContainer2Text[1]);

		SetLinkCallContainer2Text[2] = ituSceneFindWidget(&theScene, "SetCallLinkYejian2Text");
		assert(SetLinkCallContainer2Text[2]);

		SetLinkCallContainer2Text[3] = ituSceneFindWidget(&theScene, "SetCallLinkNoFace2Text");
		assert(SetLinkCallContainer2Text[3]);

		SetLinkCallContainerEnable[0] = ituSceneFindWidget(&theScene, "SetLinkLylyModeContainer");
		assert(SetLinkCallContainerEnable[0]);

		SetLinkCallContainerEnable[1] = ituSceneFindWidget(&theScene, "SetCallLinkWaichuContainer");
		assert(SetLinkCallContainerEnable[1]);

		SetLinkCallContainerEnable[2] = ituSceneFindWidget(&theScene, "SetCallLinkYejianContainer");
		assert(SetLinkCallContainerEnable[2]);

		SetLinkCallContainerEnable[3] = ituSceneFindWidget(&theScene, "SetCallLinkNoFaceContainer");
		assert(SetLinkCallContainerEnable[3]);

		SetLinkCallSprite[0] = ituSceneFindWidget(&theScene, "SetCallLinkWaichuSprite");
		assert(SetLinkCallSprite[0]);

		SetLinkCallSprite[1] = ituSceneFindWidget(&theScene, "SetCallLinkYejianSprite");
		assert(SetLinkCallSprite[1]);

		SetLinkCallSprite[2] = ituSceneFindWidget(&theScene, "SetCallLinkNoFaceSprite");
		assert(SetLinkCallSprite[2]);

		SetLinkEnableLylySprite = ituSceneFindWidget(&theScene, "SetLinkEnableLylySprite");
		assert(SetLinkEnableLylySprite);
	}

	if (g_lyly_param->Enable)
	{
		ituSpriteGoto(SetLinkEnableLylySprite, 1);
		for (i = 0; i < 4; i++)
		{
			ituSetColor(&((ITUWidget*)SetLinkCallContainer1Text[i])->color, 255, 255, 255, 255);
			ituSetColor(&((ITUWidget*)SetLinkCallContainer2Text[i])->color, 255, 255, 255, 255);
			ituWidgetEnable(SetLinkCallContainerEnable[i]);
		}

		for (i = 0; i < 3; i++)
		{
			if (g_lyly_param->Link[i])
			{
				ituSpriteGoto(SetLinkCallSprite[i], 1);
			}
			else
			{
				ituSpriteGoto(SetLinkCallSprite[i], 0);
			}
		}
	}
	else
	{
		ituSpriteGoto(SetLinkEnableLylySprite, 0);
		for (i = 0; i < 4; i++)
		{
			ituSetColor(&((ITUWidget*)SetLinkCallContainer1Text[i])->color, 255, 128, 138, 135);//冷灰
			ituSetColor(&((ITUWidget*)SetLinkCallContainer2Text[i])->color, 255, 192, 192, 192);//灰色
			ituWidgetDisable(SetLinkCallContainerEnable[i]);
		}

		for (i = 0; i < 3; i++)
		{
			if (g_lyly_param->Link[i])
			{
				ituSpriteGoto(SetLinkCallSprite[i], 2);
			}
			else
			{
				ituSpriteGoto(SetLinkCallSprite[i], 0);
			}
		}
	}

	ituTextSetString(SetLinkCallContainer2Text[0], get_str(SID_Set_Line_Lyly_Mode_Audio + g_lyly_param->Mode));
	
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
	uint8 i = 0;

	if (g_lyly_param->Enable)
	{
		g_lyly_param->Enable = FALSE;

		ituSpriteGoto(SetLinkEnableLylySprite, 0);
		for (i = 0; i < 4; i++)
		{
			ituSetColor(&((ITUWidget*)SetLinkCallContainer1Text[i])->color, 255, 128, 138, 135);//冷灰
			ituSetColor(&((ITUWidget*)SetLinkCallContainer2Text[i])->color, 255, 192, 192, 192);//灰色
			ituWidgetDisable(SetLinkCallContainerEnable[i]);
		}

		for (i = 0; i < 3; i++)
		{
			if (g_lyly_param->Link[i])
			{
				ituSpriteGoto(SetLinkCallSprite[i], 2);
			}
			else
			{
				ituSpriteGoto(SetLinkCallSprite[i], 0);
			}
		}
	}
	else
	{
		g_lyly_param->Enable = TRUE;
		ituSpriteGoto(SetLinkEnableLylySprite, 1);
		for (i = 0; i < 4; i++)
		{
			ituSetColor(&((ITUWidget*)SetLinkCallContainer1Text[i])->color, 255, 255, 255, 255);
			ituSetColor(&((ITUWidget*)SetLinkCallContainer2Text[i])->color, 255, 255, 255, 255);
			ituWidgetEnable(SetLinkCallContainerEnable[i]);
		}

		for (i = 0; i < 3; i++)
		{
			if (g_lyly_param->Link[i])
			{
				ituSpriteGoto(SetLinkCallSprite[i], 1);
			}
			else
			{
				ituSpriteGoto(SetLinkCallSprite[i], 0);
			}
		}
	}

	uint8 ret = FALSE;
	ret = storage_set_lyly_param(g_lyly_param);
	if (ret)
	{
		sys_sync_hint_state();
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
	if (!SetLinkLylyModeMsgRadioBox[0])
	{
		uint8 i = 0;
		char textname[50];
		for (i = 0; i < 3; i++)
		{
			memset(textname, 0, sizeof(textname));
			sprintf(textname, "%s%d%s", "SetLinkLylyModeMsg", i, "RadioBox");
			SetLinkLylyModeMsgRadioBox[i] = ituSceneFindWidget(&theScene, textname);
			assert(SetLinkLylyModeMsgRadioBox[i]);
		}
	}

	if(g_lyly_param->Mode > LYLY_TYPE_MAX)
	{
		g_lyly_param->Mode = LYLY_TYPE_VIDEO;
	}
	ituRadioBoxSetChecked(SetLinkLylyModeMsgRadioBox[g_lyly_param->Mode], true);

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
	if (ituRadioBoxIsChecked(SetLinkLylyModeMsgRadioBox[0]))
	{
		g_lyly_param->Mode = LYLY_TYPE_AUDIO;
	}
	else if (ituRadioBoxIsChecked(SetLinkLylyModeMsgRadioBox[1]))
	{
		g_lyly_param->Mode = LYLY_TYPE_PIC_AUDIO;
	}
	else if (ituRadioBoxIsChecked(SetLinkLylyModeMsgRadioBox[2]))
	{
		g_lyly_param->Mode = LYLY_TYPE_VIDEO;
	}
	
	uint8 ret = FALSE;
	ret = storage_set_lyly_param(g_lyly_param);
	if (ret)
	{
		sys_sync_hint_state();
	}

	ituTextSetString(SetLinkCallContainer2Text[0], get_str(SID_Set_Line_Lyly_Mode_Audio + g_lyly_param->Mode));
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

	if (g_lyly_param->Link[index])
	{
		g_lyly_param->Link[index] = FALSE;
		ituSpriteGoto(SetLinkCallSprite[index], 0);
	}
	else
	{
		g_lyly_param->Link[index] = TRUE;
		ituSpriteGoto(SetLinkCallSprite[index], 1);
	}

	uint8 ret = FALSE;
	ret = storage_set_lyly_param(g_lyly_param);
	if (ret)
	{
		sys_sync_hint_state();
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
	uint8 i = 0;

	if (!SetJDLinkSprite[0])
	{
		SetJDLinkSprite[0] = ituSceneFindWidget(&theScene, "SetJDLinkWaichuSprite");
		assert(SetJDLinkSprite[0]);

		SetJDLinkSprite[1] = ituSceneFindWidget(&theScene, "SetJDLinkZaijiaSprite");
		assert(SetJDLinkSprite[1]);
		
		SetJDLinkContainer[0] = ituSceneFindWidget(&theScene, "SetJDLinkWaichuModeContainer");
		assert(SetJDLinkContainer[0]);

		SetJDLinkContainer[1] = ituSceneFindWidget(&theScene, "SetJDLinkZaijiaModeContainer");
		assert(SetJDLinkContainer[1]);

		SetJDLink1Text[0] = ituSceneFindWidget(&theScene, "SetJDLinkWaichuMode1Text");
		assert(SetJDLink1Text[0]);

		SetJDLink1Text[1] = ituSceneFindWidget(&theScene, "SetJDLinkZaijiaMode1Text");
		assert(SetJDLink1Text[1]);

		SetJDLink2Text[0] = ituSceneFindWidget(&theScene, "SetJDLinkWaichuMode2Text");
		assert(SetJDLink2Text[0]);

		SetJDLink2Text[1] = ituSceneFindWidget(&theScene, "SetJDLinkZaijiaMode2Text");
		assert(SetJDLink2Text[1]);

	}

	for (i = 3; i < 5; i++)
	{
		if (1 == ((g_link >> i) & 0x01))
		{
			ituSpriteGoto(SetJDLinkSprite[i - 3], 1);
			ituWidgetEnable(SetJDLinkContainer[i - 3]);
			ituSetColor(&((ITUWidget*)SetJDLink1Text[i - 3])->color, 255, 255, 255, 255);
			ituSetColor(&((ITUWidget*)SetJDLink2Text[i - 3])->color, 255, 255, 255, 255);
		}
		else
		{
			ituSpriteGoto(SetJDLinkSprite[i - 3], 0);
			ituWidgetDisable(SetJDLinkContainer[i - 3]);
			ituSetColor(&((ITUWidget*)SetJDLink1Text[i - 3])->color, 255, 128, 138, 135);//冷灰
			ituSetColor(&((ITUWidget*)SetJDLink2Text[i - 3])->color, 255, 192, 192, 192);//灰色
		}
	}
	
	ituTextSetString(SetJDLink2Text[0], get_str(SID_Set_Line_Jd_Mode_Huike + g_scenlink[0]));
	ituTextSetString(SetJDLink2Text[1], get_str(SID_Set_Line_Jd_Mode_Huike + g_scenlink[1]));

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

	if (1 == ((g_link >> index) & 0x01))
	{
		SET_ONE_VALUE(g_link, 0, index);
		ituSpriteGoto(SetJDLinkSprite[index - 3], 0);
		ituWidgetDisable(SetJDLinkContainer[index - 3]);
		ituSetColor(&((ITUWidget*)SetJDLink1Text[index - 3])->color, 255, 128, 138, 135);//冷灰
		ituSetColor(&((ITUWidget*)SetJDLink2Text[index - 3])->color, 255, 192, 192, 192);//灰色
	}
	else
	{
		SET_ONE_VALUE(g_link, 1, index);
		ituSpriteGoto(SetJDLinkSprite[index - 3], 1);
		ituWidgetEnable(SetJDLinkContainer[index - 3]);
		ituSetColor(&((ITUWidget*)SetJDLink1Text[index - 3])->color, 255, 255, 255, 255);
		ituSetColor(&((ITUWidget*)SetJDLink2Text[index - 3])->color, 255, 255, 255, 255);
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
	if (!SetLinkJDModeMsgRadioBox[0])
	{
		SetLinkJDModeMsgRadioBox[0] = ituSceneFindWidget(&theScene, "SetLinkJDModeMsgHKRadioBox");
		assert(SetLinkJDModeMsgRadioBox[0]);

		SetLinkJDModeMsgRadioBox[1] = ituSceneFindWidget(&theScene, "SetLinkJDModeMsgJCRadioBox");
		assert(SetLinkJDModeMsgRadioBox[1]);

		SetLinkJDModeMsgRadioBox[2] = ituSceneFindWidget(&theScene, "SetLinkJDModeMsgYJRadioBox");
		assert(SetLinkJDModeMsgRadioBox[2]);

		SetLinkJDModeMsgRadioBox[3] = ituSceneFindWidget(&theScene, "SetLinkJDModeMsgJDRadioBox");
		assert(SetLinkJDModeMsgRadioBox[3]);

		SetLinkJDModeMsgRadioBox[4] = ituSceneFindWidget(&theScene, "SetLinkJDModeMsgPURadioBox");
		assert(SetLinkJDModeMsgRadioBox[4]);
	}
	g_JD_mode_select_index = atoi(param);

	ituRadioBoxSetChecked(SetLinkJDModeMsgRadioBox[g_scenlink[g_JD_mode_select_index]], true);

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
	if (ituRadioBoxIsChecked(SetLinkJDModeMsgRadioBox[0]))
	{
		g_scenlink[g_JD_mode_select_index] = JD_SCENE_RECEPTION;
	}
	else if (ituRadioBoxIsChecked(SetLinkJDModeMsgRadioBox[1]))
	{
		g_scenlink[g_JD_mode_select_index] = JD_SCENE_MEALS;
	}
	else if (ituRadioBoxIsChecked(SetLinkJDModeMsgRadioBox[2]))
	{
		g_scenlink[g_JD_mode_select_index] = JD_SCENE_NIGHT;
	}
	else if (ituRadioBoxIsChecked(SetLinkJDModeMsgRadioBox[3]))
	{
		g_scenlink[g_JD_mode_select_index] = JD_SCENE_POWERSAVING;
	}
	else if (ituRadioBoxIsChecked(SetLinkJDModeMsgRadioBox[4]))
	{
		g_scenlink[g_JD_mode_select_index] = JD_SCENE_GENERAL;
	}

	storage_set_link_scene(g_scenlink); // 保存情景模式

	if (0 == g_JD_mode_select_index)
	{
		ituTextSetString(SetJDLink2Text[0], get_str(SID_Set_Line_Jd_Mode_Huike + g_scenlink[0]));
	}
	else if (1 == g_JD_mode_select_index)
	{
		ituTextSetString(SetJDLink2Text[1], get_str(SID_Set_Line_Jd_Mode_Huike + g_scenlink[1]));
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
		ituWidgetEnable(SetLinkBackground);
		return;
	}
	else if (ituWidgetIsVisible(SetLinkLylyModeBackground))
	{
		ituWidgetSetVisible(SetLinkLylyModeBackground, false);
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

