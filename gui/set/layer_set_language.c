/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_language.c
Author:     	zxc
Version:
Date: 		2016-06-26
Description:	语言设置界面
History:
1. Date:
Author:
Modification:
*************************************************/
#include "layer_set.h"

static ITUSprite* SetLanguageSimplifiedSprite = NULL; 
static ITUSprite* SetLanguageTraditionalSprite = NULL;
static ITUSprite* SetLanguageEnglishSprite = NULL;
static ITURadioBox* MsgFailHintSuccess1RadioBox = NULL;
static ITULayer* SetMenuLayer = NULL;

static LANGUAGE_E g_language = CHINESE;

/*************************************************
Function:		SetSystemLanguageOnEnter
Description: 	进入语言设置界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetSystemLanguageOnEnter(ITUWidget* widget, char* param)
{
	if (!SetLanguageSimplifiedSprite)
	{
		SetLanguageSimplifiedSprite = ituSceneFindWidget(&theScene, "SetLanguageSimplifiedSprite");
		assert(SetLanguageSimplifiedSprite);

		SetLanguageTraditionalSprite = ituSceneFindWidget(&theScene, "SetLanguageTraditionalSprite");
		assert(SetLanguageTraditionalSprite);

		SetLanguageEnglishSprite = ituSceneFindWidget(&theScene, "SetLanguageEnglishSprite");
		assert(SetLanguageEnglishSprite);

		MsgFailHintSuccess1RadioBox = ituSceneFindWidget(&theScene, "MsgFailHintSuccess1RadioBox");
		assert(MsgFailHintSuccess1RadioBox);

		SetMenuLayer = ituSceneFindWidget(&theScene, "SetMenuLayer");
		assert(SetMenuLayer);
	}

	if (strcmp(param, "MsgFailHintSuccessLayer") == 0)
	{
		if (ituRadioBoxIsChecked(MsgFailHintSuccess1RadioBox))	//确认键
		{
			storage_set_language(g_language);
			hw_stop_feet_dog();
		}
		else
		{
			ituLayerGoto(SetMenuLayer);
		}
	}
	else
	{
		g_language = storage_get_language();//存储获取系统语言

		if (CHINESE == g_language)
		{
			ituSpriteGoto(SetLanguageSimplifiedSprite, 1);
			ituSpriteGoto(SetLanguageTraditionalSprite, 0);
			ituSpriteGoto(SetLanguageEnglishSprite, 0);
		}
		else if (CHNBIG5 == g_language)
		{
			ituSpriteGoto(SetLanguageSimplifiedSprite, 0);
			ituSpriteGoto(SetLanguageTraditionalSprite, 1);
			ituSpriteGoto(SetLanguageEnglishSprite, 0);
		}
		else if (ENGLISH == g_language)
		{
			ituSpriteGoto(SetLanguageSimplifiedSprite, 0);
			ituSpriteGoto(SetLanguageTraditionalSprite, 0);
			ituSpriteGoto(SetLanguageEnglishSprite, 1);
		}
	}
	
	return true;
}

/*************************************************
Function:		SetLanguageSimplifiedButtonOnMouseUp
Description: 	简体中文钮按下抬起执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetLanguageSimplifiedButtonOnMouseUp(ITUWidget* widget, char* param)
{
	g_language = CHINESE;
	ituSpriteGoto(SetLanguageSimplifiedSprite, 1);
	ituSpriteGoto(SetLanguageTraditionalSprite, 0);
	ituSpriteGoto(SetLanguageEnglishSprite, 0);

	return true;
}

/*************************************************
Function:		SetLanguageTraditionalButtonOnMouseUp
Description: 	繁体中文钮按下抬起执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetLanguageTraditionalButtonOnMouseUp(ITUWidget* widget, char* param)
{

	g_language = CHNBIG5;
	ituSpriteGoto(SetLanguageSimplifiedSprite, 0);
	ituSpriteGoto(SetLanguageTraditionalSprite, 1);
	ituSpriteGoto(SetLanguageEnglishSprite, 0);

	return true;
}

/*************************************************
Function:		SetLanguageEnglishButtonOnMouseUp
Description: 	英文钮按下抬起执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetLanguageEnglishButtonOnMouseUp(ITUWidget* widget, char* param)
{
	g_language = ENGLISH;
	ituSpriteGoto(SetLanguageSimplifiedSprite, 0);
	ituSpriteGoto(SetLanguageTraditionalSprite, 0);
	ituSpriteGoto(SetLanguageEnglishSprite, 1);
#if 0
	if (g_language != storage_get_language())
	{
		storage_set_language(g_language);
	}
#endif
	return true;
}

/*************************************************
Function:		SetSystemLanguageLayerOnReturn
Description: 	底部退出键按下执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
void SetSystemLanguageLayerOnReturn(void)
{
	if (g_language != storage_get_language())
	{
		ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_WARNNING, SID_Msg_Recover_OK, "SetSystemLanguageLayer");
	}
	else
	{
		ituLayerGoto(SetMenuLayer);
	}
}
