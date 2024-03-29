/*********************************************************
Copyright (C), 2015-2020, Aurine
File name:	layer_information.c
Author:		WuZ
Version:   	1.0
Date:		2016-06-30
Description: 信息操作函数
History:
1. Date:
Author:
Modification:
2. ...
*********************************************************/

#include "layer_information.h"

static ITULayer*		mainLayer = NULL;
static ITUWidget*		informationMSGContentContainer = NULL;
static ITUCoverFlow*	informationMSGListCoverFlow = NULL;
static ITUWidget*		informationPageContainer = NULL;
static ITUWidget*		informationEmptyContainer = NULL;
static ITUWidget*		informationPreContainer = NULL;
static ITUWidget*		informationNextContainer = NULL;
static ITUWidget*		informationDeleteContainer = NULL;
static ITUWidget*		informationNullContainer4 = NULL;
static ITUWidget*		informationNullContainer5 = NULL;
static ITUBackground*	informationBackground = NULL;
static ITUBackground*	informationTipsTransparencyBackground = NULL;
static ITUText*			informationTipsText = NULL;

//MSGList
static ITUWidget*		informationMSGListContainer = NULL;
static ITUIcon*			informationMSGListReadIcon = NULL;
static ITUIcon*			informationMSGListUnReadIcon = NULL;
static ITUText*			informationMSGListThemeText = NULL;
static ITUText*			informationMSGListTimeText = NULL;
static ITUText*			informationMSGListSenderText = NULL;

//MSGContent
static ITUTextBox*		informationMSGContentDetailTextBox = NULL;
static ITUBackground*	informationMSGContentImageIcon = NULL;
static ITUText*			informationMSGContentSenderText = NULL;
static ITUText*			informationMSGContentThemeText = NULL;
static ITUText*			informationMSGContentTimeText = NULL;


static uint8_t*	gInformationImageData;
static int		gInformationImageSize;
static char		gInformationImageFilePath[PATH_MAX];


bool informationLayerOnEnter(ITUWidget* widget, char* param)
{
	informationLayerInit(INFORMATION_LIST_PAGE);
	ituCoverFlowGoto(informationMSGListCoverFlow, 0);
	setInformationList();

	return true;
}


bool informationLayerOnLeave(ITUWidget* widget, char* param)
{
	printf("\informationLayerOnLeave!!!!!!!!!\n");

	return true;
}


void informationLayerInit(INFORMATION_PAGE_e page)
{
	if (!informationMSGContentContainer)
	{
		informationMSGContentContainer = ituSceneFindWidget(&theScene, "informationMSGContentContainer");
		assert(informationMSGContentContainer);
	}
	if (!informationMSGListCoverFlow)
	{
		informationMSGListCoverFlow = ituSceneFindWidget(&theScene, "informationMSGListCoverFlow");
		assert(informationMSGListCoverFlow);
	}
	if (!informationEmptyContainer)
	{
		informationEmptyContainer = ituSceneFindWidget(&theScene, "informationEmptyContainer");
		assert(informationEmptyContainer);
	}
	if (!informationDeleteContainer)
	{
		informationDeleteContainer = ituSceneFindWidget(&theScene, "informationDeleteContainer");
		assert(informationDeleteContainer);
	}
	if (!informationPreContainer)
	{
		informationPreContainer = ituSceneFindWidget(&theScene, "informationPreContainer");
		assert(informationPreContainer);
	}
	if (!informationNullContainer4)
	{
		informationNullContainer4 = ituSceneFindWidget(&theScene, "informationNullContainer4");
		assert(informationNullContainer4);
	}
	if (!informationNextContainer)
	{
		informationNextContainer = ituSceneFindWidget(&theScene, "informationNextContainer");
		assert(informationNextContainer);
	}
	if (!informationNullContainer5)
	{
		informationNullContainer5 = ituSceneFindWidget(&theScene, "informationNullContainer5");
		assert(informationNullContainer5);
	}
	if (!informationTipsTransparencyBackground)
	{
		informationTipsTransparencyBackground = ituSceneFindWidget(&theScene, "informationTipsTransparencyBackground");
		assert(informationTipsTransparencyBackground);
	}
	ituWidgetSetVisible(informationTipsTransparencyBackground, false);


	switch (page)
	{
	case INFORMATION_LIST_PAGE:
		ituWidgetSetVisible(informationMSGContentContainer, false);
		ituWidgetSetVisible(informationMSGListCoverFlow, true);

		ituWidgetSetVisible(informationDeleteContainer, false);
		ituWidgetSetVisible(informationEmptyContainer, true);

		ituWidgetSetVisible(informationPreContainer, false);
		ituWidgetSetVisible(informationNullContainer4, true);

		ituWidgetSetVisible(informationNextContainer, false);
		ituWidgetSetVisible(informationNullContainer5, true);
		break;

	case INFORMATION_CONTENT_PAGE:
		ituWidgetSetVisible(informationMSGListCoverFlow, false);
		ituWidgetSetVisible(informationMSGContentContainer, true);

		ituWidgetSetVisible(informationEmptyContainer, false);
		ituWidgetSetVisible(informationDeleteContainer, true);
		
		ituWidgetSetVisible(informationNullContainer4, false);
		ituWidgetSetVisible(informationPreContainer, true);

		ituWidgetSetVisible(informationNullContainer5, false);
		ituWidgetSetVisible(informationNextContainer, true);
		break;

	default:
		break;
	}
}

