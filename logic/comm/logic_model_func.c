/*************************************************
  Copyright (C), 2006-2010, Aurine
  File name: 	logic_model_func.c
  Author:    	xiewr
  Version:   	1.0
  Date: 
  Description:  ��������
				
  History:        
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#include "logic_include.h"




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
		sys_close_lcd();
		// Ϊ�˱�֤����оƬģ����������������
		sleep(2); 
    }
	hw_set_lcd_pwm0(flag);
}



