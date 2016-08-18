/*********************************************************
Copyright (C), 2015-2020, Aurine
File name:	layer_mainlayer.c
Author:		WuZ
Version:   	1.0
Date:		2016-06-17
Description: �������������
History:
1. Date:
Author:
Modification:
2. ...
*********************************************************/

#include "layer_mainmenu.h"

static ITUText*		mainDigitalClockWeekText;
static ITUSprite*	mainIPSprite;
static ITUIcon*		mainNetStatusOffIcon;
static ITUSprite*	mainNetStatusOnSprite;
static ITUText*		mainDeviceNoText;
static ITUButton*	mainSOSOnButton;
static ITUButton*	mainSOSOffButton;
static ITUIcon*		page1NoDisturbOnIcon;
static ITUText*		page1NoDisturbOnText;
static ITUIcon*		page1NoDisturbOffIcon;
static ITUText*		page1NoDisturbOffText;

static ITUIcon*		page1RecorderNumIcon;
static ITUText*		page1RecorderNumText;
static ITUText*		page1RecorderText;
static ITUIcon*		page1RecorderIcon;
static ITUIcon*		page1RecorderMiniIcon;
static ITUText*		page1RecorderScrollTittleText;
static ITUText*		page1RecorderScrollTimeText;

static ITUIcon*		page0SecurityNumIcon;
static ITUText*		page0SecurityNumText;
static ITUSprite*   page0SecuritySprite;
static ITUText*		page0SecurityScrollAlarmTypeText;
static ITUText*		page0SecurityScrollTriggerText;
static ITUText*		page0SecurityScrollTimeText;
static ITUText*		page0SecurityText;
static ITUIcon*		page0SecurityMiniIcon;
static ITUIcon*		page0SecurityIcon;

static ITUIcon*		page0PhotoMsgNumIcon;
static ITUText*		page0PhotoMsgNumText;
static ITUText*		page0PhotoMsgScrollTimeText;
static ITUIcon*		page0PhotoMsgMiniIcon;
static ITUIcon*		page0PhotoMsgIcon;
static ITUText*		page0PhotoMsgText;

static ITUIcon*		page0InformationNumIcon;
static ITUText*		page0InformationNumText;
static ITUText*		page0InformationScrollThemeText;
static ITUText*		page0InformationScrollContentText0;
static ITUText*		page0InformationScrollContentText1;
static ITUIcon*		page0InformationMiniIcon;
static ITUIcon*		page0InformationIcon;
static ITUText*		page0InformationText;

static ITUIcon*		page0MissedCallNumIcon;
static ITUText*		page0MissedCallNumText;
static ITUText*		page0IntercomScrollFromText;
static ITUText*		page0IntercomScrollTimeText;
static ITUIcon*		page0IntercomMiniIcon;
static ITUText*		page0IntercomText;
static ITUIcon*		page0IntercomIcon;


static ITUWidget*	page1RecorderScrollTextContainer;
static ITUWidget*	page0PhotoMsgScrollTextContainer;
static ITUWidget*	page0IntercomScrollTextContainer;
static ITUWidget*	page0SecurityScrollTextContainer;
static ITUWidget*	page0InformationScrollTextContainer;



static uint32_t	gMainLayerLastTimeTick;	//������¼��ʱ���ϸ�ʱ�̵�ʱ��

static bool		gIsScrollingRecorder;
static uint8_t	gRecorderTextIndex;
static bool		gIsScrollingInformation;
static uint8_t  gInformationTextIndex;
static bool		gIsScrollingPhotoMsg;
static uint8_t  gPhotoMsgTextIndex;
static bool		gIsScrollingSecurity;
static uint8_t  gSecurityTextIndex;
static bool		gIsScrollingMissedCall;
static uint8_t  gMissedCallTextIndex;

static uint8_t	gSOSBtnLongPressCount;
static bool		gSOSBtnIsPress;
static bool		gSOSIsAlarm;
static uint8_t  gScrollTimeCount;

static PJRLYLIST_INFO	gRecorderData = NULL;		//������������


bool mainLayerOnEnter(ITUWidget* widget, char* param)
{
	uint8_t tmpSize = get_size(MSG_MANAGE_PATH);
	printf("444444444444444444444 = %d", tmpSize);

	//�ڽ����������ʱ����Ҫ���Ķ����������ʼ��ͼ�꣬��ȡ״̬�ȣ���������
	mainLayerCornerNumReload();
	mainLayerScrollDataReload();

	gSOSBtnIsPress = false;
	gSOSIsAlarm = false;
	gMainLayerLastTimeTick = SDL_GetTicks();		//������ʱ��ǰҪ�Ȼ�ȡһ�ε�ǰʱ���Ա�Ա�

	return true;
}


