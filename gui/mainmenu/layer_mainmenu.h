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
#include "../information/layer_information.h"

#define MAIN_MAX_RECORDER_NUM		20
#define MAIN_MAX_MSG_NUM			20
#define MAIN_MAX_SECURITY_NUM		20
#define MAIN_MAX_MISSCALL_NUM		20

bool	mainMenuLayerOnEnter(ITUWidget* widget, char* param);
bool	mainDistrubStatusOnChange(ITUWidget* widget, char* param);
void	mainMenuLayerReset();

void	setDigtalClockWeek();
char*	getWeekStringByDay(uint16_t year, uint16_t month, uint16_t day);
void	setIpIconStatus(bool status);
void	setNetworkStatus(bool status);
void	setDeviceNo(char* deviceno);
void	setDisturbStatus(bool status);
void	setSOSBtnType(bool status);
void	setRecorderNum(uint8_t num);
void	setPhotoMsgNum(uint8_t num);
void	setInformationNum(uint8_t num);
void	setSecurityStatus(uint8_t status);
void	setSecurityAlarmNum(uint8_t num);
void	setMissCallNum(uint8_t num);

#endif // LAYER_MAINMENU_H