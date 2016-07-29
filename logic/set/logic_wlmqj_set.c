/*********************************************************************
  COPYRIGHT (C), 2006-2010, AURINE
  FILE NAME: 		logic_wlmqj_set.c
  AUTHOR:   		xiewr
  VERSION:   		1.0 
  DATE:     		2010-06-04
  DESCRIPTION:		������ǰ������

  History:        
    1. Date:
       Author:
       Modification:
    2. ...     
*********************************************************************/
#include "logic_wlmqj_set.h"

#define NET_DOOR_LEN      		188
#define ND_WAIT_TIME	  		3
static uint32 g_NetdoorIp = 0;						// ��ǰ����IP	
static unsigned char g_NetdoorMAC[6] = {0};			// ��ǰ����MAC

/*************************************************
  Function:    		send_netdoor_cmd_echodata
  Description:		��������ǰ����������
  					�˽ӿ���Ҫ����Ӧ�����ݵ�
  					��MAC��,MAC������ǰ��
  					Ӧ������� ָ�����ɵ������ͷ�
  Input: 
  	1.cmd:			���͵�����
  	2.data: 		��������
  	3.datelen:		���ݵĳ���
  Output:			��
	1.RecData		ָ��������ݵ�ָ��ĵ�ַ
	2.ReciSize		�յ������ݳ���
  Return:			ECHO_OK: 0
  					ECHO_ERROR:0x030001
  					ECHO_MAC:0x030009
  					���ݷ��ʹ���:0x030000
  					ECHO_REPEAT:0x03000A/196618
  Others:
*************************************************/
static uint32 send_netdoor_cmd_echodata(NET_COMMAND cmd, unsigned char * SendData,uint32 len, char * RecData, uint32 * ReciSize)
{
	uint8 EchoValue = 0;
	uint32 SendIp = 0;
	uint32 ret = FALSE;
	char data[200] = {0};
	
	SendIp = g_NetdoorIp;
	memcpy(data, g_NetdoorMAC, 6);
	
	if (SendData)
	{
		memcpy(data + 6, SendData, len);
	}

	ret = net_send_command(cmd, data, len + 6, SendIp, NETCMD_UDP_PORT, ND_WAIT_TIME, &EchoValue, RecData, ReciSize);
	dprintf("send_netdoor_cmd_echodata : cmd = 0x%x , SendIp = 0x%x, ret = %d EchoValue = %d, len = %d \n", cmd, SendIp, ret, EchoValue, len + 6);
	//DelayMs_nops(10);
	if (TRUE == ret)
	{
		if (EchoValue == ECHO_OK)
		{
			return NETDOOR_ECHO_SUCESS;
		}
		else
		{
			return NETDOOR_ECHO_COMM | EchoValue;
		}
	}	
	else
	{
		return NETDOOR_ERR_SEND;
	}
}

/*************************************************
  Function:    		send_netdoor_cmd_noecho
  Description:		��������ǰ��������������
  					�˽ӿ���Ҫ������Ӧ�����ݵķ���
  					��MAC,MAC������ǰ��
  Input: 
  	1.cmd:			���͵�����
  	2.data: 		��������
  	3.datelen:		���ݵĳ���
  Output:			��
  Return:			ECHO_OK: 0
  					ECHO_ERROR:0x030001
  					ECHO_MAC:0x030009
  					����:0x0300**
  Others:
*************************************************/
uint32 send_netdoor_cmd_noecho(NET_COMMAND cmd ,unsigned char * SendData, uint32 len)
{
	uint32 ret = send_netdoor_cmd_echodata(cmd, SendData, len, NULL, NULL);
	return ret;
}

