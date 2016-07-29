/*************************************************
  Copyright (C), 2006-2016, Aurine
  File name: 	logic_timer.c
  Author:		chenbh
  Version:  	2.0
  Date: 
  Description:  ��ʱ��ģ��
				
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

#include "logic_timer.h"

//����ת��Ϊ�ꡢ�¡��ա�ʱ���֡���,��Localʱ��
//#define au_localtime(timep) gmtime(timep);   // �͵�ǰʱ����ʱ�� 8
#define au_localtime(timep) localtime(timep);  // ��ǰʱ��

#define	_THREAD_VERSIOR_		

#define	ALARM_TIME_MAX				60				//���ʱʱ��
#define	DEFAULT_TIMER_COUNT_MAX		1000			//Ĭ�����ʱ������
#define MSEC_10MS       			10				//��ʱʱ����

static PMY_TIMER g_timers;
static PMY_TIMER g_FreeTimers;

static int g_IsWaitCheck = 0;
static int g_CurTimerCount = 0;							//��ǰ��ʱ������
static int g_CurFreeCount = 0;							//��ǰ��Ҫ�ͷŵĽڵ���
static int g_MaxTimerCount =DEFAULT_TIMER_COUNT_MAX;	//���ʱ����������

static int g_TimerStart = FALSE;
static sem_t g_checktimerSem;		
static pthread_t g_timerThread;			//�����߳�
static pthread_mutex_t timerLock;

#define TIMER_LOCK()   pthread_mutex_lock(&timerLock)
#define TIMER_UNLOCK() pthread_mutex_unlock(&timerLock)  

static void free_node(PMY_TIMER node);
static int check_realtimer(PMY_TIMER curtimer);
static int check_abstimer(PMY_TIMER curtimer);
static int check_timer(PMY_TIMER curtimer);
static void *check_proc(void);
static uint32 get_id(void);

/*************************************************
  Function:    		free_node
  Description: 		�����ͷ��б��еȴ��ͷ�
  Input: 			
	1.node			Ҫ�ͷŵĽڵ�
  Output:			��
  Return:			��
  Others:
*************************************************/
static void free_node(PMY_TIMER node)
{
	if ((g_CurFreeCount+g_CurTimerCount)<g_MaxTimerCount)
	{
		node->next = g_FreeTimers;
		g_FreeTimers = node;
		g_CurFreeCount++;
	}
	else
	{
		free(node);
	}
}

/*************************************************
  Function:    		check_realtimer
  Description: 		�鿴��Զ�ʱ����ʱʱ���Ƿ񵽴�
  Input: 
  	1.curtimer      ��ʱ����ʱʱ��
  Output:			��
  Return:			��
  Others:
*************************************************/
static int check_realtimer(PMY_TIMER curtimer)
{
	curtimer->uRelCount++;
	int ret =0;
	if ( curtimer->uRelCount*MSEC_10MS >= curtimer->ulRelTmLen)
	{
		ret = 1;
	}
	if ( ret )
	{
		curtimer->uRelCount =0;
	}
	return ret;
}

