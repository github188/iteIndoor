/*********************************************************
Copyright (C), 2015-2020, Aurine
File name:	layer_photomsg.c
Author:		WuZ
Version:   	1.0
Date:		2016-06-30
Description: 留影留言操作函数
History:
1. Date:
Author:
Modification:
2. ...
*********************************************************/

#include "layer_photomsg.h"


static ITULayer*		mainLayer;
static ITUCoverFlow*	photoMsgListCoverFlow;
static ITUBackground*	photoMsgVideoDrawBackground;
static ITUBackground*	photoMsgTipsTransparencyBackground;
static ITUBackground*	photoMsgBackground;
static ITUWidget*		photoMsgPageContainer;
static ITUWidget*		photoMsgListContainer;
static ITUWidget*		photoMsgEmptyContainer;
static ITUWidget*		photoMsgNullContainer0;
static ITUWidget*		photoMsgMessageContainer;
static ITUWidget*		photoMsgPlayContainer;
static ITUWidget*		photoMsgPauseContainer;
static ITUWidget*		photoMsgStopContainer;
static ITUWidget*		photoMsgBottomBarContainer0;
static ITUWidget*		photoMsgBottomBarContainer1;
static ITUWidget*		photoMsgListMiniPicIcon;
static ITUWidget*		photoMsgListTimeText;
static ITUText*			photoMsgTipsText;
static ITUTrackBar*		photoMsgVolTrackBar;
static ITUIcon*			photoMsgVoiceOffIcon;
static ITUIcon*			photoMsgVoiceOnIcon;


static		PHOTOMSG_VIDEOPLAY_STATUS_e	gPhotoMsgVideoMode;		//记录当前的界面的播放状态
uint8_t*	gPhotoMsgListIconData;
int			gPhotoMsgListIconSize;
static char gPhotoMsgListIconFilePath[PATH_MAX];
static uint8_t	gPhotoMsgPlayVol = 0;

bool photoMsgLayerOnEnter(ITUWidget* widget, char* param)
{
	photoMsgLayerInit(PHOTOMSG_LIST_PAGE); 
	ituCoverFlowGoto(photoMsgListCoverFlow, 0);
	setPhotoMsgList();

	//TODO:暂时的全局变量
	gPhotoMsgPlayVol = PHOTOMSG_PLAY_VOL;

	return true;
}


bool photoMsgLayerOnLeave(ITUWidget* widget, char* param)
{
	printf("\photoMsgLayerOnLeave!!!!!!!!!\n");

	return true;
}


void photoMsgLayerInit(PHOTOMSG_PAGE_e pageId)
{
	if (!photoMsgListCoverFlow)
	{
		photoMsgListCoverFlow = ituSceneFindWidget(&theScene, "photoMsgListCoverFlow");
		assert(photoMsgListCoverFlow);
	}
	if (!photoMsgVideoDrawBackground)
	{
		photoMsgVideoDrawBackground = ituSceneFindWidget(&theScene, "photoMsgVideoDrawBackground");
		assert(photoMsgVideoDrawBackground);
	}
	if (!photoMsgBottomBarContainer0)
	{
		photoMsgBottomBarContainer0 = ituSceneFindWidget(&theScene, "photoMsgBottomBarContainer0");
		assert(photoMsgBottomBarContainer0);
	}
	if (!photoMsgNullContainer0)
	{
		photoMsgNullContainer0 = ituSceneFindWidget(&theScene, "photoMsgNullContainer0");
		assert(photoMsgNullContainer0);
	}
	if (!photoMsgPlayContainer)
	{
		photoMsgPlayContainer = ituSceneFindWidget(&theScene, "photoMsgPlayContainer");
		assert(photoMsgPlayContainer);
	}
	if (!photoMsgPauseContainer)
	{
		photoMsgPauseContainer = ituSceneFindWidget(&theScene, "photoMsgPauseContainer");
		assert(photoMsgPauseContainer);
	}
	if (!photoMsgStopContainer)
	{
		photoMsgStopContainer = ituSceneFindWidget(&theScene, "photoMsgStopContainer");
		assert(photoMsgStopContainer);
	}
	if (!photoMsgMessageContainer)
	{
		photoMsgMessageContainer = ituSceneFindWidget(&theScene, "photoMsgMessageContainer");
		assert(photoMsgMessageContainer);
	}

	if (!photoMsgTipsTransparencyBackground)
	{
		photoMsgTipsTransparencyBackground = ituSceneFindWidget(&theScene, "photoMsgTipsTransparencyBackground");
		assert(photoMsgTipsTransparencyBackground);
	}
	ituWidgetSetVisible(photoMsgTipsTransparencyBackground, false);

	switch (pageId)
	{
	case PHOTOMSG_LIST_PAGE:
		ituWidgetSetVisible(photoMsgVideoDrawBackground, false);
		ituWidgetSetVisible(photoMsgListCoverFlow, true);
		ituWidgetSetVisible(photoMsgBottomBarContainer0, true);

		ituWidgetSetVisible(photoMsgPauseContainer, false);
		ituWidgetSetVisible(photoMsgPlayContainer, false);
		ituWidgetSetVisible(photoMsgMessageContainer, true);

		ituWidgetSetVisible(photoMsgStopContainer, false);
		ituWidgetSetVisible(photoMsgNullContainer0, true);
		break;

	case PHOTOMSG_CONTENT_PAGE:
		ituWidgetSetVisible(photoMsgBottomBarContainer0, false);
		ituWidgetSetVisible(photoMsgListCoverFlow, false);
		ituWidgetSetVisible(photoMsgVideoDrawBackground, true);

		ituWidgetSetVisible(photoMsgMessageContainer, false);
		ituWidgetSetVisible(photoMsgPlayContainer, false);
		ituWidgetSetVisible(photoMsgPauseContainer, true);

		ituWidgetSetVisible(photoMsgNullContainer0, false);
		ituWidgetSetVisible(photoMsgStopContainer, true);
		break;

	default:
		break;
	}
}