bool informationBtnOnClicked(ITUWidget* widget, char* param)
{
	uint8_t tmpIndex = (INFORMATION_BTN_e)atoi(param);

	switch (tmpIndex)
	{
	case INFORMATION_BTN_TETURN:
		informationReturnBtnOnClicked();
		break;

	case INFORMATION_BTN_EMPTY:
	case INFORMATION_BTN_DELETE:
		informationMsgBoxShow(tmpIndex);
		break;

	case INFORMATION_BTN_NEXT:
		informationNextMsgBtnOnClicked();
		break;

	case INFORMATION_BTN_PREVIOUS:
		informationPreMsgBtnOnClicked();
		break;

	default:
		break;
	}

	return true;
}

bool informationMsgBoxBtnOnClicked(ITUWidget* widget, char* param)
{
	switch (atoi(param))
	{
	case INFORMATION_MSG_BTN_CONFIRM:
		if (ituWidgetIsVisible(informationMSGListCoverFlow))
		{
			//TODO:这里写清空信息的存储操作
		}
		else
		{
			//TODO:这里写删除一条的操作，删除完退出信息内容界面！！！！！！！
			ituWidgetSetVisible(informationMSGContentContainer, false);
			ituWidgetSetVisible(informationMSGListCoverFlow, true);
		}
		break;

	default:
		break;
	}

	ituWidgetEnable(informationBackground);
	ituWidgetSetVisible(informationTipsTransparencyBackground, false);

	return true;
}

bool setInformationIsReaded(uint8_t index, INFORMATION_MSGICON_STATUS_e status)
{
	char tmpRead[50] = { 0 };
	char tmpUnread[50] = { 0 };

	memset(tmpRead, 0, sizeof(tmpRead));
	sprintf(tmpRead, "%s%d", "informationMSGListReadIcon", index);
	informationMSGListReadIcon = ituSceneFindWidget(&theScene, tmpRead);
	assert(informationMSGListReadIcon);

	memset(tmpUnread, 0, sizeof(tmpUnread));
	sprintf(tmpUnread, "%s%d", "informationMSGListUnReadIcon", index);
	informationMSGListUnReadIcon = ituSceneFindWidget(&theScene, tmpUnread);
	assert(informationMSGListUnReadIcon);

	switch (status)
	{	
	case INFORMATION_MSGICON_READED:
		ituWidgetSetVisible(informationMSGListUnReadIcon, false);
		ituWidgetSetVisible(informationMSGListReadIcon, true);
		break;

	case INFORMATION_MSGICON_UNREADED:
		ituWidgetSetVisible(informationMSGListReadIcon, false);
		ituWidgetSetVisible(informationMSGListUnReadIcon, true);
		break;

	case INFORMATION_MSGICON_HIDE:
		ituWidgetSetVisible(informationMSGListReadIcon, false);
		ituWidgetSetVisible(informationMSGListUnReadIcon, false);
		break;

	default:
		break;
	}

	return true;
}

bool setInformationListSender(uint8_t index, char* senderStr)
{
	char tmpStr[50] = { 0 };

	memset(tmpStr, 0, sizeof(tmpStr));
	sprintf(tmpStr, "%s%d", "informationMSGListSenderText", index);
	informationMSGListSenderText = ituSceneFindWidget(&theScene, tmpStr);
	assert(informationMSGListSenderText);

	ituTextSetString(informationMSGListSenderText, senderStr);

	//TODO: 可能要对传进来的指针数据进行释放，看后期数据如何传递！！！
	return true;
}

