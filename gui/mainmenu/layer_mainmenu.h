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
	//Sprite按照顺序0：夜间 1：外出  其他为无安防状态（可根据具体参数调整itu顺序）
	MAIN_SECURITY_STATUS_NIGHT,
	MAIN_SECURITY_STATUS_OUTSIDE,
}MAIN_SECURITY_STATUS_e;

#define MAIN_MAX_RECORDER_NUM		5
#define MAIN_MAX_MSG_NUM			20
#define MAIN_MAX_SECURITY_NUM		20
#define MAIN_MAX_MISSCALL_NUM		20
#define MAIN_SCROLLTEXT_SHOW_TIME	6
#define MAIN_MS_PER_SCROLL			500
#define MAIN_SCROLL_STEP_COUNT		15

bool	mainLayerOnEnter(ITUWidget* widget, char* param);
bool	mainDistrubStatusOnChange(ITUWidget* widget, char* param);
bool    mainLayerTimeoutOnTimer(ITUWidget* widget, char* param);
void	mainLayerCornerNumReload();
void	mainMenuLayerReset();


void 	zoneDateTimeToString(ZONE_DATE_TIME time, char* tmpStr);

//以下是需要定时器实时获取的状态值！！
bool	getIpIconStatus();
void	setIpIconStatus(bool status);
bool	getNetworkStatus();
void	setNetworkStatus(bool status);
char*	getDeviceNo();
void	setDeviceNo(char* deviceno);
bool	getDisturbStatus();
void	setDisturbStatus(bool status);
void	setSecurityStatus(MAIN_SECURITY_STATUS_e status);
MAIN_SECURITY_STATUS_e	getSecurityStatus();
void	setSOSBtnType(bool status);

//以下是在界面重新进入时候读取一次就行
uint8_t	getUnreadRecorderNum();
void	setUnreadRecorderNum(uint8_t num);
void	setUnreadRecorderText(uint8_t index);
void	setUnreadRecorderScroll();

uint8_t getUnreadPhotoMsgNum();
void	setUnreadPhotoMsgNum(uint8_t num);
void	setUnreadPhotoMsgText(uint8_t index);
void	setUnreadPhotoMsgScroll();

uint8_t getUnreadInformationNum();
void	setUnreadInformationNum(uint8_t num);
void	setUnreadInformationText(uint8_t index);
void	setUnreadInformationScroll();

uint8_t getUnsolvedSecurityAlarmNum();
void	setUnsolvedSecurityAlarmNum(uint8_t num);
void	setUnsolvedSecurityAlarmText(uint8_t index);
void	setUnsolvedSecurityAlarmScroll();

uint8_t getUnreadMissedCallNun();
void	setUnreadMissedCallNum(uint8_t num);
void	setUnreadMissedCallText(uint8_t index);
void	setUnreadMissedCallScroll();

#endif // LAYER_MAINMENU_H