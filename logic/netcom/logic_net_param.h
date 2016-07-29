/*************************************************
  Copyright (C), 2006-2010, Aurine
  File name:  	logic_net_param.h
  Author:     	xiewr
  Version:    	1.0
  Date: 
  Description:  ��ȡ����IP��Ϣͷ�ļ�
  
  History:        
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#ifndef _NET_GET_PARAM_H_
#define _NET_GET_PARAM_H_

#include "logic_include.h"


/*************************************************
  Function:    		NetworkInit
  Description:		�����ʼ��
  Input: 			
  Output:			��
  Return:			��
  Others:
*************************************************/
void NetworkInit(void);

/*************************************************
  Function:    		net_set_local_param
  Description:		���ñ����������
  Input: 			
  	1.NetParam		�������ָ��
  Output:			��
  Return:			��
  Others:
*************************************************/
void net_set_local_param(void);

/*************************************************
  Function:    		net_set_local_mac
  Description:		����MAC
  Input: 			
  	1.mac			MAC
  Output:			��
  Return:			��
  Others:			TRUE / FALSE
*************************************************/
uint32 net_set_local_mac(uint8 *mac);

/*************************************************
  Function:    		IPtoUlong
  Description:		��IP��ַ�ַ���ת��Ϊlong��IP��ַ
  Input:
  	1.ip			IP��ַ�ַ���
  Output:			
  Return:			ת�����unsigned long��IP��ַ
  Others:			
*************************************************/
uint32 IPtoUlong(char* ip);

/*************************************************
  Function:    		IPIsCorrect
  Description:		IP�Ƿ�����
  Input:
  	1.ip			IP��ַ�ַ���
  Output:			
  Return:			
  Others:			
*************************************************/
int IPIsCorrect(char* ip);

/*************************************************
  Function:    		UlongtoIP
  Description:		��long��IP��ַת��ΪIP��ַ�ַ���
  Input:
  	1.uIp			unsigned long��IP��ַ
  Output:			
  Return:			ת������ַ���ָ��
  Others:			����NULL ��ʾIP��ַ����
*************************************************/
char* UlongtoIP(uint32 uIp);
#endif

