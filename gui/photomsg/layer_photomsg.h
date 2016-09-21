/*********************************************************
Copyright (C), 2015-2020, Aurine
File name:	layer_photomsg.h
Author:		WuZ
Version:   	1.0
Date:		2016-06-30
Description: ÁôÓ°ÁôÑÔ²Ù×÷º¯Êý
History:
1. Date:
Author:
Modification:
2. ...
*********************************************************/

#ifndef LAYER_PHOTOMSG_H
#define LAYER_PHOTOMSG_H

#include "../gui_include.h"


#define PHOTOMSG_PLAY_VOL			5
#define MAX_POOTOMSG_LIST_NUM		20
#define PHOTOMSG_NUM_PER_PAGE		6
#define MAX_PHOTOMSG_PAGE_NUM		4

typedef enum
{
	PHOTOMSG_LIST_PAGE,
	PHOTOMSG_CONTENT_PAGE,
}PHOTOMSG_PAGE_e;

typedef enum
{
	PHOTOMSG_BTN_START,
	PHOTOMSG_BTN_PAUSE,
	PHOTOMSG_BTN_STOP,
	PHOTOMSG_BTN_TETURN,
	PHOTOMSG_BTN_EMPTY,
	PHOTOMSG_BTN_DELETE,
	PHOTOMSG_BTN_VOL,
}PHOTOMSG_BTN_e;

typedef enum
{
	PHOTOMSG_MSG_BTN_CANCEL,
	PHOTOMSG_MSG_BTN_CONFIRM,
}PHOTOMSG_MSG_BTN_e;

typedef enum
{
	PHOTOMSG_VIDEOPLAY_PLAYING,
	PHOTOMSG_VIDEOPLAY_PAUSE,
	PHOTOMSG_VIDEOPLAY_STOP,
}PHOTOMSG_VIDEOPLAY_STATUS_e;


bool photoMsgLayerOnEnter(ITUWidget* widget, char* param);
bool photoMsgLayerOnLeave(ITUWidget* widget, char* param);
bool photoMsgListOnClicked(ITUWidget* widget, char* param);
bool photoMsgBtnOnClicked(ITUWidget* widget, char* param);
bool photoMsgMsgBoxBtnOnClicked(ITUWidget* widget, char* param);
bool photoMsgVideoDrawBtnOnClicked(ITUWidget* widget, char* param);
bool photoMsgVolTrackBarOnChanged(ITUWidget* widget, char* param);
void photoMsgLayerReset();

void photoMsgLayerInit(PHOTOMSG_PAGE_e pageId);
void setPhotoMsgVideoPlayByIndex(uint8_t index);
void setPhotoMsgVideoPlayStatusSetting(PHOTOMSG_VIDEOPLAY_STATUS_e mode);
void setPhotoMsgAudioPlayPicture(char* picAddr);
void photoMsgVideoStatusBtnOnClicked(PHOTOMSG_BTN_e btnId);
void photoMsgReturnBtnOnClicked();
void photoMsgBoxShow(PHOTOMSG_BTN_e btnId);
void setPhotoMsgList();
void photoMsgVoiceBtnOnClicked();
void setPhotoMsgPlayVol(uint8_t volNum);
bool setPhotoMsgListMiniIcon(uint8_t index, char* iconAddr);
bool setPhotoMsgListTime(uint8_t index, char* timeStr, bool isUnread);
bool setPhotoMsgListIsVisible(uint8_t index, bool status);



#endif // GUI_PHOTOMSG_H