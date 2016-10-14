/*********************************************************
Copyright (C), 2015-2020, Aurine
File name:	layer_screensaver.c
Author:		chenbh
Version:   	1.0
Date:		2016-09-08
Description: 屏幕保护
History:
1. Date:
Author:
Modification:
2. ...
*********************************************************/

#include "layer_screensaver.h"

#define PER_PICT_TIME		10000

static FileList *g_List = NULL;
static FileList *g_CurNode = NULL;
static uint8 g_SaverPictCounts = 0;
static uint32 ScreenSaverLastTimeTick = 0;
static ITUIcon* ScreenSaverIcon = NULL;
static ITULayer* ScreenSaverLayer = NULL;

/*************************************************
Function:		ituLoadJpegFileOnIcon
Description:	压缩图片并在icon控件里面显示
Input:		无
Output:		无
Return:
Others:
*************************************************/
static void ituLoadJpegFileOnIcon(ITUIcon* icon, char* filepath)
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

bool ScreenSaverLayerOnEnter(ITUWidget* widget, char* param)
{
	ScreenSaverLastTimeTick = SDL_GetTicks();
	g_SaverPictCounts = get_filelist(SCREEN_SAVER_DIR_PATH, &g_List);
	printf("g_SaverPictCounts: %d\n", g_SaverPictCounts);
	if (g_SaverPictCounts == 0 || g_List == NULL)
	{
		return false;
	}
	if (!ScreenSaverIcon)
	{
		ScreenSaverIcon = ituSceneFindWidget(&theScene, "ScreenSaverIcon");
		assert(ScreenSaverIcon);
	}
	
	g_CurNode = g_List;
	ituLoadJpegFileOnIcon(ScreenSaverIcon, g_CurNode->filename);
}

bool ScreenSaverLayerOnTimer(ITUWidget* widget, char* param)
{	
	if (g_SaverPictCounts > 0)
	{	
		uint32_t duration;
		uint32_t curtime = SDL_GetTicks();
		if (curtime >= ScreenSaverLastTimeTick)
		{
			duration = curtime - ScreenSaverLastTimeTick;
		}
		else
		{
			duration = 0xFFFFFFFF - ScreenSaverLastTimeTick + curtime;
		}

		if (duration >= PER_PICT_TIME)
		{
			g_CurNode = g_CurNode->next;
			if (g_CurNode == NULL)
			{
				g_CurNode = g_List;
			}
			ituLoadJpegFileOnIcon(ScreenSaverIcon, g_CurNode->filename);
			ScreenSaverLastTimeTick = curtime;
			return true;
		}
	}
	
	return false;
}

/*************************************************
Function:		ScreenSaverLayerInit
Description: 
Input:
Output:
Return:
Others:
*************************************************/
void ScreenSaverLayerInit(void)
{	
	if (!ScreenSaverLayer)
	{
		ScreenSaverLayer = ituSceneFindWidget(&theScene, "ScreenSaverLayer");
		assert(ScreenSaverLayer);
	}	

	if (!ituWidgetIsVisible(ScreenSaverLayer))
	{
		ituLayerGoto(ScreenSaverLayer);
	}	
}