bool photoMsgListOnClicked(ITUWidget* widget, char* param)
{
	printf("photoMsgListOnClicked %s", param);

	photoMsgLayerInit(PHOTOMSG_CONTENT_PAGE);
	setPhotoMsgVideoPlayByIndex(atoi(param));

	return true;
}

bool photoMsgBtnOnClicked(ITUWidget* widget, char* param)
{
	PHOTOMSG_BTN_e tmpBtn = atoi(param);

	switch (tmpBtn)
	{
	case PHOTOMSG_BTN_START:
	case PHOTOMSG_BTN_PAUSE:
	case PHOTOMSG_BTN_STOP:
		photoMsgVideoStatusBtnOnClicked(tmpBtn);
		break;

	case PHOTOMSG_BTN_TETURN:
		photoMsgReturnBtnOnClicked();
		break;

	case PHOTOMSG_BTN_EMPTY:
	case PHOTOMSG_BTN_DELETE:
		photoMsgBoxShow(tmpBtn);
		break;

	case PHOTOMSG_BTN_VOL:
		photoMsgVoiceBtnOnClicked();
		break;

	default:
		break;
	}

	return true;
}


bool photoMsgVolTrackBarOnChanged(ITUWidget* widget, char* param)
{
	uint8_t tmpVol = atoi(param);
	gPhotoMsgPlayVol = tmpVol;
	media_set_ring_volume(gPhotoMsgPlayVol);

	printf("PhotoMsg play Vol  = %s", param);

	//TODO:通知逻辑设置音量！！！！！！
	if (tmpVol == 0)
	{
		ituWidgetSetVisible(photoMsgVoiceOnIcon, false);
		ituWidgetSetVisible(photoMsgVoiceOffIcon, true);
	}
	else
	{
		ituWidgetSetVisible(photoMsgVoiceOffIcon, false);
		ituWidgetSetVisible(photoMsgVoiceOnIcon, true);
	}

	return true;
}


bool photoMsgMsgBoxBtnOnClicked(ITUWidget* widget, char* param)
{

	switch (atoi(param))
	{
	case PHOTOMSG_MSG_BTN_CONFIRM:
		if (ituWidgetIsVisible(photoMsgListCoverFlow))
		{
			//TODO:这里写清空信息的存储操作
			setPhotoMsgList();
		}
		else
		{
			//TODO:这里写删除一条的操作，删除完退出信息内容界面！！！！！！！
			photoMsgLayerInit(PHOTOMSG_LIST_PAGE);
			setPhotoMsgList();
		}
		break;

	default:
		break;
	}

	ituWidgetEnable(photoMsgBackground);
	ituWidgetSetVisible(photoMsgTipsTransparencyBackground, false);

	return true;
}