bool setInformationListTheme(uint8_t index, char* themeStr)
{
	char tmpStr[50] = { 0 };
	char tmp_theme[50] = { 0 };

	memset(tmpStr, 0, sizeof(tmpStr));
	sprintf(tmpStr, "%s%d", "informationMSGListThemeText", index);
	informationMSGListThemeText = ituSceneFindWidget(&theScene, tmpStr);
	assert(informationMSGListThemeText);

	if (strlen(themeStr) > MAX_INFORMATION_THEME_LEN)
	{
		memset(tmpStr, 0, sizeof(tmpStr));
		strncpy(tmp_theme, themeStr, MAX_INFORMATION_THEME_LEN);
		sprintf(tmpStr, "%s.", tmp_theme);
		ituTextSetString(informationMSGListThemeText, tmpStr);
	}
	else
	{
		ituTextSetString(informationMSGListThemeText, themeStr);
	}

	//TODO: 可能要对传进来的指针数据进行释放，看后期数据如何传递！！！
	return true;
}

bool setInformationListTime(uint8_t index, char* timeStr)
{
	char tmpStr[50] = { 0 };

	memset(tmpStr, 0, sizeof(tmpStr));
	sprintf(tmpStr, "%s%d", "informationMSGListTimeText", index);
	informationMSGListTimeText = ituSceneFindWidget(&theScene, tmpStr);
	assert(informationMSGListTimeText);

	ituTextSetString(informationMSGListTimeText, timeStr);

	//TODO: 可能要对传进来的指针数据进行释放，看后期数据如何传递！！！
	return true;
}


bool setInformationListIsVisible(uint8_t index, bool status)
{
	char tmpStr[50] = { 0 };

	if (index >= MAX_INFORMATION_NUM)
	{
		printf("setting index overflow!!!!!!!!!!");
		return false;
	}
	else
	{
		memset(tmpStr, 0, sizeof(tmpStr));
		sprintf(tmpStr, "%s%d", "informationMSGListContainer", index);
		informationMSGListContainer = ituSceneFindWidget(&theScene, tmpStr);
		assert(informationMSGListContainer);

		ituWidgetSetVisible(informationMSGListContainer, status);
	}
	return true;
}


bool setInformationListIsEnable(uint8_t index, bool status)
{
	char tmpStr[50] = { 0 };

	if (index >= MAX_INFORMATION_NUM)
	{
		printf("setting index overflow!!!!!!!!!!");
		return false;
	}
	else
	{
		memset(tmpStr, 0, sizeof(tmpStr));
		sprintf(tmpStr, "%s%d", "informationMSGListContainer", index);
		informationMSGListContainer = ituSceneFindWidget(&theScene, tmpStr);
		assert(informationMSGListContainer);

		if (status)
		{
			ituWidgetEnable(informationMSGListContainer);
		}
		else
		{
			ituWidgetDisable(informationMSGListContainer);
		}
	}
	return true;
}


bool informationMSGListClicked(ITUWidget* widget, char* param)
{
	informationLayerInit(INFORMATION_CONTENT_PAGE);
	setInformationContent(atoi(param));

	return true;
}


void setInformationContent(uint8_t index)
{
	printf("Information List clieked %d", index);

	//TODO: 按照index读取存储相应信息、并且将状态置成已读(置标志位要在进信息内容框才置位，可以统一和上下条操作一起执行！！！！！)
	char tmpStr[500] = { 0 };

	memset(tmpStr, 0, sizeof(tmpStr));
	sprintf(tmpStr, "%s%d", "Center", index);
	setInformationContentSender(tmpStr);

	memset(tmpStr, 0, sizeof(tmpStr));
	sprintf(tmpStr, "%s %d", "0123456789012345678901234567890123456", index);
	setInformationContentTheme(tmpStr);

	memset(tmpStr, 0, sizeof(tmpStr));
	sprintf(tmpStr, "%s%d", "2016-07-06 12:34:", index);
	setInformationContentTime(tmpStr);

	memset(tmpStr, 0, sizeof(tmpStr));
	sprintf(tmpStr, "%s", "012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789");
	setInformationContentDetail(tmpStr);

	if (index == 0)
	{
		strcpy(gInformationImageFilePath, CFG_PRIVATE_DRIVE ":res/wallpaper/bk_01.jpg");
	}
	else if (index == 1)
	{
		strcpy(gInformationImageFilePath, CFG_PRIVATE_DRIVE ":res/wallpaper/bk_02.jpg");
	}
	else if (index == 2)
	{
		strcpy(gInformationImageFilePath, CFG_PRIVATE_DRIVE ":res/wallpaper/bk_04.jpg");
	}
	else if (index == 3)
	{
		strcpy(gInformationImageFilePath, CFG_PRIVATE_DRIVE ":res/wallpaper/bk_03.jpg");
	}
	setInformationContentImage(gInformationImageFilePath);
}


