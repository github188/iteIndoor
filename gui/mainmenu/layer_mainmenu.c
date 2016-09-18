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

static ITULayer*		mainLayer = NULL;
static ITUBackground*	mainTipsTransparencyBackground;
static ITUText*			mainDigitalClockWeekText;
static ITUCoverFlow*	mainPageCoverFlow;
static ITUSprite*		mainIPSprite;
static ITUIcon*			mainNetStatusOffIcon;
static ITUSprite*		mainNetStatusOnSprite;
static ITUText*			mainDeviceNoText;
static ITUButton*		mainSOSOnButton;
static ITUButton*		mainSOSOffButton;
static ITUIcon*			page1NoDisturbOnIcon;
static ITUText*			page1NoDisturbOnText;
static ITUIcon*			page1NoDisturbOffIcon;
static ITUText*			page1NoDisturbOffText;
static ITUIcon*			mainBackgroundImgIcon;

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



static uint32_t	gMainLayerLastTimeTick;	//用来记录定时器上个时刻的时间

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

static uint8_t  gMainBackgroundIndex = 0;

static PSysHintRecord			gMainScrollData= NULL;		//全局数据变量（公用一个）

static PJRLYLIST_INFO			gRecorderData	 = NULL;		//家人留言数据
static PLYLYLIST_INFO			gPhotoMsgData	 = NULL;		//留影留言数据 
static PMSGLIST					gInformationData = NULL;		//信息数据
static PALARM_TOUCH_INFO_LIST	gSecurityData	 = NULL;		//安防数据
static PMCALLLISTINFO			gMissedCallData	 = NULL;		//未接呼叫数据


static uint8_t*			gPicManagerImageData;
static int				gPicManagerImageSize;
static char				gPicManagerImageFilePath[PATH_MAX];


bool mainLayerOnEnter(ITUWidget* widget, char* param)
{
#if 0
	char tmpStr[50] = { 0 };
	//char tmpLan[4] = {0xd2, 0xbb, 0x00, 0x00};

	//char* tmpLan = "一二三";
	char* tmpLan = "一二三";
	printf("111111111111111111111before  = %x %x %x \n", tmpLan[0], tmpLan[1], tmpLan[2]);
	printf("before  = %s \n", tmpLan);
	gb2312ToUtf8(tmpStr, strlen(tmpLan), tmpLan, strlen(tmpLan));
	printf("22222222222222222after = %x %x %x \n", tmpStr[0], tmpStr[1], tmpStr[2]);
	printf("after = %s \n", tmpStr);
	setDeviceNo(tmpLan);
#endif

	gSOSBtnIsPress = false;
	gSOSIsAlarm = false;
	gMainLayerLastTimeTick = SDL_GetTicks();		//开启定时器前要先获取一次当前时间以便对比

	mainLayerInit();

	return true;
}


void mainLayerInit()
{
	if (!mainTipsTransparencyBackground)
	{
		mainTipsTransparencyBackground = ituSceneFindWidget(&theScene, "mainTipsTransparencyBackground");
		assert(mainTipsTransparencyBackground);
	}

	setSOSBtnType(false);

	//在进入这个界面时候，需要做的动作，比如初始化图标，读取状态等！！！！！
	mainLayerScrollDataReload();
	mainLayerCornerNumReload();

	ituWidgetSetVisible(mainTipsTransparencyBackground, false);
}


