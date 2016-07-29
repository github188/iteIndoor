/*************************************************
  Copyright (C), 2006-2010, Aurine
  File name:  	loigc_rtsp_monitor.h
  Author:     	xiewr
  Version:    	1.0
  Date: 
  Description:  监视头文件
  
  History:        
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#ifndef _LOGIC_RTSP_MONITOR_H_
#define _LOGIC_RTSP_MONITOR_H_


// RTSP 厂商ID
#define AURINE_STR			"AURINE"	
#define HIKVISION_STR		"HIKVISION"	
#define DAHUA_STR			"DAHUA"	
#define SANLI_STR			"SANLI"

// RTSP 视频类型
#define FMT_DEFAULT			0
#define H264				1
#define MPEG				2

#define H264_STR			"h264"
#define MPEG_STR			"mpeg4"

// RTSP 视频格式
#define DEC_DEFAULT			0
#define QCIF				1
#define QVGA				2
#define CIF					3
#define D1					4
#define HalfD1				5
#define VGA                 6
#define _720P               7

#define QCIF_STR			"QCIF"
#define QVGA_STR			"QVGA"
#define CIF_STR				"CIF"
#define D1_STR				"D1"
#define HalfD1_STR			"HalfD1"
#define VGA_STR				"VGA"
#define _720P_STR			"720P"

/*************************************************
  Function:		rtsp_monitor_state
  Description: 	
  Input: 		无
  Output:		无
  Return:		
  Others:		
*************************************************/
uint32 rtsp_monitor_state (void);

/*************************************************
  Function:			monitor_stop
  Description:		结束监视或通话
  Input: 	
  Output:			无
  Return:			成功与否, TRUE / FALSE
  Others:
*************************************************/
int32 rtsp_monitor_stop(void);

#ifdef _NEW_SELF_IPC_
/*************************************************
  Function:			init_ipc_state
  Description:		
  Input: 	
  Output:			无
  Return:			
  Others:			ipc状态初始化 默认离线状态
*************************************************/
void init_ipc_state(void);

/*************************************************
  Function:			ipc_ontimer
  Description:		
  Input: 	
  Output:			无
  Return:			
  Others:			ipc状态定时器
*************************************************/
void ipc_ontimer(void);

/*************************************************
  Function:			ipc_ontimer
  Description:		
  Input: 	
  	index			设备编号索引 0-9
  Output:			无
  Return:			0 离线 1 在线
  Others:			获得IPC在线状态
*************************************************/
uint8 get_ipc_state(uint8 index);

/*************************************************
  Function:			set_ipc_online
  Description:		
  Input: 	
  	index			设备编号索引 0-9
  Output:			无
  Return:			
  Others:			设置IPC在线状态
*************************************************/
void set_ipc_online(uint8 index, uint32 IP);

/*************************************************
  Function:			ipc_ontimer
  Description:		
  Input: 	
  	index			设备编号索引 0-9
  Output:			无
  Return:			
  Others:			ipc状态定时器
*************************************************/
void set_ipc_outline(uint8 index);
#endif

/*************************************************
  Function:			rtsp_distribute
  Description:		
  Input: 	
  	1.recPacket		接收包
  Output:			无
  Return:			成功与否, TRUE/FALSE
  Others:	
*************************************************/
int32 rtsp_distribute(const PRECIVE_PACKET recPacket);

/*************************************************
  Function:			rtsp_responsion
  Description:		
  Input: 	
  	1.recPacket		应答包
  	2.SendPacket	发送包
  Output:			无
  Return:			无
  Others:
*************************************************/
void rtsp_responsion(const PRECIVE_PACKET recPacket, const PSEND_PACKET SendPacket);
#endif
