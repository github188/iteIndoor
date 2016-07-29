/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_language.c
Author:     	zxc
Version:
Date: 		2016-06-26
Description:
History:
1. Date:
Author:
Modification:
*************************************************/
#include "gui_include.h"

//static ITULayer* SetMenuLayer;
static ITUSprite* SetLanguageSimplifiedSprite; 
static ITUSprite* SetLanguageTraditionalSprite;
static ITUSprite* SetLanguageEnglishSprite;

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

		//SetMenuLayer = ituSceneFindWidget(&theScene, "SetMenuLayer");
		//assert(SetMenuLayer);	
	}

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

	if (g_language != storage_get_language())
	{
		storage_set_language(g_language);
	}

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

	if (g_language != storage_get_language())
	{
		storage_set_language(g_language);
	}

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

	if (g_language != storage_get_language())
	{
		storage_set_language(g_language);
	}
	
	return true;
}

#if 0
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
	if (!ituWidgetIsVisible(SetMenuLayer))
	{
		ituLayerGoto(SetMenuLayer);
	}
}
#endif

void SetSystemLanguageReset(void)
{
	SetLanguageSimplifiedSprite = NULL;
}
