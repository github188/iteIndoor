/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_ip_protocol.c
Author:     	zxc
Version:
Date: 		2016-07-28
Description:
History:
1. Date:
Author:
Modification:
*************************************************/
#include "gui_include.h"

#define MAX_SHOW_FENJI_NUM			10					// UI最大支持显示几个分机

static ITUText* SetIpProtocolBinding2Text = NULL;
//static ITUText* SetIpProtocolBindNum1Text = NULL;
static ITUText* SetIpProtocolBindNum2Text = NULL;
static ITUText* SetIpProtocolIP2Text = NULL;

static ITUCoverFlow* SetIpProtocolCoverFlow = NULL;
static ITUCalendar* SetIpProtocolListContainer[3] = { NULL };
static ITUText* SetIpProtocolFJInfo1Text[MAX_SHOW_FENJI_NUM] = { NULL };
static ITUText* SetIpProtocolFJInfo2Text[MAX_SHOW_FENJI_NUM] = { NULL };
static ITUCalendar* SetIpProtocolFJInfoContainer[MAX_SHOW_FENJI_NUM] = { NULL };

static uint32 g_ip = 0;
static uint32 g_maincode = 0;					// 存放主机绑定码
//static uint32 g_extcode = 0;					// 与IP模块捆绑的验证码
static uint32 g_bindstatus; 					// 0 : 未绑定; 1 :已绑定 
//static uint8 g_host;							// TRUE : 主机;  FALSE : 分机
static IPMODULE_INFO ipmoduleinfo;
static IPAD_EXTENSION_LIST g_IpadList;


/*************************************************
Function:		show_win_bind
Description: 	显示绑定的具体信息
Input:		无
Output:		无
Return:	
Others:
*************************************************/
static void show_win_bind()
{
	uint8 bind_tmp[50];
	uint8 ip_tmp[50];
	uint32 text_id = 0;

	memset(bind_tmp, 0, sizeof(bind_tmp));
	memset(ip_tmp, 0, sizeof(ip_tmp));

	g_bindstatus = get_ipmodule_bindstate();

	if (g_bindstatus)
	{
		text_id = SID_Set_Bounded;
		g_maincode = get_ipmodule_bindcode();
		sprintf(bind_tmp, "%d", g_maincode);
		change_ip_to_str((get_ipmodule_addr()), ip_tmp);
	}
	else
	{
		text_id = SID_Set_Unbounded;
		sprintf(bind_tmp, "%d", g_maincode);
		change_ip_to_str(g_ip, ip_tmp);
	}
	
	ituTextSetString(SetIpProtocolBinding2Text, get_str(text_id));
	ituTextSetString(SetIpProtocolBindNum2Text, bind_tmp);
	ituTextSetString(SetIpProtocolIP2Text, ip_tmp);
}

/*************************************************
Function:		show_win_fenji
Description: 	显示分机的具体信息
Input:		无
Output:		无
Return:
Others:
*************************************************/
static void show_win_fenji()
{
	uint8 j = 0;
	uint32 extenNo = 0;
	char pExtIp[20] = { 0 };
	uint8 tmp[50] = { 0 };
	uint32 cOnlineID = 0;

	// 获取IPAD分机信息
	get_ipad_extension(&g_IpadList);

	for (j = 0; j < g_IpadList.count; j++)
	{
		extenNo = g_IpadList.ipadData[j].devno;
		memset(pExtIp, 0, sizeof(pExtIp));
		change_ip_to_str(g_IpadList.ipadData[j].ipAddr, pExtIp);
		if (g_IpadList.ipadData[j].state == 1)
		{
			cOnlineID = get_str(SID_Set_Online);
		}
		else
		{
			cOnlineID = get_str(SID_Set_Offline);
		}

		memset(tmp, 0, sizeof(tmp));
		if (g_bindstatus)
		{
			sprintf(tmp, "%s:%d (%s)", get_str(SID_Set_ExtenNo), extenNo, cOnlineID);
		}
		else
		{
			sprintf(tmp, "%s:%d", get_str(SID_Set_ExtenNo), extenNo);
		}
		ituTextSetString(SetIpProtocolFJInfo1Text[j], tmp);
		ituTextSetString(SetIpProtocolFJInfo2Text[j], pExtIp);
	}

	
	if (g_IpadList.count > 0)
	{
		ituWidgetSetVisible(SetIpProtocolListContainer[1], true);
		if (g_IpadList.count > 5)
		{
			ituWidgetSetVisible(SetIpProtocolListContainer[2], true);
		}
		else
		{
			ituWidgetSetVisible(SetIpProtocolListContainer[2], false);
		}
	}
	else
	{
		ituWidgetSetVisible(SetIpProtocolListContainer[1], false);
		ituWidgetSetVisible(SetIpProtocolListContainer[2], false);
	}
	for (j = 0; j < g_IpadList.count; j++)
	{
		ituWidgetSetVisible(SetIpProtocolFJInfoContainer[j], true);
		
	}
	for (j = g_IpadList.count; j < MAX_SHOW_FENJI_NUM; j++)
	{
		ituWidgetSetVisible(SetIpProtocolFJInfoContainer[j], false);
	}
}

