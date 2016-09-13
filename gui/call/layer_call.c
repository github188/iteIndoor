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
#define MAX_PAGE_NUM					6					// 每页最大条数
#define MAX_CENTER_NUM  				8					// 最大管理员机数
#define MAX_RECORD_PAGE_NUM  			4					// 通话记录最大页数
#define MAX_CENTER_PAGE_NUM  			2					// 管理员机最大页数

/*****************变量定义***********************/
static ITULayer* MainLayer = NULL;
static ITULayer* BeCallLayer = NULL;
static ITULayer* CallLayer = NULL;
static ITUBackground* CallRightBackground = NULL;
static ITUTextBox* CallKeyBordTextBox = NULL;
static ITUText* CallKeyBordHitText = NULL;
static ITUKeyboard* CallKeyboard = NULL;
static ITUButton* CallRightExitButton = NULL;
static ITURadioBox* CallRightCeterRadioBox = NULL;
static ITURadioBox* CallRightRecordRadioBox = NULL;
static ITURadioBox* CallBottomRecordMissRadioBox = NULL;
static ITUBackground* CallBottomBackground0 = NULL;		// 纯色底部背景
static ITUBackground* CallBottomBackground1 = NULL;		// 通话记录底部背景
static ITUBackground* CallBottomBackground2 = NULL;		// 编辑底部背景
static ITUCoverFlow* CallRecordListCoverFlow = NULL;
static ITUCoverFlow* CallCeterListCoverFlow = NULL;
static ITUBackground* CallRecordListBackgroundPage[MAX_RECORD_PAGE_NUM] = { NULL };
static ITUBackground* CallCeterListBackgroundPage[MAX_CENTER_PAGE_NUM] = { NULL };
static ITUText* CallRecordListDevTypeText[MAX_RECORD_NUM] = { NULL };
static ITUText* CallRecordListTimeText[MAX_RECORD_NUM] = { NULL };
static ITUSprite* CallRecordListSprite[MAX_RECORD_NUM] = { NULL };
static ITUContainer* CallRecordListContainer[MAX_RECORD_NUM] = { NULL };
static ITUIcon* CallCeterListIcon[MAX_CENTER_NUM] = { NULL };
static ITUText* CallCeterListDevTypeText[MAX_CENTER_NUM] = { NULL };
static ITUContainer* CallCeterListContainer[MAX_CENTER_NUM] = { NULL };
static ITUText* CallRecordMSGHitText = NULL;
static ITUBackground* CallRecordMSGHitGrayBackground = NULL;

/*****************常量定义***********************/
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
	CallCeterEvent = 0x00,
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
	CallCeterPage = 0x00,
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
		case CallCeterPage:
			ituWidgetSetVisible(CallKeyboard, false);
			ituWidgetSetVisible(CallCeterListCoverFlow, true);
			ituWidgetSetVisible(CallRecordListCoverFlow, false);
			ituWidgetSetVisible(CallBottomBackground0, true);
			ituWidgetSetVisible(CallBottomBackground1, false);
			ituWidgetSetVisible(CallBottomBackground2, false);
			break;

		case CallUserPage:
			ituWidgetSetVisible(CallKeyboard, true);
			ituWidgetSetVisible(CallCeterListCoverFlow, false);
			ituWidgetSetVisible(CallRecordListCoverFlow, false);
			ituWidgetSetVisible(CallBottomBackground0, true);
			ituWidgetSetVisible(CallBottomBackground1, false);
			ituWidgetSetVisible(CallBottomBackground2, false);
			break;

		case CallRecordPage:
			ituWidgetSetVisible(CallKeyboard, false);
			ituWidgetSetVisible(CallCeterListCoverFlow, false);
			ituWidgetSetVisible(CallRecordListCoverFlow, true);
			ituWidgetSetVisible(CallBottomBackground0, false);
			ituWidgetSetVisible(CallBottomBackground1, true);
			ituWidgetSetVisible(CallBottomBackground2, false);
			break;

		case CallEditPage:
			ituWidgetSetVisible(CallKeyboard, false);
			ituWidgetSetVisible(CallCeterListCoverFlow, false);
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
Function:		SetRecordShowInit
Description: 	通话记录页面初始化
Input:			无
Output:			无
Return:			TRUE 成功 FALSE 失败
Others:
*************************************************/
static void SetRecordShowInit(void)
{
	uint8 i;

	for (i = 0; i < MAX_RECORD_PAGE_NUM; i++)
	{
		ituWidgetSetVisible(CallRecordListBackgroundPage[i], true);
	}

	for (i = 0; i < MAX_RECORD_NUM; i++)
	{
		ituWidgetSetVisible(CallRecordListContainer[i], true);
		ituWidgetSetVisible(CallRecordListSprite[i], true);
		ituSetColor(&((ITUWidget*)CallRecordListDevTypeText[i])->color, 255, 255, 255, 255);
		ituSetColor(&((ITUWidget*)CallRecordListTimeText[i])->color, 255, 255, 255, 255);
	}
	// 初始化默认为首页
	// 解决CallRecordListCoverFlow第一页是-1的情况
	if (CallRecordListCoverFlow->frame == CallRecordListCoverFlow->totalframe)
	{
		ituCoverFlowGoto(CallRecordListCoverFlow, -1);
	}
	else
	{
		ituCoverFlowGoto(CallRecordListCoverFlow, 0);
	}
}

