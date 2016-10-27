/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_jd_yuyue_param.c
Author:     	zxc
Version:
Date: 		2016-10-18
Description: 家电预约参数设置界面
*************************************************/
#include "../layer_set.h"

#define MAX_CLONE_NUM			64					// clone的最大条数

static ITULayer* SetJdYuYueLayer = NULL;
static ITULayer* SetJdYuYueParamLayer = NULL;
static ITUBackground* SetJdYuYueParamBackground = NULL;
static ITUBackground* SetJdYuyueTypeBackground = NULL;
static ITUBackground* SetJdYuyueDeviceBackground = NULL;
static ITUBackground* SetJdYuYueParamBrightnessBackground = NULL;
static ITUBackground* SetJdYuYueParamTimeTypeBackground = NULL;
static ITUBackground* SetJdYuYueParamTimeDateBackground = NULL;
static ITUBackground* SetJdYuYueParamTimeTimeBackground = NULL;
static ITUBackground* SetJdYuYueParamTimeWeekBackground = NULL;

static ITUCoverFlow* SetJdYuYueParamListCoverFlow = NULL;
static ITUText* SetJdYuYueParamYuyueType2Text = NULL;
static ITUText* SetJdYuYueParamDevice2Text = NULL;
static ITUText* SetJdYuYueParamOpera2Text = NULL;
static ITUText* SetJdYuYueParamBrightness2Text = NULL;
static ITUText* SetJdYuYueParamTimeType2Text = NULL;
static ITUText* SetJdYuYueParamTimeDate2Text = NULL;
static ITUText* SetJdYuYueParamTimeTime2Text = NULL;
static ITUText* SetJdYuYueParamTimeWeek2Text = NULL;
static ITUContainer* SetJdYuYueParamBrightnessContainer = NULL;
static ITUText* SetJdYuYueParamBrightness1Text = NULL;
static ITUText* SetJdYuYueParamTimeDate1Text = NULL;
static ITUText* SetJdYuYueParamTimeWeek1Text = NULL;
static ITUText* SetJdYuYueParamDevice1Text = NULL;
static ITUText* SetJdYuYueParamOpera1Text = NULL;

static ITURadioBox* SetJdYuyueTypeRadioBox[6] = { NULL };

static ITUContainer* SetJdYuyueDeviceListContainer0 = NULL;
static ITUCoverFlow* SetJdYuyueDeviceListCoverFlow = NULL;
static ITUText* SetJdYuyueDeviceText = NULL;
static ITUButton* SetJdYuyueDeviceButton[MAX_CLONE_NUM] = { NULL };
static ITUSprite* SetJdYuyueDeviceSprite[MAX_CLONE_NUM] = { NULL };

static ITUProgressBar* SetJdYuYueParamBrightnessProgressBar = NULL;
static ITUTrackBar* SetJdYuYueParamBrightnessTrackBar = NULL;
static ITUText* SetJdYuYueParamBrightnessTitleText = NULL;

static ITURadioBox* SetJdYuYueParamTimeTypeListRadioBox[3] = { NULL };

static ITUWheel* SetJdYuYueParamTimeDateWheel = NULL;
static ITUWheel* SetJdYuYueParamTimeTimeHourWheel = NULL; 
static ITUWheel* SetJdYuYueParamTimeTimeMinWheel = NULL;

static ITUSprite* SetJdYuYueParamTimeWeekListSprite[7] = { NULL };

static ITURadioBox* MsgFailHintSuccess1RadioBox = NULL;



extern PBE_COMM_LIST JdYuyueList;

static uint16 YuyueRepStr[4][20];
static uint16 YuyueWeekStr[7][10];

static uint8 g_YuyueListIndex = 0; //设置第几个预约
static uint8 g_index = 0;

static BE_COMM YuyueTmp;

static PJD_DEV_LIST_INFO g_devlist = NULL;
static AU_JD_DEV_TYPE g_devtype = JD_DEV_LIGHT;
static PJD_SCENE_INFO_LIST g_SceneList = NULL;

static uint8 g_degree_flag = 0;