bool mainLayerTimeoutOnTimer(ITUWidget* widget, char* param)
{
	char buf[10] = { 0 };

	uint32_t duration;
	uint32_t curtime = SDL_GetTicks();

	uint8_t tmp = 0;

	if (curtime >= gMainLayerLastTimeTick)
	{
		duration = curtime - gMainLayerLastTimeTick;
	}
	else
	{
		duration = 0xFFFFFFFF - gMainLayerLastTimeTick + curtime;
	}


	if (duration >= MAIN_MS_PER_SCROLL)		//ʱ������Ժ���Ϊ��λ
	{
		gMainLayerLastTimeTick = curtime;
		//TODO:��ȡ�洢��ȫ�ֱ����Աȣ���һ��ʱ�������ø���״ֵ̬
		gScrollTimeCount++;

		setUnreadRecorderScroll();			//δ���������Թ�������
		setUnreadPhotoMsgScroll();			//������Ӱ���Թ�������
		setUnreadInformationScroll();		//������Ϣ��������
		setUnreadMissedCallScroll();		//����δ�������������
		setUnsolvedSecurityAlarmScroll();	//���ñ�����������

		setNetworkStatus(getNetworkStatus());		//��������״̬
		setDeviceNo(getDeviceNo());					//�����豸���
		setDisturbStatus(getDisturbStatus());		//���������״̬
		setIpIconStatus(getIpIconStatus());			//����IPģ���������

		if (gScrollTimeCount > MAIN_SCROLLTEXT_SHOW_TIME)
		{
			gScrollTimeCount = 0;
		}

		if (gSOSBtnIsPress == true)
		{
			gSOSBtnLongPressCount++;
			if (gSOSBtnLongPressCount >= 3)
			{
				setSOSBtnType(true);
				//TODO:�߼�����SOS���������
				if (gSOSIsAlarm == false)
				{
					gSOSIsAlarm = true;
					printf("11111111111111111 gSOSIsAlarm!!! ");
					sos_alarm_report();
				}
			}
		}
		else
		{
			gSOSIsAlarm = false;
			setSOSBtnType(false);
			gSOSBtnLongPressCount = 0;
		}
	}

	return true;
}


bool mainSOSBtnOnPress(ITUWidget* widget, char* param)
{
	if (atoi(param) == 0)
	{
		gSOSBtnIsPress = true;
		gSOSBtnLongPressCount = 0;
	}
	else
	{
		gSOSBtnIsPress = false;
	}

	return true;
}


void zoneDateTimeToString(DATE_TIME time, char* tmpStr)
{
	sprintf(tmpStr, "%d-%d-%d %d:%d:%d", time.year, time.month, time.day, time.hour, time.min, time.sec);
}


void mainLayerCornerNumReload()
{
	//TODO:�û��ڽ���Enterʱ�����벻��Ҫʵʱȥ��ȡ�����ݣ���������
	setUnreadRecorderNum((uint8_t)getUnreadRecorderNum());				//���ü�����������
	setUnreadPhotoMsgNum((uint8_t)getUnreadPhotoMsgNum());				//������Ӱ��������
	setUnreadInformationNum((uint8_t)getUnreadInformationNum());		//������Ϣ����
	setSecurityStatus((MAIN_SECURITY_STATUS_e)getSecurityStatus());		//���ð���״̬
	setUnsolvedSecurityAlarmNum((uint8_t)getUnsolvedSecurityAlarmNum());//���ð���������
	setUnreadMissedCallNum((uint8_t)getUnreadMissedCallNun());			//����δ��������
}


void mainLayerScrollDataReload()
{
	loadUnreadRecorderData();				//����δ��������������
	loadUnreadInformationData();			//����δ����Ϣ����
	loadUnreadMissedCallData();				//����δ�ӵ绰��Ϣ
	loadUnreaPhotoMsgData();				//����δ����Ӱ��������
	loadUnsolvedSecurityAlarmData();		//����Ϊ����ı�������
}


bool getIpIconStatus()
{
	return (bool)get_ipmodule_bindstate();		//��ȡ�Ƿ�����IPģ��
}


void setIpIconStatus(bool status)
{
	if (!mainIPSprite)
	{
		mainIPSprite = ituSceneFindWidget(&theScene, "mainIPSprite");
		assert(mainIPSprite);
	}
	ituSpriteGoto(mainIPSprite, status);
}


bool getNetworkStatus()
{
	return (bool)NetworkIsReady();
}


void setNetworkStatus(bool status)
{
	if (!mainNetStatusOffIcon)
	{
		mainNetStatusOffIcon = ituSceneFindWidget(&theScene, "mainNetStatusOffIcon");
		assert(mainNetStatusOffIcon);
	}
	if (!mainNetStatusOnSprite)
	{
		mainNetStatusOnSprite = ituSceneFindWidget(&theScene, "mainNetStatusOnSprite");
		assert(mainNetStatusOnSprite);
	}
	if (status)
	{
		ituWidgetSetVisible(mainNetStatusOffIcon, false);
		ituWidgetSetVisible(mainNetStatusOnSprite, true);
	}
	else
	{
		ituWidgetSetVisible(mainNetStatusOnSprite, false);
		ituWidgetSetVisible(mainNetStatusOffIcon, true);
	}
}


char* getDeviceNo()
{
	char* tmpStr = (char*)malloc(50);
	PFULL_DEVICE_NO tmpDev = storage_get_devparam();
	sprintf(tmpStr, "%s%s", "NO:", tmpDev->DeviceNoStr);
	//TODO:���ص�ַ��Ҫ�������������������malloc������
	return tmpStr;
}