void setInformationContentTheme(char* themeStr)
{
	if (!informationMSGContentThemeText)
	{
		informationMSGContentThemeText = ituSceneFindWidget(&theScene, "informationMSGContentThemeText");
		assert(informationMSGContentThemeText);
	}
	ituTextSetString(informationMSGContentThemeText, themeStr);
}


void setInformationContentSender(char* senderStr)
{
	if (!informationMSGContentSenderText)
	{
		informationMSGContentSenderText = ituSceneFindWidget(&theScene, "informationMSGContentSenderText");
		assert(informationMSGContentSenderText);
	}
	ituTextSetString(informationMSGContentSenderText, senderStr);
}


void setInformationContentTime(char* timeStr)
{
	if (!informationMSGContentTimeText)
	{
		informationMSGContentTimeText = ituSceneFindWidget(&theScene, "informationMSGContentTimeText");
		assert(informationMSGContentTimeText);
	}
	ituTextSetString(informationMSGContentTimeText, timeStr);
}


bool setInformationContentImage(char* imageStr)
{
	FILE*	tmpFile;

	if (!informationMSGContentImageIcon)
	{
		informationMSGContentImageIcon = ituSceneFindWidget(&theScene, "informationMSGContentImageIcon");
		assert(informationMSGContentImageIcon);
	}

	// try to load minipic jpeg file if exists
	tmpFile = fopen(imageStr, "rb");
	if (tmpFile)
	{
		struct stat sb;
		if (fstat(fileno(tmpFile), &sb) != -1)			//用_fileno代替fileno避免运行时候的警告！！！！（但是板子上编译不过！！！！）
		{
			gInformationImageSize = (int)sb.st_size;
			gInformationImageData = malloc(gInformationImageSize);
			if (gInformationImageSize)
			{
				gInformationImageSize = fread(gInformationImageData, 1, gInformationImageSize, tmpFile);
			}
		}
		fclose(tmpFile);
	}
	else
	{
		printf("open  minipic jepg icon icon failed!");
		ituWidgetSetVisible(informationMSGContentImageIcon, false);
		return false;
	}
	if (gInformationImageData)
	{
		ituIconLoadJpegData((ITUIcon*)informationMSGContentImageIcon, gInformationImageData, gInformationImageSize);
		return true;
	}
	else
	{
		printf("load minipic jepg icon failed!");
		ituWidgetSetVisible(informationMSGContentImageIcon, false);
		return false;
	}
}


void setInformationContentDetail(char* detailStr)
{
	if (!informationMSGContentDetailTextBox)
	{
		informationMSGContentDetailTextBox = ituSceneFindWidget(&theScene, "informationMSGContentDetailTextBox");
		assert(informationMSGContentDetailTextBox);
	}
	ituTextBoxSetString(informationMSGContentDetailTextBox, detailStr);
}


void informationReturnBtnOnClicked()
{
	printf("Information Return BTN clieked ");

	if (ituWidgetIsVisible(informationMSGContentContainer))
	{
		/*ituWidgetSetVisible(informationMSGContentContainer, false);
		ituWidgetSetVisible(informationMSGListCoverFlow, true);*/
		informationLayerInit(INFORMATION_LIST_PAGE);
	}
	else
	{
		if (!mainLayer)
		{
			mainLayer = ituSceneFindWidget(&theScene, "mainLayer");
			assert(mainLayer);
		}
		ituLayerGoto(mainLayer);
	}
}


void informationNextMsgBtnOnClicked()
{
	printf("informationNextMsgBTNOnClicked 111111111111111111111111111111");
}


void informationPreMsgBtnOnClicked()
{
	printf("informationPreMsgBTNOnClicked 111111111111111111111111111111");
}


void informationMsgBoxShow(INFORMATION_BTN_e index)
{
	if (true)		//TODO:信息条数大于0 才显示消息框
	{
		return;
	}

	if (!informationTipsText)
	{
		informationTipsText = ituSceneFindWidget(&theScene, "informationTipsText");
		assert(informationTipsText);
	}

	if (!informationBackground)
	{
		informationBackground = ituSceneFindWidget(&theScene, "informationBackground");
		assert(informationBackground);
	}
	//TODO:添加语言翻译！
	switch (index)
	{
	case INFORMATION_BTN_EMPTY:
		ituTextSetString(informationTipsText, get_str(SID_Bj_Query_Del_Rec_All));
		break;

	case INFORMATION_BTN_DELETE:
		ituTextSetString(informationTipsText, get_str(SID_Bj_Query_Del_Rec_One));
		break;

	default:
		break;
	}

	ituWidgetDisable(informationBackground);
	ituWidgetSetVisible(informationTipsTransparencyBackground, true);
}


