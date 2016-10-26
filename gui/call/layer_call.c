/*************************************************
Copyright (C),  2006-2016, Aurine
File name:		layer_call.c
Author:     	caogw
Version:    	
Date: 			2016-07-05
Description:
History:
1. Date:
Author:
Modification:
*************************************************/
#include "gui_include.h"

/*****************常量定义***********************/
#define MAX_MANAGER_NUM  		3					// 最大管理员机数
#define MAX_FENJI_NUM  			8					// 最大分机数
#define MAX_CENTER_NUM			MAX_FENJI_NUM		// 管理员机、分机最大条数

/*****************变量定义***********************/
static ITULayer* MainLayer = NULL;
static ITULayer* BeCallLayer = NULL;
static ITULayer* CallLayer = NULL;
static ITUBackground* CallRightBackground = NULL;
static ITUTextBox* CallKeyBordTextBox = NULL;
static ITUText* CallKeyBordHitText = NULL;
static ITUKeyboard* CallKeyboard = NULL;
static ITUButton* CallRightExitButton = NULL;
static ITURadioBox* CallRightCenterRadioBox = NULL;
static ITURadioBox* CallRightRecordRadioBox = NULL;
static ITURadioBox* CallRightUserRadioBox = NULL;
static ITURadioBox* CallBottomRecordMissRadioBox = NULL;
static ITUBackground* CallBottomBackground0 = NULL;			// 纯色底部背景
static ITUBackground* CallBottomBackground1 = NULL;			// 通话记录底部背景
static ITUBackground* CallBottomBackground2 = NULL;			// 编辑底部背景
static ITUCoverFlow* CallRecordListCoverFlow = NULL;
static ITUCoverFlow* CallCenterListCoverFlow = NULL;
static ITUText* CallRecordListDevTypeText[LIST_ITEM_COUNT] = { NULL };
static ITUText* CallRecordListTimeText[LIST_ITEM_COUNT] = { NULL };
static ITUSprite* CallRecordListSprite[LIST_ITEM_COUNT] = { NULL };
static ITUContainer* CallRecordListContainer[LIST_ITEM_COUNT] = { NULL };
static ITUButton* CallRecordListButton[LIST_ITEM_COUNT] = { NULL };
static ITUIcon* CallCenterListIcon[LIST_ITEM_COUNT] = { NULL };
static ITUText* CallCenterListDevTypeText[LIST_ITEM_COUNT] = { NULL };
static ITUContainer* CallCenterListContainer[LIST_ITEM_COUNT] = { NULL };
static ITUButton* CallCenterListButton[LIST_ITEM_COUNT] = { NULL };
static ITUText* CallRecordMSGHitText = NULL;
static ITUBackground* CallRecordMSGHitGrayBackground = NULL;
static ITUButton* CallBottomNullButton0 = NULL;
static ITUButton* CallBottomNullButton1 = NULL;

static ITUButton* g_CallRecordListButton[MAX_RECORD_NUM] = { NULL };
static ITUSprite* g_CallRecordListSprite[MAX_RECORD_NUM] = { NULL };
static ITUButton* g_CallCenterListButton[MAX_CENTER_NUM] = { NULL };

/*****************变量定义***********************/
static char g_CallNo[50];							// 呼叫号码
static uint8 g_CurPage = 0;							// 当前页面
static uint8 g_CallType = 0;						// 呼叫类型 呼叫室内
static uint8 g_MaxNoLen = 0;
static uint32 g_HintStrID = 0;						// 提示文字ID
static uint32 g_MainStrID = 0;						// 提示主文字ID
static DEVICE_TYPE_E g_DevType;
static uint8 g_ticks = 0;
static uint8  g_TimerRuning = FALSE;				// 执行定时器
static uint32 g_CallLastTick = 0;					// 实时更新的tick
static PMCALLLISTINFO g_CallRecord;
static CALL_TYPE g_RecordType;
static uint8 g_Event = 0;							// 判断是清空还是删除
static uint8 g_Index[8] = { 0 };					// 管理员机或分机列表
static uint8 g_CenterType = 0;						//  0 管理员机 1 分机
		
typedef enum
{
	CallCenterEvent = 0x00,
	CallUserEvent,
	CallFenJiEvent,
	CallRecordEvent,
	CallRecordMissEvent,
	CallRecordInEvent,
	CallRecordOutEvent,
	CallEditEvent,
	CallDelEvent,
	CallCleanEvent,
	CallExitEvent,
	CallMaxEvent,
}CallButtonEvent;

// 呼叫列表中界面风格
typedef enum
{
	CallCenterPage = 0x00,
	CallUserPage,
	CallRecordPage,
	CallEditPage,
	CallMaxPage,
}CallPage;

// 通话记录尾部小图标类型
typedef enum
{
	CallCallIcon = 0x00,
	CallUnChoseIcon,
	CallChoseIcon,
	CallMaxIcon,
}CallCallIconType;