void setDeviceNo(char* deviceno)
{
	if (!mainDeviceNoText)
	{
		mainDeviceNoText = ituSceneFindWidget(&theScene, "mainDeviceNoText");
		assert(mainDeviceNoText);
	}
	ituTextSetString(mainDeviceNoText, deviceno);

	free(deviceno);
}


void setSOSBtnType(bool status)
{	
	if (!mainSOSOffButton)
	{
		mainSOSOffButton = ituSceneFindWidget(&theScene, "mainSOSOffButton");
		assert(mainSOSOffButton);
	}
	if (!mainSOSOnButton)
	{
		mainSOSOnButton = ituSceneFindWidget(&theScene, "mainSOSOnButton");
		assert(mainSOSOnButton);
	}
	if (status)
	{
		ituWidgetSetVisible(mainSOSOffButton, false);
		ituWidgetSetVisible(mainSOSOnButton, true);
	}
	else
	{
		ituWidgetSetVisible(mainSOSOnButton, false);
		ituWidgetSetVisible(mainSOSOffButton, true);
	}
}


bool getDisturbStatus()
{
	return (bool)storage_get_noface();	//�洢��ȡ�Ƿ����������
}


void setDisturbStatus(bool status)
{
	if (!page1NoDisturbOnIcon)
	{
		page1NoDisturbOnIcon = ituSceneFindWidget(&theScene, "page1NoDisturbOnIcon");
		assert(page1NoDisturbOnIcon);
	}
	if (!page1NoDisturbOnText)
	{
		page1NoDisturbOnText = ituSceneFindWidget(&theScene, "page1NoDisturbOnText");
		assert(page1NoDisturbOnText);
	}
	if (!page1NoDisturbOffIcon)
	{
		page1NoDisturbOffIcon = ituSceneFindWidget(&theScene, "page1NoDisturbOffIcon");
		assert(page1NoDisturbOffIcon);
	}
	if (!page1NoDisturbOffText)
	{
		page1NoDisturbOffText = ituSceneFindWidget(&theScene, "page1NoDisturbOffText");
		assert(page1NoDisturbOffText);
	}

	if (status)
	{
		ituWidgetSetVisible(page1NoDisturbOffIcon, false);
		ituWidgetSetVisible(page1NoDisturbOffText, false);
		ituWidgetSetVisible(page1NoDisturbOnIcon, true);
		ituWidgetSetVisible(page1NoDisturbOnText, true);
	}
	else
	{
		ituWidgetSetVisible(page1NoDisturbOnIcon, false);
		ituWidgetSetVisible(page1NoDisturbOnText, false);
		ituWidgetSetVisible(page1NoDisturbOffIcon, true);
		ituWidgetSetVisible(page1NoDisturbOffText, true);
	}
}

bool mainDistrubStatusOnChange(ITUWidget* widget, char* param)
{
	if (getDisturbStatus())
	{
		setDisturbStatus(false);
	}
	else
	{
		setDisturbStatus(true);
	}

	return true;
}


uint8_t	getUnreadRecorderNum()
{
	return storage_get_jrlyrecord_flag();
}


void setUnreadRecorderNum(uint8_t num)
{
	char numstr[4] = { 0 };

	if (!page1RecorderNumText)
	{
		page1RecorderNumText = ituSceneFindWidget(&theScene, "page1RecorderNumText");
		assert(page1RecorderNumText);
	}
	if (!page1RecorderNumIcon)
	{
		page1RecorderNumIcon = ituSceneFindWidget(&theScene, "page1RecorderNumIcon");
		assert(page1RecorderNumIcon);
	}
	if (!page1RecorderText)
	{
		page1RecorderText = ituSceneFindWidget(&theScene, "page1RecorderText");
		assert(page1RecorderText);
	}
	if (!page1RecorderIcon)
	{
		page1RecorderIcon = ituSceneFindWidget(&theScene, "page1RecorderIcon");
		assert(page1RecorderIcon);
	}
	if (!page1RecorderMiniIcon)
	{
		page1RecorderMiniIcon = ituSceneFindWidget(&theScene, "page1RecorderMiniIcon");
		assert(page1RecorderMiniIcon);
	}
	if (!page1RecorderScrollTextContainer)
	{
		page1RecorderScrollTextContainer = ituSceneFindWidget(&theScene, "page1RecorderScrollTextContainer");
		assert(page1RecorderScrollTextContainer);
	}
	
	if (num > 0)
	{
		gIsScrollingRecorder = true;
		gRecorderTextIndex = -1;
		gScrollTimeCount = 0;

		if (num > MAIN_MAX_RECORDER_NUM)
		{
			sprintf(numstr, "%d", MAIN_MAX_RECORDER_NUM);
			printf("Recorder Num Is Too Much!!!!");
		}
		else
		{
			sprintf(numstr, "%d", num);
		}

		ituTextSetString(page1RecorderNumText, numstr);
		ituWidgetSetVisible(page1RecorderIcon, false);
		ituWidgetSetVisible(page1RecorderText, false);
		ituWidgetSetVisible(page1RecorderNumText, true);
		ituWidgetSetVisible(page1RecorderNumIcon, true);
		ituWidgetSetVisible(page1RecorderMiniIcon, true);
	}
	else
	{
		gIsScrollingRecorder = false;
		gRecorderTextIndex = -1;
		gScrollTimeCount = 0;

		ituWidgetSetVisible(page1RecorderScrollTextContainer, false);
		ituWidgetSetVisible(page1RecorderMiniIcon, false);
		ituWidgetSetVisible(page1RecorderNumIcon, false);
		ituWidgetSetVisible(page1RecorderNumText, false);
		ituWidgetSetVisible(page1RecorderIcon, true);
		ituWidgetSetVisible(page1RecorderText, true);
	}
}


