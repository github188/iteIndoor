/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_jd_ir.c
Author:     	zxc
Version:    	
Date: 		2016-08-22
Description: 空调红外学习
History:
1. Date:
Author:
Modification:
*************************************************/
#include "../layer_set.h"

#define SET_JD_DEV_NAME_ADDR_LIST_MAX			60					// 显示的列表最大条数

typedef enum
{
	LE_MODE_1 = 0,				// 制冷
	LE_MODE_2 = 1,				// 制暖
	LE_MODE_3 = 2,				// 模式
}KONGTIAO_LEARN_TYPE;

static ITUText* SetJDLearnIRType2Text = NULL;
static ITUText* SetJDLearnIRProgram2Text = NULL;
static ITULayer* SetJDLearnIRLayer = NULL;
static ITULayer* SetJdParamLayer = NULL;
static ITUBackground* SetJDLearnIRBackground = NULL;
static ITUBackground* SetJDLearnIRMsg0Background = NULL;
static ITUBackground* SetJDLearnIRMsg1Background = NULL;
static ITUBackground* SetJDLearnIRMsg2Background = NULL;
static ITURadioBox* SetJDLearnIRMsg0RadioBox[3] = { NULL };
static ITUCoverFlow* SetJDLearnIRMsg1ListCoverFlow = NULL;
static ITUSprite* SetJDLearnIRMsg1Sprite[10] = { NULL };
static ITURadioBox* SetJDLearnIRMsg2RadioBox[5] = { NULL };

static JD_DEV_INFO g_devinfo;
static KONGTIAO_LEARN_TYPE g_xuexi_type = 0;
static int g_select_index = 0;
static int g_mode_index = 0;
static uint8 g_xuexi_flag = 0;
static int g_succse_flag = 0;

#ifdef _AURINE_ELEC_NEW_
#define GUI_TIMER_ID			1
static void Kongtiao_Learn_Timer(void);
static void auJiaDianStateChanged(uint8 devtype, uint8 devindex, uint8 param2);
#endif

