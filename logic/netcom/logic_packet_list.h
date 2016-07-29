/*************************************************
  Copyright (C), 2006-2010, Aurine
  File name:  	logic_packet_list.h
  Author:     	xiewr
  Version:    	1.0
  Date: 
  Description:  ��������ͽ�������ͷ�ļ�
  
  History:        
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#ifndef _NET_PACKET_LIST_H_
#define _NET_PACKET_LIST_H_

#include "logic_include.h"

#define		PACKET_NODE_MAX_NUM			100				// ����ڵ�������
#define		PACKET_NODE_DATA_SIZE		NET_DATA_MAX	// ���ڵ����ݴ�С

// ��ʱ���ͼ����ʱ��
#if !USE_RECIVE_THREAD
#define		PACKET_OUTTIME_INTERVAL		(1000/(UDP_RECV_TIMER_TICKS*5))	
#elif !USE_DISTRIBUTE_THREAD
#define		PACKET_OUTTIME_INTERVAL		(1000/(NET_DIST_TIMER_TICKS*5))	
#endif
#define 	PACKET_TIMEOUT_VALUE		3				// Ĭ�ϳ�ʱ�ж�ʱ��, �����߿���ָ��(��)

// ���ͳ�ʱ������, ����:���Ͱ�
typedef void (*PFSEND_TIMEOUT_ECHO)(const PSEND_PACKET SendPacket);

/*************************************************
  Function:    		net_packet_list_init
  Description: 		��ʼ������,����ռ�
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
void net_packet_list_init(void);

/*************************************************
  Function:    		net_add_send_packet
  Description: 		����һ�����Ͱ�������
  Input: 
	1.data			��������
	2.size			�������ݴ�С
	3.address		���͵�ַ
	4.port			���Ͷ˿�
	5.SendTimes		���ʹ���, Ĭ�Ϸ��ʹ���Ϊ1
	6.TimeOut		���ͳ�ʱʱ��, Ĭ��ΪTIMEOUT_VALUE
  Output:			��
  Return:			���Ͱ�
  Others:
*************************************************/
PSEND_PACKET net_add_send_packet(char * data, int32 size, uint32 address, uint16 port, 
	int32 SendTimes, int32 TimeOut);

/*************************************************
  Function:    		net_del_send_packet
  Description: 		�ӷ��������Ƴ�һ�������, �ӵ���������
  Input: 
	1.node			���Ͱ�
  Output:			��
  Return:			��
  Others:
*************************************************/
void net_del_send_packet(PSEND_PACKET node);

/*************************************************
  Function:    		net_send_proc
  Description:		�����̴߳�����
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
void* net_send_proc(void *arg);

/*************************************************
  Function:    		net_timeout_proc
  Description:		��ʱ�̴߳�����
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
void* net_timeout_proc(void *arg);

/*************************************************
  Function:    		net_add_recive_packet
  Description:		�����������ݰ��߳�ִ�к���
  Input: 			
  	1.data			����
  	2.size			��С
  	3.address		��ַ
  	4.port			�˿ں�
  Output:			��
  Return:			��
  Others:			���һ�����հ�����������
*************************************************/
void net_add_recive_packet(char *data, uint32 size, uint32 address, uint16 port);

/*************************************************
  Function:    		net_get_recive_packet
  Description:		��ȡһ�����հ�, �ӽ����������Ƴ��ýڵ�
  Input: 			��
  Output:			��
  Return:			���հ�
  Others:
*************************************************/
PRECIVE_PACKET net_get_recive_packet(void);

/*************************************************
  Function:    		net_del_recive_packet
  Description:		��һ�����հ���ӵ���������
  Input: 			
	1.node			���ڵ�
  Output:			��
  Return:			��
  Others:
*************************************************/
void net_del_recive_packet(PRECIVE_PACKET node);

/*************************************************
  Function:    		net_get_sendpacket_by_responsion
  Description:		����Ӧ���ͷ,��÷��Ͱ�
  Input: 
	1.netHead		Ӧ���ͷ
  Output:			��
  Return:			���Ͱ�
  Others:
*************************************************/
PSEND_PACKET net_get_sendpacket_by_responsion(PNET_HEAD netHead);

/*************************************************
  Function:    		net_set_send_timeout_echofunc
  Description:		���÷��ͳ�ʱӦ��ص�����
  Input: 
	1.func			���ͳ�ʱӦ����ָ��
  Output:			��
  Return:			��
  Others:
*************************************************/
void net_set_send_timeout_echofunc(PFSEND_TIMEOUT_ECHO func);

/*************************************************
  Function:    		start_comm_thread
  Description:		���������߳�
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
void net_start_comm_thread(void);

/*************************************************
  Function:    		stop_comm_thread
  Description:		ֹͣ�����߳�
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
void net_stop_comm_thread(void);

#endif
