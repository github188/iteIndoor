/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_devno.c
Author:     	zxc
Version:
Date: 		2016-07-11
Description:	�豸��Ž���
*************************************************/
#include "../layer_set.h"

typedef enum
{
	DEVICE_RULE = 0,	//0���豸���
	STAIR_LEN,			//1���ݺų���
	ROOM_LEN,			//2�����ų���
	RISER_ENABLE,		//3�����õ�Ԫ��
	RISER_LEN,			//4����Ԫ�ų���
	DEVICE_MAX,
}DEVICE_BTN_PAGE;

static ITUText* SetDevnoStr2Text = NULL;
static ITUText* SetDevnoStairLen2Text = NULL;
static ITUText* SetDevnoRoomLen2Text = NULL;
static ITUText* SetDevnoUseCellLen2Text = NULL;
static ITUText* SetDevnoCellLen2Text = NULL;
static ITUText* SetDevnoCellLen1Text = NULL;
static ITUSprite* SetDevnoUseCellLenSprite = NULL;
static ITUCalendar* SetDevnoCellLenContainer = NULL;
static ITULayer* SetProjectLayer = NULL;
static ITUTextBox* SetNumKeyBordTextBox = NULL;

static PFULL_DEVICE_NO g_devparam = NULL;
static DEVICE_BTN_PAGE g_button_num = DEVICE_MAX;

/*************************************************
Function:    		comepare_devno_rule
Description:		�Ƚ��豸��Ź���
Input:
1.rule			�豸��Ź���
Output:			��
Return:			0-�ɹ� 1-��Ԫ���ȷǷ� 2-�ݺų��ȷǷ� 3-���ų��ȷǷ�
4-�ܳ��Ȳ��ܴ���18
Others:
*************************************************/
static uint32 comepare_devno_rule(DEVICENO_RULE Rule)
{
	//�Ϸ��ж�
	if (Rule.CellNoLen > 2)
	{
		return 1;
	}
	if (Rule.StairNoLen < Rule.CellNoLen || Rule.StairNoLen > 9)
	{
		return 2;
	}
	if (Rule.RoomNoLen < 3 || Rule.RoomNoLen > 9)
	{
		return 3;
	}
	if ((Rule.StairNoLen + Rule.RoomNoLen) > 17)
	{
		return 4;
	}
	if (Rule.CellNoLen == 0)
	{
		Rule.UseCellNo = FALSE;
	}

	return 0;
}

/*************************************************
Function:    		comepare_devno_len
Description:		�ж��豸��ų����Ƿ���ȷ
Input:
1.devno			�豸���ַ���
Output:			��
Return:
0-�ɹ�
1-���ȷǷ�
2-���Ŷ�ȫΪ0
3-���ڻ���ű�����1-40֮��
Others:
*************************************************/
static uint32 comepare_devno_len(char* DevNo)
{
	char temp[20];
	int nlen = 0;

	if (NULL == DevNo)
	{
		return 1;
	}

	nlen = strlen(DevNo);
	if (nlen != g_devparam->Rule.StairNoLen + g_devparam->Rule.RoomNoLen + 1)
	{
		return 1;
	}

	strncpy(temp, DevNo + g_devparam->Rule.StairNoLen, g_devparam->Rule.RoomNoLen);
	temp[g_devparam->Rule.RoomNoLen] = 0;
	if (0 == atoi(temp))
	{
		return 2;
	}

	return 0;
}

