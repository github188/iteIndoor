/*********************************************************
Copyright (C), 2015-2020, Aurine
File name:	layer_tempdetection.c
Author:		WuZ
Version:   	1.0
Date:		2016-10-17
Description: ÎÂ¶È¼à²â½çÃæ
History:
1. Date:
Author:
Modification:
2. ...
*********************************************************/


#include "layer_tempdetection.h"

static ITUBackground*		tempDetectionTempBackground = NULL;


bool tempDetectionDateMeterOnChanged(ITUWidget* widget, char* param)
{
	uint16_t tmpPosX = atoi(param);

	printf("22222222222222222222  = %d \n", atoi(param));

	if (!tempDetectionTempBackground)
	{
		tempDetectionTempBackground = ituSceneFindWidget(&theScene, "tempDetectionTempBackground");
		assert(tempDetectionTempBackground);
	}

	ituWidgetSetX(tempDetectionTempBackground, -(tmpPosX * 9));


	return true;
}