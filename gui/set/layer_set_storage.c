/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_storage.c
Author:     	zxc
Version:
Date: 		2016-07-28
Description: 存储管理界面
History:
1. Date:
Author:
Modification:
*************************************************/
#include "layer_set.h"

static ITUText* SetStorageMessage2Text = NULL;
static ITUText* SetStoragePic2Text = NULL;
static ITUText* SetStorageLyly2Text = NULL;
static ITURadioBox* MsgFailHintSuccess1RadioBox = NULL;


/*************************************************
Function:		show_win
Description: 	显示的具体信息
Input:		无
Output:		无
Return:		TRUE 成功 FALSE 失败
Others:
*************************************************/
static void  show_win()
{
	uint8 i = 0;
	char tmp[30];
	uint32 storage[3] = { 0, 0, 0 };
	ITUText* SetStorage2Text[3] = { SetStorageMessage2Text, SetStoragePic2Text, SetStorageLyly2Text };

	storage[0] = (storage_get_msg_allsize() / 1024);
	storage[1] = (storage_set_photo_size() / 1024);
	storage[2] = (storage_set_lylyrecord_size() / 1024);

	for (i = 0; i < 3; i++)
	{
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%d KB", storage[i]);
		ituTextSetString(SetStorage2Text[i], tmp);
	}
}

/*************************************************
Function:		SetStorageOnEnter
Description: 	进入存储管理设置界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetStorageOnEnter(ITUWidget* widget, char* param)
{
	if (!SetStorageMessage2Text)
	{
		SetStorageMessage2Text = ituSceneFindWidget(&theScene, "SetStorageMessage2Text");
		assert(SetStorageMessage2Text);

		SetStoragePic2Text = ituSceneFindWidget(&theScene, "SetStoragePic2Text");
		assert(SetStoragePic2Text);

		SetStorageLyly2Text = ituSceneFindWidget(&theScene, "SetStorageLyly2Text");
		assert(SetStorageLyly2Text);

		MsgFailHintSuccess1RadioBox = ituSceneFindWidget(&theScene, "MsgFailHintSuccess1RadioBox");
		assert(MsgFailHintSuccess1RadioBox);
	}

	if (strcmp(param, "MsgFailHintSuccessLayer") == 0)
	{
		if (ituRadioBoxIsChecked(MsgFailHintSuccess1RadioBox))	//确认键
		{
			storage_format_system();
			storage_set_lyly_tip(FALSE);
			// del by chenbh 
			//sys_sync_hint_state();
		}
	}

	show_win();

	return true;
}

/*************************************************
Function:		SetStorageCleanDataButtonOnMouseUp
Description: 	清空记录钮按下抬起执行函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetStorageCleanDataButtonOnMouseUp(ITUWidget* widget, char* param)
{
	ShowMsgFailHintSuccessLayer(1, SID_Msg_Format, 1);

	return true;
}