/*************************************************
Function:		check_devno_param
Description: 	�ж��Ƿ�Ϸ�
Input:
1.hDlg
Output:		��
Return:		TRUE �� FALSE ��
Others:
*************************************************/
static uint8 check_devno_param(void)
{
	uint8 ret1, ret = TRUE;
	ret1 = FALSE;

	ret = comepare_devno_rule(g_devparam->Rule);
	switch (ret)
	{
	case 0:										// �ɹ�
		break;

	case 1:										// ��Ԫ���ȷǷ�
		ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_ERROR, SID_Set_Prj_Rule_Riser_Err, "SetDevnoLayer");
		break;

	case 2:										// �ݺų��ȷǷ�
		ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_ERROR, SID_Set_Prj_Rule_Stair_Err, "SetDevnoLayer");
		break;

	case 3:										// ���ų��ȷǷ�
		ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_ERROR, SID_Set_Prj_Rule_Room_Err, "SetDevnoLayer");
		break;

	case 4:										// �ܳ��Ȳ��ܴ���18
		ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_ERROR, SID_Set_Prj_Rule_RuleAll_Err, "SetDevnoLayer");
		break;
	}

	if (0 == ret)
	{
		ret1 = comepare_devno_len(g_devparam->DeviceNoStr);
		switch (ret1)
		{
		case 0:
			return TRUE;

		case 1:
			ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_ERROR, SID_Set_Prj_Rule_Len_Err, "SetDevnoLayer");
			break;

		case 2:
			ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_ERROR, SID_Set_Prj_Rule_RoomNo_Err, "SetDevnoLayer");
			break;
		}
	}
	return FALSE;
}

/*************************************************
Function:		save_devno_param
Description: 	�������
Input:
1.hDlg
Output:		��
Return:		TRUE �� FALSE ��
Others:
*************************************************/
static void save_devno_param(void)
{
	g_devparam->Rule.Subsection = (g_devparam->Rule.StairNoLen - g_devparam->Rule.CellNoLen) * 100 + g_devparam->Rule.CellNoLen * 10 + g_devparam->Rule.RoomNoLen;
	storage_save_devno(TRUE, g_devparam->Rule, g_devparam->DeviceNoStr);
	memcpy(g_devparam, storage_get_devparam(), sizeof(FULL_DEVICE_NO));
	net_change_comm_deviceno();
}

/*************************************************
Function:		PrjGotoSetDevnoOnEnterShow
Description: 	�������ý����ⲿģ�����ý�����ʾ����
Input:		��
Output:		��
Return:		TRUE �� FALSE ��
Others:
*************************************************/
static void PrjGotoSetDevnoOnEnterShow()
{
	char tmp[40];

	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp, "%s", g_devparam->DeviceNoStr);
	ituTextSetString(SetDevnoStr2Text, tmp);

	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp, "%d", g_devparam->Rule.StairNoLen);
	ituTextSetString(SetDevnoStairLen2Text, tmp);

	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp, "%d", g_devparam->Rule.RoomNoLen);
	ituTextSetString(SetDevnoRoomLen2Text, tmp);

	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp, "%d", g_devparam->Rule.CellNoLen);
	ituTextSetString(SetDevnoCellLen2Text, tmp);

	if (g_devparam->Rule.UseCellNo)
	{
		ituTextSetString(SetDevnoUseCellLen2Text, get_str(SID_Set_Prj_Rule_Riser_Used));
		ituSpriteGoto(SetDevnoUseCellLenSprite, 1);
		ituSetColor(&((ITUWidget*)SetDevnoCellLen2Text)->color, 255, 255, 255, 255);
		ituSetColor(&((ITUWidget*)SetDevnoCellLen1Text)->color, 255, 255, 255, 255);
	}
	else
	{
		ituTextSetString(SetDevnoUseCellLen2Text, get_str(SID_Set_Prj_Rule_Riser_Unused));
		ituSpriteGoto(SetDevnoUseCellLenSprite, 0);
		ituSetColor(&((ITUWidget*)SetDevnoCellLen2Text)->color, 255, 128, 138, 135);//���
		ituSetColor(&((ITUWidget*)SetDevnoCellLen1Text)->color, 255, 192, 192, 192);//��ɫ
		ituWidgetDisable(SetDevnoCellLenContainer);
	}
}

