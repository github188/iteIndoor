/*************************************************
  Copyright (C), 2006-2016, Aurine
  File name:  	logic_getip.h
  Author:     	chenbh
  Version:    	2.0
  Date: 
  Description:  ��ȡ����IP��Ϣͷ�ļ�
  
  History:        
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#ifndef _NET_GET_IP_H_
#define _NET_GET_IP_H_

#include "logic_include.h"


/*************************************************
  Function:    		net_set_dns_areacode
  Description:		����DNS�ϵ�С����
  Input: 
	1.AreaCode		DNS�ϵ�С�����,ͨ��������
  Output:			��
  Return:			��
  Others:
*************************************************/
void net_set_dns_areacode(uint32 AreaCode);

/*************************************************
  Function:    		net_recive_dns_echo
  Description:		���յ�DNSӦ��
  Input: 
	1.IP			IP��ַ
	2.ReciveData	�������ݵ�ַ
	3.size			�������ݴ�С
  Output:			��
  Return:			��
  Others:
*************************************************/
void net_recive_dns_echo(uint32 IP, char * ReciveData, int32 size);

/*************************************************
  Function:    		net_is_lan_ip
  Description:		�Ƿ������IP
  Input: 
	1.ip			ip��ַ(�����ֽ�˳��)
  Output:			��
  Return:			�Ƿ�,true/false
  Others:
*************************************************/
int32 net_is_lan_ip(uint32 ip);

/*************************************************
  Function:    		net_get_devices_and_ips
  Description:		��ѯ�豸��IP��ַ�б�
  Input: 
	1.AreaCode		��������
	2.DeviceType	�豸����
	3.DeviceNo		�豸���ַ���
  Output:			��
  	1.DevNoList		�豸���б�
  	2.IPList		IP��ַ�б�ָ��
  Return:			����
  Others:
*************************************************/
int32 net_get_devices_and_ips(uint32 AreaCode, uint8 DeviceType, char * DeviceNo, PDEVICE_NO *DevNoList, uint32 **IPList);

/*************************************************
  Function:    		net_get_ips
  Description:		��ѯIP��ַ�б�
  Input: 
	1.AreaCode		��������
	2.DeviceType	�豸����
	3.DeviceNo		�豸���ַ���
  Output:			��
  	1.IPList		IP��ַ�б�ָ��
  Return:			IP��ַ����
  Others:
*************************************************/
int32 net_get_ips(uint32 AreaCode, uint8 DeviceType, char * DeviceNo, uint32 **IPList);

#endif