/*************************************************
Function:		DetJDLearnIROnEnter
Description: 	界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJDLearnIROnEnter(ITUWidget* widget, char* param)
{
	if (!SetJDLearnIRType2Text)
	{
		SetJDLearnIRType2Text = ituSceneFindWidget(&theScene, "SetJDLearnIRType2Text");
		assert(SetJDLearnIRType2Text);

		SetJDLearnIRProgram2Text = ituSceneFindWidget(&theScene, "SetJDLearnIRProgram2Text");
		assert(SetJDLearnIRProgram2Text);

		SetJDLearnIRBackground = ituSceneFindWidget(&theScene, "SetJDLearnIRBackground");
		assert(SetJDLearnIRBackground);

		SetJDLearnIRMsg0Background = ituSceneFindWidget(&theScene, "SetJDLearnIRMsg0Background");
		assert(SetJDLearnIRMsg0Background);

		SetJDLearnIRMsg1Background = ituSceneFindWidget(&theScene, "SetJDLearnIRMsg1Background");
		assert(SetJDLearnIRMsg1Background);

		SetJDLearnIRMsg2Background = ituSceneFindWidget(&theScene, "SetJDLearnIRMsg2Background");
		assert(SetJDLearnIRMsg2Background);
	}

	//if (strcmp(param, "SetJDLayer") == 0)
	{
		ituWidgetSetVisible(SetJDLearnIRMsg0Background, false);
		ituWidgetSetVisible(SetJDLearnIRMsg1Background, false);
		ituWidgetSetVisible(SetJDLearnIRMsg2Background, false);
		ituWidgetSetVisible(SetJDLearnIRBackground, true);
		if (!ituWidgetIsEnabled(SetJDLearnIRBackground))
		{
			ituWidgetEnable(SetJDLearnIRBackground);
		}
	}

	return true;
}

/*************************************************
Function:		SetJDLearnIRTypeButtonOnMouseUp
Description: 	类型按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJDLearnIRTypeButtonOnMouseUp(ITUWidget* widget, char* param)
{
	if (!SetJDLearnIRMsg0RadioBox[0])
	{
		uint8 textname[50];
		uint8 i;
		for (i = 0; i < 3; i++)
		{
			memset(textname, 0, sizeof(textname));
			sprintf(textname, "%s%d", "SetJDLearnIRMsg0RadioBox", i);
			SetJDLearnIRMsg0RadioBox[i] = ituSceneFindWidget(&theScene, textname);
			assert(SetJDLearnIRMsg0RadioBox[i]);
		}
	}

	ituRadioBoxSetChecked(SetJDLearnIRMsg0RadioBox[g_xuexi_type], true);

	ituWidgetDisable(SetJDLearnIRBackground);
	ituWidgetSetVisible(SetJDLearnIRMsg0Background, true);

	return true;
}

/*************************************************
Function:		SetJDLearnIRTypeMsgListButtonOnMouseUp
Description: 	类型消息框列表按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJDLearnIRTypeMsgListButtonOnMouseUp(ITUWidget* widget, char* param)
{
	char tmp[50];
	g_xuexi_type = (KONGTIAO_LEARN_TYPE)atoi(param);

	ituTextSetString(SetJDLearnIRType2Text, get_str(SID_Jd_Button_Cold + g_xuexi_type));

	if (LE_MODE_3 == g_xuexi_type)
	{
		ituTextSetString(SetJDLearnIRProgram2Text, get_str(SID_Jd_KongTiao_Refre));
		g_mode_index = 0;
	}
	else
	{
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s", "21℃");
		ituTextSetString(SetJDLearnIRProgram2Text, tmp);
		g_select_index = 0;
	}
	
	ituWidgetSetVisible(SetJDLearnIRMsg0Background, false);
	ituWidgetEnable(SetJDLearnIRBackground);
	
	return true;
}

/*************************************************
Function:		SetJDLearnIRProgramShowTemp
Description: 	显示温度消息框选项
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void SetJDLearnIRProgramShowTemp()
{
	uint8 i;

	if (!SetJDLearnIRMsg1ListCoverFlow)
	{
		char textname[50];
		for (i = 0; i < 10; i++)
		{
			memset(textname, 0, sizeof(textname));
			sprintf(textname, "%s%d", "SetJDLearnIRMsg1Sprite", i);
			SetJDLearnIRMsg1Sprite[i] = ituSceneFindWidget(&theScene, textname);
			assert(SetJDLearnIRMsg1Sprite[i]);
		}

		SetJDLearnIRMsg1ListCoverFlow = ituSceneFindWidget(&theScene, "SetJDLearnIRMsg1ListCoverFlow");
		assert(SetJDLearnIRMsg1ListCoverFlow);
	}

	for (i = 0; i < 10; i++)
	{
		if (i == g_select_index)
		{
			ituSpriteGoto(SetJDLearnIRMsg1Sprite[i], 1);
		}
		else
		{
			ituSpriteGoto(SetJDLearnIRMsg1Sprite[i], 0);
		}
	}
	ituCoverFlowGoto(SetJDLearnIRMsg1ListCoverFlow, 0);
	ituWidgetDisable(SetJDLearnIRBackground);
	ituWidgetSetVisible(SetJDLearnIRMsg1Background, true);
}

/*************************************************
Function:		SetJDLearnIRProgramShowMode
Description: 	显示模式消息框选项
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void SetJDLearnIRProgramShowMode()
{
	if (!SetJDLearnIRMsg2RadioBox[0])
	{
		uint8 textname[50];
		uint8 i;
		for (i = 0; i < 5; i++)
		{
			memset(textname, 0, sizeof(textname));
			sprintf(textname, "%s%d", "SetJDLearnIRMsg2RadioBox", i);
			SetJDLearnIRMsg2RadioBox[i] = ituSceneFindWidget(&theScene, textname);
			assert(SetJDLearnIRMsg2RadioBox[i]);
		}
	}

	ituRadioBoxSetChecked(SetJDLearnIRMsg0RadioBox[g_mode_index], true);

	ituWidgetDisable(SetJDLearnIRBackground);
	ituWidgetSetVisible(SetJDLearnIRMsg2Background, true);
}

/*************************************************
Function:		SetJDLearnIRProgramButtonOnMouseUp
Description: 	项目按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJDLearnIRProgramButtonOnMouseUp(ITUWidget* widget, char* param)
{
	if (LE_MODE_3 == g_xuexi_type)
	{
		SetJDLearnIRProgramShowMode();
	}
	else
	{
		SetJDLearnIRProgramShowTemp();
	}

	return true;
}

/*************************************************
Function:		SetJDLearnIRProgramMsgTempListButtonOnMouseUp
Description: 	项目温度消息框列表按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJDLearnIRProgramMsgTempListButtonOnMouseUp(ITUWidget* widget, char* param)
{
	char tmp[50];
	g_select_index = atoi(param);

	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp, "%d%s", 21 + g_select_index, "℃");
	ituTextSetString(SetJDLearnIRProgram2Text, tmp);

	ituWidgetSetVisible(SetJDLearnIRMsg1Background, false);
	ituWidgetEnable(SetJDLearnIRBackground);

	return true;
}

/*************************************************
Function:		SetJDLearnIRProgramMsgModeListButtonOnMouseUp
Description: 	项目模式消息框列表按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJDLearnIRProgramMsgModeListButtonOnMouseUp(ITUWidget* widget, char* param)
{
	g_mode_index = atoi(param);

	ituTextSetString(SetJDLearnIRProgram2Text, get_str(SID_Jd_KongTiao_Refre + g_mode_index));

	ituWidgetSetVisible(SetJDLearnIRMsg2Background, false);
	ituWidgetEnable(SetJDLearnIRBackground);

	return true;
}

/*************************************************
Function:		SetJDLearnIRStartButtonOnMouseUp
Description: 	开始学习按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJDLearnIRStartButtonOnMouseUp(ITUWidget* widget, char* param)
{
	if (g_xuexi_flag == 0)
	{
		g_xuexi_flag = 1;
		g_succse_flag = 0;
		switch (g_xuexi_type)
		{
		case LE_MODE_1:
			param = g_select_index + 1;
			break;

		case LE_MODE_2:
			param = g_select_index + 11;
			break;

		case LE_MODE_3:
			if (g_mode_index < 4)
			{
				param = g_mode_index + 22;
			}
			else
			{
				param = 21;
			}
			break;
		}
		jd_aurine_learn_ir(g_devinfo.Index, g_devinfo.Address, param);
		Kongtiao_Learn_Timer();
	}
	return true;
}

/*************************************************
Function:		SetJDLearnIRTestButtonOnMouseUp
Description: 	测试按键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJDLearnIRTestButtonOnMouseUp(ITUWidget* widget, char* param)
{
	if (g_xuexi_flag == 0)
	{
		switch (g_xuexi_type)
		{
		case LE_MODE_1:
			param = g_select_index + 1;
			break;

		case LE_MODE_2:
			param = g_select_index + 11;
			break;

		case LE_MODE_3:
			if (g_mode_index < 4)
			{
				param = g_mode_index + 22;
			}
			else
			{
				param = 21;
			}
			break;
		}
		jd_aurine_send_ir_to_dev(g_devinfo.Index, g_devinfo.Address, param);
	}

	return true;
}

/*************************************************
Function:			SetJDParam
Description: 		家电内容
Input:
1.hDlg:   		当前处理的窗口
Output:
Return:
Others:
*************************************************/
uint32 SetJDLearnIRParam(JD_DEV_INFO devinfo)
{
	if (!SetJDLearnIRLayer)
	{
		SetJDLearnIRLayer = ituSceneFindWidget(&theScene, "SetJDLearnIRLayer");
		assert(SetJDLearnIRLayer);
	}

	g_devinfo = devinfo;

	g_xuexi_type = 0;
	g_select_index = 0;
	g_mode_index = 0;

#ifdef _AURINE_ELEC_NEW_
	init_kt_learn_state_report((PAuJiaDianStateChanged)auJiaDianStateChanged);
#endif

	ituLayerGoto(SetJDLearnIRLayer);

	return TRUE;
}

