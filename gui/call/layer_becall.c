/*************************************************
Copyright (C),  2006-2016, Aurine
File name:		layer_becall.c
Author:     	caogw
Version:    	
Date: 			2016-07-22
Description:
History:
1. Date:
Author:
Modification:
*************************************************/
#include "gui_include.h"

/*****************常量定义***********************/

/*****************变量定义***********************/
static ITULayer* MainLayer = NULL;
static ITULayer* BeCallLayer = NULL;
static ITUContainer* BeCallRightAnswerContainer = NULL;
static ITUContainer* BeCallRightHandUpContainer = NULL;
static ITUContainer* BeCallRightLockContainer = NULL;
static ITUContainer* BeCallRightSnapContainer = NULL;
static ITUContainer* BeCallRightNullContainer[3] = { NULL };
static ITUButton* BeCallRightNullButton[3] = { NULL };
static ITUBackground* BeCallBottomBackground = NULL;
static ITUBackground* BeCallHitBackground = NULL;
static ITUText* BeCallTimeText = NULL;
static ITUText* BecallCallNoText = NULL;
static ITUText* BecallNewCallNoText = NULL;
static ITUSprite* BeCallHitSprite = NULL;
static ITUSprite* BeCallSoundSprite = NULL;
static ITUContainer* BeCallShowButtomContainer = NULL;
static ITUBackground* BeCallRightSnapGrayBackground = NULL;
static ITUButton* BeCallRightSnapButton = NULL;
static ITUTrackBar* BeCallSoundTrackBar = NULL;

/*****************常量定义***********************/
static DEVICE_TYPE_E g_DevType;
static INTER_OPER_TYPE g_OperType;
static char g_DevNo[50];
static char g_NewCallNo[50];
static uint8 g_unlock = FALSE;
static uint8 g_DrawVideo = 0;
static CALL_STATE_E g_InterState = CALL_STATE_NONE;
static int16 g_RemainTime = 0;
static uint8 g_SetVolume = FALSE;					// 是否处于设置音量状态
static uint8 g_mute = FALSE;						// 是否静音状态
static uint8 g_volume = 4;
static uint8 g_volumeticks = 0;
static uint8 g_MSGLockTicks = 0;
static uint8 g_MSGSnapTicks = 0;
static uint32 g_BeCallLastTick = 0;					// 实时更新的tick
static uint32 g_ErrHintTxtID = 0;					// 错误提示文字ID
static uint8 g_ErrHintTicks = 0;					// 错误提示计时
static uint8 g_StartCalloutTick = 0;				// 开始呼叫tick
static uint8 g_HandupDelay = 2;						// 接听需延时才可挂断

typedef enum
{
	BeCallAnswerEvent = 0x00,
	BeCallHandUpEvent,
	BeCallLockEvent,
	BeCallSnapEvent,
	BeCallExitEvent,
	BeCallMaxEvent,
}BeCallButtonEvent;

// 被叫提示图标类型
typedef enum
{
	BeCallLockMSGIcon = 0x00,
	BeCallSnapMSGIcon,
	BeCallMaxMSGIcon,
}BeCallMSGIconType;

// 是否静音图标类型
typedef enum
{
	BeCallNoMuteIcon = 0x00,
	BeCallMuteIcon,
	BeCallMaxIcon,
}BeCallSoundIcon;

