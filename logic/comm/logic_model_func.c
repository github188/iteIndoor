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

#define NODISTURB_TIME				1				// 没秒检测一次

static int g_NofaceTime	= 100;						// 免打扰超时时间
static uint32 g_NofaceTimer = 0;					// 免打扰定时器

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
		dprintf("g_NofaceTimer : %d \n", g_NofaceTimer);
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