/*************************************************
Function:		KeyBordGotoSetDevnoOnEnterShow
Description: 	�������ý����ⲿģ�����ý�����ʾ����
Input:		��
Output:		��
Return:		TRUE �� FALSE ��
Others:
*************************************************/
static void KeyBordGotoSetDevnoOnEnterShow()
{
	char tmp[40];
	uint8 nlen = 0;
	uint32 text = 0;
	char * get_keybord_text = NULL;
	uint32 text_id[5] = { SID_Set_Prj_Rule_Device_Wrong, SID_Set_Prj_Rule_Stair_Null, SID_Set_Prj_Rule_Room_Null, 0, SID_Set_Prj_Rule_Riser_Null };

	get_keybord_text = ituTextGetString(SetNumKeyBordTextBox);
	nlen = strlen(get_keybord_text);
	text = (uint8)atoi(get_keybord_text);

	switch (g_button_num)
	{
	case DEVICE_RULE:
		if (nlen > 0)
		{
			memset(g_devparam->DeviceNoStr, 0, sizeof(g_devparam->DeviceNoStr));
			memcpy(g_devparam->DeviceNoStr, get_keybord_text, nlen);

			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s", g_devparam->DeviceNoStr);
			ituTextSetString(SetDevnoStr2Text, tmp);
		}
		break;

	case STAIR_LEN:
		if (nlen > 0)
		{
			g_devparam->Rule.StairNoLen = text;

			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%d", g_devparam->Rule.StairNoLen);
			ituTextSetString(SetDevnoStairLen2Text, tmp);
		}
		break;

	case ROOM_LEN:
		if (nlen > 0)
		{
			g_devparam->Rule.RoomNoLen = text;

			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%d", g_devparam->Rule.RoomNoLen);
			ituTextSetString(SetDevnoRoomLen2Text, tmp);
		}
		break;

	case RISER_LEN:
		if (nlen > 0)
		{
			g_devparam->Rule.CellNoLen = text;

			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%d", g_devparam->Rule.CellNoLen);
			ituTextSetString(SetDevnoCellLen2Text, tmp);
		}
		break;
	}

	if (0 >= nlen)
	{
		ShowMsgFailHintSuccessLayer(HIT_SPRITE_TO_ERROR, text_id[g_button_num], "SetDevnoLayer");
	}
}

/*************************************************
Function:		SetDevnoOnEnter
Description: 	�����豸������ý����ʼ������
Input:		��
Output:		��
Return:		TRUE �� FALSE ��
Others:
*************************************************/
bool SetDevnoOnEnter(ITUWidget* widget, char* param)
{
	if (!SetDevnoStr2Text)
	{
		SetDevnoStr2Text = ituSceneFindWidget(&theScene, "SetDevnoStr2Text");
		assert(SetDevnoStr2Text);

		SetDevnoStairLen2Text = ituSceneFindWidget(&theScene, "SetDevnoStairLen2Text");
		assert(SetDevnoStairLen2Text);

		SetDevnoRoomLen2Text = ituSceneFindWidget(&theScene, "SetDevnoRoomLen2Text");
		assert(SetDevnoRoomLen2Text);

		SetDevnoUseCellLen2Text = ituSceneFindWidget(&theScene, "SetDevnoUseCellLen2Text");
		assert(SetDevnoUseCellLen2Text);

		SetDevnoCellLen2Text = ituSceneFindWidget(&theScene, "SetDevnoCellLen2Text");
		assert(SetDevnoCellLen2Text); 

		SetDevnoCellLen1Text = ituSceneFindWidget(&theScene, "SetDevnoCellLen1Text");
		assert(SetDevnoCellLen1Text);

		SetDevnoUseCellLenSprite = ituSceneFindWidget(&theScene, "SetDevnoUseCellLenSprite");
		assert(SetDevnoUseCellLenSprite); 

		SetDevnoCellLenContainer = ituSceneFindWidget(&theScene, "SetDevnoCellLenContainer");
		assert(SetDevnoCellLenContainer); 

		SetProjectLayer = ituSceneFindWidget(&theScene, "SetProjectLayer");
		assert(SetProjectLayer); 

		SetNumKeyBordTextBox = ituSceneFindWidget(&theScene, "SetNumKeyBordTextBox");
		assert(SetNumKeyBordTextBox);  
	}

	if (strcmp(param, "SetNumKeyBordLayer") == 0)
	{
		KeyBordGotoSetDevnoOnEnterShow();

		check_devno_param();
	}
	else if(strcmp(param, "SetProjectLayer") == 0)
	{
		g_button_num = DEVICE_MAX;

		if (g_devparam)
		{
			free(g_devparam);
			g_devparam = NULL;
		}
		g_devparam = (PFULL_DEVICE_NO)malloc(sizeof(FULL_DEVICE_NO));
		if (NULL == g_devparam)
		{
			return FALSE;
		}
		else
		{
			memset(g_devparam, 0, sizeof(FULL_DEVICE_NO));
		}
		memcpy(g_devparam, storage_get_devparam(), sizeof(FULL_DEVICE_NO));

		PrjGotoSetDevnoOnEnterShow();
	}

	return true;
}