void loadUnreadRecorderData()
{
	PJRLYLIST_INFO tmpListInfo = NULL;

	//����ҳ��ʱ���ȡһ�δ洢���ɣ�Ȼ�������ֱ�ӵ������ָ��
	if (gRecorderData == NULL)
	{
		gRecorderData = (PJRLYLIST_INFO)malloc(sizeof(JRLYLIST_INFO));
	}
	printf("2222222222222222  = %d", sizeof(JRLYLIST_INFO));

	storage_get_jrlyrecord(&tmpListInfo);
	memcpy(gRecorderData, tmpListInfo, sizeof(JRLYLIST_INFO));

	free(tmpListInfo);

	printf("1111111111111  = %d", gRecorderData->Count);
}


void setUnreadRecorderText(uint8_t index)
{
	char tmpStr[50] = { 0 };

	if (!page1RecorderScrollTimeText)
	{
		page1RecorderScrollTimeText = ituSceneFindWidget(&theScene, "page1RecorderScrollTimeText");
		assert(page1RecorderScrollTimeText);
	}
	//TODO:��ȡ�洢�����������ݣ���
	sprintf(tmpStr, "%s%d", "2016-08-03 08:11:1", index);
	ituTextSetString(page1RecorderScrollTimeText, tmpStr);
}


void setUnreadRecorderScroll()
{
	if (gIsScrollingRecorder == true)
	{
		if (ituWidgetIsVisible(page1RecorderScrollTextContainer) == false)
		{
			if (gRecorderTextIndex >= getUnreadRecorderNum())
			{
				gRecorderTextIndex = 0;
			}
			else
			{
				gRecorderTextIndex++;
			}
			setUnreadRecorderText(gRecorderTextIndex);
			ituWidgetShow(page1RecorderScrollTextContainer, ITU_EFFECT_SCROLL_UP, MAIN_SCROLL_STEP_COUNT);
		}
		else
		{
			if (gScrollTimeCount == MAIN_SCROLLTEXT_SHOW_TIME)
			{
				ituWidgetHide(page1RecorderScrollTextContainer, ITU_EFFECT_SCROLL_UP, MAIN_SCROLL_STEP_COUNT);
			}
		}
	}
}


uint8_t getUnreadPhotoMsgNum()
{
	return storage_get_lylyrecord_flag();
}


void setUnreadPhotoMsgNum(uint8_t num)
{
	char numstr[4] = { 0 };

	if (!page0PhotoMsgNumIcon)
	{
		page0PhotoMsgNumIcon = ituSceneFindWidget(&theScene, "page0PhotoMsgNumIcon");
		assert(page0PhotoMsgNumIcon);
	}
	if (!page0PhotoMsgNumText)
	{
		page0PhotoMsgNumText = ituSceneFindWidget(&theScene, "page0PhotoMsgNumText");
		assert(page0PhotoMsgNumText);
	}

	if (!page0PhotoMsgMiniIcon)
	{
		page0PhotoMsgMiniIcon = ituSceneFindWidget(&theScene, "page0PhotoMsgMiniIcon");
		assert(page0PhotoMsgMiniIcon);
	}
	if (!page0PhotoMsgIcon)
	{
		page0PhotoMsgIcon = ituSceneFindWidget(&theScene, "page0PhotoMsgIcon");
		assert(page0PhotoMsgIcon);
	}
	if (!page0PhotoMsgText)
	{
		page0PhotoMsgText = ituSceneFindWidget(&theScene, "page0PhotoMsgText");
		assert(page0PhotoMsgText);
	}
	if (!page0PhotoMsgScrollTextContainer)
	{
		page0PhotoMsgScrollTextContainer = ituSceneFindWidget(&theScene, "page0PhotoMsgScrollTextContainer");
		assert(page0PhotoMsgScrollTextContainer);
	}

	if (num > 0)
	{
		gIsScrollingPhotoMsg = true;
		gPhotoMsgTextIndex = -1;
		gScrollTimeCount = 0;

		if (num > MAIN_MAX_MSG_NUM)
		{
			sprintf(numstr, "%d", MAIN_MAX_MSG_NUM);
			printf("MSG Num Is Too Much!!!!");
		}
		else
		{
			sprintf(numstr, "%d", num);
		}

		
		ituTextSetString(page0PhotoMsgNumText, numstr);
		ituWidgetSetVisible(page0PhotoMsgText, false);
		ituWidgetSetVisible(page0PhotoMsgIcon, false);
		ituWidgetSetVisible(page0PhotoMsgMiniIcon, true);
		ituWidgetSetVisible(page0PhotoMsgNumText, true);
		ituWidgetSetVisible(page0PhotoMsgNumIcon, true);
	}
	else
	{
		gIsScrollingPhotoMsg = false;
		gPhotoMsgTextIndex = -1;
		gScrollTimeCount = 0;

		ituWidgetSetVisible(page0PhotoMsgScrollTextContainer, false);
		ituWidgetSetVisible(page0PhotoMsgMiniIcon, false);
		ituWidgetSetVisible(page0PhotoMsgNumIcon, false);
		ituWidgetSetVisible(page0PhotoMsgNumText, false);
		ituWidgetSetVisible(page0PhotoMsgText, true);
		ituWidgetSetVisible(page0PhotoMsgIcon, true);
	}
}


