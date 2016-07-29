/*************************************************
  Copyright (C), 2006-2016, Aurine
  File name:  	logic_dianti.h
  Author:     	luofl
  Version:    	2.0
  Date: 		2014-12-18
  Description:  
				���ݿ����߼�,�������ĵ��ݿ�����
  History:        
    1. Date:	
       Author:	
       Modification:
*************************************************/
#ifndef _DIANTI_H_
#define _DIANTI_H_

#ifndef _AU_PROTOCOL_
#define DIANTI_UDP_PORT			9000				// ���ݿ�����UDP�˿�: 9000
#define ELEVATOR_UP				0x01 				// ��������
#define ELEVATOR_DOWN			0x02				// ��������
#define ELEVATOR_CALL			0x03				// ���ݱ�������
#else
// ���ֵ��ݿ�����
#define DIANTI_UDP_PORT			8000				// ���ݿ�����UDP�˿�: 8000
#define ELEVATOR_UP				0x03 				// ��������
#define ELEVATOR_DOWN			0x02				// ��������
#define ELEVATOR_CALL			0x05				// ���ݱ�������
#define ELEVATOR_ROOM			0x0B				// ���ݻ���
#endif

// ���������ݿ�����
#define OTHER_ELEVATOR_UP		0x01 				// ��������
#define OTHER_ELEVATOR_DOWN		0x02				// ��������
#define OTHER_ELEVATOR_CALL		0x03				// ��������

/*************************************************
  Function:		dianti_set_cmd
  Description:	������������ݿ�����
  Input:
  Output:
  Return:		��
  Others:		��	
*************************************************/
void dianti_set_cmd(uint8 direction);

/*************************************************
  Function:		dianti_rec_cmd
  Description:	���յ��ݿ�����������
  Input:		��
  Output:		��	
  Return:		��
  Others:	
*************************************************/
void dianti_rec_cmd(void);

/*************************************************
  Function:		get_dianti_ip
  Description:	��ȡ���ݿ�����IP��ַ
  Input:		��
  Output:		��	
  Return:		IP��ַ
  Others:	
*************************************************/
uint32 get_dianti_ip(void);
#endif

