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

static ITULayer*		mainLayer;
static ITUWidget*		informationMSGContentContainer;
static ITUCoverFlow*	informationMSGListCoverFlow;
static ITUWidget*		informationPageContainer;
static ITUWidget*		informationEmptyContainer;
static ITUWidget*		informationPreContainer;
static ITUWidget*		informationNextContainer;
static ITUWidget*		informationDeleteContainer;
static ITUWidget*		informationNullContainer4;
static ITUWidget*		informationNullContainer5;
static ITUBackground*	informationBackground;
static ITUBackground*	informationTipsTransparencyBackground;
static ITUText*			informationTipsText;

//MSGList
static ITUWidget*		informationMSGListContainer;	
static ITUIcon*			informationMSGListReadIcon;
static ITUIcon*			informationMSGListUnReadIcon;
static ITUText*			informationMSGListThemeText;
static ITUText*			informationMSGListTimeText;
static ITUText*			informationMSGListSenderText;

//MSGContent
static ITUTextBox*		informationMSGContentDetailTextBox;
static ITUBackground*	informationMSGContentImageIcon;
static ITUText*			informationMSGContentSenderText;
static ITUText*			informationMSGContentThemeText;
static ITUText*			informationMSGContentTimeText;


static uint8_t*	gInformationImageData;
static int		gInformationImageSize;
static char		gInformationImageFilePath[PATH_MAX];


bool informationLayerOnEnter(ITUWidget* widget, char* param)
{
	informationLayerInit(INFORMATION_LIST_PAGE);
	ituCoverFlowGoto(informationMSGListCoverFlow, 0);
	setInformationList();

	return TRUE;
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
	ituWidgetSetVisible(informationTipsTransparencyBackground, FALSE);


	switch (page)
	{
	case INFORMATION_LIST_PAGE:
		ituWidgetSetVisible(informationMSGContentContainer, FALSE);
		ituWidgetSetVisible(informationMSGListCoverFlow, TRUE);

		ituWidgetSetVisible(informationDeleteContainer, FALSE);
		ituWidgetSetVisible(informationEmptyContainer, TRUE);

		ituWidgetSetVisible(informationPreContainer, FALSE);
		ituWidgetSetVisible(informationNullContainer4, TRUE);

		ituWidgetSetVisible(informationNextContainer, FALSE);
		ituWidgetSetVisible(informationNullContainer5, TRUE);
		break;

	case INFORMATION_CONTENT_PAGE:
		ituWidgetSetVisible(informationMSGListCoverFlow, FALSE);
		ituWidgetSetVisible(informationMSGContentContainer, TRUE);

		ituWidgetSetVisible(informationEmptyContainer, FALSE);
		ituWidgetSetVisible(informationDeleteContainer, TRUE);
		
		ituWidgetSetVisible(informationNullContainer4, FALSE);
		ituWidgetSetVisible(informationPreContainer, TRUE);

		ituWidgetSetVisible(informationNullContainer5, FALSE);
		ituWidgetSetVisible(informationNextContainer, TRUE);
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

	return TRUE;
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
			ituWidgetSetVisible(informationMSGContentContainer, FALSE);
			ituWidgetSetVisible(informationMSGListCoverFlow, TRUE);
		}
		break;

	default:
		break;
	}

	ituWidgetEnable(informationBackground);
	ituWidgetSetVisible(informationTipsTransparencyBackground, FALSE);

	return TRUE;
}

bool setInformationIsReaded(uint8_t index, INFORMATION_MSGICON_STATUS_e status)
{
	char tmp_read[50] = { 0 };
	char tmp_unread[50] = { 0 };

	sprintf(tmp_read, "%s%d", "informationMSGListReadIcon", index);
	informationMSGListReadIcon = ituSceneFindWidget(&theScene, tmp_read);
	assert(informationMSGListReadIcon);

	sprintf(tmp_unread, "%s%d", "informationMSGListUnReadIcon", index);
	informationMSGListUnReadIcon = ituSceneFindWidget(&theScene, tmp_unread);
	assert(informationMSGListUnReadIcon);

	switch (status)
	{	
	case INFORMATION_MSGICON_READED:
		ituWidgetSetVisible(informationMSGListUnReadIcon, FALSE);
		ituWidgetSetVisible(informationMSGListReadIcon, TRUE);
		break;

	case INFORMATION_MSGICON_UNREADED:
		ituWidgetSetVisible(informationMSGListReadIcon, FALSE);
		ituWidgetSetVisible(informationMSGListUnReadIcon, TRUE);
		break;

	case INFORMATION_MSGICON_HIDE:
		ituWidgetSetVisible(informationMSGListReadIcon, FALSE);
		ituWidgetSetVisible(informationMSGListUnReadIcon, FALSE);
		break;

	default:
		break;
	}

	return TRUE;
}

