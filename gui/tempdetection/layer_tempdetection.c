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

static ITUBackground*	tempDetectionTempBackground = NULL;
static ITUText*			tempDetectionDateText = NULL;


bool tempDetectionDateMeterOnChanged(ITUWidget* widget, char* param)
{
	uint16_t tmpPosX = atoi(param);
	char tmpStr[50] = { 0 };

	printf("22222222222222222222  = %d \n", atoi(param));

	if (!tempDetectionTempBackground)
	{
		tempDetectionTempBackground = ituSceneFindWidget(&theScene, "tempDetectionTempBackground");
		assert(tempDetectionTempBackground);
	}
	if (!tempDetectionDateText)
	{
		tempDetectionDateText = ituSceneFindWidget(&theScene, "tempDetectionDateText");
		assert(tempDetectionDateText);
	}

	ituWidgetSetX(tempDetectionTempBackground, -(tmpPosX * 9));

	memset(tmpStr, 0, sizeof(tmpStr));
	sprintf(tmpStr, "%s%d", "2016-10-2", tmpPosX / 70);

	ituTextSetString(tempDetectionDateText, tmpStr);

	return true;
}