void loadUnreaPhotoMsgData()
{	
	//����ҳ��ʱ���ȡһ�δ洢���ɣ�
}


void setUnreadPhotoMsgText(uint8_t index)
{
	char tmpStr[50] = { 0 };

	if (!page0PhotoMsgScrollTimeText)
	{
		page0PhotoMsgScrollTimeText = ituSceneFindWidget(&theScene, "page0PhotoMsgScrollTimeText");
		assert(page0PhotoMsgScrollTimeText);
	}
	//TODO:��ȡ�洢�����������ݣ���
	sprintf(tmpStr, "%s%d", "2016-08-03 09:22:1", index);
	ituTextSetString(page0PhotoMsgScrollTimeText, tmpStr);
}


void setUnreadPhotoMsgScroll()
{
	if (gIsScrollingPhotoMsg == true)
	{
		if (ituWidgetIsVisible(page0PhotoMsgScrollTextContainer) == false)
		{
			if (gPhotoMsgTextIndex >= getUnreadPhotoMsgNum())
			{
				gPhotoMsgTextIndex = 0;
			}
			else
			{
				gPhotoMsgTextIndex++;
			}
			setUnreadPhotoMsgText(gPhotoMsgTextIndex);
			ituWidgetShow(page0PhotoMsgScrollTextContainer, ITU_EFFECT_SCROLL_UP, MAIN_SCROLL_STEP_COUNT);
		}
		else
		{
			if (gScrollTimeCount == MAIN_SCROLLTEXT_SHOW_TIME)
			{
				ituWidgetHide(page0PhotoMsgScrollTextContainer, ITU_EFFECT_SCROLL_UP, MAIN_SCROLL_STEP_COUNT);
			}
		}
	}
}


uint8_t getUnreadInformationNum()
{
	return (uint8_t)storage_get_msg_state();
}

void setUnreadInformationNum(uint8_t num)
{
	char numstr[4] = { 0 };

	if (!page0InformationNumIcon)
	{
		page0InformationNumIcon = ituSceneFindWidget(&theScene, "page0InformationNumIcon");
		assert(page0InformationNumIcon);
	}
	if (!page0InformationNumText)
	{
		page0InformationNumText = ituSceneFindWidget(&theScene, "page0InformationNumText");
		assert(page0InformationNumText);
	}
	if (!page0InformationScrollTextContainer)
	{
		page0InformationScrollTextContainer = ituSceneFindWidget(&theScene, "page0InformationScrollTextContainer");
		assert(page0InformationScrollTextContainer);
	}
	if (!page0InformationText)
	{
		page0InformationText = ituSceneFindWidget(&theScene, "page0InformationText");
		assert(page0InformationText);
	}
	if (!page0InformationIcon)
	{
		page0InformationIcon = ituSceneFindWidget(&theScene, "page0InformationIcon");
		assert(page0InformationIcon);
	}
	if (!page0InformationMiniIcon)
	{
		page0InformationMiniIcon = ituSceneFindWidget(&theScene, "page0InformationMiniIcon");
		assert(page0InformationMiniIcon);
	}

	if (num > 0)
	{
		if (num > MAX_INFORMATION_NUM)
		{
			sprintf(numstr, "%d", MAX_INFORMATION_NUM);
			printf("Information Num Is Too Much!!!!");
		}
		else
		{
			sprintf(numstr, "%d", num);
		}

		gIsScrollingInformation = true;
		gInformationTextIndex = -1;
		gScrollTimeCount = 0;

		ituTextSetString(page0InformationNumText, numstr);
		ituWidgetSetVisible(page0InformationText, false);
		ituWidgetSetVisible(page0InformationIcon, false);
		ituWidgetSetVisible(page0InformationMiniIcon, true);
		ituWidgetSetVisible(page0InformationNumText, true);
		ituWidgetSetVisible(page0InformationNumIcon, true);
	}
	else
	{
		gIsScrollingInformation = false;
		gInformationTextIndex = -1;
		gScrollTimeCount = 0;

		ituWidgetSetVisible(page0InformationNumIcon, false);
		ituWidgetSetVisible(page0InformationNumText, false);
		ituWidgetSetVisible(page0InformationMiniIcon, false);
		ituWidgetSetVisible(page0InformationScrollTextContainer, false);
		ituWidgetSetVisible(page0InformationText, true);
		ituWidgetSetVisible(page0InformationIcon, true);
	}
}


