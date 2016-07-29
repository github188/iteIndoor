/*************************************************
  Copyright (C), 2006-2010, Aurine
  File name:  	logic_udp_comm.h
  Author:     	xiewr
  Version:    	1.0
  Date: 
  Description:  UDP通信头文件
  
  History:        
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#ifndef _NET_UDP_COMM_H_
#define _NET_UDP_COMM_H_
#include "logic_include.h"

#define	USE_RECIVE_THREAD		1					// 使用线程接收网络数据

#if USE_RECIVE_THREAD
//#define	MB_NETRECV				MBCom			// 网络接收msbox
//#define TPRI_NETRECV			TPRI_COM
#else
#define	UDP_RECV_TIMER_TICKS	2					// 使用定时器接收网络数据
#endif
#if 0
/***************************以下为网络包定义**************************/
// 网络发送包
typedef struct SEND_PACKET_NODE
{
	struct SEND_PACKET_NODE * next;
	struct SEND_PACKET_NODE * prev;
	char SendState;									// 发送状态, 见TSendState
	char SendTimes; 								// 当前发送次数
	char SendTimesMax; 								// 最多发送次数
	int TimeOut;									// 发送超时时间, 秒
	int WaitCount; 									// 等待时间计数
	uint32 address;									// 对方IP地址
	uint16 port;									// 对方端口号
	int size;										// 数据包大小
	char * data;									// 发送数据
}SEND_PACKET, *PSEND_PACKET;

// 网络接收包
typedef struct RECIVE_PACKET_NODE
{
	struct RECIVE_PACKET_NODE * next;
	uint32 address;									// 对方IP地址
	uint16 port;									// 对方端口号
	int size;										// 数据包大小
	char * data;									// 发送数据
}RECIVE_PACKET, *PRECIVE_PACKET;
/***************************以上为网络包定义**************************/
#endif
// 接收包处理函数定义,用于特殊命令处理定义, 参数: 接收包
typedef void (*PFRECIVE_PACKET_PROC)(NET_COMMAND cmd, const PRECIVE_PACKET RecivePacket);

/*************************************************
  Function:    		recive_cmd_data_proc
  Description:		接收UDP网络命令
  Input: 			无
  Output:			无
  Return:			无
  Others:
*************************************************/
void* recive_cmd_data_proc(void *arg);

/*************************************************
  Function:    		net_get_cmd_by_nethead
  Description:		获得命令值(含子系统代号)
  Input: 
	1.netHead		网络包头
  Output:			无
  Return:			网络命令
  Others:
*************************************************/
NET_COMMAND net_get_cmd_by_nethead(const PNET_HEAD netHead);

/*************************************************
  Function:    		net_send_cmd_packet
  Description:		发送cmd包
  Input: 
	1.data			发送数据
	2.size			数据大小
	3.address		发送地址
	4.port			发送端口
  Output:			无
  Return:			成功与否
  Others:
*************************************************/
int32 net_send_cmd_packet(char * data, int32 size, uint32 address, uint16 port);

/*************************************************
  Function:    		net_start_udp_comm
  Description:		启动UDP通讯
  Input: 			无
  Output:			无
  Return:			成功与否
  Others:
*************************************************/
int32 net_start_udp_comm(void);

/*************************************************
  Function:    		net_stop_udp_comm
  Description:		停止UDP通讯
  Input: 			无
  Output:			无
  Return:			无
  Others:
*************************************************/
void net_stop_udp_comm(void);

/*************************************************
  Function:    		net_set_mult_packet_recive
  Description: 		设置接收多包接收处理函数
  Input: 
	1.func			多包接收处理函数
  Output:			无
  Return:			无
  Others:
*************************************************/
void net_set_mult_packet_recive(PFRECIVE_PACKET_PROC func);

/*************************************************
  Function:    		net_set_dns_packet_recive
  Description: 		设置DNS包发送处理函数
  Input: 
	1.func			多包接收处理函数
  Output:			无
  Return:			无
  Others:
*************************************************/
void net_set_dns_packet_recive(PFRECIVE_PACKET_PROC func);

#endif  /*__NET_COMM_H__*/