/*************************************************
Function:		save_param
Description: 	保存
Input:
1.hDlg
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static int save_param()
{
	if (strlen(YuyueTmp.Name))
	{
		YuyueTmp.Used = TRUE; //3066预约都是启用的

		if (g_YuyueListIndex)
		{
			storage_del_yuyue(g_YuyueListIndex - 1);
		}

		// 将预约保存到预约列表中,启动预约
		if (ECHO_STORAGE_OK == storage_add_yuyue(&YuyueTmp))
		{
			storage_yuyue_init();
			return TRUE;
		}
	}
	else
	{
		ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_ERROR, SID_Msg_JD_Yuyue_UnDevice, "SetJdYuYueParamLayer");
		return FALSE;
	}

	ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_ERROR, SID_Msg_Save_Err, "SetJdYuYueParamLayer");
	return FALSE;
}


/*************************************************
Function:		update_listview
Description: 	显示的具体信息
Input:		无
Output:		无
Return:		TRUE 成功 FALSE 失败
Others:
*************************************************/
static void update_listview(void)
{
	char nums[10] = { 0 };
	uint32 typeId[6] = { SID_Jd_Mode_Self, SID_Jd_Light_Control, SID_Jd_Kongtiao_Control,
		SID_Jd_Chuanlian_Control, SID_Jd_Dianyuan_Control, SID_Jd_Meiqi_Control };

	// 预约类型
	ituTextSetString(SetJdYuYueParamYuyueType2Text, get_str(typeId[YuyueTmp.Type]));

	// 从存储中取已启用的设备的名称
	if (strlen(YuyueTmp.Name))
	{
		ituTextSetString(SetJdYuYueParamDevice2Text, YuyueTmp.Name);
		ituSetColor(&((ITUWidget*)SetJdYuYueParamDevice1Text)->color, 255, 255, 255, 255);
		ituSetColor(&((ITUWidget*)SetJdYuYueParamDevice2Text)->color, 255, 255, 255, 255);
	}
	else
	{
		ituTextSetString(SetJdYuYueParamDevice2Text, get_str(SID_Jd_Yuyue_Dev_None));
		ituSetColor(&((ITUWidget*)SetJdYuYueParamDevice1Text)->color, 255, 128, 138, 135);
		ituSetColor(&((ITUWidget*)SetJdYuYueParamDevice2Text)->color, 255, 192, 192, 192);
	}

	// 转态
	if (YuyueTmp.Type == JD_TYPE_QINGJING)
	{
		ituTextSetString(SetJdYuYueParamOpera2Text, get_str(SID_Jd_Yuyue_Action_Open));
		ituSetColor(&((ITUWidget*)SetJdYuYueParamOpera1Text)->color, 255, 128, 138, 135);
		ituSetColor(&((ITUWidget*)SetJdYuYueParamOpera2Text)->color, 255, 192, 192, 192);
	}
	else
	{
		if (YuyueTmp.Action)
		{
			ituTextSetString(SetJdYuYueParamOpera2Text, get_str(SID_Jd_Yuyue_Action_Open));
		}
		else
		{
			ituTextSetString(SetJdYuYueParamOpera2Text, get_str(SID_Jd_Yuyue_Action_Close));
		}
		ituSetColor(&((ITUWidget*)SetJdYuYueParamOpera1Text)->color, 255, 255, 255, 255);
		ituSetColor(&((ITUWidget*)SetJdYuYueParamOpera2Text)->color, 255, 255, 255, 255);
	}

	// 亮度调节/空调温度调节
	if (YuyueTmp.Type == JD_TYPE_DENGGUANG)
	{
#ifdef _AURINE_ELEC_NEW_
		if (JD_FACTORY_ACBUS == storage_get_extmode(EXT_MODE_JD_FACTORY) && YuyueTmp.IsTune == 0)
		{
			ituTextSetString(SetJdYuYueParamBrightness1Text, get_str(SID_Jd_Dev_Set_Attr));
			ituTextSetString(SetJdYuYueParamBrightness2Text, get_str(SID_Jd_Dev_Set_UnAdjust));// 不可调
		}
		else
#endif
		{
			memset(nums, 0, sizeof(nums));
			sprintf(nums, "%d", YuyueTmp.Degree);
			ituTextSetString(SetJdYuYueParamBrightness1Text, get_str(SID_Jd_Dev_Set_Light));
			ituTextSetString(SetJdYuYueParamBrightness2Text, nums);
		}
		ituWidgetSetVisible(SetJdYuYueParamBrightnessContainer, true);
	}
#ifndef _AURINE_ELEC_NEW_
	else if (YuyueTmp.Type == JD_TYPE_KONGTIAO)
	{
		ituTextSetString(SetJdYuYueParamBrightness1Text, get_str(SID_Jd_Dev_Set_Temp));// 温度
		memset(nums, 0, sizeof(nums));
		sprintf(nums, "%d", YuyueTmp.Degree + 20);
		ituTextSetString(SetJdYuYueParamBrightness2Text, nums);
		ituWidgetSetVisible(SetJdYuYueParamBrightnessContainer, true);
	}
#else
	else if (JD_FACTORY_ACBUS == storage_get_extmode(EXT_MODE_JD_FACTORY) && YuyueTmp.Type == JD_TYPE_KONGTIAO)
	{
		ituWidgetSetVisible(SetJdYuYueParamBrightnessContainer, false);
	}
#endif
	else
	{
		ituWidgetSetVisible(SetJdYuYueParamBrightnessContainer, false);
	}

	if (ituWidgetIsVisible(SetJdYuYueParamBrightnessBackground))
	{
		if (YuyueTmp.Action)
		{
			ituSetColor(&((ITUWidget*)SetJdYuYueParamBrightness1Text)->color, 255, 255, 255, 255);
			ituSetColor(&((ITUWidget*)SetJdYuYueParamBrightness2Text)->color, 255, 255, 255, 255);
		}
		else
		{
			ituSetColor(&((ITUWidget*)SetJdYuYueParamBrightness1Text)->color, 255, 128, 138, 135);
			ituSetColor(&((ITUWidget*)SetJdYuYueParamBrightness2Text)->color, 255, 192, 192, 192);
		}
	}

	// 重复方式(类型)
	ituTextSetString(SetJdYuYueParamTimeType2Text, YuyueRepStr[YuyueTmp.BeTime.BeType]);

	if (YuyueTmp.BeTime.BeType == BE_WEEK)							// 每周
	{
		ituSetColor(&((ITUWidget*)SetJdYuYueParamTimeDate1Text)->color, 255, 128, 138, 135);
		ituSetColor(&((ITUWidget*)SetJdYuYueParamTimeDate2Text)->color, 255, 192, 192, 192);
		ituSetColor(&((ITUWidget*)SetJdYuYueParamTimeWeek1Text)->color, 255, 255, 255, 255);
		ituSetColor(&((ITUWidget*)SetJdYuYueParamTimeWeek2Text)->color, 255, 255, 255, 255);
	}
	else if (YuyueTmp.BeTime.BeType == BE_MONTH)					// 每月
	{
		ituSetColor(&((ITUWidget*)SetJdYuYueParamTimeDate1Text)->color, 255, 255, 255, 255);
		ituSetColor(&((ITUWidget*)SetJdYuYueParamTimeDate2Text)->color, 255, 255, 255, 255);
		ituSetColor(&((ITUWidget*)SetJdYuYueParamTimeWeek1Text)->color, 255, 128, 138, 135);
		ituSetColor(&((ITUWidget*)SetJdYuYueParamTimeWeek2Text)->color, 255, 192, 192, 192);
	}
	else if (YuyueTmp.BeTime.BeType == BE_DAY)					// 每天
	{
		ituSetColor(&((ITUWidget*)SetJdYuYueParamTimeDate1Text)->color, 255, 128, 138, 135);
		ituSetColor(&((ITUWidget*)SetJdYuYueParamTimeDate2Text)->color, 255, 192, 192, 192);
		ituSetColor(&((ITUWidget*)SetJdYuYueParamTimeWeek1Text)->color, 255, 128, 138, 135);
		ituSetColor(&((ITUWidget*)SetJdYuYueParamTimeWeek2Text)->color, 255, 192, 192, 192);
	}
	
	ituTextSetString(SetJdYuYueParamTimeWeek2Text, YuyueWeekStr[YuyueTmp.BeTime.tm_wday + 1]);

	memset(nums, 0, sizeof(nums));
	sprintf(nums, "%02d", YuyueTmp.BeTime.tm_mday + 1);
	ituTextSetString(SetJdYuYueParamTimeDate2Text, nums);

	// 开始时间
	memset(nums, 0, sizeof(nums));
	sprintf(nums, "%02u:%02u", YuyueTmp.BeTime.tm_hour, YuyueTmp.BeTime.tm_min);
	ituTextSetString(SetJdYuYueParamTimeTime2Text, nums);

	ituCoverFlowUpdate((ITUWidget*)SetJdYuYueParamListCoverFlow, ITU_EVENT_LAYOUT, 0, 0, 0);
}

