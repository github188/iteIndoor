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

#define GO_TO_ALARM_TIMER_LEN	500		//安防进入定时器中断时间

#define ICON_DIS_X				44		
#define ICON_DIS_Y				30
#define ICON_DIS_X_ONE			152
#define ICON_DIS_Y_ONE			153

//部分函数申明
static void ShowAlarmKetBordWin();
static void AlarmHitMsgBackgroundOnShow(uint8 spriteflag, char* show_text);

// 安防界面
static ITULayer* AlarmLayer = NULL;

// 右边
static ITUContainer* AlarmRightHomeContainer = NULL;
static ITUContainer* AlarmRightNoHomeContainer = NULL;
static ITURadioBox* AlarmRightSecurityRadioBox = NULL;
static ITURadioBox* AlarmRightAlarmLogRadioBox = NULL;
static ITURadioBox* AlarmRightOperLogRadioBox = NULL;

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
static ITUBackground* AlarmRecordListBackground = NULL;
static ITUContainer* AlarmRecordListContainer = NULL;
static ITUText* AlarmRecordList0Text = NULL;
static ITUText* AlarmRecordList1Text = NULL;
static ITUText* AlarmRecordList2Text = NULL;
static ITUText* AlarmRecordList3Text = NULL;

// 八防区主界面
static ITUBackground* AlarmMainBackground = NULL;
static ITUText* AlarmHitText = NULL;
static ITUText* AlarmFqText = NULL;
static ITUButton* AlarmFqButton = NULL;
static ITUSprite* area_type_sprite[AREA_AMOUNT] = { NULL };
static ITUSprite* clone_area_type_sprite[AREA_AMOUNT] = { NULL };

// 键盘界面
static ITUTextBox* AlarmKeyBordTextBox = NULL;
static ITUText* AlarmKeyBordHitText = NULL;
static ITUBackground* AlarmKeyBordBackground = NULL;
static ITUBackground* AlarmBackground = NULL;

// 提示框界面
static ITUBackground* AlarmHitMsgBackground = NULL;
static ITUText* AlarmHitMsgText = NULL;
static ITUContainer* AlarmHitMsg0Calendar = NULL;
static ITUContainer* AlarmHitMsg1Calendar = NULL;
static ITUSprite* AlarmHitMsgSprite = NULL;
static ITURadioBox* AlarmMsgHitSuccess0RadioBox = NULL;
static ITURadioBox* AlarmMsgHitSuccess1RadioBox = NULL;

// 变量定义
static PALARM_TOUCH_INFO_LIST g_pUnreadList = NULL;				// 报警未读记录
static uint8 g_alarm_rec_scroll_open = FALSE;					// hit未读记录大于2条时滚动显示
static uint8 g_alarm_rec_scroll_timer_flag = 0;					// 计时多久滚一次未读记录
static uint32 g_unread_alarm_rec_show_num = 0;					// 记录当前显示第几条未读记录
static uint32 g_last_time_tick;									// 用来记录定时器上个时刻的时间
static uint8 g_timer_show_shanshuo_flag = 0;					// 定时器是否闪烁显示 1个Bit表示一个防区
static uint8 g_sprite_shanshuo_flag[AREA_AMOUNT] = { 0 };		// 闪烁颜色选择		// 1:蓝红闪烁	2:黄红闪烁
static uint8 g_show_flag = FALSE;								// 解决报警时报警界面还没显示出来，logic回调UI刷新界面会死机

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

	g_pUnreadList = storage_get_afbj_unread_record_ext();

	debug_log("g_pUnreadList->nCount = %d\n", g_pUnreadList->nCount);
}

/*************************************************
Function:		GetUnicodeLen
Description:  获取unicode字符串长度
Input:
1.U1
2.U2
Output:		无
Return:		长度
Others:
*************************************************/
static uint8 GetUnicodeLen(char *p)
{
	uint8 Len = 0;
	if (!p)return 0;
	while (*p)
	{
		Len++;
		p++;
	}
	return Len;
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

	if (num < 6)		//条数大于6后，使用clone出来的list，在clone时直接赋值text指针
	{
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "AlarmRecordList0Text", num);
		AlarmRecordList0Text = ituSceneFindWidget(&theScene, tmp);
		assert(AlarmRecordList0Text);
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "AlarmRecordList1Text", num);
		AlarmRecordList1Text = ituSceneFindWidget(&theScene, tmp);
		assert(AlarmRecordList1Text);
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "AlarmRecordList2Text", num);
		AlarmRecordList2Text = ituSceneFindWidget(&theScene, tmp);
		assert(AlarmRecordList2Text);
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "AlarmRecordList3Text", num);
		AlarmRecordList3Text = ituSceneFindWidget(&theScene, tmp);
		assert(AlarmRecordList3Text);
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "AlarmRecordListContainer", num);
		AlarmRecordListContainer = ituSceneFindWidget(&theScene, tmp);
		assert(AlarmRecordListContainer);
	}

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

	if (num < 6)
	{
		ituWidgetSetVisible(AlarmRecordListContainer, true);
	}
}

