/*************************************************
  Copyright (C), 2006-2016, Aurine
  File name:  	logic_distribute.h
  Author:     	chenbh
  Version:    	1.0
  Date: 		2014-12-11
  Description:  ����ַ�ģ��ͷ�ļ�
  
  History:        
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#ifndef _NET_DISTRIBUTE_H_
#define _NET_DISTRIBUTE_H_

#include "logic_include.h"


#define	USE_DISTRIBUTE_THREAD	0					// ʹ���̷ַ߳�����
#if USE_DISTRIBUTE_THREAD
#define	MB_NETDIST				MBCom				
#define TPRI_NETDIST			TPRI_COM
#else
#define	NET_DIST_TIMER_TICKS	100					// ʹ�ö�ʱ���ַ�����
#endif

// ������ճ�ʱ�����
#if !USE_RECIVE_THREAD
#define	NET_MULTI_RECV_CHECK_TICKS	UDP_RECV_TIMER_TICKS
#elif !USE_DISTRIBUTE_THREAD
#define	NET_MULTI_RECV_CHECK_TICKS	NET_DIST_TIMER_TICKS
#endif

extern DEVICE_NO G_CENTER_DEVNO;
extern DEVICE_NO G_MANAGER_DEVNO;
extern DEVICE_NO G_BROADCAST_DEVNO;
extern DEVICE_NO G_MULTCMD_DEVNO;

/*********************������ջص�***********************************/
//����������ݷ��Ͱ��ص���������, ����:���հ�, ����ֵ:�Ϸ�����TRUE, �Ƿ�����FALSE,�����߲���Ӧ��
typedef int32 (*PFDISTRIBUTE)(const PRECIVE_PACKET recPacket);
//�����������Ӧ����ص���������, ����:���հ�, ���Ͱ�
typedef void (*PFRESPONSION)(const PRECIVE_PACKET recPacket, const PSEND_PACKET SendPacket);

/**********************�������*********************************/
//������ͽ��Ӧ��ص���������, ����:�ỰID��Ӧ����
typedef void (*PFMAX_SEND_ECHO)(uint32 ID, int sendEcho);
//������ս���ص���������, ����:�ỰID, ����, ���ݳ���
typedef void (*PFMAX_RECIVE)(uint32 ID, char * data, int size);
//������ͻ����״̬, ����:�ỰID, �ܹ�����, �Ѿ����ջ��ͳɹ�����, �Ѿ����ջ��ͳɹ����ݳ���
typedef void (*PFMAX_SEND_REC_STATE)(uint32 ID, int Total, int CurNum, int size);

/*************************************************
  Function:    		net_direct_send
  Description:		����ֱ�ӷ��ͣ����ӵ�������
  Input: 
	1.cmd			����
	2.data			��������
	3.size			�������ݴ�С
	4.address		�Է�IP��ַ
	5.port			�Է��˿ں�
  Output:			��
  Return:			��ID
  Others:
*************************************************/
uint16 net_direct_send(NET_COMMAND cmd, char * data, int32 size, uint32 address, uint16 port);

/*************************************************
  Function:    		net_send_by_list
  Description:		�������������������,������
  Input: 
	1.cmd			����
	2.data			��������
	3.size			�������ݴ�С
	4.address		�Է�IP��ַ
	5.port			�Է��˿ں�
	6.SendTimes		���ʹ���
	7.TimeOut		���η��ͳ�ʱʱ��
  Output:			��
  Return:			��ID
  Others:
*************************************************/
uint16 net_send_by_list(NET_COMMAND cmd, char * data, int32 size, uint32 address, uint16 port, 
				int32 SendTimes, int32 TimeOut);

/*************************************************
  Function:    		net_send_command
  Description:		���������������ͣ������������߳��еȴ�Ӧ��
  Input: 
	1.cmd			����
	2.data			��������
	3.size			�������ݴ�С
	4.address		�Է�IP��ַ
	5.port			�Է��˿ں�
	6.TimeOut		���ͳ�ʱʱ��
  Output:			
	1.EchoValue		Ӧ����
	2.ReciData		����Ӧ������, ע��:���ⲿ����ռ�
	3.ReciSize		����Ӧ�����ݴ�С
  Return:			�ɹ����, TRUE/FALSE
  Others:
*************************************************/
int32 net_send_command(NET_COMMAND cmd, char * data, int32 size, uint32 address, uint16 port, int32 TimeOut,
				 uint8 * EchoValue, char * ReciData, int32 * ReciSize);

/*************************************************
  Function:    		net_send_echo_packet
  Description:		����Ӧ���
  Input: 
	1.recPacket		���հ�
	2.echoValue		Ӧ����
	3.data			��������
	4.size			�������ݴ�С
  Output:			��
  Return:			��
  Others:
*************************************************/
void net_send_echo_packet(PRECIVE_PACKET recPacket, ECHO_VALUE_E echoValue, char * data, int32 size);