/*************************************************
Function:		SetJdYuYueOnEnter
Description: 	进入界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJdYuYueParamOnEnter(ITUWidget* widget, char* param)
{
	if (!SetJdYuYueParamListCoverFlow)
	{
		SetJdYuYueParamListCoverFlow = ituSceneFindWidget(&theScene, "SetJdYuYueParamListCoverFlow");
		assert(SetJdYuYueParamListCoverFlow);

		SetJdYuYueParamYuyueType2Text = ituSceneFindWidget(&theScene, "SetJdYuYueParamYuyueType2Text");
		assert(SetJdYuYueParamYuyueType2Text);

		SetJdYuYueParamDevice2Text = ituSceneFindWidget(&theScene, "SetJdYuYueParamDevice2Text");
		assert(SetJdYuYueParamDevice2Text);

		SetJdYuYueParamOpera2Text = ituSceneFindWidget(&theScene, "SetJdYuYueParamOpera2Text");
		assert(SetJdYuYueParamOpera2Text);

		SetJdYuYueParamBrightness2Text = ituSceneFindWidget(&theScene, "SetJdYuYueParamBrightness2Text");
		assert(SetJdYuYueParamBrightness2Text);

		SetJdYuYueParamTimeType2Text = ituSceneFindWidget(&theScene, "SetJdYuYueParamTimeType2Text");
		assert(SetJdYuYueParamTimeType2Text);

		SetJdYuYueParamTimeDate2Text = ituSceneFindWidget(&theScene, "SetJdYuYueParamTimeDate2Text");
		assert(SetJdYuYueParamTimeDate2Text);

		SetJdYuYueParamTimeTime2Text = ituSceneFindWidget(&theScene, "SetJdYuYueParamTimeTime2Text");
		assert(SetJdYuYueParamTimeTime2Text);

		SetJdYuYueParamTimeWeek2Text = ituSceneFindWidget(&theScene, "SetJdYuYueParamTimeWeek2Text");
		assert(SetJdYuYueParamTimeWeek2Text);

		SetJdYuYueParamTimeWeek2Text = ituSceneFindWidget(&theScene, "SetJdYuYueParamTimeWeek2Text");
		assert(SetJdYuYueParamTimeWeek2Text);

		SetJdYuYueParamBrightnessContainer = ituSceneFindWidget(&theScene, "SetJdYuYueParamBrightnessContainer");
		assert(SetJdYuYueParamBrightnessContainer);

		SetJdYuYueParamBrightness1Text = ituSceneFindWidget(&theScene, "SetJdYuYueParamBrightness1Text");
		assert(SetJdYuYueParamBrightness1Text);

		SetJdYuYueParamTimeDate1Text = ituSceneFindWidget(&theScene, "SetJdYuYueParamTimeDate1Text");
		assert(SetJdYuYueParamTimeDate1Text);

		SetJdYuYueParamTimeWeek1Text = ituSceneFindWidget(&theScene, "SetJdYuYueParamTimeWeek1Text");
		assert(SetJdYuYueParamTimeWeek1Text);

		SetJdYuYueParamDevice1Text = ituSceneFindWidget(&theScene, "SetJdYuYueParamDevice1Text");
		assert(SetJdYuYueParamDevice1Text);

		SetJdYuYueParamOpera1Text = ituSceneFindWidget(&theScene, "SetJdYuYueParamOpera1Text");
		assert(SetJdYuYueParamOpera1Text);

		SetJdYuYueParamBackground = ituSceneFindWidget(&theScene, "SetJdYuYueParamBackground");
		assert(SetJdYuYueParamBackground);

		SetJdYuyueTypeBackground = ituSceneFindWidget(&theScene, "SetJdYuyueTypeBackground");
		assert(SetJdYuyueTypeBackground);

		SetJdYuyueDeviceBackground = ituSceneFindWidget(&theScene, "SetJdYuyueDeviceBackground");
		assert(SetJdYuyueDeviceBackground);

		SetJdYuYueParamBrightnessBackground = ituSceneFindWidget(&theScene, "SetJdYuYueParamBrightnessBackground");
		assert(SetJdYuYueParamBrightnessBackground);

		SetJdYuYueParamTimeTypeBackground = ituSceneFindWidget(&theScene, "SetJdYuYueParamTimeTypeBackground");
		assert(SetJdYuYueParamTimeTypeBackground);

		SetJdYuYueParamTimeDateBackground = ituSceneFindWidget(&theScene, "SetJdYuYueParamTimeDateBackground");
		assert(SetJdYuYueParamTimeDateBackground);

		SetJdYuYueParamTimeTimeBackground = ituSceneFindWidget(&theScene, "SetJdYuYueParamTimeTimeBackground");
		assert(SetJdYuYueParamTimeTimeBackground);

		SetJdYuYueParamTimeWeekBackground = ituSceneFindWidget(&theScene, "SetJdYuYueParamTimeWeekBackground");
		assert(SetJdYuYueParamTimeWeekBackground);
	}
	if (strcmp(param, "SetJdYuYueLayer") == 0)
	{
		ituCoverFlowGoto(SetJdYuYueParamListCoverFlow, 0);
		update_listview();

		ituWidgetSetVisible(SetJdYuyueTypeBackground, false);
		ituWidgetSetVisible(SetJdYuyueDeviceBackground, false);
		ituWidgetSetVisible(SetJdYuYueParamBrightnessBackground, false);
		ituWidgetSetVisible(SetJdYuYueParamTimeTypeBackground, false);
		ituWidgetSetVisible(SetJdYuYueParamTimeDateBackground, false);
		ituWidgetSetVisible(SetJdYuYueParamTimeTimeBackground, false); 
		ituWidgetSetVisible(SetJdYuYueParamTimeWeekBackground, false);
		ituWidgetSetVisible(SetJdYuYueParamBackground, true);
		if (!ituWidgetIsEnabled(SetJdYuYueParamBackground))
		{
			ituWidgetEnable(SetJdYuYueParamBackground);
		}
	}
	else if (strcmp(param, "MsgFailHintSuccessLayer") == 0)
	{
		if (!MsgFailHintSuccess1RadioBox)
		{
			MsgFailHintSuccess1RadioBox = ituSceneFindWidget(&theScene, "MsgFailHintSuccess1RadioBox");
			assert(MsgFailHintSuccess1RadioBox);
		}

		if (ituRadioBoxIsChecked(MsgFailHintSuccess1RadioBox))	//确认键
		{
			if (FALSE == save_param())
			{
				return true;
			}
		}
		if (!SetJdYuYueLayer)
		{
			SetJdYuYueLayer = ituSceneFindWidget(&theScene, "SetJdYuYueLayer");
			assert(SetJdYuYueLayer);
		}
		ituLayerGoto(SetJdYuYueLayer);
	}

	return true;
}

/*************************************************
Function:		SetJdYuYueParamYuyueTypeButtonOnMouseUp
Description: 	预约类型按下
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJdYuYueParamYuyueTypeButtonOnMouseUp(ITUWidget* widget, char* param)
{
	if (!SetJdYuyueTypeRadioBox[0])
	{
		char tmp[50];
		uint8 i = 0;

		for (i = 0; i < 6; i++)
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d%s", "SetJdYuyueType", i, "RadioBox");
			SetJdYuyueTypeRadioBox[i] = ituSceneFindWidget(&theScene, tmp);
			assert(SetJdYuyueTypeRadioBox[i]);
		}
	}

	ituRadioBoxSetChecked(SetJdYuyueTypeRadioBox[YuyueTmp.Type], true);

	ituWidgetDisable(SetJdYuYueParamBackground);
	ituWidgetSetVisible(SetJdYuyueTypeBackground, true);
	
	return true;
}

/*************************************************
Function:		updata_data
Description:
Input:
1.hDlg
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void updata_data()
{
	char text_tmp1[50] = { 0 };
	uint8 len = 50;
	uint32 ScenID[JD_SCENE_MAX] = { SID_Jd_Mode_Huike, SID_Jd_Mode_Jiucan,
		SID_Jd_Mode_Yejian, SID_Jd_Mode_Jiedian, SID_Jd_Mode_Putong };

	if (YuyueTmp.Type == JD_TYPE_QINGJING)
	{
#ifdef _AURINE_ELEC_NEW_
		if (JD_FACTORY_ACBUS == storage_get_extmode(EXT_MODE_JD_FACTORY))
		{
			YuyueTmp.Index = g_SceneList->pjd_scene_info[g_index].SceneID;
			YuyueTmp.Address = g_SceneList->pjd_scene_info[g_index].Address;
		}
		else
		{
			YuyueTmp.Index = g_SceneList->pjd_scene_info[g_index].SceneIndex;
		}
#else
		YuyueTmp.Index = g_SceneList->pjd_scene_info[g_index].SceneIndex;
#endif
		YuyueTmp.BeType = EV_BE_SM;
	}
	else
	{
		g_devtype = YuyueTmp.Type - 1;
		free_jd_memory(&g_devlist);
		g_devlist = storage_get_jddev(g_devtype, TRUE);
		YuyueTmp.Index = g_devlist->pjd_dev_info[g_index].Index;
		YuyueTmp.Address = g_devlist->pjd_dev_info[g_index].Address;
		YuyueTmp.BeType = EV_BE_JD;

		if (YuyueTmp.Type == JD_TYPE_DENGGUANG)
		{
			YuyueTmp.IsTune = g_devlist->pjd_dev_info[g_index].IsTune;
		}
		else
		{
#ifdef _AURINE_ELEC_NEW_
			if (JD_FACTORY_ACBUS == storage_get_extmode(EXT_MODE_JD_FACTORY))
			{
				if (YuyueTmp.Type == JD_TYPE_KONGTIAO)
				{
					YuyueTmp.IsTune = 1;
				}
			}
			else
			{
				if (YuyueTmp.Type == JD_TYPE_KONGTIAO)
				{
					YuyueTmp.IsTune = g_devlist->pjd_dev_info[g_index].IsTune;
				}
			}
#else
			if (YuyueTmp.Type == JD_TYPE_KONGTIAO)
			{
				YuyueTmp.IsTune = g_devlist->pjd_dev_info[g_index].IsTune;
			}
#endif
		}
	}

#ifdef _USERDEFINE_JDNAME_							// 使用用户自定义英文名
	if (YuyueTmp.Type == JD_TYPE_QINGJING)
	{
#ifdef _AURINE_ELEC_NEW_
		if (JD_FACTORY_ACBUS == storage_get_extmode(EXT_MODE_JD_FACTORY))
		{
			if (g_index < 5)
			{
				memcpy(text_tmp1, get_str(ScenID[g_SceneList->pjd_scene_info[g_index].SceneIndex]), len);
			}
			else
			{
				strcpy(text_tmp1, g_SceneList->pjd_scene_info[g_index].SceneName);
			}
		}
		else
		{
			strcpy(text_tmp1, g_SceneList->pjd_scene_info[g_index].SceneName);
		}
#else
		strcpy(text_tmp1, g_SceneList->pjd_scene_info[g_index].SceneName);
#endif

	}
	else
	{
		strcpy(text_tmp1, g_devlist->pjd_dev_info[g_index].Name);
	}

	memset(YuyueTmp.Name, 0, sizeof(YuyueTmp.Name));
	strcpy(YuyueTmp.Name, text_tmp1);

#else

	memset(YuyueTmp.Name, 0, sizeof(YuyueTmp.Name));
	memset(text_tmp1, 0, sizeof(text_tmp1));
	if (YuyueTmp.Type == JD_TYPE_QINGJING)
	{
#ifdef _AURINE_ELEC_NEW_
		if (JD_FACTORY_ACBUS == storage_get_extmode(EXT_MODE_JD_FACTORY))
		{
#ifdef _IP_MODULE_JD_
			if (get_ipmodule())
			{
				if (g_index < 5)
				{
					memcpy(text_tmp1, get_str(ScenID[g_SceneList->pjd_scene_info[g_index].SceneIndex]), len);
				}
				else
				{
					strcpy(text_tmp1, g_SceneList->pjd_scene_info[g_index].SceneName);
				}

				strcat(YuyueTmp.Name, text_tmp1);
			}
			else
#endif
			{
				if (g_index < 5)
				{
					strcat(YuyueTmp.Name, get_str(ScenID[g_SceneList->pjd_scene_info[g_index].SceneIndex]));
				}
				else
				{
					strcat(YuyueTmp.Name, get_str(g_SceneList->pjd_scene_info[g_index].TextIDPos));
					strcat(YuyueTmp.Name, get_str(g_SceneList->pjd_scene_info[g_index].TextIDName));
				}
			}
		}
		else
		{
			strcat(YuyueTmp.Name, get_str(ScenID[g_SceneList->pjd_scene_info[g_index].SceneIndex]));
		}
#else
		strcat(YuyueTmp.Name, get_str(ScenID[g_SceneList->pjd_scene_info[g_index].SceneIndex]));
#endif
	}
	else
	{
#ifdef _IP_MODULE_JD_
		if (get_ipmodule())
		{
			strcat(YuyueTmp.Name, g_devlist->pjd_dev_info[g_index].Name);
		}
		else
#endif	
		if ((0 != g_devlist->pjd_dev_info[g_index].TextIDPos) || (0 != g_devlist->pjd_dev_info[g_index].TextIDName))
		{
			strcpy(YuyueTmp.Name, get_str(g_devlist->pjd_dev_info[g_index].TextIDPos));
			strcat(YuyueTmp.Name, get_str(g_devlist->pjd_dev_info[g_index].TextIDName));
		}
	}
#endif			
}

/*************************************************
Function:		SetJdYuyueTypeRadioBoxOnMouseUp
Description: 	预约类型列表按下
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJdYuyueTypeRadioBoxOnMouseUp(ITUWidget* widget, char* param)
{
	int index = atoi(param);
	YuyueTmp.Type = (uint8)index;
	g_index = 0;
	updata_data();

	update_listview();
	
	ituWidgetSetVisible(SetJdYuyueTypeBackground, false);
	ituWidgetEnable(SetJdYuYueParamBackground);

	return true;
}

/*************************************************
Function:		clone_one_device_name_list
Description: 	clone一个设备名称的列表
Input:		无
Output:		无
Return:		TRUE 成功 FALSE 失败
Others:
*************************************************/
static void clone_one_device_name_list(uint8 num)
{
	char tmp[100];
	bool result = FALSE;
	ITUContainer* CloneChildContainer = NULL;
	ITUBackground* LineBackground = NULL;
	ITUIcon* SpriteIcon = NULL;

	result = ituWidgetClone(SetJdYuyueDeviceListContainer0, &CloneChildContainer);
	if (result)
	{
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "SetJdYuyueDeviceListContainer", num);
		ituWidgetSetName(CloneChildContainer, tmp);
		ituWidgetSetX(CloneChildContainer, 0);
		ituWidgetSetY(CloneChildContainer, 0);

		SetJdYuyueDeviceButton[num] = itcTreeGetChildAt(CloneChildContainer, 0);
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "SetJdYuyueDeviceButton", num);
		ituWidgetSetName(SetJdYuyueDeviceButton[num], tmp);

		SetJdYuyueDeviceText = itcTreeGetChildAt(CloneChildContainer, 1);
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "SetJdYuyueDeviceText", num);
		ituWidgetSetName(SetJdYuyueDeviceText, tmp);

		SetJdYuyueDeviceSprite[num] = itcTreeGetChildAt(CloneChildContainer, 2);
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "SetJdYuyueDeviceSprite", num);
		ituWidgetSetName(SetJdYuyueDeviceSprite[num], tmp);

		LineBackground = itcTreeGetChildAt(CloneChildContainer, 3);
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "SetJdYuyueDeviceLineBackground", num);
		ituWidgetSetName(LineBackground, tmp);

		SpriteIcon = itcTreeGetChildAt(SetJdYuyueDeviceSprite[num], 0);
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "SetJdYuyueDeviceSpriteNIcon", num);
		ituWidgetSetName(SpriteIcon, tmp);

		SpriteIcon = itcTreeGetChildAt(SetJdYuyueDeviceSprite[num], 1);
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "SetJdYuyueDeviceSpriteYIcon", num);
		ituWidgetSetName(SpriteIcon, tmp);

		ituSpriteGoto(SetJdYuyueDeviceSprite[num], 0);

		ituWidgetAdd(SetJdYuyueDeviceListCoverFlow, CloneChildContainer);
		ituCoverFlowUpdate((ITUWidget*)SetJdYuyueDeviceListCoverFlow, ITU_EVENT_LAYOUT, 0, 0, 0);
	}
}

