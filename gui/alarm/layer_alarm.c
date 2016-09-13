/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_alarm.c
Author:     	zxc
Version:
Date: 		2016-08-31
Description: 安防界面
History:
1. Date:
Author:
Modification:
*************************************************/
#include "layer_alarm.h"

#define GO_TO_ALARM_TIMER_LEN	1000	//安防进入定时器中断时间

// 右边
static ITUContainer* AlarmRightHomeContainer = NULL;
static ITUContainer* AlarmRightNoHomeContainer = NULL;
static ITURadioBox* AlarmRightSecurityRadioBox = NULL;
static ITURadioBox* AlarmRightAlarmLogRadioBox = NULL;
//static ITURadioBox* AlarmRightOperLogRadioBox = NULL;

// 底部
static ITUContainer* AlarmBottomContainer = NULL;
static ITUContainer* AlarmBottomCleanBtnContainer = NULL;
static ITURadioBox* AlarmBottomHomeRadioBox = NULL;
static ITURadioBox* AlarmBottomAwayRadioBox = NULL;
static ITURadioBox* AlarmBottomNightRadioBox = NULL;
static ITUButton* AlarmBottomResetButton = NULL;
static ITUSprite* AlarmBottomSprite[3] = { NULL };
static ITUText* AlarmBottomText[3] = { NULL };


// 记录界面
static ITUCoverFlow* AlarmRecordListCoverFlow = NULL;
static ITUContainer* AlarmRecordListContainer = NULL;
static ITUText* AlarmRecordList0Text = NULL;
static ITUText* AlarmRecordList1Text = NULL;
static ITUText* AlarmRecordList2Text = NULL;
static ITUText* AlarmRecordList3Text = NULL;

// 八防区主界面
static ITUBackground* AlarmMainBackground = NULL;
static ITUText* AlarmHitText = NULL;


static PALARM_TOUCH_INFO_LIST g_pUnreadList = NULL;		// 报警未读记录
static uint8 g_alarm_rec_scroll_open = FALSE;			// hit未读记录大于2条时滚动显示
static uint8 g_alarm_rec_scroll_timer_flag = 0;			// 计时多久滚一次未读记录
static uint32 g_unread_alarm_rec_show_num = 0;			// 记录当前显示第几条未读记录
static uint32 g_last_time_tick;							// 用来记录定时器上个时刻的时间



/*************************************************
Function:		updata_unread_alarm_rec_data
Description: 	更新UI未读记录
Input:		无
Output:		无
Return:		TRUE 成功 FALSE 失败
Others:
*************************************************/
static void updata_unread_alarm_rec_data()
{
	if (g_pUnreadList)
	{
		free(g_pUnreadList);
		g_pUnreadList = NULL;
	}

	g_pUnreadList = storage_get_afbj_unread_record();
}

/*************************************************
Function:		ui_get_record_textid
Description: 	获取控制执行者
Input:		无
Output:		无
Return:		TRUE 成功 FALSE 失败
Others:
*************************************************/
static uint16 ui_get_record_textid(EXECUTOR_E type)
{
	switch (type)
	{
	case EXECUTOR_LOCAL_HOST:
		return SID_Bj_Bengjikongzhi;

	case EXECUTOR_MANAGER:
		return SID_DevManager;

	case EXECUTOR_REMOTE_DEVICE:				// add by luofl 2011-4-27
		return SID_Bj_Fengjikongzhi;

	default:
		return SID_Bj_Bengjikongzhi;
	}
}

