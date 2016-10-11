/*************************************************
Copyright (C),  2006-2016, Aurine
File name:		layer_call.c
Author:     	caogw
Version:
Date: 			2016-07-05
Description:
History:
1. Date:
Author:
Modification:
*************************************************/
#include "gui_include.h"

static ITUBackground*   calledRemoteBackground = NULL;


#if (CFG_CHIP_FAMILY == 9850) && (CFG_VIDEO_ENABLE)
void calledRemoteBackgroundDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha)
{
	int destx, desty;
	ITURectangle* rect = (ITURectangle*)&widget->rect;

	destx = rect->x + x;
	desty = rect->y + y;

	ituDrawVideoSurface(dest, destx, desty, rect->width, rect->height);
}
#endif


int BackgroundDrawVideo_init(char *background)
{
	if (background == NULL)
	{
		return FALSE;
	}
	int x, y, width, height;
	dprintf("background: %s\n", background);
	calledRemoteBackground = ituSceneFindWidget(&theScene, background);
	assert(calledRemoteBackground);
	#if (CFG_CHIP_FAMILY == 9850) && (CFG_VIDEO_ENABLE)        
	ituWidgetSetDraw(calledRemoteBackground, calledRemoteBackgroundDraw);
	#endif 

	// video
	ituWidgetGetGlobalPosition(calledRemoteBackground, &x, &y);
	width = ituWidgetGetWidth(calledRemoteBackground);
	height = ituWidgetGetHeight(calledRemoteBackground);

	#ifdef CFG_VIDEO_ENABLE     
	itv_set_video_window(x, y, width, height);
	#endif

	SceneEnterVideoState();
	return TRUE;
}

void BackgroundDrawVideo_exit(void)
{
	SceneLeaveVideoState();
}