/*************************************************
Function:		clone_one_list
Description: 	clone列表控件
Input:		无
Output:		无
Return:		TRUE 成功 FALSE 失败
Others:
*************************************************/
static void clone_one_list(uint8 num)
{
	char tmp[100];
	bool result = FALSE;
	ITUBackground* BackgroundOld = NULL;
	ITUContainer* CloneChildContainer = NULL;
	ITUButton* AlarmRecordListButton = NULL;

	BackgroundOld = (ITUBackground*)ituSceneFindWidget(&theScene, "AlarmRecordListBackground5");

	AlarmRecordListBackground = NULL;
	result = ituWidgetClone(BackgroundOld, &AlarmRecordListBackground);
	if (result)
	{
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "AlarmRecordListBackground", num);
		ituWidgetSetName(AlarmRecordListBackground, tmp);
		ituWidgetSetX(AlarmRecordListBackground, 0);
		ituWidgetSetY(AlarmRecordListBackground, 0);

		CloneChildContainer = itcTreeGetChildAt(AlarmRecordListBackground, 0);
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "AlarmRecordListContainer", num);
		ituWidgetSetName(CloneChildContainer, tmp);

		AlarmRecordListButton = itcTreeGetChildAt(CloneChildContainer, 0);
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "AlarmRecordListButton", num);
		ituWidgetSetName(AlarmRecordListButton, tmp);

		AlarmRecordList0Text = itcTreeGetChildAt(CloneChildContainer, 1);
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "AlarmRecordList0Text", num);
		ituWidgetSetName(AlarmRecordList0Text, tmp);

		AlarmRecordList1Text = itcTreeGetChildAt(CloneChildContainer, 2);
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "AlarmRecordList1Text", num);
		ituWidgetSetName(AlarmRecordList1Text, tmp);

		AlarmRecordList3Text = itcTreeGetChildAt(CloneChildContainer, 3);
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "AlarmRecordList3Text", num);
		ituWidgetSetName(AlarmRecordList3Text, tmp);

		AlarmRecordList2Text = itcTreeGetChildAt(CloneChildContainer, 4);
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "AlarmRecordList2Text", num);
		ituWidgetSetName(AlarmRecordList2Text, tmp);

		ituWidgetAdd(AlarmRecordListCoverFlow, AlarmRecordListBackground);	
		ituCoverFlowUpdate((ITUWidget*)AlarmRecordListCoverFlow, ITU_EVENT_LAYOUT, 0, 0, 0);
	}
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
	ITUBackground* BackgroundFlag = NULL;

	logic_get_alarm_param((uint8 *)(&alarm_data));

	for (i = 6; i < AF_REC_MAX; i++)
	{
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "AlarmRecordListBackground", i);
		BackgroundFlag = (ITUBackground*)ituSceneFindWidget(&theScene, tmp);
		if (NULL != BackgroundFlag)							//判断是否clone过
		{
			itcTreeRemove(BackgroundFlag);						// 删除已有的
			ituWidgetExit(BackgroundFlag);
			BackgroundFlag = NULL;
			debug_log("Remove a clone list background!!!\n");
		}
	}

	if (1 == page_type)
	{
		paf_bj_list = storage_get_afbj_record();
		if (paf_bj_list && paf_bj_list->nCount > 0)
		{
			max = paf_bj_list->nCount;
			memset(&cz_rec_null, 0, sizeof(AF_CZ_REC));

			for (i = 0; i < max; i++)
			{
				if (i >= 6)				//条数大于等于6时之后的列表都是用clone
				{
					clone_one_list(i);
				}
				ShowOneRecordWin(page_type, i, paf_bj_list->pAfBjRec[i], cz_rec_null, alarm_data);
			}

			//if (g_pUnreadList->nCount > 0)	//查看完有未读记录，则清除未读记录
			{
				storage_clear_afbj_unread_state();		
				updata_unread_alarm_rec_data();
				sys_sync_hint_state_ext(SYS_HINT_ALARM_WARNING);
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
				if (i >= 6)				//条数大于等于6时之后的列表都是用clone
				{
					clone_one_list(i);
				}
				ShowOneRecordWin(page_type, i, bj_rec_null, paf_cz_list->pAfCzRec[i], alarm_data);
			}
		}
	}

	if (max < 6)
	{
		for (max; max < 6; max++)
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "AlarmRecordListContainer", max);
			AlarmRecordListContainer = ituSceneFindWidget(&theScene, tmp);
			assert(AlarmRecordListContainer);
			ituWidgetSetVisible(AlarmRecordListContainer, false);
		}
	}

	ituCoverFlowGoto(AlarmRecordListCoverFlow, 0);

	ituWidgetSetVisible(AlarmMainBackground, false);
	ituWidgetSetVisible(AlarmRecordListCoverFlow, true);
	ituWidgetSetVisible(AlarmRightNoHomeContainer, false);
	ituWidgetSetVisible(AlarmRightHomeContainer, true);
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
	char tmp[200] = { 0 };
	char tmp1[100] = { 0 };
	uint8 touch_num = 0;
	AF_FLASH_DATA alarm_data;

	if (g_pUnreadList && (g_pUnreadList->nCount > 0))
	{
		ituSetColor(&((ITUWidget*)AlarmHitText)->color, 255, 255, 0, 0);	//红色字体
		if (1 == g_pUnreadList->nCount)			//未读条数为1，不滚动显示
		{
			logic_get_alarm_param((uint8 *)(&alarm_data));

			touch_num = g_pUnreadList->pAlarmRec[0].TouchNum;
			memset(tmp, 0, sizeof(tmp));
			strcpy(tmp, get_str(SID_Bj_Report_Type_Chufa));
			memset(tmp1, 0, sizeof(tmp1));
			sprintf(tmp1, "%s%s%s%d", " ", get_str(SID_Bj_Fangqu), ":", touch_num);
			strcat(tmp, tmp1);
			memset(tmp1, 0, sizeof(tmp1));
			sprintf(tmp1, "%s%s%s", " ", get_str(SID_Bj_SOS + alarm_data.area_type[touch_num - 1]), " ");
			strcat(tmp, tmp1);
			strcat(tmp, g_pUnreadList->pAlarmRec[0].time);

			ituTextSetString(AlarmHitText, tmp);
			ituWidgetSetVisible(AlarmHitText, true);

			g_alarm_rec_scroll_open = FALSE;
			g_unread_alarm_rec_show_num = 0;
		}
		else									//未读条数不为1，滚动显示
		{
			g_alarm_rec_scroll_open = TRUE;
		}
	}
	else
	{
		if (DIS_DEFEND == storage_get_defend_state())
		{
			ituSetColor(&((ITUWidget*)AlarmHitText)->color, 255, 0, 0, 255);		//蓝色字体
			ituTextSetString(AlarmHitText, get_str(SID_Bj_Already_Bufang));
			ituWidgetSetVisible(AlarmHitText, true);
		}
		else
		{
			ituWidgetSetVisible(AlarmHitText, false);
		}

		g_alarm_rec_scroll_open = FALSE;
		g_unread_alarm_rec_show_num = 0;
	}
}

