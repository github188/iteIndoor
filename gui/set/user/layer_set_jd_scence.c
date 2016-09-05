/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_jd_scence.c
Author:     	zxc
Version:
Date: 		2016-08-29
Description: 家电情景设置
*************************************************/
#include "../layer_set.h"

static ITUCoverFlow* SetJdScenceListCoverFlow = NULL;
static ITUText* SetJDSceneList1Text[32] = { NULL };
static ITUText* SetJDSceneList2Text[32] = { NULL };

static PJD_SCENE_INFO_LIST g_SceneList = NULL;

/*************************************************
Function:		ShowSetJDScenceCoverFlowList
Description: 	显示情景设置列表
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void ShowSetJDScenceCoverFlowList()
{
	int i, max = 0;
	char text_tmp[80];
	uint32 TextID[JD_SCENE_MAX] = { SID_Jd_Mode_Huike, SID_Jd_Mode_Jiucan, SID_Jd_Mode_Yejian, SID_Jd_Mode_Jiedian, SID_Jd_Mode_Putong };

	free_scene_memory(&g_SceneList);
	g_SceneList = storage_get_aurine_scene_info(0);
	if (g_SceneList)
	{
		g_SceneList->nCount = 32;
		max = 32;
	}

	for (i = 0; i < max; i++)
	{
		if (i < JD_SCENE_MAX)
		{
			ituTextSetString(SetJDSceneList1Text[i], get_str(TextID[i]));
			ituTextSetString(SetJDSceneList2Text[i], get_str(SID_Set_Used));
		}
		else
		{
			memset(text_tmp, 0, sizeof(text_tmp));

			if (g_SceneList->pjd_scene_info[i].IsUsed == 1)
			{
				ituTextSetString(SetJDSceneList2Text[i], get_str(SID_Set_Used));

				#ifdef _IP_MODULE_JD_
				if (JD_FACTORY_ACBUS == storage_get_extmode(EXT_MODE_JD_FACTORY) && get_ipmodule())
				{
					strcpy(text_tmp, g_SceneList->pjd_scene_info[i].SceneName);
				}
				else
				#endif
				{
					#ifdef _USERDEFINE_JDNAME_
					strcpy(text_tmp1, g_SceneList->pjd_scene_info[i].SceneName);
					#else
					strcpy(text_tmp, get_str(g_SceneList->pjd_scene_info[i].TextIDPos));
					strcat(text_tmp, get_str(g_SceneList->pjd_scene_info[i].TextIDName));
				#endif
				}
			}
			else
			{
				ituTextSetString(SetJDSceneList2Text[i], get_str(SID_Set_UnUsed));

				sprintf(text_tmp, "%s%d", get_str(SID_Jd_Mode_Self), i + 1);
			}

			ituTextSetString(SetJDSceneList1Text[i], text_tmp);
		}
	}
}

/*************************************************
Function:		SetJDParamOnEnter
Description: 	界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJdScenceOnEnter(ITUWidget* widget, char* param)
{
	if (!SetJdScenceListCoverFlow)
	{
		char tmp[50];
		uint8 i = 0;

		SetJdScenceListCoverFlow = ituSceneFindWidget(&theScene, "SetJdScenceListCoverFlow");
		assert(SetJdScenceListCoverFlow);

		for (i = 0; i < 32; i++)
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "SetJDSceneList1Text", i);
			SetJDSceneList1Text[i] = ituSceneFindWidget(&theScene, tmp);
			assert(SetJDSceneList1Text[i]);

			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "SetJDSceneList2Text", i);
			SetJDSceneList2Text[i] = ituSceneFindWidget(&theScene, tmp);
			assert(SetJDSceneList2Text[i]);
		}
	}

	if (strcmp(param, "SetUserLayer") == 0)
	{
		ituCoverFlowGoto(SetJdScenceListCoverFlow, 0);
	}

	ShowSetJDScenceCoverFlowList();

	return true;
}

/*************************************************
Function:		SetJDSceneListCoverFlowButtonOnMouseUp
Description: 	情景设置选项设置按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJDSceneListCoverFlowButtonOnMouseUp(ITUWidget* widget, char* param)
{
	int index = atoi(param);

	SetAurinJDSceneList(index, g_SceneList);
	
	return true;
}
