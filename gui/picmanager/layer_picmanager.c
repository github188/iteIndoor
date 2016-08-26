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
static ITUBackground*	picManagerBackground;
static ITUText*			picManagerTipsText;
static ITUWidget*		picManagerEmptyContainer;
static ITUWidget*		picManagerDeleteContainer;
static ITUWidget*		picManagerPicEditContainer;
static ITUCoverFlow*	picManagerPictureCoverFlow;
static ITUWidget*		picManagerMiniPicListContainer;
static ITUWidget*		picManagerMiniPicContainer;
static ITUWidget*		picManagerBottomBarContainer;
static ITUWidget*		picManagerNullContainer3;
static ITUIcon*			picManagerMiniPicIcon;
	
static ITUIcon*			picManagerMiniPicUnChenkIcon;
static ITUIcon*			picManagerMiniPicChenkIcon;

static uint8_t*	gPicManagerImageData;
static int		gPicManagerImageSize;
static char		gPicManagerImageFilePath[PATH_MAX];

bool picManagerLayerOnEnter(ITUWidget* widget, char* param)
{
	picManagerPageInit(PICMANAGER_PAGE_MINIPIC);
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
	if (!picManagerMiniPicListContainer)
	{
		picManagerMiniPicListContainer = ituSceneFindWidget(&theScene, "picManagerMiniPicListContainer");
		assert(picManagerMiniPicListContainer);
	}
	if (!picManagerNullContainer3)
	{
		picManagerNullContainer3 = ituSceneFindWidget(&theScene, "picManagerNullContainer3");
		assert(picManagerNullContainer3);
	}
	if (!picManagerBottomBarContainer)
	{
		picManagerBottomBarContainer = ituSceneFindWidget(&theScene, "picManagerBottomBarContainer");
		assert(picManagerBottomBarContainer);
	}
	if (!picManagerTipsTransparencyBackground)
	{
		picManagerTipsTransparencyBackground = ituSceneFindWidget(&theScene, "picManagerTipsTransparencyBackground");
		assert(picManagerTipsTransparencyBackground);
	}
	ituWidgetSetVisible(picManagerTipsTransparencyBackground, false);


	switch (pageId)
	{
	case PICMANAGER_PAGE_MINIPIC:
		ituWidgetSetVisible(picManagerBottomBarContainer, true);

		ituWidgetSetVisible(picManagerPictureCoverFlow, false);
		ituWidgetSetVisible(picManagerMiniPicListContainer, true);

		ituWidgetSetVisible(picManagerEmptyContainer, false);
		ituWidgetSetVisible(picManagerPicEditContainer, true);

		ituWidgetSetVisible(picManagerDeleteContainer, false);
		ituWidgetSetVisible(picManagerNullContainer3, true);

		break;

	case PICMANAGER_PAGE_CONTENT:
		ituWidgetSetVisible(picManagerBottomBarContainer, false);

		ituWidgetSetVisible(picManagerMiniPicListContainer, false);
		ituWidgetSetVisible(picManagerPictureCoverFlow, true);

		break;

	case PICMANAGER_PAGE_EDIT:
		ituWidgetSetVisible(picManagerBottomBarContainer, true);

		ituWidgetSetVisible(picManagerPictureCoverFlow, false);
		ituWidgetSetVisible(picManagerMiniPicListContainer, true);

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
	PICMANAGER_BTN_e tmpBtn = atoi(param);

	switch (tmpBtn)
	{
	case PICMANAGER_BTN_RETURN:
		picManagerRetutnBtnOnClicked();
		break;

	case PICMANAGER_BTN_EMPTY:
	case PICMANAGER_BTN_DELETE:
		picManagerMsgBoxShow(tmpBtn);
		break;

	case PICMANAGER_BTN_EDIT:
		picManagerEditBtnOnClicked();
		break;

	default:
		break;
	}

	return true;
}


void picManagerRetutnBtnOnClicked()
{
	printf("picManagerRetutnBtnOnClicked");

	if (ituWidgetIsVisible(picManagerPictureCoverFlow))
	{
		picManagerPageInit(PICMANAGER_PAGE_MINIPIC);
	}
	else if (!ituWidgetIsVisible(picManagerPicEditContainer))
	{
		picManagerPageInit(PICMANAGER_PAGE_MINIPIC);
		setMiniPicListIsChecked(MINIPIC_CORNER_ICON_NULL);
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


void picManagerEditBtnOnClicked()
{
	picManagerPageInit(PICMANAGER_PAGE_EDIT);
	setMiniPicListIsChecked(MINIPIC_CORNER_ICON_UNCHECK);
}


void picManagerMsgBoxShow(PICMANAGER_BTN_e btnId)
{
	if (!picManagerTipsText)
	{
		picManagerTipsText = ituSceneFindWidget(&theScene, "picManagerTipsText");
		assert(picManagerTipsText);
	}

	if (!picManagerBackground)
	{
		picManagerBackground = ituSceneFindWidget(&theScene, "picManagerBackground");
		assert(picManagerBackground);
	}
	//TODO:添加语言翻译！
	switch (btnId)
	{
	case PICMANAGER_BTN_EMPTY:
		ituTextSetString(picManagerTipsText, "Empty Message");
		setMiniPicListIsChecked(MINIPIC_CORNER_ICON_CHECK);
		break;

	case PICMANAGER_BTN_DELETE:
		ituTextSetString(picManagerTipsText, "Delete Message");
		break;

	default:
		break;
	}

	ituWidgetDisable(picManagerBackground);
	ituWidgetSetVisible(picManagerTipsTransparencyBackground, true);
}


bool picManagerMsgBoxBtnOnClicked(ITUWidget* widget, char* param)
{
	uint16 i = 0;
	uint16 tmpId = atoi(param);

	switch (tmpId)
	{
	case PICMANAGER_MSG_BTN_CONFIRM:
		for (i = 0; i < PICMANAGER_MINIPIC_MAX; i++)
		{
			if (getMiniPicIsChecked(i) == MINIPIC_CORNER_ICON_CHECK)
			{
				//TODO:这里写删除所选照片的操作，删除完重新绘制内容界面！！！！！！！
			}
		}
		setPicManagerMiniPicList();
		break;

	default:
		break;
	}

	ituWidgetEnable(picManagerBackground);
	ituWidgetSetVisible(picManagerTipsTransparencyBackground, false);

	return true;
}


bool picManagerMiniPicBtnClicked(ITUWidget* widget, char* param)
{
	uint16 tmpIndex  = atoi(param);
	MINIPIC_ICON_STATUS_e tmpStatus = getMiniPicIsChecked(tmpIndex);

	switch (tmpStatus)
	{
	case MINIPIC_CORNER_ICON_CHECK:
		setMiniPicIsChecked(tmpIndex, MINIPIC_CORNER_ICON_UNCHECK);
		break;

	case MINIPIC_CORNER_ICON_UNCHECK:
		setMiniPicIsChecked(tmpIndex, MINIPIC_CORNER_ICON_CHECK);
		break;

	case MINIPIC_CORNER_ICON_NULL:
		//TODO:进入照片浏览界面！！！！！
		picManagerPageInit(PICMANAGER_PAGE_CONTENT);
		break;

	default:
		break;
	}
	return true;
}


void setMiniPicIsChecked(uint8_t index, MINIPIC_ICON_STATUS_e status)
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


void setMiniPicListIsChecked(MINIPIC_ICON_STATUS_e status)
{
	uint8_t i = 0;
	char tmpStr[50] = { 0 };

	for (i = 0; i < PICMANAGER_MINIPIC_MAX; i++)
	{
		sprintf(tmpStr, "%s%d", "picManagerMiniPicContainer", i);
		picManagerMiniPicContainer = ituSceneFindWidget(&theScene, tmpStr);
		assert(picManagerMiniPicContainer);

		if (ituWidgetIsVisible(picManagerMiniPicContainer))
		{
			setMiniPicIsChecked(i, status);
		}
	}
}


MINIPIC_ICON_STATUS_e getMiniPicIsChecked(uint8_t index)
{
	char tmpCheck[50] = { 0 };
	char tmpUncheck[50] = { 0 };

	if (index >= PICMANAGER_MINIPIC_MAX)
	{
		printf("index is too large!!!!!!!!!!");
		return MINIPIC_CORNER_ICON_NULL;
	}

	sprintf(tmpCheck, "%s%d", "picManagerMiniPicChenkIcon", index);
	picManagerMiniPicChenkIcon = ituSceneFindWidget(&theScene, tmpCheck);
	assert(picManagerMiniPicChenkIcon);

	sprintf(tmpUncheck, "%s%d", "picManagerMiniPicUnChenkIcon", index);
	picManagerMiniPicUnChenkIcon = ituSceneFindWidget(&theScene, tmpUncheck);
	assert(picManagerMiniPicUnChenkIcon);

	if (ituWidgetIsVisible(picManagerMiniPicChenkIcon))
	{
		return MINIPIC_CORNER_ICON_CHECK;
	}
	else if (ituWidgetIsVisible(picManagerMiniPicUnChenkIcon))
	{
		return MINIPIC_CORNER_ICON_UNCHECK;
	}
	else
	{
		return MINIPIC_CORNER_ICON_NULL;
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
	uint8_t miniPicNum = 10;

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