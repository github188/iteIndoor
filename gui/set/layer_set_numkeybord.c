/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_ketbord.c
Author:     	zxc
Version:
Date: 		2016-06-18
Description:
History:
1. Date:
Author:
Modification:
*************************************************/
#include "gui_include.h"

static ITULayer* GotoOldLayer = NULL;
static ITULayer* GotoNewLayer = NULL;
static ITUTextBox* SetNumKeyBordTextBox = NULL;
static ITUText* SetNumKeyBordHitText = NULL;
static ITUSprite* SetNumKeyBordDianSprite = NULL;
static ITULayer* SetNumKeyBordLayer = NULL;

static uint8 g_numkeybord_textbox_data[50] = {0};
static PASS_TYPE g_UserOrPrj_pwd = PASS_TYPE_MAX;

/*************************************************
Function:		SetNumKeyBordOnEnter
Description: 	进入数字键盘界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetNumKeyBordOnEnter(ITUWidget* widget, char* param)
{
	dprintf("SetNumKeyBordOnEnter param = %s\n", param);

	GotoOldLayer = ituSceneFindWidget(&theScene, param);
	assert(GotoOldLayer);

	SetNumKeyBordTextBox->text.layout = ITU_LAYOUT_MIDDLE_CENTER;

	ituWidgetDisable(GotoOldLayer);
	ituWidgetShow(GotoOldLayer, ITU_EFFECT_NONE, 0);

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
void SetNumKeyBordLayerOnReturn(void)
{
	//if (!ituWidgetIsVisible(GotoOldLayer))
	{
		ituTextBoxSetString(SetNumKeyBordTextBox, g_numkeybord_textbox_data);

		ituWidgetEnable(GotoOldLayer);
		ituLayerGoto(GotoOldLayer);
	}
}

/*************************************************
Function:		SetNumKeyBord1ButtonOnMouseUp
Description: 	键盘按键按下执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetNumKeyBord1ButtonOnMouseUp(ITUWidget* widget, char* param)
{
	ITUButton* btn = (ITUButton*)widget;
	char* input = ituTextGetString(&btn->text);
	char* str_textbox = ituTextGetString(SetNumKeyBordTextBox);
	int count_textbox = str_textbox ? strlen(str_textbox) : 0;
	char* str_text = ituTextGetString(SetNumKeyBordHitText);
	int count_text = str_text ? strlen(str_text) : 0;

	if (count_text > 0)
		ituTextSetString(SetNumKeyBordHitText, "");

	if (count_textbox > SetNumKeyBordTextBox->maxLen)
		ituTextSetString(SetNumKeyBordTextBox, NULL);

	if (strcmp(input, ".") == 0)	//按下“.”按钮
	{
		if (0 == SetNumKeyBordDianSprite->frame)	//根据SetNumKeyBordDianSprite里面Icon叠放顺序，1是显示“.”； 0是显示“X”
		{	
			SetNumKeyBordLayerOnReturn();
			return true;
		}
	}

	ituTextBoxInput(SetNumKeyBordTextBox, input);

	return true;
}

/*************************************************
Function:		PasswordDeal
Description: 	密码判断函数执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static bool PasswordDeal(char* pass)
{
	char userpass[7];	
	char prjpass[7]; 
	memset(userpass, 0, sizeof(userpass));
	memset(prjpass, 0, sizeof(prjpass));
	sprintf(prjpass, "%s", storage_get_pass(PASS_TYPE_ADMIN));	//存储获取工程密码;
	sprintf(userpass, "%s", storage_get_pass(PASS_TYPE_USER));  //存储获取用户密码

	if (PASS_TYPE_USER == g_UserOrPrj_pwd)
	{
		if (0 == strcmp(pass, userpass) || (0 == strcmp(pass, prjpass)))
		{
			g_UserOrPrj_pwd = PASS_TYPE_MAX;
			//if (!ituWidgetIsVisible(GotoNewLayer))
			{
				ituWidgetEnable(GotoOldLayer);
				ituLayerGoto(GotoNewLayer);
				return true;
			}
		}
		else
		{
			ituTextBoxSetString(SetNumKeyBordTextBox, NULL);
			ituTextSetString(SetNumKeyBordHitText, get_str(SID_Set_Pwd_Err));
			return false;
		}
	}
	else 
	{
		if (0 == strcmp(pass, prjpass))
		{
			g_UserOrPrj_pwd = PASS_TYPE_MAX;
			//if (!ituWidgetIsVisible(GotoNewLayer))
			{
				ituWidgetEnable(GotoOldLayer);
				ituLayerGoto(GotoNewLayer);
				return true;
			}
		}
		else
		{
			ituTextBoxSetString(SetNumKeyBordTextBox, NULL);
			ituTextSetString(SetNumKeyBordHitText, get_str(SID_Set_Pwd_Err));
			return false;
		}
	}

	return false;
}

/*************************************************
Function:		SetNumKeyBordYesButtonOnMouseUp
Description: 	键盘确认键按下执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetNumKeyBordYesButtonOnMouseUp(ITUWidget* widget, char* param)
{
	char* TextBox_data = ituTextGetString(SetNumKeyBordTextBox);

	if ((PASS_TYPE_USER == g_UserOrPrj_pwd) || (PASS_TYPE_ADMIN == g_UserOrPrj_pwd))//密码判断
	{
		return PasswordDeal(TextBox_data);
	}

	//if (!ituWidgetIsVisible(GotoOldLayer))
	{
		ituWidgetEnable(GotoOldLayer);
		ituLayerGoto(GotoOldLayer);
	}
	
	return true;
}

/*************************************************
Function:		KeybordLayerOnShow
Description: 	显示键盘界面
Input:		
	1、widget:	点击确认后进入的下一个界面的widget
	2、pass_type：输入密码的类型
	3、text_maxlen：输入框输入的最大字符长度
	4、text_flag：明文显示密文显示；	 33：输入的用“*”显示；  32：输入的用数字显示
	5、sprite_flag：	0：键盘显示“X”；  1：键盘显示“.”
	6、buf： 传入显示的数据
Output:		无
Return:		
Others:
*************************************************/
void KeybordLayerOnShow(ITULayer* widget, PASS_TYPE pass_type, int text_maxlen, NUM_CAHR_TYPE text_flag, SPRITE_KEYBORD_BTN_TYPE sprite_flag, char * buf)
{
	if (!SetNumKeyBordTextBox)
	{
		SetNumKeyBordTextBox = ituSceneFindWidget(&theScene, "SetNumKeyBordTextBox");
		assert(SetNumKeyBordTextBox);

		SetNumKeyBordHitText = ituSceneFindWidget(&theScene, "SetNumKeyBordHitText");
		assert(SetNumKeyBordHitText);

		SetNumKeyBordDianSprite = ituSceneFindWidget(&theScene, "SetNumKeyBordDianSprite");
		assert(SetNumKeyBordDianSprite);

		SetNumKeyBordLayer = ituSceneFindWidget(&theScene, "SetNumKeyBordLayer");
		assert(SetNumKeyBordLayer);
	}

	GotoNewLayer = widget;
	g_UserOrPrj_pwd = pass_type;

	SetNumKeyBordTextBox->maxLen = text_maxlen;					// 修改输入框输入的最大个数
	SetNumKeyBordTextBox->textboxFlags = text_flag;					 // 33：输入的用“*”显示；   32：输入的用数字显示
	ituSpriteGoto(SetNumKeyBordDianSprite, sprite_flag);			// 0：键盘显示“X”；  1：键盘显示“.”
	
	if (PASS_TYPE_USER == g_UserOrPrj_pwd)
	{
		ituTextSetString(SetNumKeyBordHitText, get_str(SID_Set_EnterUserPwd));
	}
	else if (PASS_TYPE_ADMIN == g_UserOrPrj_pwd)
	{
		ituTextSetString(SetNumKeyBordHitText, get_str(SID_Set_EnterProjectPwd));
	}
	else
	{
		ituTextSetString(SetNumKeyBordHitText, "");
	}

	if (NULL != buf)
	{
		ituTextBoxSetString(SetNumKeyBordTextBox, NULL);
		sprintf(g_numkeybord_textbox_data, "%s", buf);
		ituTextBoxInput(SetNumKeyBordTextBox, g_numkeybord_textbox_data);
	}
	else
	{
		ituTextBoxSetString(SetNumKeyBordTextBox, NULL);
	}

	ituLayerGoto(SetNumKeyBordLayer);

}

/*************************************************
Function:		MsgFailHintSuccessLayerOnLeave
Description:
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetNumKeyBordLayerOnLeave(ITUWidget* widget, char* param)
{
	if (!ituWidgetIsEnabled(GotoOldLayer))
	{
		ituWidgetEnable(GotoOldLayer);
	}
	return true;
}

void SetNumKeyBordReset(void)
{
	SetNumKeyBordTextBox = NULL;
	GotoOldLayer = NULL;
	GotoNewLayer = NULL;
}
