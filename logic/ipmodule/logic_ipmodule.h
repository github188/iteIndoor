/********************************************************
  Copyright (C), 2006-2010, Aurine
  File name:	logic_ipmodule.h
  Author:		
  Version:		
  Date:			
  Description:	
  
  History:        
    1. Date:
       Author:
       Modification:
    2. ... 
*********************************************************/
#ifndef __LOGIC_IPMODULE_H__
#define __LOGIC_IPMODULE_H__

#include "logic_include.h"


// �ҵ糧��
typedef enum
{
	IP_MODULE_CODE_OK = 200,						// OK
	IP_MODULE_CODE_UNLINE = 408,					// ģ������
	IP_MODULE_CODE_IP_ERR = 484,					// ģ���·�IP����
	IP_MODULE_CODE_CODE_ERR = 401,					// �������
}IP_MODULE_ERR_CODE;


typedef struct
{
	unsigned char JdType;
	unsigned char Index;
	unsigned char IsUsed;
	unsigned char IsTune;
	unsigned char  Addr;
	unsigned char  Reserve1;
	unsigned short Reserve2;
	char Name[20];
}AU_JDDEV_INFO, *PAU_JDDEV_INFO;


/*************************************************
  Function:			get_ipmodule_online
  Description:		��ȡIPģ��״̬
  Input: 			��
  Output:			��
  Return:			IP��ַ
  Others:
*************************************************/
uint8 get_ipmodule_online(void);

/*************************************************
  Function:     get_ipmodule_state
  Description:  IPģ��ͼ����ʾ״̬
  Input:        ��
  Output:       ��
  Return:       ��
  Others:       
*************************************************/
uint8 get_ipmodule_state(void);

/*************************************************
  Function:			public_distribute
  Description:		����ģ����շ��Ͱ��ص�����
  Input: 	
  	1.recPacket		���հ�
  Output:			��
  Return:			�ɹ����,true/fasle
  Others:
*************************************************/
int ipmodule_distribute(const PRECIVE_PACKET recPacket);

/*************************************************
  Function:			set_door_ip
  Description:		��ȡ������ǰ��IP1 
  Input: 	
  	1.type			EXT_MODE_TYPE
  	2.IP	
  Output:			��
  Return:			��
  Others:			
*************************************************/
void set_netdoor_ip(EXT_MODE_TYPE type, uint32 IP);

#ifdef _USE_ELEVATOR_
/*************************************************
  Function:			ipmodule_elevator_becken
  Description:		�ƶ��ն����������ڻ����������ٻ�
  Input: 	
  	1.recPacket		���հ�
  Output:			��
  Return:			��
  Others:			
*************************************************/
void ipmodule_elevator_becken(const PRECIVE_PACKET recPacket);

/*************************************************
  Function:			elevator_becken_unlock
  Description:		�ƶ��ն������������������������ٻ�����	
  Input: 	
  	1.recPacket		���հ�
  Output:			��
  Return:			��
  Others:			
*************************************************/
void ipmodule_elevator_becken_unlock(const PRECIVE_PACKET recPacket);
#endif

/*************************************************
  Function:			ipmodule_get_extensionAddr
  Description:		���ڷֻ���IPģ���ȡ�ֻ���IP��ַ
  Input: 			
  	1.extensionNo	�ֻ���
  Output:			��
  Return:			�ֻ�IP��ַ4B
  Others:
*************************************************/
int ipmodule_get_extensionAddr(uint32 extensionNo);

/*************************************************
  Function:    		get_ipmodule
  Description:		
  Input: 			��
  Output:			��
  Return:			�����״̬
  Others:
*************************************************/
uint32 get_ipmodule(void);

/*************************************************
  Function:			ipmodule_get_room_param
  Description:		��ȡ������������ز�����Ϣ����
  Input: 	
  	1.recPacket		���հ�
  Output:			��
  Return:			��
  Others:			
*************************************************/
void ipmodule_get_room_param(const PRECIVE_PACKET recPacket);

/*************************************************
  Function:			ipmodule_get_netdoor_list
  Description:		��ȡ�������������������ǰ����Ϣ�б�
  Input: 	
  	1.recPacket		���հ�
  Output:			��
  Return:			��
  Others:			
*************************************************/
void ipmodule_get_netdoor_list(const PRECIVE_PACKET recPacket);

/*************************************************
  Function:			ipmodule_get_manager_list
  Description:		��ȡ������������Ĺ��������б�	
  Input: 	
  	1.recPacket		���հ�
  Output:			��
  Return:			��
  Others:			
*************************************************/
void ipmodule_get_manager_list(const PRECIVE_PACKET recPacket);

/*************************************************
  Function:			get_ipmodule_address
  Description:		��ȡIPģ��״̬
  Input: 			��
  Output:			��
  Return:			IP��ַ
  Others:
*************************************************/
uint32 get_ipmodule_address(void);

/*************************************************
  Function:			init_ipmodule
  Description:		IPģ���ʼ��
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
void init_ipmodule(void);
#endif


