/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_jd_yuyue.c
Author:     	zxc
Version:
Date: 		2016-10-09
Description: 家电预约主界面界面
*************************************************/
#include "../layer_set.h"

static ITULayer* SetUserLayer = NULL;
static ITUText* SetJdYuYueTitleText = NULL;
static ITUCoverFlow* SetJdYuYueListCoverFlow = NULL;
static ITUCoverFlow* SetDelJdYuYueListCoverFlow = NULL;
static ITUContainer* SetJdYuYueNoneContainer = NULL;
static ITUText* SetJdYuYueDelText = NULL;
static ITUText* SetJdYuYueAdd1Text = NULL;
static ITUText* SetJdYuYueAdd2Text = NULL;

static ITUButton* YuyueDataListButton[MAX_YUYUE_NUM] = { NULL };
static ITUText* SetJdYuYueListData1Text = NULL;
static ITUText* SetJdYuYueListData2Text = NULL;
static ITUContainer* SetJdYuYueListDataContainer0 = NULL;

static ITUContainer* SetDelJdYuYueListDataContainer0 = NULL;
static ITUButton* SetDelJdYuYueListDataButton[MAX_YUYUE_NUM] = { NULL };
static ITUSprite* SetDelJdYuYueListDataSprite[MAX_YUYUE_NUM] = { NULL };
static ITUText* SetDelJdYuYueListData1Text = NULL;
static ITUText* SetDelJdYuYueListData2Text = NULL;

extern PBE_COMM_LIST JdYuyueList;					// 家电预约列表

static char YuyueRepStr[4][20] = { { 0 } };
static char YuyueWeekStr[7][10] = { { 0 } };

