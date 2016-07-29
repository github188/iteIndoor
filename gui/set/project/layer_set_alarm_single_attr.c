/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_alarm_single_attr.c
Author:     	zxc
Version:
Date: 		2016-07-20
Description:
History:
1. Date:
Author:
Modification:
*************************************************/
#include "gui_include.h"

typedef enum										// 属性设置界面选项
{
	ATTR_ENABLE = 0,		//是否启用
	ATTR_AREA_TYPE,			//探头类型
	ATTR_DELAY,				//报警延迟
	ATTR_PART_VALID,		//局防有效
	ATTR_24H,				//是否24小时防区
	ATTR_PICTURE,			// 画面
	ATTR_VOICE,				// 是否有声音
	ATTR_FINDER,			//探头状态
	MAX_LIST_NUM
}ATTR_ITEM_TYPE_E;

static ITUText* SetAlarmSingleAttrTitleText = NULL;
static ITUText* SetAlarmSingleAttrList1Text[8] = { NULL };
static ITUText* SetAlarmSingleAttrList2Text[8] = { NULL }; 
static ITUCalendar* SetAlarmSingleAttrListContainer[8] = { NULL };
static ITUSprite* SetAlarmSingleAttrEnableSprite = NULL;
static ITUSprite* SetAlarmSingleAttrJufangSprite = NULL;
static ITUSprite* SetAlarmSingleAttr24hSprite = NULL;
static ITUSprite* SetAlarmSingleAttrSeeSprite = NULL;
static ITUSprite* SetAlarmSingleAttrHearSprite = NULL;
static ITUCoverFlow* SetAlarmSingleAttrListCoverFlow = NULL;
static ITUBackground* SetAlarmSingleAttrTimeMsgBackground = NULL;
static ITUBackground* SetAlarmSingleAttrBackground = NULL;
static ITURadioBox* SetAlarmSingleAttrTimeMsgRadioBox[5] = { NULL };

static AF_FLASH_DATA g_ui_secu_data;
static uint8  g_area_num =	0;	 					//取值从0-7		

