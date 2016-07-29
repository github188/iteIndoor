/*************************************************
  Copyright (C), 2006-2016, Aurine
  File name: 	logic_getip.c
  Author:    	chenbh
  Version:   	2.0
  Date: 		2014-12-11
  Description:  ��ȡIP��ַ��Ϣ
				
  History:        
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#include "logic_getip.h"

#define ADDR_COUNT					100
#define USED_QUERYIP_FROM_DNS		1

static uint32		g_AreaCode = 0;					// ��ǰDNS��������С����
static uint32 		g_AddressList[ADDR_COUNT]; 		// ��ַ�б�
static DEVICE_NO	g_DevNoList[ADDR_COUNT]; 		// �豸���б�
static uint8 		g_EchoCount = 0;				// Ӧ��ĵ�ַ���� ,�������ڷֻ�ʱ�õ�
static uint8 		g_IsWaitEcho = FALSE;			// �Ƿ�ȴ�Ӧ��

/*************************************************
  Function:    		get_ip_by_broadcast
  Description:		�����豸�Ź㲥���IP��ַ
  Input: 
  	1.DeviceType	�豸����
	2.DeviceNo		�豸���
  Output:			��
  Return:			IP��ַ, �����ֽ�˳��Ϊ0ʱΪ�޷���ֵ
  Others:
*************************************************/
static int get_ips_by_broadcast(uint8 DeviceType, char * DeviceNo)
{
	char RecData[30] = {0};
	int32 len = strlen(DeviceNo);
	char SendData[50]={0};
	uint8 EchoValue;
	int32 ReciSize;
	int32 IsMatch = FALSE; 				//�Ƿ�ͨ���
	int32 ret = FALSE;
	int32 n = 30;

	SendData[0] = DeviceType;
	memcpy(SendData+1, DeviceNo, len); 
	
	if (len==2)
	{
		if (DeviceNo[0]=='_' || DeviceNo[1]=='_')
		{
			IsMatch = TRUE;
		}	
	}
	else
	{
		if (DeviceNo[len-1]=='_' || DeviceNo[len-1]=='*')
		{
			IsMatch = TRUE;
		}	
	}

	// �㲥����, ip��ַΪ�㲥��ַ
	g_EchoCount = 0;
	if (IsMatch)									// ��ͨ����Ĳ���Ӧ��
	{
		g_IsWaitEcho = TRUE;
		set_nethead(G_BROADCAST_DEVNO, PRIRY_DEFAULT);
		net_direct_send(CMD_DNS_GETIP, SendData, len+1, INADDR_BROADCAST, NETCMD_UDP_PORT);
		
		// ��ʱ30ms
		while (n)
		{
			DelayMs_nops(50);
			n--;
		}
		g_IsWaitEcho = FALSE;
	}
	else											// ��ͨ����ĵ�Ӧ��
	{
		set_nethead(G_BROADCAST_DEVNO, PRIRY_DEFAULT);
		ret = net_send_command(CMD_DNS_GETIP, SendData, len+1, INADDR_BROADCAST, NETCMD_UDP_PORT, 3, &EchoValue, RecData, &ReciSize);
		if (TRUE == ret)
		{
			if (EchoValue == ECHO_OK)
			{
				memcpy(&g_DevNoList[0], RecData, sizeof(DEVICE_NO));
				memcpy(&g_AddressList[0], RecData+sizeof(DEVICE_NO), 4);
				g_EchoCount = 1;
			}
			//free(RecData);
		}
	}
	return g_EchoCount;
}

/*************************************************
  Function:    		net_set_dns_areacode
  Description:		����DNS�ϵ�С����
  Input: 
	1.AreaCode		DNS�ϵ�С�����,ͨ��������
  Output:			��
  Return:			��
  Others:
*************************************************/
void net_set_dns_areacode(uint32 AreaCode)
{
	g_AreaCode = AreaCode;
}

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
void net_recive_dns_echo(uint32 IP, char * ReciveData, int32 size)
{
	dprintf("IP:0x%x, size:%d\n", IP, size);
	int32 i = 0;
	if (size != sizeof(DEVICE_NO)+4)
	{
		return;
	}
	if (!g_IsWaitEcho || g_EchoCount >= ADDR_COUNT)
	{
		return;
	}

	// add by chenbh   �����ظ�IP Ӧ���ε��ж� 
	// ipģ��Ӧ�������ƶ��豸��Ӧ��ͻ����
	for (i = 0; i < g_EchoCount; i++)
	{
		if (g_AddressList[i] == (uint32)(ReciveData + sizeof(DEVICE_NO)))
		{
			dprintf(" echo ip have in list yet \n");
			return;
		}
	}
	
	memcpy(&g_DevNoList[g_EchoCount], ReciveData, sizeof(DEVICE_NO));
	memcpy(&g_AddressList[g_EchoCount], ReciveData+sizeof(DEVICE_NO), 4);
	if (g_AddressList[g_EchoCount] == 0)
	{
		g_AddressList[g_EchoCount] = IP;
		dprintf("g_EchoCount:%d, IP:%x\n", g_EchoCount, g_AddressList[g_EchoCount]);
	}
	g_EchoCount++;	
}

