/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_password.c
Author:     	zxc
Version:
Date: 		2016-06-21
Description:
History:
1. Date:
Author:
Modification:
*************************************************/
#include "gui_include.h"

typedef enum
{
	OPENDOORPASSWORDPAGE = 0,	//0：开门密码界面；
	ALARMPASSWORDPAGE,			//1：安防密码界面
	PRJPASSWORDPAGE,			//2：工程密码设置
	NONESETPASSWORDPAGE,
}PASSWOEDPAGE;

typedef enum
{
	NEWPWDBOTTON = 0,			//0：新密码按键；
	TRUENEWPWDBOTTON,			//1：确认新密码按键
	NONEBOTTON,
}NEWORTUREBOTTON;

static ITUBackground* SetPasswordBackground;
static ITUBackground* SetPasswordMsgBackground;
//static ITUBackground* SetPasswordMsgOKBackground;
static ITUText* SetPasswordTitleText;
static ITUText* SetUserPwd1Text;
static ITUText* SetUserPwd2Text;
static ITUText* SetServanPwd1Text;
static ITUText* SetServanPwd2Text; 
static ITUText* SetPasswordMsgTitleText;
static ITULayer* SetMenuLayer;
static ITUText* SetPasswordMsgNewPassBtnText;
static ITUText* SetPasswordMsgTruePassBtnText;
static ITUTextBox* SetNumKeyBordTextBox;
static ITURadioBox* SetPasswordMsgNewPassBtnRadioBox;
static ITURadioBox* SetPasswordMsgTruePassBtnRadioBox;
//static ITULayer* SetNumKeyBordLayer;
static ITULayer* SetProjectLayer;
//static ITULayer* MsgFailHintSuccessLayer;

static PASS_TYPE g_type_pass = PASS_TYPE_MAX;			//密码类型
static PASSWOEDPAGE g_page_pass = NONESETPASSWORDPAGE;	//开门密码界面\安防密码界面\工程密码界面
static NEWORTUREBOTTON g_new_ture_btn = NONEBOTTON;		//确认密码\新密码按键

static uint8 g_new_pass[MAX_ADMIN_LEN + 1];
static uint8 g_ture_pass[MAX_ADMIN_LEN + 1];
static uint8 g_background_flag = 0;

