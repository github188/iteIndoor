/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_jd.c
Author:     	zxc
Version:    	
Date: 		2016-08-22
Description: 设置家电主界面
History:
1. Date:
Author:
Modification:
*************************************************/
#include "../layer_set.h"

static ITUCoverFlow* SetJDCoverFlow = NULL;
static ITUCoverFlow* SetJDListCoverFlow = NULL;
static ITUText* SetJDList1Text[64] = { NULL };
static ITUText* SetJDList2Text[64] = { NULL };
static ITUContainer* SetJDListContainer[64] = { NULL };
static ITUText* SetJDTitleText = NULL;
static ITULayer* SetProjectLayer = NULL;
static ITUContainer* SetJDUpdataContainer = NULL;
static ITUContainer* SetJDFactorySelsctContainer = NULL;
static ITUSprite* SetJDFactorySelsctSprite = NULL;

static JIADIAN_PAGE g_page = JD_ALL;
static PJD_DEV_LIST_INFO g_devlist = NULL;
static PJD_SCENE_INFO_LIST g_SceneList = NULL;

/*************************************************
Function:		SetJDCoverFlowButtonOnMouseUp
Description: 	显示家电设置列表
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void ShowSetJDCoverFlowList(JIADIAN_PAGE page)
{
	int i, max = 0;
	char tmp[60];
	AU_JD_DEV_TYPE dev;
	uint32 text_id1[5] = { SID_Jd_Lamp_Set, SID_Jd_ChuangLian_Set, SID_Jd_KongTiao_Set, SID_Jd_Dianyuan_Set, SID_Jd_Meiqi_Set };
	uint32 text_id2[5] = { SID_Jd_Lamp, SID_Jd_ChuangLian, SID_Jd_KongTiao, SID_Jd_Dianyuan, SID_Jd_Meiqi };

	free_jd_memory(&g_devlist);
	switch (page)
	{
	case JD_LIGHT:
		dev = JD_DEV_LIGHT;
#ifdef _AURINE_ELEC_NEW_
		if (JD_FACTORY_ACBUS == storage_get_extmode(EXT_MODE_JD_FACTORY))
		{
			max = MAX_JD_LIGHT;
		}
		else
		{
			max = MAX_JD_LIGHT / 2;
		}
#else
		max = MAX_JD_LIGHT;
#endif
		break;

	case JD_WINDOW:
		dev = JD_DEV_WINDOW;
		max = MAX_JD_WINDOW;
		break;

	case JD_KONGTIAO:
		dev = JD_DEV_KONGTIAO;
		max = MAX_JD_KONGTIAO;
		break;

	case JD_POWER:
		dev = JD_DEV_POWER;
		max = MAX_JD_POWER;
		break;

	case JD_GAS:
		dev = JD_DEV_GAS;
		max = MAX_JD_GAS;
		break;
	}

	g_devlist = storage_get_jddev(dev, FALSE);
	if (g_devlist)
	{
		ituTextSetString(SetJDTitleText, get_str(text_id1[page - 1]));

		g_devlist->nCount = max;
		for (i = 0; i < max; i++)
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", get_str(text_id2[dev]), i + 1);
			ituTextSetString(SetJDList1Text[i], tmp);

			if (g_devlist->pjd_dev_info[i].IsUsed)
			{
				ituTextSetString(SetJDList2Text[i], get_str(SID_Set_Used));
			}
			else
			{
				ituTextSetString(SetJDList2Text[i], get_str(SID_Set_UnUsed));
			}

			ituWidgetSetVisible(SetJDListContainer[i], true);
		}

		for (i = max; i < MAX_JD_LIGHT; i++)
		{
			ituWidgetSetVisible(SetJDListContainer[i], false);
		}
	}
}

/*************************************************
Function:		SetJDOnEnter
Description: 	设置家电界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJDOnEnter(ITUWidget* widget, char* param)
{
	if (!SetJDCoverFlow)
	{
		SetJDCoverFlow = ituSceneFindWidget(&theScene, "SetJDCoverFlow");
		assert(SetJDCoverFlow);

		SetJDListCoverFlow = ituSceneFindWidget(&theScene, "SetJDListCoverFlow");
		assert(SetJDListCoverFlow);

		SetJDTitleText = ituSceneFindWidget(&theScene, "SetJDTitleText");
		assert(SetJDTitleText);

		SetProjectLayer = ituSceneFindWidget(&theScene, "SetProjectLayer");
		assert(SetProjectLayer);

		SetJDUpdataContainer = ituSceneFindWidget(&theScene, "SetJDUpdataContainer");
		assert(SetJDUpdataContainer);

		SetJDFactorySelsctContainer = ituSceneFindWidget(&theScene, "SetJDFactorySelsctContainer");
		assert(SetJDFactorySelsctContainer);
	}

	if (strcmp(param, "SetProjectLayer") == 0)
	{
		ituCoverFlowGoto(SetJDCoverFlow, 0);
		ituTextSetString(SetJDTitleText, get_str(SID_Jd_Set));
		#ifdef _IP_MODULE_JD_
		if (JD_FACTORY_ACBUS == storage_get_extmode(EXT_MODE_JD_FACTORY) && get_ipmodule())
		{
			ituWidgetSetVisible(SetJDUpdataContainer, true);
		}
		else
		#endif
		{
			ituWidgetSetVisible(SetJDUpdataContainer, false);
		}

		ituWidgetSetVisible(SetJDListCoverFlow, false);
		ituWidgetSetVisible(SetJDFactorySelsctContainer, false);
		ituWidgetSetVisible(SetJDCoverFlow, true);
	}
	else if (strcmp(param, "SetJdParamLayer") == 0)
	{
		ShowSetJDCoverFlowList(g_page);
	}

	return true;
}

/*************************************************
Function:		SetJDCoverFlowButtonOnMouseUp
Description: 	家电设置按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJDCoverFlowButtonOnMouseUp(ITUWidget* widget, char* param)
{
	if (!SetJDList1Text[0])
	{
		uint8 textname[50];
		uint8 i;
		for (i = 0; i < 64; i++)
		{
			memset(textname, 0, sizeof(textname));
			sprintf(textname, "%s%d", "SetJDList1Text", i);
			SetJDList1Text[i] = ituSceneFindWidget(&theScene, textname);
			assert(SetJDList1Text[i]);

			memset(textname, 0, sizeof(textname));
			sprintf(textname, "%s%d", "SetJDList2Text", i);
			SetJDList2Text[i] = ituSceneFindWidget(&theScene, textname);
			assert(SetJDList2Text[i]);

			memset(textname, 0, sizeof(textname));
			sprintf(textname, "%s%d", "SetJDListContainer", i);
			SetJDListContainer[i] = ituSceneFindWidget(&theScene, textname);
			assert(SetJDListContainer[i]);
		}
	}

	g_page = (JIADIAN_PAGE)atoi(param);
	ShowSetJDCoverFlowList(g_page);

	ituCoverFlowGoto(SetJDListCoverFlow, 0);
	ituWidgetSetVisible(SetJDCoverFlow, false);
	ituWidgetSetVisible(SetJDListCoverFlow, true);

	return true;
}

/*************************************************
Function:		SetJDListCoverFlowButtonOnMouseUp
Description: 	家电选项设置按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJDListCoverFlowButtonOnMouseUp(ITUWidget* widget, char* param)
{
	AU_JD_DEV_TYPE dev;
	int index = atoi(param);

	switch (g_page)
	{
	case JD_LIGHT:
		dev = JD_DEV_LIGHT;
		break;

	case JD_WINDOW:
		dev = JD_DEV_WINDOW;
		break;

	case JD_KONGTIAO:
		dev = JD_DEV_KONGTIAO;
		break;

	case JD_POWER:
		dev = JD_DEV_POWER;
		break;

	case JD_GAS:
		dev = JD_DEV_GAS;
		break;
	}
#ifdef _AURINE_ELEC_RILI_
#else
#ifdef _IP_MODULE_JD_
	if (JD_FACTORY_ACBUS == storage_get_extmode(EXT_MODE_JD_FACTORY))
	{
		if (!get_ipmodule())
		{
			SetJDParam(g_devlist, dev, index);
		}
	}
	else
#endif	
	{
		SetJDParam(g_devlist, dev, index);
	}
#endif
	
	return true;
}

/*************************************************
Function:		SetJDFactoryButtonOnMouseUp
Description: 	家电厂商按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJDFactoryButtonOnMouseUp(ITUWidget* widget, char* param)
{
	//uint32 typeId[JD_FACTORY_MAX] = { JD_FACTORY_X10, JD_FACTORY_ACBUS };
	uint32 ext_factory = storage_get_extmode(EXT_MODE_JD_FACTORY);

	if (!SetJDFactorySelsctSprite)
	{
		SetJDFactorySelsctSprite = ituSceneFindWidget(&theScene, "SetJDFactorySelsctSprite");
		assert(SetJDFactorySelsctSprite);
	}

	if (JD_FACTORY_ACBUS == ext_factory)
	{
		ituSpriteGoto(SetJDFactorySelsctSprite, 1);
	}
	else
	{
		ituSpriteGoto(SetJDFactorySelsctSprite, 0);
	}

	ituTextSetString(SetJDTitleText, get_str(SID_Jd_Factory));
	ituWidgetSetVisible(SetJDCoverFlow, false);
	ituWidgetSetVisible(SetJDFactorySelsctContainer, true);

	return true;
}

/*************************************************
Function:		SetJDUpdataButtonOnMouseUp
Description: 	家电同步按键
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJDUpdataButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 res = ECHO_ERROR;
#ifdef _IP_MODULE_JD_
	if (JD_FACTORY_ACBUS == storage_get_extmode(EXT_MODE_JD_FACTORY))
	{
		if (!get_ipmodule_online())
		{
			ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_ERROR, SID_Msg_IP_Unline, "SetJDLayer");
		}
		else
		{
			res = ipmodule_jd_net_get_all_info();
			if (res == ECHO_OK)
			{
			}
			else
			{
				ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_ERROR, SID_Msg_Obtain_Jd_False, "SetJDLayer");
			}
		}
	}
#endif

	return true;
}

/*************************************************
Function:		SetJDLayerOnReturn
Description: 	底部退出键按下执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
void SetJDLayerOnReturn(void)
{
	if (ituWidgetIsVisible(SetJDCoverFlow))
	{
		ituLayerGoto(SetProjectLayer);
		return;
	}
	else if (ituWidgetIsVisible(SetJDFactorySelsctContainer))
	{
		ituWidgetSetVisible(SetJDFactorySelsctContainer, false);
		ituTextSetString(SetJDTitleText, get_str(SID_Jd_Set));
		ituWidgetSetVisible(SetJDCoverFlow, true);
	}
	else if (ituWidgetIsVisible(SetJDListCoverFlow))
	{
		ituWidgetSetVisible(SetJDListCoverFlow, false);
		ituTextSetString(SetJDTitleText, get_str(SID_Jd_Set));
		ituWidgetSetVisible(SetJDCoverFlow, true);
		return;
	}
}