/*************************************************
Function:		clone_one_fangqu_sprite
Description: 	clone一个防区
Input:
Output:		无
Return:		无
Others:
*************************************************/
static void clone_one_fangqu_sprite(ITUSprite* old_sprite, uint8 num, char* name_buf)
{
	uint8 i;
	char tmp[100];
	bool result = FALSE;
	ITUIcon* CloneChildIcon = NULL;
	ITUSprite* CloneAreaTypeSprite = NULL;

	result = ituWidgetClone(old_sprite, &CloneAreaTypeSprite);
	if (result)
	{
		ituWidgetSetName(CloneAreaTypeSprite, name_buf);
		ituWidgetSetX(CloneAreaTypeSprite, ICON_DIS_X + ICON_DIS_X_ONE * (num % 4));
		ituWidgetSetY(CloneAreaTypeSprite, ICON_DIS_Y + ICON_DIS_Y_ONE * (num / 4));

		for(i = 0; i < 5; i++)
		{
			CloneChildIcon = itcTreeGetChildAt(CloneAreaTypeSprite, i);
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%s%d", name_buf, "Icon", i);
			ituWidgetSetName(CloneChildIcon, tmp);
			CloneChildIcon = NULL;
		}

		ituWidgetAdd(AlarmMainBackground, CloneAreaTypeSprite);
		if (!ituWidgetIsVisible(CloneAreaTypeSprite))
		{
			ituWidgetSetVisible(CloneAreaTypeSprite, true);
		}

		clone_area_type_sprite[num] = CloneAreaTypeSprite;
		
		ituWidgetUpdate(AlarmMainBackground, ITU_EVENT_LAYOUT, 0, 0, 0);
	}
}