/*************************************************
Function:		CallLayerPage
Description: 	呼叫界面
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void CallLayerPage(CallPage type)
{
	g_CurPage = type;
	switch (type)
	{
		case CallCenterPage:
			ituWidgetSetVisible(CallKeyboard, false);
			ituWidgetSetVisible(CallCenterListCoverFlow, true);
			ituWidgetSetVisible(CallRecordListCoverFlow, false);
			ituWidgetSetVisible(CallBottomBackground0, true);
			ituWidgetSetVisible(CallBottomBackground1, false);
			ituWidgetSetVisible(CallBottomBackground2, false);
			break;

		case CallUserPage:
			ituWidgetSetVisible(CallKeyboard, true);
			ituWidgetSetVisible(CallCenterListCoverFlow, false);
			ituWidgetSetVisible(CallRecordListCoverFlow, false);
			ituWidgetSetVisible(CallBottomBackground0, true);
			ituWidgetSetVisible(CallBottomBackground1, false);
			ituWidgetSetVisible(CallBottomBackground2, false);
			break;

		case CallRecordPage:
			ituWidgetSetVisible(CallKeyboard, false);
			ituWidgetSetVisible(CallCenterListCoverFlow, false);
			ituWidgetSetVisible(CallRecordListCoverFlow, true);
			ituWidgetSetVisible(CallBottomBackground0, false);
			ituWidgetSetVisible(CallBottomBackground1, true);
			ituWidgetSetVisible(CallBottomBackground2, false);
			break;

		case CallEditPage:
			ituWidgetSetVisible(CallKeyboard, false);
			ituWidgetSetVisible(CallCenterListCoverFlow, false);
			ituWidgetSetVisible(CallRecordListCoverFlow, true);
			ituWidgetSetVisible(CallBottomBackground0, false);
			ituWidgetSetVisible(CallBottomBackground1, false);
			ituWidgetSetVisible(CallBottomBackground2, true);
			break;

		default:
			break;
	}
}

/*************************************************
Function:		SetCallRecordPageDisable
Description: 	设置通话记录页面使能
Input:			无
Output:			无
Return:			无
Others:			为了使有消息框时候按不动
*************************************************/
static void SetCallRecordPageDisable(uint8 type)
{
	if (type == true)
	{
		ituWidgetSetVisible(CallRecordMSGHitGrayBackground, true);
		ituWidgetDisable(CallRecordListCoverFlow);
		ituWidgetDisable(CallBottomBackground2);
		ituWidgetDisable(CallRightBackground);
	}
	else
	{
		ituWidgetSetVisible(CallRecordMSGHitGrayBackground, false);
		ituWidgetEnable(CallRecordListCoverFlow);
		ituWidgetEnable(CallBottomBackground2);
		ituWidgetEnable(CallRightBackground);
	}
}