/*************************************************
Function:		show_name
Description: 	显示设备名称信息
Input:		无
Output:		无
Return:		TRUE 成功 FALSE 失败
Others:
*************************************************/
static void show_device_name(void)
{
	uint8 i;
	int32 max = 0;
	char tmp[150] = { 0 };
	ITUContainer* ListContainerFlag = NULL;
	char TextData[100] = { 0 };

	uint32 ScenID[JD_SCENE_MAX] = { SID_Jd_Mode_Huike, SID_Jd_Mode_Jiucan,
		SID_Jd_Mode_Yejian, SID_Jd_Mode_Jiedian, SID_Jd_Mode_Putong };

	// 显示设备名称
	if (YuyueTmp.Type == JD_TYPE_QINGJING)
	{
		max = g_SceneList->nCount;
	}
	else
	{
		g_devtype = YuyueTmp.Type - 1;
		free_jd_memory(&g_devlist);
		g_devlist = storage_get_jddev(g_devtype, TRUE);
		max = g_devlist->nCount;
	}

	for (i = 1; i < MAX_CLONE_NUM; i++)
	{
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "SetJdYuyueDeviceListContainer", i);
		ListContainerFlag = (ITUContainer*)ituSceneFindWidget(&theScene, tmp);
		if (NULL != ListContainerFlag)							//判断是否clone过
		{
			itcTreeRemove(ListContainerFlag);						// 删除已有的
			ituWidgetExit(ListContainerFlag);
			ListContainerFlag = NULL;
			SetJdYuyueDeviceButton[i] = NULL;
			SetJdYuyueDeviceSprite[i] = NULL;
			debug_log("Remove a clone list background!!!\n");
		}
	}

	for (i = 0; i < max; i++)
	{
		if (0 == i)
		{
			SetJdYuyueDeviceButton[0] = ituSceneFindWidget(&theScene, "SetJdYuyueDeviceButton0");
			assert(SetJdYuyueDeviceButton[0]);

			SetJdYuyueDeviceText = ituSceneFindWidget(&theScene, "SetJdYuyueDeviceText0");
			assert(SetJdYuyueDeviceText);

			SetJdYuyueDeviceSprite[0] = ituSceneFindWidget(&theScene, "SetJdYuyueDeviceSprite0");
			assert(SetJdYuyueDeviceSprite[0]);

			ituSpriteGoto(SetJdYuyueDeviceSprite[0], 0);
		}
		else
		{
			clone_one_device_name_list(i);
		}

		if (g_index == i)
		{
			ituSpriteGoto(SetJdYuyueDeviceSprite[i], 1);
		}

		memset(TextData, 0, sizeof(TextData));
#ifdef _AURINE_ELEC_NEW_
		if (YuyueTmp.Type == JD_TYPE_QINGJING)
		{
			if (JD_FACTORY_ACBUS == storage_get_extmode(EXT_MODE_JD_FACTORY))
			{
#ifdef _IP_MODULE_JD_
				if (get_ipmodule())
				{
					if (i < 5)
					{
						strcpy(TextData, get_str(ScenID[g_SceneList->pjd_scene_info[i].SceneIndex]));
					}
					else
					{
						strcpy(TextData, g_SceneList->pjd_scene_info[i].SceneName);
					}
				}
				else
#endif
				{
					if (i < 5)
					{
						strcat(TextData, get_str(ScenID[g_SceneList->pjd_scene_info[i].SceneIndex]));
					}
					else
					{
						strcat(TextData, get_str(g_SceneList->pjd_scene_info[i].TextIDPos));
						strcat(TextData, get_str(g_SceneList->pjd_scene_info[i].TextIDName));
					}
				}
			}
			else
			{
				strcat(TextData, get_str(ScenID[g_SceneList->pjd_scene_info[i].SceneIndex]));
			}
		}
		else
		{
#ifdef _IP_MODULE_JD_
			if (get_ipmodule())
			{
				strcat(TextData, g_devlist->pjd_dev_info[i].Name);
			}
			else
#endif
			{
				strcat(TextData, get_str(g_devlist->pjd_dev_info[i].TextIDPos));
				strcat(TextData, get_str(g_devlist->pjd_dev_info[i].TextIDName));
			}
		}
#else
		if (YuyueTmp.Type == JD_TYPE_QINGJING)
		{
			strcat(TextData, get_str(ScenID[g_SceneList->pjd_scene_info[i].SceneIndex]));
		}
		else
		{
			strcat(TextData, get_str(g_devlist->pjd_dev_info[i].TextIDPos));
			strcat(TextData, get_str(g_devlist->pjd_dev_info[i].TextIDName));
		}
#endif
		ituTextSetString(SetJdYuyueDeviceText, TextData);
	}
}