/*************************************************
Function:		ShowOneRecordWin
Description: 	显示一条记录
Input:		
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void ShowOneRecordWin(uint8 page_type, uint8 num, AF_BJ_REC bj_rec, AF_CZ_REC cz_rec, AF_FLASH_DATA alarm_data)
{
	char tmp[100];

	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp, "%s%d", "AlarmRecordList0Text", num);
	AlarmRecordList0Text = ituSceneFindWidget(&theScene, tmp);
	assert(AlarmRecordList0Text);
	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp, "%s%d", "AlarmRecordList0Text", num);
	AlarmRecordList1Text = ituSceneFindWidget(&theScene, tmp);
	assert(AlarmRecordList1Text);
	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp, "%s%d", "AlarmRecordList0Text", num);
	AlarmRecordList2Text = ituSceneFindWidget(&theScene, tmp);
	assert(AlarmRecordList2Text);
	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp, "%s%d", "AlarmRecordList0Text", num);
	AlarmRecordList3Text = ituSceneFindWidget(&theScene, tmp);
	assert(AlarmRecordList3Text);
	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp, "%s%d", "AlarmRecordListContainer", num);
	AlarmRecordListContainer = ituSceneFindWidget(&theScene, tmp);
	assert(AlarmRecordListContainer);

	if (1 == page_type)
	{
		ituTextSetString(AlarmRecordList0Text, get_str(SID_Bj_Report_Type_Chufa));

		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%s%d", get_str(SID_Bj_Fangqu), ":", bj_rec.areaNum);
		ituTextSetString(AlarmRecordList1Text, tmp);

		ituTextSetString(AlarmRecordList2Text, get_str(SID_Bj_SOS + alarm_data.area_type[bj_rec.areaNum - 1]));

		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%04d-%02d-%02d %02d:%02d:%02d", bj_rec.time.year, bj_rec.time.month, bj_rec.time.day,
				bj_rec.time.hour, bj_rec.time.min, bj_rec.time.sec);
		ituTextSetString(AlarmRecordList3Text, tmp);

		if (FALSE == bj_rec.bReaded)
		{
			ituSetColor(&((ITUWidget*)AlarmRecordList0Text)->color, 255, 255, 0, 0);
			ituSetColor(&((ITUWidget*)AlarmRecordList1Text)->color, 255, 255, 0, 0);
			ituSetColor(&((ITUWidget*)AlarmRecordList2Text)->color, 255, 255, 0, 0);
			ituSetColor(&((ITUWidget*)AlarmRecordList3Text)->color, 255, 255, 0, 0);
		}
		else
		{
			ituSetColor(&((ITUWidget*)AlarmRecordList0Text)->color, 255, 255, 255, 255);
			ituSetColor(&((ITUWidget*)AlarmRecordList1Text)->color, 255, 255, 255, 255);
			ituSetColor(&((ITUWidget*)AlarmRecordList2Text)->color, 255, 255, 255, 255);
			ituSetColor(&((ITUWidget*)AlarmRecordList3Text)->color, 255, 255, 255, 255);
		}
		ituWidgetSetVisible(AlarmRecordList2Text, true);
	}
	else
	{
		ituTextSetString(AlarmRecordList0Text, get_str(SID_Bj_Bf_Home + cz_rec.type));
		ituTextSetString(AlarmRecordList1Text, get_str(ui_get_record_textid(cz_rec.executor)));
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%04d-%02d-%02d %02d:%02d:%02d", cz_rec.time.year, cz_rec.time.month, cz_rec.time.day,
			cz_rec.time.hour, cz_rec.time.min, cz_rec.time.sec);
		ituTextSetString(AlarmRecordList3Text, tmp);

		ituSetColor(&((ITUWidget*)AlarmRecordList0Text)->color, 255, 255, 255, 255);
		ituSetColor(&((ITUWidget*)AlarmRecordList1Text)->color, 255, 255, 255, 255);
		//ituSetColor(&((ITUWidget*)AlarmRecordList2Text)->color, 255, 255, 255, 255);
		ituSetColor(&((ITUWidget*)AlarmRecordList3Text)->color, 255, 255, 255, 255);
		ituWidgetSetVisible(AlarmRecordList2Text, false);
	}
	ituWidgetSetVisible(AlarmRecordListContainer, true);
}

/*************************************************
Function:		ShowRecordWin
Description: 	显示记录页面
Input:		page_type   1: 报警记录	2：操作记录
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void ShowRecordWin(uint8 page_type)
{
	uint8 i = 0;
	uint32 max = 0;
	char tmp[100] = { 0 };
	PAF_CZ_LIST paf_cz_list = NULL;
	PAF_BJ_LIST  paf_bj_list = NULL;
	AF_FLASH_DATA alarm_data;
	AF_BJ_REC bj_rec_null;
	AF_CZ_REC cz_rec_null;

	logic_get_alarm_param((uint8 *)(&alarm_data));

	if (1 == page_type)
	{
		paf_bj_list = storage_get_afbj_record();
		if (paf_bj_list && paf_bj_list->nCount > 0)
		{
			max = paf_bj_list->nCount;
			memset(&cz_rec_null, 0, sizeof(AF_CZ_REC));
			for (i = 0; i < max; i++)
			{
				ShowOneRecordWin(page_type, i, paf_bj_list->pAfBjRec[i], cz_rec_null, alarm_data);
			}
		}
	}
	else
	{
		paf_cz_list = storage_get_afcz_record();
		if (paf_cz_list && paf_cz_list->nCount > 0)
		{
			max = paf_cz_list->nCount;
			memset(&bj_rec_null, 0, sizeof(AF_BJ_REC));
			for (i = 0; i < max; i++)
			{
				ShowOneRecordWin(page_type, i, bj_rec_null, paf_cz_list->pAfCzRec[i], alarm_data);
			}
		}
	}

	for (i = max; i < AF_REC_MAX; i++)
	{
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "AlarmRecordListContainer", i);
		AlarmRecordListContainer = ituSceneFindWidget(&theScene, tmp);
		assert(AlarmRecordListContainer);
		ituWidgetSetVisible(AlarmRecordListContainer, false);
	}

	ituCoverFlowGoto(AlarmRecordListCoverFlow, 0);
	if (!AlarmRightAlarmLogRadioBox)
	{
		AlarmRightAlarmLogRadioBox = ituSceneFindWidget(&theScene, "AlarmRightAlarmLogRadioBox");
		assert(AlarmRightAlarmLogRadioBox);
	}
	ituRadioBoxSetChecked(AlarmRightAlarmLogRadioBox, true);

	ituWidgetSetVisible(AlarmMainBackground, false);
	ituWidgetSetVisible(AlarmRecordListCoverFlow, true);
	ituWidgetSetVisible(AlarmRightHomeContainer, false);
	ituWidgetSetVisible(AlarmRightNoHomeContainer, true);
	ituWidgetSetVisible(AlarmBottomContainer, false);
	ituWidgetSetVisible(AlarmBottomCleanBtnContainer, true);
}

/*************************************************
Function:		show_alarm_main_hit
Description: 	显示安防主界面提示
Input:
Output:		无
Return:		无
Others:
*************************************************/
static void show_alarm_main_hit()
{
	uint8 state = 0;

	//updata_unread_alarm_rec_data();
	if (g_pUnreadList && (g_pUnreadList->nCount > 0))
	{
		if (1 == g_pUnreadList->nCount)
		{
			ituWidgetSetVisible(AlarmHitText, true);
		}
		else
		{
			g_alarm_rec_scroll_open = TRUE;
		}
	}
	else
	{
		if (DIS_DEFEND == storage_get_defend_state())
		{
			state = judge_can_be_defend();
			if (0 == state)
			{
				ituTextSetString(AlarmHitText, get_str(SID_Bj_Already_Bufang));
				ituWidgetSetVisible(AlarmHitText, true);
			}
			else
			{
				ituWidgetSetVisible(AlarmHitText, false);
			}
		}
		else
		{
			ituWidgetSetVisible(AlarmHitText, false);
		}
	}
}