/*************************************************
Function:		WhetherMissUnRead
Description: 	判断是否有未读的未接记录
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static uint8 WhetherMissUnRead(void)
{
	uint8 i, max = 0;

	if (g_CallRecord)
	{
		if (g_CallRecord->CallInfo)
		{
			free(g_CallRecord->CallInfo);
			g_CallRecord->CallInfo = NULL;
		}
		free(g_CallRecord);
		g_CallRecord = NULL;
	}

	g_CallRecord = storage_get_callrecord(MISSED);
	if (g_CallRecord == NULL)
	{
		printf("g_CallRecord is NULL\n");
		return FALSE;
	}

	if (g_CallRecord && g_CallRecord->CallCount > 0)
	{
		max = g_CallRecord->CallCount;
		if (max > MAX_RECORD_NUM)
		{
			max = MAX_RECORD_NUM;
		}

		for (i = 0; i < max; i++)
		{
			if (g_CallRecord->CallInfo[i].UnRead)
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

/*************************************************
Function:		ShowRecordWin
Description: 	显示通话记录具体信息
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void ShowRecordWin(CALL_TYPE Calltype)
{
	uint32 ypos = 0;
	uint8 i, max = 0, count = 0, result = 0;
	char devtype[50], timestr[50], tmp[50];
	ITUButton* CloneChildCallRecordListButton;
	ITUText* CloneChildCallRecordListDevTypeText;
	ITUText* CloneChildCallRecordListTimeText;
	ITUSprite* CloneChildCallRecordListSprite;
	ITUContainer* CloneCallRecordListContainer;
	ITUContainer* OldCallRecordListContainer;
	
	if (g_CallRecord)
	{
		if (g_CallRecord->CallInfo)
		{
			free(g_CallRecord->CallInfo);
			g_CallRecord->CallInfo = NULL;
		}
		free(g_CallRecord);
		g_CallRecord = NULL;
	}

	// 删除已经Clone的List
	for (i = LIST_ITEM_COUNT; i < MAX_RECORD_NUM; i++)
	{
		if (NULL != g_CallRecordListButton[i])
		{
			g_CallRecordListButton[i] = NULL;
			OldCallRecordListContainer = NULL;
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "CallRecordListContainer", i);
			OldCallRecordListContainer = (ITUContainer*)ituSceneFindWidget(&theScene, tmp);
			itcTreeRemove(OldCallRecordListContainer);						// 删除已有的
			ituWidgetExit(OldCallRecordListContainer);
			dprintf("Remove CallRecordListContainer List!!!\n");
		}

	}
	g_RecordType = Calltype;
	g_CallRecord = storage_get_callrecord(Calltype);
	if (g_CallRecord == NULL)
	{
		dprintf("g_CallRecord is NULL\n");
		return;
	}
	// 默认显示从第一条开始显示
	ituCoverFlowGoto(CallRecordListCoverFlow, 0);

	max = g_CallRecord->CallCount;
	if (max > MAX_RECORD_NUM)
	{
		max = MAX_RECORD_NUM;
	}

	if (max <= MAX_PAGE_NUM)
	{
		count = MAX_PAGE_NUM;
	}
	else
	{
		count = max;
	}

	for (i = 0; i < count; i++)
	{
		CloneChildCallRecordListButton = NULL;
		CloneChildCallRecordListDevTypeText = NULL;
		CloneChildCallRecordListTimeText = NULL;
		CloneChildCallRecordListSprite = NULL;
		CloneCallRecordListContainer = NULL;

		if (i < LIST_ITEM_COUNT)
		{
			// 前2条为Clone模板
			CloneChildCallRecordListButton = CallRecordListButton[i];
			CloneChildCallRecordListDevTypeText = CallRecordListDevTypeText[i];
			CloneChildCallRecordListTimeText = CallRecordListTimeText[i];
			CloneChildCallRecordListSprite = CallRecordListSprite[i];

			// 模板初始化
			ituWidgetEnable(CloneChildCallRecordListButton);
			ituWidgetSetVisible(CloneChildCallRecordListSprite, true);
			ituTextSetString(CloneChildCallRecordListDevTypeText, "");
			ituTextSetString(CloneChildCallRecordListTimeText, "");
		}
		else
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "CallRecordListContainer", i);
			result = ituWidgetClone(CallRecordListContainer[1], &CloneCallRecordListContainer);
			if (result)
			{
				ituWidgetSetName(CloneCallRecordListContainer, tmp);
				ituWidgetSetX(CloneCallRecordListContainer, 0);
				if (i < MAX_PAGE_NUM)
				{
					ypos = i * 62;
				}
				else
				{
					ypos = 0;
				}
				ituWidgetSetY(CloneCallRecordListContainer, ypos);

				memset(tmp, 0, sizeof(tmp));
				CloneChildCallRecordListButton = itcTreeGetChildAt(CloneCallRecordListContainer, 0);
				sprintf(tmp, "%s%d", "CallRecordListButton", i);
				ituWidgetSetName(CloneChildCallRecordListButton, tmp);

				memset(tmp, 0, sizeof(tmp));
				CloneChildCallRecordListDevTypeText = itcTreeGetChildAt(CloneCallRecordListContainer, 1);
				sprintf(tmp, "%s%d", "CallRecordListDevTypeText", i);
				ituWidgetSetName(CloneChildCallRecordListDevTypeText, tmp);

				memset(tmp, 0, sizeof(tmp));
				CloneChildCallRecordListTimeText = itcTreeGetChildAt(CloneCallRecordListContainer, 2);
				sprintf(tmp, "%s%d", "CallRecordListTimeText", i);
				ituWidgetSetName(CloneChildCallRecordListTimeText, tmp);

				memset(tmp, 0, sizeof(tmp));
				CloneChildCallRecordListSprite = itcTreeGetChildAt(CloneCallRecordListContainer, 3);
				sprintf(tmp, "%s%d", "CallRecordListSprite", i);
				ituWidgetSetName(CloneChildCallRecordListSprite, tmp);

				ituWidgetAdd(CallRecordListCoverFlow, CloneCallRecordListContainer);
			}
			ituCoverFlowUpdate((ITUWidget*)CallRecordListCoverFlow, ITU_EVENT_LAYOUT, 0, 0, 0);
		}
		// 记录按键指针、Sprite指针
		g_CallRecordListButton[i] = CloneChildCallRecordListButton;
		g_CallRecordListSprite[i] = CloneChildCallRecordListSprite;

		if (i < max)
		{
			memset(devtype, 0, sizeof(devtype));
			memset(timestr, 0, sizeof(timestr));

			get_dev_description(g_CallRecord->CallInfo[i].Type, g_CallRecord->CallInfo[i].devno, devtype, 50);
			sprintf(timestr, "%04d-%02d-%02d %02d:%02d:%02d", g_CallRecord->CallInfo[i].Time.year, g_CallRecord->CallInfo[i].Time.month, g_CallRecord->CallInfo[i].Time.day,
				g_CallRecord->CallInfo[i].Time.hour, g_CallRecord->CallInfo[i].Time.min, g_CallRecord->CallInfo[i].Time.sec);

			// 初始化为白色字体
			ituSetColor(&((ITUWidget*)CloneChildCallRecordListDevTypeText)->color, 255, 255, 255, 255);
			ituSetColor(&((ITUWidget*)CloneChildCallRecordListTimeText)->color, 255, 255, 255, 255);

			// 如果是未接来电，且未读则字体颜色改为红色
			if (Calltype == MISSED)
			{
				if (g_CallRecord->CallInfo[i].UnRead)
				{
					g_CallRecord->CallInfo[i].UnRead = 0;
					ituSetColor(&((ITUWidget*)CloneChildCallRecordListDevTypeText)->color, 255, 255, 0, 0);
					ituSetColor(&((ITUWidget*)CloneChildCallRecordListTimeText)->color, 255, 255, 0, 0);
				}
			}

			ituTextSetString(CloneChildCallRecordListDevTypeText, devtype);
			ituTextSetString(CloneChildCallRecordListTimeText, timestr);

			// 如果处于编辑状态尾部小图标应改为未选中状态
			if (g_CurPage == CallRecordPage)
			{
				if (g_CallRecord->CallInfo[i].Type == DEVICE_TYPE_AREA || g_CallRecord->CallInfo[i].Type == DEVICE_TYPE_STAIR
					|| g_CallRecord->CallInfo[i].Type == DEVICE_TYPE_DOOR_NET)
				{
					ituWidgetSetVisible(CloneChildCallRecordListSprite, false);
				}
				else
				{
					ituWidgetSetVisible(CloneChildCallRecordListSprite, true);
					ituSpriteGoto(CloneChildCallRecordListSprite, CallCallIcon);
				}
			}
			else if (g_CurPage == CallEditPage)
			{
				ituWidgetSetVisible(CloneChildCallRecordListSprite, true);
				ituSpriteGoto(CloneChildCallRecordListSprite, CallUnChoseIcon);
			}
		}
		else
		{
			ituWidgetDisable(CloneChildCallRecordListButton);
			ituWidgetSetVisible(CloneChildCallRecordListSprite, false);
			ituTextSetString(CloneChildCallRecordListDevTypeText, "");
			ituTextSetString(CloneChildCallRecordListTimeText, "");
		}
	}

	if (Calltype == MISSED)
	{
		storage_set_callrecord_state();
		sys_sync_hint_state_ext(SYS_HINT_MISSED_CALLS);
	}
}

/*************************************************
Function:		ShowCenterWin
Description: 	显示管理员机或分机列表
Input:
1.type			0 管理员机列表 1 分机列表
Output:			无
Return:			无
Others:			无
*************************************************/
static void ShowCenterWin(uint8 type)
{
	uint32 ypos = 0;
	uint8 i, max = 0, count = 0;
	uint8 result = 0, managerflag = 0;
	char devtype[50], devno[5], tmp[50];
	ITUButton* CloneChildCallCenterListButton;
	ITUText* CloneChildCallCenterListDevTypeText;
	ITUIcon* CloneChildCallCenterListIcon;
	ITUContainer* CloneCallCenterListContainer;
	ITUContainer* OldCallCenterListContainer;

	// 删除已经Clone的List
	for (i = LIST_ITEM_COUNT; i < MAX_CENTER_NUM; i++)
	{
		if (NULL != g_CallCenterListButton[i])
		{
			g_CallCenterListButton[i] = NULL;
			OldCallCenterListContainer = NULL;
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "CallCenterListContainer", i);
			OldCallCenterListContainer = (ITUContainer*)ituSceneFindWidget(&theScene, tmp);
			itcTreeRemove(OldCallCenterListContainer);						// 删除已有的
			ituWidgetExit(OldCallCenterListContainer);
			dprintf("Remove CallCenterListContainer List!!!\n");
		}
	}
	// 默认显示从第一条开始显示
	ituCoverFlowGoto(CallCenterListCoverFlow, 0);
	g_CenterType = type;
	memset(g_Index, 0, sizeof(g_Index));

	if (type == 0)
	{
		// 管理员机
		for (i = 0; i < MAX_MANAGER_NUM; i++)
		{
			if (storage_get_manager_ip(i + 1))
			{
				g_Index[max] = i + MANAGER_NUM + 1;
				max++;
			}
		}
		
		if (0 == max)
		{
			max = 1;
			managerflag = 1;
		}
	}
	else
	{
		// 分机
		for (i = 0; i < MAX_FENJI_NUM; i++)
		{
			if (storage_get_subdev_ip(i))
			{
				g_Index[max] = i;
				max++;
			}
		}
	}

	if (max <= MAX_PAGE_NUM)
	{
		count = MAX_PAGE_NUM;
	}
	else
	{
		count = max;
	}

	for (i = 0; i < count; i++)
	{
		CloneChildCallCenterListButton = NULL;
		CloneChildCallCenterListDevTypeText = NULL;
		CloneChildCallCenterListIcon = NULL;
		CloneCallCenterListContainer = NULL;

		if (i < LIST_ITEM_COUNT)
		{
			CloneChildCallCenterListButton = CallCenterListButton[i];
			CloneChildCallCenterListDevTypeText = CallCenterListDevTypeText[i];
			CloneChildCallCenterListIcon = CallCenterListIcon[i];

			// 模板初始化
			ituWidgetEnable(CloneChildCallCenterListButton);
			ituWidgetSetVisible(CloneChildCallCenterListIcon, true);
			ituTextSetString(CloneChildCallCenterListDevTypeText, "");
		}
		else
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "CallCenterListContainer", i);
			result = ituWidgetClone(CallCenterListContainer[1], &CloneCallCenterListContainer);
			if (result)
			{
				ituWidgetSetName(CloneCallCenterListContainer, tmp);
				ituWidgetSetX(CloneCallCenterListContainer, 0);
				if (i < MAX_PAGE_NUM)
				{
					ypos = i * 62;
				}
				else
				{
					ypos = 0;
				}
				ituWidgetSetY(CloneCallCenterListContainer, ypos);

				memset(tmp, 0, sizeof(tmp));
				CloneChildCallCenterListButton = itcTreeGetChildAt(CloneCallCenterListContainer, 0);
				sprintf(tmp, "%s%d", "CallCenterListButton", i);
				ituWidgetSetName(CloneChildCallCenterListButton, tmp);

				memset(tmp, 0, sizeof(tmp));
				CloneChildCallCenterListDevTypeText = itcTreeGetChildAt(CloneCallCenterListContainer, 1);
				sprintf(tmp, "%s%d", "CallCenterListDevTypeText", i);
				ituWidgetSetName(CloneChildCallCenterListDevTypeText, tmp);

				memset(tmp, 0, sizeof(tmp));
				CloneChildCallCenterListIcon = itcTreeGetChildAt(CloneCallCenterListContainer, 2);
				sprintf(tmp, "%s%d", "CallCenterListIcon", i);
				ituWidgetSetName(CloneChildCallCenterListIcon, tmp);

				ituWidgetAdd(CallCenterListCoverFlow, CloneCallCenterListContainer);
			}
			ituCoverFlowUpdate((ITUWidget*)CallCenterListCoverFlow, ITU_EVENT_LAYOUT, 0, 0, 0);
		}
		g_CallCenterListButton[i] = CloneChildCallCenterListButton;

		if (i < max)
		{
			if (type == 0)
			{
				if (managerflag)
				{
					g_Index[0] = 0xFF;
					ituTextSetString(CloneChildCallCenterListDevTypeText, get_str(SID_DevManager));
				}
				else
				{
					memset(devno, 0, sizeof(devno));
					memset(devtype, 0, sizeof(devtype));

					sprintf(devno, "%d", g_Index[i]);
					get_dev_description(DEVICE_TYPE_MANAGER, devno, devtype, 50);
					ituTextSetString(CloneChildCallCenterListDevTypeText, devtype);
				}
			}
			else
			{
				memset(devno, 0, sizeof(devno));
				memset(devtype, 0, sizeof(devtype));

				sprintf(devno, "%d", g_Index[i]);
				get_dev_description(DEVICE_TYPE_FENJI_NET, devno, devtype, 50);
				ituTextSetString(CloneChildCallCenterListDevTypeText, devtype);
			}
		}
		else
		{
			ituWidgetDisable(CloneChildCallCenterListButton);
			ituWidgetSetVisible(CloneChildCallCenterListIcon, false);
			ituTextSetString(CloneChildCallCenterListDevTypeText, "");
		}
	}
}