void photoMsgBoxShow(PHOTOMSG_BTN_e btnId)
{
	if (!photoMsgTipsText)
	{
		photoMsgTipsText = ituSceneFindWidget(&theScene, "photoMsgTipsText");
		assert(photoMsgTipsText);
	}

	if (!photoMsgBackground)
	{
		photoMsgBackground = ituSceneFindWidget(&theScene, "photoMsgBackground");
		assert(photoMsgBackground);
	}
	//TODO:添加语言翻译！
	switch (btnId)
	{
	case PHOTOMSG_BTN_EMPTY:
		ituTextSetString(photoMsgTipsText, "Empty Message");
		break;

	case PHOTOMSG_BTN_DELETE:
		ituTextSetString(photoMsgTipsText, "Delete Message");
		break;

	default:
		break;
	}

	ituWidgetDisable(photoMsgBackground);
	ituWidgetSetVisible(photoMsgTipsTransparencyBackground, true);
}


bool photoMsgVideoDrawBtnOnClicked(ITUWidget* widget, char* param)
{
	if (gPhotoMsgVideoMode == PHOTOMSG_VIDEOPLAY_PLAYING)
	{
		if (ituWidgetIsVisible(photoMsgBottomBarContainer1))
		{
			ituWidgetSetVisible(photoMsgBottomBarContainer1, false);
		}
		else
		{
			ituWidgetSetVisible(photoMsgBottomBarContainer1, true);

			setPhotoMsgPlayVol(gPhotoMsgPlayVol);
		}
	}
	return true;
}


void photoMsgVoiceBtnOnClicked()
{
	printf("recorderVoiceBtnOnClicked");

	//TODO:获取当前系统音量值，
	if (gPhotoMsgPlayVol != 0)
	{
		gPhotoMsgPlayVol = 0;
		setPhotoMsgPlayVol(gPhotoMsgPlayVol);
	}
	else
	{
		gPhotoMsgPlayVol = PHOTOMSG_PLAY_VOL;
		setPhotoMsgPlayVol(gPhotoMsgPlayVol);
	}
}

void setPhotoMsgPlayVol(uint8_t volNum)
{
	if (!photoMsgVolTrackBar)
	{
		photoMsgVolTrackBar = ituSceneFindWidget(&theScene, "photoMsgVolTrackBar");
		assert(photoMsgVolTrackBar);
	}
	if (!photoMsgVoiceOnIcon)
	{
		photoMsgVoiceOnIcon = ituSceneFindWidget(&theScene, "photoMsgVoiceOnIcon");
		assert(photoMsgVoiceOnIcon);
	}
	if (!photoMsgVoiceOffIcon)
	{
		photoMsgVoiceOffIcon = ituSceneFindWidget(&theScene, "photoMsgVoiceOffIcon");
		assert(photoMsgVoiceOffIcon);
	}

	if (volNum == 0)
	{
		ituWidgetSetVisible(photoMsgVoiceOnIcon, false);
		ituWidgetSetVisible(photoMsgVoiceOffIcon, true);
	}
	else
	{
		ituWidgetSetVisible(photoMsgVoiceOffIcon, false);
		ituWidgetSetVisible(photoMsgVoiceOnIcon, true);
	}

	ituTrackBarSetValue(photoMsgVolTrackBar, volNum);

	//photoMsgVoiceOnIcon
}


bool setPhotoMsgListIsVisible(uint8_t index, bool status)
{
	char tmpStr[50] = { 0 };

	if (index >= MAX_POOTOMSG_LIST_NUM)
	{
		printf("setting index overflow!!!!!!!!!!");
		return false;
	}
	else
	{
		memset(tmpStr, 0, sizeof(tmpStr));
		sprintf(tmpStr, "%s%d", "photoMsgListContainer", index);
		photoMsgListContainer = ituSceneFindWidget(&theScene, tmpStr);
		assert(photoMsgListContainer);
		ituWidgetSetVisible(photoMsgListContainer, status);
	}
	return true;
}