/*************************************************
Function:		SetJdYuYueParamDeviceButtonOnMouseUp
Description: 	设备名称按下
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJdYuYueParamDeviceButtonOnMouseUp(ITUWidget* widget, char* param)
{
	if (!SetJdYuyueDeviceListContainer0)
	{
		SetJdYuyueDeviceListContainer0 = ituSceneFindWidget(&theScene, "SetJdYuyueDeviceListContainer0");
		assert(SetJdYuyueDeviceListContainer0);

		SetJdYuyueDeviceListCoverFlow = ituSceneFindWidget(&theScene, "SetJdYuyueDeviceListCoverFlow");
		assert(SetJdYuyueDeviceListCoverFlow);
	}

	if (strlen(YuyueTmp.Name))
	{
		show_device_name();

		ituCoverFlowGoto(SetJdYuyueDeviceListCoverFlow, 0);
		ituWidgetDisable(SetJdYuYueParamBackground);
		ituWidgetSetVisible(SetJdYuyueDeviceBackground, true);
	}

	return true;
}

/*************************************************
Function:		SetJdYuyueDeviceListButtonOnMouseUp
Description: 	设备名称列表按下
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJdYuyueDeviceListButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 i;
	int32 max = 0;
	if (YuyueTmp.Type == JD_TYPE_QINGJING)
	{
		max = g_SceneList->nCount;
	}
	else
	{
		max = g_devlist->nCount;
	}

	for (i = 0; i < max; i++)
	{
		if (SetJdYuyueDeviceButton[i] == (ITUButton*)widget)
		{
			g_index = i;

			ituSpriteGoto(SetJdYuyueDeviceSprite[i], 1);
			updata_data();
			update_listview();

			ituWidgetEnable(SetJdYuYueParamBackground);
			ituWidgetSetVisible(SetJdYuyueDeviceBackground, false);

			return true;
		}
	}

	return true;
}

/*************************************************
Function:		SetJdYuYueParamOperaButton
Description: 	设备名称列表按下
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJdYuYueParamOperaButtonOnMouseUp(ITUWidget* widget, char* param)
{
	// modi by luofl 2011-07-19 情景模式没有关动作
	if (YuyueTmp.Type == JD_TYPE_QINGJING)
	{
		YuyueTmp.Action = TRUE;
	}
	else
	{
		if (YuyueTmp.Action)
		{
			YuyueTmp.Action = FALSE;
		}
		else
		{
			YuyueTmp.Action = TRUE;
		}
	}
	update_listview();

	return true;
}

/*************************************************
Function:		ShowSetJdYuYueParamBrightnessBackground
Description: 	显示设置温度和亮度值界面
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void ShowSetJdYuYueParamBrightnessBackground()
{
	if (!SetJdYuYueParamBrightnessProgressBar)
	{
		SetJdYuYueParamBrightnessProgressBar = ituSceneFindWidget(&theScene, "SetJdYuYueParamBrightnessProgressBar");
		assert(SetJdYuYueParamBrightnessProgressBar);

		SetJdYuYueParamBrightnessTrackBar = ituSceneFindWidget(&theScene, "SetJdYuYueParamBrightnessTrackBar");
		assert(SetJdYuYueParamBrightnessTrackBar);

		SetJdYuYueParamBrightnessTitleText = ituSceneFindWidget(&theScene, "SetJdYuYueParamBrightnessTitleText");
		assert(SetJdYuYueParamBrightnessTitleText);
	}

	if (YuyueTmp.Type == JD_TYPE_DENGGUANG)
	{
		g_degree_flag = YuyueTmp.Degree;
		SetJdYuYueParamBrightnessProgressBar->min = 0;
		SetJdYuYueParamBrightnessProgressBar->max = 10;
		SetJdYuYueParamBrightnessTrackBar->min = 0;
		SetJdYuYueParamBrightnessTrackBar->max = 10;
		ituProgressBarSetValue(SetJdYuYueParamBrightnessProgressBar, YuyueTmp.Degree);
		ituTrackBarSetValue(SetJdYuYueParamBrightnessTrackBar, YuyueTmp.Degree);
		ituTextSetString(SetJdYuYueParamBrightnessTitleText, get_str(SID_Jd_Dev_Set_Light));
	}
	else if (YuyueTmp.Type == JD_TYPE_KONGTIAO)
	{
		g_degree_flag = YuyueTmp.Degree + 20;
		SetJdYuYueParamBrightnessProgressBar->min = 20;
		SetJdYuYueParamBrightnessProgressBar->max = 30;
		SetJdYuYueParamBrightnessTrackBar->min = 20;
		SetJdYuYueParamBrightnessTrackBar->max = 30;
		ituProgressBarSetValue(SetJdYuYueParamBrightnessProgressBar, YuyueTmp.Degree + 20);
		ituTrackBarSetValue(SetJdYuYueParamBrightnessTrackBar, YuyueTmp.Degree + 20);
		ituTextSetString(SetJdYuYueParamBrightnessTitleText, get_str(SID_Jd_Dev_Set_Temp));
	}

	ituWidgetDisable(SetJdYuYueParamBackground);
	ituWidgetSetVisible(SetJdYuYueParamBrightnessBackground, true);
}

/*************************************************
Function:		SetJdYuYueParamBrightnessButtonOnMouseUp
Description: 	预约灯光亮度设置和空调温度设置
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJdYuYueParamBrightnessButtonOnMouseUp(ITUWidget* widget, char* param)
{
	if (YuyueTmp.Type == JD_TYPE_DENGGUANG)
	{
		if (YuyueTmp.IsTune == 1 && YuyueTmp.Action)
		{
			// 显示亮度调节页面
			ShowSetJdYuYueParamBrightnessBackground();
		}
	}
#ifndef _AURINE_ELEC_NEW_
	else if (YuyueTmp.Type == JD_TYPE_KONGTIAO)         // 取消空调温度预约
	{
		if (YuyueTmp.IsTune == 1 && YuyueTmp.Action)
		{
			// 显示空调温度调节页面
			ShowSetJdYuYueParamBrightnessBackground();
		}
	}
#else 
	else if (YuyueTmp.Type == JD_TYPE_KONGTIAO && JD_FACTORY_ACBUS == storage_get_extmode(EXT_MODE_JD_FACTORY))
	{
		return true;
	}
#endif
	else
	{
		return true;
	}

	return true;
}

/*************************************************
Function:		SetJdYuYueParamBrightnessProgressBarOnChanged
Description: 	灯光亮度调节函数,空调温度调节
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJdYuYueParamBrightnessProgressBarOnChanged(ITUWidget* widget, char* param)
{
	g_degree_flag = atoi(param);

	return true;
}

/*************************************************
Function:		SetJdYuYueParamBrightnessProgressBarOnChanged
Description: 	灯光亮度调节函数,空调温度调节
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJdYuYueParamBrightnessTureButtonOnMouseUp(ITUWidget* widget, char* param)
{
	if (YuyueTmp.Type == JD_TYPE_DENGGUANG)
	{
		YuyueTmp.Degree = g_degree_flag;
	}
	else if (YuyueTmp.Type == JD_TYPE_KONGTIAO)
	{
		YuyueTmp.Degree = g_degree_flag - 20;
	}
	
	update_listview();

	ituWidgetEnable(SetJdYuYueParamBackground);
	ituWidgetSetVisible(SetJdYuYueParamBrightnessBackground, false);

	return true;
}

/*************************************************
Function:		SetJdYuYueParamTimeTypeButtonOnMouseUp
Description: 	预约时间类型按下
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJdYuYueParamTimeTypeButtonOnMouseUp(ITUWidget* widget, char* param)
{
	if (!SetJdYuYueParamTimeTypeListRadioBox[0])
	{
		char tmp[50];
		uint8 i = 0;

		for (i = 0; i < 3; i++)
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d%s", "SetJdYuYueParamTimeTypeList", i, "RadioBox");
			SetJdYuYueParamTimeTypeListRadioBox[i] = ituSceneFindWidget(&theScene, tmp);
			assert(SetJdYuYueParamTimeTypeListRadioBox[i]);
		}
	}

	if (YuyueTmp.BeTime.BeType <= BE_MONTH)
	{
		ituRadioBoxSetChecked(SetJdYuYueParamTimeTypeListRadioBox[YuyueTmp.BeTime.BeType], true);
	}
	
	ituWidgetDisable(SetJdYuYueParamBackground);
	ituWidgetSetVisible(SetJdYuYueParamTimeTypeBackground, true);

	return true;
}

/*************************************************
Function:		SetJdYuYueParamTimeTypeListRadioBoxOnMouseUp
Description: 	预约时间类型列表按下
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJdYuYueParamTimeTypeListRadioBoxOnMouseUp(ITUWidget* widget, char* param)
{
	int index = atoi(param);
	switch (index)
	{
	case 0:
		YuyueTmp.BeTime.BeType = BE_DAY;
		break;

	case 1:
		YuyueTmp.BeTime.BeType = BE_WEEK;
		break;

	case 2:
		YuyueTmp.BeTime.BeType = BE_MONTH;
		break;
	}

	YuyueTmp.BeTime.tm_mday = 0;
	YuyueTmp.BeTime.tm_wday = 0;

	update_listview();

	ituWidgetEnable(SetJdYuYueParamBackground);
	ituWidgetSetVisible(SetJdYuYueParamTimeTypeBackground, false);

	return true;
}

/*************************************************
Function:		SetJdYuYueParamTimeDateButtonOnMouseUp
Description: 	预约时间日期按下
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJdYuYueParamTimeDateButtonOnMouseUp(ITUWidget* widget, char* param)
{
	if (!SetJdYuYueParamTimeDateWheel)
	{
		SetJdYuYueParamTimeDateWheel = ituSceneFindWidget(&theScene, "SetJdYuYueParamTimeDateWheel");
		assert(SetJdYuYueParamTimeDateWheel);
	}

	if (YuyueTmp.BeTime.BeType == BE_MONTH)
	{
		ituWheelGoto(SetJdYuYueParamTimeDateWheel, YuyueTmp.BeTime.tm_mday);

		ituWidgetDisable(SetJdYuYueParamBackground);
		ituWidgetSetVisible(SetJdYuYueParamTimeDateBackground, true);
	}

	return true;
}

/*************************************************
Function:		SetJdYuYueParamTimeDateOKButtonOnMouseUp
Description: 	预约时间日期确认键按下
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJdYuYueParamTimeDateOKButtonOnMouseUp(ITUWidget* widget, char* param)
{
	YuyueTmp.BeTime.tm_mday = SetJdYuYueParamTimeDateWheel->focusIndex;

	update_listview();

	ituWidgetEnable(SetJdYuYueParamBackground);
	ituWidgetSetVisible(SetJdYuYueParamTimeDateBackground, false);

	return true;
}

/*************************************************
Function:		SetJdYuYueParamTimeTimeButtonOnMouseUp
Description: 	预约时间时间设置按下
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJdYuYueParamTimeTimeButtonOnMouseUp(ITUWidget* widget, char* param)
{
	if (!SetJdYuYueParamTimeTimeHourWheel)
	{
		SetJdYuYueParamTimeTimeHourWheel = ituSceneFindWidget(&theScene, "SetJdYuYueParamTimeTimeHourWheel");
		assert(SetJdYuYueParamTimeTimeHourWheel);

		SetJdYuYueParamTimeTimeMinWheel = ituSceneFindWidget(&theScene, "SetJdYuYueParamTimeTimeMinWheel");
		assert(SetJdYuYueParamTimeTimeMinWheel);
	}

	ituWheelGoto(SetJdYuYueParamTimeTimeHourWheel, YuyueTmp.BeTime.tm_hour);
	ituWheelGoto(SetJdYuYueParamTimeTimeMinWheel, YuyueTmp.BeTime.tm_min);

	ituWidgetDisable(SetJdYuYueParamBackground);
	ituWidgetSetVisible(SetJdYuYueParamTimeTimeBackground, true);

	return true;
}

/*************************************************
Function:		SetJdYuYueParamTimeTimeOKButtonOnMouseUp
Description: 	预约时间时间设置确认按下
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJdYuYueParamTimeTimeOKButtonOnMouseUp(ITUWidget* widget, char* param)
{
	YuyueTmp.BeTime.tm_hour = SetJdYuYueParamTimeTimeHourWheel->focusIndex;
	YuyueTmp.BeTime.tm_min = SetJdYuYueParamTimeTimeMinWheel->focusIndex;

	update_listview();

	ituWidgetEnable(SetJdYuYueParamBackground);
	ituWidgetSetVisible(SetJdYuYueParamTimeTimeBackground, false);

	return true;
}

/*************************************************
Function:		SetJdYuYueParamTimeWeekButtonOnMouseUp
Description: 	预约时间星期设置按下
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJdYuYueParamTimeWeekButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 i = 0;

	if (!SetJdYuYueParamTimeWeekListSprite[0])
	{
		char tmp[50];

		for (i = 0; i < 7; i++)
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "SetJdYuYueParamTimeWeekListSprite", i);
			SetJdYuYueParamTimeWeekListSprite[i] = ituSceneFindWidget(&theScene, tmp);
			assert(SetJdYuYueParamTimeWeekListSprite[i]);
		}
	}

	if (YuyueTmp.BeTime.BeType == BE_WEEK)
	{
		for (i = 0; i < 7; i++)
		{
			if (i == YuyueTmp.BeTime.tm_wday + 1)
			{
				ituSpriteGoto(SetJdYuYueParamTimeWeekListSprite[i], 1);
			}
			else
			{
				ituSpriteGoto(SetJdYuYueParamTimeWeekListSprite[i], 0);
			}
		}

		ituWidgetDisable(SetJdYuYueParamBackground);
		ituWidgetSetVisible(SetJdYuYueParamTimeWeekBackground, true);
	}

	return true;
}

/*************************************************
Function:		SetJdYuYueParamTimeWeekListButtonOnMouseUp
Description: 	预约时间星期列表按下
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJdYuYueParamTimeWeekListButtonOnMouseUp(ITUWidget* widget, char* param)
{
	int index = atoi(param);
	YuyueTmp.BeTime.tm_wday = index - 1;
	update_listview();
	ituWidgetEnable(SetJdYuYueParamBackground);
	ituWidgetSetVisible(SetJdYuYueParamTimeWeekBackground, false);

	return true;
}

/*************************************************
Function:		init_param
Description: 	初始化页面所需的参数
Input:		无
Output:		无
Return:
Others:
*************************************************/
static void init_param(void)
{
	char text_tmp1[50] = { 0 };
	uint8 len = 50;
	uint32 ScenID[JD_SCENE_MAX] = { SID_Jd_Mode_Huike, SID_Jd_Mode_Jiucan, SID_Jd_Mode_Yejian, SID_Jd_Mode_Jiedian, SID_Jd_Mode_Putong };

	free_scene_memory(&g_SceneList);
#ifdef _AURINE_ELEC_NEW_
	if (JD_FACTORY_ACBUS == storage_get_extmode(EXT_MODE_JD_FACTORY))
	{
		g_SceneList = storage_get_aurine_scene_info(1);
	}
	else
	{
		g_SceneList = storage_get_scene_info();
	}
#else
	g_SceneList = storage_get_scene_info();
#endif

	dprintf("---- YuyueListIndex: %d\n", g_YuyueListIndex);
	if (g_YuyueListIndex)
	{
		memcpy(&YuyueTmp, &JdYuyueList->be_comm[g_YuyueListIndex -1], sizeof(BE_COMM));
		g_index = YuyueTmp.Index;
	}
	else
	{
		// 新增加的预约设备类型默认为情景模式1, 执行动作为开 modi by luofl 2011-07-19
		memset(&YuyueTmp, 0, sizeof(BE_COMM));
		YuyueTmp.Action = 1;
		YuyueTmp.Type = JD_TYPE_QINGJING;
#ifdef _AURINE_ELEC_NEW_
		if (JD_FACTORY_ACBUS == storage_get_extmode(EXT_MODE_JD_FACTORY))
		{
			YuyueTmp.Address = g_SceneList->pjd_scene_info[0].Address;
			YuyueTmp.Index = g_SceneList->pjd_scene_info[0].SceneID;
#ifdef _IP_MODULE_JD_
			if (get_ipmodule())
			{
				strcpy(text_tmp1, g_SceneList->pjd_scene_info[0].SceneName);
			}
			else
#endif
			{
				memcpy(text_tmp1, get_str(ScenID[g_SceneList->pjd_scene_info[0].SceneIndex]), len);
			}
		}
		else
		{
			YuyueTmp.Index = g_SceneList->pjd_scene_info[0].SceneIndex;
#ifdef _USERDEFINE_JDNAME_							// 使用用户自定义英文名
			strcpy(text_tmp1, g_SceneList->pjd_scene_info[0].SceneName);
#else
			memcpy(text_tmp1, get_str(ScenID[g_SceneList->pjd_scene_info[0].SceneIndex]), len);
#endif

		}
		YuyueTmp.BeType = EV_BE_SM;
#else
		YuyueTmp.BeType = EV_BE_SM;
		YuyueTmp.Index = g_SceneList->pjd_scene_info[0].SceneIndex;

#ifdef _IP_MODULE_JD_
		if (JD_FACTORY_ACBUS == storage_get_extmode(EXT_MODE_JD_FACTORY) && get_ipmodule())
		{
			strcpy(text_tmp1, g_SceneList->pjd_scene_info[0].SceneName);
		}
		else
#endif
		{
#ifdef _USERDEFINE_JDNAME_							// 使用用户自定义英文名
			strcpy(text_tmp1, g_SceneList->pjd_scene_info[0].SceneName);
#else
			mencpy(text_tmp1, get_str(ScenID[g_SceneList->pjd_scene_info[0].SceneIndex]), len);
#endif
		}
#endif		
		g_index = YuyueTmp.Index;
		strcat(YuyueTmp.Name, text_tmp1);
	}
}

