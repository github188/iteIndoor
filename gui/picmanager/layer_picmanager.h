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
#define PICMANAGER_CONTENT_NUM			3


typedef enum
{
	PICMANAGER_MSG_BTN_CANCEL,
	PICMANAGER_MSG_BTN_CONFIRM,
}PICMANAGER_MSG_BTN_e;


typedef enum
{
	PICMANAGER_BTN_RETURN,
	PICMANAGER_BTN_EMPTY,
	PICMANAGER_BTN_DELETE,
	PICMANAGER_BTN_EDIT,
}PICMANAGER_BTN_e;


typedef enum
{
	MINIPIC_CORNER_ICON_CHECK,
	MINIPIC_CORNER_ICON_UNCHECK,
	MINIPIC_CORNER_ICON_NULL,
}MINIPIC_ICON_STATUS_e;


typedef enum
{
	PICMANAGER_PAGE_MINIPIC,
	PICMANAGER_PAGE_EDIT,
	PICMANAGER_PAGE_CONTENT,
}PICMANAGER_PAGE_e;


bool picManagerLayerOnEnter(ITUWidget* widget, char* param);
bool picManagerBtnOnClicked(ITUWidget* widget, char* param);
bool picManagerMsgBoxBtnOnClicked(ITUWidget* widget, char* param);
bool picManagerMiniPicBtnClicked(ITUWidget* widget, char* param);
bool picManagerPictureOnChanged(ITUWidget* widget, char* param);

void picManagerRetutnBtnOnClicked();
void picManagerEditBtnOnClicked();
void picManagerMsgBoxShow(PICMANAGER_BTN_e btnId);
void picManagerPageInit(PICMANAGER_PAGE_e pageId);
void setPicManagerPicture();
bool setPicManagerPictureContent(uint8_t index, char* addrStr);
void setPicManagerMiniPicList();
void setMiniPicListIsChecked(MINIPIC_ICON_STATUS_e status);
bool setMiniPicContent(uint8_t index, char* addrStr);
void setMiniPicIsChecked(uint8_t index, MINIPIC_ICON_STATUS_e status);
MINIPIC_ICON_STATUS_e getMiniPicIsChecked(uint8_t index);

#endif // LAYER_PICMANAGER_H