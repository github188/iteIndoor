/*********************************************************
Copyright (C), 2015-2020, Aurine
File name:	layer_picmanager.c
Author:		WuZ
Version:   	1.0
Date:		2016-08-18
Description: 图片管理操作函数
History:
1. Date:
Author:
Modification:
2. ...
*********************************************************/

#include "layer_picmanager.h"

static ITULayer*		mainLayer;
static ITUBackground*	picManagerTipsTransparencyBackground;
static ITUWidget*		picManagerEmptyContainer;
static ITUWidget*		picManagerDeleteContainer;
static ITUWidget*		picManagerPicEditContainer;
static ITUCoverFlow*	picManagerPictureCoverFlow;
static ITUWidget*		picManagerMiniPicContainer;
static ITUWidget*		picManagerNullContainer3;
static ITUIcon*			picManagerMiniPicIcon;
	
static ITUIcon*			picManagerMiniPicUnChenkIcon;
static ITUIcon*			picManagerMiniPicChenkIcon;

static uint8_t*	gPicManagerImageData;
static int		gPicManagerImageSize;
static char		gPicManagerImageFilePath[PATH_MAX];

bool picManagerLayerOnEnter(ITUWidget* widget, char* param)
{
	picManagerPageInit(PICMANAGER_MINIPIC_PAGE);
	setPicManagerMiniPicList();

	return true;
}


void picManagerPageInit(PICMANAGER_PAGE_e pageId)
{
	if (!picManagerEmptyContainer)
	{
		picManagerEmptyContainer = ituSceneFindWidget(&theScene, "picManagerEmptyContainer");
		assert(picManagerEmptyContainer);
	}
	if (!picManagerDeleteContainer)
	{
		picManagerDeleteContainer = ituSceneFindWidget(&theScene, "picManagerDeleteContainer");
		assert(picManagerDeleteContainer);
	}
	if (!picManagerPicEditContainer)
	{
		picManagerPicEditContainer = ituSceneFindWidget(&theScene, "picManagerPicEditContainer");
		assert(picManagerPicEditContainer);
	}
	if (!picManagerPictureCoverFlow)
	{
		picManagerPictureCoverFlow = ituSceneFindWidget(&theScene, "picManagerPictureCoverFlow");
		assert(picManagerPictureCoverFlow);
	}
	if (!picManagerMiniPicContainer)
	{
		picManagerMiniPicContainer = ituSceneFindWidget(&theScene, "picManagerMiniPicContainer");
		assert(picManagerMiniPicContainer);
	}
	if (!picManagerNullContainer3)
	{
		picManagerNullContainer3 = ituSceneFindWidget(&theScene, "picManagerNullContainer3");
		assert(picManagerNullContainer3);
	}
	if (!picManagerTipsTransparencyBackground)
	{
		picManagerTipsTransparencyBackground = ituSceneFindWidget(&theScene, "picManagerTipsTransparencyBackground");
		assert(picManagerTipsTransparencyBackground);
	}
	ituWidgetSetVisible(picManagerTipsTransparencyBackground, false);


	switch (pageId)
	{
	case PICMANAGER_MINIPIC_PAGE:
		ituWidgetSetVisible(picManagerPictureCoverFlow, false);
		ituWidgetSetVisible(picManagerMiniPicContainer, true);

		ituWidgetSetVisible(picManagerEmptyContainer, false);
		ituWidgetSetVisible(picManagerPicEditContainer, true);

		ituWidgetSetVisible(picManagerDeleteContainer, false);
		ituWidgetSetVisible(picManagerNullContainer3, true);

		break;

	case PICMANAGER_PICCONTENT_PAGE:
		ituWidgetSetVisible(picManagerMiniPicContainer, false);
		ituWidgetSetVisible(picManagerPictureCoverFlow, true);

		ituWidgetSetVisible(picManagerPicEditContainer, false);
		ituWidgetSetVisible(picManagerEmptyContainer, true);

		ituWidgetSetVisible(picManagerNullContainer3, false);
		ituWidgetSetVisible(picManagerDeleteContainer, true);
		break;

	default:
		break;
	}
}


bool picManagerBtnOnClicked(ITUWidget* widget, char* param)
{

	return true;
}


bool picManagerMsgBoxBtnOnClicked(ITUWidget* widget, char* param)
{

	return true;
}


