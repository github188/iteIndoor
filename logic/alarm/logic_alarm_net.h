/********************************************************
  Copyright (C), 2006-2010, Aurine
  File name:	logic_alarm_net.h
  Author:		wengjf
  Version:		V1.0
  Date:			2010-4-10
  Description:	���صı���������ƿ���
  
  History:        
                  
    1. Date:
       Author:
       Modification:Ϊ��ֹ�����Ļ��ң����еķ�����ȫ����0-7
       ��ʾ1-8�����ţ���Ҫ���ġ�
    2. ... 
*********************************************************/
#ifndef __LOGIC_ALARM_NET_H__
#define __LOGIC_ALARM_NET_H__

#include "logic_include.h"


// �������Խṹ(�ϱ�������Ա����)
typedef struct 
{
	uint8 area_amount;								// ������
	uint8 bypass[2];								// ���ò���
	uint8 area_type[16];							// ����(16B)	
}ALARM_DEFINE_NET_DATA, *PALARM_DEFINE_NET_DATA;

// ����״̬�ṹ(�ϱ�������Ա����), ÿһλ����һ������
typedef struct
{
	uint8 defend_state;								// ����״̬,����״̬,�ַ�״̬
	uint8 area_amount;								// ������1B
	uint8 area_state[2];							// ����״̬(2B)
	uint8 alarm_state[2];							// ��������״̬(2B)
	uint8 fault_state[2];							// ��������״̬(2B)	
}ALARM_STATUES_NET_DATA, *PALARM_STATUES_NET_DATA;

/*************************************************
  Function:    		alarm_define_report
  Description: 		�����ϱ�
  Input:        
  Output:       	��
  Return:			�ϱ��ɹ����
  Others:       	
*************************************************/
int32 alarm_define_report(void);

/*************************************************
  Function:    		alarm_status_report
  Description: 		����״̬�ϱ�
  Input:        
  Output:       	��
  Return:			�ϱ��ɹ����
  Others:       	
*************************************************/
int32 alarm_status_report(void);

/*************************************************
  Function:    		set_event_report
  Description: 		���������¼��ϱ�
  Input:        	
  	1.controlby		����ִ����
  Output:      		��
  Return:			�ϱ��ɹ����
  Others:
*************************************************/
int32 set_event_report(EXECUTOR_E controlby);

/*************************************************
  Function:    		unset_event_report
  Description: 		���������¼��ϱ�
  Input:        	
  	1.controlby		����ִ����
  Output:       	��
  Return:			�ϱ��ɹ����
  Others:
*************************************************/
int32 unset_event_report(EXECUTOR_E controlby);

/*************************************************
  Function:    		partset_event_report
  Description: 		�ַ������¼��ϱ�
  Input:       
  	1.controlby		����ִ����
  Output:       	��
  Return:			�ϱ��ɹ����
  Others:
*************************************************/
int32 partset_event_report(EXECUTOR_E controlby);

/*************************************************
  Function:    		bypass_event_report
  Description: 		��·�����¼�
  Input:        
  	1.bypass_num	��·������
    2.bypass_area	��·����
  Output:       	��
  Return:			�ϱ��ɹ����
  Others:       	bit0��15��1����·�ķ���
*************************************************/
int32 bypass_event_report(int8 bypass_num, uint16 bypass_area);

/*************************************************
  Function:    		force_alarm_report
  Description: 		Ю�ֳ��������¼��ϱ�
  Input:   	    	
  Output:       	��
  Return:			�ϱ��ɹ����
  Others:
*************************************************/
int force_alarm_report(void);

/*************************************************
  Function:    		sos_alarm_report
  Description: 		������ȱ����¼��ϱ�
  Input:   	   		
  Output:       	��
  Return:			�ϱ��ɹ����
  Others:
*************************************************/
int sos_alarm_report(void);

/*************************************************
  Function:    		break_alarm_report
  Description: 		�������������¼��ϱ�
  Input:   	    	
  	1. area_num     ��������
  Output:       	��
  Return:			�ϱ��ɹ����
  Others:
*************************************************/
int32 break_alarm_report(uint8 area_num);

/*************************************************
  Function:    		alarm_distribute
  Description: 		����������ķַ�
  Input:       		
  	1.recPacket		���հ�
  Output:
  Return: 	   
  Others:      		
*************************************************/
int32 alarm_distribute(const PRECIVE_PACKET recPacket);

/*************************************************
  Function:			alarm_responsion
  Description:		��������Ӧ���������
  Input: 	
  	1.recPacket		Ӧ���
  	2.SendPacket	���Ͱ�
  Output:			��
  Return:			��
  Others:
*************************************************/
void alarm_responsion(const PRECIVE_PACKET recPacket, const PSEND_PACKET SendPacket);

/*************************************************
  Function:    		sync_comm_event_send
  Description: 		ͬ�������¼��ϱ�ͨ�ú���
  Input:   	    
  	1.command		���������¼�
    2.controlby		����ִ����
  Output:       	��
  Return:			�ϱ��ɹ����
  Others:
*************************************************/
int32 sync_comm_event_send(uint16 command, EXECUTOR_E controlby);
#endif

