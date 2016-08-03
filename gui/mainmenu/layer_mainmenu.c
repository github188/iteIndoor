/*********************************************************
Copyright (C), 2015-2020, Aurine
File name:	layer_mainlayer.c
Author:		WuZ
Version:   	1.0
Date:		2016-06-17
Description: 主界面操作函数
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
static ITUText*		page0SecurityScrollTrigerText;
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
static ITUText*		page0IntercomScrollTittleText;
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



static bool		gDistrubStstus;
static uint32_t	gMainLayerLastTimeTick;	//用来记录定时器上个时刻的时间


static bool		gIsScrollingRecorder;
static uint8_t  gRecorderTextIndex;
static bool		gIsScrollingInformation;
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
	//在进入这个界面时候，需要做的动作，比如初始化图标，读取状态等！！！！！
	mainLayerCornerNumReload();
	
	setNetworkStatus(TRUE);				//设置网络状态
	setDeviceNo((char*)"NO:010101011");	//设置设备编号
	setSOSBtnType(TRUE);				//设置SOS按键状态
	setDisturbStatus(gDistrubStstus);	//设置免打扰状态
	setIpIconStatus(TRUE);				//设置IP模块启用与否

	setUnreadRecorderNum((uint8_t)getUnreadRecorderNum());	    //设置家人留言条数
	setUnreadPhotoMsgNum((uint8_t)getUnreadPhotoMsgNum());		//设置留影留言条数
	setUnreadInformationNum((uint8_t)getUnreadInformationNum());	//设置信息条数
	setSecurityStatus((uint8_t)1);								//设置安防状态
	setUnsolvedSecurityAlarmNum((uint8_t)11);					//设置安防报警数
	setUnreadMissCallNum((uint8_t)11);							//设置未接来电数

	gMainLayerLastTimeTick = SDL_GetTicks();		//开启定时器前要先获取一次当前时间以便对比

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


	if (duration >= MAIN_MS_PER_SCROLL)		//时间差是以毫秒为单位
	{
		gMainLayerLastTimeTick = curtime;
		//TODO:读取存储和全局变量对比，不一样时候在设置各种状态值
		gScrollTimeCount++;

		setUnreadRecorderScroll();		//未读家人留言滚动功能
		setUnreadPhotoMsgScroll();		//设置留影留言滚动功能
		setUnreadInformationScroll();	//设置信息滚动功能

		if (gScrollTimeCount > MAIN_SCROLLTEXT_SHOW_TIME)
		{
			gScrollTimeCount = 0;
		}
	}

	return true;
}


void mainLayerCornerNumReload()
{
	//TODO:用户在界面Enter时候载入不需要实时去获取的数据！！！！！
}


bool getIpIconStatus()
{
	return (bool)get_ipmodule_bindstate();		//获取是否启用IP模块
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
	return (bool)storage_get_noface();	//存储获取是否启用免打扰
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
	if (!page1RecorderScrollTextContainer)
	{
		page1RecorderScrollTextContainer = ituSceneFindWidget(&theScene, "page1RecorderScrollTextContainer");
		assert(page1RecorderScrollTextContainer);
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

		ituWidgetSetVisible(page1RecorderScrollTextContainer, FALSE);
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

	if (!page1RecorderScrollTimeText)
	{
		page1RecorderScrollTimeText = ituSceneFindWidget(&theScene, "page1RecorderScrollTimeText");
		assert(page1RecorderScrollTimeText);
	}
	//TODO:读取存储设置文字内容！！
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
	return 10;
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
		gPhotoMsgTextIndex = 0;
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
		ituWidgetSetVisible(page0PhotoMsgText, FALSE);
		ituWidgetSetVisible(page0PhotoMsgIcon, FALSE);
		ituWidgetSetVisible(page0PhotoMsgMiniIcon, TRUE);
		ituWidgetSetVisible(page0PhotoMsgNumText, TRUE);
		ituWidgetSetVisible(page0PhotoMsgNumIcon, TRUE);
	}
	else
	{
		gIsScrollingPhotoMsg = false;
		gPhotoMsgTextIndex = 0;
		gScrollTimeCount = 0;

		ituWidgetSetVisible(page0PhotoMsgScrollTextContainer, FALSE);
		ituWidgetSetVisible(page0PhotoMsgMiniIcon, FALSE);
		ituWidgetSetVisible(page0PhotoMsgNumIcon, FALSE);
		ituWidgetSetVisible(page0PhotoMsgNumText, FALSE);
		ituWidgetSetVisible(page0PhotoMsgText, TRUE);
		ituWidgetSetVisible(page0PhotoMsgIcon, TRUE);
	}
}


void setUnreadPhotoMsgText(uint8_t index)
{
	char tmpStr[50] = { 0 };

	if (!page0PhotoMsgScrollTimeText)
	{
		page0PhotoMsgScrollTimeText = ituSceneFindWidget(&theScene, "page0PhotoMsgScrollTimeText");
		assert(page0PhotoMsgScrollTimeText);
	}
	//TODO:读取存储设置文字内容！！
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
	return 8;
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

		gIsScrollingInformation = TRUE;
		gInformationTextIndex = 0;
		gScrollTimeCount = 0;

		ituTextSetString(page0InformationNumText, numstr);
		ituWidgetSetVisible(page0InformationText, FALSE);
		ituWidgetSetVisible(page0InformationIcon, FALSE);
		ituWidgetSetVisible(page0InformationMiniIcon, TRUE);
		ituWidgetSetVisible(page0InformationNumText, TRUE);
		ituWidgetSetVisible(page0InformationNumIcon, TRUE);
	}
	else
	{
		gIsScrollingInformation = false;
		gInformationTextIndex = 0;
		gScrollTimeCount = 0;

		ituWidgetSetVisible(page0InformationNumIcon, FALSE);
		ituWidgetSetVisible(page0InformationNumText, FALSE);
		ituWidgetSetVisible(page0InformationMiniIcon, FALSE);
		ituWidgetSetVisible(page0InformationScrollTextContainer, FALSE);
	}
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
	//TODO:读取存储设置文字内容！！
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
	//Sprite按照顺序0：夜间 1：外出  其他为无安防状态（可根据具体参数调整itu顺序）

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
	//TODO:上电时候把静态变量全部初始化为NULL
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


