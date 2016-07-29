/*************************************************
  Copyright (C), 2006-2016, Aurine
  File name: 	logic_message.c
  Author:    	luofl
  Version:   	2.0
  Date: 		2014-11-06
  Description:	��Ϣ�����߼�
  				���ķ���ס���ĳ�Ϊ��Ϣ�����Ͱ������֡�ͼƬ����������ϣ�
  				�û�ͨ���ֻ�����ס���ĳ�Ϊ����, ����Ϊ������, һ�β�����70�����֡�
  History:                    
    1. Date:		 
       Author:		 
       Modification: 
    2. ...
*********************************************************/
#ifndef __LOGIC_MESSAGE_H__
#define __LOGIC_MESSAGE_H__

#include "logic_include.h"


/*************************************************
  Function:			msg_distribute
  Description:		���ն��ŷ�������Ӧ������
  Input:
  	1.packet		���հ�
  Output:			��
  Return:			�Ϸ�����TRUE, �Ƿ�����FALSE
  Others:
**************************************************/
int msg_distribute(const PRECIVE_PACKET recPacket);

/*************************************************
  Function:			message_responsion
  Description:		���ն���Ӧ������Ӧ������
  Input:
  	1.recPacket		������հ�
  	2.SendPacket	���緢�Ͱ�
  Output:			��
  Return:			��
  Others:
**************************************************/
void  msg_responsion(const PRECIVE_PACKET recPacket, const PSEND_PACKET SendPacket);
#endif

