/*********************************************************
  Copyright (C), 2006-2010, Aurine
  File name:	logic_pubcmd.h
  Author:   	xiewr
  Version:   	
  Date: 		
  Description:  公共命令
  
  History:                   
    1. Date:			 
       Author:				
       Modification:	
    2. ...
*********************************************************/
#ifndef __LOGIC_PUBCMD_H__
#define __LOGIC_PUBCMD_H__

#include "logic_include.h"


extern RP_EVENT_SERVER_INFO g_Event_Server;


/*************************************************
  Function:			init_rtsp_timer
  Description:		
  Input: 	
  Output:			无
  Return:			
  Others:
*************************************************/
void init_rtsp_timer(void);

/*************************************************
  Function:    		logic_reg_center_state
  Description:		获取终端注册状态
  Input:			
  Output:			
  Return:			
  Others:			
*************************************************/
int32 logic_reg_center_state(void);

/*************************************************
  Function:    		logic_reg_center_ini
  Description:		终端注册
  Input:			
  Output:			
  Return:			
  Others:		MAC 码+旧设备编号+硬件版本标识（64B）+软件版本标识（64B）+本机设备名称（32字节，’\0’结尾					
*************************************************/
void logic_reg_center_ini(void);

/*************************************************
  Function:    		logic_reg_center_ini_again
  Description:		重新向服务器注册
  Input:			
  Output:			
  Return:			
  Others:			
*************************************************/
void logic_reg_center_ini_again(void);

/*************************************************
  Function:			public_distribute
  Description:		公共命令接收包回调函数
  Input: 	
  	1.recPacket		接收包
  Output:			无
  Return:			成功与否, TRUE/FALSE
  Others:	
*************************************************/
int32 public_distribute(const PRECIVE_PACKET recPacket);

/*************************************************
  Function:			public_responsion
  Description:		公共命令接收应答包处理函数
  Input: 	
  	1.recPacket		应答包
  	2.SendPacket	发送包
  Output:			无
  Return:			无
  Others:
*************************************************/
void public_responsion(const PRECIVE_PACKET recPacket, const PSEND_PACKET SendPacket);

#endif