/*************************************************
  Function:    		net_send_event
  Description:		�ϱ��¼����������¼�
  Input: 
	1.cmd			����
	2.data			��������
	3.size			�������ݴ�С
	4.SendTo		���ͷ�, ����ƽ̨/���ķ�����/����Ա�� 
  Output:			
  Return:			�Ƿ�ɷ���, TRUE/FALSE�� FALSE��������Ϊ�Ƕ�����¼���������ݳ��Ȳ�������
  Others:
*************************************************/
int32 net_send_event(NET_COMMAND cmd, char * data, int32 size, EVENT_SENDTO_E SendTo);

/*************************************************
  Function:    		net_set_recivedata_func
  Description:		���ú����ص�����
  Input: 
	1.subSysCode	��ϵͳ����
	2.distribute	���շ�������Ӧ������
	3.responsion	����Ӧ������Ӧ������
  Output:			��
  Return:			��
  Others:
*************************************************/
int32 net_set_recivedata_func(SUB_SYS_CODE_E SubSysCode, PFDISTRIBUTE distribute, PFRESPONSION responsion);

/*************************************************
  Function:    		net_start_comm
  Description:		����ͨѶģ��
  Input: 
	1.FullDeviceNo	�豸���
	2.NetParam		�����������ָ��
  Output:			��
  Return:			��
  Others:
*************************************************/
void net_start_comm(PFULL_DEVICE_NO FullDeviceNo, PNET_PARAM NetParam);

/*************************************************
  Function:    		net_stop_comm
  Description:		ֹͣͨѶģ��
  Input: 			��		
  Output:			��
  Return:			��
  Others:
*************************************************/
void net_stop_comm(void);

/*************************************************
  Function:    		net_get_comm_device_type
  Description:		�������ģ���豸����
  Input: 			��
  Output:			��
  Return:			�豸����
  Others:
*************************************************/
DEVICE_TYPE_E net_get_comm_device_type(void);

/*************************************************
  Function:    		net_change_comm_deviceno
  Description:		��������ģ���豸����Ϣ����С����
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
void net_change_comm_deviceno(void);

/*************************************************
  Function:    		net_start_multipacket_recive
  Description:		��ʼ�ְ���������
  Input: 
	1.address		�Է�IP��ַ
	2.port			�Է��˿ں�
	3.TimeOut		��ʱʱ��, ��, 0ʱ,ΪĬ�ϳ�ʱʱ��
	4.func			���ճɹ���Ӧ��ص�����
	5.state			����״̬�ص�����
  Output:			��
  Return:			���ջỰID, 0ΪæӦ��
  Others:
*************************************************/
uint32 net_start_multipacket_recive(uint32 address, uint16 port, uint8 TimeOut, PFMAX_RECIVE func, PFMAX_SEND_REC_STATE state);

/*************************************************
  Function:    		net_stop_multipacket_recive
  Description:		�жϷְ���������, (�û������жϷְ�����)
  Input: 
	1.ID			���ջỰID
  Output:			��
  Return:			��
  Others:
*************************************************/
void net_stop_multipacket_recive(uint32 ID);

/*************************************************
  Function:    		net_recive_mult_timeout_check
  Description:		������ճ�ʱ���
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
void* net_recive_mult_timeout_proc(void *arg);

/*************************************************
  Function:    		net_multi_send_check
  Description:		����Ƿ��ж������������Ҫ����
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
void net_multi_send_check(void);

/*************************************************
  Function:    		net_start_multi_packet_send
  Description:		�ְ����ݷ���
  Input: 
	1.ID			�ỰID
	2.data			����
	3.size			���ݴ�С
	4.address		�Է�IP��ַ
	5.port			�Է��˿ں�
	6.func			���ͳɹ���Ӧ��ص�����
	7.state			����״̬�ص�����
  Output:			��
  Return:			�Ƿ�ɹ�, TRUE / FALSE
  Others:
*************************************************/
int32 net_start_multi_packet_send(uint32 ID, char * data, int32 size, uint32 address, uint16 port, PFMAX_SEND_ECHO func, PFMAX_SEND_REC_STATE state);

/*************************************************
  Function:    		net_stop_multipacket_send
  Description:		�жϷְ���������, (�û������жϷְ�����)
  Input: 
	1.ID			���ͻỰID
	2.address		���͵�ַ
  Output:			��
  Return:			��
  Others:
*************************************************/
void net_stop_multipacket_send(uint32 ID, uint32 address);

/*************************************************
  Function:    		net_get_utc_time
  Description:		��ȡUTCʱ��
  Input: 
  Output:			
  	1.DateTime		ʱ��ṹ��
  Return:			��
  Others:			Ĭ��ʱ��Ϊ8
*************************************************/
void net_get_utc_time(DATE_TIME * DateTime);

/*************************************************
  Function:    		set_nethead
  Description: 		����Ŀ���豸�����ź��豸��, �����ȼ�
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
void set_nethead(DEVICE_NO DeviceNo, uint16 Priry);
#endif

