/*********************************************************
Copyright (C), 2015-2020, Aurine
File name:	layer_photomsg.c
Author:		WuZ
Version:   	1.0
Date:		2016-06-30
Description: ��Ӱ���Բ�������
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


static uint8_t*	gPhotoMsgListIconData;
static int		gPhotoMsgListIconSize;
static uint8_t*	gPhotoMsgVideoBackgroundData;
static int		gPhotoMsgVideoBackgroundSize;
static uint8_t	gPhotoMsgPlayVol = 0;
static uint8_t  gPhotoMsgNum;
static PLYLYLIST_INFO gPhotoMsgList = NULL;
static PHOTOMSG_VIDEOPLAY_STATUS_e	gPhotoMsgVideoMode;		//��¼��ǰ�Ľ���Ĳ���״̬

bool photoMsgLayerOnEnter(ITUWidget* widget, char* param)
{
	photoMsgLayerInit(PHOTOMSG_LIST_PAGE); 
	ituCoverFlowGoto(photoMsgListCoverFlow, 0);
	setPhotoMsgList();

	//TODO:��ʱ��ȫ�ֱ���
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

	//TODO:֪ͨ�߼���������������������
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
			//TODO:����д�����Ϣ�Ĵ洢����
			setPhotoMsgList();
		}
		else
		{
			//TODO:����дɾ��һ���Ĳ�����ɾ�����˳���Ϣ���ݽ��棡������������
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
	//TODO:������Է��룡
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

	//TODO:��ȡ��ǰϵͳ����ֵ��
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
	char tmpAddr[50] = { 0 };

	if (gPhotoMsgList != NULL)
	{
		free(gPhotoMsgList);
		gPhotoMsgList = NULL;
	}
	storage_get_lylyrecord(&gPhotoMsgList);
	gPhotoMsgNum = gPhotoMsgList->Count;
	
	//TODO: ��ȡ�洢���������б���Ϣ��������������
	for (i = 0; i < MAX_POOTOMSG_LIST_NUM; i++)
	{
		if (gPhotoMsgNum < PHOTOMSG_NUM_PER_PAGE)
		{
			if (i < gPhotoMsgNum)
			{
				switch (gPhotoMsgList->LylyInfo[i].LyType)
				{
				case LYLY_TYPE_AUDIO:
					break;

				case LYLY_TYPE_PIC_AUDIO:
					get_lylypic_path(tmpAddr, &gPhotoMsgList->LylyInfo[i].Time);
					setPhotoMsgListMiniIcon(i, tmpAddr);
					break;

				case LYLY_TYPE_VIDEO:
					break;

				default:
					break;
				}
				memset(tmpStr, 0, sizeof(tmpStr));
				zoneDateTimeToString(gPhotoMsgList->LylyInfo[i].Time, tmpStr);
				setPhotoMsgListTime(i, tmpStr, gPhotoMsgList->LylyInfo[i].UnRead);
			}
			else if (i >= gPhotoMsgNum && i < PHOTOMSG_NUM_PER_PAGE)
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
			if (i < gPhotoMsgNum)
			{
				switch (gPhotoMsgList->LylyInfo[i].LyType)
				{
				case LYLY_TYPE_AUDIO:
					break;

				case LYLY_TYPE_PIC_AUDIO:
					get_lylypic_path(tmpAddr, &gPhotoMsgList->LylyInfo[i].Time);
					setPhotoMsgListMiniIcon(i, tmpAddr);
					break;

				case LYLY_TYPE_VIDEO:
					break;

				default:
					break;
				}
				memset(tmpStr, 0, sizeof(tmpStr));
				zoneDateTimeToString(gPhotoMsgList->LylyInfo[i].Time, tmpStr);
				setPhotoMsgListTime(i, tmpStr, gPhotoMsgList->LylyInfo[i].UnRead);
			}
			else
			{
				setPhotoMsgListIsVisible(i, false);
			}
		}
		
	}
	//��û�����ݵ�ҳ�������������ﵽ���ܻ�����Ч����
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

	// try to load minipic jpeg file if exists
	tmpFile = fopen(iconAddr, "rb");
	if (tmpFile)
	{
		struct stat sb;	
		if (fstat(fileno(tmpFile), &sb) != -1)			//��_fileno����fileno��������ʱ��ľ��棡�����������ǰ����ϱ��벻������������
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

	//TODO: ����Ҫ�Դ�������ָ�����ݽ����ͷţ�������������δ��ݣ�����
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

	//TODO: ����Ҫ�Դ�������ָ�����ݽ����ͷţ�������������δ��ݣ�����
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
		if (fstat(fileno(tmpFile), &sb) != -1)			//��_fileno����fileno��������ʱ��ľ��棡�����������ǰ����ϱ��벻������������
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

	//TODO: ����Ҫ�Դ�������ָ�����ݽ����ͷţ�������������δ��ݣ�����
	free(gPhotoMsgVideoBackgroundData);
	return true;
}


void setPhotoMsgVideoPlayByIndex(uint8_t index)
{
	//TODO�����Ӧ�߼��洢����������ҳ�水��״̬�����ʼ���ţ�
	char tmpAddr[50] = { 0 };

	switch (gPhotoMsgList->LylyInfo[index].LyType)
	{
	case LYLY_TYPE_AUDIO:
		get_lylywav_path(tmpAddr, &gPhotoMsgList->LylyInfo[index].Time);
		//��ʼ���Ŵ���Ƶ�ļ�������������
		break;

	case LYLY_TYPE_PIC_AUDIO:
		get_lylypic_path(tmpAddr, &gPhotoMsgList->LylyInfo[index].Time);
		setPhotoMsgAudioPlayPicture(tmpAddr);
		get_lylywav_path(tmpAddr, &gPhotoMsgList->LylyInfo[index].Time);
		//��ʼ������Ƶ�ļ�

		break;

	case LYLY_TYPE_VIDEO:
		get_lylyavi_path(tmpAddr, &gPhotoMsgList->LylyInfo[index].Time);
		//��ʼ������Ƶ�ļ���
		break;

	default:
		break;
	}

	storage_set_lylyrecord_flag(index, false);
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