bool setInformationListSender(uint8_t index, char* senderStr)
{
	char tmpStr[50] = { 0 };

	sprintf(tmpStr, "%s%d", "informationMSGListSenderText", index);
	informationMSGListSenderText = ituSceneFindWidget(&theScene, tmpStr);
	assert(informationMSGListSenderText);

	ituTextSetString(informationMSGListSenderText, senderStr);

	//TODO: 可能要对传进来的指针数据进行释放，看后期数据如何传递！！！
	return TRUE;
}

bool setInformationListTheme(uint8_t index, char* themeStr)
{
	char tmpStr[50] = { 0 };
	char tmp_theme[50] = { 0 };
	sprintf(tmpStr, "%s%d", "informationMSGListThemeText", index);
	informationMSGListThemeText = ituSceneFindWidget(&theScene, tmpStr);
	assert(informationMSGListThemeText);

	if (strlen(themeStr) > MAX_INFORMATION_THEME_LEN)
	{
		strncpy(tmp_theme, themeStr, MAX_INFORMATION_THEME_LEN);
		sprintf(tmpStr, "%s.", tmp_theme);
		ituTextSetString(informationMSGListThemeText, tmpStr);
	}
	else
	{
		ituTextSetString(informationMSGListThemeText, themeStr);
	}

	//TODO: 可能要对传进来的指针数据进行释放，看后期数据如何传递！！！
	return TRUE;
}

bool setInformationListTime(uint8_t index, char* timeStr)
{
	char tmpStr[50] = { 0 };

	sprintf(tmpStr, "%s%d", "informationMSGListTimeText", index);
	informationMSGListTimeText = ituSceneFindWidget(&theScene, tmpStr);
	assert(informationMSGListTimeText);

	ituTextSetString(informationMSGListTimeText, timeStr);

	//TODO: 可能要对传进来的指针数据进行释放，看后期数据如何传递！！！
	return TRUE;
}

bool setInformationListIsVisible(uint8_t index, bool status)
{
	char tmpStr[50] = { 0 };

	if (index >= MAX_INFORMATION_NUM)
	{
		printf("setting index overflow!!!!!!!!!!");
		return FALSE;
	}
	else
	{
		sprintf(tmpStr, "%s%d", "informationMSGListContainer", index);
		informationMSGListContainer = ituSceneFindWidget(&theScene, tmpStr);
		assert(informationMSGListContainer);

		ituWidgetSetVisible(informationMSGListContainer, status);
	}
	return TRUE;
}

bool informationMSGListClicked(ITUWidget* widget, char* param)
{
	informationLayerInit(INFORMATION_CONTENT_PAGE);
	setInformationContent(atoi(param));

	return TRUE;
}

void setInformationContent(uint8_t index)
{
	printf("Information List clieked %d", index);

	//TODO: 按照index读取存储相应信息、并且将状态置成已读(置标志位要在进信息内容框才置位，可以统一和上下条操作一起执行！！！！！)
	char tmpStr[500] = { 0 };

	sprintf(tmpStr, "%s%d", "Center", index);
	setInformationContentSender(tmpStr);

	sprintf(tmpStr, "%s %d", "0123456789012345678901234567890123456", index);
	setInformationContentTheme(tmpStr);

	sprintf(tmpStr, "%s%d", "2016-07-06 12:34:", index);
	setInformationContentTime(tmpStr);

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
		ituWidgetSetVisible(informationMSGContentImageIcon, FALSE);
		return FALSE;
	}
	if (gInformationImageData)
	{
		ituIconLoadJpegData((ITUIcon*)informationMSGContentImageIcon, gInformationImageData, gInformationImageSize);
		return TRUE;
	}
	else
	{
		printf("load minipic jepg icon failed!");
		ituWidgetSetVisible(informationMSGContentImageIcon, FALSE);
		return FALSE;
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
		/*ituWidgetSetVisible(informationMSGContentContainer, FALSE);
		ituWidgetSetVisible(informationMSGListCoverFlow, TRUE);*/
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
	//return TRUE;
}

