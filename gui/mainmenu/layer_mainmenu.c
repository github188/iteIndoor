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
static ITUIcon*		page0SecurityNumIcon;
static ITUText*		page0SecurityNumText;
static ITUSprite*   page0SecuritySprite;
static ITUIcon*		page0PhotoMsgNumIcon;
static ITUText*		page0PhotoMsgNumText;
static ITUIcon*		page0InformationNumIcon;
static ITUText*		page0InformationNumText;
static ITUIcon*		page0MissCallNumIcon;
static ITUText*		page0MissCallNumText;

static ITUText*		page0SecurityText;


static bool		gDistrubStstus = FALSE;
static uint32_t	gMainLayerLastTimeTick;	//用来记录定时器上个时刻的时间



bool mainLayerOnEnter(ITUWidget* widget, char* param)
{
	//在进入这个界面时候，需要做的动作，比如初始化图标，读取状态等！！！！！
	mainLayerCornerNumReload();

	if (!page0SecurityText)
	{
		page0SecurityText = ituSceneFindWidget(&theScene, "page0SecurityText");
		assert(page0SecurityText);
	}
	ituWidgetHide(page0SecurityText, ITU_EFFECT_SCROLL_UP, 10);
	
	setNetworkStatus(TRUE);				//设置网络状态
	setDeviceNo((char*)"NO:010101011");	//设置设备编号
	setSOSBtnType(TRUE);				//设置SOS按键状态
	setDisturbStatus(gDistrubStstus);	//设置免打扰状态
	setRecorderNum((uint8_t)13);	    //设置家人留言条数
	setPhotoMsgNum((uint8_t)15);		//设置留影留言条数
	setInformationNum((uint8_t)18);		//设置信息条数
	setSecurityStatus((uint8_t)1);		//设置安防状态
	setSecurityAlarmNum((uint8_t)11);	//设置安防报警数
	setMissCallNum((uint8_t)11);		//设置未接来电数
	setIpIconStatus(TRUE);				//设置IP模块启用与否

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

	if (duration >= MS_PER_SECOND)		//时间差是以毫秒为单位
	{
		gMainLayerLastTimeTick = curtime;
		//TODO:读取存储和全局变量对比，不一样时候在设置各种状态值
		if (ituWidgetIsVisible(page0SecurityText) == false)
			ituWidgetShow(page0SecurityText, ITU_EFFECT_SCROLL_UP, 10);
		else
			ituWidgetHide(page0SecurityText, ITU_EFFECT_SCROLL_UP, 10);
	}
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


uint8_t	getRecorderNum()
{
	return 0;
}


void setRecorderNum(uint8_t num)
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
	if (num > 0)
	{
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
		ituWidgetSetVisible(page1RecorderNumText, TRUE);
		ituWidgetSetVisible(page1RecorderNumIcon, TRUE);
	}
	else
	{
		ituWidgetSetVisible(page1RecorderNumIcon, FALSE);
		ituWidgetSetVisible(page1RecorderNumText, FALSE);
	}
}


uint8_t getPhotoMsgNum()
{
	return 0;
}


void setPhotoMsgNum(uint8_t num)
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


uint8_t getInformationNum()
{
	return 0;
}

void setInformationNum(uint8_t num)
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


uint8_t getMissCallNun()
{
	return 0;
}


void	setMissCallNum(uint8_t num)
{
	char numstr[4] = { 0 };

	if (!page0MissCallNumIcon)
	{
		page0MissCallNumIcon = ituSceneFindWidget(&theScene, "page0MissCallNumIcon");
		assert(page0MissCallNumIcon);
	}
	if (!page0MissCallNumText)
	{
		page0MissCallNumText = ituSceneFindWidget(&theScene, "page0MissCallNumText");
		assert(page0MissCallNumText);
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

		ituTextSetString(page0MissCallNumText, numstr);
		ituWidgetSetVisible(page0MissCallNumText, TRUE);
		ituWidgetSetVisible(page0MissCallNumIcon, TRUE);
	}
	else
	{
		ituWidgetSetVisible(page0MissCallNumIcon, FALSE);
		ituWidgetSetVisible(page0MissCallNumText, FALSE);
	}
}


uint8_t getSecurityAlarmNum()
{
	return 0;
}


void setSecurityAlarmNum(uint8_t num)
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