void setMiniPicIsChecked(uint8_t index, MINIPIC_CORNER_ICON_STATUS_e status)
{
	char tmpCheck[50] = { 0 };
	char tmpUncheck[50] = { 0 };

	if (index >= PICMANAGER_MINIPIC_MAX)
	{
		printf("index is too large!!!!!!!!!!");
		return;
	}

	sprintf(tmpCheck, "%s%d", "picManagerMiniPicChenkIcon", index);
	picManagerMiniPicChenkIcon = ituSceneFindWidget(&theScene, tmpCheck);
	assert(picManagerMiniPicChenkIcon);

	sprintf(tmpUncheck, "%s%d", "picManagerMiniPicUnChenkIcon", index);
	picManagerMiniPicUnChenkIcon = ituSceneFindWidget(&theScene, tmpUncheck);
	assert(picManagerMiniPicUnChenkIcon);

	switch (status)
	{
	case MINIPIC_CORNER_ICON_CHECK:
		ituWidgetSetVisible(picManagerMiniPicUnChenkIcon, false);
		ituWidgetSetVisible(picManagerMiniPicChenkIcon, true);
		break;

	case MINIPIC_CORNER_ICON_UNCHECK:
		ituWidgetSetVisible(picManagerMiniPicChenkIcon, false);
		ituWidgetSetVisible(picManagerMiniPicUnChenkIcon, true);
		break;

	case MINIPIC_CORNER_ICON_NULL:
		ituWidgetSetVisible(picManagerMiniPicChenkIcon, false);
		ituWidgetSetVisible(picManagerMiniPicUnChenkIcon, false);
		break;

	default:
		break;
	}
}


bool setMiniPicContent(uint8_t index, char* addrStr)
{
	FILE*	tmpFile;
	char tmpStr[50] = { 0 };

	if (index >= PICMANAGER_MINIPIC_MAX)
	{
		printf("index is too large!!!!!!!!!!");
		return false;
	}

	sprintf(tmpStr, "%s%d", "picManagerMiniPicIcon", index);
	picManagerMiniPicIcon = ituSceneFindWidget(&theScene, tmpStr);
	assert(picManagerMiniPicIcon);

	// try to load minipic jpeg file if exists
	tmpFile = fopen(addrStr, "rb");
	if (tmpFile)
	{
		struct stat sb;
		if (fstat(fileno(tmpFile), &sb) != -1)			//用_fileno代替fileno避免运行时候的警告！！！！（但是板子上编译不过！！！！）
		{
			gPicManagerImageSize = (int)sb.st_size;
			gPicManagerImageData = malloc(gPicManagerImageSize);
			if (gPicManagerImageSize)
			{
				gPicManagerImageSize = fread(gPicManagerImageData, 1, gPicManagerImageSize, tmpFile);
			}
		}
		fclose(tmpFile);
	}
	else
	{
		printf("open  minipic jepg icon icon failed!");
		ituWidgetSetVisible(picManagerMiniPicIcon, false);
		return false;
	}
	if (gPicManagerImageData)
	{
		ituIconLoadJpegData((ITUIcon*)picManagerMiniPicIcon, gPicManagerImageData, gPicManagerImageSize);
	}
	else
	{
		printf("load minipic jepg icon failed!");
		ituWidgetSetVisible(picManagerMiniPicIcon, false);
		return false;
	}

	return true;
}

void setPicManagerMiniPicList()
{
	uint8_t i = 0;
	char tmpStr[50] = { 0 };
	uint8_t miniPicNum = 15;

	//TODO: 读取存储内容缩略图信息信息！！！！！！！
	for (i = 0; i < PICMANAGER_MINIPIC_MAX; i++)
	{
		sprintf(tmpStr, "%s%d", "picManagerMiniPicContainer", i);
		picManagerMiniPicContainer = ituSceneFindWidget(&theScene, tmpStr);
		assert(picManagerMiniPicContainer);

		setMiniPicIsChecked(i, MINIPIC_CORNER_ICON_NULL);

		if (i < miniPicNum)
		{
			ituWidgetSetVisible(picManagerMiniPicContainer, true);

		}
		else
		{
			ituWidgetSetVisible(picManagerMiniPicContainer, false);
		}
	}


}