/*************************************************
Function:		SetJDParamLayerOnReturn
Description: 	底部退出键按下执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
void SetJDLearnIRLayerOnReturn(void)
{
	if (ituWidgetIsVisible(SetJDLearnIRMsg0Background))
	{
		ituWidgetSetVisible(SetJDLearnIRMsg0Background, false);
		ituWidgetEnable(SetJDLearnIRBackground);
		return;
	}
	else if (ituWidgetIsVisible(SetJDLearnIRMsg1Background))
	{
		ituWidgetSetVisible(SetJDLearnIRMsg1Background, false);
		ituWidgetEnable(SetJDLearnIRBackground);
		return;
	}
	else if (ituWidgetIsVisible(SetJDLearnIRMsg2Background))
	{
		ituWidgetSetVisible(SetJDLearnIRMsg2Background, false);
		ituWidgetEnable(SetJDLearnIRBackground);
		return;
	}
	else if (ituWidgetIsVisible(SetJDLearnIRBackground))
	{
		if (!SetJdParamLayer)
		{
			SetJdParamLayer = ituSceneFindWidget(&theScene, "SetJdParamLayer");
			assert(SetJdParamLayer);
		}
		ituLayerGoto(SetJdParamLayer);
		return;
	}
}

#ifdef _AURINE_ELEC_NEW_
/*************************************************
Function:			learn_time_stop
Description: 		家电内容
Input:
1.hDlg:   		当前处理的窗口
Output:
Return:
Others:
*************************************************/
static void learn_time_stop(void)
{
#if 0
	g_xuexi_flag = 0;
	KillTimer(FormSetJDParam.hWnd, GUI_TIMER_ID);
	SendMessage(FrmMsgCtrl.hWnd, WM_MSG_Close, 0, 0);

	if (g_succse_flag == 1 && g_msg_hitflag == 1)
	{
		g_msg_hitflag = 0;
		show_msg(FormSetJDParam.hWnd, IDC_Msg_Notice, MSG_INFORMATION, SID_Jd_Set_KtLearn_Ok);
	}
	else if (g_succse_flag == 0)
	{
		show_msg(FormSetJDParam.hWnd, IDC_Msg_Notice, MSG_INFORMATION, SID_Jd_Set_KtLearn_Err);
	}
#endif
}

