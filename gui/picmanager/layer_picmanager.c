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

static ITULayer*		mainLayer = NULL;
static ITUBackground*	picManagerTipsTransparencyBackground = NULL;
static ITUBackground*	picManagerBackground = NULL;
static ITUText*			picManagerTipsText = NULL;
static ITUWidget*		picManagerEmptyContainer = NULL;
static ITUWidget*		picManagerDeleteContainer = NULL;
static ITUWidget*		picManagerPicEditContainer = NULL;
static ITUCoverFlow*	picManagerPictureCoverFlow = NULL;
static ITUWidget*		picManagerMiniPicListContainer = NULL;
static ITUWidget*		picManagerMiniPicContainer = NULL;
static ITUWidget*		picManagerBottomBarContainer = NULL;
static ITUWidget*		picManagerNullContainer3 = NULL;
static ITUIcon*			picManagerMiniPicIcon = NULL;
static ITUIcon*			picManagerPicContentIcon = NULL;
static ITUIcon*			picManagerMiniPicUnChenkIcon = NULL;
static ITUIcon*			picManagerMiniPicChenkIcon = NULL;

static uint8_t	gPictureCurrentIndex[PICMANAGER_CONTENT_NUM];
static uint8_t  gCoverFlowCurrentIndex;
static uint8_t  gCoverFlowLastIndex;
static uint8_t	gPictureNumCount;				//用来记录抓拍数量
static uint8_t*	gPicManagerImageData;
static int		gPicManagerImageSize;
static PPHOTOLIST_INFO gPicManagerList = NULL;	//用来存放照片信息


bool picManagerLayerOnEnter(ITUWidget* widget, char* param)
{
	gPictureNumCount = 0;

	picManagerPageInit(PICMANAGER_PAGE_MINIPIC);
	setPicManagerMiniPicList();

	return true;
}


bool picManagerLayerOnLeave(ITUWidget* widget, char* param)
{
	printf("\picManagerLayerOnLeave!!!!!!!!!\n");

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

		setPicManagerPicture();

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


bool picManagerPictureOnChanged(ITUWidget* widget, char* param)
{
	uint8_t tmpCoverFlowIndex = picManagerPictureCoverFlow->focusIndex;
	bool  leftRightFlag = true;			//true  =  left
	uint8_t i = 0;
	char tmpFile[50] = { 0 };

	if (gPictureNumCount <= PICMANAGER_CONTENT_NUM)
	{
		return false;
	}

	if (gCoverFlowLastIndex - tmpCoverFlowIndex == 1 || gCoverFlowLastIndex - tmpCoverFlowIndex == -2)
	{
		leftRightFlag = false;
	}
	else
	{
		leftRightFlag = true;
	}

	printf("111111111111111111 picnum = %d direction = %d", gPictureNumCount, leftRightFlag);

	if (leftRightFlag)		//左滑动只要载入最左边一张
	{
		for (i = 0; i < PICMANAGER_CONTENT_NUM; i++)
		{
			if (i != gCoverFlowLastIndex && i != tmpCoverFlowIndex)
			{
				if ((gPictureCurrentIndex[tmpCoverFlowIndex] + 1) >= gPictureNumCount)
				{
					gPictureCurrentIndex[i] = 0;
					get_photo_path(tmpFile, &((gPicManagerList->PhotoInfo[gPictureCurrentIndex[i]]).Time));
					setPicManagerPictureContent(i, tmpFile);
				}
				else
				{
					gPictureCurrentIndex[i] = gPictureCurrentIndex[tmpCoverFlowIndex] + 1;
					get_photo_path(tmpFile, &((gPicManagerList->PhotoInfo[gPictureCurrentIndex[i]]).Time));
					setPicManagerPictureContent(i, tmpFile);
				}
			}
		}
	}
	else					//右滑动只要载入最右边一张即可！！！
	{
		for (i = 0; i < PICMANAGER_CONTENT_NUM; i++)
		{
			if (i != gCoverFlowLastIndex && i != tmpCoverFlowIndex)
			{
				if ((gPictureCurrentIndex[tmpCoverFlowIndex] - 1) >= 0)
				{
					gPictureCurrentIndex[i] = gPictureCurrentIndex[tmpCoverFlowIndex] - 1;
					get_photo_path(tmpFile, &((gPicManagerList->PhotoInfo[gPictureCurrentIndex[i]]).Time));
					setPicManagerPictureContent(i, tmpFile);
				}
				else
				{
					gPictureCurrentIndex[i] = gPictureNumCount - 1;
					get_photo_path(tmpFile, &((gPicManagerList->PhotoInfo[gPictureCurrentIndex[i]]).Time));
					setPicManagerPictureContent(i, tmpFile);
				}
			}
		}
	}

	gCoverFlowLastIndex = tmpCoverFlowIndex;

	return true;
}


void setPicManagerPicture()
{
	uint8_t	i = 0;
	char tmpFile[50] = { 0 };

	ituCoverFlowGoto(picManagerPictureCoverFlow, 0);
	gCoverFlowLastIndex = 0;

	get_photo_path(tmpFile, &((gPicManagerList->PhotoInfo[gPictureCurrentIndex[0]]).Time));
	setPicManagerPictureContent(0, tmpFile);

	if ((gPictureCurrentIndex[0] - 1) >= 0)
	{
		gPictureCurrentIndex[2] = gPictureCurrentIndex[0] - 1;
		get_photo_path(tmpFile, &((gPicManagerList->PhotoInfo[gPictureCurrentIndex[2]]).Time));
		setPicManagerPictureContent(2, tmpFile);
	}
	else
	{
		gPictureCurrentIndex[2] = gPictureNumCount - 1;
		get_photo_path(tmpFile, &((gPicManagerList->PhotoInfo[gPictureNumCount - 1]).Time));
		setPicManagerPictureContent(2, tmpFile);
	}

	if ((gPictureCurrentIndex[0] + 1) >= gPictureNumCount)
	{
		gPictureCurrentIndex[1] = 0;
		get_photo_path(tmpFile, &((gPicManagerList->PhotoInfo[gPictureCurrentIndex[1]]).Time));
		setPicManagerPictureContent(1, tmpFile);
	}
	else
	{
		gPictureCurrentIndex[1] = gPictureCurrentIndex[0] + 1;
		get_photo_path(tmpFile, &((gPicManagerList->PhotoInfo[gPictureCurrentIndex[1]]).Time));
		setPicManagerPictureContent(1, tmpFile);
	}
}


bool setPicManagerPictureContent(uint8_t index, char* addrStr)
{
	FILE* tmpFile;
	char  tmpStr[50] = { 0 };

	memset(tmpStr, 0, sizeof(tmpStr));
	sprintf(tmpStr, "%s%d", "picManagerPicContentIcon", index);
	picManagerPicContentIcon = ituSceneFindWidget(&theScene, tmpStr);
	assert(picManagerPicContentIcon);

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
		ituWidgetSetVisible(picManagerPicContentIcon, false);
		free(gPicManagerImageData);

		return false;
	}
	if (gPicManagerImageData)
	{
		ituIconLoadJpegData(picManagerPicContentIcon, gPicManagerImageData, gPicManagerImageSize);
	}
	else
	{
		printf("load minipic jepg icon failed!");
		ituWidgetSetVisible(picManagerPicContentIcon, false);
		free(gPicManagerImageData);

		return false;
	}

	free(gPicManagerImageData);

	return true;
}


