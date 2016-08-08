/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_extmodule.c
Author:     	zxc
Version:
Date: 		2016-07-11
Description:	设备编号界面
*************************************************/
#include "gui_include.h"

typedef enum
{
	DEVICE_RULE = 0,	//0：设备编号
	STAIR_LEN,			//1：梯号长度
	ROOM_LEN,			//2：房号长度
	RISER_ENABLE,		//3：启用单元号
	RISER_LEN,			//4：单元号长度
	DEVICE_MAX,
}DEVICE_BTN_PAGE;

static ITUText* SetDevnoStr2Text;
static ITUText* SetDevnoStairLen2Text;
static ITUText* SetDevnoRoomLen2Text;
static ITUText* SetDevnoUseCellLen2Text;
static ITUText* SetDevnoCellLen2Text;
static ITUText* SetDevnoCellLen1Text;
static ITUSprite* SetDevnoUseCellLenSprite;
static ITUCalendar* SetDevnoCellLenContainer;
static ITULayer* SetProjectLayer;
static ITUTextBox* SetNumKeyBordTextBox;
//static ITULayer* SetNumKeyBordLayer;
static ITUBackground* SetDevnoMsgErrBackground;
static ITUBackground* SetDevnoBackground;
static ITUText* SetDevnoMsgErrText;

static PFULL_DEVICE_NO g_devparam = NULL;
static DEVICE_BTN_PAGE g_button_num = DEVICE_MAX;
//static char * g_get_keybord_text = NULL;

/*************************************************
Function:    		comepare_devno_rule
Description:		比较设备编号规则
Input:
1.rule			设备编号规则
Output:			无
Return:			0-成功 1-单元长度非法 2-梯号长度非法 3-房号长度非法
4-总长度不能大于18
Others:
*************************************************/
static uint32 comepare_devno_rule(DEVICENO_RULE Rule)
{
	//合法判断
	if (Rule.CellNoLen > 2)
	{
		return 1;
	}
	if (Rule.StairNoLen < Rule.CellNoLen || Rule.StairNoLen > 9)
	{
		return 2;
	}
	if (Rule.RoomNoLen < 3 || Rule.RoomNoLen > 9)
	{
		return 3;
	}
	if ((Rule.StairNoLen + Rule.RoomNoLen) > 17)
	{
		return 4;
	}
	if (Rule.CellNoLen == 0)
	{
		Rule.UseCellNo = FALSE;
	}

	return 0;
}

/*************************************************
Function:    		comepare_devno_len
Description:		判断设备编号长度是否正确
Input:
1.devno			设备号字符串
Output:			无
Return:
0-成功
1-长度非法
2-房号段全为0
3-区口机编号必须在1-40之间
Others:
*************************************************/
static uint32 comepare_devno_len(char* DevNo)
{
	char temp[20];
	int nlen = 0;

	if (NULL == DevNo)
	{
		return 1;
	}

	nlen = strlen(DevNo);
	if (nlen != g_devparam->Rule.StairNoLen + g_devparam->Rule.RoomNoLen + 1)
	{
		return 1;
	}

	strncpy(temp, DevNo + g_devparam->Rule.StairNoLen, g_devparam->Rule.RoomNoLen);
	temp[g_devparam->Rule.RoomNoLen] = 0;
	if (0 == atoi(temp))
	{
		return 2;
	}

	return 0;
}

