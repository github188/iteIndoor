/*********************************************************
  Copyright (C), 2006-2010, Aurine
  File name:	logic_model_func.h
  Author:   	
  Version:   	
  Date: 		
  Description:  公共命令
  
  History:                   
    1. Date:			 
       Author:				
       Modification:	
    2. ...
*********************************************************/
#ifndef __LOGIC_MODEL_FUNC_H__
#define __LOGIC_MODEL_FUNC_H__


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
void timer_reboot_control(uint8 flag);



#endif
