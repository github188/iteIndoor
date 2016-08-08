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

#define NODISTURB_TIME				1				// û����һ��

static int g_NofaceTime	= 100;						// ����ų�ʱʱ��
static uint32 g_NofaceTimer = 0;					// ����Ŷ�ʱ��

/*************************************************
  Function:		disturb_timer_proc
  Description:  ����ż�ʱ��ʱ��
  Input:		��
  Output:		��
  Return:		��
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
  Description:  ��������Ŷ�ʱ��
  Input:		��
  Output:		��
  Return:		��
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

		// ���붨ʱ��
		g_NofaceTimer = add_aurine_realtimer((1000*NODISTURB_TIME), disturb_timer_proc, NULL);	
		dprintf("g_NofaceTimer : %d \n", g_NofaceTimer);
	}
}

/*************************************************
  Function:		stop_disturb_timer
  Description:  ֹͣ����Ŷ�ʱ��
  Input:		��
  Output:		��
  Return:		��
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