/*************************************************
Function:		show_alarm_main_eight_area
Description: 	显示安防八防区
Input:
Output:		无
Return:		无
Others:
*************************************************/
static void show_alarm_main_eight_area()
{

}

/*************************************************
Function:		show_alarm_main_bottom_enable
Description: 	显示安防界面底部控件是否使能
Input:
Output:		无
Return:		无
Others:
*************************************************/
static void show_alarm_main_bottom_enable(uint8 enable)
{
	uint8 i = 0;
	if (!AlarmBottomResetButton)
	{
		AlarmBottomResetButton = ituSceneFindWidget(&theScene, "AlarmBottomResetButton");
		assert(AlarmBottomResetButton);

		AlarmBottomSprite[0] = ituSceneFindWidget(&theScene, "AlarmBottomAwaySprite");
		assert(AlarmBottomSprite[0]);

		AlarmBottomSprite[1] = ituSceneFindWidget(&theScene, "AlarmBottomNightSprite");
		assert(AlarmBottomSprite[1]);

		AlarmBottomSprite[2] = ituSceneFindWidget(&theScene, "AlarmBottomResetSprite");
		assert(AlarmBottomSprite[2]);

		AlarmBottomText[0] = ituSceneFindWidget(&theScene, "AlarmBottomAwayText");
		assert(AlarmBottomText[0]);

		AlarmBottomText[1] = ituSceneFindWidget(&theScene, "AlarmBottomNightText");
		assert(AlarmBottomText[1]);

		AlarmBottomText[2] = ituSceneFindWidget(&theScene, "AlarmBottomResetText");
		assert(AlarmBottomText[2]);
	}

	if (enable)
	{
		for (i = 0; i < 3; i++)
		{
			ituSpriteGoto(AlarmBottomSprite[i], 0);
			ituSetColor(&((ITUWidget*)AlarmBottomText[i])->color, 255, 255, 255, 255);
		}
		ituWidgetEnable(AlarmBottomAwayRadioBox);
		ituWidgetEnable(AlarmBottomNightRadioBox);
		ituWidgetEnable(AlarmBottomResetButton);
	}
	else
	{
		for (i = 0; i < 3; i++)
		{
			ituSpriteGoto(AlarmBottomSprite[i], 1);
			ituSetColor(&((ITUWidget*)AlarmBottomText[i])->color, 255, 85, 85, 85);
		}
		ituWidgetDisable(AlarmBottomAwayRadioBox);
		ituWidgetDisable(AlarmBottomNightRadioBox);
		ituWidgetDisable(AlarmBottomResetButton);
	}
}

