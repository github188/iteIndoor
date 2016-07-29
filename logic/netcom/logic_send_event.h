/*************************************************
  Copyright (C), 2006-2010, Aurine
  File name:	logic_send_event.h
  Author:   	xiewr
  Version:   	1.0
  Date: 		
  Description:  δ���ͳɹ��¼�ͷ�ļ�
  
  History:        
                  
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#ifndef __NET_SEND_EVENT_H__
#define __NET_SEND_EVENT_H__
#include "logic_include.h"

#define SEND_RECORD_MAXLEN       	20 													// ÿ����¼�ĳ���
#define SEND_RECORD_HEAD_LEN       	(1 + sizeof(NET_COMMAND)) 							// ÿ����¼ͷ�ĳ���
#define SEND_RECORD_DATA_LEN       	(SEND_RECORD_MAXLEN - SEND_RECORD_HEAD_LEN) 		// ÿ����¼��Ч�غ�
#define SEND_RECORD_STOR_COUNT		2000												// �洢��¼��

/*************************************************
  Function:			net_ini_unsend_file
  Description: 		����ʱ,��ʼ���¼��ļ�
  Input: 			��
  Output:			��
  Return:			��
*************************************************/
void net_ini_unsend_file(void);

/*************************************************
  Function:    		net_close_unsend_file
  Description:		�ر�δ���ͳɹ��ļ�(�ڳ������ʱ�ر�)
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
void net_close_unsend_file(void);

/*************************************************
  Function:			net_send_event_from_file
  Description: 		�Ӵ洢���ļ��з���ָ�����ͷ����¼�
  Input: 
	1.cmd			����
	2.data			��������
	3.size			�������ݴ�С
	4.SendTo		���ͷ�, ����ƽ̨/���ķ�����/����Ա�� 
  Output:			��
  Return:			�Ƿ�ɹ�
*************************************************/
int32 net_send_event_from_file(NET_COMMAND cmd, char * data, int32 size, EVENT_SENDTO_E SendTo);

/*************************************************
  Function:			net_check_is_report_event_echo
  Description: 		�жϸ�Ӧ����Ƿ��ϱ��¼���Ӧ����
  Input: 
	1.ID			Ӧ�������
  Output:			��
  Return:			�Ƿ�ɹ�
*************************************************/
int32 net_check_is_report_event_echo(NET_COMMAND cmd, uint16 ID);

/*************************************************
  Function:			net_update_event_file
  Description: 		��ʱ�����¼������ļ�
  Input: 			��
  Output:			��
  Return:			��
*************************************************/
void net_update_event_file(void);

#endif 

