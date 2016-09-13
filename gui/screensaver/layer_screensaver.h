/*********************************************************
Copyright (C), 2015-2020, Aurine
File name:	layer_screensaver.h
Author:		chenbh
Version:   	1.0
Date:		2016-09-08
Description: ÆÁÄ»±£»¤
History:
1. Date:
Author:
Modification:
2. ...
*********************************************************/

#ifndef LAYER_SCREENSAVER_H
#define LAYER_SCREENSAVER_H

#include "../gui_include.h"


bool ScreenSaverLayerOnEnter(ITUWidget* widget, char* param);
bool ScreenSaverLayerOnTimer(ITUWidget* widget, char* param);

/*************************************************
Function:		ScreenSaverLayerInit
Description:
Input:
Output:
Return:
Others:
*************************************************/
void ScreenSaverLayerInit(void);

#endif // GUI_CLEANSCREEN_H
