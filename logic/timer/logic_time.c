/*************************************************
  Copyright (C), 2006-2016, Aurine
  File name: 	logic_time.c
  Author:		chenbh
  Version:  	1.0
  Date: 		2014-12-11
  Description:  ʱ�Ӵ���
				
  History:        
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#include <stdio.h> 
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <semaphore.h>
#include <pthread.h>

#include "logic_include.h"

#define MAX_RE_TIMER	(3*60*60)                // �������ʱ���� 2�㵽5��

static struct tm  g_reboot_timer;
static uint32 g_RebootTimerID = 0;              // ��ʱ��ID

/*************************************************
  Function:		CouldBeReboot
  Description: 	�ж��Ƿ��������
  Input:		
  Output:		��
  Return:		TRUE �� FALSE ��
  Others:		
*************************************************/
static int32 CouldBeReboot(void)
{
	#if 0
	if (SYS_LCD_CLOSE == sys_get_lcd_state()) 		// �،��²ſ�������
	{
		return TRUE;
	}
	#else
	if (SYS_MEDIA_NONE == sys_get_media_state())
	{
		return TRUE;
	}	
	#endif

	return FALSE;
}

/*************************************************
  Function:     timer_reboot_control
  Description:  ��ʱ��������������л�
  Input:        
    1.flag       1: ��Ƭ������������ 
                 0: N32926����������
  Output:       ��
  Return:       ��
  Others:   
  ҹ������ʱ���������������쿪������
  �������û�����쳣��
*************************************************/
void timer_reboot_control(uint8 flag)
{	
    if(flag == TRUE)
    {
    	storage_set_isOpenScreen(TRUE);
    }
    else
    {
    	storage_set_isOpenScreen(FALSE);
		ScreenOff();
		// Ϊ�˱�֤����оƬģ����������������
		//sleep(2); 
    }
	hw_set_lcd_pwm0(flag);
}

/*************************************************
  Function:		set_reboot_timer
  Description: 	��������ʱ��
  Input: 		
  	ptimer	:   ����ʱ�� 	
  Output:		��
  Return:		��
  Others:
*************************************************/
static void set_reboot_timer(struct tm timer)
{
	g_reboot_timer.tm_hour = timer.tm_hour;
	g_reboot_timer.tm_min  = timer.tm_min;
	g_reboot_timer.tm_sec  = timer.tm_sec;
}

/*************************************************
  Function:			deal_reboot_timer_proc
  Description: 		��ʱ����
  Input: 		
  Output:		
  Return:		 
  Others:
*************************************************/
static void * deal_reboot_timer_proc(uint32 cmd, void * arg)
{
	// �ж��Ƿ��������
	if (CouldBeReboot())
	{
		#ifdef _TIMER_REBOOT_NO_LCD_
        timer_reboot_control(TRUE);
		#else
		hw_stop_feet_dog();							// �õ�Ƭ��������
		#endif
	}
	else
	{
		// ���������,������Ļ���ڲ���,���ӳ�20����������
		struct tm timer;
		timer.tm_sec = g_reboot_timer.tm_sec;
		timer.tm_min = (g_reboot_timer.tm_min + 20)%60;
		timer.tm_hour = g_reboot_timer.tm_hour + (g_reboot_timer.tm_min + 20)/60; 

		cancel_aurine_timer(&g_RebootTimerID, NULL); // ȡ��ԭ����ʱ��
		g_RebootTimerID = 0;
		
		// �賿5���û������,ǿ������
		if (timer.tm_hour > 5 || (timer.tm_hour == 5 && timer.tm_min > 0) || timer.tm_hour < 2) 
		{			
			//init_reboot_timer();
			hw_stop_feet_dog();
		}
		else
		{
			set_reboot_timer(timer);
			g_RebootTimerID = add_aurine_abstimer(timer, deal_reboot_timer_proc, NULL);
		}
	}
	
	return NULL;
}

/*************************************************
  Function:			init_reboot_timer
  Description: 		��ʱ������ʼ��
  Input: 		
  Output:		
  Return:		 
  Others:
*************************************************/
int32 init_reboot_timer(void)
{
	uint32 timerdiff;
	uint8 mac[6] = {0};
	uint64 MacInt64 = 0;
	uint32 ip = storage_get_netparam_bytype(HOST_IPADDR);
	
	memcpy(mac, storage_get_mac(HOUSE_MAC), 6);
	MacInt64 = mac[5]|((uint64)mac[4]<<8)|((uint64)mac[3]<<16)|
		((uint64)mac[2]<<24)|((uint64)mac[1]<<32)|((uint64)mac[0]<<40);
	srand((unsigned)ip|MacInt64);
	timerdiff = rand()%MAX_RE_TIMER;
	dprintf("init_reboot_timer  timerdiff : %d\n", timerdiff);

	//  ����ʱ��Ϊ2 �㵽5 �����ʱ��
	memset(&g_reboot_timer, -1, sizeof(struct tm));  // -1 ��ʾ��ʱ���в����Ƚ�
	g_reboot_timer.tm_hour = 2 + timerdiff/(60*60);
	g_reboot_timer.tm_min  = (0 + timerdiff/60)%60;
	g_reboot_timer.tm_sec  = 0 + timerdiff%60;
	printf("reboot time : %02d:%02d:%02d\n", g_reboot_timer.tm_hour, g_reboot_timer.tm_min, g_reboot_timer.tm_sec);

	g_RebootTimerID = add_aurine_abstimer(g_reboot_timer, deal_reboot_timer_proc, NULL);
	if (g_RebootTimerID == 0)
	{
		printf("add_aurine_abstimer return false !!!!!!\n");
		return FALSE;
	}

	return TRUE;
}

/*************************************************
  Function:			init_reboot_timer
  Description: 		��ʱ������ʼ��
  Input: 		
  Output:		
  Return:		 
  Others:
*************************************************/
int32 test_reboot_timer(void)
{
	//  ���Ե�����
	memset(&g_reboot_timer, -1, sizeof(struct tm));  // -1 ��ʾ��ʱ���в����Ƚ�
	g_reboot_timer.tm_hour = 5;
	g_reboot_timer.tm_min  = 5;
	g_reboot_timer.tm_sec  = 0;
	dprintf("reboot time : %02d:%02d:%02d\n", g_reboot_timer.tm_hour, g_reboot_timer.tm_min, g_reboot_timer.tm_sec);

	g_RebootTimerID = add_aurine_abstimer(g_reboot_timer, deal_reboot_timer_proc, NULL);
	if (g_RebootTimerID == 0)
	{
		dprintf("add_aurine_abstimer return false !!!!!!\n");
		return FALSE;
	}

	return TRUE;
}

