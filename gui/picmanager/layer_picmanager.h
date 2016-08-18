/*********************************************************
Copyright (C), 2015-2020, Aurine
File name:	layer_picmanager.h
Author:		WuZ
Version:   	1.0
Date:		2016-08-18
Description: 图片管理操作函数
History:
1. Date:
Author:
Modification:
2. ...
*********************************************************/

#ifndef LAYER_PICMANAGER_H
#define LAYER_PICMANAGER_H

#include "../gui_include.h"

#define PICMANAGER_MINIPIC_MAX			16

typedef enum
{
	PICMANAGER_MINIPIC_PAGE,
	PICMANAGER_PICCONTENT_PAGE,
}PICMANAGER_PAGE_e;

typedef enum
{
	MINIPIC_CORNER_ICON_CHECK,
	MINIPIC_CORNER_ICON_UNCHECK,
	MINIPIC_CORNER_ICON_NULL,
}MINIPIC_CORNER_ICON_STATUS_e;

bool picManagerLayerOnEnter(ITUWidget* widget, char* param);
bool picManagerBtnOnClicked(ITUWidget* widget, char* param);
bool picManagerMsgBoxBtnOnClicked(ITUWidget* widget, char* param);

void picManagerPageInit(PICMANAGER_PAGE_e pageId);
void setPicManagerMiniPicList();
bool setMiniPicContent(uint8_t index, char* addrStr);
void setMiniPicIsChecked(uint8_t index, MINIPIC_CORNER_ICON_STATUS_e status);

#endif // LAYER_PICMANAGER_H