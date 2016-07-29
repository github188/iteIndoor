/*************************************************
  Copyright (C), 2006-2010, Aurine
  File name: 	logic_model_func.c
  Author:    	xiewr
  Version:   	1.0
  Date: 
  Description:  公共命令
				
  History:        
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#include "logic_include.h"




/*************************************************
  Function:     timer_reboot_control
  Description:  定时重启屏背光控制切换
  Input:        
    1.flag       1: 单片机控制屏背光 
                 0: N32926控制屏背光
  Output:       无
  Return:       无
  Others:   
  夜间重启时不能亮屏、不能响开机声音
  ，以免用户察觉异常。
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
		// 为了保证网络芯片模块重启后正常工作
		sleep(2); 
    }
	hw_set_lcd_pwm0(flag);
}