/*************************************************
  Function:    		check_abstimer
  Description: 		�鿴���Զ�ʱʱ���Ƿ񵽴�
  Input: 
  	1.curtimer      ��ʱ����ʱʱ��
  Output:			��
  Return:			��
  Others:
*************************************************/
static int check_abstimer(PMY_TIMER curtimer)
{	
	time_t timep;
	time(&timep);
	struct tm *p = au_localtime(&timep);

	NT_BOOL ret  = FALSE;
	if ( (curtimer->stAbsTime.tm_year !=-1 && curtimer->stAbsTime.tm_year == p->tm_year)
		||  curtimer->stAbsTime.tm_year == -1)
	{
		ret =  TRUE;		
	}	
	else
		ret = FALSE;
	
	if ( ret && ( ( curtimer->stAbsTime.tm_yday !=-1 && curtimer->stAbsTime.tm_yday == p->tm_yday )
		||curtimer->stAbsTime.tm_yday ==-1 ) ) 
	{
		ret = TRUE;
	}
	else
		ret = FALSE;
	if ( ret && ( (curtimer->stAbsTime.tm_mon !=-1 && curtimer->stAbsTime.tm_mon == p->tm_mon)
		|| curtimer->stAbsTime.tm_mon ==-1 ) ) 
	{
		ret =  TRUE;
	}
	else
		ret = FALSE;

	if ( ret && ((curtimer->stAbsTime.tm_wday !=-1 && curtimer->stAbsTime.tm_wday == p->tm_wday ) 
		|| curtimer->stAbsTime.tm_wday ==-1 )) 
	{	
		ret = TRUE;
	}
	else
		ret = FALSE;
	
	if ( ret && (( curtimer->stAbsTime.tm_mday !=-1 && curtimer->stAbsTime.tm_mday == p->tm_mday )
		|| curtimer->stAbsTime.tm_mday==-1 ))
	{	
		ret =  TRUE;
	}	
	else
		ret = FALSE;
		
	if ( ret && (( curtimer->stAbsTime.tm_hour !=-1 && curtimer->stAbsTime.tm_hour == p->tm_hour )
		||  curtimer->stAbsTime.tm_hour ==-1 ))
	{
		ret =  TRUE;
	}	
	else 
		ret = FALSE;
		
	if ( ret && ((curtimer->stAbsTime.tm_min !=-1 && curtimer->stAbsTime.tm_min == p->tm_min )
		|| curtimer->stAbsTime.tm_min ==-1 )) 
	{
		ret =  TRUE;
	}
	else
		ret = FALSE;
		
	if ( ret && ( ( curtimer->stAbsTime.tm_sec !=-1 && curtimer->stAbsTime.tm_sec == p->tm_sec )
		|| curtimer->stAbsTime.tm_sec==-1 )) 
	{	
		ret =  TRUE;
	}
	else
		ret = FALSE;

	return ret;
}

/*************************************************
  Function:    		check_timer
  Description: 		�鿴��ʱ��ʱ���Ƿ񵽴�
  Input: 
  	1.curtimer      ��ʱ����ʱʱ��
  Output:			��
  Return:			��
  Others:
*************************************************/
static int check_timer(PMY_TIMER curtimer)
{
	int ret =0;
	switch ( curtimer->ulTmFlag )
	{
		case AU_REL_TIMER:
			ret = check_realtimer(curtimer);
			break;
			
		case AU_ABS_TIMER:
		{	
			ret = check_abstimer(curtimer);
			/*
			if (ret)            
			{					
				if ( curtimer->uRelCount!=0)   // ��ʵ��ʲô���� ??????????
				{	
					ret = FALSE;
					curtimer->uRelCount++;
				}	
				else 
				{
					if ( ++curtimer->uRelCount > (1000/MSEC_100MS))
						curtimer->uRelCount = 0;
				}	
			}
			*/
			break;
		}	
	}
	return ret;
}

#ifdef _THREAD_VERSIOR_
/*************************************************
  Function:    		can_check
  Description: 		���ÿɷ����ź���
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
static void can_check(void)
{
	//��Ʋ������źŵ�ֵԭ�ӵؼ�1����ʾ����һ���ɷ��ʵ���Դ
	if (g_IsWaitCheck)
	{	
		sem_post(&g_checktimerSem); 
	}	
}

/*************************************************
  Function:    		check_proc
  Description: 		��ʱִ�к���
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
static void *check_proc(void)
{
	while (1)
	{
		PMY_TIMER q = g_timers;
		if (!q)
		{
			g_IsWaitCheck = TRUE ;
			sem_wait(&g_checktimerSem);  			// �����������ȴ��������㷵��  sem_trywait(sem_t * sem)������
			g_IsWaitCheck = FALSE;	
		}	
		else			
		{		
			while (q)
			{
				if ( q->func && q->ulTmRunInfo == AU_TIMER_RUN  && check_timer(q)) 
				{
					(*(q->func)) (q->ID, q->param);
				}				
				q = q -> next;
			}	
		}	
		usleep(MSEC_10MS*1000);
	}
	pthread_exit(NULL);
}
#else
/*************************************************
  Function:    		check_proc
  Description: 		��ʱִ�к���
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
static void *check_proc(void)
{
	if (!g_timers)
	{	
		g_NextTime = 0; //û��������
		return NULL;
	}	

	if ( g_checktimering ) return NULL;
	
	g_checktimering = TRUE;
	
	PMY_TIMER q = g_timers;
	while (q)
	{
		if ( q->func && q->ulTmRunInfo == AU_TIMER_RUN  && check_timer(q)) 
		{
			(*(q->func)) (q->ID, q->param);
		}				
		
		q = q -> next;
	}
	
	g_checktimering = FALSE;
	return NULL;
}
#endif 

/*************************************************
  Function:    		get_id
  Description: 		�����һ����ʱ��ID
  Input: 			��
  Output:			��
  Return:			��ʱ��ID, ʧ�ܷ���0
  Others:
*************************************************/
static uint32 get_id(void)
{
	static uint32 id = 0;
	
	while(1)
	{
		if (id==0xFFFFFFFF)
		{
			id = 1;
		}
		else	
		{
			id++;
		}
		PMY_TIMER q = g_timers;
		int repeat = 0;
		//�ж��Ƿ��ظ�
		while (q)                             
		{
			if (q->ID == id) 
			{	
				repeat = 1;
				break;
			}	
			else
			{
				q = q -> next;
			}
		}
		if (!repeat)
			break;
	}
	return id;
}