void informationNextMsgBtnOnClicked()
{
	printf("informationNextMsgBTNOnClicked 111111111111111111111111111111");

	//return TRUE;
}

void informationPreMsgBtnOnClicked()
{
	printf("informationPreMsgBTNOnClicked 111111111111111111111111111111");

	//return TRUE;
}

void informationMsgBoxShow(INFORMATION_BTN_e index)
{
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
		ituTextSetString(informationTipsText, "Empty Message");
		break;

	case INFORMATION_BTN_DELETE:
		ituTextSetString(informationTipsText, "Delete Message");
		break;

	default:
		break;
	}

	ituWidgetDisable(informationBackground);
	ituWidgetSetVisible(informationTipsTransparencyBackground, TRUE);
}

void setInformationList()
{
	bool a = FALSE;
	uint8_t i = 0;
	char tmpStr[50] = { 0 };

	uint8_t msgNum = 20;
	//uint8_t msgNum = getUnreadinformationNum();

	//TODO: 读取存储内容设置列表信息！！！！！！！
	for (i = 0; i < MAX_INFORMATION_NUM; i++)
	{
		if (msgNum < INFORMATION_PER_PAGE)
		{
			if (i < msgNum)
			{
				if (a)
				{
					a = FALSE;
				}
				else
				{
					a = TRUE;
				}
				setInformationIsReaded(i, a);

				sprintf(tmpStr, "%s%d", "Center", i);
				setInformationListSender(i, tmpStr);

				sprintf(tmpStr, "%s%d", "01234567890123456789", i);
				setInformationListTheme(i, tmpStr);

				sprintf(tmpStr, "%s%d", "2016-06-06 11:11:", i);
				setInformationListTime(i, tmpStr);
			}
			else if (i >= msgNum && i < INFORMATION_PER_PAGE)
			{
				setInformationIsReaded(i, INFORMATION_MSGICON_HIDE);
				setInformationListSender(i, "");
				setInformationListTheme(i, "");
				setInformationListTime(i, "");
			}
			else
			{
				setInformationListIsVisible(i, FALSE);
			}
		}
		else
		{
			if (i < msgNum)
			{
				if (a)
				{
					a = FALSE;
				}
				else
				{
					a = TRUE;
				}
				setInformationIsReaded(i, a);

				sprintf(tmpStr, "%s%d", "Center", i);
				setInformationListSender(i, tmpStr);

				sprintf(tmpStr, "%s%d", "01234567890123456789", i);
				setInformationListTheme(i, tmpStr);

				sprintf(tmpStr, "%s%d", "2016-06-06 11:11:", i);
				setInformationListTime(i, tmpStr);
			}
			else
			{
				setInformationListIsVisible(i, FALSE);
			}
		}

	}
	//将没有内容的页面隐藏起来，达到不能滑动的效果！
	for (i = 0; i < NIFORMATION_PAGE_NUM; i++)
	{
		sprintf(tmpStr, "%s%d", "informationPageContainer", i);
		informationPageContainer = ituSceneFindWidget(&theScene, tmpStr);
		assert(informationPageContainer);

		if (i <= (msgNum / INFORMATION_PER_PAGE))
		{
			if ((i == (msgNum / INFORMATION_PER_PAGE)) && (msgNum % INFORMATION_PER_PAGE) == 0)
			{
				ituWidgetSetVisible(informationPageContainer, FALSE);
			}
			else
			{
				ituWidgetSetVisible(informationPageContainer, TRUE);
			}
		}
		else
		{
			ituWidgetSetVisible(informationPageContainer, FALSE);

		}
	}
}

void informationLayerReset()
{
	mainLayer = NULL;
	informationMSGContentContainer	= NULL;
	informationMSGListCoverFlow		= NULL;
}