bool mainLayerOnLeave(ITUWidget* widget, char* param)
{
	printf("\mainLayerOnLeave!!!!!!!!!\n");

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


	if (duration >= MAIN_MS_PER_SCROLL)		//时间差是以毫秒为单位
	{
		gMainLayerLastTimeTick = curtime;
		//TODO:读取存储和全局变量对比，不一样时候在设置各种状态值
		gScrollTimeCount++;

		setUnreadRecorderScroll();			//未读家人留言滚动功能
		setUnreadPhotoMsgScroll();			//设置留影留言滚动功能
		setUnreadInformationScroll();		//设置信息滚动功能
		setUnreadMissedCallScroll();		//设置未接来电滚动功能
		setUnsolvedSecurityAlarmScroll();	//设置报警滚动功能

		setNetworkStatus(getNetworkStatus());		//设置网络状态
		setDeviceNo(getDeviceNo());					//设置设备编号
		setDisturbStatus(getDisturbStatus());		//设置免打扰状态
		setIpIconStatus(getIpIconStatus());			//设置IP模块启用与否

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
				//TODO:逻辑发送SOS求助命令！！
				if (gSOSIsAlarm == false)
				{
					gSOSIsAlarm = true;
					printf("\ngSOSIsAlarm!!!\n");
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

		return true;
	}

	return false;
}


bool mainCoverFlowOnChanged(ITUWidget* widget, char* param)
{
	if (!mainPageCoverFlow)
	{
		mainPageCoverFlow = ituSceneFindWidget(&theScene, "mainPageCoverFlow");
		assert(mainPageCoverFlow);
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


bool mainBtnOnClicked(ITUWidget* widget, char* param)
{

	return true;
}


void zoneDateTimeToString(DATE_TIME time, char* tmpStr)
{
	sprintf(tmpStr, "%04d-%02d-%02d %02d:%02d:%02d", time.year, time.month, time.day, time.hour, time.min, time.sec);
}


void mainLayerCornerNumReload()
{
	//TODO:用户在界面Enter时候载入不需要实时去获取的数据！！！！！
	//setUnreadRecorderNum((uint8_t)getUnreadRecorderNum());				//设置家人留言条数
	//setUnreadPhotoMsgNum((uint8_t)getUnreadPhotoMsgNum());				//设置留影留言条数
	//setUnreadInformationNum((uint8_t)getUnreadInformationNum());		//设置信息条数
	//setSecurityStatus((MAIN_SECURITY_STATUS_e)getSecurityStatus());		//设置安防状态
	//setUnsolvedSecurityAlarmNum((uint8_t)getUnsolvedSecurityAlarmNum());//设置安防报警数
	//setUnreadMissedCallNum((uint8_t)getUnreadMissedCallNun());			//设置未接来电数

	setUnreadRecorderNum((uint8_t)gMainScrollData->syshintnum[SYS_HINT_FAMILY]);					//设置家人留言条数
	setUnreadPhotoMsgNum((uint8_t)gMainScrollData->syshintnum[SYS_HINT_LEAVEWORD]);					//设置留影留言条数
	setUnreadInformationNum((uint8_t)gMainScrollData->syshintnum[SYS_HINT_INFO]);					//设置信息条数
	setSecurityStatus((MAIN_SECURITY_STATUS_e)gMainScrollData->syshintnum[SYS_HINT_ALARM_STATE]);	//设置安防状态
	setUnsolvedSecurityAlarmNum((uint8_t)gMainScrollData->syshintnum[SYS_HINT_ALARM_WARNING]);		//设置安防报警数
	setUnreadMissedCallNum((uint8_t)gMainScrollData->syshintnum[SYS_HINT_MISSED_CALLS]);			//设置未接来电数

	setMainBackgroundImg();												//设置主界面背景图
}


void mainLayerScrollDataReload()
{
	if (gMainScrollData == NULL)
	{
		gMainScrollData = (PSysHintRecord)sys_get_hint_list();
	}

	//loadUnreadRecorderData();				//载入未读家人留言数据
	//loadUnreadInformationData();			//载入未读信息数据
	//loadUnreadMissedCallData();				//载入未接电话信息
	//loadUnreaPhotoMsgData();				//载入未读留影留言数据
	//loadUnsolvedSecurityAlarmData();		//载入为处理的报警数据
}


bool setMainBackgroundImg()
{
	char		tmpAddr[50] = { 0 };
	uint8_t		tmpIndex	= 0;
	FILE*		tmpFile;

	if (!mainBackgroundImgIcon)
	{
		mainBackgroundImgIcon = ituSceneFindWidget(&theScene, "mainBackgroundImgIcon");
		assert(mainBackgroundImgIcon);
	}

	tmpIndex = storage_get_desk();
	//if (tmpIndex == gMainBackgroundIndex)
	//{
	//	return false;
	//}
	gMainBackgroundIndex = tmpIndex;

	memset(tmpAddr, 0, sizeof(tmpAddr));
	sprintf(tmpAddr, "%s%s%d%s", WALL_PAPER_DIR_PATH, "bk_", gMainBackgroundIndex, ".jpg");

	// try to load minipic jpeg file if exists
	tmpFile = fopen(tmpAddr, "rb");
	if (tmpFile)
	{
		struct stat sb;
		if (fstat(fileno(tmpFile), &sb) != -1)			//用_fileno代替fileno避免运行时候的警告！！！！（但是板子上编译不过！！！！）
		{
			gPicManagerImageSize = (int)sb.st_size;
			gPicManagerImageData = malloc(gPicManagerImageSize);
			if (gPicManagerImageSize)
			{
				gPicManagerImageSize = fread(gPicManagerImageData, 1, gPicManagerImageSize, tmpFile);
			}
		}
		fclose(tmpFile);
	}
	else
	{
		printf("open  minipic jepg icon icon failed!");
		return false;
	}
	if (gPicManagerImageData)
	{
		ituIconLoadJpegData((ITUIcon*)mainBackgroundImgIcon, gPicManagerImageData, gPicManagerImageSize);
	}
	else
	{
		printf("load minipic jepg icon failed!");
		return false;
	}

	//TODO: 可能要对传进来的指针数据进行释放，看后期数据如何传递！！！
	free(gPicManagerImageData);
	return true;

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
	memset(tmpStr, 0, sizeof(tmpStr));
	sprintf(tmpStr, "%s%s", "NO:", tmpDev->DeviceNoStr);
	//TODO:返回地址需要申请变量！！！！！！malloc！！！
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
		storage_set_noface_enable(false);
	}
	else
	{
		setDisturbStatus(true);
		storage_set_noface_enable(true);
	}

	start_disturb_timer();

	return true;
}


uint8_t	getUnreadRecorderNum()
{
	uint8_t tmpCount = 0;
	uint8_t i = 0;

	if (gRecorderData == NULL)
	{
		loadUnreadRecorderData();
	}

	for (i = 0; i < gRecorderData->Count; i++)
	{
		if (gRecorderData->JrlyInfo[i].UnRead == TRUE)
		{
			tmpCount++;
		}
	}
	return tmpCount;
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

		memset(numstr, 0, sizeof(numstr));
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
		gRecorderTextIndex = 0;
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

	//进入页面时候读取一次存储即可，然后滚动条直接调用这个指针
	if (gRecorderData == NULL)
	{
		gRecorderData = (PJRLYLIST_INFO)malloc(sizeof(JRLYLIST_INFO));
	}
	storage_get_jrlyrecord(&tmpListInfo);
	memcpy(gRecorderData, tmpListInfo, sizeof(JRLYLIST_INFO));

	free(tmpListInfo);

	printf("\n unread recorder count  = %d \n", gRecorderData->Count);
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
	memset(tmpStr, 0, sizeof(tmpStr));
	zoneDateTimeToString(gMainScrollData->JrlyUnReadList->JrlyInfo[index].Time, tmpStr);
	printf("\n1111111111111 = %d    %s\n",index, tmpStr);
	ituTextSetString(page1RecorderScrollTimeText, tmpStr);
}


void setUnreadRecorderScroll()
{
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t tmpIndex = 0;

	if (gIsScrollingRecorder == true)
	{
		if (ituWidgetIsVisible(page1RecorderScrollTextContainer) == false)
		{
			if (gRecorderTextIndex >= (uint8_t)gMainScrollData->JrlyUnReadList->Count)
			{
				gRecorderTextIndex = 0;
			}
			tmpIndex = gRecorderTextIndex;

			for (i = tmpIndex; i <= (uint8_t)gMainScrollData->JrlyUnReadList->Count; i++)
			{
				if (!gMainScrollData->JrlyUnReadList->JrlyInfo[i].UnRead)
				{
					gRecorderTextIndex++;

					if (gRecorderTextIndex >= (uint8_t)gMainScrollData->JrlyUnReadList->Count)
					{
						gRecorderTextIndex = 0;

						for (j = 0; j < tmpIndex; j++)
						{
							if (gMainScrollData->JrlyUnReadList->JrlyInfo[j].UnRead)
							{
								break;
							}
							else
							{
								gRecorderTextIndex++;
							}
						}
					}
				}
				else
				{
					break;
				}
			}

			setUnreadRecorderText(gRecorderTextIndex);
			gRecorderTextIndex++;
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
	uint8_t tmpCount = 0; 
	uint8_t i = 0;

	if (gPhotoMsgData == NULL)
	{
		loadUnreaPhotoMsgData();
	}
	for (i = 0; i < gPhotoMsgData->Count; i++)
	{
		if (gPhotoMsgData->LylyInfo[i].UnRead == TRUE)
		{
			tmpCount++;
		}
	}

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

		memset(numstr, 0, sizeof(numstr));
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
	PLYLYLIST_INFO tmpListInfo = NULL;

	//进入页面时候读取一次存储即可，然后滚动条直接调用这个指针
	if (gPhotoMsgData == NULL)
	{
		gPhotoMsgData = (PJRLYLIST_INFO)malloc(sizeof(JRLYLIST_INFO));
	}
	storage_get_lylyrecord(&tmpListInfo);
	memcpy(gPhotoMsgData, tmpListInfo, sizeof(JRLYLIST_INFO));

	free(tmpListInfo);

	printf("\n unread PhotoMsg count  = %d \n", gPhotoMsgData->Count);
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
	memset(tmpStr, 0, sizeof(tmpStr));
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
	uint8_t tmpCount = 0;
	uint8_t i = 0;

	if (gInformationData == NULL)
	{
		loadUnreadInformationData();
	}
	for (i = 0; i < gInformationData->ncount; i++)
	{
		if (1 == gInformationData->pinfo_data[i].is_unread)
		{
			tmpCount++;
		}
	}

	return tmpCount;
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

	memset(numstr, 0, sizeof(numstr));
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
	PMSGLIST tmpListInfo = NULL;

	//进入页面时候读取一次存储即可，然后滚动条直接调用这个指针
	if (gInformationData == NULL)
	{
		gInformationData = (PMSGLIST)malloc(sizeof(MSGLIST));
	}
	tmpListInfo = storage_read_msg_list();
	memcpy(gInformationData, tmpListInfo, sizeof(MSGLIST));

	free(tmpListInfo);

	printf("\n unread PhotoMsg count  = %d \n", gInformationData->ncount);
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
	memset(tmpStr, 0, sizeof(tmpStr));
	sprintf(tmpStr, "%s%d", "Theme", index);
	ituTextSetString(page0InformationScrollThemeText, tmpStr);

	memset(tmpStr, 0, sizeof(tmpStr));
	sprintf(tmpStr, "%s%d", "01234567890", index);
	ituTextSetString(page0InformationScrollContentText0, tmpStr);

	memset(tmpStr, 0, sizeof(tmpStr));
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
	uint8_t tmpCount = 0;
	uint8_t i = 0;

	if (gMissedCallData == NULL)
	{
		loadUnreadMissedCallData();
	}
	for (i = 0; i < gMissedCallData->CallCount; i++)
	{
		if (1 == gMissedCallData->CallInfo[i].UnRead)
		{
			tmpCount++;
		}
	}

	return tmpCount;
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

	memset(numstr, 0, sizeof(numstr));
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
	PMCALLLISTINFO tmpListInfo = NULL;

	//进入页面时候读取一次存储即可，然后滚动条直接调用这个指针
	if (gMissedCallData == NULL)
	{
		gMissedCallData = (PMCALLLISTINFO)malloc(sizeof(MCALLLISTINFO));
	}
	tmpListInfo = storage_get_callrecord(FLAG_CALLMISS);
	memcpy(gMissedCallData, tmpListInfo, sizeof(MCALLLISTINFO));
	free(tmpListInfo);

	printf("\n unread PhotoMsg count  = %d \n", gMissedCallData->CallCount);
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

	//TODO:读取存储设置文字内容！！
	memset(tmpStr, 0, sizeof(tmpStr));
	sprintf(tmpStr, "%s%d", "Admin", index);
	ituTextSetString(page0IntercomScrollFromText, tmpStr);

	memset(tmpStr, 0, sizeof(tmpStr));
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
	if (gSecurityData == NULL)
	{
		loadUnsolvedSecurityAlarmData();
	}

	return gSecurityData->nCount;
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
	
	memset(numstr, 0, sizeof(numstr));
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
	PALARM_TOUCH_INFO_LIST tmpListInfo = NULL;

	//进入页面时候读取一次存储即可，然后滚动条直接调用这个指针
	if (gSecurityData == NULL)
	{
		gSecurityData = (PALARM_TOUCH_INFO_LIST)malloc(sizeof(ALARM_TOUCH_INFO_LIST));
	}
	tmpListInfo = storage_get_afbj_unread_record();
	memcpy(gSecurityData, tmpListInfo, sizeof(ALARM_TOUCH_INFO_LIST));
	free(tmpListInfo);

	printf("\n unread PhotoMsg count  = %d \n", gSecurityData->nCount);
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

	//TODO:读取存储设置文字内容！！
	memset(tmpStr, 0, sizeof(tmpStr));
	sprintf(tmpStr, "%s%d", "GAS", index);
	ituTextSetString(page0SecurityScrollAlarmTypeText, tmpStr);

	memset(tmpStr, 0, sizeof(tmpStr));
	sprintf(tmpStr, "%s%d", "Triger", index);
	ituTextSetString(page0SecurityScrollTriggerText, tmpStr);

	memset(tmpStr, 0, sizeof(tmpStr));
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
		//布防！！（外出）
		return (MAIN_SECURITY_STATUS_e)1;
	}
	else if (storage_get_defend_state() == PART_DEFEND)
	{
		//局防！！！（夜间）
		return (MAIN_SECURITY_STATUS_e)0;
	}
	else
	{
		//撤防！！！（无图标）
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


void mainLayerShow()
{
	if (!mainLayer)
	{
		mainLayer = ituSceneFindWidget(&theScene, "mainLayer");
		assert(mainLayer);
	}
	if (!ituWidgetIsVisible(mainLayer))
	{
		ituLayerGoto(mainLayer);
	}
}

