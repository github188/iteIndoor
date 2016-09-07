/*********************************************************
Copyright (C), 2015-2020, Aurine
File name:	layer_information.h
Author:		WuZ
Version:   	1.0
Date:		2016-06-30
Description: 信息操作函数
History:
1. Date:
Author:
Modification:
2. ...
*********************************************************/

#ifndef LAYER_INFORMATION_H
#define LAYER_INFORMATION_H

#include "../gui_include.h"


#define MAX_INFORMATION_NUM			20
#define INFORMATION_PER_PAGE		6
#define NIFORMATION_PAGE_NUM		4
#define MAX_INFORMATION_THEME_LEN	15

typedef enum
{
	INFORMATION_MSGICON_READED,
	INFORMATION_MSGICON_UNREADED,
	INFORMATION_MSGICON_HIDE,
}INFORMATION_MSGICON_STATUS_e;

typedef enum 
{
	INFORMATION_LIST_PAGE,
	INFORMATION_CONTENT_PAGE,
}INFORMATION_PAGE_e;

typedef enum
{
	INFORMATION_BTN_TETURN,
	INFORMATION_BTN_EMPTY,
	INFORMATION_BTN_DELETE,
	INFORMATION_BTN_NEXT,
	INFORMATION_BTN_PREVIOUS,
}INFORMATION_BTN_e;

typedef enum
{
	INFORMATION_MSG_BTN_CANCEL,
	INFORMATION_MSG_BTN_CONFIRM,
}INFORMATION_MSG_BTN_e;


bool informationLayerOnEnter(ITUWidget* widget, char* param);
bool informationLayerOnLeave(ITUWidget* widget, char* param);
bool informationMSGListClicked(ITUWidget* widget, char* param);
bool informationBtnOnClicked(ITUWidget* widget, char* param);
bool informationMsgBoxBtnOnClicked(ITUWidget* widget, char* param);
void informationLayerReset();

void informationReturnBtnOnClicked();
void informationPreMsgBtnOnClicked();
void informationMsgBoxShow(INFORMATION_BTN_e index);
void informationNextMsgBtnOnClicked();
void informationLayerInit(INFORMATION_PAGE_e page);
void setInformationContent(uint8_t index);
void setInformationContentTheme(char* themeStr);
void setInformationContentSender(char* senderStr);
void setInformationContentTime(char* timeStr);
void setInformationContentDetail(char* detailStr);
bool setInformationContentImage(char* imageStr);
void setInformationList();
bool setInformationListIsVisible(uint8_t index, bool status);
bool setInformationIsReaded(uint8_t index, INFORMATION_MSGICON_STATUS_e status);
bool setInformationListSender(uint8_t index, char* senderStr);
bool setInformationListTheme(uint8_t index, char* themeStr);
bool setInformationListTime(uint8_t index, char* timeStr);

#endif // GUI_INFORMATION_H