/*************************************************
Function:		InitRightButton
Description: 	初始化右边按键
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void InitRightButton(void)
{
	uint8 i;

	if (g_OperType == INTER_CALLIN_E)
	{
		ituWidgetSetVisible(BeCallRightAnswerContainer, true);
		ituWidgetSetVisible(BeCallRightHandUpContainer, false);
		if (g_unlock == TRUE)
		{
			ituWidgetSetVisible(BeCallRightLockContainer, true);
			ituWidgetSetVisible(BeCallRightSnapContainer, true);
			ituWidgetSetVisible(BeCallRightNullContainer[0], false);
			ituWidgetSetVisible(BeCallRightNullContainer[1], false);
			ituWidgetSetVisible(BeCallRightSnapGrayBackground, false);
			ituWidgetEnable(BeCallRightSnapButton);
		}
		else
		{
			ituWidgetSetVisible(BeCallRightLockContainer, false);
			ituWidgetSetVisible(BeCallRightSnapContainer, false);
			ituWidgetSetVisible(BeCallRightNullContainer[0], true);
			ituWidgetSetVisible(BeCallRightNullContainer[1], true);
		}
	}
	else
	{
		ituWidgetSetVisible(BeCallRightAnswerContainer, false);
		ituWidgetSetVisible(BeCallRightHandUpContainer, true);
		ituWidgetSetVisible(BeCallRightLockContainer, false);
		ituWidgetSetVisible(BeCallRightSnapContainer, false);
		ituWidgetSetVisible(BeCallRightNullContainer[0], true);
		ituWidgetSetVisible(BeCallRightNullContainer[1], true);
	}

	for (i = 0; i < 3; i++)
	{
		ituWidgetDisable(BeCallRightNullButton[i]);
	}
}

/*************************************************
Function:		DrawStringHint
Description: 	画状态提示
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void DrawStringHint(void)
{
	char time[10] = { 0 };
	char temp[50] = { 0 };
	char DevDesc[50] = { 0 };
	char NoText[100] = { 0 };
	uint32 StrId = 0;

	switch (g_InterState)
	{
		case CALL_STATE_REQUEST:
			StrId = SID_Inter_Connecting;
			break;

		case CALL_STATE_CALLING:
			if (g_OperType == INTER_CALLOUT_E)
			{
				StrId = SID_Inter_Calling;
			}
			else if (g_OperType == INTER_CALLIN_E)
			{
				StrId = SID_Inter_BeCalling;
			}
			break;

		case CALL_STATE_RECORDHINT:
			StrId = SID_Inter_RecordHinting;
			break;

		case CALL_STATE_RECORDING:
			StrId = SID_Inter_Recording;
			break;

		case CALL_STATE_TALK:
			StrId = SID_Inter_Talking;
			break;

		case CALL_STATE_END:
			StrId = 0;
			break;

		default:
			StrId = 0;
			break;
	}
	// 画设备描述
	if (strlen(g_DevNo) > 0)
	{
		if (atoi(g_DevNo) == 0xFF)
		{
			strcpy(DevDesc, get_str(SID_DevManager));
		}
		else
		{
			get_dev_description(g_DevType, g_DevNo, DevDesc, 50); // 获得设备描述
		}
	}
	sprintf(temp, "[%s]", DevDesc);

	if (g_ErrHintTxtID)
	{
		sprintf(NoText, "%s %s", temp, get_str(g_ErrHintTxtID));
		ituTextSetString(BecallCallNoText, NoText);
	}
	else if (StrId)
	{
		sprintf(NoText, "%s %s", temp, get_str(StrId));
		ituTextSetString(BecallCallNoText, NoText);
	}
	else
	{
		ituTextSetString(BecallCallNoText, temp);
	}

	// 画时间提示: time
	if (g_RemainTime)
	{
		sprintf(time, "%d", g_RemainTime);
		ituTextSetString(BeCallTimeText, time);
	}

	// 画新来电提示
	if (strlen(g_NewCallNo) > 0)
	{
		uint16 DevType = g_NewCallNo[1];
		char NewCallNo[30] = { 0 };

		sprintf(NewCallNo, "%s", g_NewCallNo+2);
		memset(DevDesc, 0, sizeof(DevDesc));
		memset(temp, 0, sizeof(temp));
		memset(NoText, 0, sizeof(NoText));
		get_dev_description(DevType, NewCallNo, DevDesc, 50);
		StrId = SID_Inter_BeCalling;

		if (StrId)
		{
			sprintf(temp, "[%s]", DevDesc);
			sprintf(NoText, "%s %s", temp, get_str(StrId));
			ituTextSetString(BecallNewCallNoText, NoText);
		}
	}
}

/*************************************************
Function:		CallinAnswerCall
Description: 	接听时改变右键
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void CallinAnswerCall(void)
{
	// 接听
	uint32 ret = inter_answer_call(NULL);
	if (ret == FALSE)
	{
		return;
	}
	ituWidgetSetVisible(BeCallRightHandUpContainer, true);
	ituWidgetSetVisible(BeCallRightAnswerContainer, false);
}

/*************************************************
Function:		ChangeVolume
Description: 	调节音量
Input:			无
Output:			无
Return:			无
Others:
*************************************************/
static void ChangeVolume(void)
{
	uint8 flag = FALSE;

	g_volumeticks = 4;
	// 音量为0时，自动为静音
	if (g_mute)
	{
		flag = TRUE;
	}
	else if (0 == g_volume)
	{
		flag = TRUE;
	}
	else
	{
		flag = FALSE;
	}

	if (TRUE == flag)
	{
		// 设置静音音量
		media_set_talk_volume(g_DevType, 0);
	}
	else
	{
		// 设置volume音量
		media_set_talk_volume(g_DevType, g_volume);
	}
}