/*************************************************
Function:		update_area_state
Description: 	显示安防八防区
Input:		
Output:		无
Return:		无
Others:
*************************************************/
static void update_area_state(uint8 clone_flag)
{
	uint8 i;
	char tmp[50];
	uint8 is24hour;
	uint8 no24clear;					// 非24小时防区是否清除警戒标志
	uint8 no24touch;					// 非24小时防区触发标志
	AF_FLASH_DATA af_data;
	ITUSprite* free_sprite = NULL;
	//状态与图标索引的对应
	uint8 state_to_bmpoder[5] = { 0, 0, 3, 2, 1};

	if (!area_type_sprite[0])
	{
		area_type_sprite[0] = ituSceneFindWidget(&theScene, "AlarmFqUrgencySprite");
		assert(area_type_sprite[0]);

		area_type_sprite[1] = ituSceneFindWidget(&theScene, "AlarmFqFireSprite");
		assert(area_type_sprite[1]);

		area_type_sprite[2] = ituSceneFindWidget(&theScene, "AlarmFqGasSprite");
		assert(area_type_sprite[2]);

		area_type_sprite[3] = ituSceneFindWidget(&theScene, "AlarmFqDoorSprite");
		assert(area_type_sprite[3]);

		area_type_sprite[4] = ituSceneFindWidget(&theScene, "AlarmFqRoomSprite");
		assert(area_type_sprite[4]);

		area_type_sprite[5] = ituSceneFindWidget(&theScene, "AlarmFqWindow1Sprite");
		assert(area_type_sprite[5]);

		area_type_sprite[6] = ituSceneFindWidget(&theScene, "AlarmFqWindow2Sprite");
		assert(area_type_sprite[6]);

		area_type_sprite[7] = ituSceneFindWidget(&theScene, "AlarmFqBalconySprite");
		assert(area_type_sprite[7]);
	}

	logic_get_alarm_param((uint8 *)&af_data);

	if (1 == clone_flag)
	{
		for (i = 0; i < AREA_AMOUNT; i++)
		{
			clone_area_type_sprite[i] = NULL;

			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "clone_area_type_sprite", i);
			free_sprite = (ITUSprite*)ituSceneFindWidget(&theScene, tmp);
			if (NULL != free_sprite)							//判断是否clone过
			{
				itcTreeRemove(free_sprite);						// 删除已有的
				ituWidgetExit(free_sprite);
				free_sprite = NULL;
				debug_log("Remove a clone sprite!!!\n");
			}

			if (i == af_data.area_type[i])
			{
				clone_area_type_sprite[i] = area_type_sprite[i];

				if (!ituWidgetIsVisible(clone_area_type_sprite[i]))
				{
					ituWidgetSetVisible(clone_area_type_sprite[i], true);
				}
			}
			else
			{
				ituWidgetSetVisible(area_type_sprite[i], false);
				clone_one_fangqu_sprite(area_type_sprite[af_data.area_type[i]], i, tmp);
			}
		}
	}
	
	is24hour = af_data.is_24_hour;
	no24clear = alarm_get_24clear_param();
	no24touch = alarm_get_no24touch_param();

	debug_log("is24hour %d, no24clear = %d, no24touch = %d\n", is24hour, no24clear, no24touch);

	memset(g_sprite_shanshuo_flag, 0, sizeof(g_sprite_shanshuo_flag));
	g_timer_show_shanshuo_flag = 0;

	for (i = 0; i < AREA_AMOUNT; i++)
	{
		memset(tmp, 0, sizeof(tmp));													//防区名字显示
		sprintf(tmp, "%s%d", "AlarmFqText", i);
		AlarmFqText = ituSceneFindWidget(&theScene, tmp);
		assert(AlarmFqText);
		ituTextSetString(AlarmFqText, get_str(af_data.area_type[i] + SID_Bj_SOS));		

		if (ALARM_SHOW_STATE == af_data.show_state[i])									//防区图标及闪烁提示
		{
			ituSetColor(&((ITUWidget*)AlarmFqText)->color, 255, 255, 0, 0);	// 字体红色
			g_timer_show_shanshuo_flag |= (1 << i);

			if ((0 == ((is24hour >> i) & 0x01) && (1 == ((no24clear >> i) & 0x01))) || (0 == ((is24hour >> i) & 0x01) && (1 == ((no24touch >> i) & 0x01))))
			{
				// 蓝红闪烁
				g_sprite_shanshuo_flag[i] = 1;
			}
			else
			{	// 黄红闪烁
				g_sprite_shanshuo_flag[i] = 2;
			}
			ituSpriteGoto(clone_area_type_sprite[i], 3);	//一报警就先显示红色，不然会出现别的界面切换到报警界面时，如果这个防区的clone出来的，会先闪一下被clone的那个防区的属性
		}
		else
		{
			g_timer_show_shanshuo_flag &= ~(1 << i);

			if ((UNENABLE_SHOW_STATE == af_data.show_state[i]) || (ISO_SHOW_STATE == af_data.show_state[i]))
			{
				ituSetColor(&((ITUWidget*)AlarmFqText)->color, 255, 85, 85, 85);	//灰色字体
			}
			else
			{
				ituSetColor(&((ITUWidget*)AlarmFqText)->color, 255, 255, 255, 255);	//白色字体
			}
			ituSpriteGoto(clone_area_type_sprite[i], state_to_bmpoder[af_data.show_state[i]]);
		}	

		memset(tmp, 0, sizeof(tmp));					//防区按键是否使能
		sprintf(tmp, "%s%d", "AlarmFqButton", i);
		AlarmFqButton = ituSceneFindWidget(&theScene, tmp);
		assert(AlarmFqButton);
		if (NORM_SHOW_STATE == af_data.show_state[i])
		{
			ituWidgetEnable(AlarmFqButton);
		}
		else
		{
			ituWidgetDisable(AlarmFqButton);
		}
	}
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
static void ShowAlarmMainWin(uint8 flag)
{
	show_alarm_main_hit();
	update_area_state(flag);
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
	uint8 alarming;
	DEFEND_STATE defend_state;

	g_show_flag = FALSE;

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

		AlarmRightAlarmLogRadioBox = ituSceneFindWidget(&theScene, "AlarmRightAlarmLogRadioBox");
		assert(AlarmRightAlarmLogRadioBox);

		AlarmRightOperLogRadioBox = ituSceneFindWidget(&theScene, "AlarmRightOperLogRadioBox");
		assert(AlarmRightOperLogRadioBox);

		AlarmRightSecurityRadioBox = ituSceneFindWidget(&theScene, "AlarmRightSecurityRadioBox");
		assert(AlarmRightSecurityRadioBox);

		AlarmBackground = ituSceneFindWidget(&theScene, "AlarmBackground");
		assert(AlarmBackground);

		AlarmKeyBordBackground = ituSceneFindWidget(&theScene, "AlarmKeyBordBackground");
		assert(AlarmKeyBordBackground);

		AlarmHitMsgBackground = ituSceneFindWidget(&theScene, "AlarmHitMsgBackground");
		assert(AlarmHitMsgBackground);
	}

	ituWidgetSetVisible(AlarmKeyBordBackground, false);
	ituWidgetSetVisible(AlarmHitMsgBackground, false);
	if (!ituWidgetIsEnabled(AlarmBackground))
	{
		ituWidgetEnable(AlarmBackground);
	}

	g_last_time_tick = SDL_GetTicks();		//开启定时器前要先获取一次当前时间以便对比

	updata_unread_alarm_rec_data();

	alarming = logic_get_g_whole_alarm_state_param();
	defend_state = storage_get_defend_state();
	if ((0 == alarming) && (DIS_DEFEND == defend_state))
	{
		if (g_pUnreadList && (g_pUnreadList->nCount > 0))
		{
			ShowRecordWin(1);
			ituRadioBoxSetChecked(AlarmRightAlarmLogRadioBox, true);
			g_show_flag = TRUE;
			return true;
		}
	}
	ShowAlarmMainWin(TRUE);
	ituRadioBoxSetChecked(AlarmRightSecurityRadioBox, true);
	g_show_flag = TRUE;

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

	if (ituWidgetIsVisible(AlarmHitText) == false)
	{
		logic_get_alarm_param((uint8 *)(&alarm_data));

		debug_log("g_unread_alarm_rec_show_num = %d, g_pUnreadList->nCount = %d\n", g_unread_alarm_rec_show_num, g_pUnreadList->nCount);
		
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

		if (g_unread_alarm_rec_show_num >= g_pUnreadList->nCount - 1)
		{
			g_unread_alarm_rec_show_num = 0;
		}
		else
		{
			g_unread_alarm_rec_show_num++;
		}
	}
	else
	{
		if (g_alarm_rec_scroll_timer_flag == 3)
		{
			ituWidgetHide(AlarmHitText, ITU_EFFECT_SCROLL_UP, 15);
		}
	}
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
	uint8 i = 0;
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

		g_alarm_rec_scroll_timer_flag++;
		if (g_alarm_rec_scroll_timer_flag >= 4)		//安防界面未读记录滚动显示
		{
			g_alarm_rec_scroll_timer_flag = 0;
		}

		if (TRUE == g_alarm_rec_scroll_open)		//hit未读记录滚动显示 2s切一条
		{
			ScrollShowAlarmUnreadRec();
		}
		
		if ((0 != g_timer_show_shanshuo_flag) && ((0 == g_alarm_rec_scroll_timer_flag) || (2 == g_alarm_rec_scroll_timer_flag)))	//报警闪烁 1s闪一下
		{
			for (i = 0; i < AREA_AMOUNT; i++)
			{
				if (1 == g_sprite_shanshuo_flag[i])		// 1:蓝红闪烁
				{
					if (2 == g_alarm_rec_scroll_timer_flag)
					{
						ituSpriteGoto(clone_area_type_sprite[i], 1);	//蓝
					}
					else 
					{
						ituSpriteGoto(clone_area_type_sprite[i], 3);	//红
					}	
				}
				else if (2 == g_sprite_shanshuo_flag[i])	// 2:黄红闪烁
				{
					if (2 == g_alarm_rec_scroll_timer_flag)
					{
						ituSpriteGoto(clone_area_type_sprite[i], 4);	//黄
					}
					else 
					{
						ituSpriteGoto(clone_area_type_sprite[i], 3);	//红
					}
				}
			}
			return true;
		}
	}

	return false;
}

