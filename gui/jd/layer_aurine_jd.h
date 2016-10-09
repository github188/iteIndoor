/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_aurine_jd.h
Author:     	caogw
Version:
Date: 			2016-09-23
Description:	家电所有的外部函数
History:
1. Date:
Author:
Modification:
*************************************************/
#ifndef _LAYER_AURINE_JD_H_
#define _LAYER_AURINE_JD_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "gui_include.h"

// AurineJDLayer
bool AurineJDLayerOnEnter(ITUWidget* widget, char* param);
bool AurineJDLayerButtonOnMouseUp(ITUWidget* widget, char* param);
bool AurineJDLayerOnTimer(ITUWidget* widget, char* param);
bool AurineJDSceneButtonOnMouseUp(ITUWidget* widget, char* param);
bool AurineJDLightUnControlButtonOnMouseUp(ITUWidget* widget, char* param);
bool AurineJDLightControlButtonOnMouseUp(ITUWidget* widget, char* param);
bool AurineJDAirButtonOnMouseUp(ITUWidget* widget, char* param);
bool AurineJDCurtainButtonOnMouseUp(ITUWidget* widget, char* param);
bool AurineJDPowerButtonOnMouseUp(ITUWidget* widget, char* param);
bool AurineJDGasButtonOnMouseUp(ITUWidget* widget, char* param);
bool AurineJDLightOnChanged(ITUWidget* widget, char* param);
bool AurineJDExitLightControlButtonOnMouseUp(ITUWidget* widget, char* param);
void AurineJDWin(void);

#ifdef __cplusplus
}
#endif

#endif 