/*************************************************
Function:		show_alarm_main_bottom
Description: 	显示安防界面底部控件
Input:
Output:		无
Return:		无
Others:
*************************************************/
static void show_alarm_main_bottom()
{
	if (!AlarmBottomHomeRadioBox)
	{
		AlarmBottomHomeRadioBox = ituSceneFindWidget(&theScene, "AlarmBottomHomeRadioBox");
		assert(AlarmBottomHomeRadioBox);

		AlarmBottomAwayRadioBox = ituSceneFindWidget(&theScene, "AlarmBottomAwayRadioBox");
		assert(AlarmBottomAwayRadioBox);

		AlarmBottomNightRadioBox = ituSceneFindWidget(&theScene, "AlarmBottomNightRadioBox");
		assert(AlarmBottomNightRadioBox);
	}

	switch (storage_get_defend_state())
	{
	case DIS_DEFEND:
		ituRadioBoxSetChecked(AlarmBottomHomeRadioBox, true);
		show_alarm_main_bottom_enable(TRUE);
		ituWidgetSetVisible(AlarmRightNoHomeContainer, false);
		ituWidgetSetVisible(AlarmRightHomeContainer, true);
		break;

	case SET_DEFEND:
		ituRadioBoxSetChecked(AlarmBottomAwayRadioBox, true);
		show_alarm_main_bottom_enable(FALSE);
		ituWidgetSetVisible(AlarmRightHomeContainer, false);
		ituWidgetSetVisible(AlarmRightNoHomeContainer, true);
		break;

	case PART_DEFEND:
		ituRadioBoxSetChecked(AlarmBottomNightRadioBox, true);
		show_alarm_main_bottom_enable(FALSE);
		ituWidgetSetVisible(AlarmRightHomeContainer, false);
		ituWidgetSetVisible(AlarmRightNoHomeContainer, true);
		break;
	}
}