/*************************************************
Function:		ShowAlarmKetBordWin
Description: 	安防界提示框器函数
Input:		index:	0：正在报警不能布防
					1：正在报警，无法操作
					2：防区（x）触发不能布防
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void ShowAlarmFailHitMsgWin(uint32 err)
{
	char tmp[100];
	memset(tmp, 0, sizeof(tmp));

	switch (err)
	{
	case AF_BAOJINGNOBUFANG:
		strcpy(tmp, get_str(SID_Bj_Alarm_Cannot_Bf));	//正在报警不能布防
		break;

	case AF_FQCHUFA:									//防区触发，不能布防
		{
			uint8 i = 0;
			uint8 len = 0;
			uint8 state = 0;
			
			state = judge_can_be_defend();
		
			strcpy(tmp, get_str(SID_Bj_Fangqu));
			strcat(tmp, "(");
			len = GetUnicodeLen(tmp);

			for (i = 0; i < AREA_AMOUNT; i++)
			{
				if (1 == (0x01 & (state >> i)))					//防区触发
				{
					tmp[len++] = '1' + i;
					tmp[len++] = ',';
				}
			}
			tmp[len - 1] = 0;
			strcat(tmp, ")");
			strcat(tmp, get_str(SID_Bj_Touch_Cannot_Bf));
		}
		break;

	case 9:
		strcpy(tmp, get_str(SID_Bj_Alarm_Cannot_Qc));	//正在报警，不能操作
		break;
	}
	AlarmHitMsgBackgroundOnShow(0, tmp);
}

/*************************************************
Function:		AlarmBottomButtonOnMouseUp
Description: 	安防底部按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool AlarmBottomButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint32 ret = 0;
	int bottom_index = atoi(param);

	switch (bottom_index)
	{
	case 0:							// 在家按键
		ShowAlarmKetBordWin();
		break;

	case 1:							// 外出按键
		ret = alarm_set_operator(EXECUTOR_LOCAL_HOST);
		if (AF_SUCCESS == ret)
		{
			show_alarm_main_hit();
			update_area_state(FALSE);
			show_alarm_main_bottom();
		}
		else
		{
			show_alarm_main_bottom();
			ShowAlarmFailHitMsgWin(ret);
		}
		break;

	case 2:							// 夜间按键
		ret = alarm_partset_operator(EXECUTOR_LOCAL_HOST);
		if (AF_SUCCESS == ret)
		{
			show_alarm_main_hit();
			update_area_state(FALSE);
			show_alarm_main_bottom();
		}
		else
		{
			show_alarm_main_bottom();
			ShowAlarmFailHitMsgWin(ret);
		}
		break;

	case 3:							// 清除警示按键
		if (logic_get_g_whole_alarm_state_param())
		{
			ShowAlarmFailHitMsgWin(9);
		}
		else
		{
			alarm_clear_alerts_operator();
			//if (g_pUnreadList->nCount > 0)	
			{
				storage_clear_afbj_unread_state();
				updata_unread_alarm_rec_data();
				sys_sync_hint_state_ext(SYS_HINT_ALARM_WARNING);
			}
		}
		show_alarm_main_hit();
		update_area_state(FALSE);
		show_alarm_main_bottom();
		break;

	case 4:							// 清空按键
		if (ituRadioBoxIsChecked(AlarmRightOperLogRadioBox))
		{
			PAF_CZ_LIST paf_cz_list = NULL;
			paf_cz_list = storage_get_afcz_record();
			if (paf_cz_list && paf_cz_list->nCount > 0)
			{
				AlarmHitMsgBackgroundOnShow(1, get_str(SID_Bj_Alarm_Clear_data));
			}
		}
		else if (ituRadioBoxIsChecked(AlarmRightAlarmLogRadioBox))
		{
			PAF_BJ_LIST  paf_bj_list = NULL;
			paf_bj_list = storage_get_afbj_record();
			if (paf_bj_list && paf_bj_list->nCount > 0)
			{
				AlarmHitMsgBackgroundOnShow(1, get_str(SID_Bj_Alarm_Clear_data));
			}
		}
		break;

	default:
		break;
	}

	return true;
}

/*************************************************
Function:		AlarmRightButtonOnMouseUp
Description: 	安防右边按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool AlarmRightButtonOnMouseUp(ITUWidget* widget, char* param)
{
	int right_index = atoi(param);
	uint8 alarming;

	alarming = logic_get_g_whole_alarm_state_param();

	switch (right_index)
	{
	case 0:							// 安防按键
		ShowAlarmMainWin(TRUE);
		break;

	case 1:							// 报警记录按键
		if (1 == alarming)
		{
			ShowAlarmFailHitMsgWin(9);
			ituRadioBoxSetChecked(AlarmRightSecurityRadioBox, true);
		}
		else
		{
			ShowRecordWin(1);
		}
		break;

	case 2:							// 操作记录按键
		if (1 == alarming)
		{
			ShowAlarmFailHitMsgWin(9);
			ituRadioBoxSetChecked(AlarmRightSecurityRadioBox, true);
		}
		else
		{
			ShowRecordWin(2);
		}
		break;

	default:
		break;
	}

	return true;
}

/*************************************************
Function:		AlarmFqSingleSetButtonOnMouseUp
Description: 	安防单防区按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool AlarmFqSingleSetButtonOnMouseUp(ITUWidget* widget, char* param)
{
	AF_RETURN_TYPE ret;
	uint8 fangqu_index = (uint8)atoi(param);
	
	ret = alarm_single_set_operator(fangqu_index);
	if (AF_SUCCESS == ret)
	{
		show_alarm_main_hit();
		update_area_state(FALSE);
		show_alarm_main_bottom();
	}
	return true;
}

/*************************************************

		消息框界面函数

*************************************************/
/*************************************************
Function:		ShowMsgFailHintSuccessLayer
Description: 	调用消息框处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
param:
spriteflag:		图标显示第几个	0：“X”	1：“！”	
show_text：		显示文字内容
calendarflag:	显示一个按键还是两个按键	0:确认；	1：确认，取消
*************************************************/
static void AlarmHitMsgBackgroundOnShow(uint8 spriteflag, char* show_text)
{
	if (!AlarmHitMsgText)
	{
		AlarmHitMsgText = ituSceneFindWidget(&theScene, "AlarmHitMsgText");
		assert(AlarmHitMsgText);

		AlarmHitMsg0Calendar = ituSceneFindWidget(&theScene, "AlarmHitMsg0Calendar");
		assert(AlarmHitMsg0Calendar);

		AlarmHitMsg1Calendar = ituSceneFindWidget(&theScene, "AlarmHitMsg1Calendar");
		assert(AlarmHitMsg1Calendar);

		AlarmHitMsgSprite = ituSceneFindWidget(&theScene, "AlarmHitMsgSprite");
		assert(AlarmHitMsgSprite);

		AlarmMsgHitSuccess0RadioBox = ituSceneFindWidget(&theScene, "AlarmMsgHitSuccess0RadioBox");
		assert(AlarmMsgHitSuccess0RadioBox);

		AlarmMsgHitSuccess1RadioBox = ituSceneFindWidget(&theScene, "AlarmMsgHitSuccess1RadioBox");
		assert(AlarmMsgHitSuccess1RadioBox);
	}

	ituSpriteGoto(AlarmHitMsgSprite, spriteflag);
	ituTextSetString(AlarmHitMsgText, show_text);
	if (0 == spriteflag)
	{
		ituWidgetSetVisible(AlarmHitMsg1Calendar, false);
		ituWidgetSetVisible(AlarmHitMsg0Calendar, true);
	}
	else
	{
		ituRadioBoxSetChecked(AlarmMsgHitSuccess0RadioBox, false);
		ituRadioBoxSetChecked(AlarmMsgHitSuccess1RadioBox, false);

		ituWidgetSetVisible(AlarmHitMsg0Calendar, false);
		ituWidgetSetVisible(AlarmHitMsg1Calendar, true);
	}

	ituWidgetDisable(AlarmBackground);
	ituWidgetSetVisible(AlarmHitMsgBackground, true);
}

