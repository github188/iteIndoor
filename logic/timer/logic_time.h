/*************************************************
  Copyright (C), 2006-2016, Aurine
  File name: 	logic_time.h
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

#ifndef _AU_TIME_H
#define _AU_TIME_H

#include "logic_include.h"
#ifdef  __cplusplus
extern "C"
{
#endif


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
void timer_reboot_control(uint8 flag);

/*************************************************
  Function:			init_reboot_timer
  Description: 		��ʱ������ʼ��
  Input: 		
  Output:		
  Return:		 
  Others:
*************************************************/
int32 init_reboot_timer(void);

/*************************************************
  Function:			init_reboot_timer
  Description: 		��ʱ������ʼ��
  Input: 		
  Output:		
  Return:		 
  Others:
*************************************************/
int32 test_reboot_timer(void);

#ifdef __cplusplus
};
#endif

#endif 





