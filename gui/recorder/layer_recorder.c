/*********************************************************
Copyright (C), 2015-2020, Aurine
File name:	layer_recorder.c
Author:		WuZ
Version:   	1.0
Date:		2016-06-30
Description: �������Բ�������
History:
1. Date:
Author:
Modification:
2. ...
*********************************************************/

#include "layer_recorder.h"

static ITULayer*		mainLayer = NULL;
static ITUBackground*	recorderTipsTransparencyBackground = NULL;
static ITUBackground*	recorderBackground = NULL;
static ITUText*			recorderTipsText = NULL;
static ITUTrackBar*		recorderVolTrackBar = NULL;
static ITUIcon*			recorderVoiceOffIcon = NULL;
static ITUIcon*			recorderVoiceOnIcon = NULL;
static ITUText*			recorderRecordDurationText = NULL;
static ITUText*			recorderRecordCreateTimeText = NULL;
static ITUText*			recorderRecordText = NULL;
static ITUIcon*			recorderRecordTopLeftIcon = NULL;
static ITUButton*		recorderRecordNullButton = NULL;
static ITURadioBox*		recorderRecordRadioBox = NULL;
static ITUButton*		recorderRecordStartButton = NULL;
static ITUButton*		recorderRecordPauseButton = NULL;
static ITUButton*		recorderRecordPlayButton = NULL;
static ITUButton*		recorderRecordStopButton = NULL;

//��̬ȫ�ֲ������û��������״̬��Ϣ�Ȳ���
static uint8_t		gRecorderPlayVol;			//������¼��������
static uint32_t		gRecorderLastTimeTick;		//������¼��ʱ���ϸ�ʱ�̵�ʱ��
static uint8_t		gRecorderTimeCount;			//������ʾ��ʱ
static uint8_t		gRecordNumCount;			//������¼¼������
static uint8_t		gCurrentRecordIndex;		//������¼��ǰѡ�е�¼��
static uint8_t		gNewRecordIndex;			//������¼��¼���Ĵ��λ��
static RECORDER_STATUS_e	gRecorderStatus;	//����������浱ǰ״̬�����š�¼������ͣ��ֹͣ��
static PJRLYLIST_INFO gRecorderList = NULL;
static DATE_TIME	gNewRecorderTime;

bool recorderLayerOnEnter(ITUWidget* widget, char* param)
{
	gRecorderTimeCount = 0;
	gRecordNumCount = 0;
	gCurrentRecordIndex = MAX_RECORDER_NUM;		//��ʼ��ʱ��ǰ��ѡ�и�ֵһ�����ڸ�����ֵ����
	gRecorderStatus = RECORDER_STATUS_NULL;

	recorderLayerInit();

	//TODO:��ȡϵͳ��ǰ����
	gRecorderPlayVol = JRLY_VOLUME;
	setRecorderPlayVol(gRecorderPlayVol);
	media_set_ring_volume(gRecorderPlayVol);
	
	return true;
}