void setPhotoMsgList()
{
	uint8_t i = 0;
	char tmpStr[50] = { 0 };

	uint8_t msgNum = 0;		//TODO:读取信息条数！！！！！

	//TODO: 读取存储内容设置列表信息！！！！！！！
	for (i = 0; i < MAX_POOTOMSG_LIST_NUM; i++)
	{
		if (msgNum < PHOTOMSG_NUM_PER_PAGE)
		{
			if (i < msgNum)
			{
				strcpy(gPhotoMsgListIconFilePath, CFG_PRIVATE_DRIVE ":res/wallpaper/bk_05.jpg");
				setPhotoMsgListMiniIcon(i, gPhotoMsgListIconFilePath);
				memset(tmpStr, 0, sizeof(tmpStr));
				sprintf(tmpStr, "%s%d", "2016-07-15 11:11:", i);
				setPhotoMsgListTime(i, tmpStr, false);
			}
			else if (i >= msgNum && i < PHOTOMSG_NUM_PER_PAGE)
			{
				setPhotoMsgListMiniIcon(i, "");
				setPhotoMsgListTime(i, "", false);
			}
			else
			{
				setPhotoMsgListIsVisible(i, false);
			}
		}
		else
		{
			if (i < msgNum)
			{
				strcpy(gPhotoMsgListIconFilePath, CFG_PRIVATE_DRIVE ":res/wallpaper/bk_02.jpg");
				setPhotoMsgListMiniIcon(i, gPhotoMsgListIconFilePath);
				memset(tmpStr, 0, sizeof(tmpStr));
				sprintf(tmpStr, "%s%d", "2016-07-15 11:11:", i);
				setPhotoMsgListTime(i, tmpStr, true);
			}
			else
			{
				setPhotoMsgListIsVisible(i, false);
			}
		}
		
	}
	//将没有内容的页面隐藏起来，达到不能滑动的效果！
	for (i = 0; i < MAX_PHOTOMSG_PAGE_NUM; i++)
	{
		memset(tmpStr, 0, sizeof(tmpStr));
		sprintf(tmpStr, "%s%d", "photoMsgPageContainer", i);
		photoMsgPageContainer = ituSceneFindWidget(&theScene, tmpStr);
		assert(photoMsgPageContainer);

		if (i <= (msgNum / PHOTOMSG_NUM_PER_PAGE))
		{
			if ((i == (msgNum / PHOTOMSG_NUM_PER_PAGE)) && (msgNum % PHOTOMSG_NUM_PER_PAGE) == 0)
			{
				if (msgNum == 0)
				{
					ituWidgetSetVisible(photoMsgPageContainer, true);
				}
				else
				{
					ituWidgetSetVisible(photoMsgPageContainer, false);
				}
			}
			else
			{
				ituWidgetSetVisible(photoMsgPageContainer, true);
			}
		}
		else
		{
			ituWidgetSetVisible(photoMsgPageContainer, false);

		}
	}
}


bool setPhotoMsgListMiniIcon(uint8_t index, char* iconAddr)
{
	FILE*	tmpFile;
	char	tmpStr[50] = { 0 };
	
	if (index > MAX_POOTOMSG_LIST_NUM)
	{
		printf("photomsg list index overflow!!!!!!!!!!!!!");
		return false;
	}
	memset(tmpStr, 0, sizeof(tmpStr));
	sprintf(tmpStr, "%s%d", "photoMsgListMiniPicIcon", index);
	photoMsgListMiniPicIcon = ituSceneFindWidget(&theScene, tmpStr);
	assert(photoMsgListMiniPicIcon);

	// try to load minipic jpeg file if exists
	tmpFile = fopen(iconAddr, "rb");
	if (tmpFile)
	{
		struct stat sb;	
		if (fstat(fileno(tmpFile), &sb) != -1)			//用_fileno代替fileno避免运行时候的警告！！！！（但是板子上编译不过！！！！）
		{
			gPhotoMsgListIconSize = (int)sb.st_size;
			gPhotoMsgListIconData = malloc(gPhotoMsgListIconSize);
			if (gPhotoMsgListIconSize)
			{
				gPhotoMsgListIconSize = fread(gPhotoMsgListIconData, 1, gPhotoMsgListIconSize, tmpFile);
			}
		}
		fclose(tmpFile);
	}
	else
	{
		printf("open  minipic jepg icon icon failed!");
		ituWidgetSetVisible(photoMsgListMiniPicIcon, false);
		return false;
	}
	if (gPhotoMsgListIconData)
	{
		ituIconLoadJpegData((ITUIcon*)photoMsgListMiniPicIcon, gPhotoMsgListIconData, gPhotoMsgListIconSize);
	}
	else
	{
		printf("load minipic jepg icon failed!");
		ituWidgetSetVisible(photoMsgListMiniPicIcon, false);
		return false;
	}

	//TODO: 可能要对传进来的指针数据进行释放，看后期数据如何传递！！！
	free(gPhotoMsgListIconData);
	return true;

}


