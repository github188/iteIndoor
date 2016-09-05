/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_extmodule.c
Author:     	zxc
Version:
Date: 		2016-07-06
Description:	外部模块界面
*************************************************/
#include "../layer_set.h"

static ITUSprite* SetExtJDSprite = NULL;
static ITUSprite* SetExtDoor1Sprite = NULL;
static ITUSprite* SetExtDoor2Sprite = NULL;
static ITUSprite* SetExtStairSprite = NULL;
static ITUText* SetExtJD2Text = NULL;
static ITUText* SetExtDoor12Text = NULL;
static ITUText* SetExtDoor22Text = NULL;
static ITUText* SetExtStair2Text = NULL;

static uint32 g_ext[EXT_MODE_MAX];

/*************************************************
Function:		SetAlarmRemoteOnEnterShow
Description: 	进入外部模块设置界面显示函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void SetExtModuleOnEnterShow()
{
	if (TRUE == g_ext[EXT_MODE_JD])
	{
		ituSpriteGoto(SetExtJDSprite, 1);
		ituTextSetString(SetExtJD2Text, get_str(SID_Set_Prj_ExtJD_Used));
	}
	else
	{
		ituSpriteGoto(SetExtJDSprite, 0);
		ituTextSetString(SetExtJD2Text, get_str(SID_Set_Prj_ExtJD_Unused));
	}

	if (TRUE == g_ext[EXT_MODE_NETDOOR1])
	{
		ituSpriteGoto(SetExtDoor1Sprite, 1);
		ituTextSetString(SetExtDoor12Text, get_str(SID_Set_Prj_ExtDoor1_Used));
	}
	else
	{
		ituSpriteGoto(SetExtDoor1Sprite, 0);
		ituTextSetString(SetExtDoor12Text, get_str(SID_Set_Prj_ExtDoor1_Unused));
	}

	if (TRUE == g_ext[EXT_MODE_NETDOOR2])
	{
		ituSpriteGoto(SetExtDoor2Sprite, 1);
		ituTextSetString(SetExtDoor22Text, get_str(SID_Set_Prj_ExtDoor2_Used));
	}
	else
	{
		ituSpriteGoto(SetExtDoor2Sprite, 0);
		ituTextSetString(SetExtDoor22Text, get_str(SID_Set_Prj_ExtDoor2_Unused));
	}

	if (TRUE == g_ext[EXT_MODE_GENERAL_STAIR])
	{
		ituSpriteGoto(SetExtStairSprite, 1);
		ituTextSetString(SetExtStair2Text, get_str(SID_Set_Prj_ExtStair_Used));
	}
	else
	{
		ituSpriteGoto(SetExtStairSprite, 0);
		ituTextSetString(SetExtStair2Text, get_str(SID_Set_Prj_ExtStair_Unused));
	}
}

/*************************************************
Function:		SetExtModuleOnEnter
Description: 	进入外部模块设置界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetExtModuleOnEnter(ITUWidget* widget, char* param)
{
	uint8 i = 0;

	if (!SetExtJDSprite)
	{
		SetExtJDSprite = ituSceneFindWidget(&theScene, "SetExtJDSprite");
		assert(SetExtJDSprite);

		SetExtDoor1Sprite = ituSceneFindWidget(&theScene, "SetExtDoor1Sprite");
		assert(SetExtDoor1Sprite);

		SetExtDoor2Sprite = ituSceneFindWidget(&theScene, "SetExtDoor2Sprite");
		assert(SetExtDoor2Sprite);

		SetExtStairSprite = ituSceneFindWidget(&theScene, "SetExtStairSprite");
		assert(SetExtStairSprite);

		SetExtJD2Text = ituSceneFindWidget(&theScene, "SetExtJD2Text");
		assert(SetExtJD2Text);

		SetExtDoor12Text = ituSceneFindWidget(&theScene, "SetExtDoor12Text");
		assert(SetExtDoor12Text);

		SetExtDoor22Text = ituSceneFindWidget(&theScene, "SetExtDoor22Text");
		assert(SetExtDoor22Text);

		SetExtStair2Text = ituSceneFindWidget(&theScene, "SetExtStair2Text");
		assert(SetExtStair2Text);
	}

	// 取外部模块启用状态
	for (i = 0; i < EXT_MODE_MAX; i++)
	{
		g_ext[i] = storage_get_extmode(EXT_MODE_ALARM + i);
	}

	SetExtModuleOnEnterShow();

	return true;
}

/*************************************************
Function:		SetExtModuleListButtonOnMouseUp
Description: 	外部模块列表按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetExtModuleListButtonOnMouseUp(ITUWidget* widget, char* param)
{
	int index = atoi(param);

	switch (index)
	{
	case EXT_MODE_JD:
		if (TRUE == g_ext[EXT_MODE_JD])
		{
			g_ext[EXT_MODE_JD] = FALSE;
			ituSpriteGoto(SetExtJDSprite, 0);
			ituTextSetString(SetExtJD2Text, get_str(SID_Set_Prj_ExtJD_Unused));
		}
		else
		{
			g_ext[EXT_MODE_JD] = TRUE;
			ituSpriteGoto(SetExtJDSprite, 1);
			ituTextSetString(SetExtJD2Text, get_str(SID_Set_Prj_ExtJD_Used));
		}

		storage_set_extmode(1, EXT_MODE_JD, g_ext[EXT_MODE_JD]);
	/*
		#ifdef _JD_MODE_
		if (storage_get_extmode(EXT_MODE_JD))
		{
			jd_logic_init();
			#ifdef _AURINE_ELEC_NEW_
			jd_aurine_init();
			#endif

			#ifdef _IP_MODULE_JD_
			if (get_ipmodule())
			{
				JD_FACTORY_TYPE factory = storage_get_extmode(EXT_MODE_JD_FACTORY);
				if (factory != g_ext[EXT_MODE_JD_FACTORY])
				{
					hw_stop_feet_dog();
				}
			}
			#endif
		}
		#endif
	*/
		break;

	case EXT_MODE_NETDOOR1:
		if (TRUE == g_ext[EXT_MODE_NETDOOR1])
		{
			g_ext[EXT_MODE_NETDOOR1] = FALSE;
			ituSpriteGoto(SetExtDoor1Sprite, 0);
			ituTextSetString(SetExtDoor12Text, get_str(SID_Set_Prj_ExtDoor1_Unused));
		}
		else
		{
			g_ext[EXT_MODE_NETDOOR1] = TRUE;
			ituSpriteGoto(SetExtDoor1Sprite, 1);
			ituTextSetString(SetExtDoor12Text, get_str(SID_Set_Prj_ExtDoor1_Used));
		}

		storage_set_extmode(1, EXT_MODE_NETDOOR1, g_ext[EXT_MODE_NETDOOR1]);

		break;

	case EXT_MODE_NETDOOR2:
		if (TRUE == g_ext[EXT_MODE_NETDOOR2])
		{
			g_ext[EXT_MODE_NETDOOR2] = FALSE;
			ituSpriteGoto(SetExtDoor2Sprite, 0);
			ituTextSetString(SetExtDoor22Text, get_str(SID_Set_Prj_ExtDoor2_Unused));
		}
		else
		{
			g_ext[EXT_MODE_NETDOOR2] = TRUE;
			ituSpriteGoto(SetExtDoor2Sprite, 1);
			ituTextSetString(SetExtDoor22Text, get_str(SID_Set_Prj_ExtDoor2_Used));
		}

		storage_set_extmode(1, EXT_MODE_NETDOOR2, g_ext[EXT_MODE_NETDOOR2]);

		break;

	case EXT_MODE_GENERAL_STAIR:
		if (TRUE == g_ext[EXT_MODE_GENERAL_STAIR])
		{
			g_ext[EXT_MODE_GENERAL_STAIR] = FALSE;
			ituSpriteGoto(SetExtStairSprite, 0);
			ituTextSetString(SetExtStair2Text, get_str(SID_Set_Prj_ExtStair_Unused));
		}
		else
		{
			g_ext[EXT_MODE_GENERAL_STAIR] = TRUE;
			ituSpriteGoto(SetExtStairSprite, 1);
			ituTextSetString(SetExtStair2Text, get_str(SID_Set_Prj_ExtStair_Used));
		}

		storage_set_extmode(0, EXT_MODE_GENERAL_STAIR, g_ext[EXT_MODE_GENERAL_STAIR]);

		break;
	}

	return true;
}
