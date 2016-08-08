/*************************************************
  Copyright (C), 2006-2016, Aurine
  File name: 	logic_timer.h
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

#ifndef _AU_TIMER_H
#define _AU_TIMER_H

#include <time.h>

#include "logic_include.h"

//#include "au_config.h"

#ifdef  __cplusplus
extern "C"
{
#endif


/*��ʱ��������Ϣ*/
/*��ʱ������*/
#define AU_ABS_TIMER  0x1
#define AU_REL_TIMER  0x2

/*��ʱ������״̬*/
#define AU_TIMER_RUN  0x1
#define AU_TIMER_STOP 0x2


//ִ�ж�����������ӿ�
typedef void *(*FTIMER_FUNC)(uint32, void *);

/*timer type*/
typedef enum _AU_E_TimerAttr
{
    AU_TIMER_LOOP  = 0,       /* the timer start loop */
    AU_TIMER_ONE_SHOOT ,      /* the timer start once and then stop */
    AU_TIMER_AUTO_FREE        /* the timer start once and then free */
}AU_E_TimerAttr;

/*��ʱ��ά����Ϣ���ݽṹ*/
typedef struct _MY_TIMER
{
    uint32 ID;		
    uint32 ulTmFlag; 	    /*���/���Զ�ʱ����־��VTOP_ABS_TIMERΪ���Զ�ʱ����VTOP_REL_TIMERΪ��Զ�ʱ�� */
    struct _MY_TIMER *next;	/*��ʱ��ָ��*/
    uint32 ulTmAttr;        /*��ʱ�����ͣ����ڣ�VTOP_TIMER_LOOP���Զ��ͷţ�VTOP_TIMER_AUTO_FREE��ֹͣ�����ͷţ�VTOP_TIMER_ONE_SHOOT */
    uint32 ulTmRunInfo;     /*��ʱ������״̬�����У�VTOP_TIMER_RUN��ֹͣ��VTOP_TIMER_STOP*/
    struct tm  stAbsTime;   /*���Զ�ʱ����ʱʱ��*/
    uint32 ulRelTmLen;		/*��Զ�ʱ��ʱ��*/
    uint32 uRelCount;       //��ʱ����
	FTIMER_FUNC func;		//��ʱִ�ж���
	void *param;			//ִ�в���
}MY_TIMER ,*PMY_TIMER;

/*************************************************
  Function:    		set_aurine_timer_count
  Description: 		��ʼ����ʱ������
  Input: 			
	1.count			������
  Output:			��
  Return:			0: ʧ��;>0�ɹ�
  Others:
*************************************************/
int init_auTimer(void);


/*************************************************
  Function:    		set_aurine_timer_count
  Description: 		���ö�ʱ������, Ĭ��ֵΪ2000
  Input: 			
	1.count			������
  Output:			��
  Return:			���óɹ�������
  Others:
*************************************************/
int set_aurine_timer_count(int count);


/*************************************************
  Function:    		add_aurine_timer
  Description: 		����һ����Զ�ʱ��
  Input: 			
	1.speed			��Զ�ʱʱ��,
	2.func			��ʱ��ִ�ж���
	3.param			��ʱ��ִ�ж�������
  Output:			��
  Return:			���ض�ʱ��ID
  Others:
*************************************************/
uint32 add_aurine_realtimer(uint32 speed, FTIMER_FUNC func, void * param);


/*************************************************
  Function:    		add_aurine_timer
  Description: 		����һ�����Զ�ʱ��
  Input: 			
	1.abstime		���Զ�ʱʱ��,
	2.func			��ʱ��ִ�ж���
	3.param			��ʱ��ִ�ж�������
  Output:			��
  Return:			���ض�ʱ��ID
  Others:
*************************************************/
uint32 add_aurine_abstimer(struct tm abstime, FTIMER_FUNC func, void * param);
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
int cancel_aurine_timer(uint32 *ID, FTIMER_FUNC func);


/*************************************************
  Function:    		free_aurine_timer
  Description: 		������ж�ʱ��, �ɲ�ִ��
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
void free_aurine_timer(void);

#ifdef __cplusplus
};
#endif

#endif /* !_AU_TIMER_H */