void loadUnreadInformationData()
{
	//����ҳ��ʱ���ȡһ�δ洢���ɣ�����

}


void setUnreadInformationText(uint8_t index)
{
	char tmpStr[50] = { 0 };

	if (!page0InformationScrollContentText1)
	{
		page0InformationScrollContentText1 = ituSceneFindWidget(&theScene, "page0InformationScrollContentText1");
		assert(page0InformationScrollContentText1);
	}
	if (!page0InformationScrollContentText0)
	{
		page0InformationScrollContentText0 = ituSceneFindWidget(&theScene, "page0InformationScrollContentText0");
		assert(page0InformationScrollContentText0);
	}
	if (!page0InformationScrollThemeText)
	{
		page0InformationScrollThemeText = ituSceneFindWidget(&theScene, "page0InformationScrollThemeText");
		assert(page0InformationScrollThemeText);
	}
	//TODO:��ȡ�洢�����������ݣ���
	sprintf(tmpStr, "%s%d", "Theme", index);
	ituTextSetString(page0InformationScrollThemeText, tmpStr);

	sprintf(tmpStr, "%s%d", "01234567890", index);
	ituTextSetString(page0InformationScrollContentText0, tmpStr);
	sprintf(tmpStr, "%s%d", "01234567890", index);
	ituTextSetString(page0InformationScrollContentText1, tmpStr);

}


void setUnreadInformationScroll()
{
	if (gIsScrollingInformation == true)
	{
		if (ituWidgetIsVisible(page0InformationScrollTextContainer) == false)
		{
			if (gInformationTextIndex >= getUnreadInformationNum())
			{
				gInformationTextIndex = 0;
			}
			else
			{
				gInformationTextIndex++;
			}
			setUnreadInformationText(gInformationTextIndex);
			ituWidgetShow(page0InformationScrollTextContainer, ITU_EFFECT_SCROLL_UP, MAIN_SCROLL_STEP_COUNT);
		}
		else
		{
			if (gScrollTimeCount == MAIN_SCROLLTEXT_SHOW_TIME)
			{
				ituWidgetHide(page0InformationScrollTextContainer, ITU_EFFECT_SCROLL_UP, MAIN_SCROLL_STEP_COUNT);
			}
		}
	}
}


uint8_t getUnreadMissedCallNun()
{
	return storage_get_callrecord_state();
}


void setUnreadMissedCallNum(uint8_t num)
{
	char numstr[4] = { 0 };
	
	if (!page0MissedCallNumIcon)
	{
		page0MissedCallNumIcon = ituSceneFindWidget(&theScene, "page0MissedCallNumIcon");
		assert(page0MissedCallNumIcon);
	}
	if (!page0MissedCallNumText)
	{
		page0MissedCallNumText = ituSceneFindWidget(&theScene, "page0MissedCallNumText");
		assert(page0MissedCallNumText);
	}
	if (!page0IntercomScrollTextContainer)
	{
		page0IntercomScrollTextContainer = ituSceneFindWidget(&theScene, "page0IntercomScrollTextContainer");
		assert(page0IntercomScrollTextContainer);
	}
	if (!page0IntercomIcon)
	{
		page0IntercomIcon = ituSceneFindWidget(&theScene, "page0IntercomIcon");
		assert(page0IntercomIcon);
	}
	if (!page0IntercomMiniIcon)
	{
		page0IntercomMiniIcon = ituSceneFindWidget(&theScene, "page0IntercomMiniIcon");
		assert(page0IntercomMiniIcon);
	}
	if (!page0IntercomText)
	{
		page0IntercomText = ituSceneFindWidget(&theScene, "page0IntercomText");
		assert(page0IntercomText);
	}

	if (num > 0)
	{
		if (num > MAIN_MAX_MISSCALL_NUM)
		{
			sprintf(numstr, "%d", MAX_INFORMATION_NUM);
			printf("MissCall Num Is Too Much!!!!");
		}
		else
		{
			sprintf(numstr, "%d", num);
		}

		gIsScrollingMissedCall = true;
		gMissedCallTextIndex = -1;
		gScrollTimeCount = 0;

		ituTextSetString(page0MissedCallNumText, numstr);
		ituWidgetSetVisible(page0IntercomText, false);
		ituWidgetSetVisible(page0IntercomIcon, false);
		ituWidgetSetVisible(page0IntercomMiniIcon, true);
		ituWidgetSetVisible(page0MissedCallNumText, true);
		ituWidgetSetVisible(page0MissedCallNumIcon, true);
	}
	else
	{
		gIsScrollingMissedCall = false;
		gMissedCallTextIndex = -1;
		gScrollTimeCount = 0;

		ituWidgetSetVisible(page0IntercomScrollTextContainer, false);
		ituWidgetSetVisible(page0IntercomMiniIcon, false);
		ituWidgetSetVisible(page0MissedCallNumIcon, false);
		ituWidgetSetVisible(page0MissedCallNumText, false);
		ituWidgetSetVisible(page0IntercomText, true);
		ituWidgetSetVisible(page0IntercomIcon, true);
	}
}