/*************************************************
Function:		SetRecordShowNum
Description: 	显示通话记录具体条数
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void SetRecordShowNum(uint8 max)
{
	uint8 i, count = 0;
	uint8 pagenum = 0, rest = 0;

	// 隐藏多余页
	if (max <= MAX_PAGE_NUM)
	{
		pagenum = 1;
	}
	else
	{
		pagenum = max / MAX_PAGE_NUM;
		rest = max % MAX_PAGE_NUM;
		if (rest > 0)
		{
			pagenum++;
		}
	}
	for (i = MAX_RECORD_PAGE_NUM; i > pagenum; i--)
	{
		ituWidgetSetVisible(CallRecordListBackgroundPage[i - 1], false);
	}

	// 隐藏多余行(目前通话记录只支持20条)
	count = pagenum * MAX_PAGE_NUM;
	if (count > MAX_RECORD_NUM)
	{
		count = MAX_RECORD_NUM;
	}

	for (i = count; i > max; i--)
	{
		ituWidgetSetVisible(CallRecordListContainer[i - 1], false);
	}
	printf("SetRecordShowNum..:count %d\n", count);
	printf("SetRecordShowNum..:max %d\n", max);
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
	uint8 i, max = 0;
	char devtype[50], timestr[50];
	
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
	SetRecordShowInit();
	g_RecordType = Calltype;

	g_CallRecord = storage_get_callrecord(Calltype);
	if (g_CallRecord == NULL)
	{
		printf("g_CallRecord is NULL\n");
		return;
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
			memset(devtype, 0, sizeof(devtype));
			memset(timestr, 0, sizeof(timestr));

			get_dev_description(g_CallRecord->CallInfo[i].Type, g_CallRecord->CallInfo[i].devno, devtype, 50);
			sprintf(timestr, "%04d-%02d-%02d %02d:%02d:%02d", g_CallRecord->CallInfo[i].Time.year, g_CallRecord->CallInfo[i].Time.month, g_CallRecord->CallInfo[i].Time.day,
				g_CallRecord->CallInfo[i].Time.hour, g_CallRecord->CallInfo[i].Time.min, g_CallRecord->CallInfo[i].Time.sec);

			// 如果是未接来电，且未读则字体颜色改为红色
			if (Calltype == MISSED)
			{
				if (g_CallRecord->CallInfo[i].UnRead)
				{
					g_CallRecord->CallInfo[i].UnRead = 0;
					ituSetColor(&((ITUWidget*)CallRecordListDevTypeText[i])->color, 255, 255, 0, 0);
					ituSetColor(&((ITUWidget*)CallRecordListTimeText[i])->color, 255, 255, 0, 0);
				}
			}

			ituTextSetString(CallRecordListDevTypeText[i], devtype);
			ituTextSetString(CallRecordListTimeText[i], timestr);
			// 如果处于编辑状态尾部小图标应改为未选中状态
			if (g_CurPage == CallRecordPage)
			{
				if (g_CallRecord->CallInfo[i].Type == DEVICE_TYPE_AREA || g_CallRecord->CallInfo[i].Type == DEVICE_TYPE_STAIR
					|| g_CallRecord->CallInfo[i].Type == DEVICE_TYPE_DOOR_NET)
				{
					ituWidgetSetVisible(CallRecordListSprite[i], false);
				}
				else
				{
					ituSpriteGoto(CallRecordListSprite[i], CallCallIcon);
				}
			}
			else if (g_CurPage == CallEditPage)
			{
				ituWidgetSetVisible(CallRecordListSprite[i], true);
				ituSpriteGoto(CallRecordListSprite[i], CallUnChoseIcon);
			}
		}

		if (Calltype == MISSED)
		{
			storage_set_callrecord_state(g_CallRecord);
		}
		SetRecordShowNum(max);
	}
	else
	{
		SetRecordShowNum(0);
	}
}

/*************************************************
Function:		SetCenterShowInit
Description: 	显示页面初始化
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void SetCenterShowInit(void)
{
	uint8 i;

	for (i = 0; i < MAX_CENTER_PAGE_NUM; i++)
	{
		ituWidgetSetVisible(CallCeterListBackgroundPage[i], true);
	}

	for (i = 0; i < MAX_CENTER_NUM; i++)
	{
		ituWidgetSetVisible(CallCeterListContainer[i], true);
	}
	// 初始化默认为首页
	// 解决CallCeterListCoverFlow第一页是-1的情况
	if (CallCeterListCoverFlow->frame == CallCeterListCoverFlow->totalframe)
	{
		ituCoverFlowGoto(CallCeterListCoverFlow, -1);
	}
	else
	{
		ituCoverFlowGoto(CallCeterListCoverFlow, 0);
	}
}

/*************************************************
Function:		SetCenterShowNum
Description: 	显示管理员机或分机具体条数
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void SetCenterShowNum(uint8 max)
{
	uint8 i, count = 0;
	uint8 pagenum = 0, rest = 0;

	// 隐藏多余页
	if (max <= MAX_PAGE_NUM)
	{
		pagenum = 1;
	}
	else
	{
		pagenum = max / MAX_PAGE_NUM;
		rest = max % MAX_PAGE_NUM;
		if (rest > 0)
		{
			pagenum++;
		}
	}

	for (i = MAX_CENTER_PAGE_NUM; i > pagenum; i--)
	{
		ituWidgetSetVisible(CallCeterListBackgroundPage[i - 1], false);
	}

	// 隐藏多余行(目前分机只支持8条)
	count = pagenum * MAX_PAGE_NUM;
	if (count > MAX_CENTER_NUM)
	{
		count = MAX_CENTER_NUM;
	}

	for (i = count; i > max; i--)
	{
		ituWidgetSetVisible(CallCeterListContainer[i - 1], false);
	}
	printf("SetCenterShowNum..:count %d\n", count);
	printf("SetCenterShowNum..:max %d\n", max);
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
	uint8 i, count = 0;
	char devtype[50], devno[5];

	SetCenterShowInit();
	g_CenterType = type;
	memset(g_Index, 0, sizeof(g_Index));

	if (type == 0)
	{
		for (i = 0; i < 3; i++)
		{
			if (storage_get_manager_ip(i+1))
			{
				g_Index[count] = i+ MANAGER_NUM + 1;
				count++;
			}
		}

		if (count)
		{
			for (i = 0; i < count; i++)
			{
				memset(devno, 0, sizeof(devno));
				memset(devtype, 0, sizeof(devtype));

				sprintf(devno, "%d", g_Index[i]);
				printf("devno : %s g_Index[%d] : %d\n", devno, i, g_Index[i]);
				get_dev_description(DEVICE_TYPE_MANAGER, devno, devtype, 50);
				ituTextSetString(CallCeterListDevTypeText[i], devtype);
			}
			SetCenterShowNum(count);
		}
		else
		{
			g_Index[0] = 0xFF;
			ituTextSetString(CallCeterListDevTypeText[0], get_str(SID_DevManager));
			SetCenterShowNum(1);
		}
	}
	else
	{
		for (i = 0; i < 8; i++)
		{
			if (storage_get_subdev_ip(i))
			{
				g_Index[count] = i;
				count++;
			}
		}
		if (count)
		{
			for (i = 0; i < count; i++)
			{
				memset(devno, 0, sizeof(devno));
				memset(devtype, 0, sizeof(devtype));

				sprintf(devno, "%d", g_Index[i]);
				get_dev_description(DEVICE_TYPE_FENJI_NET, devno, devtype, 50);
				ituTextSetString(CallCeterListDevTypeText[i], devtype);
			}
			SetCenterShowNum(count);
		}
		else
		{
			SetCenterShowNum(0);
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
		if (CallChoseIcon == CallRecordListSprite[i]->frame)
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
		ituSpriteGoto(CallRecordListSprite[i], CallChoseIcon);
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
			dprintf("media is not null\n");
		}
	}
}

/*************************************************
Function:		CallDestroyProc
Description: 	销毁处理函数
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void CallDestroyProc(void)
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
			if (g_HintStrID == SID_Inter_WrongNo || g_HintStrID == SID_Inter_NoNotFound)
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
			}
		}
	}

	return true;
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
	uint8 index = atoi(param);

	dprintf("RecordList index......:%d\n", index);
	if (g_CurPage == CallEditPage)
	{	
		if (CallChoseIcon == CallRecordListSprite[index]->frame)
		{
			ituSpriteGoto(CallRecordListSprite[index], CallUnChoseIcon);
		}
		else
		{
			ituSpriteGoto(CallRecordListSprite[index], CallChoseIcon);
		}
	}
	else
	{
		if (g_CallRecord->CallInfo[index].Type == DEVICE_TYPE_MANAGER || g_CallRecord->CallInfo[index].Type == DEVICE_TYPE_ROOM ||
			g_CallRecord->CallInfo[index].Type == DEVICE_TYPE_FENJI_NET)
		{
			if (g_CallRecord->CallInfo[index].Type == DEVICE_TYPE_ROOM)
			{
				PFULL_DEVICE_NO dev = storage_get_devparam();

				g_TimerRuning = TRUE;
				SetCalloutInfo(CallUserEvent);
				CallLayerPage(CallUserPage);
				strncpy(g_CallNo, g_CallRecord->CallInfo[index].devno, dev->DevNoLen - 1);
				g_CallLastTick = SDL_GetTicks();
				ituTextBoxSetString(CallKeyBordTextBox, NULL);
				CalloutEventStart();
			}
			else
			{	
				INTER_INFO_S CallInfo = { 0 };

				CallInfo.InterType = INTER_CALLOUT_E;
				if (g_CallRecord->CallInfo[index].Type == DEVICE_TYPE_MANAGER)
				{
					CallInfo.DevType = DEVICE_TYPE_MANAGER;
				}
				else
				{
					CallInfo.DevType = DEVICE_TYPE_FENJI_NET;
				}
				strcpy(CallInfo.DevStr, g_CallRecord->CallInfo[index].devno);
				BeCallWin(&CallInfo);
			}
		}
	}

	return true;
}

/*************************************************
Function:		CallCeterListButtonOnMouseUp
Description: 	管理员机、分机按下执行函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool CallCeterListButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 index = atoi(param);
	INTER_INFO_S CallInfo = { 0 };

	int32 ret = ui_show_win_arbitration(SYS_OPER_CALLOUT);
	if (ret == TRUE)
	{
		CallInfo.InterType = INTER_CALLOUT_E;
		if (g_CenterType == 0)
		{
			CallInfo.DevType = DEVICE_TYPE_MANAGER;
			sprintf(CallInfo.DevStr, "%d", g_Index[index]);
			BeCallWin(&CallInfo);
		}
		else
		{
			CallInfo.DevType = DEVICE_TYPE_FENJI_NET;
			sprintf(CallInfo.DevStr, "%d", g_Index[index]);
			BeCallWin(&CallInfo);
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
				if (CallChoseIcon == CallRecordListSprite[i]->frame)
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
		case CallCeterEvent:
			CallLayerPage(CallCeterPage);
			ShowCenterWin(0);
			break;

		case CallFenJiEvent:
			CallLayerPage(CallCeterPage);
			ShowCenterWin(1);
			break;

		case CallUserEvent:
			SetCalloutInfo(btn_event);
			CallLayerPage(CallUserPage);
			break;

		case CallRecordEvent:
			printf(" Record Press.........\n");
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
			printf(" Exit Press.........\n");
			if (ituWidgetIsVisible(CallBottomBackground2))
			{
				ituRadioBoxSetChecked(CallBottomRecordMissRadioBox, true);
				CallLayerPage(CallRecordPage);
				ShowRecordWin(MISSED);
			}
			else
			{
				CallDestroyProc();
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

		CallRightCeterRadioBox = ituSceneFindWidget(&theScene, "CallRightCeterRadioBox");
		assert(CallRightCeterRadioBox);

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

		CallCeterListCoverFlow = ituSceneFindWidget(&theScene, "CallCeterListCoverFlow");
		assert(CallKeyboard);

		CallRecordMSGHitText = ituSceneFindWidget(&theScene, "CallRecordMSGHitText");
		assert(CallRecordMSGHitText);

		CallRecordMSGHitGrayBackground = ituSceneFindWidget(&theScene, "CallRecordMSGHitGrayBackground");
		assert(CallRecordMSGHitGrayBackground);

		for (i = 0; i < MAX_RECORD_PAGE_NUM; i++)
		{
			memset(callname, 0, sizeof(callname));
			sprintf(callname, "%s%d", "CallRecordListBackgroundPage", i);
			CallRecordListBackgroundPage[i] = ituSceneFindWidget(&theScene, callname);
			assert(CallRecordListBackgroundPage[i]);
		}

		for (i = 0; i < MAX_CENTER_PAGE_NUM; i++)
		{
			memset(callname, 0, sizeof(callname));
			sprintf(callname, "%s%d", "CallCeterListBackgroundPage", i);
			CallCeterListBackgroundPage[i] = ituSceneFindWidget(&theScene, callname);
			assert(CallCeterListBackgroundPage[i]);
		}

		for (i = 0; i < MAX_RECORD_NUM; i++)
		{
			memset(callname, 0, sizeof(callname));
			sprintf(callname, "%s%d", "CallRecordListContainer", i);
			CallRecordListContainer[i] = ituSceneFindWidget(&theScene, callname);
			assert(CallRecordListContainer[i]);

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
		}

		for (i = 0; i < MAX_CENTER_NUM; i++)
		{
			memset(callname, 0, sizeof(callname));
			sprintf(callname, "%s%d", "CallCeterListContainer", i);
			CallCeterListContainer[i] = ituSceneFindWidget(&theScene, callname);
			assert(CallCeterListContainer[i]);

			memset(callname, 0, sizeof(callname));
			sprintf(callname, "%s%d", "CallCeterListDevTypeText", i);
			CallCeterListDevTypeText[i] = ituSceneFindWidget(&theScene, callname);
			assert(CallCeterListDevTypeText[i]);
			 
			memset(callname, 0, sizeof(callname));
			sprintf(callname, "%s%d", "CallCeterListIcon", i);
			CallCeterListIcon[i] = ituSceneFindWidget(&theScene, callname);
			assert(CallCeterListIcon[i]);
		}
	}
	g_Event = 0;
	g_TimerRuning = FALSE;
	g_RecordType = MISSED;
	SetCallRecordPageDisable(false);
	if (WhetherMissUnRead() == TRUE)
	{
		CallLayerPage(CallRecordPage);
		ituRadioBoxSetChecked(CallRightRecordRadioBox, true);
		ituRadioBoxSetChecked(CallBottomRecordMissRadioBox, true);
		ShowRecordWin(MISSED);
	}
	else
	{
		CallLayerPage(CallCeterPage);
		ituRadioBoxSetChecked(CallRightCeterRadioBox, true);
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
	uint8 i = 0, tmp = 0;
	INTER_INFO_S CallInfo = { 0 };

	int32 ret = ui_show_win_arbitration(SYS_OPER_CALLOUT);
	if (ret == TRUE)
	{
		CallInfo.InterType = INTER_CALLOUT_E;
		CallInfo.DevType = DEVICE_TYPE_MANAGER;

		for (i = 0; i < 3; i++)
		{
			if (storage_get_manager_ip(i + 1))
			{
				tmp = i + MANAGER_NUM + 1;
				continue;
			}
		}

		if (0 == tmp)
		{
			tmp = 0xFF;
		}
		sprintf(CallInfo.DevStr, "%d", tmp);
		BeCallWin(&CallInfo);
	}
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