void picManagerRetutnBtnOnClicked()
{
	printf("picManagerRetutnBtnOnClicked");

	if (ituWidgetIsVisible(picManagerPictureCoverFlow))
	{
		picManagerPageInit(PICMANAGER_PAGE_MINIPIC);
		setPicManagerMiniPicList();
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
	if (gPictureNumCount)
	{
		picManagerPageInit(PICMANAGER_PAGE_EDIT);
		setMiniPicListIsChecked(MINIPIC_CORNER_ICON_UNCHECK);
	}
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
	uint8_t i = 0;
	uint8_t tmpId = atoi(param);
	uint8_t delCount = 0;

	switch (tmpId)
	{
	case PICMANAGER_MSG_BTN_CONFIRM:
		for (i = 0; i < PICMANAGER_MINIPIC_MAX; i++)
		{
			if (getMiniPicIsChecked(i) == MINIPIC_CORNER_ICON_CHECK)
			{
				//TODO:这里写删除所选照片的操作，删除完重新绘制内容界面！！！！！！！
				storage_del_photo(i - delCount);
				delCount++;
			}
		}
		picManagerPageInit(PICMANAGER_PAGE_MINIPIC);
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
		gPictureCurrentIndex[0] = tmpIndex;
		setPicManagerPicture();
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
	memset(tmpCheck, 0, sizeof(tmpCheck));
	sprintf(tmpCheck, "%s%d", "picManagerMiniPicChenkIcon", index);
	picManagerMiniPicChenkIcon = ituSceneFindWidget(&theScene, tmpCheck);
	assert(picManagerMiniPicChenkIcon);

	memset(tmpUncheck, 0, sizeof(tmpUncheck));
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
		memset(tmpStr, 0, sizeof(tmpStr));
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

	memset(tmpCheck, 0, sizeof(tmpCheck));
	sprintf(tmpCheck, "%s%d", "picManagerMiniPicChenkIcon", index);
	picManagerMiniPicChenkIcon = ituSceneFindWidget(&theScene, tmpCheck);
	assert(picManagerMiniPicChenkIcon);

	memset(tmpUncheck, 0, sizeof(tmpUncheck));
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

	memset(tmpStr, 0, sizeof(tmpStr));
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

	free(gPicManagerImageData);
	return true;
}


void setPicManagerMiniPicList()
{
	uint8_t i = 0;
	char tmpStr[50] = { 0 };
	char tmpFile[50] = { 0 };

	if (gPicManagerList != NULL)
	{
		free(gPicManagerList);
		gPicManagerList = NULL;
	}

	storage_get_photo(&gPicManagerList);
	gPictureNumCount = gPicManagerList->Count;

	//TODO: 读取存储内容缩略图信息信息！！！！！！！
	for (i = 0; i < PICMANAGER_MINIPIC_MAX; i++)
	{
		memset(tmpStr, 0, sizeof(tmpStr));
		sprintf(tmpStr, "%s%d", "picManagerMiniPicContainer", i);
		picManagerMiniPicContainer = ituSceneFindWidget(&theScene, tmpStr);
		assert(picManagerMiniPicContainer);

		setMiniPicIsChecked(i, MINIPIC_CORNER_ICON_NULL);

		if (i < gPictureNumCount)
		{
			get_photo_path(tmpFile, &gPicManagerList->PhotoInfo[i].Time);
			setMiniPicContent(i, tmpFile);
			ituWidgetSetVisible(picManagerMiniPicContainer, true);
		}
		else
		{
			ituWidgetSetVisible(picManagerMiniPicContainer, false);
		}
	}


}