/*************************************************
Function:		SetCalloutInfo
Description: 	输入号码界面信息初始化
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void SetCalloutInfo(CallButtonEvent event)
{
	PFULL_DEVICE_NO pDevParam = storage_get_devparam();
	g_MaxNoLen = 0;
	g_MainStrID = 0;
	g_HintStrID = 0;
	g_CallLastTick = 0;
	g_CallType = DEVICE_TYPE_NONE;
	memset(g_CallNo, 0, sizeof(g_CallNo));
	ituTextSetString(CallKeyBordHitText, NULL);
	ituTextBoxSetString(CallKeyBordTextBox, NULL);

	if (event == CallUserEvent)
	{
		g_CallType = DEVICE_TYPE_ROOM;
	}
	else if (event == CallFenJiEvent)
	{
		g_CallType = DEVICE_TYPE_FENJI_NET;
	}

	if (g_CallType == DEVICE_TYPE_ROOM)				// 呼叫住户输入号码最大值
	{
		g_MaxNoLen = pDevParam->DevNoLen - 1;
		g_MainStrID = SID_Inter_EnterUserNo;

	}
	else if (g_CallType == DEVICE_TYPE_FENJI_NET)	// 呼叫分机输入号码最大值
	{
		g_MaxNoLen = 1;
		g_MainStrID = SID_Inter_EnterFJNo;
	}
	CallKeyBordTextBox->maxLen = g_MaxNoLen;
	ituTextSetString(CallKeyBordHitText, get_str(g_MainStrID));
}

/*************************************************
Function:		WhetherDelRecord
Description: 	判断是否删除通话记录
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static uint8 WhetherDelRecord(void)
{
	uint8 i;

	for (i = 0; i < g_CallRecord->CallCount; i++)
	{
		if (CallChoseIcon == g_CallRecordListSprite[i]->frame)
		{
			return TRUE;
		}
	}

	return FALSE;
}

/*************************************************
Function:		SetRecordChose
Description: 	设置通话记录为选择状态
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void SetRecordChose(void)
{
	uint8 i;

	for (i = 0; i < g_CallRecord->CallCount; i++)
	{
		ituSpriteGoto(g_CallRecordListSprite[i], CallChoseIcon);
	}
}

/*************************************************
Function:		CheckCallNOValid
Description: 	检测输入号码合法性
Input:
1.CallNo		输入号码
Output:			无
Return:			TRUE / FALSE
Others:			只做长度检测
*************************************************/
static uint8 CheckCallNOValid(char *CallNo)
{
	int8 len;

	if (CallNo == NULL)
	{
		dprintf("CallNo is null\n");
		return FALSE;
	}

	len = strlen(CallNo);
	if (len <= 0)
	{
		dprintf("CallNo is len : %d\n", len);
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

/*************************************************
Function:		CalloutEventStart
Description: 	开始呼叫
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void CalloutEventStart(void)
{
	uint8 ret = FALSE;

	ret = CheckCallNOValid(g_CallNo);

	if (ret == FALSE)
	{
		g_HintStrID = SID_Inter_WrongNo;
		ituTextSetString(CallKeyBordHitText, get_str(g_HintStrID));
	}
	else
	{
		if (g_CallType == DEVICE_TYPE_ROOM)
		{
			ret = inter_call_resident(FALSE, g_CallNo);
		}
		else if (g_CallType == DEVICE_TYPE_FENJI_NET)
		{
			ret = inter_call_sub_terminal(g_CallNo);
		}
		if (ret == 0)
		{
			g_HintStrID = SID_Inter_Connecting;
			ituTextSetString(CallKeyBordHitText, get_str(g_HintStrID));
			call_out();			// 启动主叫线程
		}
		else if (ret == 1)
		{
			g_HintStrID = SID_Inter_WrongNo;
			ituTextSetString(CallKeyBordHitText, get_str(g_HintStrID));
		}
		else
		{
			g_HintStrID = SID_Dev_Busy;
			ituTextSetString(CallKeyBordHitText, get_str(g_HintStrID));
			dprintf("media is not null\n");
		}
	}
}

/*************************************************
Function:		CallCallRequestState
Description: 	呼叫请求回调执行函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool CallCallRequestState(ITUWidget* widget, char* param)
{
	uint32 temp = 0;
	INTER_INFO_S CallInfo = { 0 };
	PINTER_CALLBACK pcallbak_data = (PINTER_CALLBACK)param;

	dprintf("state : %d\n", pcallbak_data->InterState);
	switch (pcallbak_data->InterState)
	{
		case CALL_STATE_REQUEST:
			if (pcallbak_data->DataLen != 1)
			{
				CallInfo.InterType = INTER_CALLOUT_E;
				CallInfo.DevType = DEVICE_TYPE_ROOM;
				strcpy(CallInfo.DevStr, pcallbak_data->Buf);
				BeCallWin(&CallInfo);
			}
			break;

		case CALL_STATE_END:
			temp = atoi(pcallbak_data->Buf);
			if (temp == END_BY_REQUESET_ERR)
			{
				g_HintStrID = SID_Inter_NoNotFound;
				ituTextSetString(CallKeyBordHitText, get_str(g_HintStrID));
			}
			else
			{
				g_HintStrID = 0;
			}
			break;

		default:
			break;
	}

	return true;
}

/*************************************************
Function:		CallLayerOnLeave
Description: 	退出执行函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool CallLayerOnLeave(ITUWidget* widget, char* param)
{
	if (g_CallRecord)
	{
		if (g_CallRecord->CallInfo)
		{
			free(g_CallRecord->CallInfo);
			g_CallRecord->CallInfo = NULL;
		}
		free(g_CallRecord);
		g_CallRecord = NULL;
	}

	return true;
}

/*************************************************
Function:		CallLayerOnTimer
Description: 	定时器
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool CallLayerOnTimer(ITUWidget* widget, char* param)
{
	if (g_TimerRuning)
	{
		uint32 tick = SDL_GetTicks();
		uint32 diff = tick - g_CallLastTick;
		if (diff >= 1000)
		{
			g_CallLastTick = tick;
			// 输入号码错误、查无此号 两个提示显示2s后显示输入号码提示或输入号码
			if (g_HintStrID == SID_Inter_WrongNo || g_HintStrID == SID_Inter_NoNotFound || g_HintStrID == SID_Dev_Busy)
			{
				if (g_ticks == 2)
				{
					g_ticks = 0;
					g_HintStrID = 0;
					memset(g_CallNo, 0, sizeof(g_CallNo));
					ituTextBoxSetString(CallKeyBordTextBox, NULL);
					ituTextSetString(CallKeyBordHitText, get_str(g_MainStrID));
				}
				g_ticks++;
				return true;
			}
		}
	}

	return false;
}

/*************************************************
Function:		CallKeyBordButtonOnMouseUp
Description: 	数字按键按下执行函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool CallKeyBordButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 len = 0;
	char* input_text = NULL;
	ITUButton* num_btn = (ITUButton*)widget;
	input_text = ituTextGetString(&num_btn->text);

	len = strlen(g_CallNo);
	if (len >= g_MaxNoLen)
	{
		return true;
	}
	else
	{
		if (len == 0)
		{
			g_HintStrID = 0;
			ituTextSetString(CallKeyBordHitText, NULL);
		}
		g_CallNo[len] = *input_text;
		g_CallNo[len + 1] = 0;
	}
	ituTextBoxInput(CallKeyBordTextBox, input_text);
	dprintf("add len...: %d g_CallNo...: %s\n",len, g_CallNo);

	return true;
}

/*************************************************
Function:		CallKeyBordDelButtonOnMouseUp
Description: 	回删按键执行函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool CallKeyBordDelButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 len = 0;

	len = strlen(g_CallNo);
	if (len > 0)
	{
		g_CallNo[len - 1] = 0;
	}

	len = strlen(g_CallNo);
	if (len == 0)
	{
		// 删除号码完毕，显示输入提示
		ituTextSetString(CallKeyBordHitText, get_str(g_MainStrID));
	}
	dprintf("del len...: %d g_CallNo...: %s\n", len, g_CallNo);

	return true;
}
/*************************************************
Function:		CallKeyBordDelButtonOnMouseLongPress
Description: 	回删长按执行函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool CallKeyBordDelButtonOnMouseLongPress(ITUWidget* widget, char* param)
{
	memset(g_CallNo, 0, sizeof(g_CallNo));
	ituTextBoxSetString(CallKeyBordTextBox, NULL);
	// 删除号码完毕，显示输入提示
	ituTextSetString(CallKeyBordHitText, get_str(g_MainStrID));

	return true;
}

/*************************************************
Function:		CallKeyBordCalloutButtonOnMouseUp
Description: 	呼出按键执行函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool CallKeyBordCalloutButtonOnMouseUp(ITUWidget* widget, char* param)
{
	g_CallLastTick = SDL_GetTicks();
	ituTextBoxSetString(CallKeyBordTextBox, NULL);
	CalloutEventStart();

	return true;
}

/*************************************************
Function:		CallRecordListButtonOnMouseUp
Description: 	通话记录按下执行函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool CallRecordListButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 i;

	for (i = 0; i < g_CallRecord->CallCount; i++)
	{
		if (g_CallRecordListButton[i] == (ITUButton*)widget)
		{
			if (g_CurPage == CallEditPage)
			{
				if (CallChoseIcon == g_CallRecordListSprite[i]->frame)
				{
					ituSpriteGoto(g_CallRecordListSprite[i], CallUnChoseIcon);
				}
				else
				{
					ituSpriteGoto(g_CallRecordListSprite[i], CallChoseIcon);
				}
			}
			else
			{
				if (g_CallRecord->CallInfo[i].Type == DEVICE_TYPE_MANAGER || g_CallRecord->CallInfo[i].Type == DEVICE_TYPE_ROOM ||
					g_CallRecord->CallInfo[i].Type == DEVICE_TYPE_FENJI_NET)
				{
					if (g_CallRecord->CallInfo[i].Type == DEVICE_TYPE_ROOM)
					{
						PFULL_DEVICE_NO dev = storage_get_devparam();

						g_TimerRuning = TRUE;
						SetCalloutInfo(CallUserEvent);
						CallLayerPage(CallUserPage);
						ituRadioBoxSetChecked(CallRightUserRadioBox, true);
						strncpy(g_CallNo, g_CallRecord->CallInfo[i].devno, dev->DevNoLen - 1);
						g_CallLastTick = SDL_GetTicks();
						ituTextBoxSetString(CallKeyBordTextBox, NULL);
						CalloutEventStart();
					}
					else
					{
						INTER_INFO_S CallInfo = { 0 };

						CallInfo.InterType = INTER_CALLOUT_E;
						if (g_CallRecord->CallInfo[i].Type == DEVICE_TYPE_MANAGER)
						{
							CallInfo.DevType = DEVICE_TYPE_MANAGER;
						}
						else
						{
							CallInfo.DevType = DEVICE_TYPE_FENJI_NET;
						}
						strcpy(CallInfo.DevStr, g_CallRecord->CallInfo[i].devno);
						BeCallWin(&CallInfo);
					}
				}
			}
			break;
		}
	}

	return true;
}

/*************************************************
Function:		CallCenterListButtonOnMouseUp
Description: 	管理员机、分机按下执行函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool CallCenterListButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 i;
	INTER_INFO_S CallInfo = { 0 };

	for (i = 0; i < MAX_CENTER_NUM; i++)
	{
		if (g_CallCenterListButton[i] == (ITUButton*)widget)
		{
			int32 ret = ui_show_win_arbitration(SYS_OPER_CALLOUT);
			if (ret == TRUE)
			{
				CallInfo.InterType = INTER_CALLOUT_E;
				if (g_CenterType == 0)
				{
					CallInfo.DevType = DEVICE_TYPE_MANAGER;
					sprintf(CallInfo.DevStr, "%d", g_Index[i]);
					BeCallWin(&CallInfo);
				}
				else
				{
					CallInfo.DevType = DEVICE_TYPE_FENJI_NET;
					sprintf(CallInfo.DevStr, "%d", g_Index[i]);
					BeCallWin(&CallInfo);
				}
			}
			break;
		}
	}
	
	return true;
}

/*************************************************
Function:		CallRecordMSGButtonOnMouseUp
Description: 	消息框按键按下执行函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool CallRecordMSGButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 index = atoi(param);
	
	if (index)
	{
		if (g_Event == CallDelEvent)
		{
			uint8 i;
			DEL_LIST DelList;

			memset(&DelList, 0, sizeof(DEL_LIST));
			for (i = 0; i < g_CallRecord->CallCount; i++)
			{
				if (CallChoseIcon == g_CallRecordListSprite[i]->frame)
				{
					DelList.DelFlg[i] = 1;
					DelList.Counts++;
				}
			}
			dprintf("DelList.Counts : %d\n", DelList.Counts);
			storage_del_callrecord(g_RecordType, &DelList);
		}
		else if (g_Event == CallCleanEvent)
		{
			storage_clear_callrecord(g_RecordType);
		}
		ShowRecordWin(g_RecordType);
	}
	SetCallRecordPageDisable(false);

	return true;
}

/*************************************************
Function:		CallLayerbButtonOnMouseUp
Description: 	呼叫界面右键弹起函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool CallLayerButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 res = FALSE;
	CALL_TYPE calltype;
	uint8 btn_event = atoi(param);

	if (btn_event == CallUserEvent)
	{
		g_TimerRuning = TRUE;
	}
	else
	{
		g_TimerRuning = FALSE;
	}

	dprintf("btn_event: %d\n", btn_event);
	switch (btn_event)
	{
		case CallCenterEvent:
			CallLayerPage(CallCenterPage);
			ShowCenterWin(0);
			break;

		case CallFenJiEvent:
			CallLayerPage(CallCenterPage);
			ShowCenterWin(1);
			break;

		case CallUserEvent:
			SetCalloutInfo(btn_event);
			CallLayerPage(CallUserPage);
			break;

		case CallRecordEvent:
			ituRadioBoxSetChecked(CallBottomRecordMissRadioBox, true);
			CallLayerPage(CallRecordPage);
			ShowRecordWin(MISSED);
			break;

		case CallRecordMissEvent:
		case CallRecordInEvent:
		case CallRecordOutEvent:
			calltype = btn_event - CallRecordMissEvent;
			CallLayerPage(CallRecordPage);
			ShowRecordWin(calltype);
			break;

		case CallEditEvent:
			if (g_CallRecord && g_CallRecord->CallCount)
			{
				CallLayerPage(CallEditPage);
				ShowRecordWin(g_RecordType);
			}
			break;

		case CallDelEvent:
			ituTextSetString(CallRecordMSGHitText, get_str(SID_Bj_Query_Del_Rec_One));
			res = WhetherDelRecord();
			if (res)
			{
				SetCallRecordPageDisable(true);
				g_Event = CallDelEvent;
			}
			break;

		case CallCleanEvent:
			SetRecordChose();
			ituTextSetString(CallRecordMSGHitText, get_str(SID_Bj_Query_Del_Rec_All));
			SetCallRecordPageDisable(true);
			g_Event = CallCleanEvent;
			break;

		case CallExitEvent:
			if (ituWidgetIsVisible(CallBottomBackground2))
			{
				ituRadioBoxSetChecked(CallBottomRecordMissRadioBox, true);
				CallLayerPage(CallRecordPage);
				ShowRecordWin(MISSED);
			}
			else
			{
				ituLayerGoto(MainLayer);
			}
			break;

		default:
			break;
	}
	return true;
}

/*************************************************
Function:		CallLayerOnEnter
Description: 	呼叫界面初始化处理函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool CallLayerOnEnter(ITUWidget* widget, char* param)
{
	if (!CallLayer)
	{
		uint8 i;
		char callname[50];

		CallLayer = ituSceneFindWidget(&theScene, "CallLayer");
		assert(CallLayer);

		MainLayer = ituSceneFindWidget(&theScene, "mainLayer");
		assert(MainLayer);

		BeCallLayer = ituSceneFindWidget(&theScene, "BeCallLayer");
		assert(BeCallLayer);

		CallRightBackground = ituSceneFindWidget(&theScene, "CallRightBackground");
		assert(CallRightBackground);

		CallRightCenterRadioBox = ituSceneFindWidget(&theScene, "CallRightCenterRadioBox");
		assert(CallRightCenterRadioBox);

		CallRightUserRadioBox = ituSceneFindWidget(&theScene, "CallRightUserRadioBox");
		assert(CallRightUserRadioBox);

		CallRightRecordRadioBox = ituSceneFindWidget(&theScene, "CallRightRecordRadioBox");
		assert(CallRightRecordRadioBox);
		
		CallBottomRecordMissRadioBox = ituSceneFindWidget(&theScene, "CallBottomRecordMissRadioBox");
		assert(CallBottomRecordMissRadioBox);
		
		CallRightExitButton = ituSceneFindWidget(&theScene, "CallRightExitButton");
		assert(CallRightExitButton);
		
		CallBottomBackground0 = ituSceneFindWidget(&theScene, "CallBottomBackground0");
		assert(CallBottomBackground0);

		CallBottomBackground1 = ituSceneFindWidget(&theScene, "CallBottomBackground1");
		assert(CallBottomBackground1);

		CallBottomBackground2 = ituSceneFindWidget(&theScene, "CallBottomBackground2");
		assert(CallBottomBackground2);

		CallKeyBordTextBox = ituSceneFindWidget(&theScene, "CallKeyBordTextBox");
		assert(CallKeyBordTextBox);

		CallKeyBordHitText = ituSceneFindWidget(&theScene, "CallKeyBordHitText");
		assert(CallKeyBordHitText);

		CallKeyboard = ituSceneFindWidget(&theScene, "CallKeyboard");
		assert(CallKeyboard);

		CallRecordListCoverFlow = ituSceneFindWidget(&theScene, "CallRecordListCoverFlow");
		assert(CallRecordListCoverFlow);

		CallCenterListCoverFlow = ituSceneFindWidget(&theScene, "CallCenterListCoverFlow");
		assert(CallKeyboard);

		CallRecordMSGHitText = ituSceneFindWidget(&theScene, "CallRecordMSGHitText");
		assert(CallRecordMSGHitText);

		CallRecordMSGHitGrayBackground = ituSceneFindWidget(&theScene, "CallRecordMSGHitGrayBackground");
		assert(CallRecordMSGHitGrayBackground);

		for (i = 0; i < LIST_ITEM_COUNT; i++)
		{
			memset(callname, 0, sizeof(callname));
			sprintf(callname, "%s%d", "CallRecordListContainer", i);
			CallRecordListContainer[i] = ituSceneFindWidget(&theScene, callname);
			assert(CallRecordListContainer[i]);

			memset(callname, 0, sizeof(callname));
			sprintf(callname, "%s%d", "CallRecordListButton", i);
			CallRecordListButton[i] = ituSceneFindWidget(&theScene, callname);
			assert(CallRecordListButton[i]);

			memset(callname, 0, sizeof(callname));
			sprintf(callname, "%s%d", "CallRecordListDevTypeText", i);
			CallRecordListDevTypeText[i] = ituSceneFindWidget(&theScene, callname);
			assert(CallRecordListDevTypeText[i]);

			memset(callname, 0, sizeof(callname));
			sprintf(callname, "%s%d", "CallRecordListTimeText", i);
			CallRecordListTimeText[i] = ituSceneFindWidget(&theScene, callname);
			assert(CallRecordListTimeText[i]);

			memset(callname, 0, sizeof(callname));
			sprintf(callname, "%s%d", "CallRecordListSprite", i);
			CallRecordListSprite[i] = ituSceneFindWidget(&theScene, callname);
			assert(CallRecordListSprite[i]);

			memset(callname, 0, sizeof(callname));
			sprintf(callname, "%s%d", "CallCenterListContainer", i);
			CallCenterListContainer[i] = ituSceneFindWidget(&theScene, callname);
			assert(CallCenterListContainer[i]);

			memset(callname, 0, sizeof(callname));
			sprintf(callname, "%s%d", "CallCenterListButton", i);
			CallCenterListButton[i] = ituSceneFindWidget(&theScene, callname);
			assert(CallCenterListButton[i]);

			memset(callname, 0, sizeof(callname));
			sprintf(callname, "%s%d", "CallCenterListDevTypeText", i);
			CallCenterListDevTypeText[i] = ituSceneFindWidget(&theScene, callname);
			assert(CallCenterListDevTypeText[i]);

			memset(callname, 0, sizeof(callname));
			sprintf(callname, "%s%d", "CallCenterListIcon", i);
			CallCenterListIcon[i] = ituSceneFindWidget(&theScene, callname);
			assert(CallCenterListIcon[i]);
		}
		CallBottomNullButton0 = ituSceneFindWidget(&theScene, "CallBottomNullButton0");
		assert(CallBottomNullButton0);

		CallBottomNullButton1 = ituSceneFindWidget(&theScene, "CallBottomNullButton1");
		assert(CallBottomNullButton1);
	}
	g_Event = 0;
	g_TimerRuning = FALSE;
	g_RecordType = MISSED;
	SetCallRecordPageDisable(false);
	ituWidgetDisable(CallBottomNullButton0);
	ituWidgetDisable(CallBottomNullButton1);
	if (WhetherMissUnRead() == TRUE)
	{
		CallLayerPage(CallRecordPage);
		ituRadioBoxSetChecked(CallRightRecordRadioBox, true);
		ituRadioBoxSetChecked(CallBottomRecordMissRadioBox, true);
		ShowRecordWin(MISSED);
	}
	else
	{
		CallLayerPage(CallCenterPage);
		ituRadioBoxSetChecked(CallRightCenterRadioBox, true);
		ShowCenterWin(0);
	}
	CallKeyBordTextBox->text.layout = ITU_LAYOUT_MIDDLE_CENTER;

	return true;
}

/*************************************************
Function:		CalloutManager
Description:	快捷呼叫管理员机
Input:
Output:			无
Return:			无
Others:			无
*************************************************/
bool CalloutManager(ITUWidget* widget, char* param)
{
	TouchManagerKey();

	return true;
}

/*************************************************
Function:		CallReset
Description: 	呼叫界面初始化函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
void CallReset(void)
{
	CallLayer = NULL;
}