/*************************************************
Function:		get_yuyue_str
Description: 	根据字符串ID取字符串
Input:
1.hDlg
Output:		无
Return:		无
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
	ITUContainer* CloneChildContainer = NULL;
	ITUBackground* LineBackground = NULL;

	result = ituWidgetClone(SetJdYuYueListDataContainer0, &CloneChildContainer);
	if (result)
	{
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "SetJdYuYueListDataContainer", num);
		ituWidgetSetName(CloneChildContainer, tmp);
		ituWidgetSetX(CloneChildContainer, 0);
		ituWidgetSetY(CloneChildContainer, 0);

		YuyueDataListButton[num] = itcTreeGetChildAt(CloneChildContainer, 0);
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "SetJdYuYueListDataButton", num);
		ituWidgetSetName(YuyueDataListButton[num], tmp);

		SetJdYuYueListData1Text = itcTreeGetChildAt(CloneChildContainer, 1);
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "SetJdYuYueListData1Text", num);
		ituWidgetSetName(SetJdYuYueListData1Text, tmp);

		SetJdYuYueListData2Text = itcTreeGetChildAt(CloneChildContainer, 2);
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "SetJdYuYueListData2Text", num);
		ituWidgetSetName(SetJdYuYueListData1Text, tmp);

		LineBackground = itcTreeGetChildAt(CloneChildContainer, 3);
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "SetJdYuYueListDataLineBackground", num);
		ituWidgetSetName(LineBackground, tmp);

		ituWidgetAdd(SetJdYuYueListCoverFlow, CloneChildContainer);
		ituCoverFlowUpdate((ITUWidget*)SetJdYuYueListCoverFlow, ITU_EVENT_LAYOUT, 0, 0, 0);
	}
}

/*************************************************
Function:		show_yuyue_text1_text2
Description: 	显示预约记录列表内容
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void show_yuyue_text1_text2(uint8 i, ITUText* text1, ITUText* text2)
{
	char tmp[150] = { 0 };
	uint8 type, index;
	BESPOKE_TYPE betime;
	uint16 bewday, bemday;
	char  num[20] = { 0 };

	type = JdYuyueList->be_comm[i].Type;		// 设备类型
	index = JdYuyueList->be_comm[i].Index;		// 设备索引
	betime = JdYuyueList->be_comm[i].BeTime.BeType;
	bewday = JdYuyueList->be_comm[i].BeTime.tm_wday + 1;
	bemday = JdYuyueList->be_comm[i].BeTime.tm_mday + 1;

	ituTextSetString(text1, JdYuyueList->be_comm[i].Name);

	memset(tmp, 0, sizeof(tmp));
	memcpy(tmp, YuyueRepStr[betime], sizeof(YuyueRepStr[0]));

	if (BE_WEEK == betime)					// 每周
	{
		strcat(tmp, YuyueWeekStr[bewday]);
	}
	else if (BE_MONTH == betime)				// 每月
	{
		memset(num, 0, sizeof(num));
		sprintf(num, "%02u", bemday);
		strcat(tmp, num);
		strcat(tmp, YuyueRepStr[3]);   // 日
	}

	sprintf(num, " %02u:%02u ", JdYuyueList->be_comm[i].BeTime.tm_hour, JdYuyueList->be_comm[i].BeTime.tm_min);
	strcat(tmp, num);

	memset(num, 0, sizeof(num));
	if (JdYuyueList->be_comm[i].Action)
	{
		sprintf(num, "%s", get_str(SID_Jd_Yuyue_Action_Open));
	}
	else
	{
		sprintf(num, "%s", get_str(SID_Jd_Yuyue_Action_Close));
	}
	strcat(tmp, num);

	ituTextSetString(text2, tmp);
}

/*************************************************
Function:		update_listview
Description: 	显示预约记录
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void update_listview()
{
	uint8 i = 0;
	char tmp[150] = { 0 };
	ITUContainer* YuyueListContainerFlag = NULL;

	for (i = 1; i < MAX_YUYUE_NUM; i++)
	{
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "SetJdYuYueListDataContainer", i);
		YuyueListContainerFlag = (ITUContainer*)ituSceneFindWidget(&theScene, tmp);
		if (NULL != YuyueListContainerFlag)							//判断是否clone过
		{
			itcTreeRemove(YuyueListContainerFlag);						// 删除已有的
			ituWidgetExit(YuyueListContainerFlag);
			YuyueListContainerFlag = NULL;
			YuyueDataListButton[i] = NULL;
			debug_log("Remove a clone list background!!!\n");
		}
	}

	if (JdYuyueList->nCount >= MAX_YUYUE_NUM) //条数大于20条
	{
		ituSetColor(&((ITUWidget*)SetJdYuYueAdd1Text)->color, 255, 128, 138, 135);//冷灰
		ituWidgetSetVisible(SetJdYuYueAdd2Text, true);
		ituWidgetSetY(SetJdYuYueAdd1Text, 0);
	}
	else
	{
		ituSetColor(&((ITUWidget*)SetJdYuYueAdd1Text)->color, 255, 255, 255, 255);
		ituWidgetSetVisible(SetJdYuYueAdd2Text, false);
		ituWidgetSetY(SetJdYuYueAdd1Text, 14);
	}

	if (0 == JdYuyueList->nCount)
	{
		ituWidgetSetVisible(SetJdYuYueNoneContainer, true);
		ituWidgetSetVisible(SetJdYuYueListDataContainer0, false);
		ituSetColor(&((ITUWidget*)SetJdYuYueDelText)->color, 255, 128, 138, 135);//冷灰
	}
	else
	{
		ituWidgetSetVisible(SetJdYuYueListDataContainer0, true);
		ituWidgetSetVisible(SetJdYuYueNoneContainer, false);
		ituSetColor(&((ITUWidget*)SetJdYuYueDelText)->color, 255, 255, 255, 255);

		for (i = 0; i < JdYuyueList->nCount; i++)
		{
			if (0 == i)
			{
				YuyueDataListButton[0] = ituSceneFindWidget(&theScene, "SetJdYuYueListDataButton0");
				assert(YuyueDataListButton[0]);

				SetJdYuYueListData1Text = ituSceneFindWidget(&theScene, "SetJdYuYueListData1Text0");
				assert(SetJdYuYueListData1Text);

				SetJdYuYueListData2Text = ituSceneFindWidget(&theScene, "SetJdYuYueListData2Text0");
				assert(SetJdYuYueListData2Text);
			}
			else
			{
				clone_one_list(i);
			}

			show_yuyue_text1_text2(i, SetJdYuYueListData1Text, SetJdYuYueListData2Text);
		}
	}

	ituCoverFlowUpdate((ITUWidget*)SetJdYuYueListCoverFlow, ITU_EVENT_LAYOUT, 0, 0, 0);
	ituCoverFlowGoto(SetJdYuYueListCoverFlow, 0);
	ituTextSetString(SetJdYuYueTitleText, get_str(SID_Jd_Yuyue));
	ituWidgetSetVisible(SetDelJdYuYueListCoverFlow, false);
	ituWidgetSetVisible(SetJdYuYueListCoverFlow, true);
}

/*************************************************
Function:		SetJdYuYueOnEnter
Description: 	进入界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJdYuYueOnEnter(ITUWidget* widget, char* param)
{
	if (!SetJdYuYueListCoverFlow)
	{
		SetJdYuYueListCoverFlow = ituSceneFindWidget(&theScene, "SetJdYuYueListCoverFlow");
		assert(SetJdYuYueListCoverFlow);

		SetJdYuYueTitleText = ituSceneFindWidget(&theScene, "SetJdYuYueTitleText");
		assert(SetJdYuYueTitleText);

		SetJdYuYueNoneContainer = ituSceneFindWidget(&theScene, "SetJdYuYueNoneContainer");
		assert(SetJdYuYueNoneContainer);

		SetJdYuYueListDataContainer0 = ituSceneFindWidget(&theScene, "SetJdYuYueListDataContainer0");
		assert(SetJdYuYueListDataContainer0);

		SetDelJdYuYueListCoverFlow = ituSceneFindWidget(&theScene, "SetDelJdYuYueListCoverFlow");
		assert(SetDelJdYuYueListCoverFlow);

		SetJdYuYueDelText = ituSceneFindWidget(&theScene, "SetJdYuYueDelText");
		assert(SetJdYuYueDelText);

		SetJdYuYueAdd1Text = ituSceneFindWidget(&theScene, "SetJdYuYueAdd1Text");
		assert(SetJdYuYueAdd1Text);

		SetJdYuYueAdd2Text = ituSceneFindWidget(&theScene, "SetJdYuYueAdd2Text");
		assert(SetJdYuYueAdd2Text);
	}

	storage_yuyue_init();

	get_yuyue_str();

	update_listview();

	return true;
}

/*************************************************
Function:		SetJdYuYueListDataButtonOnMouseUp
Description: 	预约列表按下
Input:		无
Output:		无 
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJdYuYueListDataButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 i;

	if (0 == JdYuyueList->nCount)
	{
		debug_log("JdYuyueList->nCount = 0 !!!\n");
		return false;
	}

	for (i = 0; i < JdYuyueList->nCount; i++)
	{
		if (YuyueDataListButton[i] == (ITUButton*)widget)
		{
			SetJdYuYueParamLayerOnShow(i + 1);
		}
	}

	return true;
}

/*************************************************
Function:		SetJdYuYueAddButtonOnMouseUp
Description: 	预约添加按钮
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJdYuYueAddButtonOnMouseUp(ITUWidget* widget, char* param)
{
	if (JdYuyueList->nCount >= MAX_YUYUE_NUM)
	{
		return true;
	}

	SetJdYuYueParamLayerOnShow(0);

	return true;
}

/*************************************************
Function:		clone_one_del_list
Description: 	clone列表控件
Input:		无
Output:		无
Return:		TRUE 成功 FALSE 失败
Others:
*************************************************/
static void clone_one_del_list(uint8 num)
{
	char tmp[100];
	bool result = FALSE;
	ITUContainer* CloneChildContainer = NULL;
	ITUBackground* LineBackground = NULL;
	ITUIcon* SetDelJdYuYueListDataIcon = NULL;

	result = ituWidgetClone(SetDelJdYuYueListDataContainer0, &CloneChildContainer);
	if (result)
	{
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "SetDelJdYuYueListDataContainer", num);
		ituWidgetSetName(CloneChildContainer, tmp);
		ituWidgetSetX(CloneChildContainer, 0);
		ituWidgetSetY(CloneChildContainer, 0);

		SetDelJdYuYueListDataButton[num] = itcTreeGetChildAt(CloneChildContainer, 0);
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "SetDelJdYuYueListDataButton", num);
		ituWidgetSetName(SetDelJdYuYueListDataButton[num], tmp);

		SetDelJdYuYueListData1Text = itcTreeGetChildAt(CloneChildContainer, 1);
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "SetDelJdYuYueListData1Text", num);
		ituWidgetSetName(SetDelJdYuYueListData1Text, tmp);

		SetDelJdYuYueListData2Text = itcTreeGetChildAt(CloneChildContainer, 2);
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "SetDelJdYuYueListData2Text", num);
		ituWidgetSetName(SetDelJdYuYueListData2Text, tmp);

		LineBackground = itcTreeGetChildAt(CloneChildContainer, 3);
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "SetDelJdYuYueListDataLineBackground", num);
		ituWidgetSetName(LineBackground, tmp);

		SetDelJdYuYueListDataSprite[num] = itcTreeGetChildAt(CloneChildContainer, 4);
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "SetDelJdYuYueListDataSprite", num);
		ituWidgetSetName(SetDelJdYuYueListDataSprite[num], tmp);

		SetDelJdYuYueListDataIcon = itcTreeGetChildAt(SetDelJdYuYueListDataSprite[num], 0);
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "SetDelJdYuYueListDataNIcon", num);
		ituWidgetSetName(SetDelJdYuYueListDataIcon, tmp);

		SetDelJdYuYueListDataIcon = itcTreeGetChildAt(SetDelJdYuYueListDataSprite[num], 1);
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "SetDelJdYuYueListDataYIcon", num);
		ituWidgetSetName(SetDelJdYuYueListDataIcon, tmp);

		ituWidgetAdd(SetDelJdYuYueListCoverFlow, CloneChildContainer);
		ituCoverFlowUpdate((ITUWidget*)SetDelJdYuYueListCoverFlow, ITU_EVENT_LAYOUT, 0, 0, 0);
	}
}