/*************************************************
Function:		SetAlarmSingleAttrOnEnter
Description: 	防区参数属性设置界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void SetAlarmSingleAttrListShow()
{
	uint8 i = 0;
	uint8 attr_val[MAX_LIST_NUM];
	uint32 secondColStr[MAX_LIST_NUM];
	uint32 delay_time_type[] = { SID_Bj_0Seconds, SID_Bj_30Seconds, SID_Bj_1Minute, SID_Bj_2Minutes, SID_Bj_5Minutes };

	attr_val[0] = (g_ui_secu_data.enable >> g_area_num) & 0x01;
	attr_val[1] = g_ui_secu_data.area_type[g_area_num];
	attr_val[2] = g_ui_secu_data.delay_time[g_area_num];
	attr_val[3] = (g_ui_secu_data.part_valid >> g_area_num) & 0x01;
	attr_val[4] = (g_ui_secu_data.is_24_hour >> g_area_num) & 0x01;
	attr_val[5] = (g_ui_secu_data.can_see >> g_area_num) & 0x01;
	attr_val[6] = (g_ui_secu_data.can_hear >> g_area_num) & 0x01;
	attr_val[7] = (g_ui_secu_data.finder_state >> g_area_num) & 0x01;
	
	secondColStr[0] = SID_Set_UnUsed + attr_val[0];
	secondColStr[1] = SID_Bj_SOS + attr_val[1];
	secondColStr[2] = delay_time_type[g_ui_secu_data.delay_time[g_area_num]];
	secondColStr[3] = SID_Set_User_AfJuFang_Close + attr_val[3];
	secondColStr[4] = SID_Bj_Set_24h_Unused + attr_val[4];
	secondColStr[5] = SID_Bj_Set_UnSee + attr_val[5];
	secondColStr[6] = SID_Bj_Set_UnHear + attr_val[6];
	secondColStr[7] = SID_Bj_Tantou_Close + attr_val[7];

	ituTextSetString(SetAlarmSingleAttrTitleText, get_str(SID_Bj_Fangqu1 + g_area_num));
	for (i = 0; i < MAX_LIST_NUM; i++)
	{
		ituTextSetString(SetAlarmSingleAttrList2Text[i], get_str(secondColStr[i]));
	}

	if (1 == ((g_ui_secu_data.enable >> g_area_num) & 0x01))
	{
		ituSpriteGoto(SetAlarmSingleAttrEnableSprite, 1);
		ituSetColor(&((ITUWidget*)SetAlarmSingleAttrList1Text[ATTR_FINDER])->color, 255, 255, 255, 255);
		ituSetColor(&((ITUWidget*)SetAlarmSingleAttrList2Text[ATTR_FINDER])->color, 255, 255, 255, 255);
		ituWidgetEnable(SetAlarmSingleAttrListContainer[ATTR_FINDER]);

		if (0 == g_area_num)
		{
			for (i = ATTR_AREA_TYPE; i < ATTR_FINDER; i++)
			{
				ituSetColor(&((ITUWidget*)SetAlarmSingleAttrList1Text[i])->color, 255, 128, 138, 135);//冷灰
				ituSetColor(&((ITUWidget*)SetAlarmSingleAttrList2Text[i])->color, 255, 192, 192, 192);//灰色
				ituWidgetDisable(SetAlarmSingleAttrListContainer[i]);
			}
			ituSpriteGoto(SetAlarmSingleAttrJufangSprite, 2);
			ituSpriteGoto(SetAlarmSingleAttr24hSprite, 2);
			ituSpriteGoto(SetAlarmSingleAttrSeeSprite, 2);
			ituSpriteGoto(SetAlarmSingleAttrHearSprite, 2);
		}
		else
		{
			for (i = ATTR_AREA_TYPE; i < ATTR_FINDER; i++)
			{
				ituWidgetEnable(SetAlarmSingleAttrListContainer[i]);
				ituSetColor(&((ITUWidget*)SetAlarmSingleAttrList1Text[i])->color, 255, 255, 255, 255);
				ituSetColor(&((ITUWidget*)SetAlarmSingleAttrList2Text[i])->color, 255, 255, 255, 255);
			}

			if (1 == ((g_ui_secu_data.is_24_hour >> g_area_num) & 0x01))
			{
				ituSpriteGoto(SetAlarmSingleAttr24hSprite, 1);
				if (1 == ((g_ui_secu_data.part_valid >> g_area_num) & 0x01))
				{
					ituSpriteGoto(SetAlarmSingleAttrJufangSprite, 2);
				}
				else
				{
					ituSpriteGoto(SetAlarmSingleAttrJufangSprite, 0);
				}
				ituWidgetDisable(SetAlarmSingleAttrListContainer[ATTR_DELAY]);
				ituWidgetDisable(SetAlarmSingleAttrListContainer[ATTR_PART_VALID]);
				ituSetColor(&((ITUWidget*)SetAlarmSingleAttrList1Text[ATTR_DELAY])->color, 255, 128, 138, 135);//冷灰
				ituSetColor(&((ITUWidget*)SetAlarmSingleAttrList2Text[ATTR_DELAY])->color, 255, 192, 192, 192);//灰色
				ituSetColor(&((ITUWidget*)SetAlarmSingleAttrList1Text[ATTR_PART_VALID])->color, 255, 128, 138, 135);//冷灰
				ituSetColor(&((ITUWidget*)SetAlarmSingleAttrList2Text[ATTR_PART_VALID])->color, 255, 192, 192, 192);//灰色	
			}
			else
			{
				ituSpriteGoto(SetAlarmSingleAttr24hSprite, 0);
				if (1 == ((g_ui_secu_data.part_valid >> g_area_num) & 0x01))
				{
					ituSpriteGoto(SetAlarmSingleAttrJufangSprite, 1);
				}
				else
				{
					ituSpriteGoto(SetAlarmSingleAttrJufangSprite, 0);
				}
			}

			if (1 == ((g_ui_secu_data.can_see >> g_area_num) & 0x01))
			{
				ituSpriteGoto(SetAlarmSingleAttrSeeSprite, 1);
			}
			else
			{
				ituSpriteGoto(SetAlarmSingleAttrSeeSprite, 0);
			}
			if (1 == ((g_ui_secu_data.can_hear >> g_area_num) & 0x01))
			{
				ituSpriteGoto(SetAlarmSingleAttrHearSprite, 1);
			}
			else
			{
				ituSpriteGoto(SetAlarmSingleAttrHearSprite, 0);
			}
		}
	}
	else
	{
		ituSpriteGoto(SetAlarmSingleAttrEnableSprite, 0);
		for (i = ATTR_AREA_TYPE; i < MAX_LIST_NUM; i++)
		{
			ituSetColor(&((ITUWidget*)SetAlarmSingleAttrList1Text[i])->color, 255, 128, 138, 135);//冷灰
			ituSetColor(&((ITUWidget*)SetAlarmSingleAttrList2Text[i])->color, 255, 192, 192, 192);//灰色
			ituWidgetDisable(SetAlarmSingleAttrListContainer[i]);
		}

		if (1 == ((g_ui_secu_data.is_24_hour >> g_area_num) & 0x01))
		{
			ituSpriteGoto(SetAlarmSingleAttr24hSprite, 2);
			if (1 == ((g_ui_secu_data.part_valid >> g_area_num) & 0x01))
			{
				ituSpriteGoto(SetAlarmSingleAttrJufangSprite, 2);
			}
			else
			{
				ituSpriteGoto(SetAlarmSingleAttrJufangSprite, 0);
			}
		}
		else
		{
			ituSpriteGoto(SetAlarmSingleAttr24hSprite, 0);
			if (1 == ((g_ui_secu_data.part_valid >> g_area_num) & 0x01))
			{
				ituSpriteGoto(SetAlarmSingleAttrJufangSprite, 2);
			}
			else
			{
				ituSpriteGoto(SetAlarmSingleAttrJufangSprite, 0);
			}
		}

		if (1 == ((g_ui_secu_data.can_see >> g_area_num) & 0x01))
		{
			ituSpriteGoto(SetAlarmSingleAttrSeeSprite, 2);
		}
		else
		{
			ituSpriteGoto(SetAlarmSingleAttrSeeSprite, 0);
		}
		if (1 == ((g_ui_secu_data.can_hear >> g_area_num) & 0x01))
		{
			ituSpriteGoto(SetAlarmSingleAttrHearSprite, 2);
		}
		else
		{
			ituSpriteGoto(SetAlarmSingleAttrHearSprite, 0);
		}
	}
}

/*************************************************
Function:		SetAlarmSingleAttrOnEnter
Description: 	防区参数属性设置界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetAlarmSingleAttrOnEnter(ITUWidget* widget, char* param)
{
	if (!SetAlarmSingleAttrTitleText)
	{
		uint8 i;
		char tmp[50];
		for (i = 0; i < 8; i++)
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d%s", "SetAlarmSingleAttrList", i, "1Text");
			SetAlarmSingleAttrList1Text[i] = ituSceneFindWidget(&theScene, tmp);
			assert(SetAlarmSingleAttrList1Text[i]);

			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d%s", "SetAlarmSingleAttrList", i, "2Text");
			SetAlarmSingleAttrList2Text[i] = ituSceneFindWidget(&theScene, tmp);
			assert(SetAlarmSingleAttrList2Text[i]);

			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d%s", "SetAlarmSingleAttrList", i, "Container");
			SetAlarmSingleAttrListContainer[i] = ituSceneFindWidget(&theScene, tmp);
			assert(SetAlarmSingleAttrListContainer[i]);
		}

		for (i = 0; i < 5; i++)
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d%s", "SetAlarmSingleAttrTimeMsg", i, "RadioBox");
			SetAlarmSingleAttrTimeMsgRadioBox[i] = ituSceneFindWidget(&theScene, tmp);
			assert(SetAlarmSingleAttrTimeMsgRadioBox[i]); 
		}

		SetAlarmSingleAttrTitleText = ituSceneFindWidget(&theScene, "SetAlarmSingleAttrTitleText");
		assert(SetAlarmSingleAttrTitleText);

		SetAlarmSingleAttrEnableSprite = ituSceneFindWidget(&theScene, "SetAlarmSingleAttrEnableSprite");
		assert(SetAlarmSingleAttrEnableSprite);

		SetAlarmSingleAttrJufangSprite = ituSceneFindWidget(&theScene, "SetAlarmSingleAttrJufangSprite");
		assert(SetAlarmSingleAttrJufangSprite);

		SetAlarmSingleAttr24hSprite = ituSceneFindWidget(&theScene, "SetAlarmSingleAttr24hSprite");
		assert(SetAlarmSingleAttr24hSprite);

		SetAlarmSingleAttrSeeSprite = ituSceneFindWidget(&theScene, "SetAlarmSingleAttrSeeSprite");
		assert(SetAlarmSingleAttrSeeSprite); 

		SetAlarmSingleAttrHearSprite = ituSceneFindWidget(&theScene, "SetAlarmSingleAttrHearSprite");
		assert(SetAlarmSingleAttrHearSprite);

		SetAlarmSingleAttrListCoverFlow = ituSceneFindWidget(&theScene, "SetAlarmSingleAttrListCoverFlow");
		assert(SetAlarmSingleAttrListCoverFlow);

		SetAlarmSingleAttrTimeMsgBackground = ituSceneFindWidget(&theScene, "SetAlarmSingleAttrTimeMsgBackground");
		assert(SetAlarmSingleAttrTimeMsgBackground);

		SetAlarmSingleAttrBackground = ituSceneFindWidget(&theScene, "SetAlarmSingleAttrBackground");
		assert(SetAlarmSingleAttrBackground);
	}

	ituCoverFlowGoto(SetAlarmSingleAttrListCoverFlow, 0);

	logic_get_alarm_param((uint8*)&g_ui_secu_data);
	SetAlarmSingleAttrListShow();

	ituWidgetSetVisible(SetAlarmSingleAttrTimeMsgBackground, false);
	ituWidgetSetVisible(SetAlarmSingleAttrBackground, true);
	
	return true;
}

/*************************************************
Function:		AlarmAreaButtonOnMouseUp
Description: 	列表按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetAlarmSingleAttrListButtonOnMouseUp(ITUWidget* widget, char* param)
{
	int index = atoi(param);

	switch (index)
	{
	case ATTR_ENABLE:		
		if (1 == ((g_ui_secu_data.enable >> (g_area_num)) & 0x01))
		{
			SET_ONE_VALUE(g_ui_secu_data.enable, 0, g_area_num);
		}
		else
		{
			SET_ONE_VALUE(g_ui_secu_data.enable, 1, g_area_num);
		}

		logic_set_alarm_param((uint8*)(&g_ui_secu_data));
		SetAlarmSingleAttrListShow();
		break;

	case ATTR_AREA_TYPE:
		
		break;

	case ATTR_DELAY:
		ituRadioBoxSetChecked(SetAlarmSingleAttrTimeMsgRadioBox[g_ui_secu_data.delay_time[g_area_num]], true);
		ituWidgetDisable(SetAlarmSingleAttrBackground);
		ituWidgetSetVisible(SetAlarmSingleAttrTimeMsgBackground, true);
		break;

	case ATTR_PART_VALID:
		if (1 == ((g_ui_secu_data.part_valid >> g_area_num) & 0x01))
		{
			SET_ONE_VALUE(g_ui_secu_data.part_valid, 0, g_area_num);
		}
		else
		{
			SET_ONE_VALUE(g_ui_secu_data.part_valid, 1, g_area_num);
		}

		logic_set_alarm_param((uint8*)(&g_ui_secu_data));
		SetAlarmSingleAttrListShow();
		break;

	case ATTR_24H:
		if (1 == ((g_ui_secu_data.is_24_hour >> g_area_num) & 0x01))
		{
			SET_ONE_VALUE(g_ui_secu_data.is_24_hour, 0, g_area_num);
		}
		else
		{
			SET_ONE_VALUE(g_ui_secu_data.is_24_hour, 1, g_area_num);
			g_ui_secu_data.delay_time[g_area_num] = 0;
			SET_ONE_VALUE(g_ui_secu_data.part_valid, 1, g_area_num);
		}

		logic_set_alarm_param((uint8*)(&g_ui_secu_data));
		SetAlarmSingleAttrListShow();
		break;

	case ATTR_PICTURE:
		if (1 == ((g_ui_secu_data.can_see >> g_area_num) & 0x01))
		{
			SET_ONE_VALUE(g_ui_secu_data.can_see, 0, g_area_num);
		}
		else
		{
			SET_ONE_VALUE(g_ui_secu_data.can_see, 1, g_area_num);
		}

		logic_set_alarm_param((uint8*)(&g_ui_secu_data));
		SetAlarmSingleAttrListShow();
		break;

	case ATTR_VOICE:
		if (1 == ((g_ui_secu_data.can_hear >> g_area_num) & 0x01))
		{
			SET_ONE_VALUE(g_ui_secu_data.can_hear, 0, g_area_num);
		}
		else
		{
			SET_ONE_VALUE(g_ui_secu_data.can_hear, 1, g_area_num);
		}

		logic_set_alarm_param((uint8*)(&g_ui_secu_data));
		SetAlarmSingleAttrListShow();
		break;

	case ATTR_FINDER:
		if (1 == ((g_ui_secu_data.finder_state >> g_area_num) & 0x01))
		{
			SET_ONE_VALUE(g_ui_secu_data.finder_state, 0, g_area_num);
		}
		else
		{
			SET_ONE_VALUE(g_ui_secu_data.finder_state, 1, g_area_num);
		}

		logic_set_alarm_param((uint8*)(&g_ui_secu_data));
		SetAlarmSingleAttrListShow();
		break;
	}

	return true;
}

/*************************************************
Function:		SetAlarmSingleAttrTimeMsgRadioBoxOnMouseUp
Description: 	时间列表按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetAlarmSingleAttrTimeMsgRadioBoxOnMouseUp(ITUWidget* widget, char* param)
{
	int index = atoi(param);
	uint32 delay_time_type[] = { SID_Bj_0Seconds, SID_Bj_30Seconds, SID_Bj_1Minute, SID_Bj_2Minutes, SID_Bj_5Minutes };

	g_ui_secu_data.delay_time[g_area_num] = (uint8)index;

	ituTextSetString(SetAlarmSingleAttrList2Text[ATTR_DELAY], get_str(delay_time_type[index]));

	logic_set_alarm_param((uint8*)(&g_ui_secu_data));
	
	ituWidgetSetVisible(SetAlarmSingleAttrTimeMsgBackground, false);
	ituWidgetEnable(SetAlarmSingleAttrBackground);

	return true;
}

/*************************************************
Function:		SetAlarmSingleAttr
Description:	安防设置单个属性窗口
Input:
1.hDlg		当前处理的窗口
2.num 		防区号
Output:
Return:
Others:
*************************************************/
uint32 SetAlarmSingleAttr(uint8 num)
{
	ITULayer* SetAlarmSingleAttrLayer = NULL;
	SetAlarmSingleAttrLayer = ituSceneFindWidget(&theScene, "SetAlarmSingleAttrLayer");
	assert(SetAlarmSingleAttrLayer);

	g_area_num = num;

	if (!ituWidgetIsVisible(SetAlarmSingleAttrLayer))
	{
		ituLayerGoto(SetAlarmSingleAttrLayer);
	}

	return TRUE;
}

/*************************************************
Function:		SetAlarmSingleAttrLayerOnReturn
Description: 	底部退出键按下执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
void SetAlarmSingleAttrLayerOnReturn(void)
{
	if (ituWidgetIsVisible(SetAlarmSingleAttrTimeMsgBackground))
	{
		ituWidgetSetVisible(SetAlarmSingleAttrTimeMsgBackground, false);
		ituWidgetEnable(SetAlarmSingleAttrBackground);
		return;
	}
	else 
	{
		SetAlarmAreaList(3);
		return;
	}
}