/*************************************************
  Function:			logic_set_netdoor_netparam
  Description:		����������ǰ���������
  Input: 				
    1.netparam		�������
  Output:			��
  Return:			ECHO_OK: 0
  					ECHO_ERROR:0x030001
  					ECHO_MAC:0x030009
  					���ݷ�������:0x030000
  Others:
*************************************************/
int32 logic_set_netdoor_netparam(PNET_PARAM netparam)
{
	int32 ret;
	uint32 ip;
	int32 len = sizeof(NET_PARAM_SEND);
	char data[250] = {0};
	NET_PARAM_SEND param;

	memset(data, 0, sizeof(data));
	memset(&param, 0, sizeof(NET_PARAM_SEND));

	param.IP = netparam->IP;									
	param.SubNet = netparam->SubNet;								
	param.DefaultGateway = netparam->DefaultGateway;						
	param.DNS1 = netparam->DNS1;									
	param.DNS2 = netparam->DNS2;									
	param.CenterIP = netparam->CenterIP;								
	param.ManagerIP = netparam->ManagerIP[0];							
	sprintf(param.SipProxyServer, "%s", UlongtoIP(netparam->SipProxyServer));
	sprintf(param.AurineServer, "%s", UlongtoIP(netparam->AurineServer));
	#ifdef _LIVEMEDIA_
	ip = (storage_get_netparam_bytype(RTSP_IPADDR));
	if ( ip == 0)
	{
		ip = g_Rtsp_ServerInfo.ip;
	}
	sprintf(param.StunServer, "%s", UlongtoIP(ip));
	#else
	sprintf(param.StunServer, "%s", UlongtoIP(netparam->StunServer));
	#endif
	param.IP1 = netparam->IP1;          		
	param.SubNet1 =netparam->SubNet1;        		
	
	memcpy(data, &param,  sizeof(NET_PARAM_SEND));
	dprintf("logic_set_netdoor_netparam:\n IP:\t0x%x\nSubNet:\t0x%x\nDefaultGateWay:\t0x%x\n", \
		netparam->IP, netparam->SubNet, netparam->DefaultGateway);
	ret = send_netdoor_cmd_noecho(CMD_GATEWAY_SET_NET_PARAM, data, len);
	if (NETDOOR_ECHO_SUCESS == ret)
	{
		g_NetdoorIp = netparam->IP;
	}
	
	return ret;
}

/*************************************************
  Function:			logic_get_netdevice_netparam
  Description:		��������豸�������
  Input: 				
  Output:			
  	1.netparam		�������
  Return:			ECHO_OK: 0
  					ECHO_ERROR:0x030001
  					ECHO_MAC:0x030009
  					����:0x0300**
  Others:			
*************************************************/
int32 logic_get_netdevice_netparam(PNET_PARAM netparam)
{
	char RecData[NET_DOOR_LEN+2] = {0};	
	uint32 ReciSize;
	uint32 ret;
	NET_PARAM_SEND param;
	
	memset(&param, 0, sizeof(NET_PARAM_SEND));	
	ret = send_netdoor_cmd_echodata(CMD_GATEWAY_GET_NET_PARAM, NULL, 0, RecData, &ReciSize);
	if (!ret)
	{
		if (ReciSize == NET_DOOR_LEN)
		{
			if (RecData) 
			{
				memcpy(&param, RecData, sizeof(NET_PARAM_SEND));
				netparam->IP = param.IP ;									
				netparam->SubNet = param.SubNet ;									
				netparam->DefaultGateway = param.DefaultGateway ;									
				netparam->DNS1 = param.DNS1 ;									
				netparam->DNS2 = param.DNS2 ;									
				netparam->CenterIP = param.CenterIP ;			
				netparam->ManagerIP[0] = param.ManagerIP;		
				netparam->SipProxyServer = IPtoUlong(param.SipProxyServer);
				netparam->AurineServer = IPtoUlong(param.AurineServer);
				netparam->StunServer = IPtoUlong(param.StunServer);
				netparam->IP1 = param.IP1;          	
				netparam->SubNet1 = param.SubNet1;
				dprintf("logic_get_netdevice_netparam:\n IP:\t0x%x\nSubNet:\t0x%x\nDefaultGateWay:\t0x%x\n", \
					netparam->IP, netparam->SubNet, netparam->DefaultGateway);
			}
		}
	}

	return ret;
}