/*************************************************
Function:		show_del_yuyue_list
Description: 	显示要删除的预约记录
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void show_del_yuyue_list()
{
	uint8 i = 0;
	char tmp[150] = { 0 };
	uint8 type, index;
	BESPOKE_TYPE betime;
	uint16 bewday, bemday;
	char  num[20] = { 0 };
	ITUContainer* YuyueListContainerFlag = NULL;

	for (i = 1; i < MAX_YUYUE_NUM; i++)
	{
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "SetDelJdYuYueListDataContainer", i); 
		YuyueListContainerFlag = (ITUContainer*)ituSceneFindWidget(&theScene, tmp);
		if (NULL != YuyueListContainerFlag)							//判断是否clone过
		{
			itcTreeRemove(YuyueListContainerFlag);						// 删除已有的
			ituWidgetExit(YuyueListContainerFlag);
			YuyueListContainerFlag = NULL;
			SetDelJdYuYueListDataButton[i] = NULL;
			SetDelJdYuYueListDataSprite[i] = NULL;
			debug_log("Remove a clone list background!!!\n");
		}
	}

	if (0 == JdYuyueList->nCount)
	{
		debug_log("JdYuyueList->nCount = 0 !!!\n");
		return;
	}

	for (i = 0; i < JdYuyueList->nCount; i++)
	{
		if (0 == i)
		{
			SetDelJdYuYueListDataButton[0] = ituSceneFindWidget(&theScene, "SetDelJdYuYueListDataButton0");
			assert(SetDelJdYuYueListDataButton[0]);

			SetDelJdYuYueListData1Text = ituSceneFindWidget(&theScene, "SetDelJdYuYueListData1Text0");
			assert(SetDelJdYuYueListData1Text);

			SetDelJdYuYueListData2Text = ituSceneFindWidget(&theScene, "SetDelJdYuYueListData2Text0");
			assert(SetDelJdYuYueListData2Text);

			SetDelJdYuYueListDataSprite[0] = ituSceneFindWidget(&theScene, "SetDelJdYuYueListDataSprite0");
			assert(SetDelJdYuYueListDataSprite[0]);

			ituSpriteGoto(SetDelJdYuYueListDataSprite[0], 0);
		}
		else
		{
			clone_one_del_list(i);
		}

		show_yuyue_text1_text2(i, SetDelJdYuYueListData1Text, SetDelJdYuYueListData2Text);
	}
}

/*************************************************
Function:		SetJdYuYueDelButtonOnMouseUp
Description: 	预约删除按钮
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetJdYuYueDelButtonOnMouseUp(ITUWidget* widget, char* param)
{
	if (0 == JdYuyueList->nCount)
	{
		debug_log("JdYuyueList->nCount = 0 !!!\n");
		return true;
	}

	if (!SetDelJdYuYueListDataContainer0)
	{
		SetDelJdYuYueListDataContainer0 = ituSceneFindWidget(&theScene, "SetDelJdYuYueListDataContainer0");
		assert(SetDelJdYuYueListDataContainer0);
	}

	show_del_yuyue_list();

	ituCoverFlowGoto(SetDelJdYuYueListCoverFlow, 0);
	ituTextSetString(SetJdYuYueTitleText, get_str(SID_Jd_Yuyue_Del));
	ituWidgetSetVisible(SetDelJdYuYueListCoverFlow, true);
	ituWidgetSetVisible(SetJdYuYueListCoverFlow, false);
	
	return true;
}

/*************************************************
Function:		SetDelJdYuYueListDataListButtonOnMouseUp
Description: 	删除预约列表按下执行函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool SetDelJdYuYueListDataListButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 i;

	if (0 == JdYuyueList->nCount)
	{
		debug_log("JdYuyueList->nCount = 0 !!!\n");
		return true;
	}

	for (i = 0; i < JdYuyueList->nCount; i++)
	{
		if (SetDelJdYuYueListDataButton[i] == (ITUButton*)widget)
		{
			ituSpriteGoto(SetDelJdYuYueListDataSprite[i], 1);
		}
	}

	return true;
}

/*************************************************
Function:		SetDelJdYuYueListDataListButtonOnMouseUp
Description: 	删除预约界面删除键按下执行函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool SetDelJdYuYueDelButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 i;
	BE_DEL_LIST DelList;

	memset(&DelList, 0, sizeof(BE_DEL_LIST));

	for (i = 0; i < JdYuyueList->nCount; i++)
	{
		if (1 == SetDelJdYuYueListDataSprite[i]->frame)
		{
			DelList.DelFlg[i] = 1;
			DelList.Counts++;
		}
	}
	storage_del_yuyues(&DelList);

	storage_yuyue_init();

	if (0 == JdYuyueList->nCount)
	{
		update_listview();
	}
	else
	{
		show_del_yuyue_list();
	}

	return true;
}

/*************************************************
Function:		SetUserLayerOnReturn
Description: 	底部退出键按下执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
void SetJdYuYueLayerOnReturn(void)
{
	if (ituWidgetIsVisible(SetDelJdYuYueListCoverFlow))
	{
		update_listview();
		return;
	}
	else if (ituWidgetIsVisible(SetJdYuYueListCoverFlow))
	{
		if (!SetUserLayer)
		{
			SetUserLayer = ituSceneFindWidget(&theScene, "SetUserLayer");
			assert(SetUserLayer);
		}
		ituLayerGoto(SetUserLayer);
		return;
	}
}