/*************************************************
Function:			Kongtiao_Learn_Timer
Description: 		家电内容
Input:
1.hDlg:   		当前处理的窗口
Output:
Return:
Others:
*************************************************/
static void Kongtiao_Learn_Timer(void)
{
#if 0
	KillTimer(FormSetJDParam.hWnd, GUI_TIMER_ID);
	show_msg(FormSetJDParam.hWnd, IDC_MSG_CTRL, MSG_HIT, SID_Jd_Kongtiao_Learning);
	SetTimer(FormSetJDParam.hWnd, GUI_TIMER_ID, 500);
#endif
}

/*************************************************
Function:			auJiaDianStateChanged
Description: 		家电内容
Input:
1.hDlg:   		当前处理的窗口
Output:
Return:
Others:
*************************************************/
static void auJiaDianStateChanged(uint8 devtype, uint8 devindex, uint8 param2)
{
	if (devtype != JD_DEV_KONGTIAO)
	{
		return;
	}

	if (g_xuexi_flag != 0)
	{
		dprintf("param2..........: %d\n", param2);
		if (param2 > 0)
		{
			g_succse_flag = 0;
		}
		else
		{
			g_succse_flag = 1;
			//g_msg_hitflag = 1;
		}
		learn_time_stop();
	}
}
#endif