/*************************************************
  Function:			logic_set_netdoor_devnoinfo
  Description:		����������ǰ���豸����Լ�����
  Input: 				
    1.devno			�豸���
    2.rule 			��Ź���
  Output:			��
  Return:			ECHO_OK: 0
  					ECHO_ERROR:0x030001
  					ECHO_MAC:0x030009
  					����:0x0300**
  Others:
*************************************************/
int32 logic_set_netdoor_devnoinfo(PFULL_DEVICE_NO devno, DEVICENO_RULE * rule)
{
	int32 ret;
	int32 devicenolen = sizeof(FULL_DEVICE_NO);
	int32 devicerulllen = sizeof(DEVICENO_RULE);
	int32 datalen = devicenolen + devicerulllen;
	char data[100] = {0};
		
	if (devno == NULL &&  rule == NULL)							// �������ܶ�Ϊ��
	{
		return NETDOOR_ERR_ECHO;
	}
	else if(devno == NULL)										// �������豸���
	{
		memcpy(data, rule, devicerulllen);
		datalen = devicerulllen;
	}
	else if(rule == NULL)										// �����ù���
	{
		memcpy(data, devno, devicenolen);
		datalen = devicenolen;
		dprintf("logic_set_netdoor_devnoinfo : devno->devicenostr:%s \n", devno->DeviceNoStr);
	}	
	else														// ͬʱ���ñ�ź͹���
	{
		memcpy(data, devno, devicenolen);
		memcpy(data + devicenolen, rule, devicerulllen);
		datalen = devicenolen + devicerulllen;
		dprintf("logic_set_netdoor_devnoinfo : devno->devicenostr:%s \n", devno->DeviceNoStr);
	}
	
	ret = send_netdoor_cmd_noecho(CMD_GATEWAY_SET_DEV_INFO, data, datalen);
	return ret;

}

/*************************************************
  Function:			logic_get_netdevice_devnoinfo
  Description:		��ȡ������ǰ���豸����Լ�����
  Input: 			
  Output:			
    1.devno			�豸���
    2.rule 			��Ź���
  Return:			ECHO_OK: 0
  					ECHO_ERROR:0x030001
  					ECHO_MAC:0x030009
  					����:0x0300**
  Others:
*************************************************/
int32 logic_get_netdevice_devnoinfo(PFULL_DEVICE_NO devno, DEVICENO_RULE * rule)
{
	int32 devicenolen = sizeof(FULL_DEVICE_NO);
	int32 devicerulllen = sizeof(DEVICENO_RULE);
	char RecData[100] = {0};									
	uint32 ReciSize;
	uint32 ret;
	
	ret = send_netdoor_cmd_echodata(CMD_GATEWAY_GET_DEV_INFO, NULL, 0, RecData, &ReciSize);
	if (!ret)
	{
		if (RecData) 
		{
			if (devno)
			{
				memcpy(devno, RecData, devicenolen);
				dprintf("logic_get_netdevice_devnoinfo : devno->devicenostr:%s \n", devno->DeviceNoStr);
			}
			if (rule)
			{
				memcpy(rule, RecData + devicenolen, devicerulllen);
			}
		}	
	}

	return ret;
}

/*************************************************
  Function:			logic_get_netdoor_version
  Description:		���������ǰ���ں˺ͳ���汾��
  Input: 			��
  Output:			
    1.KerVer		�ں˰汾��
  	2.BuildTime		����ʱ��
  	3.ver			����汾��
  Return:			ECHO_OK: 0
  					ECHO_ERROR:0x030001
  					ECHO_MAC:0x030009
  					����:0x0300**
  Others:
*************************************************/
int32 logic_get_netdoor_version(FWM_VERSION_T * KerVer, char * BuildTime, char * ver)
{
	uint32 len = sizeof(FWM_VERSION_T);
	char RecData[200] = {0};									
	uint32 ReciSize;
	uint32 ret;
	
	ret = send_netdoor_cmd_echodata(CMD_GATEWAY_GET_VER, NULL, 0, RecData, &ReciSize);
	if (!ret)
	{
		if (RecData)
		{
			if (KerVer)
			{
				memcpy(KerVer, RecData, len);
			}
			
			if (BuildTime)
			{
				memcpy(BuildTime, RecData + len, 80);
			}
		
			if (ver)
			{
				memcpy(ver, RecData + len + 80, ReciSize - len - 80);
			}
			
			dprintf("logic_get_netdoor_version : len = %d ,kervel->u32algver = %d, BuildTime = %s ver = %s \n", len, KerVer->u32AlgVer, BuildTime, ver);
		}
	}
	return ret;
}