/*************************************************
  Function:    		init_auTimer
  Description: 		��ʼ����ʱ������
  Input: 			
  Output:			��
  Return:			
  Others:
*************************************************/
int init_auTimer(void)
{
	#ifdef _THREAD_VERSIOR_
	pthread_mutex_init (&timerLock, NULL); 	
	sem_init(&g_checktimerSem, 0, 0) ;
	if (pthread_create(&g_timerThread, NULL, (void*)check_proc, NULL) == 0)
	{
		g_TimerStart = TRUE;
	}
	#endif 
    return TRUE;
}

/*************************************************
  Function:    		set_aurine_timer_count
  Description: 		���ö�ʱ���������
  Input: 			
	1.count			������
  Output:			��
  Return:			���óɹ�������
  Others:
*************************************************/
int set_aurine_timer_count(int count)
{
	if (count<0)
		return -1;
	//�ͷ�ʣ��Ľڵ�	
	if (count<g_MaxTimerCount && (g_CurTimerCount+g_CurFreeCount)>count)	
	{
		while(g_CurFreeCount > 0)
		{
			PMY_TIMER node = g_FreeTimers;
			g_FreeTimers = g_FreeTimers->next;
			free(node);
			g_CurFreeCount--;
		}
	}
	g_MaxTimerCount = count;
	return count;
}

/*************************************************
  Function:    		add_aurine_realtimer
  Description: 		����һ����ʱ��
  Input: 			
	1.when			��ʱʱ��, time_tֵ
	2.func			��ʱ��ִ�ж���
	3.param			��ʱ��ִ�ж�������
  Output:			��
  Return:			���ض�ʱ��ID
  Others:
*************************************************/
uint32 add_aurine_realtimer(uint32 speed, FTIMER_FUNC func, void * param)  
{
	if (g_TimerStart == FALSE) 
	{
		return 0;
	}
	//�����������
	if (g_CurTimerCount >= g_MaxTimerCount)
	{
		return 0;
	}
	//ʱ��Ƿ�
	if(speed <= 0)
	{
		return 0;
	}
	PMY_TIMER q;

	#ifdef _THREAD_VERSIOR_
		TIMER_LOCK();                             
	#else
		while (g_checktimering)
		{
			usleep(10);
		}	
		g_checktimering = TRUE;	
	#endif 
	
	if (g_FreeTimers)
	{
		q = g_FreeTimers;
		g_FreeTimers = g_FreeTimers->next;
	}
	else
	{
		//����һ���ڵ�
		q = malloc(sizeof(MY_TIMER));
		if (NULL==q)
		{
			#ifdef _THREAD_VERSIOR_
			TIMER_UNLOCK();
			#else		
			g_checktimering = FALSE;
			#endif 	
			dprintf("add_aurine_realtimer  NULL g_FreeTimers \n ");
			return 0;
		}	
	}
	q->ulTmFlag = AU_REL_TIMER;
	q->ID = get_id();
	q->ulRelTmLen = speed;
	q->func = func;
	q->param = param;
	q->ulTmAttr = AU_TIMER_LOOP;
	q->ulTmRunInfo = AU_TIMER_RUN;
	q->uRelCount= 0;
	g_CurTimerCount++;

	can_check();
	//�Ҹ�λ�ò���
	//��һλ
	q->next = g_timers;
	g_timers = q;
	#ifdef _THREAD_VERSIOR_
		TIMER_UNLOCK();
	#else		
		g_checktimering = FALSE;
	#endif 	
	return q->ID;

}