/*************************************************
Function:		save_devno_param
Description: 	保存
Input:
1.hDlg
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void save_devno_param(void)
{
	uint32 text_id[4] = { SID_Set_Prj_Rule_Riser_Err, SID_Set_Prj_Rule_Stair_Err, SID_Set_Prj_Rule_Room_Err, SID_Set_Prj_Rule_RuleAll_Err };
	uint8 ret1, ret = TRUE;
	ret1 = FALSE;

	ret = comepare_devno_rule(g_devparam->Rule);
	switch (ret)
	{
	case 0:										// 成功
		//memcpy(&g_devparam->Rule, storage_get_devrule(),sizeof(DEVICENO_RULE));
		break;

	case 1:										// 单元长度非法
		break;

	case 2:										// 梯号长度非法
		break;

	case 3:										// 房号长度非法
		break;

	case 4:										// 总长度不能大于18
		break;
	}

	if (0 == ret)
	{
		ret1 = comepare_devno_len(g_devparam->DeviceNoStr);
		switch (ret1)
		{
		case 0:
			g_devparam->Rule.Subsection = (g_devparam->Rule.StairNoLen - g_devparam->Rule.CellNoLen) * 100 + g_devparam->Rule.CellNoLen * 10 + g_devparam->Rule.RoomNoLen;
			storage_save_devno(TRUE, g_devparam->Rule, g_devparam->DeviceNoStr);
			memcpy(g_devparam, storage_get_devparam(), sizeof(FULL_DEVICE_NO));
			//net_change_comm_deviceno();
			break;

		case 1:
			ituTextSetString(SetDevnoMsgErrText, get_str(SID_Set_Prj_Rule_Len_Err));
			ituWidgetDisable(SetDevnoBackground);
			ituWidgetSetVisible(SetDevnoMsgErrBackground, true);
			break;

		case 2:
			ituTextSetString(SetDevnoMsgErrText, get_str(SID_Set_Prj_Rule_RoomNo_Err));
			ituWidgetDisable(SetDevnoBackground);
			ituWidgetSetVisible(SetDevnoMsgErrBackground, true);
			break;
		}
	}
	else
	{
		ituTextSetString(SetDevnoMsgErrText, get_str(text_id[ret-1]));
		ituWidgetDisable(SetDevnoBackground);
		ituWidgetSetVisible(SetDevnoMsgErrBackground, true);
	}
}

/*************************************************
Function:		PrjGotoSetDevnoOnEnterShow
Description: 	工程设置进入外部模块设置界面显示函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void PrjGotoSetDevnoOnEnterShow()
{
	char tmp[40];

	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp, "%s", g_devparam->DeviceNoStr);
	ituTextSetString(SetDevnoStr2Text, tmp);

	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp, "%d", g_devparam->Rule.StairNoLen);
	ituTextSetString(SetDevnoStairLen2Text, tmp);

	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp, "%d", g_devparam->Rule.RoomNoLen);
	ituTextSetString(SetDevnoRoomLen2Text, tmp);

	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp, "%d", g_devparam->Rule.CellNoLen);
	ituTextSetString(SetDevnoCellLen2Text, tmp);

	if (g_devparam->Rule.UseCellNo)
	{
		ituTextSetString(SetDevnoUseCellLen2Text, get_str(SID_Set_Prj_Rule_Riser_Used));
		ituSpriteGoto(SetDevnoUseCellLenSprite, 1);
		ituSetColor(&((ITUWidget*)SetDevnoCellLen2Text)->color, 255, 255, 255, 255);
		ituSetColor(&((ITUWidget*)SetDevnoCellLen1Text)->color, 255, 255, 255, 255);
	}
	else
	{
		ituTextSetString(SetDevnoUseCellLen2Text, get_str(SID_Set_Prj_Rule_Riser_Unused));
		ituSpriteGoto(SetDevnoUseCellLenSprite, 0);
		ituSetColor(&((ITUWidget*)SetDevnoCellLen2Text)->color, 255, 128, 138, 135);//冷灰
		ituSetColor(&((ITUWidget*)SetDevnoCellLen1Text)->color, 255, 192, 192, 192);//灰色
		ituWidgetDisable(SetDevnoCellLenContainer);
	}

	ituWidgetSetVisible(SetDevnoMsgErrBackground, false);
	ituWidgetSetVisible(SetDevnoBackground, true);
}

/*************************************************
Function:		KeyBordGotoSetDevnoOnEnterShow
Description: 	键盘设置进入外部模块设置界面显示函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void KeyBordGotoSetDevnoOnEnterShow()
{
	char tmp[40];
	uint8 nlen = 0;
	uint32 text = 0;
	char * get_keybord_text = NULL;
	uint32 text_id[5] = { SID_Set_Prj_Rule_Device_Wrong, SID_Set_Prj_Rule_Stair_Null, SID_Set_Prj_Rule_Room_Null, 0, SID_Set_Prj_Rule_Riser_Null };

	get_keybord_text = ituTextGetString(SetNumKeyBordTextBox);
	nlen = strlen(get_keybord_text);
	text = (uint8)atoi(get_keybord_text);

	switch (g_button_num)
	{
	case DEVICE_RULE:
		if (nlen > 0)
		{
			memset(g_devparam->DeviceNoStr, 0, sizeof(g_devparam->DeviceNoStr));
			memcpy(g_devparam->DeviceNoStr, get_keybord_text, nlen);

			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s", g_devparam->DeviceNoStr);
			ituTextSetString(SetDevnoStr2Text, tmp);
		}
		break;

	case STAIR_LEN:
		if (nlen > 0)
		{
			g_devparam->Rule.StairNoLen = text;

			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%d", g_devparam->Rule.StairNoLen);
			ituTextSetString(SetDevnoStairLen2Text, tmp);
		}
		break;

	case ROOM_LEN:
		if (nlen > 0)
		{
			g_devparam->Rule.RoomNoLen = text;

			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%d", g_devparam->Rule.RoomNoLen);
			ituTextSetString(SetDevnoRoomLen2Text, tmp);
		}
		break;

	case RISER_LEN:
		if (nlen > 0)
		{
			g_devparam->Rule.CellNoLen = text;

			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%d", g_devparam->Rule.CellNoLen);
			ituTextSetString(SetDevnoCellLen2Text, tmp);
		}
		break;
	}

	if (nlen > 0)
	{
	}
	else
	{
		ituTextSetString(SetDevnoMsgErrText, get_str(text_id[g_button_num]));
		ituWidgetDisable(SetDevnoBackground);
		ituWidgetSetVisible(SetDevnoMsgErrBackground, true);
	}
}

/*************************************************
Function:		SetDevnoOnEnter
Description: 	进入设备编号设置界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetDevnoOnEnter(ITUWidget* widget, char* param)
{
	if (!SetDevnoStr2Text)
	{
		SetDevnoStr2Text = ituSceneFindWidget(&theScene, "SetDevnoStr2Text");
		assert(SetDevnoStr2Text);

		SetDevnoStairLen2Text = ituSceneFindWidget(&theScene, "SetDevnoStairLen2Text");
		assert(SetDevnoStairLen2Text);

		SetDevnoRoomLen2Text = ituSceneFindWidget(&theScene, "SetDevnoRoomLen2Text");
		assert(SetDevnoRoomLen2Text);

		SetDevnoUseCellLen2Text = ituSceneFindWidget(&theScene, "SetDevnoUseCellLen2Text");
		assert(SetDevnoUseCellLen2Text);

		SetDevnoCellLen2Text = ituSceneFindWidget(&theScene, "SetDevnoCellLen2Text");
		assert(SetDevnoCellLen2Text); 

		SetDevnoCellLen1Text = ituSceneFindWidget(&theScene, "SetDevnoCellLen1Text");
		assert(SetDevnoCellLen1Text);

		SetDevnoUseCellLenSprite = ituSceneFindWidget(&theScene, "SetDevnoUseCellLenSprite");
		assert(SetDevnoUseCellLenSprite); 

		SetDevnoCellLenContainer = ituSceneFindWidget(&theScene, "SetDevnoCellLenContainer");
		assert(SetDevnoCellLenContainer); 

		SetProjectLayer = ituSceneFindWidget(&theScene, "SetProjectLayer");
		assert(SetProjectLayer); 

		SetNumKeyBordTextBox = ituSceneFindWidget(&theScene, "SetNumKeyBordTextBox");
		assert(SetNumKeyBordTextBox); 

		//SetNumKeyBordLayer = ituSceneFindWidget(&theScene, "SetNumKeyBordLayer");
		//assert(SetNumKeyBordLayer); 

		SetDevnoMsgErrBackground = ituSceneFindWidget(&theScene, "SetDevnoMsgErrBackground");
		assert(SetDevnoMsgErrBackground);

		SetDevnoBackground = ituSceneFindWidget(&theScene, "SetDevnoBackground");
		assert(SetDevnoBackground); 

		SetDevnoMsgErrText = ituSceneFindWidget(&theScene, "SetDevnoMsgErrText");
		assert(SetDevnoMsgErrText);
	}

	if (strcmp(param, "SetNumKeyBordLayer") == 0)
	{
		KeyBordGotoSetDevnoOnEnterShow();

		save_devno_param();
	}
	else
	{
		g_button_num = DEVICE_MAX;

		if (g_devparam)
		{
			free(g_devparam);
			g_devparam = NULL;
		}
		g_devparam = (PFULL_DEVICE_NO)malloc(sizeof(FULL_DEVICE_NO));
		if (NULL == g_devparam)
		{
			return FALSE;
		}
		else
		{
			memset(g_devparam, 0, sizeof(FULL_DEVICE_NO));
		}
		memcpy(g_devparam, storage_get_devparam(), sizeof(FULL_DEVICE_NO));

		PrjGotoSetDevnoOnEnterShow();
	}

	return true;
}

/*************************************************
Function:		SetDevnoListButtonOnMouseUp
Description: 	设备编号列表按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetDevnoListButtonOnMouseUp(ITUWidget* widget, char* param)
{
	char tmp[40];
	int index = atoi(param);
	g_button_num = index;

	switch (g_button_num)
	{
	case DEVICE_RULE:
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s", g_devparam->DeviceNoStr);
		KeybordLayerOnShow(NULL, PASS_TYPE_MAX, 16, EXPRESS_CHAR, CANCEL_BTN, tmp);
		break;

	case STAIR_LEN:
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%d", g_devparam->Rule.StairNoLen);
		KeybordLayerOnShow(NULL, PASS_TYPE_MAX, 1, EXPRESS_CHAR, CANCEL_BTN, tmp);
		break;

	case ROOM_LEN:
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%d", g_devparam->Rule.RoomNoLen);
		KeybordLayerOnShow(NULL, PASS_TYPE_MAX, 1, EXPRESS_CHAR, CANCEL_BTN, tmp);
		break;

	case RISER_LEN:
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%d", g_devparam->Rule.CellNoLen);
		KeybordLayerOnShow(NULL, PASS_TYPE_MAX, 1, EXPRESS_CHAR, CANCEL_BTN, tmp);
		break;

	case RISER_ENABLE:
		if (g_devparam->Rule.UseCellNo)
		{
			g_devparam->Rule.UseCellNo = 0;
			ituTextSetString(SetDevnoUseCellLen2Text, get_str(SID_Set_Prj_Rule_Riser_Unused));
			ituSpriteGoto(SetDevnoUseCellLenSprite, 0);
			ituSetColor(&((ITUWidget*)SetDevnoCellLen1Text)->color, 255, 128, 138, 135);//冷灰
			ituSetColor(&((ITUWidget*)SetDevnoCellLen2Text)->color, 255, 192, 192, 192);//灰色
			ituWidgetDisable(SetDevnoCellLenContainer);
		}
		else
		{
			g_devparam->Rule.UseCellNo = 1;
			ituTextSetString(SetDevnoUseCellLen2Text, get_str(SID_Set_Prj_Rule_Riser_Used));
			ituSpriteGoto(SetDevnoUseCellLenSprite, 1);
			ituSetColor(&((ITUWidget*)SetDevnoCellLen2Text)->color, 255, 255, 255, 255);
			ituSetColor(&((ITUWidget*)SetDevnoCellLen1Text)->color, 255, 255, 255, 255);
			ituWidgetEnable(SetDevnoCellLenContainer);
		}
		return true;
	}

	return true;
}

/*************************************************
Function:		SetDevnoLayerOnReturn
Description: 	底部退出键按下执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
void SetDevnoLayerOnReturn(void)
{
	if (ituWidgetIsVisible(SetDevnoMsgErrBackground))
	{
		ituWidgetEnable(SetDevnoBackground);
		ituWidgetSetVisible(SetDevnoMsgErrBackground, false);
		return;
	}
	else if (ituWidgetIsVisible(SetDevnoBackground))
	{
		if (!ituWidgetIsVisible(SetProjectLayer))
		{
			save_devno_param();
			if (ituWidgetIsVisible(SetDevnoBackground))
			{
				ituLayerGoto(SetProjectLayer);
			}
			return;
		}
	}
}

void SetDevnoLayerReset(void)
{
	SetDevnoStr2Text = NULL;
}