/*************************************************
  Function:			logic_mng_netdoor_card
  Description:		������Ϣ
  Input: 	
  	1.OperType		��������
    2.CardNo		����ָ��
  Output:			��
  Return:			ECHO_OK: 0
  					ECHO_ERROR:0x030001/196609
  					ECHO_MAC:0x030009/196617
  					NETDOOR_ERR_SEND:0x030000/196608		
  Others:
*************************************************/
int32 logic_mng_netdoor_card(uint8 OperType, char * CardNo)
{
	int32 ret = 0;
	unsigned char SendData[4] = {0};
	SendData[0] = OperType;
	if (CardNo == NULL)
	{
		memset(SendData + 1, 0, 3);
	}
	else
	{
		memcpy(SendData + 1, CardNo, 3);
	}
	ret = send_netdoor_cmd_noecho(CMD_NETDOOR_MNG_CARD, SendData, 4);
	dprintf("logic_mng_netdoor_card : ret = %d \n", ret);
	return ret;
}

/*************************************************
  Function:			logic_get_netdevice_lockinfo
  Description:		�������Ϣ
  Input: 			��
  Output:			
  	1.LockType		������
  	2.LockTime		��ʱ��
  Return:			ECHO_OK: 0
  					ECHO_ERROR:0x030001/196609
  					ECHO_MAC:0x030009/196617
  					NETDOOR_ERR_SEND:0x030000/196608		
  Others:
*************************************************/
int32 logic_get_netdevice_lockinfo(uint8 * LockType, int32 * LockTime)
{
	char RecData[100] = {0};									
	uint32 ReciSize;
	uint32 ret;
	
	ret = send_netdoor_cmd_echodata(CMD_NETDOOR_GET_LOCK, NULL, 0, RecData, &ReciSize);
	if (!ret)
	{
		if (ReciSize == 5)
		{
			if (RecData) 
			{
				memcpy(LockType, RecData, 1);
				memcpy(LockTime, RecData + 1, 4);
				dprintf("logic_get_netdevice_lockinfo: LockType = %d , LockTime = %d \n", *LockType, *LockTime);
			}
		}
	}

	return ret;
}

/*************************************************
  Function:			logic_set_netdevice_lockinfo
  Description:		��������Ϣ
  Input: 	
  	1.LockType		������
    2.LockTime		��ʱ��
  Output:			��
  Return:			ECHO_OK: 0
  					ECHO_ERROR:0x030001/196609
  					ECHO_MAC:0x030009/196617
  					NETDOOR_ERR_SEND:0x030000/196608		
  Others:
*************************************************/
int32 logic_set_netdevice_lockinfo(uint8 LockType, int32 LockTime)
{
	int32 ret = 0;
	unsigned char SendData[5] = {0};
	SendData[0] = LockType;
	memcpy(SendData + 1, &LockTime, 4);
	dprintf("logic_set_netdevice_lockinfo: LockType = %d , LockTime = %d \n", LockType, LockTime);
	ret = send_netdoor_cmd_noecho(CMD_NETDOOR_SET_LOCK, SendData, 5);
	return ret;
}