/*************************************************
Function:     get_yuyue_str
Description:  根据字符串ID取字符串
Input:
1.hDlg
Output:       无
Return:       无
Others:
*************************************************/
static void get_yuyue_str()
{
	uint8 i;

	// 取每日、每周、 每月字符串
	for (i = 0; i < 4; i++)
	{
		memcpy(YuyueRepStr[i], get_str(SID_Jd_Yuyue_EveryDay + i), sizeof(YuyueRepStr[0]));
	}

	// 取星期一到星期日字符串
	for (i = 0; i < 7; i++)
	{
		memcpy(YuyueWeekStr[i], get_str(SID_Jd_Yuyue_Week_Sun + i), sizeof(YuyueWeekStr[0]));
	}
}

/*************************************************
Function:		SetJdYuYueParamLayerOnShow
Description: 	进入预约设置参数界面
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
void SetJdYuYueParamLayerOnShow(uint8 index)
{
	if (!SetJdYuYueParamLayer)
	{
		SetJdYuYueParamLayer = ituSceneFindWidget(&theScene, "SetJdYuYueParamLayer");
		assert(SetJdYuYueParamLayer);

		get_yuyue_str();
	}

	g_YuyueListIndex = index;

	init_param();

	ituLayerGoto(SetJdYuYueParamLayer);
}

/*************************************************
Function:		SetJdYuYueParamLayerOnReturn
Description: 	底部退出键按下执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
void SetJdYuYueParamLayerOnReturn(void)
{
	if (ituWidgetIsVisible(SetJdYuyueTypeBackground))
	{
		ituWidgetEnable(SetJdYuYueParamBackground);
		ituWidgetSetVisible(SetJdYuyueTypeBackground, false);
		return;
	}
	else if (ituWidgetIsVisible(SetJdYuyueDeviceBackground))
	{
		ituWidgetEnable(SetJdYuYueParamBackground);
		ituWidgetSetVisible(SetJdYuyueDeviceBackground, false);
		return;
	}
	else if (ituWidgetIsVisible(SetJdYuYueParamBrightnessBackground))
	{
		ituWidgetEnable(SetJdYuYueParamBackground);
		ituWidgetSetVisible(SetJdYuYueParamBrightnessBackground, false);
	}
	else if (ituWidgetIsVisible(SetJdYuYueParamTimeTypeBackground))
	{
		ituWidgetEnable(SetJdYuYueParamBackground);
		ituWidgetSetVisible(SetJdYuYueParamTimeTypeBackground, false);
	}
	else if (ituWidgetIsVisible(SetJdYuYueParamTimeDateBackground))
	{
		ituWidgetEnable(SetJdYuYueParamBackground);
		ituWidgetSetVisible(SetJdYuYueParamTimeDateBackground, false);
	}
	else if (ituWidgetIsVisible(SetJdYuYueParamTimeTimeBackground))
	{
		ituWidgetEnable(SetJdYuYueParamBackground);
		ituWidgetSetVisible(SetJdYuYueParamTimeTimeBackground, false);
	}
	else if (ituWidgetIsVisible(SetJdYuYueParamTimeWeekBackground))
	{
		ituWidgetEnable(SetJdYuYueParamBackground);
		ituWidgetSetVisible(SetJdYuYueParamTimeWeekBackground, false);
	}
	else if (ituWidgetIsVisible(SetJdYuYueParamBackground))
	{
		ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_WARNNING, SID_Msg_Param_Suer_Save, "SetJdYuYueParamLayer");

		return;
	}
}