void loadUnreadMissedCallData()
{
	//����ҳ��ʱ���ȡһ�δ洢���ɣ�����
}


void setUnreadMissedCallText(uint8_t index)
{
	char tmpStr[50] = { 0 };

	if (!page0IntercomScrollFromText)
	{
		page0IntercomScrollFromText = ituSceneFindWidget(&theScene, "page0IntercomScrollFromText");
		assert(page0IntercomScrollFromText);
	}
	if (!page0IntercomScrollTimeText)
	{
		page0IntercomScrollTimeText = ituSceneFindWidget(&theScene, "page0IntercomScrollTimeText");
		assert(page0IntercomScrollTimeText);
	}

	//TODO:��ȡ�洢�����������ݣ���
	sprintf(tmpStr, "%s%d", "Admin", index);
	ituTextSetString(page0IntercomScrollFromText, tmpStr);

	sprintf(tmpStr, "%s%d", "2016-08-03 16:33:1", index);
	ituTextSetString(page0IntercomScrollTimeText, tmpStr);

}


void setUnreadMissedCallScroll()
{
	if (gIsScrollingMissedCall == true)
	{
		if (ituWidgetIsVisible(page0IntercomScrollTextContainer) == false)
		{
			if (gMissedCallTextIndex >= getUnreadMissedCallNun())
			{
				gMissedCallTextIndex = 0;
			}
			else
			{
				gMissedCallTextIndex++;
			}
			setUnreadMissedCallText(gMissedCallTextIndex);
			ituWidgetShow(page0IntercomScrollTextContainer, ITU_EFFECT_SCROLL_UP, MAIN_SCROLL_STEP_COUNT);
		}
		else
		{
			if (gScrollTimeCount == MAIN_SCROLLTEXT_SHOW_TIME)
			{
				ituWidgetHide(page0IntercomScrollTextContainer, ITU_EFFECT_SCROLL_UP, MAIN_SCROLL_STEP_COUNT);
			}
		}
	}
}


uint8_t getUnsolvedSecurityAlarmNum()
{
	uint8_t tmpNum = 0;
	PALARM_TOUCH_INFO_LIST tmpList;
	tmpList = storage_get_afbj_unread_record();
	tmpNum = tmpList->nCount;
	free(tmpList);

	return tmpNum;
}


void setUnsolvedSecurityAlarmNum(uint8_t num)
{
	char numstr[4] = { 0 };
	
	if (!page0SecurityNumIcon)
	{
		page0SecurityNumIcon = ituSceneFindWidget(&theScene, "page0SecurityNumIcon");
		assert(page0SecurityNumIcon);
	}
	if (!page0SecurityNumText)
	{
		page0SecurityNumText = ituSceneFindWidget(&theScene, "page0SecurityNumText");
		assert(page0SecurityNumText);
	}
	if (!page0SecurityScrollTextContainer)
	{
		page0SecurityScrollTextContainer = ituSceneFindWidget(&theScene, "page0SecurityScrollTextContainer");
		assert(page0SecurityScrollTextContainer);
	}
	if (!page0SecurityIcon)
	{
		page0SecurityIcon = ituSceneFindWidget(&theScene, "page0SecurityIcon");
		assert(page0SecurityIcon);
	}
	if (!page0SecurityMiniIcon)
	{
		page0SecurityMiniIcon = ituSceneFindWidget(&theScene, "page0SecurityMiniIcon");
		assert(page0SecurityMiniIcon);
	}
	if (!page0SecurityText)
	{
		page0SecurityText = ituSceneFindWidget(&theScene, "page0SecurityText");
		assert(page0SecurityText);
	}
	if (!page0SecuritySprite)
	{
		page0SecuritySprite = ituSceneFindWidget(&theScene, "page0SecuritySprite");
		assert(page0SecuritySprite);
	}
	
	if (num > 0)
	{
		if (num > MAIN_MAX_SECURITY_NUM)
		{
			sprintf(numstr, "%d", MAIN_MAX_SECURITY_NUM);
			printf("Security Num Is Too Much!!!!");
		}
		else
		{
			sprintf(numstr, "%d", num);
		}
		gIsScrollingSecurity = true;
		gSecurityTextIndex = -1;
		gScrollTimeCount = 0;

		ituTextSetString(page0SecurityNumText, numstr);
		ituWidgetSetVisible(page0SecurityIcon, false);
		ituWidgetSetVisible(page0SecurityText, false);
		ituWidgetSetVisible(page0SecuritySprite, false);
		ituWidgetSetVisible(page0SecurityNumText, true);
		ituWidgetSetVisible(page0SecurityNumIcon, true);
		ituWidgetSetVisible(page0SecurityMiniIcon, true);

	}
	else
	{
		gIsScrollingSecurity = false;
		gSecurityTextIndex = -1;
		gScrollTimeCount = 0;

		ituWidgetSetVisible(page0SecurityScrollTextContainer, false);
		ituWidgetSetVisible(page0SecurityNumIcon, false);
		ituWidgetSetVisible(page0SecurityNumText, false);
		ituWidgetSetVisible(page0SecurityMiniIcon, false);
		ituWidgetSetVisible(page0SecurityIcon, true);
		ituWidgetSetVisible(page0SecurityText, true);
	}
}