/*************************************************
  Function:    		get_netdevice_mac
  Description:		���ָ�������豸��Ӳ����ַ
  Input: 			
  	1.ip			�豸IP��ַ
  Output:			
	1.HWaddr		��õ�Ӳ����ַ
  Return:			�ɹ����, TRUE/FALSE
  Others:			�ɻ������mac��������ǰ��mac,����IP��ַ��
  					��ͬ
*************************************************/
static uint32 get_netdevice_mac(uint32 IP, unsigned char * HWaddr)
{
	int32 ret = NETDOOR_ERR_ECHO;
	uint8 EchoValue;
	int32 RecSize;
	char RecData[100] = {0};

	if (HWaddr == NULL)
	{
		return NETDOOR_ERR_ECHO;
	}
	if (IP == 0)
	{
		return NETDOOR_ERR_ECHO;
	}

	ret = net_send_command(CMD_GATEWAY_GET_MAC, NULL, 0, IP, NETCMD_UDP_PORT, ND_WAIT_TIME, &EchoValue, RecData, &RecSize);
	if (ret)
	{
		if (!EchoValue)
		{
			memcpy(HWaddr, RecData, 6);
			dprintf("get_netdevice_mac : EchoValue = %d \n netdeviceMAC = %02X-%02X-%02X-%02X-%02X-%02X\n", \
				EchoValue, HWaddr[0], HWaddr[1], HWaddr[2], HWaddr[3], HWaddr[4], HWaddr[5]);
			ret = NETDOOR_ECHO_SUCESS;
		}
		else
		{
			ret = NETDOOR_ERR_ECHO;
		}
	}
	else
	{
		ret = NETDOOR_ERR_ECHO;
	}
	dprintf("get_netdevice_ma ret : %x \n", ret );
	
	return ret;	
}

/*************************************************
  Function:    		get_netdoor_mac
  Description:		���������ǰ����MAC��ַ
  Input: 			
    1.index			������ǰ����ID
  Output:			
	1.HWaddr		��õ�Ӳ����ַ
  Return:			�ɹ����, TRUE/FALSE
  Others:			�ڽ�����ǰ������ʱ���ȵ��ô˺���
*************************************************/
uint32 logic_get_netdoor_mac(uint8 index, unsigned char * HWaddr)
{
	uint32 ret = NETDOOR_ERR_ECHO;
	unsigned char mac[6] = {0};
	
	ret = get_netdevice_mac(NETDOOR_FAULT_IP, mac);
	dprintf("logic_get_netdoor_mac : netdoor default ip = 0x%x, ret = %d\n", NETDOOR_FAULT_IP, ret);	
	if (NETDOOR_ECHO_SUCESS == ret)
	{
		if (HWaddr)
		{
			memcpy(HWaddr, mac, 6);
		}
		dprintf("index: %d \n", index);
		g_NetdoorIp = NETDOOR_FAULT_IP;
		memcpy(g_NetdoorMAC, mac, 6);

		// ��MAC���浽�ļ���
		if (index == 1)
		{
			storage_set_mac(DOOR1_MAC, mac);
			dprintf("storage_set_mac(DOOR1_MAC, mac):%x \n", mac[5]);
		}
		else if (index == 2)
		{
			storage_set_mac(DOOR2_MAC, mac);
			dprintf("storage_set_mac(DOOR2_MAC, mac): %x \n", mac[5]);

		}
		return NETDOOR_ECHO_SUCESS;
	}
	
	return NETDOOR_ERR_ECHO;
}

/*************************************************
  Function:			logic_set_netdoor_other_settings
  Description:		�����豸��������
  Input: 				
    1.OtherSettings	����ֵ 0:������,1:����
  Output:			��
  Return:			ECHO_OK: 0
  					ECHO_ERROR:0x030001
  					ECHO_MAC:0x030009
  					���ݷ�������:0x030000
  Others:			�����СΪ10,�����һ��Ԫ��Ϊ�������õ�ֵ,
  					�ڶ���Ԫ��Ϊ�������õ�ֵ,������Ԫ��Ϊ����
  					���ĸ�Ԫ��Ϊ��Ƶ��ʽ
*************************************************/
uint32 logic_set_netdoor_other_settings(unsigned char * OtherSettings)
{
	int32 ret;
	unsigned char OtherSettingData[10] = {0};

	if (OtherSettings == NULL)
	{
		return NETDOOR_ERR_SEND;
	}
	
	memcpy(OtherSettingData, OtherSettings, sizeof(OtherSettingData));
	ret = send_netdoor_cmd_noecho(CMD_GATEWAY_SET_OTHERS, OtherSettingData, sizeof(OtherSettingData));
	return ret;	
}

