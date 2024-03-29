/*********************************************************
Copyright (C), 2015-2020, Aurine
File name:	layer_mainlayer.h
Author:		WuZ
Version:   	1.0
Date:		2016-06-02
Description: 主界面操作函数
History:
1. Date:
Author:
Modification:
2. ...
*********************************************************/

#ifndef LAYER_MAINMENU_H
#define LAYER_MAINMENU_H

#include "../gui_include.h"


typedef enum
{
	MAIN_BTN_AUTOMATION,	//家电控制按钮
	MAIN_BTN_SECURITY,		//安防按钮
}MAIN_BTN_e;


#define MAIN_MAX_RECORDER_NUM		5
#define MAIN_MAX_MSG_NUM			20
#define MAIN_MAX_SECURITY_NUM		20
#define MAIN_MAX_MISSCALL_NUM		20
#define MAIN_SCROLLTEXT_SHOW_TIME	4
#define MAIN_MS_PER_SCROLL			1000
#define MAIN_SCROLL_STEP_COUNT		15

bool mainLayerOnEnter(ITUWidget* widget, char* param);
bool mainLayerOnLeave(ITUWidget* widget, char* param);
bool mainDistrubStatusOnChange(ITUWidget* widget, char* param);
bool mainLayerTimeoutOnTimer(ITUWidget* widget, char* param);
bool mainSOSBtnOnPress(ITUWidget* widget, char* param);
bool mainCoverFlowOnChanged(ITUWidget* widget, char* param);
bool mainBtnOnClicked(ITUWidget* widget, char* param);
bool mainMsgBoxBtnOnClicked(ITUWidget* widget, char* param);
void mainLayerCornerNumReload();
void mainLayerScrollDataReload();
void mainLayerInit();
void mainLayerShow();

void zoneDateTimeToString(DATE_TIME time, char* tmpStr);

//以下是需要定时器实时获取的状态值！！
uint8_t getIpIconStatus();
void setIpIconStatus(uint8_t status);
bool getNetworkStatus();
void setNetworkStatus(bool status);
char* getDeviceNo();
void setDeviceNo(char* deviceno);
bool getDisturbStatus();
void setDisturbStatus(bool status);
void setSecurityStatus(DEFEND_STATE status);
void setSOSBtnType(bool status);

//以下是在界面重新进入时候读取一次就行

bool setMainBackgroundImg();

void setUnreadRecorderNum(uint8_t num);
void setUnreadRecorderText(uint8_t index);
void setUnreadRecorderScroll();

void setUnreadPhotoMsgNum(uint8_t num);
void setUnreadPhotoMsgText(uint8_t index);
void setUnreadPhotoMsgScroll();

void setUnreadInformationNum(uint8_t num);
void setUnreadInformationText(uint8_t index);
void setUnreadInformationScroll();

void setUnsolvedSecurityAlarmNum(uint8_t num);
void setUnsolvedSecurityAlarmText(uint8_t index);
void setUnsolvedSecurityAlarmScroll();

void setUnreadMissedCallNum(uint8_t num);
void setUnreadMissedCallText(uint8_t index);
void setUnreadMissedCallScroll();

#endif // LAYER_MAINMENU_H