/*************************************************
Function:		SetPasswordOnEnter
Description: 	进入密码设置界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetPasswordOnEnter(ITUWidget* widget, char* param)
{
	uint8 get_pass[10];
	uint8 buf1[MAX_ADMIN_LEN + 1];
	uint8 buf2[MAX_ADMIN_LEN + 1];

	if (!SetPasswordBackground)
	{
		SetPasswordBackground = ituSceneFindWidget(&theScene, "SetPasswordBackground");
		assert(SetPasswordBackground); 

		SetPasswordMsgBackground = ituSceneFindWidget(&theScene, "SetPasswordMsgBackground");
		assert(SetPasswordMsgBackground); 

		//SetPasswordMsgOKBackground = ituSceneFindWidget(&theScene, "SetPasswordMsgOKBackground");
		//assert(SetPasswordMsgOKBackground);

		SetPasswordTitleText = ituSceneFindWidget(&theScene, "SetPasswordTitleText");
		assert(SetPasswordTitleText);

		SetUserPwd1Text = ituSceneFindWidget(&theScene, "SetUserPwd1Text");
		assert(SetUserPwd1Text);

		SetUserPwd2Text = ituSceneFindWidget(&theScene, "SetUserPwd2Text");
		assert(SetUserPwd2Text);

		SetServanPwd1Text = ituSceneFindWidget(&theScene, "SetServanPwd1Text");
		assert(SetServanPwd1Text);

		SetServanPwd2Text = ituSceneFindWidget(&theScene, "SetServanPwd2Text");
		assert(SetServanPwd2Text); 

		SetPasswordMsgTitleText = ituSceneFindWidget(&theScene, "SetPasswordMsgTitleText");
		assert(SetPasswordMsgTitleText);

		SetMenuLayer = ituSceneFindWidget(&theScene, "SetMenuLayer");
		assert(SetMenuLayer);    

		SetPasswordMsgNewPassBtnText = ituSceneFindWidget(&theScene, "SetPasswordMsgNewPassBtnText");
		assert(SetPasswordMsgNewPassBtnText);

		SetPasswordMsgTruePassBtnText = ituSceneFindWidget(&theScene, "SetPasswordMsgTruePassBtnText");
		assert(SetPasswordMsgTruePassBtnText);

		SetNumKeyBordTextBox = ituSceneFindWidget(&theScene, "SetNumKeyBordTextBox");
		assert(SetNumKeyBordTextBox);   

		SetPasswordMsgNewPassBtnRadioBox = ituSceneFindWidget(&theScene, "SetPasswordMsgNewPassBtnRadioBox");
		assert(SetPasswordMsgNewPassBtnRadioBox);

		SetPasswordMsgTruePassBtnRadioBox = ituSceneFindWidget(&theScene, "SetPasswordMsgTruePassBtnRadioBox");
		assert(SetPasswordMsgTruePassBtnRadioBox);

		//SetNumKeyBordLayer = ituSceneFindWidget(&theScene, "SetNumKeyBordLayer");
		//assert(SetNumKeyBordLayer);		

		SetProjectLayer = ituSceneFindWidget(&theScene, "SetProjectLayer");
		assert(SetProjectLayer); 

		//MsgFailHintSuccessLayer = ituSceneFindWidget(&theScene, "MsgFailHintSuccessLayer");
		//assert(MsgFailHintSuccessLayer);
	}

	if (1 == g_background_flag)
	{
		g_background_flag = 0;

		memset(get_pass, 0, sizeof(get_pass));
		switch (g_page_pass)
		{
		case OPENDOORPASSWORDPAGE:
			ituTextSetString(SetPasswordTitleText, get_str(SID_Set_DoorPwd));
			ituTextSetString(SetUserPwd1Text, get_str(SID_Set_Pass_DoorUser));
			ituTextSetString(SetUserPwd2Text, get_str(SID_Set_Pass_Change_DoorUser));
			ituTextSetString(SetServanPwd1Text, get_str(SID_Set_Pass_DoorServer));
			sprintf(get_pass, "%s", storage_get_pass(PASS_TYPE_DOOR_SERVER));
			ituTextSetString(SetServanPwd2Text, get_pass);

			ituWidgetSetVisible(SetPasswordMsgBackground, false);
			ituWidgetSetVisible(SetPasswordBackground, true);
			break;

		case ALARMPASSWORDPAGE:
			ituTextSetString(SetPasswordTitleText, get_str(SID_Set_SecurityPwd));
			ituTextSetString(SetUserPwd1Text, get_str(SID_Set_Pass_AlarmUser));
			ituTextSetString(SetUserPwd2Text, get_str(SID_Set_Pass_Change_AlarmUser));
			ituTextSetString(SetServanPwd1Text, get_str(SID_Set_Pass_AlarmServer));
			sprintf(get_pass, "%s", storage_get_pass(PASS_TYPE_SERVER));
			ituTextSetString(SetServanPwd2Text, get_pass);

			ituWidgetSetVisible(SetPasswordMsgBackground, false);
			ituWidgetSetVisible(SetPasswordBackground, true);
			break;

		case PRJPASSWORDPAGE:
			g_type_pass = PASS_TYPE_ADMIN;
			ituTextSetString(SetPasswordMsgTitleText, get_str(SID_Set_Pass_Project));

			ituTextSetString(SetPasswordMsgTruePassBtnText, "");
			ituTextSetString(SetPasswordMsgNewPassBtnText, "");
			ituRadioBoxSetChecked(SetPasswordMsgNewPassBtnRadioBox, true);

			//ituWidgetDisable(SetProjectLayer);
			ituWidgetSetVisible(SetPasswordBackground, false);
			ituWidgetSetVisible(SetPasswordMsgBackground, true);
			break;
		}		
	}
	else
	{
		char* strbuf = ituTextGetString(SetNumKeyBordTextBox);
		int count_str = strbuf ? strlen(strbuf) : 0;

		memset(buf1, 0, sizeof(buf1));
		memset(buf2, 0, sizeof(buf2));
		memcpy(buf1, ".....", strlen(g_new_pass));
		memcpy(buf2, ".....", strlen(g_ture_pass));

		if (NEWPWDBOTTON == g_new_ture_btn)
		{
			ituRadioBoxSetChecked(SetPasswordMsgNewPassBtnRadioBox, true);
			sprintf(g_new_pass, "%s", strbuf);
			memset(buf1, 0, sizeof(buf1));
			memcpy(buf1, ".....", strlen(g_new_pass));
		}
		else if (TRUENEWPWDBOTTON == g_new_ture_btn)
		{
			ituRadioBoxSetChecked(SetPasswordMsgTruePassBtnRadioBox, true);
			sprintf(g_ture_pass, "%s", strbuf);
			memset(buf2, 0, sizeof(buf2));
			memcpy(buf2, ".....", strlen(g_ture_pass));
		}
		else
		{
			ituRadioBoxSetChecked(SetPasswordMsgNewPassBtnRadioBox, true);
		}
		g_new_ture_btn = NONEBOTTON;
		ituTextSetString(SetPasswordMsgNewPassBtnText, buf1);
		ituTextSetString(SetPasswordMsgTruePassBtnText, buf2);
		ituWidgetDisable(SetPasswordBackground);
		ituWidgetSetVisible(SetPasswordMsgBackground, true);
	}

	if (PRJPASSWORDPAGE == g_page_pass)
	{
		ituWidgetShow(SetProjectLayer, ITU_EFFECT_NONE, 0);
	}

	//ituWidgetSetVisible(SetPasswordMsgOKBackground, false);
	return true;
}

/*************************************************
Function:		SetUserPwdButtonOnMouseUp
Description: 	住户开门密码或者住户安防密码按键按下执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetUserPwdButtonOnMouseUp(ITUWidget* widget, char* param)
{
	if (OPENDOORPASSWORDPAGE == g_page_pass)
	{
		g_type_pass = PASS_TYPE_DOOR_USER;
		ituTextSetString(SetPasswordMsgTitleText, get_str(SID_Set_Pass_DoorUser));
	}
	else if (ALARMPASSWORDPAGE == g_page_pass)
	{
		g_type_pass = PASS_TYPE_USER;
		ituTextSetString(SetPasswordMsgTitleText, get_str(SID_Set_Pass_AlarmUser));
	}

	ituTextSetString(SetPasswordMsgTruePassBtnText, "");
	ituTextSetString(SetPasswordMsgNewPassBtnText, "");
	ituRadioBoxSetChecked(SetPasswordMsgNewPassBtnRadioBox, true);
	ituWidgetDisable(SetPasswordBackground);
	ituWidgetSetVisible(SetPasswordMsgBackground, true);

	return true;
}

/*************************************************
Function:		SetServanPwdButtonOnMouseUp
Description: 	家政开门密码或者住家政防密码按键按下执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetServanPwdButtonOnMouseUp(ITUWidget* widget, char* param)
{
	if (OPENDOORPASSWORDPAGE == g_page_pass)
	{
		g_type_pass = PASS_TYPE_DOOR_SERVER;
		ituTextSetString(SetPasswordMsgTitleText, get_str(SID_Set_Pass_DoorServer));
	}
	else if (ALARMPASSWORDPAGE == g_page_pass)
	{
		g_type_pass = PASS_TYPE_SERVER;
		ituTextSetString(SetPasswordMsgTitleText, get_str(SID_Set_Pass_AlarmServer));
	}

	ituTextSetString(SetPasswordMsgTruePassBtnText, "");
	ituTextSetString(SetPasswordMsgNewPassBtnText, "");
	ituRadioBoxSetChecked(SetPasswordMsgNewPassBtnRadioBox, true);
	ituWidgetDisable(SetPasswordBackground);
	ituWidgetSetVisible(SetPasswordMsgBackground, true);

	return true;
}

/*************************************************
Function:		SetPasswordMsgNewPassBtnRadioBoxOnMouseUp
Description: 	新密码按下
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetPasswordMsgNewPassBtnRadioBoxOnMouseUp(ITUWidget* widget, char* param)
{
	g_new_ture_btn = NEWPWDBOTTON;
	//ituLayerGoto(SetNumKeyBordLayer);
	KeybordLayerOnShow(NULL, PASS_TYPE_MAX, 5, 33, 0, NULL);

	return true;
}

/*************************************************
Function:		SetPasswordMsgTruePassBtnRadioBoxOnMouseUp
Description: 	确认新密码按下
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetPasswordMsgTruePassBtnRadioBoxOnMouseUp(ITUWidget* widget, char* param)
{
	g_new_ture_btn = TRUENEWPWDBOTTON;
	//ituLayerGoto(SetNumKeyBordLayer);
	KeybordLayerOnShow(NULL, PASS_TYPE_MAX, 5, 33, 0, NULL);

	return true;
}

/*************************************************
Function:		SetPasswordMsgTureButtonOnMouseUp
Description: 	确认按下
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetPasswordMsgTureButtonOnMouseUp(ITUWidget* widget, char* param)
{
	if (0 == strcmp(g_new_pass, g_ture_pass))
	{	
		printf("g_new_pass = %s\n", g_new_pass);
	
		storage_set_pass(g_type_pass, g_new_pass);

		if ((PASS_TYPE_SERVER == g_type_pass) || (PASS_TYPE_DOOR_SERVER == g_type_pass))
		{
			ituTextSetString(SetServanPwd2Text, g_new_pass);
		}
		
		if (PASS_TYPE_ADMIN == g_type_pass)
		{
			ituLayerGoto(SetProjectLayer);
		}
		else
		{
			ituWidgetSetVisible(SetPasswordMsgBackground, false);
			ituWidgetEnable(SetPasswordBackground);
		}
	}
	else
	{
		//ituWidgetSetVisible(SetPasswordMsgBackground, false);
		//ituWidgetSetVisible(SetPasswordMsgOKBackground, true);
		ShowMsgFailHintSuccessLayer(0, SID_Set_Pwd_Two_Err);
	}

	memset(g_new_pass, 0, sizeof(g_new_pass));
	memset(g_ture_pass, 0, sizeof(g_ture_pass));
	return true;
}

/*************************************************
Function:		SetPasswordMsgFalseButtonOnMouseUp
Description: 	取消按下
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetPasswordMsgFalseButtonOnMouseUp(ITUWidget* widget, char* param)
{
	memset(g_new_pass, 0, sizeof(g_new_pass));
	memset(g_ture_pass, 0, sizeof(g_ture_pass));

	if (PASS_TYPE_ADMIN == g_type_pass)
	{
		ituLayerGoto(SetProjectLayer);
	}
	else
	{
		ituWidgetSetVisible(SetPasswordMsgBackground, false);
		ituWidgetEnable(SetPasswordBackground);
	}

	return true;
}
#if 0
/*************************************************
Function:		SetPasswordMsgOKButtonOnMouseUp
Description: 	两次密码不同消息框确定键按下
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetPasswordMsgOKButtonOnMouseUp(ITUWidget* widget, char* param)
{
	memset(g_new_pass, 0, sizeof(g_new_pass));
	memset(g_ture_pass, 0, sizeof(g_ture_pass));

	//ituWidgetSetVisible(SetPasswordMsgOKBackground, false);
	ituWidgetEnable(SetPasswordBackground);

	return true;
}
#endif
/*************************************************
Function:		SetNoDisturLayerOnReturn
Description: 	底部退出键按下执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
void SetPasswordLayerOnReturn(void)
{
	if (ituWidgetIsVisible(SetPasswordMsgBackground))
	{
		SetPasswordMsgFalseButtonOnMouseUp(NULL, NULL);
		return;
	}
#if 0
	else if (ituWidgetIsVisible(SetPasswordMsgOKBackground))
	{
		SetPasswordMsgOKButtonOnMouseUp(NULL, NULL);
		return;
	}
#endif
	else if (!ituWidgetIsVisible(SetMenuLayer))
	{
		ituLayerGoto(SetMenuLayer);
		return;
	}
}

/*************************************************
Function:		UpdataPassType
Description: 	进入该页面前先调此函数判断是进入哪个页面
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
void UpdataPassType(int data)
{
	g_page_pass = data;
	g_background_flag = 1;
	memset(g_new_pass, 0, sizeof(g_new_pass));
	memset(g_ture_pass, 0, sizeof(g_ture_pass));
}

void SetPasswordReset(void)
{
	SetPasswordBackground = NULL;
}