/*************************************************
Function:		SetDevnoListButtonOnMouseUp
Description: 	�豸����б������´�����
Input:		��
Output:		��
Return:		TRUE �� FALSE ��
Others:
*************************************************/
bool SetDevnoListButtonOnMouseUp(ITUWidget* widget, char* param)
{
	char tmp[40];
	int index = atoi(param);
	g_button_num = index;

	switch (g_button_num)
	{
	case DEVICE_RULE:
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s", g_devparam->DeviceNoStr);
		KeybordLayerOnShow(NULL, PASS_TYPE_MAX, 16, EXPRESS_CHAR, CANCEL_BTN, tmp, "SetDevnoLayer");
		break;

	case STAIR_LEN:
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%d", g_devparam->Rule.StairNoLen);
		KeybordLayerOnShow(NULL, PASS_TYPE_MAX, 1, EXPRESS_CHAR, CANCEL_BTN, tmp, "SetDevnoLayer");
		break;

	case ROOM_LEN:
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%d", g_devparam->Rule.RoomNoLen);
		KeybordLayerOnShow(NULL, PASS_TYPE_MAX, 1, EXPRESS_CHAR, CANCEL_BTN, tmp, "SetDevnoLayer");
		break;

	case RISER_LEN:
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%d", g_devparam->Rule.CellNoLen);
		KeybordLayerOnShow(NULL, PASS_TYPE_MAX, 1, EXPRESS_CHAR, CANCEL_BTN, tmp, "SetDevnoLayer");
		break;

	case RISER_ENABLE:
		if (g_devparam->Rule.UseCellNo)
		{
			g_devparam->Rule.UseCellNo = 0;
			ituTextSetString(SetDevnoUseCellLen2Text, get_str(SID_Set_Prj_Rule_Riser_Unused));
			ituSpriteGoto(SetDevnoUseCellLenSprite, 0);
			ituSetColor(&((ITUWidget*)SetDevnoCellLen1Text)->color, 255, 128, 138, 135);//���
			ituSetColor(&((ITUWidget*)SetDevnoCellLen2Text)->color, 255, 192, 192, 192);//��ɫ
			ituWidgetDisable(SetDevnoCellLenContainer);
		}
		else
		{
			g_devparam->Rule.UseCellNo = 1;
			ituTextSetString(SetDevnoUseCellLen2Text, get_str(SID_Set_Prj_Rule_Riser_Used));
			ituSpriteGoto(SetDevnoUseCellLenSprite, 1);
			ituSetColor(&((ITUWidget*)SetDevnoCellLen2Text)->color, 255, 255, 255, 255);
			ituSetColor(&((ITUWidget*)SetDevnoCellLen1Text)->color, 255, 255, 255, 255);
			ituWidgetEnable(SetDevnoCellLenContainer);
		}
		return true;
	}

	return true;
}

/*************************************************
Function:		SetDevnoLayerOnReturn
Description: 	�ײ��˳�������ִ�к���
Input:		��
Output:		��
Return:		TRUE �� FALSE ��
Others:
*************************************************/
void SetDevnoLayerOnReturn(void)
{
	uint8 ret = FALSE;

	if (!ituWidgetIsVisible(SetProjectLayer))
	{
		ret = check_devno_param();
		if (TRUE == ret)
		{
			save_devno_param();

			ituLayerGoto(SetProjectLayer);
			return;
		}
	}
}
