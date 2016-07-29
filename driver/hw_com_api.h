/*************************************************
  Copyright (C), 2006-2016, Aurine
  File name: 	hw_com_api.c
  Author:    	luofl
  Version:   	2.0
  Date: 		2014-09-30
  Description:  串口驱动程序
  				串口1---用于调试和与单片机通信共用
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
  Description: 	启动串口喂狗
  Input: 		无
  Output:		无
  Return:		无
  Others:		
*************************************************/
int hw_start_com_dog(void);

/*************************************************
  Function:		init_alarm_callback
  Description:	初始化报警回调函数
  Input: 
	1.func		报警回调函数指针
  Output:
  Return:		成功 = 0，其它值失败
  Others:		报警8个防区端口状态回调,
  	防区端口状态有变化时,单片机会上报
*************************************************/
int32 init_alarm_callback(AlarmCallBack func);

/*************************************************
  Function:		send_485_pack
  Description: 	发送一帧数据到485
  Input: 
	1.			待发送数据
	2.			数据长度
  Output:		无
  Return:		实际发送长度
  Others:
*************************************************/
int8 send_485_pack(char * data, uint8 len);

/*************************************************
  Function:		init_jd_callbackt
  Description:	初始化家电回调函数
  Input: 
	1.func		家电回调函数指针
  Output:
  Return:		成功 = 0，其它值失败
  Others:		
*************************************************/
int32 init_jd_callbackt(JdCallBack func);

/*************************************************
  Function:     hw_set_lcd_pwm0
  Description:  设置单片机接管屏背光控制
  Input:       	
  	1.flag: 	TRUE: 关屏 FALSE: 开屏				
  Output:       无
  Return:       无
  Others:
*************************************************/
void hw_set_lcd_pwm0(uint8 flag);
#endif

