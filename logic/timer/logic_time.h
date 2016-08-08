/*************************************************
  Copyright (C), 2006-2016, Aurine
  File name: 	logic_time.h
  Author:		chenbh
  Version:  	1.0
  Date: 		2014-12-11
  Description:  时钟处理
				
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
void timer_reboot_control(uint8 flag);

/*************************************************
  Function:			init_reboot_timer
  Description: 		定时重启初始化
  Input: 		
  Output:		
  Return:		 
  Others:
*************************************************/
int32 init_reboot_timer(void);

/*************************************************
  Function:			init_reboot_timer
  Description: 		定时重启初始化
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





