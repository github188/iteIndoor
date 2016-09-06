/*********************************************************
Copyright (C), 2015-2020, Aurine
File name:	layer_recorder.c
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

#include "layer_recorder.h"

static ITULayer*		mainLayer;
static ITUBackground*	recorderTipsTransparencyBackground;
static ITUBackground*	recorderBackground;
static ITUText*			recorderTipsText;
static ITUTrackBar*		recorderVolTrackBar;
static ITUIcon*			recorderVoiceOffIcon;
static ITUIcon*			recorderVoiceOnIcon;
static ITUText*			recorderRecordDurationText; 
static ITUText*			recorderRecordCreateTimeText;
static ITUText*			recorderRecordText;
static ITUIcon*			recorderRecordTopLeftIcon;
static ITUButton*		recorderRecordNullButton;
static ITURadioBox*		recorderRecordRadioBox;

static ITUButton*		recorderRecordStartButton;   
static ITUButton*		recorderRecordPauseButton;
static ITUButton*		recorderRecordPlayButton;
static ITUButton*		recorderRecordStopButton;


//静态全局参数，用户保存界面状态信息等参数
static uint8_t		gRecorderPlayVol;			//用来记录播放音量
static uint32_t		gRecorderLastTimeTick;		//用来记录定时器上个时刻的时间
static uint8_t		gRecorderTimeCount;			//用来显示计时
static uint8_t		gRecordNumCount;			//用来记录录音数量
static uint8_t		gCurrentRecordIndex;		//用来记录当前选中的录音
static uint8_t		gNewRecordIndex;			//用来记录新录音的存放位置
static RECORDER_STATUS_e	gRecorderStatus;	//用来保存界面当前状态（播放、录音、暂停、停止）


bool recorderLayerOnEnter(ITUWidget* widget, char* param)
{
	gRecorderStatus = RECORDER_STATUS_STOP;
	gRecorderTimeCount = 0;
	gRecordNumCount = 0;

	recorderLayerInit();

	//TODO:获取系统当前音量
	gRecorderPlayVol = DEFAULT_PLAYVOL;
	media_set_ring_volume(DEFAULT_PLAYVOL);
	setRecorderPlayVol(gRecorderPlayVol);

	return true;
}

void recorderLayerInit()
{
	uint8_t i = 0;

	if (!recorderTipsTransparencyBackground)
	{
		recorderTipsTransparencyBackground = ituSceneFindWidget(&theScene, "recorderTipsTransparencyBackground");
		assert(recorderTipsTransparencyBackground);
	}

	for (i = 0; i < MAX_RECORDER_NUM; i++)
	{
		//TODO:读取存储设置界面记录内容！！并设置第一个非空录音为默认选项
		gRecordNumCount = 3;

		//if (i == 1)
		//	setRecorderRecordStatus(i, RECORDER_RECORD_NULL);
		//else
			setRecorderRecordStatus(i, RECORDER_RECORD_UNREAD);

		setRecordRadioBoxStatus(i, false);
	}

	gCurrentRecordIndex = MAX_RECORDER_NUM;		//初始化时候当前无选中赋值一个大于个数的值就行

	setRecorderRecordCreateTime("");

	setRecorderDuration("00:00");

	setRecorderAudioBtnStatus(RECORDER_STATUS_NULL);

	ituWidgetSetVisible(recorderTipsTransparencyBackground, false);

}


bool recorderBtnOnClicked(ITUWidget* widget, char* param)
{
	uint8_t tmpIndex = (RECORDER_BTN_e)atoi(param);

	printf("recorderBtnOnClicked = %d", tmpIndex);

	switch (tmpIndex)
	{
	case RECORDER_BTN_RETURN:
		recorderReturnBtnOnClicked();
		break;

	case RECORDER_BTN_DELETE:
		recorderDeleteBtnOnClicked();
		break;

	case RECORDER_BTN_VOICE:
		recorderVoiceBtnOnClicked();
		break;

	case RECORDER_BTN_RECORD:
		recorderRecordingBtnOnClicked();
		break;

	case RECORDER_BTN_PLAY:
		recorderPlayingBtnOnClicked();
		break;

	case RECORDER_BTN_PAUSE:
		recorderPauseBtnOnClicked();
		break;

	case RECORDER_BTN_STOP:
		recorderStopBtnOnClicked();
		break;

	default:
		break;
	}

	return true;
}


bool recorderTimingOnTimer(ITUWidget* widget, char* param)
{
	char buf[10] = { 0 };

	if (gRecorderStatus == RECORDER_STATUS_PLAYING || gRecorderStatus == RECORDER_STATUS_RECORDING)
	{
		if (gRecorderTimeCount < RECORD_TIME_MAX)
		{
			uint32_t duration;
			uint32_t curtime = SDL_GetTicks();

			if (curtime >= gRecorderLastTimeTick)
			{
				duration = curtime - gRecorderLastTimeTick;
			}
			else
			{
				duration = 0xFFFFFFFF - gRecorderLastTimeTick + curtime;
			}

			if (duration >= MS_PER_SECOND)		//时间差是以毫秒为单位
			{
				gRecorderTimeCount += (uint32_t)duration / 1000;
				gRecorderLastTimeTick = curtime;

				if (((int)gRecorderTimeCount) < RECORD_TIME_MAX)
				{
					memset(buf, 0, sizeof(buf));
					if (gRecorderTimeCount < 10)
					{
						sprintf(buf, "%s%d", "00:0", gRecorderTimeCount);
					}
					else
					{
						sprintf(buf, "%s%d", "00:", gRecorderTimeCount);
					}
					setRecorderDuration(buf);
				}
				else
				{
					//TODO:超时停止录音，设置界面状态
					sys_stop_family_record();
					setRecorderRecordStatus(gNewRecordIndex, RECORDER_RECORD_UNREAD);
					setRecordRadioBoxStatus(gNewRecordIndex, true);
				}
			}
		}	
	}

	return true;
}


bool recorderRecordRadioBoxChanged(ITUWidget* widget, char* param)
{
	char tmpIndex = atoi(param);
	gCurrentRecordIndex = tmpIndex;

	//TODO:读取存储填充录音录制时间！！！！！
	setRecorderRecordCreateTime("2016-07-28 10:41:45");
	setRecorderAudioBtnStatus(RECORDER_STATUS_STOP);

	return true;
}


bool recorderVolTrackBarOnChanged(ITUWidget* widget, char* param)
{
	printf("PlayVol = %s", param);

	uint8_t tmpVol = atoi(param);
	gRecorderPlayVol = tmpVol;
	media_set_ring_volume(gRecorderPlayVol);	//TODO:通知逻辑设置音量！！！！！！

	if (tmpVol == 0)
	{
		ituWidgetSetVisible(recorderVoiceOnIcon, false);
		ituWidgetSetVisible(recorderVoiceOffIcon, true);
	}
	else
	{
		ituWidgetSetVisible(recorderVoiceOffIcon, false);
		ituWidgetSetVisible(recorderVoiceOnIcon, true);
	}

	return true;
}


void setRecorderPlayVol(uint8_t volNum)
{
	if (!recorderVolTrackBar)
	{
		recorderVolTrackBar = ituSceneFindWidget(&theScene, "recorderVolTrackBar");
		assert(recorderVolTrackBar);
	}
	if (!recorderVoiceOnIcon)
	{
		recorderVoiceOnIcon = ituSceneFindWidget(&theScene, "recorderVoiceOnIcon");
		assert(recorderVoiceOnIcon);
	}
	if (!recorderVoiceOffIcon)
	{
		recorderVoiceOffIcon = ituSceneFindWidget(&theScene, "recorderVoiceOffIcon");
		assert(recorderVoiceOffIcon);
	}

	if (volNum == 0)
	{
		ituWidgetSetVisible(recorderVoiceOnIcon, false);
		ituWidgetSetVisible(recorderVoiceOffIcon, true);
	}
	else
	{
		ituWidgetSetVisible(recorderVoiceOffIcon, false);
		ituWidgetSetVisible(recorderVoiceOnIcon, true);
	}

	ituTrackBarSetValue(recorderVolTrackBar, volNum);
}


void setRecorderAudioBtnStatus(RECORDER_STATUS_e status)
{
	if (!recorderRecordStartButton)
	{
		recorderRecordStartButton = ituSceneFindWidget(&theScene, "recorderRecordStartButton");
		assert(recorderRecordStartButton);
	}
	if (!recorderRecordPauseButton)
	{
		recorderRecordPauseButton = ituSceneFindWidget(&theScene, "recorderRecordPauseButton");
		assert(recorderRecordPauseButton);
	}
	if (!recorderRecordPlayButton)
	{
		recorderRecordPlayButton = ituSceneFindWidget(&theScene, "recorderRecordPlayButton");
		assert(recorderRecordPlayButton);
	}
	if (!recorderRecordStopButton)
	{
		recorderRecordStopButton = ituSceneFindWidget(&theScene, "recorderRecordStopButton");
		assert(recorderRecordStopButton);
	}
	
	switch (status)
	{
	case RECORDER_STATUS_RECORDING:
		ituWidgetSetVisible(recorderRecordStartButton, false);
		ituWidgetSetVisible(recorderRecordPauseButton, false);
		ituWidgetSetVisible(recorderRecordPlayButton, false);
		ituWidgetSetVisible(recorderRecordStopButton, true);
		break;

	case RECORDER_STATUS_PLAYING:
		ituWidgetSetVisible(recorderRecordPlayButton, false);
		ituWidgetSetVisible(recorderRecordStartButton, false);
		ituWidgetSetVisible(recorderRecordStopButton, true);
		ituWidgetSetVisible(recorderRecordPauseButton, true);
		break;

	case RECORDER_STATUS_PAUSE:
		ituWidgetSetVisible(recorderRecordStartButton, false);
		ituWidgetSetVisible(recorderRecordPauseButton, false);
		ituWidgetSetVisible(recorderRecordPlayButton, true);
		ituWidgetSetVisible(recorderRecordStopButton, true);
		break;

	case RECORDER_STATUS_STOP:
		ituWidgetSetVisible(recorderRecordStopButton, false);
		ituWidgetSetVisible(recorderRecordPauseButton, false);
		ituWidgetSetVisible(recorderRecordPlayButton, true);
		ituWidgetSetVisible(recorderRecordStartButton, true);
		break;

	case RECORDER_STATUS_NULL:
		ituWidgetSetVisible(recorderRecordStopButton, false);
		ituWidgetSetVisible(recorderRecordPauseButton, false);
		ituWidgetSetVisible(recorderRecordPlayButton, false);
		ituWidgetSetVisible(recorderRecordStartButton, true);
		break;

	default:
		break;
	}

	gRecorderStatus = status;
}

void setRecorderRecordStatus(uint8_t index, RECORDER_RECORD_STATUS_e status)
{
	char tmpStr[50] = { 0 };

	if (index >= MAX_RECORDER_NUM)
	{
		printf("recorder index is overFlow!!!!");
		return;
	}

	memset(tmpStr, 0, sizeof(tmpStr));
	sprintf(tmpStr, "%s%d", "recorderRecordRadioBox", index);
	recorderRecordRadioBox = ituSceneFindWidget(&theScene, tmpStr);
	assert(recorderRecordRadioBox);

	memset(tmpStr, 0, sizeof(tmpStr));
	sprintf(tmpStr, "%s%d", "recorderRecordText", index);
	recorderRecordText = ituSceneFindWidget(&theScene, tmpStr);
	assert(recorderRecordText);

	memset(tmpStr, 0, sizeof(tmpStr));
	sprintf(tmpStr, "%s%d", "recorderRecordTopLeftIcon", index);
	recorderRecordTopLeftIcon = ituSceneFindWidget(&theScene, tmpStr);
	assert(recorderRecordTopLeftIcon);

	memset(tmpStr, 0, sizeof(tmpStr));
	sprintf(tmpStr, "%s%d", "recorderRecordNullButton", index);
	recorderRecordNullButton = ituSceneFindWidget(&theScene, tmpStr);
	assert(recorderRecordNullButton);

	switch (status)
	{
	case RECORDER_RECORD_NULL:
		ituWidgetSetVisible(recorderRecordTopLeftIcon, false);
		ituWidgetSetVisible(recorderRecordRadioBox, false);
		ituWidgetSetVisible(recorderRecordText, false);
		ituWidgetSetVisible(recorderRecordNullButton, true);
		break;

	case RECORDER_RECORD_READ:
		ituWidgetSetVisible(recorderRecordTopLeftIcon, false);
		ituWidgetSetVisible(recorderRecordNullButton, false);
		ituWidgetSetVisible(recorderRecordRadioBox, true);
		ituWidgetSetVisible(recorderRecordText, true);
		break;

	case RECORDER_RECORD_UNREAD:
		ituWidgetSetVisible(recorderRecordNullButton, false);
		ituWidgetSetVisible(recorderRecordTopLeftIcon, true);
		ituWidgetSetVisible(recorderRecordRadioBox, true);
		ituWidgetSetVisible(recorderRecordText, true);
		break;

	default:
		break;
	}

}

RECORDER_RECORD_STATUS_e getRecorderRecordStatus(uint8_t index)
{
	char tmpStr[50] = { 0 };

	if (index >= MAX_RECORDER_NUM)
	{
		printf("recorder index is overFlow!!!!");
		return (RECORDER_RECORD_STATUS_e)0;
	}

	sprintf(tmpStr, "%s%d", "recorderRecordRadioBox", index);
	recorderRecordRadioBox = ituSceneFindWidget(&theScene, tmpStr);
	assert(recorderRecordRadioBox);

	sprintf(tmpStr, "%s%d", "recorderRecordText", index);
	recorderRecordText = ituSceneFindWidget(&theScene, tmpStr);
	assert(recorderRecordText);

	sprintf(tmpStr, "%s%d", "recorderRecordTopLeftIcon", index);
	recorderRecordTopLeftIcon = ituSceneFindWidget(&theScene, tmpStr);
	assert(recorderRecordTopLeftIcon);

	sprintf(tmpStr, "%s%d", "recorderRecordNullButton", index);
	recorderRecordNullButton = ituSceneFindWidget(&theScene, tmpStr);
	assert(recorderRecordNullButton);


	if ((ituWidgetIsVisible(recorderRecordTopLeftIcon) == false) && (ituWidgetIsVisible(recorderRecordRadioBox) == false) && (ituWidgetIsVisible(recorderRecordText) == false) && (ituWidgetIsVisible(recorderRecordNullButton) == true))
		return RECORDER_RECORD_NULL;

	else if ((ituWidgetIsVisible(recorderRecordTopLeftIcon) == false) && (ituWidgetIsVisible(recorderRecordRadioBox) == true) && (ituWidgetIsVisible(recorderRecordText) == true) && (ituWidgetIsVisible(recorderRecordNullButton) == false))
		return RECORDER_RECORD_READ;

	else if ((ituWidgetIsVisible(recorderRecordTopLeftIcon) == true) && (ituWidgetIsVisible(recorderRecordRadioBox) == true) && (ituWidgetIsVisible(recorderRecordText) == true) && (ituWidgetIsVisible(recorderRecordNullButton) == false))
		return RECORDER_RECORD_UNREAD;

	else
		return (RECORDER_RECORD_STATUS_e)0;
}


void recorderRecordingBtnOnClicked()
{
	uint8_t i = 0;
	char tmpFileName[50] = { 0 };
	PDATE_TIME tmpTime;
	bool recordFlag = false;

	for (i = 0; i < MAX_RECORDER_NUM; i++)
	{
		//顺序找到第一个空录音位置即可！！！
		if (getRecorderRecordStatus(i) == RECORDER_RECORD_NULL)
		{
			gNewRecordIndex = i;
			recordFlag = true;
			break;				
		}
	}
	if (recordFlag)
	{
		gRecorderLastTimeTick = SDL_GetTicks();		//开启定时器前要先获取一次当前时间以便对比

		//TODO:开始录音存储
		get_timer(tmpTime);
		memset(tmpFileName, 0, sizeof(tmpFileName));
		sprintf(tmpFileName, "%s/%04d%02d%02d%02d%02d%02d.WAV", JRLY_DIR_PATH, tmpTime->year, tmpTime->month, tmpTime->day, tmpTime->hour, tmpTime->min, tmpTime->sec);
		sys_start_family_record(tmpFileName, NULL, NULL);

		setRecorderAudioBtnStatus(RECORDER_STATUS_RECORDING);
	}
}


void recorderPlayingBtnOnClicked()
{
	char tmpStr[50] = { 0 };

	if (gRecordNumCount > 0)
	{
		gRecorderLastTimeTick = SDL_GetTicks();		//开启定时器前要先获取一次当前时间以便对比

		if (getRecorderRecordStatus(gCurrentRecordIndex) == RECORDER_RECORD_UNREAD)
		{
			//TODO:设置存储为已读
			setRecorderRecordStatus(gCurrentRecordIndex, RECORDER_RECORD_READ);
		}

		if (gRecorderTimeCount > 0)
		{
			//TODO:继续播放录音、并继续计时（失能上面的RadioBox按键）
		}
		else
		{
			//TODO:重头开始播放录音、并重头开始计时（失能上面的RadioBox按键）

		}

		setRecorderAudioBtnStatus(RECORDER_STATUS_PLAYING);
	}
}


void recorderPauseBtnOnClicked()
{
	setRecorderAudioBtnStatus(RECORDER_STATUS_PAUSE);
}


void recorderStopBtnOnClicked()
{
	if (gRecorderStatus == RECORDER_STATUS_PLAYING)
	{
		//TODO:播放停止
	}
	else if (gRecorderStatus == RECORDER_STATUS_RECORDING)
	{
		//TODO:录音停止
		sys_stop_family_record();
		setRecorderRecordStatus(gNewRecordIndex, RECORDER_RECORD_UNREAD);
		setRecordRadioBoxStatus(gNewRecordIndex, true);
	}
	gRecorderTimeCount = 0;
	setRecorderDuration("00:00");
	setRecorderAudioBtnStatus(RECORDER_STATUS_STOP);

}


void setRecorderDuration(char* timeStr)
{
	if (!recorderRecordDurationText)
	{
		recorderRecordDurationText = ituSceneFindWidget(&theScene, "recorderRecordDurationText");
		assert(recorderRecordDurationText);
	}
	ituTextSetString(recorderRecordDurationText, timeStr);
}


void setRecorderRecordCreateTime(char* timeStr)
{
	if (!recorderRecordCreateTimeText)
	{
		recorderRecordCreateTimeText = ituSceneFindWidget(&theScene, "recorderRecordCreateTimeText");
		assert(recorderRecordCreateTimeText);
	}
	ituTextSetString(recorderRecordCreateTimeText, timeStr);
}


void setRecordRadioBoxStatus(uint8_t index, bool status)
{
	char tmpStr[50] = { 0 };

	sprintf(tmpStr, "%s%d", "recorderRecordRadioBox", index);
	recorderRecordRadioBox = ituSceneFindWidget(&theScene, tmpStr);
	assert(recorderRecordRadioBox);

	if (status == true)
	{
		gCurrentRecordIndex = index;
	}

	ituRadioBoxSetChecked(recorderRecordRadioBox, status);
}


void recorderReturnBtnOnClicked()
{
	printf("recorderReturnBtnOnClicked");

	if (!mainLayer)
	{
		mainLayer = ituSceneFindWidget(&theScene, "mainLayer");
		assert(mainLayer);
	}
	ituLayerGoto(mainLayer);
}


void recorderDeleteBtnOnClicked()
{
	printf("recorderDeleteBtnOnClicked");

	if (!recorderTipsText)
	{
		recorderTipsText = ituSceneFindWidget(&theScene, "recorderTipsText");
		assert(recorderTipsText);
	}

	if (!recorderBackground)
	{
		recorderBackground = ituSceneFindWidget(&theScene, "recorderBackground");
		assert(recorderBackground);
	}
	//TODO:添加语言翻译！

	ituTextSetString(recorderTipsText, "Delete Message");

	ituWidgetDisable(recorderBackground);
	ituWidgetSetVisible(recorderTipsTransparencyBackground, true);
}


void recorderVoiceBtnOnClicked()
{
	printf("recorderVoiceBtnOnClicked");

	//TODO:获取当前系统音量值，
	if (gRecorderPlayVol != 0)
	{
		gRecorderPlayVol = 0;
		setRecorderPlayVol(gRecorderPlayVol);
	}
	else
	{
		gRecorderPlayVol = DEFAULT_PLAY_VOL;
		setRecorderPlayVol(gRecorderPlayVol);
	}
	media_set_ring_volume(gRecorderPlayVol);	//TODO:通知逻辑设置音量！！！！！！
}


bool recorderMsgBoxBtnOnClicked(ITUWidget* widget, char* param)
{
	switch (atoi(param))
	{
	case RECORDER_MSG_BTN_COMFIRM:
		//TODO:删除本条留言信息，刷新存储，刷新界面！！！！！！
		if (gCurrentRecordIndex < MAX_RECORDER_NUM)
		{
			storage_del_jrlyrecord(gCurrentRecordIndex);
			recorderLayerInit();
		}
		break;

	default:
		break;
	}

	ituWidgetEnable(recorderBackground);
	ituWidgetSetVisible(recorderTipsTransparencyBackground, false);

	return true;
}