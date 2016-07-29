/*************************************************
  Copyright (C), 2006-2016, Aurine
  File name: 	hw_com_api.c
  Author:    	luofl
  Version:   	2.0
  Date: 		2014-09-30
  Description:  ������������
  				����1---���ڵ��Ժ��뵥Ƭ��ͨ�Ź���
  History:        
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#ifndef __HW_COMM_API_H__
#define __HW_COMM_API_H__


typedef	void (* AlarmCallBack)(uint8 PortStatus);
typedef	void (* JdCallBack)(uint8 * buf);;



/*************************************************
  Function:		hw_start_com_dog
  Description: 	��������ι��
  Input: 		��
  Output:		��
  Return:		��
  Others:		
*************************************************/
int hw_start_com_dog(void);

/*************************************************
  Function:		init_alarm_callback
  Description:	��ʼ�������ص�����
  Input: 
	1.func		�����ص�����ָ��
  Output:
  Return:		�ɹ� = 0������ֵʧ��
  Others:		����8�������˿�״̬�ص�,
  	�����˿�״̬�б仯ʱ,��Ƭ�����ϱ�
*************************************************/
int32 init_alarm_callback(AlarmCallBack func);

/*************************************************
  Function:		send_485_pack
  Description: 	����һ֡���ݵ�485
  Input: 
	1.			����������
	2.			���ݳ���
  Output:		��
  Return:		ʵ�ʷ��ͳ���
  Others:
*************************************************/
int8 send_485_pack(char * data, uint8 len);

/*************************************************
  Function:		init_jd_callbackt
  Description:	��ʼ���ҵ�ص�����
  Input: 
	1.func		�ҵ�ص�����ָ��
  Output:
  Return:		�ɹ� = 0������ֵʧ��
  Others:		
*************************************************/
int32 init_jd_callbackt(JdCallBack func);

/*************************************************
  Function:     hw_set_lcd_pwm0
  Description:  ���õ�Ƭ���ӹ����������
  Input:       	
  	1.flag: 	TRUE: ���� FALSE: ����				
  Output:       ��
  Return:       ��
  Others:
*************************************************/
void hw_set_lcd_pwm0(uint8 flag);
#endif