/*************************************************
Function:		MsgHitSuccessRadioBoxOnMouseUp
Description: 	清空消息框确认键按下执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool AlarmMsgHitSuccessRadioBoxOnMouseUp(ITUWidget* widget, char* param)
{
	if (ituRadioBoxIsChecked(AlarmRightOperLogRadioBox))
	{
		storage_clear_afcz_record();
		ShowRecordWin(2);
	}
	else if (ituRadioBoxIsChecked(AlarmRightAlarmLogRadioBox))
	{
		storage_clear_afbj_record();
		updata_unread_alarm_rec_data();
		ShowRecordWin(1);
	}

	ituWidgetEnable(AlarmBackground);
	ituWidgetSetVisible(AlarmHitMsgBackground, false);

	return true;
}

/*************************************************

		安防界面数字键盘函数

*************************************************/
/*************************************************
Function:		ShowAlarmKetBordWin
Description: 	安防界面键盘函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void ShowAlarmKetBordWin()
{
	if (!AlarmKeyBordTextBox)
	{
		AlarmKeyBordTextBox = ituSceneFindWidget(&theScene, "AlarmKeyBordTextBox");
		assert(AlarmKeyBordTextBox);

		AlarmKeyBordHitText = ituSceneFindWidget(&theScene, "AlarmKeyBordHitText");
		assert(AlarmKeyBordHitText);
	}

	AlarmKeyBordTextBox->text.layout = ITU_LAYOUT_MIDDLE_CENTER;
	ituTextBoxSetString(AlarmKeyBordTextBox, NULL);
	ituTextSetString(AlarmKeyBordHitText, get_str(SID_Set_Enter_Alarm_Pwd));

	ituWidgetDisable(AlarmBackground);
	ituWidgetSetVisible(AlarmKeyBordBackground, true);
}

/*************************************************
Function:		AlarmKeyBordButtonOnMouseUp
Description: 	键盘数字按键按下执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool AlarmKeyBordButtonOnMouseUp(ITUWidget* widget, char* param)
{
	ITUButton* btn = (ITUButton*)widget;
	char* input = ituTextGetString(&btn->text);
	char* str_textbox = ituTextGetString(AlarmKeyBordTextBox);
	int count_textbox = str_textbox ? strlen(str_textbox) : 0;
	char* str_text = ituTextGetString(AlarmKeyBordHitText);
	int count_text = str_text ? strlen(str_text) : 0;

	if (count_text > 0)
		ituTextSetString(AlarmKeyBordHitText, "");

	if (count_textbox > AlarmKeyBordTextBox->maxLen)
		ituTextSetString(AlarmKeyBordTextBox, NULL);

	ituTextBoxInput(AlarmKeyBordTextBox, input);

	return true;
}

/*************************************************
Function:		AlarmKeyBordYesButtonOnMouseUp
Description: 	键盘确认键按下执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool AlarmKeyBordYesButtonOnMouseUp(ITUWidget* widget, char* param)
{
	MSG_EVENT event = MSG_EVENT_NO;

	char* TextBox_data = ituTextGetString(AlarmKeyBordTextBox);

	event = msg_pass_oper_deal(PASS_TYPE_USER, 0xff, TextBox_data);

	switch (event)
	{
	case MSG_EVENT_YES:
		if (AF_SUCCESS == alarm_unset_operator(EXECUTOR_LOCAL_HOST, 0))
		{
			show_alarm_main_hit();
			update_area_state(FALSE);
			show_alarm_main_bottom();
		}
		break;

	case MSG_EVENT_XIECHI:
		if (AF_SUCCESS == alarm_unset_operator(EXECUTOR_LOCAL_HOST, 1))
		{
			show_alarm_main_hit();
			update_area_state(FALSE);
			show_alarm_main_bottom();
		}
		break;

	case MSG_EVENT_NO:
		ituTextSetString(AlarmKeyBordTextBox, NULL);
		ituTextSetString(AlarmKeyBordHitText, get_str(SID_Set_Pwd_Err));
		return true;
	}

	if (ituWidgetIsVisible(AlarmKeyBordBackground))
	{
		ituWidgetSetVisible(AlarmKeyBordBackground, false);
		ituWidgetEnable(AlarmBackground);
	}

	return true;
}

/*************************************************
Function:		AlarmKeyBordDianButtonOnMouseUp
Description: 	键盘确取消按下执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool AlarmKeyBordDianButtonOnMouseUp(ITUWidget* widget, char* param)
{
	show_alarm_main_bottom();

	if (ituWidgetIsVisible(AlarmKeyBordBackground))
	{
		ituWidgetSetVisible(AlarmKeyBordBackground, false);
		ituWidgetEnable(AlarmBackground);
	}

	return true;
}

/*************************************************
Function:		AlarmKeyBordDelButtonOnMouseLongPress
Description: 	回删长按执行函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool AlarmKeyBordDelButtonOnMouseLongPress(ITUWidget* widget, char* param)
{
	ituTextBoxSetString(AlarmKeyBordTextBox, NULL);
	// 删除号码完毕，显示输入提示
	ituTextSetString(AlarmKeyBordHitText, get_str(SID_Set_Enter_Alarm_Pwd));

	return true;
}

/*************************************************

				逻辑回调用到的函数

*************************************************/
/*************************************************
Function:		AlarmLayerOnGoto
Description: 	进入安防界面
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool AlarmLayerOnGoto(ITUWidget* widget, char* param)
{
	debug_log("****** GoTo AlarmLayer***********\n");
	if (FALSE == storage_get_extmode(EXT_MODE_ALARM))
	{
		dprintf("alarm mode not open!!\n");
		return FALSE;
	}

	if (!AlarmLayer)
	{
		AlarmLayer = ituSceneFindWidget(&theScene, "AlarmLayer");
		assert(AlarmLayer);
	}

	if (!ituWidgetIsVisible(AlarmLayer))
	{
		ituLayerGoto(AlarmLayer);
		g_show_flag = FALSE;
	}
	else
	{
		if (ituWidgetIsVisible(AlarmKeyBordBackground))
		{
			ituWidgetSetVisible(AlarmKeyBordBackground, false);
		}
		if (ituWidgetIsVisible(AlarmKeyBordBackground))
		{
			ituWidgetSetVisible(AlarmHitMsgBackground, false);
		}
		if (!ituWidgetIsEnabled(AlarmBackground))
		{
			ituWidgetEnable(AlarmBackground);
		}
		updata_unread_alarm_rec_data();
		ShowAlarmMainWin(FALSE);
		ituRadioBoxSetChecked(AlarmRightSecurityRadioBox, true);
	}
	
	debug_log("****** GoTo AlarmLayer End***********\n");

	return true;
}

/*************************************************
Function:		UpdataAlarmLayerOnShow
Description: 	刷新安防八防区界面
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool UpdataAlarmLayerOnShow(ITUWidget* widget, char* param)
{
	debug_log("******Alarm Logic CallBack UI Updata Fangqu***********\n");
	if (!AlarmLayer)
	{
		AlarmLayer = ituSceneFindWidget(&theScene, "AlarmLayer");
		assert(AlarmLayer);
	}
	if (!AlarmMainBackground)
	{
		AlarmMainBackground = ituSceneFindWidget(&theScene, "AlarmMainBackground");
		assert(AlarmMainBackground);
	}

	if (ituWidgetIsVisible(AlarmLayer) && ituWidgetIsVisible(AlarmMainBackground) && (g_show_flag == TRUE))
	{
		show_alarm_main_hit();
		update_area_state(FALSE);
		show_alarm_main_bottom();
	}
	debug_log("******Alarm Logic CallBack UI Updata Fangqu End***********\n");

	return true;
}
