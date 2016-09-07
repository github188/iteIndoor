/*********************************************************
Copyright (C), 2015-2020, Aurine
File name:	layer_recorder.h
Author:		WuZ
Version:   	1.0
Date:		2016-06-30
Description: 家人留言操作函数
History:
1. Date:
Author:
Modification:
2. ...
*********************************************************/

#ifndef LAYER_RECORDER_H
#define LAYER_RECORDER_H

#include "../gui_include.h"

#define MS_PER_SECOND			1000
#define DEFAULT_PLAY_VOL		40
#define MAX_RECORDER_NUM		5
#define DEFAULT_PLAYVOL			5


typedef enum
{
	RECORDER_STATUS_RECORDING,
	RECORDER_STATUS_PLAYING,
	RECORDER_STATUS_PAUSE,
	RECORDER_STATUS_STOP,
	RECORDER_STATUS_NULL,
}RECORDER_STATUS_e;

typedef enum
{
	RECORDER_RECORD_NULL = 0,
	RECORDER_RECORD_READ,
	RECORDER_RECORD_UNREAD,
}RECORDER_RECORD_STATUS_e;

typedef enum
{
	RECORDER_BTN_RETURN,
	RECORDER_BTN_DELETE,
	RECORDER_BTN_VOICE,
	RECORDER_BTN_RECORD,
	RECORDER_BTN_PLAY,
	RECORDER_BTN_PAUSE,
	RECORDER_BTN_STOP,
}RECORDER_BTN_e;

typedef enum
{	
	RECORDER_MSG_BTN_CANCEL,
	RECORDER_MSG_BTN_COMFIRM,
}RECORDER_MSG_BTN_e;

bool recorderLayerOnEnter(ITUWidget* widget, char* param);
bool recorderLayerOnLeave(ITUWidget* widget, char* param);
bool recorderBtnOnClicked(ITUWidget* widget, char* param);
bool recorderMsgBoxBtnOnClicked(ITUWidget* widget, char* param);
bool recorderVolTrackBarOnChanged(ITUWidget* widget, char* param);
bool recorderTimingOnTimer(ITUWidget* widget, char* param);
bool recorderRecordRadioBoxChanged(ITUWidget* widget, char* param);

void recorderLayerInit();
void setRecorderPlayVol(uint8_t volNum);
void setRecorderRecordStatus(uint8_t index, RECORDER_RECORD_STATUS_e status);
RECORDER_RECORD_STATUS_e getRecorderRecordStatus(uint8_t index);
void setRecorderAudioBtnStatus(RECORDER_STATUS_e status);
void setRecorderDuration(char* timeStr);
void setRecorderRecordCreateTime(char* timeStr);
void setRecordRadioBoxStatus(uint8_t index, bool status);
void recorderReturnBtnOnClicked();
void recorderDeleteBtnOnClicked();
void recorderVoiceBtnOnClicked();
void recorderRecordingBtnOnClicked();
void recorderPlayingBtnOnClicked();
void recorderPauseBtnOnClicked();
void recorderStopBtnOnClicked();

#endif // GUI_RECORDER_H