/*************************************************
Function:		SetBeCallLockAndSnap
Description: 	设置被叫开锁或抓拍
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void SetBeCallLockAndSnap(BeCallButtonEvent event)
{
	int32 ret = FALSE;
	if (event == BeCallLockEvent)
	{
		if (g_unlock == TRUE)
		{
			ret = inter_unlock();
			if (ret == TRUE)
			{
				#ifdef _USE_ELEVATOR_			//被动招梯
				if (g_DevType == DEVICE_TYPE_STAIR)
				{
					// modify caogw 2016-09-29 因视频卡顿现象暂时屏蔽
					//dianti_set_cmd(ELEVATOR_CALL);
				}
				#endif
				dprintf("becall lock ok!\n");
			}
			else
			{
				// 提示开锁失败
				dprintf("becall lock fail!\n");
			}
		}
		ituWidgetSetVisible(BeCallHitBackground, true);
		ituSpriteGoto(BeCallHitSprite, BeCallLockMSGIcon);
		g_MSGLockTicks = 3;
	}
	else
	{
		ituWidgetDisable(BeCallRightSnapButton);
		ituWidgetSetVisible(BeCallRightSnapGrayBackground, true);
		inter_video_snap();
		// 超时判断使用，防止抓拍失败，无法再次抓拍
		g_MSGSnapTicks = 5;
	}
}

/*************************************************
Function:		BeCallCalloutStart
Description: 	管理员机和分机开始呼叫
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void BeCallCalloutStart(void)
{
	uint8 ret = FALSE;

	if (g_DevType == DEVICE_TYPE_MANAGER)
	{
		ret = logic_call_center(g_DevNo);
	}
	else if (g_DevType == DEVICE_TYPE_FENJI_NET)
	{
		ret = logic_call_sub_terminal(g_DevNo);
	}
	else
	{
		ret = TRUE;
	}

	if (ret == TRUE)
	{
		g_InterState = CALL_STATE_CALLING;
	}
	else
	{
		g_ErrHintTxtID = SID_Inter_ConnFailed;
		g_ErrHintTicks = 3;
		g_InterState = CALL_STATE_END;
	}
	DrawStringHint();
}

/*************************************************
Function:		BeCallLayerOnLeave
Description:	销毁处理函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool BeCallLayerOnLeave(ITUWidget* widget, char* param)
{
	if (TRUE == g_DrawVideo)
	{
		BackgroundDrawVideo_exit();
	}
	return true;
}

/*************************************************
Function:		BeCallLayerKeyOnMouseUp
Description: 	开锁、通话快捷键
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool BeCallLayerKeyOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 btn_event = atoi(param);
	char temp[10] = { 0 };

	dprintf("key btn_event........: %d\n", btn_event);
	if (0 == btn_event)
	{
		if (false == g_unlock)
		{
			return false;
		}
		sprintf(temp, "%d", BeCallLockEvent);
	}
	else
	{
		if (true == ituWidgetIsVisible(BeCallRightHandUpContainer))
		{
			sprintf(temp, "%d", BeCallHandUpEvent);
		}
		else
		{
			sprintf(temp, "%d", BeCallAnswerEvent);
		}
	}
	BeCallLayerButtonOnMouseUp(NULL, temp);

	return true;
}

/*************************************************
Function:		BeCallLayerOnTimer
Description: 	定时器
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool BeCallLayerOnTimer(ITUWidget* widget, char* param)
{
	uint32 tick = SDL_GetTicks();
	uint32 diff = tick - g_BeCallLastTick;
	if (diff >= 1000)
	{
		g_BeCallLastTick = tick;

		if (g_SetVolume == TRUE)
		{
			g_volumeticks--;
			if (0 == g_volumeticks)
			{
				g_SetVolume = FALSE;
				// 保存铃声或通话音量
				if (g_InterState == CALL_STATE_CALLING)
				{
					storage_set_volume(g_volume, storage_get_talkvolume(), storage_get_keykeep());
				}
				else
				{
					storage_set_volume(storage_get_ringvolume(), g_volume, storage_get_keykeep());
				}
				ituWidgetSetVisible(BeCallBottomBackground, false);
			}
		}

		if (g_MSGLockTicks)
		{
			g_MSGLockTicks--;
			if (0 == g_MSGLockTicks)
			{
				ituWidgetSetVisible(BeCallHitBackground, false);
			}
		}

		if (g_MSGSnapTicks)
		{
			g_MSGSnapTicks--;
			if (0 == g_MSGSnapTicks)
			{
				ituWidgetSetVisible(BeCallHitBackground, false);
				ituWidgetSetVisible(BeCallRightSnapGrayBackground, false);
				ituWidgetEnable(BeCallRightSnapButton);
			}
		}

		if (g_ErrHintTicks)
		{
			g_ErrHintTicks--;
			if (0 == g_ErrHintTicks)
			{
				if (g_InterState == CALL_STATE_END)
				{
					ituLayerGoto(MainLayer);
				}
				g_ErrHintTxtID = 0;
			}
		}

		// 开始呼叫管理员机和分机
		if (g_StartCalloutTick)
		{
			g_StartCalloutTick--;
			if (0 == g_StartCalloutTick)
			{
				BeCallCalloutStart();
			}
		}

		// 挂机延时
		if (g_HandupDelay)
		{
			g_HandupDelay--;
		}
	}

	return true;
}

/*************************************************
Function:		BeCallCallInState
Description: 	主叫回调执行函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool BeCallCallInState(ITUWidget* widget, char* param)
{
	uint32 temp = 0;
	PINTER_CALLBACK pcallbak_data = (PINTER_CALLBACK)param;

	dprintf("pcallbak_data->InterState......:%d\n", pcallbak_data->InterState);
	switch (pcallbak_data->InterState)
	{
		case CALL_STATE_REQUEST:
			break;

		case CALL_STATE_CALLING:
			if (g_InterState == CALL_STATE_CALLING)
			{
				g_volume = storage_get_ringvolume();
			}
			else
			{
				g_volume = storage_get_talkvolume();
			}
			g_InterState = CALL_STATE_CALLING;
			break;

		case CALL_NEW_CALLING:
			if (0 == pcallbak_data->DataLen)
			{
				memset(g_NewCallNo, 0, sizeof(g_NewCallNo));
			}
			else
			{
				memcpy(g_NewCallNo, (PINTER_INFO_S)pcallbak_data->Buf, sizeof(INTER_INFO_S));
			}
			break;

		case CALL_STATE_RECORDHINT:
			g_InterState = CALL_STATE_RECORDHINT;
			break;

		case CALL_STATE_RECORDING:
			g_InterState = CALL_STATE_RECORDING;
			break;

		case CALL_STATE_TALK:
			ituWidgetSetVisible(BeCallShowButtomContainer, true);
			g_volume = storage_get_talkvolume();
			ituTrackBarSetValue(BeCallSoundTrackBar, g_volume);
			g_InterState = CALL_STATE_TALK;
			break;

		case CALL_STATE_END:
			g_InterState = CALL_STATE_NONE;			
			ituLayerGoto(MainLayer);
			return true;
			break;

		case CALL_TIMER:
			temp = atoi(pcallbak_data->Buf);
			g_RemainTime = temp >> 16;
			break;

		case CALL_SNAP_CALLBACK:
			if (1 == atoi(pcallbak_data->Buf))
			{
				ituWidgetSetVisible(BeCallHitBackground, true);
				ituSpriteGoto(BeCallHitSprite, BeCallSnapMSGIcon);
				g_MSGSnapTicks = 3;
			}
			break;

		default:
			break;
	}
	DrawStringHint();

	return true;
}

/*************************************************
Function:		BeCallCallOutState
Description: 	主叫回调执行函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool BeCallCallOutState(ITUWidget* widget, char* param)
{
	uint32 temp = 0;
	PINTER_CALLBACK pcallbak_data = (PINTER_CALLBACK)param;

	dprintf("pcallbak_data->InterState......:%d\n", pcallbak_data->InterState);
	switch (pcallbak_data->InterState)
	{
		case CALL_STATE_REQUEST:
			break;

		case CALL_STATE_CALLING:
			g_InterState = CALL_STATE_CALLING;
			break;

		case CALL_NEW_CALLING:
			if (0 == pcallbak_data->DataLen)
			{
				memset(g_NewCallNo, 0, sizeof(g_NewCallNo));
			}
			else
			{
				memcpy(g_NewCallNo, (PINTER_INFO_S)pcallbak_data->Buf, sizeof(INTER_INFO_S));
			}
			break;

		case CALL_STATE_RECORDHINT:
			g_InterState = CALL_STATE_RECORDHINT;
			break;

		case CALL_STATE_RECORDING:
			g_InterState = CALL_STATE_RECORDING;
			break;

		case CALL_STATE_TALK:
			ituWidgetSetVisible(BeCallShowButtomContainer, true);
			g_volume = storage_get_talkvolume();
			ituTrackBarSetValue(BeCallSoundTrackBar, g_volume);
			g_InterState = CALL_STATE_TALK;
			break;

		case CALL_STATE_END:
			g_InterState = CALL_STATE_NONE;			
			ituLayerGoto(MainLayer);
			return true;
			break;

		case CALL_TIMER:
			temp = atoi(pcallbak_data->Buf);
			g_RemainTime = temp >> 16;
			break;

		default:
			break;
	}
	DrawStringHint();

	return true;
}

/*************************************************
Function:		BeCallButtomSoundOnChanged
Description: 	音量按下执行函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool BeCallButtomSoundOnChanged(ITUWidget* widget, char* param)
{
	uint8 volume = atoi(param);

	g_volume = volume;
	ChangeVolume();
	printf("volume..........%d\n", volume);

	return true;
}

/*************************************************
Function:		BeCallButtomSoundButtonOnMouseUp
Description: 	是否静音按下执行函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool BeCallButtomSoundButtonOnMouseUp(ITUWidget* widget, char* param)
{
	if (BeCallSoundSprite->frame == BeCallNoMuteIcon)
	{
		ituSpriteGoto(BeCallSoundSprite, BeCallMuteIcon);
	}
	else
	{
		ituSpriteGoto(BeCallSoundSprite, BeCallNoMuteIcon);
	}

	g_mute = !g_mute;
	ChangeVolume();

	return true;
}

/*************************************************
Function:		BeCallHideSoundOnMouseUp
Description: 	隐藏声音进度条执行函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool BeCallHideSoundOnMouseUp(ITUWidget* widget, char* param)
{
	if (g_SetVolume)
	{
		g_volumeticks = 0;
		g_SetVolume = FALSE;
		// 保存铃声或通话音量
		if (g_InterState == CALL_STATE_CALLING)
		{
			storage_set_volume(g_volume, storage_get_talkvolume(), storage_get_keykeep());
		}
		else
		{
			storage_set_volume(storage_get_ringvolume(), g_volume, storage_get_keykeep());
		}
		ituWidgetSetVisible(BeCallBottomBackground, false);
	}
	dprintf("g_SetVolume..........%d\n", g_SetVolume);
}

/*************************************************
Function:		BeCallShowButtomBackgroundOnMouseUp
Description: 	是否显示音量调节函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool BeCallShowButtomBackgroundOnMouseUp(ITUWidget* widget, char* param)
{
	if (!g_SetVolume)
	{
		g_volumeticks = 4;
		// 被叫响铃或通话时才可调音量
		if ((g_OperType == INTER_CALLIN_E && g_InterState == CALL_STATE_CALLING)
			|| g_InterState == CALL_STATE_TALK)
		{
			g_SetVolume = TRUE;

			// 显示音量初始化
			if (g_InterState == CALL_STATE_CALLING)
			{
				g_volume = storage_get_ringvolume();
			}
			else
			{
				g_volume = storage_get_talkvolume();
			}
		}
		ituWidgetSetVisible(BeCallHitBackground, false);
		ituWidgetSetVisible(BeCallBottomBackground, true);
	}
	printf(" BeCallShowButtomBackgroundOnMouseUp.........\n");

	return true;
}

/*************************************************
Function:		BeCallLayerButtonOnMouseUp
Description: 	右边按键弹起函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool BeCallLayerButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 btn_event = atoi(param);

	dprintf("btn_event: %d\n", btn_event);
	switch (btn_event)
	{
		case BeCallAnswerEvent:
			CallinAnswerCall();
			g_HandupDelay = 2;
			break;

		case BeCallLockEvent:
		case BeCallSnapEvent:
			if (g_SetVolume)
			{
				g_volumeticks = 0;
				g_SetVolume = FALSE;
				// 保存铃声或通话音量
				if (g_InterState == CALL_STATE_CALLING)
				{
					storage_set_volume(g_volume, storage_get_talkvolume(), storage_get_keykeep());
				}
				else
				{
					storage_set_volume(storage_get_ringvolume(), g_volume, storage_get_keykeep());
				}
				ituWidgetSetVisible(BeCallBottomBackground, false);
			}
			SetBeCallLockAndSnap(btn_event);
			break;

		case BeCallHandUpEvent:
			if (g_HandupDelay)
			{
				break;
			}
		case BeCallExitEvent:
			inter_hand_down();
			break;

		default:
			break;
	}
	return true;
}

/*************************************************
Function:		BeCallLayerOnEnter
Description:	被叫初始化处理函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool BeCallLayerOnEnter(ITUWidget* widget, char* param)
{
	g_BeCallLastTick = SDL_GetTicks();
	InitRightButton();
	ituWidgetSetVisible(BeCallHitBackground, false);
	ituWidgetSetVisible(BeCallBottomBackground, false);
	ituWidgetSetVisible(BeCallShowButtomContainer, false);
	ituTextSetString(BeCallTimeText, NULL);
	ituTextSetString(BecallCallNoText, NULL);
	ituTextSetString(BecallNewCallNoText, NULL);
	DrawStringHint();

	if (g_OperType == INTER_CALLOUT_E)
	{
		if (g_DevType == DEVICE_TYPE_MANAGER || g_DevType == DEVICE_TYPE_FENJI_NET)
		{
			g_StartCalloutTick = 1;
		}
	}

	if (TRUE == g_DrawVideo)
	{
		BackgroundDrawVideo_init("BeCallBackground");
	}
	
	return true;
}

/*************************************************
Function:		InitBeCallLayer
Description: 	被叫页面初始化
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void InitBeCallLayer(void)
{
	if (!BeCallLayer)
	{
		uint8 i;
		char callname[50];

		MainLayer = ituSceneFindWidget(&theScene, "mainLayer");
		assert(MainLayer);

		BeCallLayer = ituSceneFindWidget(&theScene, "BeCallLayer");
		assert(BeCallLayer);

		BeCallRightAnswerContainer = ituSceneFindWidget(&theScene, "BeCallRightAnswerContainer");
		assert(BeCallRightAnswerContainer);

		BeCallRightHandUpContainer = ituSceneFindWidget(&theScene, "BeCallRightHandUpContainer");
		assert(BeCallRightHandUpContainer);

		BeCallRightLockContainer = ituSceneFindWidget(&theScene, "BeCallRightLockContainer");
		assert(BeCallRightLockContainer);

		BeCallRightSnapContainer = ituSceneFindWidget(&theScene, "BeCallRightSnapContainer");
		assert(BeCallRightSnapContainer);

		BeCallBottomBackground = ituSceneFindWidget(&theScene, "BeCallBottomBackground");
		assert(BeCallBottomBackground);

		BeCallHitBackground = ituSceneFindWidget(&theScene, "BeCallHitBackground");
		assert(BeCallHitBackground);

		BeCallTimeText = ituSceneFindWidget(&theScene, "BeCallTimeText");
		assert(BeCallTimeText);

		BecallCallNoText = ituSceneFindWidget(&theScene, "BecallCallNoText");
		assert(BecallCallNoText);

		BecallNewCallNoText = ituSceneFindWidget(&theScene, "BecallNewCallNoText");
		assert(BecallNewCallNoText);

		BeCallHitSprite = ituSceneFindWidget(&theScene, "BeCallHitSprite");
		assert(BeCallHitSprite);

		BeCallSoundSprite = ituSceneFindWidget(&theScene, "BeCallSoundSprite");
		assert(BeCallSoundSprite);
		
		BeCallShowButtomContainer = ituSceneFindWidget(&theScene, "BeCallShowButtomContainer");
		assert(BeCallShowButtomContainer);

		BeCallRightSnapGrayBackground = ituSceneFindWidget(&theScene, "BeCallRightSnapGrayBackground");
		assert(BeCallRightSnapGrayBackground);

		BeCallRightSnapButton = ituSceneFindWidget(&theScene, "BeCallRightSnapButton");
		assert(BeCallRightSnapButton);

		BeCallSoundTrackBar = ituSceneFindWidget(&theScene, "BeCallSoundTrackBar");
		assert(BeCallSoundTrackBar);

		for (i = 0; i < 3; i++)
		{
			memset(callname, 0, sizeof(callname));
			sprintf(callname, "%s%d", "BeCallRightNullContainer", i);
			BeCallRightNullContainer[i] = ituSceneFindWidget(&theScene, callname);
			assert(BeCallRightNullContainer[i]);

			memset(callname, 0, sizeof(callname));
			sprintf(callname, "%s%d", "BeCallRightNullButton", i);
			BeCallRightNullButton[i] = ituSceneFindWidget(&theScene, callname);
			assert(BeCallRightNullButton[i]);
		}
	}
}

/*************************************************
Function:		SetInterInfo
Description: 	设置对讲信息
Input:
1.DevType		设备类型
2.DevStr		设备编号
Output:			无
Return:			无
Others:			无
*************************************************/
static void SetInterInfo(INTER_OPER_TYPE OperType, DEVICE_TYPE_E DevType, char * DevStr)
{
	g_OperType = OperType;
	g_DevType = DevType;
	memset(g_DevNo, 0, sizeof(g_DevNo));
	strcpy(g_DevNo, DevStr);
	memset(g_NewCallNo, 0, sizeof(g_NewCallNo));

	if (g_DevType == DEVICE_TYPE_STAIR || g_DevType == DEVICE_TYPE_AREA
		|| g_DevType == DEVICE_TYPE_DOOR_NET || g_DevType == DEVICE_TYPE_DOOR_PHONE)
	{
		g_DrawVideo = TRUE;
		g_unlock = TRUE;
	}
	else
	{
		g_DrawVideo = FALSE;
		g_unlock = FALSE;
	}
	
	if (g_OperType == INTER_CALLOUT_E)
	{
		if (g_DevType == DEVICE_TYPE_MANAGER || g_DevType == DEVICE_TYPE_FENJI_NET)
		{
			g_InterState = CALL_STATE_REQUEST;
		}
		else
		{
			g_InterState = CALL_STATE_CALLING;
		}
	}
	else
	{
		g_InterState = CALL_STATE_CALLING;
	}

	g_RemainTime = 0;
	g_SetVolume = FALSE;
	g_mute = FALSE;
	g_volume = storage_get_ringvolume();
	g_volumeticks = 0;
	g_MSGLockTicks = 0;
	g_MSGSnapTicks = 0;
	g_ErrHintTxtID = 0;
	g_ErrHintTicks = 0;
	g_StartCalloutTick = 0;
}

/*************************************************
Function:		BeCallWin
Description: 	被叫界面
Input:
1.info			参数
Output:			无
Return:			无
Others:			无
*************************************************/
void BeCallWin(INTER_INFO_S* info)
{
	INTER_INFO_S * pInterInfo = info;

	if (pInterInfo != NULL)
	{
		if (pInterInfo->InterType == INTER_CALLIN_E)
		{
			int32 ret = ui_show_win_arbitration(SYS_OPER_CALLIN);
			if (ret == FALSE)
			{
				return;
			}
		}
		SetInterInfo(pInterInfo->InterType, pInterInfo->DevType, pInterInfo->DevStr);
	}
	else
	{
		return;
	}
	InitBeCallLayer();
	ituLayerGoto(BeCallLayer);
}

/*************************************************
Function:		BeCallReset
Description: 	被叫界面初始化函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
void BeCallReset(void)
{
	BeCallLayer = NULL;
}

