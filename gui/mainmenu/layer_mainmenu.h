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

//以下是需要定时器实时获取的状态值！！
bool	getIpIconStatus();
void	setIpIconStatus(bool status);
bool	getNetworkStatus();
void	setNetworkStatus(bool status);
char*	getDeviceNo();
void	setDeviceNo(char* deviceno);
bool	getDisturbStatus();
void	setDisturbStatus(bool status);
uint8_t	grtSecurityStatus();
void	setSecurityStatus(uint8_t status);

void	setSOSBtnType(bool status);

//以下是在界面重新进入时候读取一次就行
uint8_t	getUnreadRecorderNum();
void	setUnreadRecorderNum(uint8_t num);
void	setUnreadRecorderText(uint8_t index);
uint8_t getUnreadPhotoMsgNum();
void	setUnreadPhotoMsgNum(uint8_t num);
uint8_t getUnreadInformationNum();
void	setUnreadInformationNum(uint8_t num);
uint8_t getUnsolvedSecurityAlarmNum();
void	setUnsolvedSecurityAlarmNum(uint8_t num);
uint8_t getUnreadMissCallNun();
void	setUnreadMissCallNum(uint8_t num);

#endif // LAYER_MAINMENU_H