void loadUnsolvedSecurityAlarmData()
{
	//����ҳ��ʱ���ȡһ�δ洢���ɣ�
}


void setUnsolvedSecurityAlarmText(uint8_t index)
{
	char tmpStr[50] = { 0 };

	if (!page0SecurityScrollAlarmTypeText)
	{
		page0SecurityScrollAlarmTypeText = ituSceneFindWidget(&theScene, "page0SecurityScrollAlarmTypeText");
		assert(page0SecurityScrollAlarmTypeText);
	}
	if (!page0SecurityScrollTriggerText)
	{
		page0SecurityScrollTriggerText = ituSceneFindWidget(&theScene, "page0SecurityScrollTriggerText");
		assert(page0SecurityScrollTriggerText);
	}
	if (!page0SecurityScrollTimeText)
	{
		page0SecurityScrollTimeText = ituSceneFindWidget(&theScene, "page0SecurityScrollTimeText");
		assert(page0SecurityScrollTimeText);
	}

	//TODO:��ȡ�洢�����������ݣ���
	sprintf(tmpStr, "%s%d", "GAS", index);
	ituTextSetString(page0SecurityScrollAlarmTypeText, tmpStr);

	sprintf(tmpStr, "%s%d", "Triger", index);
	ituTextSetString(page0SecurityScrollTriggerText, tmpStr);

	sprintf(tmpStr, "%s%d", "2016-08-03 17:03:1", index);
	ituTextSetString(page0SecurityScrollTimeText, tmpStr);
}


void setUnsolvedSecurityAlarmScroll()
{
	if (gIsScrollingSecurity == true)
	{
		if (ituWidgetIsVisible(page0SecurityScrollTextContainer) == false)
		{
			if (gSecurityTextIndex >= getUnsolvedSecurityAlarmNum())
			{
				gSecurityTextIndex = 0;
			}
			else
			{
				gSecurityTextIndex++;
			}
			setUnsolvedSecurityAlarmText(gSecurityTextIndex);
			ituWidgetShow(page0SecurityScrollTextContainer, ITU_EFFECT_SCROLL_UP, MAIN_SCROLL_STEP_COUNT);
		}
		else
		{
			if (gScrollTimeCount == MAIN_SCROLLTEXT_SHOW_TIME)
			{
				ituWidgetHide(page0SecurityScrollTextContainer, ITU_EFFECT_SCROLL_UP, MAIN_SCROLL_STEP_COUNT);
			}
		}
	}
}

MAIN_SECURITY_STATUS_e	getSecurityStatus()
{
	if (storage_get_defend_state() == SET_DEFEND)
	{
		//���������������
		return (MAIN_SECURITY_STATUS_e)1;
	}
	else if (storage_get_defend_state() == PART_DEFEND)
	{
		//�ַ���������ҹ�䣩
		return (MAIN_SECURITY_STATUS_e)0;
	}
	else
	{
		//��������������ͼ�꣩
		return (MAIN_SECURITY_STATUS_e)2;
	}
}


void setSecurityStatus(MAIN_SECURITY_STATUS_e status)
{
	if (!page0SecuritySprite)
	{
		page0SecuritySprite = ituSceneFindWidget(&theScene, "page0SecuritySprite");
		assert(page0SecuritySprite);
	}
	switch (status)
	{
	case MAIN_SECURITY_STATUS_NIGHT:
	case MAIN_SECURITY_STATUS_OUTSIDE:
		ituWidgetSetVisible(page0SecuritySprite, true);
		ituSpriteGoto(page0SecuritySprite, (uint8_t)status);
		break;

	default:
		ituWidgetSetVisible(page0SecuritySprite, false);
		break;
	}
}


void mainMenuLayerReset()
{
	//TODO:�ϵ�ʱ��Ѿ�̬����ȫ����ʼ��ΪNULL
	mainDigitalClockWeekText = NULL;
	mainIPSprite = NULL;

	mainNetStatusOffIcon  = NULL;
	mainNetStatusOnSprite = NULL;

	mainDeviceNoText = NULL;

	mainSOSOnButton  = NULL;
	mainSOSOffButton = NULL;

	page1NoDisturbOnIcon  = NULL;
	page1NoDisturbOnText  = NULL;
	page1NoDisturbOffIcon = NULL;
	page1NoDisturbOffText = NULL;

	page1RecorderNumIcon = NULL;
	page1RecorderNumText = NULL;

	page0SecurityNumIcon = NULL;
	page0SecurityNumText = NULL;
	page0SecuritySprite  = NULL;

	page0PhotoMsgNumIcon = NULL;
	page0PhotoMsgNumText = NULL;

	page0InformationNumIcon = NULL;
	page0InformationNumText = NULL;
}