/*************************************************
  Function:    		add_aurine_abstimer
  Description: 		����һ����ʱ��
  Input: 			
	1.when			��ʱʱ��, time_tֵ
	2.func			��ʱ��ִ�ж���
	3.param			��ʱ��ִ�ж�������
  Output:			��
  Return:			���ض�ʱ��ID
  Others:
*************************************************/
uint32 add_aurine_abstimer(struct tm abstime, FTIMER_FUNC func, void * param)
{
	if (g_TimerStart == FALSE) 
	{
		return 0;
	}
	
	// �����������
	if (g_CurTimerCount >= g_MaxTimerCount)
	{
		return 0;
	}
	
	// ʱ��Ƿ�
	PMY_TIMER q;

	#ifdef _THREAD_VERSIOR_
		TIMER_LOCK();
	#else	
		while (g_checktimering)
		{
			usleep(10);
		}
		
		g_checktimering = TRUE;
	#endif 
	
	if (g_FreeTimers)
	{
		q = g_FreeTimers;
		g_FreeTimers = g_FreeTimers->next;
	}
	else
	{
		// ����һ���ڵ�
		q = malloc(sizeof(MY_TIMER));
		if (NULL==q)
		{
		#ifdef _THREAD_VERSIOR_
			TIMER_UNLOCK();
		#else		
			g_checktimering = FALSE;
		#endif 	
		return 0;
		}
	}
	q->ulTmFlag = AU_ABS_TIMER;
	q->ID = get_id();
	q->ulRelTmLen = 0;
	q->stAbsTime = abstime;
	q->func = func;
	q->param = param;
	q->ulTmAttr=AU_TIMER_LOOP;
	q->ulTmRunInfo = AU_TIMER_RUN;
	q->uRelCount= 0;
	g_CurTimerCount++;
	can_check();
	// �Ҹ�λ�ò���
	// ��һλ
	q->next = g_timers;
	g_timers = q;
	#ifdef _THREAD_VERSIOR_
		TIMER_UNLOCK();
	#else		
		g_checktimering = FALSE;
	#endif 	
	return q->ID;
}

/*************************************************
  Function:    		cancel_aurine_timer
  Description: 		ȡ��һ����ʱ��
  Input: 			
	1.when			��ʱ��ID
	2.func			��ʱ������Ҫִ�еĶ���
  Output:			��
  Return:			�Ƿ�ɹ� TRUE/FALSE
  Others:
*************************************************/
int cancel_aurine_timer(uint32 ID, FTIMER_FUNC func)
{	
	#ifdef _THREAD_VERSIOR_
		TIMER_LOCK();
	#else	
		while(g_checktimering)
		{
			usleep(10);
		}		
		g_checktimering = TRUE;
	#endif 	

	PMY_TIMER t, q;
	t = (PMY_TIMER)0;
	
	for (q = g_timers; q; q = q->next) 
	{
		if ( q->ID == ID )//&& q->func == func) 
		{
			if (t)
				t -> next = q -> next;
			else
				g_timers = q -> next;
			break;
		}
		t = q;
	}
	int ret = FALSE;
	//�ҵ�
	if (q) 
	{
		g_CurTimerCount--;
		free_node(q);
		ret = TRUE; 
	}
		
	#ifdef _THREAD_VERSIOR_
		TIMER_UNLOCK();
	#else
		g_checktimering = FALSE;
	#endif		
	return ret; 
}

/*************************************************
  Function:    		free_aurine_timer
  Description: 		������ж�ʱ��, �ɲ�ִ��
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
void free_aurine_timer(void)
{
	PMY_TIMER t;
	
	while (g_timers)
	{
		t = g_timers; 
		g_timers = g_timers->next;
		free(t);
	}
	g_CurTimerCount = 0;
	while (g_FreeTimers)
	{
		t = g_FreeTimers; 
		g_FreeTimers = g_FreeTimers->next;
		free(t);
	}
	g_CurFreeCount = 0;
}