/*************************************************
Function:		ShowAlarmMainWin
Description: 	显示安防主界面
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void ShowAlarmMainWin()
{
	show_alarm_main_hit();
	show_alarm_main_eight_area();
	show_alarm_main_bottom();
	ituWidgetSetVisible(AlarmRecordListCoverFlow, false);
	ituWidgetSetVisible(AlarmMainBackground, true);
	ituWidgetSetVisible(AlarmBottomCleanBtnContainer, false);
	ituWidgetSetVisible(AlarmBottomContainer, true);
}

/*************************************************
Function:		AlarmOnEnter
Description: 	安防界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool AlarmOnEnter(ITUWidget* widget, char* param)
{
	uint8 i = 0;
	//char tmp[100];
	PALARM_TOUCH_INFO_LIST pUnreadList = NULL;
	uint8 alarming;

	if (!AlarmBottomContainer)
	{
		AlarmBottomContainer = ituSceneFindWidget(&theScene, "AlarmBottomContainer");
		assert(AlarmBottomContainer);

		AlarmBottomCleanBtnContainer = ituSceneFindWidget(&theScene, "AlarmBottomCleanBtnContainer");
		assert(AlarmBottomCleanBtnContainer);

		AlarmRightHomeContainer = ituSceneFindWidget(&theScene, "AlarmRightHomeContainer");
		assert(AlarmRightHomeContainer);

		AlarmRightNoHomeContainer = ituSceneFindWidget(&theScene, "AlarmRightNoHomeContainer");
		assert(AlarmRightNoHomeContainer);

		AlarmMainBackground = ituSceneFindWidget(&theScene, "AlarmMainBackground");
		assert(AlarmMainBackground);

		AlarmRecordListCoverFlow = ituSceneFindWidget(&theScene, "AlarmRecordListCoverFlow");
		assert(AlarmRecordListCoverFlow);

		AlarmHitText = ituSceneFindWidget(&theScene, "AlarmHitText");
		assert(AlarmHitText);
	}

	g_last_time_tick = SDL_GetTicks();		//开启定时器前要先获取一次当前时间以便对比

	updata_unread_alarm_rec_data();

	alarming = logic_get_g_whole_alarm_state_param();
	if (0 == alarming)
	{
		if (pUnreadList && (pUnreadList->nCount > 0))
		{
			ShowRecordWin(1);
			return true;
		}
	}
	ShowAlarmMainWin();

	return true;
}

/*************************************************
Function:		ScrollShowAlarmUnreadRec
Description: 	hit滚动显示未读报警记录
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void ScrollShowAlarmUnreadRec()
{	
	char tmp[200] = { 0 };
	char tmp1[100] = { 0 };
	uint8 touch_num = 0;
	AF_FLASH_DATA alarm_data;

	if (NULL == g_pUnreadList)
	{
		return;
	}
	
	ituWidgetHide(AlarmHitText, ITU_EFFECT_SCROLL_UP, 15);

	//if (ituWidgetIsVisible(AlarmHitText) == false)
	{
		logic_get_alarm_param((uint8 *)(&alarm_data));

		if (g_unread_alarm_rec_show_num >= g_pUnreadList->nCount)
		{
			g_unread_alarm_rec_show_num = 0;
		}
		else
		{
			g_unread_alarm_rec_show_num++;
		}
		
		touch_num = g_pUnreadList->pAlarmRec[g_unread_alarm_rec_show_num].TouchNum;
		memset(tmp, 0, sizeof(tmp));
		strcpy(tmp, get_str(SID_Bj_Report_Type_Chufa));
		memset(tmp1, 0, sizeof(tmp1));
		sprintf(tmp1, "%s%s%s%d", " ", get_str(SID_Bj_Fangqu), ":", touch_num);
		strcat(tmp, tmp1);
		memset(tmp1, 0, sizeof(tmp1));
		sprintf(tmp1, "%s%s%s", " ", get_str(SID_Bj_SOS + alarm_data.area_type[touch_num - 1]), " ");
		strcat(tmp, tmp1);
		strcat(tmp, g_pUnreadList->pAlarmRec[g_unread_alarm_rec_show_num].time);

		ituTextSetString(AlarmHitText, tmp);
		ituWidgetShow(AlarmHitText, ITU_EFFECT_SCROLL_UP, 15);
	}
#if 0
	else
	{
		if (gScrollTimeCount == MAIN_SCROLLTEXT_SHOW_TIME)
		{
			ituWidgetHide(AlarmHitText, ITU_EFFECT_SCROLL_UP, 15);
		}
	}
#endif
}

/*************************************************
Function:		AlarmLayerTimeoutOnTimer
Description: 	安防界面定时器函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool AlarmLayerTimeoutOnTimer(ITUWidget* widget, char* param)
{
	uint32 duration;
	uint32 curtime = SDL_GetTicks();

	if (curtime >= g_last_time_tick)
	{
		duration = curtime - g_last_time_tick;
	}
	else
	{
		duration = 0xFFFFFFFF - g_last_time_tick + curtime;
	}

	if (duration >= GO_TO_ALARM_TIMER_LEN)		//时间差是以毫秒为单位
	{
		g_last_time_tick = curtime;

		if (2 >= g_alarm_rec_scroll_timer_flag)
		{
			g_alarm_rec_scroll_timer_flag = 0;

			if (TRUE == g_alarm_rec_scroll_open)
			{
				ScrollShowAlarmUnreadRec();
			}
		}
		else
		{
			g_alarm_rec_scroll_timer_flag++;
		}
	}

	return true;
}
