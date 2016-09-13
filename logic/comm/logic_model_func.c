/*************************************************
  Copyright (C), 2006-2010, Aurine
  File name: 	logic_model_func.c
  Author:    	chenbh 
  Version:   	1.0
  Date: 
  Description:  
				
  History:        
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#include "logic_include.h"

#define MAXLENGTH					256
#define NODISTURB_TIME				1				// 没秒检测一次

static int g_NofaceTime	= 100;						// 免打扰超时时间
static uint32 g_NofaceTimer = 0;					// 免打扰定时器

static MMP_WCHAR  gpDestText[MAXLENGTH];

/*************************************************
  Function:		disturb_timer_proc
  Description:  免打扰记时定时器
  Input:		无
  Output:		无
  Return:		无
  Others:		
*************************************************/
void* disturb_timer_proc(uint32 ID, void * arg)
{
	if (storage_get_noface())
	{
		if (g_NofaceTime > 0)
		{
			g_NofaceTime--;			
		}
		else
		{
			storage_set_noface_enable(FALSE);
			cancel_aurine_timer(&g_NofaceTimer, NULL);	
			g_NofaceTimer = 0;
		}
	}
	else
	{
		if (g_NofaceTimer)
		{
			cancel_aurine_timer(&g_NofaceTimer, NULL);	
			g_NofaceTimer = 0;
		}
	}
}

/*************************************************
  Function:		start_disturb_timer
  Description:  开启免打扰定时器
  Input:		无
  Output:		无
  Return:		无
  Others:		
*************************************************/
void start_disturb_timer(void)
{
	g_NofaceTime = storage_get_noface_time();
	if (storage_get_noface())
	{
		if (g_NofaceTimer)
		{
			cancel_aurine_timer(&g_NofaceTimer, NULL);	
			g_NofaceTimer = 0;
		}

		// 三秒定时器
		g_NofaceTimer = add_aurine_realtimer((1000*NODISTURB_TIME), disturb_timer_proc, NULL);	
		dprintf("g_NofaceTimer : %d , g_NofaceTime: %d\n", g_NofaceTimer, g_NofaceTime);
	}
}

/*************************************************
  Function:		stop_disturb_timer
  Description:  停止免打扰定时器
  Input:		无
  Output:		无
  Return:		无
  Others:		
*************************************************/
void stop_disturb_timer(void)
{
	if (g_NofaceTimer)
	{
		cancel_aurine_timer(&g_NofaceTimer, NULL);	
		g_NofaceTimer = 0;
	}
}


/*************************************************
Function:		gb2312ToUtf8
Description:	字符编码转换
Input:
1.ptDestText	目标字符串
2.nDestLength	长度
3.ptSrcText		源字符串
4.nSrcLength	长度

Output:			无
Return:			无
Others:
*************************************************/
void gb2312ToUtf8(char*  ptDestText, int  nDestLength, char*  ptSrcText, int  nSrcLength)
{
	int  nResult = 0;
	int  i;
	int  nTemp, nTemp1;

	if (!ptDestText
		|| !ptSrcText
		|| nDestLength <= 0
		|| nSrcLength <= 0
		)
		return -1;

	nTemp = 0;
	nTemp1 = 0;
	memset(gpDestText, 0, sizeof(gpDestText));

	for (i = 0; i<(nSrcLength); i++, nTemp1++)
	{
		//nResult = gb2312_mbtowc(0, &gpDestText[nTemp1], &ptSrcText[i], 2);
		if ((ptSrcText[i] >= 0x20) && (ptSrcText[i] <= 0x80))
		{
			gpDestText[nTemp1] = ptSrcText[i];
		}
		else  if (gb2312_mbtowc(0, &gpDestText[nTemp1], &ptSrcText[i], 2) == 2)
		{
			i++;
			nTemp++;
		}

	}
	gpDestText[(nSrcLength)-nTemp] = 0;
	nResult = 0;
	Unicode32ToUtf8(gpDestText, sizeof(gpDestText), ptDestText, nDestLength);

}