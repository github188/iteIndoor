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
static ITUIcon*		page0PhotoMsgNumIcon;
static ITUText*		page0PhotoMsgNumText;
static ITUIcon*		page0InformationNumIcon;
static ITUText*		page0InformationNumText;
static ITUIcon*		page0MissedCallNumIcon;
static ITUText*		page0MissedCallNumText;

static ITUWidget*	page1RecorderScrollTextContainer;


static bool		gDistrubStstus;
static uint32_t	gMainLayerLastTimeTick;	//������¼��ʱ���ϸ�ʱ�̵�ʱ��


static bool		gIsScrollingRecorder;
static uint8_t  gRecorderTextIndex;
static bool		gIsScrollinginformation;
static uint8_t  gInformationTextIndex;
static bool		gIsScrollingPhotoMsg;
static uint8_t  gPhotoMsgTextIndex;
static bool		gIsScrollingSecurity;
static uint8_t  gSecurityTextIndex;
static bool		gIsScrollingMissedCall;
static uint8_t  gMissedCallTextIndex;

static uint8_t  gScrollTimeCount;


bool mainLayerOnEnter(ITUWidget* widget, char* param)
{
	//�ڽ����������ʱ����Ҫ���Ķ����������ʼ��ͼ�꣬��ȡ״̬�ȣ���������
	mainLayerCornerNumReload();

	if (!page1RecorderScrollTextContainer)
	{
		page1RecorderScrollTextContainer = ituSceneFindWidget(&theScene, "page1RecorderScrollTextContainer");
		assert(page1RecorderScrollTextContainer);
	}
	ituWidgetHide(page1RecorderScrollTextContainer, ITU_EFFECT_SCROLL_UP, 20);
	
	setNetworkStatus(TRUE);				//��������״̬
	setDeviceNo((char*)"NO:010101011");	//�����豸���
	setSOSBtnType(TRUE);				//����SOS����״̬
	setDisturbStatus(gDistrubStstus);	//���������״̬
	setUnreadRecorderNum((uint8_t)5);	    //���ü�����������
	setUnreadPhotoMsgNum((uint8_t)15);		//������Ӱ��������
	setUnreadInformationNum((uint8_t)18);		//������Ϣ����
	setSecurityStatus((uint8_t)1);			//���ð���״̬
	setUnsolvedSecurityAlarmNum((uint8_t)11);	//���ð���������
	setUnreadMissCallNum((uint8_t)11);		//����δ��������
	setIpIconStatus(TRUE);					//����IPģ���������

	gMainLayerLastTimeTick = SDL_GetTicks();		//������ʱ��ǰҪ�Ȼ�ȡһ�ε�ǰʱ���Ա�Ա�

	return TRUE;
}


bool mainLayerTimeoutOnTimer(ITUWidget* widget, char* param)
{
	char buf[10] = { 0 };

	uint32_t duration;
	uint32_t curtime = SDL_GetTicks();

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

		if (gScrollTimeCount > MAIN_SCROLLTEXT_SHOW_TIME)
		{
			gScrollTimeCount = 0;
		}
	}

	return true;
}


void mainLayerCornerNumReload()
{
	//TODO:�û��ڽ���Enterʱ�����벻��Ҫʵʱȥ��ȡ�����ݣ���������
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
		ituWidgetSetVisible(mainNetStatusOffIcon, FALSE);
		ituWidgetSetVisible(mainNetStatusOnSprite, TRUE);
	}
	else
	{
		ituWidgetSetVisible(mainNetStatusOnSprite, FALSE);
		ituWidgetSetVisible(mainNetStatusOffIcon, TRUE);
	}
}


char* getDeviceNo()
{
	char tmpStr[50] = { 0 };
	PFULL_DEVICE_NO tmpDev = storage_get_devparam();
	sprintf(tmpStr, "%s%s", "No:", tmpDev->DeviceNoStr);

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
		ituWidgetSetVisible(mainSOSOffButton, FALSE);
		ituWidgetSetVisible(mainSOSOnButton, TRUE);
	}
	else
	{
		ituWidgetSetVisible(mainSOSOnButton, FALSE);
		ituWidgetSetVisible(mainSOSOffButton, TRUE);
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
		ituWidgetSetVisible(page1NoDisturbOffIcon, FALSE);
		ituWidgetSetVisible(page1NoDisturbOffText, FALSE);
		ituWidgetSetVisible(page1NoDisturbOnIcon, TRUE);
		ituWidgetSetVisible(page1NoDisturbOnText, TRUE);
	}
	else
	{
		ituWidgetSetVisible(page1NoDisturbOnIcon, FALSE);
		ituWidgetSetVisible(page1NoDisturbOnText, FALSE);
		ituWidgetSetVisible(page1NoDisturbOffIcon, TRUE);
		ituWidgetSetVisible(page1NoDisturbOffText, TRUE);
	}
}