/*************************************************
Function:		SetIpProtocolHostOnEnter
Description: 	进入ip模块设置界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetIpProtocolHostOnEnter(ITUWidget* widget, char* param)
{
	if (!SetIpProtocolBinding2Text)
	{
		uint8 i;
		char tmp[50] = { 0 };

		SetIpProtocolBinding2Text = ituSceneFindWidget(&theScene, "SetIpProtocolBinding2Text");
		assert(SetIpProtocolBinding2Text);

		//SetIpProtocolBindNum1Text = ituSceneFindWidget(&theScene, "SetIpProtocolBindNum1Text");
		//assert(SetIpProtocolBindNum1Text);

		SetIpProtocolBindNum2Text = ituSceneFindWidget(&theScene, "SetIpProtocolBindNum2Text");
		assert(SetIpProtocolBindNum2Text);

		SetIpProtocolIP2Text = ituSceneFindWidget(&theScene, "SetIpProtocolIP2Text");
		assert(SetIpProtocolIP2Text);

		SetIpProtocolCoverFlow = ituSceneFindWidget(&theScene, "SetIpProtocolCoverFlow");
		assert(SetIpProtocolCoverFlow);

		for (i = 0; i < 3; i++)
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "SetIpProtocolListContainer", i);
			SetIpProtocolListContainer[i] = ituSceneFindWidget(&theScene, tmp);
			assert(SetIpProtocolListContainer[i]);
		}

		for (i = 0; i < MAX_SHOW_FENJI_NUM; i++)
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "SetIpProtocolFJInfo1Text", i);
			SetIpProtocolFJInfo1Text[i] = ituSceneFindWidget(&theScene, tmp);
			assert(SetIpProtocolFJInfo1Text[i]);

			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "SetIpProtocolFJInfo2Text", i);
			SetIpProtocolFJInfo2Text[i] = ituSceneFindWidget(&theScene, tmp);
			assert(SetIpProtocolFJInfo2Text[i]);

			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "SetIpProtocolFJInfoContainer", i);
			SetIpProtocolFJInfoContainer[i] = ituSceneFindWidget(&theScene, tmp);
			assert(SetIpProtocolFJInfoContainer[i]);
		}

		//g_maincode = get_ipmodule_bindcode();
		g_maincode = 0;
		g_ip = 0;
		//g_host = is_main_DeviceNo();
		show_win_bind();
		show_win_fenji();
		ituCoverFlowGoto(SetIpProtocolCoverFlow, 0);
	}

	return true;
}

/*************************************************
Function:		SetIpProtocolBindNumButtonOnMouseUp
Description: 	生成绑定码钮按下抬起执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetIpProtocolBindNumButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint64 code;
	uint8 tmp[50];

	srand(time(NULL));
	code = rand() % 6553600;
	if (code < 100000)
	{
		while (code < 100000)
		{
			code = code * 2;
		}
	}

	if (code > 1000000)
	{
		while (code > 1000000)
		{
			code = code / 2;
		}
	}
	g_maincode = code;
	ipmoduleinfo.BindCode = g_maincode;
	ipmoduleinfo.State = 0;
	ipmoduleinfo.IpAddr = 0;
	set_ipmodule_info(&ipmoduleinfo);

	sprintf(tmp, "%d", g_maincode);
	ituTextSetString(SetIpProtocolBindNum2Text, tmp);

	return true;
}

/*************************************************
Function:		SetIpProtocolGetFJButtonOnMouseUp
Description: 	获取分机钮按下抬起执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetIpProtocolGetFJButtonOnMouseUp(ITUWidget* widget, char* param)
{



	return true;
}