bool setPhotoMsgListTime(uint8_t index, char* timeStr, bool isUnread)
{
	char tmpStr[50] = { 0 };

	if (index > MAX_POOTOMSG_LIST_NUM)
	{
		printf("photomsg list index overflow!!!!!!!!!!!!!");
		return false;
	}

	memset(tmpStr, 0, sizeof(tmpStr));
	sprintf(tmpStr, "%s%d", "photoMsgListTimeText", index);
	photoMsgListTimeText = ituSceneFindWidget(&theScene, tmpStr);
	assert(photoMsgListTimeText);

	ituTextSetString(photoMsgListTimeText, timeStr);

	if (isUnread)
	{
		ituSetColor((ITUColor*)&photoMsgListTimeText->color, 255,255,0,0);
	}
	else
	{
		ituSetColor((ITUColor*)&photoMsgListTimeText->color, 255, 255, 255, 255);
	}

	//TODO: 可能要对传进来的指针数据进行释放，看后期数据如何传递！！！
	return true;
}


void photoMsgVideoStatusBtnOnClicked(PHOTOMSG_BTN_e btnId)
{

}


void setPhotoMsgVideoPlayStatusSetting(PHOTOMSG_VIDEOPLAY_STATUS_e mode)
{
	if (!photoMsgBottomBarContainer1)
	{
		photoMsgBottomBarContainer1 = ituSceneFindWidget(&theScene, "photoMsgBottomBarContainer1");
		assert(photoMsgBottomBarContainer1);
	}

	switch (mode)
	{
	case PHOTOMSG_VIDEOPLAY_PLAYING:
		ituWidgetSetVisible(photoMsgPlayContainer, false);
		ituWidgetSetVisible(photoMsgPauseContainer, true);
			  
		if (ituWidgetIsVisible(photoMsgBottomBarContainer1))
		{
			ituWidgetSetVisible(photoMsgBottomBarContainer1, false);
		}
		gPhotoMsgVideoMode = PHOTOMSG_VIDEOPLAY_PLAYING;

		//TODO：开始播放相对应留影留言
		break;
	
	case PHOTOMSG_VIDEOPLAY_PAUSE:
		ituWidgetSetVisible(photoMsgPauseContainer, false);
		ituWidgetSetVisible(photoMsgPlayContainer, true);

		gPhotoMsgVideoMode = PHOTOMSG_VIDEOPLAY_PAUSE;

		//TODO：暂停播放相对应留影留言
		break;

	case PHOTOMSG_VIDEOPLAY_STOP:
		ituWidgetSetVisible(photoMsgPauseContainer, false);
		ituWidgetSetVisible(photoMsgPlayContainer, true);

		if (!ituWidgetIsVisible(photoMsgBottomBarContainer1))
		{
			ituWidgetSetVisible(photoMsgBottomBarContainer1, true);
		}
		gPhotoMsgVideoMode = PHOTOMSG_VIDEOPLAY_STOP;

		//TODO:	停止播放留影留言
		break;

	default:
		break;
	}
}


void setPhotoMsgVideoPlayByIndex(uint8_t index)
{
	//TODO：相对应逻辑存储操作，设置页面按键状态。最后开始播放！
	setPhotoMsgVideoPlayStatusSetting(PHOTOMSG_VIDEOPLAY_PLAYING);

}


void photoMsgReturnBtnOnClicked()
{
	printf("photoMsg Return BTN clieked ");

	if (ituWidgetIsVisible(photoMsgVideoDrawBackground))
	{
		/*ituWidgetSetVisible(informationMSGContentContainer, false);
		ituWidgetSetVisible(informationMSGListCoverFlow, true);*/
		photoMsgLayerInit(PHOTOMSG_LIST_PAGE);
		setPhotoMsgList();
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
	//return true;
}


void photoMsgLayerReset()
{

}

