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


static ITULayer*		mainLayer = NULL;
static ITUCoverFlow*	photoMsgListCoverFlow = NULL;
static ITUBackground*	photoMsgVideoDrawBackground = NULL;
static ITUBackground*	photoMsgTipsTransparencyBackground = NULL;
static ITUBackground*	photoMsgBackground = NULL;
static ITUWidget*		photoMsgPageContainer = NULL;
static ITUWidget*		photoMsgListContainer = NULL;
static ITUWidget*		photoMsgEmptyContainer = NULL;
static ITUWidget*		photoMsgNullContainer0 = NULL;
static ITUWidget*		photoMsgMessageContainer = NULL;
static ITUWidget*		photoMsgPlayContainer = NULL;
static ITUWidget*		photoMsgPauseContainer = NULL;
static ITUWidget*		photoMsgStopContainer = NULL;
static ITUButton*		photoMsgPreVideoButton = NULL;
static ITUButton*		photoMsgNextVideoButton = NULL;
static ITUWidget*		photoMsgBottomBarContainer0 = NULL;
static ITUWidget*		photoMsgBottomBarContainer1 = NULL;
static ITUIcon*			photoMsgListMiniPicIcon = NULL;
static ITUWidget*		photoMsgListTimeText = NULL;
static ITUWidget*		photoMsgListSenderText = NULL;
static ITUText*			photoMsgTipsText = NULL;
static ITUTrackBar*		photoMsgVolTrackBar = NULL;
static ITUIcon*			photoMsgVoiceOffIcon = NULL;
static ITUIcon*			photoMsgVoiceOnIcon = NULL;


static uint8_t*	gPhotoMsgListIconData;
static int		gPhotoMsgListIconSize;
static uint8_t*	gPhotoMsgVideoBackgroundData;
static int		gPhotoMsgVideoBackgroundSize;
static uint8_t	gPhotoMsgPlayVol;
static uint8_t  gPhotoMsgNum;
static int8_t  gPhotoMsgCurrentIndex;
static PLYLYLIST_INFO gPhotoMsgList = NULL;
static PHOTOMSG_VIDEOPLAY_STATUS_e	gPhotoMsgVideoMode;		//记录当前的界面的播放状态

bool photoMsgLayerOnEnter(ITUWidget* widget, char* param)
{
	photoMsgLayerInit(PHOTOMSG_LIST_PAGE); 
	ituCoverFlowGoto(photoMsgListCoverFlow, 0);
	setPhotoMsgList();

	gPhotoMsgPlayVol = PHOTOMSG_PLAY_VOL;
	gPhotoMsgCurrentIndex = MAX_POOTOMSG_LIST_NUM;	//当前播放index，任意数值大于总条数即可

	return true;
}


bool photoMsgLayerOnLeave(ITUWidget* widget, char* param)
{
	printf("\photoMsgLayerOnLeave!!!!!!!!!\n");

	if (gPhotoMsgVideoMode != PHOTOMSG_VIDEOPLAY_STOP)
	{
		photoMsgVideoStatusBtnOnClicked(PHOTOMSG_BTN_STOP);
	}

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
	if (!photoMsgBottomBarContainer1)
	{
		photoMsgBottomBarContainer1 = ituSceneFindWidget(&theScene, "photoMsgBottomBarContainer1");
		assert(photoMsgBottomBarContainer1);
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
		ituWidgetSetVisible(photoMsgBottomBarContainer1, false);

		ituWidgetSetVisible(photoMsgPauseContainer, false);
		ituWidgetSetVisible(photoMsgPlayContainer, false);
		ituWidgetSetVisible(photoMsgMessageContainer, true);

		ituWidgetSetVisible(photoMsgStopContainer, false);
		ituWidgetSetVisible(photoMsgNullContainer0, true);
		break;

	case PHOTOMSG_CONTENT_PAGE:
		ituWidgetSetVisible(photoMsgBottomBarContainer0, false);
		ituWidgetSetVisible(photoMsgBottomBarContainer1, true);
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

	gPhotoMsgCurrentIndex = atoi(param);

	photoMsgLayerInit(PHOTOMSG_CONTENT_PAGE);
	setPhotoMsgVideoPlayByIndex(gPhotoMsgCurrentIndex);

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

	case PHOTOMSG_BTN_PRE:
	case PHOTOMSG_BTN_NEXT:
		setPhotoMsgPreNextVideo(tmpBtn);
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
			storage_clear_lylyrecord();
			setPhotoMsgList();
		}
		else
		{
			//TODO:这里写删除一条的操作，删除完退出信息内容界面！！！！！！！
			storage_del_lylyrecord(gPhotoMsgCurrentIndex);
			gPhotoMsgCurrentIndex = MAX_POOTOMSG_LIST_NUM;
			photoMsgLayerInit(PHOTOMSG_LIST_PAGE);
			setPhotoMsgList();
		}
		break;

	default:
		break;
	}
	sys_sync_hint_state();

	ituWidgetEnable(photoMsgBackground);
	ituWidgetSetVisible(photoMsgTipsTransparencyBackground, false);

	return true;
}


