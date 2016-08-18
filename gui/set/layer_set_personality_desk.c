/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_personality_desk.c
Author:     	zxc
Version:
Date: 		2016-07-31
Description:	�������汳��
*************************************************/
#include "layer_set.h"

#define MAX_DESK_INDEX			8

static ITUBackground* SetDeskPicBackground = NULL;
static ITUBackground* SetPersonalityPreviewBackground = NULL;
static ITURadioBox* SetPersonalityDeskRadioBox[MAX_DESK_INDEX] = { NULL };
static ITUIcon* SetPersonalityDeskIcon[MAX_DESK_INDEX] = { NULL };
static ITUIcon* SetPersonalityPreviewDownIcon = NULL;
static ITUSprite* SetPersonalityPreviewUpSprite = NULL;
static ITULayer* SetPersonalityLayer = NULL;

static uint8 g_desk_index = 0;

/*************************************************
Function:		ituIconLoadJpegStretchFile
Description:	ѹ��ͼƬ����icon�ؼ�������ʾ
Input:		��
Output:		��
Return:
Others:
*************************************************/
static void ituIconLoadJpegStretchFile(ITUIcon* icon, char* filepath)
{
	uint8_t* data;
	int size = 0;
	struct stat sb;

	FILE* fPJpegFile = fopen(filepath, "rb");
	if (fPJpegFile)
	{
		if (fstat(fileno(fPJpegFile), &sb) != -1)
		{
			size = sb.st_size;
			data = malloc(size);
			if (data)
			{
				size = fread(data, 1, size, fPJpegFile);
				ituIconLoadJpegData(icon, data, size);
				free(data);
			}
		}
		fclose(fPJpegFile);
	}
}

/*************************************************
Function:		show_win
Description: 	
Input:		��
Output:		��
Return:		
Others:
*************************************************/
static void show_win()
{
	uint8 tmp[50];
	uint8 i = 0;

	ituRadioBoxSetChecked(SetPersonalityDeskRadioBox[g_desk_index], true);

	for (i = 0; i < MAX_DESK_INDEX; i++)
	{
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%s%d%s", WALL_PAPER_DIR_PATH, "bk_", i, ".jpg");

		ituIconLoadJpegStretchFile(SetPersonalityDeskIcon[i], tmp);
	}
}

/*************************************************
Function:		SetPersonalityDeskOnEnter
Description: 	�����������ý����ʼ������
Input:		��
Output:		��
Return:		TRUE �� FALSE ��
Others:
*************************************************/
bool SetPersonalityDeskOnEnter(ITUWidget* widget, char* param)
{
	if (!SetDeskPicBackground)
	{
		char tmp[50];
		uint8 i = 0;

		for (i = 0; i < MAX_DESK_INDEX; i++)
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "SetPersonalityDeskIcon", i);
			SetPersonalityDeskIcon[i] = ituSceneFindWidget(&theScene, tmp);
			assert(SetPersonalityDeskIcon[i]);

			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "SetPersonalityDeskRadioBox", i);
			SetPersonalityDeskRadioBox[i] = ituSceneFindWidget(&theScene, tmp);
			assert(SetPersonalityDeskRadioBox[i]);
		}

		SetDeskPicBackground = ituSceneFindWidget(&theScene, "SetDeskPicBackground");
		assert(SetDeskPicBackground);

		SetPersonalityPreviewBackground = ituSceneFindWidget(&theScene, "SetPersonalityPreviewBackground");
		assert(SetPersonalityPreviewBackground);
	}

	g_desk_index = storage_get_desk();

	show_win();

	ituWidgetSetVisible(SetPersonalityPreviewBackground, false);
	ituWidgetSetVisible(SetDeskPicBackground, true);

	return true;
}

/*************************************************
Function:		SetPersonalityDeskRadioBoxOnMouseUp
Description: 	����ͼƬ�������´�����
Input:		��
Output:		��
Return:		TRUE �� FALSE ��
Others:
*************************************************/
bool SetPersonalityDeskRadioBoxOnMouseUp(ITUWidget* widget, char* param)
{
	int index = atoi(param);
	g_desk_index = (uint8)index;

	return true;
}

/*************************************************
Function:		RightPreviewButtonOnMouseUp
Description: 	Ԥ���������´�����
Input:		��
Output:		��
Return:		TRUE �� FALSE ��
Others:
*************************************************/
bool RightPreviewButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 tmp[50];

	if (!SetPersonalityPreviewDownIcon)
	{
		SetPersonalityPreviewDownIcon = ituSceneFindWidget(&theScene, "SetPersonalityPreviewDownIcon");
		assert(SetPersonalityPreviewDownIcon);

		SetPersonalityPreviewUpSprite = ituSceneFindWidget(&theScene, "SetPersonalityPreviewUpSprite");
		assert(SetPersonalityPreviewUpSprite);
	}

	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp, "%s%s%d%s", WALL_PAPER_DIR_PATH, "bk_", g_desk_index, ".jpg");
	ituIconLoadJpegStretchFile(SetPersonalityPreviewDownIcon, tmp);
	
	if (0 == g_desk_index)
	{
		ituSpriteGoto(SetPersonalityPreviewUpSprite, 0);
	}
	else
	{
		ituSpriteGoto(SetPersonalityPreviewUpSprite, 1);
	}
	ituWidgetSetVisible(SetDeskPicBackground, false);
	ituWidgetSetVisible(SetPersonalityPreviewBackground, true);

	return true;
}

/*************************************************
Function:		RightSetDeskButtonOnMouseUp
Description: 	��Ϊ���汳���������´�����
Input:		��
Output:		��
Return:		TRUE �� FALSE ��
Others:
*************************************************/
bool RightSetDeskButtonOnMouseUp(ITUWidget* widget, char* param)
{
	storage_set_desk(TRUE, g_desk_index);

	if (!SetPersonalityLayer)
	{
		SetPersonalityLayer = ituSceneFindWidget(&theScene, "SetPersonalityLayer");
		assert(SetPersonalityLayer);
	}

	if (!ituWidgetIsVisible(SetPersonalityLayer))
	{
		ituLayerGoto(SetPersonalityLayer);
	}

	return true;
}

/*************************************************
Function:		SetPersonalityPreviewButtonOnMouseUp
Description: 	�˳�Ԥ���������´�����
Input:		��
Output:		��
Return:		TRUE �� FALSE ��
Others:
*************************************************/
bool SetPersonalityPreviewButtonOnMouseUp(ITUWidget* widget, char* param)
{
	ituWidgetSetVisible(SetPersonalityPreviewBackground, false);
	ituWidgetSetVisible(SetDeskPicBackground, true);
	show_win();

	return true;
}
