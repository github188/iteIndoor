/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_alarm_area_list.c
Author:     	zxc
Version:
Date: 		2016-07-02
Description: 1:隔离 2:局防有效 3:防区属性 界面
History:
1. Date:
Author:
Modification:
*************************************************/
#include "../layer_set.h"

static ITUSprite* AlarmAreaSprite[8] = { NULL };
static ITUText* AlarmArea2Text[8] = { NULL };
static ITUText* AlarmAreaListTitleText = NULL;
static ITUCoverFlow* AlarmAreaListCoverFlow = NULL;
static ITULayer* SetAlarmAreaListLayer = NULL;

static ITULayer* OldLayer = NULL;
static uint8  g_isolate = 0;						// 防区隔离参数
static uint8  g_part_valid = 0;						// 防区局防有效参数
static uint8  g_page_type = 0;						// 1:隔离 2:局防有效 3:防区属性 
static AF_FLASH_DATA g_alarm_data = { 0 };

/*************************************************
Function:		SetAlarmAreaLlistOnEnter
Description: 	防区隔离、局防有效、防区设置设置界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void SetAlarmAreaListShowStart(void)
{
	uint8 i = 0;
	uint8 text_tmp[100];
	uint8 tmp[30];

	logic_get_alarm_param((uint8*)&g_alarm_data);

	switch (g_page_type)
	{
	case 1:
		g_isolate = logic_get_area_isolation();
		ituTextSetString(AlarmAreaListTitleText, get_str(SID_Set_User_AfGeLi));

		for (i = 0; i < 8; i++)
		{
			if (1 == (0x01 & (g_isolate >> i)))
			{
				ituSpriteGoto(AlarmAreaSprite[i], 1);
				ituTextSetString(AlarmArea2Text[i], get_str(SID_Set_User_AfGeLi_Open));
			}
			else
			{
				ituSpriteGoto(AlarmAreaSprite[i], 0);
				ituTextSetString(AlarmArea2Text[i], get_str(SID_Set_User_AfGeLi_Close));
			}
		}
		break;

	case 2:
		g_part_valid = logic_get_part_valid();
		ituTextSetString(AlarmAreaListTitleText, get_str(SID_Set_User_AfJuFang));

		for (i = 0; i < 8; i++)
		{
			if (1 == (0x01 & (g_part_valid >> i)))
			{
				if (1 == ((g_alarm_data.is_24_hour >> i) & 0x01))
				{
					ituSpriteGoto(AlarmAreaSprite[i], 2);
				}
				else
				{
					ituSpriteGoto(AlarmAreaSprite[i], 1);
				}
				ituTextSetString(AlarmArea2Text[i], get_str(SID_Set_User_AfJuFang_Open));
			}
			else
			{
				ituSpriteGoto(AlarmAreaSprite[i], 0);
				ituTextSetString(AlarmArea2Text[i], get_str(SID_Set_User_AfJuFang_Close));
			}
		}
		break;

	case 3:
		ituTextSetString(AlarmAreaListTitleText, get_str(SID_Set_Prj_AfProperty));

		for (i = 0; i < 8; i++)
		{
			ituSpriteGoto(AlarmAreaSprite[i], 3);	

			memset(tmp, 0, sizeof(tmp));
			memset(text_tmp, 0, sizeof(text_tmp));
			strcpy(text_tmp, get_str(SID_Bj_SOS + g_alarm_data.area_type[i]));
			sprintf(tmp, "%s", ",");
			strcat(text_tmp, tmp);
			if (1 == ((g_alarm_data.enable >> i) & 0x01))
			{
				memset(tmp, 0, sizeof(tmp));
				strcpy(tmp, get_str(SID_Set_Used));
			}
			else
			{
				memset(tmp, 0, sizeof(tmp));
				strcpy(tmp, get_str(SID_Set_UnUsed));
			}
			strcat(text_tmp, tmp);
			
			ituTextSetString(AlarmArea2Text[i], text_tmp);
		}
		break; 
	}	
}

/*************************************************
Function:		SetAlarmAreaLlistOnEnter
Description: 	防区隔离、局防有效、防区设置设置界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetAlarmAreaListOnEnter(ITUWidget* widget, char* param)
{
	if (!AlarmAreaListTitleText)
	{
		uint8 i;
		char tmp[50];
		for (i = 0; i < 8; i++)
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d%s", "AlarmArea", i+1, "Sprite");
			AlarmAreaSprite[i] = ituSceneFindWidget(&theScene, tmp);
			assert(AlarmAreaSprite[i]);

			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d%s", "AlarmArea", i+1, "2Text");
			AlarmArea2Text[i] = ituSceneFindWidget(&theScene, tmp);
			assert(AlarmArea2Text[i]);
		}

		AlarmAreaListTitleText = ituSceneFindWidget(&theScene, "AlarmAreaListTitleText");
		assert(AlarmAreaListTitleText); 

		AlarmAreaListCoverFlow = ituSceneFindWidget(&theScene, "AlarmAreaListCoverFlow");
		assert(AlarmAreaListCoverFlow);
	}

	if (strcmp(param, "SetAlarmSingleAttrLayer") != 0)
	{
		OldLayer = ituSceneFindWidget(&theScene, param);
		assert(OldLayer);

		ituCoverFlowGoto(AlarmAreaListCoverFlow, 0);
	}

	SetAlarmAreaListShowStart();
	
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
bool AlarmAreaButtonOnMouseUp(ITUWidget* widget, char* param)
{
	int index = atoi(param);

	switch (g_page_type)
	{
	case 1:
		if (1 == (0x01 & (g_isolate >> index)))
		{
			SET_ONE_VALUE(g_isolate, 0, index);

			ituSpriteGoto(AlarmAreaSprite[index], 0);
			ituTextSetString(AlarmArea2Text[index], get_str(SID_Set_User_AfGeLi_Close));
		}
		else
		{
			SET_ONE_VALUE(g_isolate, 1, index);

			ituSpriteGoto(AlarmAreaSprite[index], 1);
			ituTextSetString(AlarmArea2Text[index], get_str(SID_Set_User_AfGeLi_Open));
		}
		logic_set_area_isolation(g_isolate);
		break;

	case 2:
		if (0 == ((g_alarm_data.is_24_hour >> index) & 0x01))
		{
			if (1 == (0x01 & (g_part_valid >> index)))
			{
				SET_ONE_VALUE(g_part_valid, 0, index);

				ituSpriteGoto(AlarmAreaSprite[index], 0);
				ituTextSetString(AlarmArea2Text[index], get_str(SID_Set_User_AfJuFang_Close));
			}
			else
			{
				SET_ONE_VALUE(g_part_valid, 1, index);

				ituSpriteGoto(AlarmAreaSprite[index], 1);
				ituTextSetString(AlarmArea2Text[index], get_str(SID_Set_User_AfJuFang_Open));
			}
		}
		logic_set_part_valid(g_part_valid);
		break;

	case 3:
		SetAlarmSingleAttr((uint8)index);
		break;
	}

	return true;
}

/*************************************************
Function:			SetAlarmAreaList
Description: 		安防防区列表窗口
Input:
	1.type			1:隔离 2:局防有效 3:防区属性 
Output:
Return:
Others:
*************************************************/
uint32 SetAlarmAreaList(uint8 type)
{
	g_page_type = type;

	if (!SetAlarmAreaListLayer)
	{
		SetAlarmAreaListLayer = ituSceneFindWidget(&theScene, "SetAlarmAreaListLayer");
		assert(SetAlarmAreaListLayer);
	}

	if (!ituWidgetIsVisible(SetAlarmAreaListLayer))
	{
		ituLayerGoto(SetAlarmAreaListLayer);
	}
	
	return TRUE;
}

/*************************************************
Function:		SetAlarmAreaListLayerOnReturn
Description: 	底部退出键按下执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
void SetAlarmAreaListLayerOnReturn(void)
{
	if (!ituWidgetIsVisible(OldLayer))
	{
		ituLayerGoto(OldLayer);
	}
}
