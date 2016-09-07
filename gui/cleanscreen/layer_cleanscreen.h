/*********************************************************
Copyright (C), 2015-2020, Aurine
File name:	layer_cleanscreen.h
Author:		WuZ
Version:   	1.0
Date:		2016-06-017
Description: Çå½àÆÁÄ»²Ù×÷º¯Êý
History:
1. Date:
Author:
Modification:
2. ...
*********************************************************/

#ifndef LAYER_CLEANSCREEN_H
#define LAYER_CLEANSCREEN_H

#include "../gui_include.h"

#define CLEANSCREEN_TIMEOUT		10

bool cleanScreenLayerOnEnter(ITUWidget* widget, char* param);
bool cleanScreenLayerOnLeave(ITUWidget* widget, char* param);
bool cleanScreenTimeoutOnTimer(ITUWidget* widget, char* param);
void cleanScreenReset(void);


#endif // GUI_CLEANSCREEN_H