void photoMsgBoxShow(PHOTOMSG_BTN_e btnId)
{
	if (!gPhotoMsgNum)
	{
		return;
	}

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


void setPhotoMsgListIsEnable(uint8_t index, bool  status)
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
		if (status)
		{
			ituWidgetEnable(photoMsgListContainer);
		}
		else
		{
			ituWidgetDisable(photoMsgListContainer);
		}
	}
	return true;
}


void setPhotoMsgList()
{
	uint8_t i = 0;
	char tmpStr[50] = { 0 };
	char tmpAddr[50] = { 0 };

	if (gPhotoMsgList != NULL)
	{
		free(gPhotoMsgList);
		gPhotoMsgList = NULL;
	}
	storage_get_lylyrecord(&gPhotoMsgList);
	gPhotoMsgNum = gPhotoMsgList->Count;
	
	//TODO: 读取存储内容设置列表信息！！！！！！！
	for (i = 0; i < MAX_POOTOMSG_LIST_NUM; i++)
	{
		if (gPhotoMsgNum < PHOTOMSG_NUM_PER_PAGE)
		{
			if (i < gPhotoMsgNum)
			{
				switch (gPhotoMsgList->LylyInfo[i].LyType)
				{
				case LYLY_TYPE_AUDIO:
					sprintf(tmpAddr, "%s%s", PHOTO_MSG_DIR_PATH, "photomsg_audio.jpg");
					setPhotoMsgListMiniIcon(i, tmpAddr);
					break;

				case LYLY_TYPE_PIC_AUDIO:
					get_lylypic_path(tmpAddr, &gPhotoMsgList->LylyInfo[i].Time);
					setPhotoMsgListMiniIcon(i, tmpAddr);
					break;

				case LYLY_TYPE_VIDEO:
					sprintf(tmpAddr, "%s%s", PHOTO_MSG_DIR_PATH, "photomsg_video.jpg");
					setPhotoMsgListMiniIcon(i, tmpAddr);
					break;

				default:
					break;
				}
				printf("\n1111111111111111111 =%d =  %s \n", i, tmpAddr);

				memset(tmpStr, 0, sizeof(tmpStr));
				zoneDateTimeToString(gPhotoMsgList->LylyInfo[i].Time, tmpStr);
				setPhotoMsgListTime(i, tmpStr, gPhotoMsgList->LylyInfo[i].UnRead);
				get_dev_description(gPhotoMsgList->LylyInfo[i].DevType, gPhotoMsgList->LylyInfo[i].DevNo, tmpStr, sizeof(tmpStr)); // 获得设备描述
				setPhotoMsgListSender(i, tmpStr, gPhotoMsgList->LylyInfo[i].UnRead);
				setPhotoMsgListIsEnable(i, true);
			}
			else if (i >= gPhotoMsgNum && i < PHOTOMSG_NUM_PER_PAGE)
			{
				setPhotoMsgListMiniIcon(i, "");
				setPhotoMsgListSender(i, "", false);
				setPhotoMsgListTime(i, "", false);
				setPhotoMsgListIsEnable(i, false);
			}
			else
			{
				setPhotoMsgListIsVisible(i, false);
			}
		}
		else
		{
			if (i < gPhotoMsgNum)
			{
				switch (gPhotoMsgList->LylyInfo[i].LyType)
				{
				case LYLY_TYPE_AUDIO:
					sprintf(tmpAddr, "%s%s", PHOTO_MSG_DIR_PATH, "photomsg_audio.jpg");
					setPhotoMsgListMiniIcon(i, tmpAddr);
					break;

				case LYLY_TYPE_PIC_AUDIO:
					get_lylypic_path(tmpAddr, &gPhotoMsgList->LylyInfo[i].Time);
					setPhotoMsgListMiniIcon(i, tmpAddr);
					break;

				case LYLY_TYPE_VIDEO:
					sprintf(tmpAddr, "%s%s", PHOTO_MSG_DIR_PATH, "photomsg_video.jpg");
					setPhotoMsgListMiniIcon(i, tmpAddr);
					break;

				default:
					break;
				}
				printf("\n1111111111111111111 =%d =  %s \n", i, tmpAddr);
				memset(tmpStr, 0, sizeof(tmpStr));
				zoneDateTimeToString(gPhotoMsgList->LylyInfo[i].Time, tmpStr);
				setPhotoMsgListTime(i, tmpStr, gPhotoMsgList->LylyInfo[i].UnRead);
				get_dev_description(gPhotoMsgList->LylyInfo[i].DevType, gPhotoMsgList->LylyInfo[i].DevNo, tmpStr, sizeof(tmpStr)); // 获得设备描述
				setPhotoMsgListSender(i, tmpStr, gPhotoMsgList->LylyInfo[i].UnRead);
				setPhotoMsgListIsEnable(i, true);
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

		if (i <= (gPhotoMsgNum / PHOTOMSG_NUM_PER_PAGE))
		{
			if ((i == (gPhotoMsgNum / PHOTOMSG_NUM_PER_PAGE)) && (gPhotoMsgNum % PHOTOMSG_NUM_PER_PAGE) == 0)
			{
				if (gPhotoMsgNum == 0)
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

	printf("\n 22222222222222222222222222!  = %d   = %s \n", index, tmpStr);

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
		printf("\n open  minipic jepg icon icon failed!  = %d  \n", index);
		ituWidgetSetVisible(photoMsgListMiniPicIcon, false);
		return false;
	}
	if (gPhotoMsgListIconData)
	{
		ituIconLoadJpegData((ITUIcon*)photoMsgListMiniPicIcon, gPhotoMsgListIconData, gPhotoMsgListIconSize);
		photoMsgListMiniPicIcon->widget.flags |= ITU_EXTERNAL_IMAGE;
		printf("\n 333333333333333333333333333!  = %d   = %d \n", gPhotoMsgListIconData, gPhotoMsgListIconSize);

	}
	else
	{
		printf("\n load minipic jepg icon failed!  = %d \n ", index);
		ituWidgetSetVisible(photoMsgListMiniPicIcon, false);
		return false;
	}
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

	return true;
}


void setPhotoMsgListSender(uint8_t index, char* senderStr, bool isUnread)
{
	char tmpStr[50] = { 0 };

	if (index > MAX_POOTOMSG_LIST_NUM)
	{
		printf("photomsg list index overflow!!!!!!!!!!!!!");
		return false;
	}

	memset(tmpStr, 0, sizeof(tmpStr));
	sprintf(tmpStr, "%s%d", "photoMsgListSenderText", index);
	photoMsgListSenderText = ituSceneFindWidget(&theScene, tmpStr);
	assert(photoMsgListSenderText);

	ituTextSetString(photoMsgListSenderText, senderStr);

	if (isUnread)
	{
		ituSetColor((ITUColor*)&photoMsgListSenderText->color, 255, 255, 0, 0);
	}
	else
	{
		ituSetColor((ITUColor*)&photoMsgListSenderText->color, 255, 255, 255, 255);
	}

	return true;
}


void photoMsgVideoStatusBtnOnClicked(PHOTOMSG_BTN_e btnId)
{
	switch (btnId)
	{
	case PHOTOMSG_BTN_START:
		//暂停时候，继续播放留影留言、停止时候，重新播放留影留言
		setPhotoMsgVideoPlayByIndex(gPhotoMsgCurrentIndex);
		//setPhotoMsgVideoPlayStatusSetting(PHOTOMSG_VIDEOPLAY_PLAYING);
		break;

	case PHOTOMSG_BTN_PAUSE:
		//暂停播放，记录播放位置
		setPhotoMsgVideoPlayStatusSetting(PHOTOMSG_VIDEOPLAY_PAUSE);
		break;

	case PHOTOMSG_BTN_STOP:
		//停止播放，重置界面
		//sys_stop_play_audio(SYS_MEDIA_MUSIC);
		sys_stop_play_leaveword();
		setPhotoMsgVideoPlayStatusSetting(PHOTOMSG_VIDEOPLAY_STOP);
		break;

	default:
		break;
	}
}


void setPhotoMsgVideoPlayStatusSetting(PHOTOMSG_VIDEOPLAY_STATUS_e mode)
{
	if (!photoMsgBottomBarContainer1)
	{
		photoMsgBottomBarContainer1 = ituSceneFindWidget(&theScene, "photoMsgBottomBarContainer1");
		assert(photoMsgBottomBarContainer1);
	}
	if (!photoMsgPreVideoButton)
	{
		photoMsgPreVideoButton = ituSceneFindWidget(&theScene, "photoMsgPreVideoButton");
		assert(photoMsgPreVideoButton);
	}
	if (!photoMsgNextVideoButton)
	{
		photoMsgNextVideoButton = ituSceneFindWidget(&theScene, "photoMsgNextVideoButton");
		assert(photoMsgNextVideoButton);
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
		if (ituWidgetIsVisible(photoMsgPreVideoButton))
		{
			ituWidgetSetVisible(photoMsgPreVideoButton, false);
		}
		if (ituWidgetIsVisible(photoMsgNextVideoButton))
		{
			ituWidgetSetVisible(photoMsgNextVideoButton, false);
		}
		gPhotoMsgVideoMode = PHOTOMSG_VIDEOPLAY_PLAYING;
		break;
	
	case PHOTOMSG_VIDEOPLAY_PAUSE:
		ituWidgetSetVisible(photoMsgPauseContainer, false);
		ituWidgetSetVisible(photoMsgPlayContainer, true);

		gPhotoMsgVideoMode = PHOTOMSG_VIDEOPLAY_PAUSE;
		break;

	case PHOTOMSG_VIDEOPLAY_STOP:
		ituWidgetSetVisible(photoMsgPauseContainer, false);
		ituWidgetSetVisible(photoMsgPlayContainer, true);

		if (!ituWidgetIsVisible(photoMsgBottomBarContainer1))
		{
			ituWidgetSetVisible(photoMsgBottomBarContainer1, true);
		}
		if (!ituWidgetIsVisible(photoMsgPreVideoButton))
		{
			ituWidgetSetVisible(photoMsgPreVideoButton, true);
		}
		if (!ituWidgetIsVisible(photoMsgNextVideoButton))
		{
			ituWidgetSetVisible(photoMsgNextVideoButton, true);
		}
		gPhotoMsgVideoMode = PHOTOMSG_VIDEOPLAY_STOP;
		break;

	default:
		break;
	}
}


void setPhotoMsgAudioPlayPicture(char* picAddr)
{
	FILE*	tmpFile;
	char	tmpStr[50] = { 0 };

	// try to load minipic jpeg file if exists
	tmpFile = fopen(picAddr, "rb");
	if (tmpFile)
	{
		struct stat sb;
		if (fstat(fileno(tmpFile), &sb) != -1)			//用_fileno代替fileno避免运行时候的警告！！！！（但是板子上编译不过！！！！）
		{
			gPhotoMsgVideoBackgroundSize = (int)sb.st_size;
			gPhotoMsgVideoBackgroundData = malloc(gPhotoMsgVideoBackgroundSize);
			if (gPhotoMsgVideoBackgroundSize)
			{
				gPhotoMsgVideoBackgroundSize = fread(gPhotoMsgVideoBackgroundData, 1, gPhotoMsgVideoBackgroundSize, tmpFile);
			}
		}
		fclose(tmpFile);
	}

	if (gPhotoMsgVideoBackgroundData)
	{
		ituIconLoadJpegData((ITUIcon*)photoMsgVideoDrawBackground, gPhotoMsgVideoBackgroundData, gPhotoMsgVideoBackgroundSize);
	}

	//TODO: 可能要对传进来的指针数据进行释放，看后期数据如何传递！！！
	free(gPhotoMsgVideoBackgroundData);
	return true;
}


void photoMsgPlayingStopCallback()
{
	printf("\n 1111111111111111111photoMsgPlayingCallback!!!!!!\n");
}


int32 photoMsgPlayingCallback(int32 playTime, int32 playPrecent, int32 state)
{
	printf("\n 22222222222222222222222photoMsgPlayingCallback  time = %d  percent = %d state = %d \n", playTime, playPrecent, state);
	photoMsgVideoStatusBtnOnClicked(PHOTOMSG_BTN_STOP);

	return 0;
}


void setPhotoMsgPreNextVideo(PHOTOMSG_BTN_e btnId)
{
	if (gPhotoMsgNum <= 1)
	{
		//只有一条时候不进行上下条操作
		return;
	}
	switch (btnId)
	{
	case PHOTOMSG_BTN_PRE:
		gPhotoMsgCurrentIndex--;
		if (gPhotoMsgCurrentIndex < 0)
		{
			gPhotoMsgCurrentIndex = gPhotoMsgNum - 1;
		}
		break;

	case PHOTOMSG_BTN_NEXT:
		gPhotoMsgCurrentIndex++;
		if (gPhotoMsgCurrentIndex >= gPhotoMsgNum)
		{
			gPhotoMsgCurrentIndex = 0;
		}
		break;

	default:
		break;
	}
	setPhotoMsgVideoPlayByIndex(gPhotoMsgCurrentIndex);
}


void setPhotoMsgVideoPlayByIndex(uint8_t index)
{
	//TODO：相对应逻辑存储操作，设置页面按键状态。最后开始播放！
	char tmpAddr[50] = { 0 };

	setPhotoMsgPlayVol(gPhotoMsgPlayVol);
	media_set_ring_volume(gPhotoMsgPlayVol);	//TODO:通知逻辑设置音量！！！！！！
	setPhotoMsgVideoPlayStatusSetting(PHOTOMSG_VIDEOPLAY_PLAYING);
	storage_set_lylyrecord_flag(index, false);	//设置为已读留言
	sys_sync_hint_state();

	switch (gPhotoMsgList->LylyInfo[index].LyType)
	{
	case LYLY_TYPE_AUDIO:
		get_lylywav_path(tmpAddr, &gPhotoMsgList->LylyInfo[index].Time);
		//开始播放纯音频文件！！！！！！
		//sys_start_play_audio(SYS_MEDIA_MUSIC, tmpAddr, false, storage_get_ringvolume(), photoMsgPlayingCallback, photoMsgPlayingStopCallback);
		sys_start_play_leaveword(tmpAddr, LYLY_TYPE_AUDIO, storage_get_ringvolume(), photoMsgPlayingCallback, photoMsgPlayingStopCallback);
		break;

	case LYLY_TYPE_PIC_AUDIO:
		get_lylypic_path(tmpAddr, &gPhotoMsgList->LylyInfo[index].Time);
		setPhotoMsgAudioPlayPicture(tmpAddr);
		get_lylywav_path(tmpAddr, &gPhotoMsgList->LylyInfo[index].Time);
		//开始播放音频文件
		//sys_start_play_audio(SYS_MEDIA_MUSIC, tmpAddr, false, storage_get_ringvolume(), photoMsgPlayingCallback, photoMsgPlayingStopCallback);
		sys_start_play_leaveword(tmpAddr, LYLY_TYPE_PIC_AUDIO, storage_get_ringvolume(), photoMsgPlayingCallback, photoMsgPlayingStopCallback);
		break;

	case LYLY_TYPE_VIDEO:
		BackgroundDrawVideo_init("photoMsgVideoDrawButton");
		get_lylyavi_path(tmpAddr, &gPhotoMsgList->LylyInfo[index].Time);
		//开始播放视频文件！
		sys_start_play_leaveword(tmpAddr, LYLY_TYPE_VIDEO, storage_get_ringvolume(), photoMsgPlayingCallback, photoMsgPlayingStopCallback);
		break;

	default:
		break;
	}
}


void photoMsgReturnBtnOnClicked()
{
	printf("photoMsg Return BTN clieked ");

	if (ituWidgetIsVisible(photoMsgVideoDrawBackground))
	{
		if (gPhotoMsgVideoMode != PHOTOMSG_VIDEOPLAY_STOP)
		{
			photoMsgVideoStatusBtnOnClicked(PHOTOMSG_BTN_STOP);
		}
		else
		{
			photoMsgLayerInit(PHOTOMSG_LIST_PAGE);
			setPhotoMsgList();
		}
		
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

