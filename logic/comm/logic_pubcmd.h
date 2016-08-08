/*********************************************************
  Copyright (C), 2006-2010, Aurine
  File name:	logic_pubcmd.h
  Author:   	xiewr
  Version:   	
  Date: 		
  Description:  ��������
  
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
  Output:			��
  Return:			
  Others:
*************************************************/
void init_rtsp_timer(void);

/*************************************************
  Function:    		logic_reg_center_state
  Description:		��ȡ�ն�ע��״̬
  Input:			
  Output:			
  Return:			
  Others:			
*************************************************/
int32 logic_reg_center_state(void);

/*************************************************
  Function:    		logic_reg_center_ini
  Description:		�ն�ע��
  Input:			
  Output:			
  Return:			
  Others:		MAC ��+���豸���+Ӳ���汾��ʶ��64B��+����汾��ʶ��64B��+�����豸���ƣ�32�ֽڣ���\0����β					
*************************************************/
void logic_reg_center_ini(void);

/*************************************************
  Function:    		logic_reg_center_ini_again
  Description:		�����������ע��
  Input:			
  Output:			
  Return:			
  Others:			
*************************************************/
void logic_reg_center_ini_again(void);

/*************************************************
  Function:			public_distribute
  Description:		����������հ��ص�����
  Input: 	
  	1.recPacket		���հ�
  Output:			��
  Return:			�ɹ����, TRUE/FALSE
  Others:	
*************************************************/
int32 public_distribute(const PRECIVE_PACKET recPacket);

/*************************************************
  Function:			public_responsion
  Description:		�����������Ӧ���������
  Input: 	
  	1.recPacket		Ӧ���
  	2.SendPacket	���Ͱ�
  Output:			��
  Return:			��
  Others:
*************************************************/
void public_responsion(const PRECIVE_PACKET recPacket, const PSEND_PACKET SendPacket);

#endif

