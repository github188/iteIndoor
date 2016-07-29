/*************************************************
  Copyright (C), 2006-2010, Aurine
  File name:  	logic_udp_comm.h
  Author:     	xiewr
  Version:    	1.0
  Date: 
  Description:  UDPͨ��ͷ�ļ�
  
  History:        
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#ifndef _NET_UDP_COMM_H_
#define _NET_UDP_COMM_H_
#include "logic_include.h"

#define	USE_RECIVE_THREAD		1					// ʹ���߳̽�����������

#if USE_RECIVE_THREAD
//#define	MB_NETRECV				MBCom			// �������msbox
//#define TPRI_NETRECV			TPRI_COM
#else
#define	UDP_RECV_TIMER_TICKS	2					// ʹ�ö�ʱ��������������
#endif
#if 0
/***************************����Ϊ���������**************************/
// ���緢�Ͱ�
typedef struct SEND_PACKET_NODE
{
	struct SEND_PACKET_NODE * next;
	struct SEND_PACKET_NODE * prev;
	char SendState;									// ����״̬, ��TSendState
	char SendTimes; 								// ��ǰ���ʹ���
	char SendTimesMax; 								// ��෢�ʹ���
	int TimeOut;									// ���ͳ�ʱʱ��, ��
	int WaitCount; 									// �ȴ�ʱ�����
	uint32 address;									// �Է�IP��ַ
	uint16 port;									// �Է��˿ں�
	int size;										// ���ݰ���С
	char * data;									// ��������
}SEND_PACKET, *PSEND_PACKET;

// ������հ�
typedef struct RECIVE_PACKET_NODE
{
	struct RECIVE_PACKET_NODE * next;
	uint32 address;									// �Է�IP��ַ
	uint16 port;									// �Է��˿ں�
	int size;										// ���ݰ���С
	char * data;									// ��������
}RECIVE_PACKET, *PRECIVE_PACKET;
/***************************����Ϊ���������**************************/
#endif
// ���հ�����������,���������������, ����: ���հ�
typedef void (*PFRECIVE_PACKET_PROC)(NET_COMMAND cmd, const PRECIVE_PACKET RecivePacket);

/*************************************************
  Function:    		recive_cmd_data_proc
  Description:		����UDP��������
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
void* recive_cmd_data_proc(void *arg);

/*************************************************
  Function:    		net_get_cmd_by_nethead
  Description:		�������ֵ(����ϵͳ����)
  Input: 
	1.netHead		�����ͷ
  Output:			��
  Return:			��������
  Others:
*************************************************/
NET_COMMAND net_get_cmd_by_nethead(const PNET_HEAD netHead);

/*************************************************
  Function:    		net_send_cmd_packet
  Description:		����cmd��
  Input: 
	1.data			��������
	2.size			���ݴ�С
	3.address		���͵�ַ
	4.port			���Ͷ˿�
  Output:			��
  Return:			�ɹ����
  Others:
*************************************************/
int32 net_send_cmd_packet(char * data, int32 size, uint32 address, uint16 port);

/*************************************************
  Function:    		net_start_udp_comm
  Description:		����UDPͨѶ
  Input: 			��
  Output:			��
  Return:			�ɹ����
  Others:
*************************************************/
int32 net_start_udp_comm(void);

/*************************************************
  Function:    		net_stop_udp_comm
  Description:		ֹͣUDPͨѶ
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
void net_stop_udp_comm(void);

/*************************************************
  Function:    		net_set_mult_packet_recive
  Description: 		���ý��ն�����մ�����
  Input: 
	1.func			������մ�����
  Output:			��
  Return:			��
  Others:
*************************************************/
void net_set_mult_packet_recive(PFRECIVE_PACKET_PROC func);

/*************************************************
  Function:    		net_set_dns_packet_recive
  Description: 		����DNS�����ʹ�����
  Input: 
	1.func			������մ�����
  Output:			��
  Return:			��
  Others:
*************************************************/
void net_set_dns_packet_recive(PFRECIVE_PACKET_PROC func);

#endif  /*__NET_COMM_H__*/


