﻿/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_alarm.h
Author:     	caogw
Version:
Date: 			2016-08-31
Description:	报警界面
History:
1. Date:
Author:
Modification:
*************************************************/
#ifndef _LAYER_ALARM_H_
#define _LAYER_ALARM_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "gui_include.h"

	bool AlarmOnEnter(ITUWidget* widget, char* param);
	bool AlarmLayerTimeoutOnTimer(ITUWidget* widget, char* param);


#ifdef __cplusplus
}
#endif

#endif 