void setInformationList()
{
	bool a = false;
	uint8_t i = 0;
	char tmpStr[50] = { 0 };
	uint8_t msgNum = 0;
	MSGLIST* tmpList;
	printf("1111111111111111111111111111111111111 = %d", msgNum);
	tmpList = storage_read_msg_list();
	msgNum = tmpList->ncount;
	printf("msgNum = %d", msgNum);

	//TODO: 读取存储内容设置列表信息！！！！！！！
	for (i = 0; i < MAX_INFORMATION_NUM; i++)
	{
		if (msgNum < INFORMATION_PER_PAGE)
		{
			if (i < msgNum)
			{
				setInformationIsReaded(i, tmpList->pinfo_data[i].is_unread);		//设置已读未读
				printf("unread = %d", tmpList->pinfo_data[i].is_unread);

				gb2312ToUtf8(tmpStr, strlen(tmpList->pinfo_data[i].des), tmpList->pinfo_data[i].des, strlen(tmpList->pinfo_data[i].des));
				setInformationListSender(i, tmpStr);								//设置信息发送者
				printf("des = %s", tmpStr);

				gb2312ToUtf8(tmpStr, strlen(tmpList->pinfo_data[i].Head.title), tmpList->pinfo_data[i].Head.title, strlen(tmpList->pinfo_data[i].Head.title));
				setInformationListTheme(i, tmpStr);									//设置信息主题
				printf("tittle = %s", tmpStr);

				zoneDateTimeToString(tmpList->pinfo_data[i].time, tmpStr);			
				printf("time = %s", tmpStr);
				setInformationListTime(i, tmpStr);									//设置信息时间

				setInformationListIsEnable(i, true);
			}
			else if (i >= msgNum && i < INFORMATION_PER_PAGE)
			{
				setInformationIsReaded(i, INFORMATION_MSGICON_HIDE);
				setInformationListSender(i, "");
				setInformationListTheme(i, "");
				setInformationListTime(i, "");
				setInformationListIsEnable(i, false);
			}
			else
			{
				setInformationListIsVisible(i, false);
			}
		}
		else
		{
			if (i < msgNum)
			{
				setInformationIsReaded(i, tmpList->pinfo_data[i].is_unread);		//设置已读未读
				printf("unread = %d", tmpList->pinfo_data[i].is_unread);

				setInformationListSender(i, tmpList->pinfo_data[i].des);			//设置信息发送者
				printf("des = %s", tmpList->pinfo_data[i].des);

				setInformationListTheme(i, tmpList->pinfo_data[i].Head.title);		//设置信息主题
				printf("tittle = %s", tmpList->pinfo_data[i].Head.title);

				zoneDateTimeToString(tmpList->pinfo_data[i].time, tmpStr);
				printf("time = %s", tmpStr);
				setInformationListTime(i, tmpStr);									//设置信息时间

				setInformationListIsEnable(i, true);
			}
			else
			{
				setInformationListIsVisible(i, false);
			}
		}

	}
	//将没有内容的页面隐藏起来，达到不能滑动的效果！
	for (i = 0; i < NIFORMATION_PAGE_NUM; i++)
	{
		memset(tmpStr, 0, sizeof(tmpStr));
		sprintf(tmpStr, "%s%d", "informationPageContainer", i);
		informationPageContainer = ituSceneFindWidget(&theScene, tmpStr);
		assert(informationPageContainer);

		if (i <= (msgNum / INFORMATION_PER_PAGE))
		{
			if ((i == (msgNum / INFORMATION_PER_PAGE)) && (msgNum % INFORMATION_PER_PAGE) == 0)
			{
				if (msgNum == 0)
				{	
					ituWidgetSetVisible(informationPageContainer, true);
				}
				else
				{
					ituWidgetSetVisible(informationPageContainer, false);
				}
			}
			else
			{
				ituWidgetSetVisible(informationPageContainer, true);
			}
		}
		else
		{
			ituWidgetSetVisible(informationPageContainer, false);

		}
	}

	free(tmpList);
}

void informationLayerReset()
{
	mainLayer = NULL;
	informationMSGContentContainer	= NULL;
	informationMSGListCoverFlow		= NULL;
}