/*************************************************
  Function:			logic_get_netdoor_other_settings
  Description:		��������豸��������
  Input: 				
  Output:			
  	1.OtherSettings	��������ֵ
  Return:			ECHO_OK: 0
  					ECHO_ERROR:0x030001
  					ECHO_MAC:0x030009
  					���ݷ�������:0x030000
  					����:0x0300**
  Others:			
*************************************************/
uint32 logic_get_netdoor_other_settings(unsigned char * OtherSettings)
{
	char RecData[100] = {0};									
	uint32 ReciSize;
	uint32 ret;
	int32 len = 10;
	
	ret = send_netdoor_cmd_echodata(CMD_GATEWAY_GET_OTHERS, NULL, 0, RecData, &ReciSize);
	if (!ret)
	{
		if (ReciSize == len)
		{
			if (RecData) 
			{
				memcpy(OtherSettings, RecData, len);	
				dprintf("logic_get_netdoor_other_settings : IsShoot = %d, IsMonitored = %d \n", OtherSettings[0], OtherSettings[1]);
			} 
		}
	}

	return ret;	
}

/*************************************************
  Function:    		get_netdoor_ip
  Description:		���������ǰ��IP��ַ
  Input:			������ǰ��������
  Output:			
  Return:			FALSE / IP��ַ
  Others:			
*************************************************/
static uint32 get_netdoor_ip(uint8 index)
{
	PFULL_DEVICE_NO dev = storage_get_devparam();
	uint32 LocalAreaCode = dev->AreaNo;
	char DeviceNo[20] = {0};
	uint32 * IPs = NULL;
	int32 count = 0;

	// ��������豸��
	strncpy(DeviceNo, dev->DeviceNoStr, dev->DevNoLen - 1);
	if (index == 1)
	{
		DeviceNo[dev->DevNoLen-1] = '8';
	}
	else if(index == 2)
	{
		DeviceNo[dev->DevNoLen-1] = '9';
	}
	else
	{
		return FALSE;
	}
	DeviceNo[dev->DevNoLen] = 0;

	// ��ȡIP��ַ
	count = net_get_ips(LocalAreaCode, DEVICE_TYPE_ROOM, DeviceNo, &IPs);
	if (0 == count)
	{
		printf("get_netdoor_ip : cann't netdoor ip!\n");
		return FALSE;														
	}
	else
	{
		dprintf("get_netdoor_ip : ip = %d \n", IPs[0]);
		return IPs[0];
	}
}

/*************************************************
  Function:    		logic_netdoor_pub_ini
  Description:		��ʼ��:���������ǰ����ַ��MAC��ַ
  Input: 			��
  Output:			
  Return:			�ɹ����, TRUE/FALSE
  Others:			�����ն������ؽ�������֮ǰҪ���ô˺���
*************************************************/
uint32 logic_netdoor_pub_ini(uint8 index)
{
	uint32 ret = 0;
	uint32 NetdoorIp = 0;
	unsigned char NetdoorMac[10] = {0};
	
	NetdoorIp = get_netdoor_ip(index);
	if (index == 1)
	{
		memcpy(NetdoorMac, storage_get_mac(DOOR1_MAC), 6);
	}
	else
	{
		memcpy(NetdoorMac, storage_get_mac(DOOR2_MAC), 6);
	}
	g_NetdoorIp = NetdoorIp;
	memcpy(g_NetdoorMAC, NetdoorMac, 6);
	
	if (!NetdoorIp)
	{
		dprintf("logic_netdoor_pub_ini : can not get NetdoorIp  !!\n");
		return NETDOOR_ERR_ECHO;
	}
	return NETDOOR_ECHO_SUCESS;	
}