bool recorderLayerOnLeave(ITUWidget* widget, char* param)
{
	printf("\nrecorderLayerOnLeave!!!!!!!!!\n");

	//TODO: ǿ���뿪ʱ���ж�ҳ��״̬���������¼��״̬�򱣴�¼���������� ����״̬��ֱ���˳����ɣ�

	if (gRecorderStatus != RECORDER_STATUS_STOP)
	{
		recorderStopBtnOnClicked();
	}

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

	storage_free_jrly_memory(&gRecorderList);
	storage_get_jrlyrecord(&gRecorderList);
	gRecordNumCount = gRecorderList->Count;

	for (i = 0; i < MAX_RECORDER_NUM; i++)
	{
		//TODO:��ȡ�洢���ý����¼���ݣ�����������Ĭ��ѡ�ɾ����Ĭ��ѡ���һ����
		if (i < gRecordNumCount)
		{
			if ((gRecorderList->JrlyInfo[i]).UnRead)
			{
				setRecorderRecordStatus(i, RECORDER_RECORD_UNREAD);
			}
			else
			{
				setRecorderRecordStatus(i, RECORDER_RECORD_READ);
			}
		}
		else
		{
			setRecorderRecordStatus(i, RECORDER_RECORD_NULL);
		}
		setRecordRadioBoxStatus(i, false);
	}

	setRecorderAudioBtnStatus(RECORDER_STATUS_NULL);
	setRecorderRecordCreateTime("");
	setRecorderDuration("00:00");
	setRecorderRadioBoxIsEnable(true);
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

			if (duration >= MS_PER_SECOND)		//ʱ������Ժ���Ϊ��λ
			{
				gRecorderTimeCount += (uint32_t)duration / 1000;
				gRecorderLastTimeTick = curtime;

				if (((int)gRecorderTimeCount) < RECORD_TIME_MAX)
				{
					memset(buf, 0, sizeof(buf));
					sprintf(buf, "%s%02d", "00:", gRecorderTimeCount);
					setRecorderDuration(buf);
				}
				else
				{
					//TODO:��ʱֹͣ¼�������ý���״̬
					recorderStopBtnOnClicked();
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
	setRecordRadioBoxStatus(gCurrentRecordIndex, true);

	return true;
}


bool recorderVolTrackBarOnChanged(ITUWidget* widget, char* param)
{
	printf("PlayVol = %s", param);

	uint8_t tmpVol = atoi(param);
	gRecorderPlayVol = tmpVol;
	media_set_ring_volume(gRecorderPlayVol);	//TODO:֪ͨ�߼���������������������

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
	bool recordFlag = false;

	for (i = 0; i < MAX_RECORDER_NUM; i++)
	{
		//˳���ҵ���һ����¼��λ�ü��ɣ�����
		if (getRecorderRecordStatus(i) == RECORDER_RECORD_NULL)
		{
			//gNewRecordIndex = i;
			recordFlag = true;
			break;				
		}
	}
	if (recordFlag)
	{
		gRecorderLastTimeTick = SDL_GetTicks();		//������ʱ��ǰҪ�Ȼ�ȡһ�ε�ǰʱ���Ա�Ա�

		//TODO:��ʼ¼���洢
		get_timer(&gNewRecorderTime);
		memset(tmpFileName, 0, sizeof(tmpFileName));
		get_jrlyrecord_path(tmpFileName, &gNewRecorderTime);
		sys_start_family_record(tmpFileName, NULL, NULL);
		setRecorderAudioBtnStatus(RECORDER_STATUS_RECORDING);
		setRecorderRadioBoxIsEnable(false);
	}
}


void recorderPlayingStopCallback()
{
	printf("\n 11111111111111111111recorderPlayingStopCallback!!!!!!\n");
}


int32 recorderPlayingCallback(int32 playTime, int32 playPrecent, int32 state)
{
	printf("\n 22222222222222222222222recorderPlayingcallback  time = %d  percent = %d state = %d \n", playTime, playPrecent, state);
	recorderStopBtnOnClicked();

	return 0;
}


void recorderPlayingBtnOnClicked()
{
	char tmpFileName[50] = { 0 };

	if (gRecordNumCount > 0 && gCurrentRecordIndex < MAX_RECORDER_NUM)
	{
		gRecorderLastTimeTick = SDL_GetTicks();		//������ʱ��ǰҪ�Ȼ�ȡһ�ε�ǰʱ���Ա�Ա�

		if (getRecorderRecordStatus(gCurrentRecordIndex) == RECORDER_RECORD_UNREAD)
		{
			//TODO:���ô洢Ϊ�Ѷ�
			setRecorderRecordStatus(gCurrentRecordIndex, RECORDER_RECORD_READ);
			storage_set_jrlyrecord_flag(gCurrentRecordIndex, false);			
			sys_sync_hint_state_ext(SYS_HINT_FAMILY);
		}
		media_set_ring_volume(gRecorderPlayVol);	//TODO:֪ͨ�߼���������������������

		if (gRecorderTimeCount > 0)
		{
			//TODO:��������¼������������ʱ��ʧ�������RadioBox������
		}
		else
		{
			//TODO:��ͷ��ʼ����¼��������ͷ��ʼ��ʱ��ʧ�������RadioBox������
			get_jrlyrecord_path(tmpFileName, &(gRecorderList->JrlyInfo[gCurrentRecordIndex].Time));
			sys_start_play_audio(SYS_MEDIA_MUSIC, tmpFileName, false, storage_get_ringvolume(), recorderPlayingCallback, recorderPlayingStopCallback);
		}

		setRecorderAudioBtnStatus(RECORDER_STATUS_PLAYING);
		setRecorderRadioBoxIsEnable(false);
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
		//TODO:����ֹͣ
		sys_stop_play_audio(SYS_MEDIA_MUSIC);
	}
	else if (gRecorderStatus == RECORDER_STATUS_RECORDING)
	{
		//TODO:¼��ֹͣ
		sys_stop_family_record();
		storage_add_jrlyrecord(gNewRecorderTime);
		sys_sync_hint_state_ext(SYS_HINT_FAMILY);
		recorderLayerInit();

		//if (gRecorderList != NULL)
		//{
		//	free(gRecorderList);
		//	gRecorderList = NULL;
		//}
		//storage_get_jrlyrecord(&gRecorderList);
		//gRecordNumCount = gRecorderList->Count;

		gNewRecordIndex = 0;
		setRecordRadioBoxStatus(gNewRecordIndex, true);
	}
	gRecorderTimeCount = 0;
	setRecorderDuration("00:00");
	setRecorderAudioBtnStatus(RECORDER_STATUS_STOP);
	setRecorderRadioBoxIsEnable(true);
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


void setRecorderRadioBoxIsEnable(bool status)
{
	char tmpStr[50] = { 0 };
	uint8_t i = 0;

	for (i = 0; i < MAX_RECORDER_NUM; i++)
	{
		sprintf(tmpStr, "%s%d", "recorderRecordRadioBox", i);
		recorderRecordRadioBox = ituSceneFindWidget(&theScene, tmpStr);
		assert(recorderRecordRadioBox);
		
		if (status)	
			ituWidgetEnable(recorderRecordRadioBox);
		else
			ituWidgetDisable(recorderRecordRadioBox);
	}

}


void setRecordRadioBoxStatus(uint8_t index, bool status)
{
	char tmpStr[50] = { 0 };
	char tmpTime[50] = { 0 };

	sprintf(tmpStr, "%s%d", "recorderRecordRadioBox", index);
	recorderRecordRadioBox = ituSceneFindWidget(&theScene, tmpStr);
	assert(recorderRecordRadioBox);

	if (status == true)
	{
		gCurrentRecordIndex = index;
		//TODO:��ȡ�洢���¼��¼��ʱ�䣡��������
		zoneDateTimeToString(gRecorderList->JrlyInfo[index].Time, tmpTime);
		setRecorderRecordCreateTime(tmpTime);
		setRecorderAudioBtnStatus(RECORDER_STATUS_STOP);
	}

	ituRadioBoxSetChecked(recorderRecordRadioBox, status);
}


void recorderReturnBtnOnClicked()
{
	printf("recorderReturnBtnOnClicked");

	if (gRecorderStatus != RECORDER_STATUS_STOP && gRecorderStatus != RECORDER_STATUS_NULL)
	{
		return;
	}

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

	if (gCurrentRecordIndex >= MAX_RECORDER_NUM)
	{
		return;
	}
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
	//TODO:������Է��룡

	ituTextSetString(recorderTipsText, get_str(SID_Bj_Query_Del_Rec_One));

	ituWidgetDisable(recorderBackground);
	ituWidgetSetVisible(recorderTipsTransparencyBackground, true);
}


void recorderVoiceBtnOnClicked()
{
	printf("recorderVoiceBtnOnClicked");

	//TODO:��ȡ��ǰϵͳ����ֵ��
	if (gRecorderPlayVol != 0)
	{
		gRecorderPlayVol = 0;
		setRecorderPlayVol(gRecorderPlayVol);
	}
	else
	{
		gRecorderPlayVol = JRLY_VOLUME;
		setRecorderPlayVol(gRecorderPlayVol);
	}
	media_set_ring_volume(gRecorderPlayVol);	//TODO:֪ͨ�߼���������������������
}


bool recorderMsgBoxBtnOnClicked(ITUWidget* widget, char* param)
{
	uint8_t tmpId = atoi(param);

	switch (tmpId)
	{
	case RECORDER_MSG_BTN_COMFIRM:
		//TODO:ɾ������������Ϣ��ˢ�´洢��ˢ�½��棡����������
		if (gCurrentRecordIndex < MAX_RECORDER_NUM)
		{
			storage_del_jrlyrecord(gCurrentRecordIndex);
			gCurrentRecordIndex = MAX_RECORDER_NUM;
			sys_sync_hint_state_ext(SYS_HINT_FAMILY);
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