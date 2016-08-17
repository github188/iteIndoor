﻿/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_call.h
Author:     	caogw
Version:
Date: 			2016-08-11
Description:	呼叫所有的外部函数
History:
1. Date:
Author:
Modification:
*************************************************/
#ifndef _LAYER_CALL_H_
#define _LAYER_CALL_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "gui_include.h"

// CallLayer
bool CallLayerOnEnter(ITUWidget* widget, char* param);
bool CallLayerButtonOnMouseUp(ITUWidget* widget, char* param);
bool CallKeyBordButtonOnMouseUp(ITUWidget* widget, char* param);
bool CallKeyBordCalloutButtonOnMouseUp(ITUWidget* widget, char* param);
bool CallKeyBordDelButtonOnMouseUp(ITUWidget* widget, char* param);
bool CallLayerOnTimer(ITUWidget* widget, char* param);
bool CallKeyBordDelButtonOnMouseLongPress(ITUWidget* widget, char* param);
bool CallRecordListButtonOnMouseUp(ITUWidget* widget, char* param);
bool CallCeterListButtonOnMouseUp(ITUWidget* widget, char* param);
bool CallRecordMSGButtonOnMouseUp(ITUWidget* widget, char* param);
bool CallCallRequestState(ITUWidget* widget, char* param);

// MonitorSearch
bool MonitorSearchLayerOnEnter(ITUWidget* widget, char* param);
bool MonitorSearchLayerButtonOnMouseUp(ITUWidget* widget, char* param);
bool MonitorSearchListButtonOnMouseUp(ITUWidget* widget, char* param);
bool MonitorSearchMSGButtonOnMouseUp(ITUWidget* widget, char* param);

// BeCall
bool BeCallLayerOnEnter(ITUWidget* widget, char* param);
bool BeCallLayerButtonOnMouseUp(ITUWidget* widget, char* param);
bool BeCallShowButtomBackgroundOnMouseUp(ITUWidget* widget, char* param);
bool BeCallButtomSoundButtonOnMouseUp(ITUWidget* widget, char* param);
bool BeCallButtomSoundOnChanged(ITUWidget* widget, char* param);
bool BeCallLayerOnTimer(ITUWidget* widget, char* param);
bool BeCallCallOutState(ITUWidget* widget, char* param);
bool BeCallCallInState(ITUWidget* widget, char* param);
uint8 BeCallWin(INTER_INFO_S* info);

#ifdef __cplusplus
}
#endif

#endif 