/*************************************************
  Function:    		net_is_lan_ip
  Description:		�Ƿ������IP
  Input: 
	1.ip			ip��ַ(�����ֽ�˳��)
  Output:			��
  Return:			�Ƿ�,true/false
  Others:
*************************************************/
int32 net_is_lan_ip(uint32 ip)
{
	/*
	a����
	 10.0.0.0~10.255.255.255		1
	0x0A000000~0x0AFFFFFF
	b����
	 172.16.0.0~172.31.255.255		16
	0xAC100000~0xAC1FFFFF
	c����
	 192.168.0.0~192.168.255.255	255
	0xC0A80000~0xC0A8FFFF
	*/
	if (ip>=0x0A000000 && ip<0x0AFFFFFF)
	{
		return TRUE;
	}	
	if (ip>=0xAC100000 && ip<0xAC1FFFFF)
	{
		return TRUE;
	}	
	if (ip>=0xC0A80000 && ip<0xC0A8FFFF)
	{
		return TRUE;
	}	
	
	return FALSE;
}

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
int32 net_get_devices_and_ips(uint32 AreaCode, uint8 DeviceType, char * DeviceNo, PDEVICE_NO *DevNoList, uint32 **IPList)
{
	// �����豸�ж�
	#if 0 	
	if (AreaCode!=g_AreaCode)
		return 0;
	#endif	
	
	int32 count = 0;
	int32 ret = FALSE; 

#if	USED_QUERYIP_FROM_DNS		
	// �������ķ�������ѯ
	int32 len = strlen(DeviceNo);
	char SendData[50];
	uint8 EchoValue;
	char RecData[200];
	int32 ReciSize;
	uint32 ServerIP;
	int32 i = 0;
	
	SendData[0] = DeviceType;
	memcpy(SendData+1, DeviceNo, len); 
	ServerIP = storage_get_center_ip();
	set_nethead(G_CENTER_DEVNO, PRIRY_DEFAULT);
	ret = net_send_command(CMD_SERVER_GETIP, SendData, len+1, ServerIP, NETCMD_UDP_PORT, 1, &EchoValue, RecData, &ReciSize);
	if (TRUE == ret)
	{
		if (EchoValue==ECHO_OK && ReciSize>=4)
		{
			memcpy(&count, RecData, 4);
			if (count>0 && ((sizeof(DEVICE_NO) + 4)*count+4) == ReciSize)
			{
				//����4B���豸��ţ�IP��ַ4B �˴�������϶˿ں�����Ҫ����4B
				for (i=0; i<count; i++)
				{
					memcpy(&g_DevNoList[i], RecData+4+i*(sizeof(DEVICE_NO) + 4), sizeof(DEVICE_NO));
					memcpy(&g_AddressList[i], RecData+(i+1)*(sizeof(DEVICE_NO)+4), 4);
				}
			}
			else 
			{
				ret	= FALSE;
				count = 0;
			}
		}
		else 
		{
			ret	= FALSE;
		}	
	}
#endif	

	// ���ķ�������ѯ�����ٹ㲥����
	if (!ret)
	{
		count = get_ips_by_broadcast(DeviceType, DeviceNo);
	}
	
	if (count)
	{
		if (IPList)
		{
			*IPList = g_AddressList;
		}
		if (DevNoList)
		{
			*DevNoList = g_DevNoList;
		}
	}
	else
	{
		IPList = NULL;
	}
	
	dprintf("count:%d\n", count);
	return count;
}

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
int32 net_get_ips(uint32 AreaCode, uint8 DeviceType, char * DeviceNo, uint32 **IPList)
{
	return net_get_devices_and_ips(AreaCode, DeviceType, DeviceNo, NULL, IPList);
}