bool mainDistrubStatusOnChange(ITUWidget* widget, char* param)
{
	if (gDistrubStstus)
	{
		gDistrubStstus = FALSE;
		setDisturbStatus(gDistrubStstus);
	}
	else
	{
		gDistrubStstus = TRUE;
		setDisturbStatus(gDistrubStstus);
	}

	return TRUE;
}


uint8_t	getUnreadRecorderNum()
{
	return 5;
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
	
	if (num > 0)
	{
		gIsScrollingRecorder = true;
		gRecorderTextIndex = 0;
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
		ituWidgetSetVisible(page1RecorderIcon, FALSE);
		ituWidgetSetVisible(page1RecorderText, FALSE);
		ituWidgetSetVisible(page1RecorderNumText, TRUE);
		ituWidgetSetVisible(page1RecorderNumIcon, TRUE);
		ituWidgetSetVisible(page1RecorderMiniIcon, TRUE);
	}
	else
	{
		gIsScrollingRecorder = false;
		gRecorderTextIndex = 0;
		gScrollTimeCount = 0;

		ituWidgetSetVisible(page1RecorderMiniIcon, FALSE);
		ituWidgetSetVisible(page1RecorderNumIcon, FALSE);
		ituWidgetSetVisible(page1RecorderNumText, FALSE);
		ituWidgetSetVisible(page1RecorderIcon, TRUE);
		ituWidgetSetVisible(page1RecorderText, TRUE);
	}
}


void setUnreadRecorderText(uint8_t index)
{
	char tmpStr[50] = { 0 };

	//if (!page1RecorderScrollTittleText)
	//{
	//	page1RecorderScrollTittleText = ituSceneFindWidget(&theScene, "page1RecorderScrollTittleText");
	//	assert(page1RecorderScrollTittleText);
	//}
	if (!page1RecorderScrollTimeText)
	{
		page1RecorderScrollTimeText = ituSceneFindWidget(&theScene, "page1RecorderScrollTimeText");
		assert(page1RecorderScrollTimeText);
	}
	sprintf(tmpStr, "%s%d", "2016-08-03 08:11:1", index);

	//ituTextSetString(page1RecorderScrollTittleText, numstr);
	ituTextSetString(page1RecorderScrollTimeText, tmpStr);


}


uint8_t getUnreadPhotoMsgNum()
{
	return 0;
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
	if (num > 0)
	{
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
		ituWidgetSetVisible(page0PhotoMsgNumText, TRUE);
		ituWidgetSetVisible(page0PhotoMsgNumIcon, TRUE);
	}
	else
	{
		ituWidgetSetVisible(page0PhotoMsgNumIcon, FALSE);
		ituWidgetSetVisible(page0PhotoMsgNumText, FALSE);
	}
}


uint8_t getUnreadInformationNum()
{
	return 0;
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

		ituTextSetString(page0InformationNumText, numstr);
		ituWidgetSetVisible(page0InformationNumText, TRUE);
		ituWidgetSetVisible(page0InformationNumIcon, TRUE);
	}
	else
	{
		ituWidgetSetVisible(page0InformationNumIcon, FALSE);
		ituWidgetSetVisible(page0InformationNumText, FALSE);
	}
}


uint8_t getUnreadMissCallNun()
{
	return 0;
}


void	setUnreadMissCallNum(uint8_t num)
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

		ituTextSetString(page0MissedCallNumText, numstr);
		ituWidgetSetVisible(page0MissedCallNumText, TRUE);
		ituWidgetSetVisible(page0MissedCallNumIcon, TRUE);
	}
	else
	{
		ituWidgetSetVisible(page0MissedCallNumIcon, FALSE);
		ituWidgetSetVisible(page0MissedCallNumText, FALSE);
	}
}


uint8_t getUnsolvedSecurityAlarmNum()
{
	return 0;
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

		ituTextSetString(page0SecurityNumText, numstr);
		ituWidgetSetVisible(page0SecurityNumText, TRUE);
		ituWidgetSetVisible(page0SecurityNumIcon, TRUE);
	}
	else
	{
		ituWidgetSetVisible(page0SecurityNumIcon, FALSE);
		ituWidgetSetVisible(page0SecurityNumText, FALSE);
	}
}


void setSecurityStatus(uint8_t status)
{
	//Sprite����˳��0��ҹ�� 1�����  ����Ϊ�ް���״̬���ɸ��ݾ����������itu˳��

	if (!page0SecuritySprite)
	{
		page0SecuritySprite = ituSceneFindWidget(&theScene, "page0SecuritySprite");
		assert(page0SecuritySprite);
	}
	if (status <= 1)
	{
		ituWidgetSetVisible(page0SecuritySprite, TRUE);
		ituSpriteGoto(page0SecuritySprite, status);
	}
	else
	{
		ituWidgetSetVisible(page0SecuritySprite, FALSE);
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


