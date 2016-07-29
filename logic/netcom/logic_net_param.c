/*************************************************
  Copyright (C), 2006-2010, Aurine
  File name: 	logic_net_param.c
  Author:    	xiewr
  Version:   	1.0
  Date: 
  Description:  ��ȡIP��ַ��Ϣ
				
  History:        
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#include <netinet/in.h> 
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>
#include <time.h>
#include "lwip/ip.h"
#include "lwip/dns.h"
#include "ite/itp.h"
#include "logic_net_param.h"


#define DHCP_TIMEOUT_MSEC       (60 * 1000) //60sec

static bool networkIsReady;
static bool networkHeartbeatIsReady;
static time_t networkLastRecvTime;
static int networkSocket;
static bool networkToReset, networkQuit;
static bool networkbindflg;								// ���ſڻ��� ��ʱ����IP
static NET_PARAM g_BindNetParam;						// �����õ��������

bool NetworkIsReady(void)
{
    return networkIsReady;
}

static void ResetEthernet(void)
{
	ITPEthernetSetting setting[CFG_NET_ETHERNET_COUNT];
    ITPEthernetInfo info;
    unsigned long mscnt = 0;
    char buf[16];
	int i;
    char* saveptr;

	for (i = 0; i < CFG_NET_ETHERNET_COUNT; i++)
	{
		memset(&setting[i], 0, sizeof (ITPEthernetSetting));
		setting[i].index = i;
	}

    // dhcp
	setting[0].dhcp = 0;

    // autoip
	setting[0].autoip = 0;

    // ipaddr   
    PNET_PARAM NetParam = storage_get_netparam();
	uint32 n_ip = htonl(NetParam->IP);
	//strcpy(buf, ipaddr_ntoa((const ip_addr_t *)&n_ip));
	strcpy(buf, inet_ntoa(n_ip));
	debug_log("NetParam->IP: %x, buf: %s\n", NetParam->IP, buf);
	setting[0].ipaddr[0] = atoi(strtok_r(buf, ".", &saveptr));
	setting[0].ipaddr[1] = atoi(strtok_r(NULL, ".", &saveptr));
	setting[0].ipaddr[2] = atoi(strtok_r(NULL, ".", &saveptr));
	setting[0].ipaddr[3] = atoi(strtok_r(NULL, " ", &saveptr));

    // netmask
    uint32 n_subnet = htonl(NetParam->SubNet);
    //strcpy(buf, ipaddr_ntoa((const ip_addr_t *)&n_subnet));
    strcpy(buf, inet_ntoa(n_subnet));
	debug_log("buf: %s\n", buf);
	setting[0].netmask[0] = atoi(strtok_r(buf, ".", &saveptr));
	setting[0].netmask[1] = atoi(strtok_r(NULL, ".", &saveptr));
	setting[0].netmask[2] = atoi(strtok_r(NULL, ".", &saveptr));
	setting[0].netmask[3] = atoi(strtok_r(NULL, " ", &saveptr));

    // gateway
    uint32 n_gate = htonl(NetParam->DefaultGateway);
    //strcpy(buf, ipaddr_ntoa((const ip_addr_t *)&n_gate));
    strcpy(buf, inet_ntoa(n_gate));
	debug_log("buf: %s\n", buf);
	setting[0].gw[0] = atoi(strtok_r(buf, ".", &saveptr));
	setting[0].gw[1] = atoi(strtok_r(NULL, ".", &saveptr));
	setting[0].gw[2] = atoi(strtok_r(NULL, ".", &saveptr));
	setting[0].gw[3] = atoi(strtok_r(NULL, " ", &saveptr));

	ioctl(ITP_DEVICE_ETHERNET, ITP_IOCTL_RESET, &setting[CFG_NET_ETHERNET_COUNT - 1]);

	// ����mac 
	char macaddr[6];
	memcpy(macaddr, storage_get_mac(HOUSE_MAC), 6);
	debug_log("macaddr: %02x:%02x:%02x:%02x:%02x:%02x\n", macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);
	ioctl(ITP_DEVICE_ETHERNET, ITP_IOCTL_WIRTE_MAC, macaddr);
	
	warn_log("Wait ethernet cable to plugin...\n");
    while (!ioctl(ITP_DEVICE_ETHERNET, ITP_IOCTL_IS_CONNECTED, NULL))
    {
        sleep(1);
		mscnt += 100;
        putchar('.');
        fflush(stdout);

		if (setting[0].dhcp == true)
		{
			if (mscnt >= DHCP_TIMEOUT_MSEC)
			{
				printf("\nDHCP timeout, use default settings\n");
				setting[0].dhcp = setting[0].autoip = 0;
				ioctl(ITP_DEVICE_ETHERNET, ITP_IOCTL_RESET, &setting[0]);
				break;
			}
		}
    }

    warn_log("\nWait DHCP settings");
    while (!ioctl(ITP_DEVICE_ETHERNET, ITP_IOCTL_IS_AVAIL, NULL))
    {
        usleep(100000);
        mscnt += 100;

        putchar('.');
        fflush(stdout);

        if (mscnt >= DHCP_TIMEOUT_MSEC)
        {
            printf("\nDHCP timeout, use default settings\n");
			setting[0].dhcp = setting[0].autoip = 0;
			ioctl(ITP_DEVICE_ETHERNET, ITP_IOCTL_RESET, &setting[0]);
            break;
        }
    }
    puts("");

    if (ioctl(ITP_DEVICE_ETHERNET, ITP_IOCTL_IS_AVAIL, NULL))
    {
        char* ip;

		for(i =0; i<CFG_NET_ETHERNET_COUNT; i++)
		{
	        info.index = i;
        	ioctl(ITP_DEVICE_ETHERNET, ITP_IOCTL_GET_INFO, &info);
        	ip = ipaddr_ntoa((const ip_addr_t*)&info.address);

			printf("IP address[%d]: %s \r\n", i, ip);
		}
        networkIsReady = true;
    }
}

static void BindSetEthernet(void)
{
	ITPEthernetSetting setting[CFG_NET_ETHERNET_COUNT];
    ITPEthernetInfo info;
    unsigned long mscnt = 0;
    char buf[16];
	int i;
    char* saveptr;

	for (i = 0; i < CFG_NET_ETHERNET_COUNT; i++)
	{
		memset(&setting[i], 0, sizeof (ITPEthernetSetting));
		setting[i].index = i;
	}

    // dhcp
	setting[0].dhcp = 0;

    // autoip
	setting[0].autoip = 0;

    // ipaddr   
	uint32 n_ip = htonl(g_BindNetParam.IP);
	strcpy(buf, ipaddr_ntoa((const ip_addr_t *)&n_ip));
	debug_log("g_BindNetParam.IP: %x, buf: %s\n", g_BindNetParam.IP, buf);
	setting[0].ipaddr[0] = atoi(strtok_r(buf, ".", &saveptr));
	setting[0].ipaddr[1] = atoi(strtok_r(NULL, ".", &saveptr));
	setting[0].ipaddr[2] = atoi(strtok_r(NULL, ".", &saveptr));
	setting[0].ipaddr[3] = atoi(strtok_r(NULL, " ", &saveptr));

    // netmask
    uint32 n_subnet = htonl(g_BindNetParam.SubNet);
    strcpy(buf, ipaddr_ntoa((const ip_addr_t *)&n_subnet));
	debug_log("buf: %s\n", buf);
	setting[0].netmask[0] = atoi(strtok_r(buf, ".", &saveptr));
	setting[0].netmask[1] = atoi(strtok_r(NULL, ".", &saveptr));
	setting[0].netmask[2] = atoi(strtok_r(NULL, ".", &saveptr));
	setting[0].netmask[3] = atoi(strtok_r(NULL, " ", &saveptr));

    // gateway
    uint32 n_gate = htonl(g_BindNetParam.DefaultGateway);
    strcpy(buf, ipaddr_ntoa((const ip_addr_t *)&n_gate));
	debug_log("buf: %s\n", buf);
	setting[0].gw[0] = atoi(strtok_r(buf, ".", &saveptr));
	setting[0].gw[1] = atoi(strtok_r(NULL, ".", &saveptr));
	setting[0].gw[2] = atoi(strtok_r(NULL, ".", &saveptr));
	setting[0].gw[3] = atoi(strtok_r(NULL, " ", &saveptr));

	ioctl(ITP_DEVICE_ETHERNET, ITP_IOCTL_RESET, &setting[CFG_NET_ETHERNET_COUNT - 1]);

	warn_log("Wait ethernet cable to plugin...\n");
    while (!ioctl(ITP_DEVICE_ETHERNET, ITP_IOCTL_IS_CONNECTED, NULL))
    {
        sleep(1);
		mscnt += 100;
        putchar('.');
        fflush(stdout);

		if (setting[0].dhcp == true)
		{
			if (mscnt >= DHCP_TIMEOUT_MSEC)
			{
				printf("\nDHCP timeout, use default settings\n");
				setting[0].dhcp = setting[0].autoip = 0;
				ioctl(ITP_DEVICE_ETHERNET, ITP_IOCTL_RESET, &setting[0]);
				break;
			}
		}
    }

    warn_log("\nWait DHCP settings");
    while (!ioctl(ITP_DEVICE_ETHERNET, ITP_IOCTL_IS_AVAIL, NULL))
    {
        usleep(100000);
        mscnt += 100;

        putchar('.');
        fflush(stdout);

        if (mscnt >= DHCP_TIMEOUT_MSEC)
        {
            printf("\nDHCP timeout, use default settings\n");
			setting[0].dhcp = setting[0].autoip = 0;
			ioctl(ITP_DEVICE_ETHERNET, ITP_IOCTL_RESET, &setting[0]);
            break;
        }
    }
    puts("");

    if (ioctl(ITP_DEVICE_ETHERNET, ITP_IOCTL_IS_AVAIL, NULL))
    {
        char* ip;

		for(i =0; i<CFG_NET_ETHERNET_COUNT; i++)
		{
	        info.index = i;
        	ioctl(ITP_DEVICE_ETHERNET, ITP_IOCTL_GET_INFO, &info);
        	ip = ipaddr_ntoa((const ip_addr_t*)&info.address);

			printf("IP address[%d]: %s \r\n", i, ip);
		}
        networkIsReady = true;
    }
}

static void* NetworkTask(void* arg)
{  
	ResetEthernet();
	
    while (!networkQuit)
    {
        networkIsReady = ioctl(ITP_DEVICE_ETHERNET, ITP_IOCTL_IS_CONNECTED, NULL);

        if (networkToReset)
        {
        	if (networkbindflg == true)
        	{
        		BindSetEthernet();
            	networkbindflg = false;
        	}
			else
			{
				ResetEthernet();
			}			
            networkToReset = false;
        }

        sleep(1);
    }
    return NULL;
}

/*************************************************
  Function:    		NetworkInit
  Description:		�����ʼ��
  Input: 			
  Output:			��
  Return:			��
  Others:
*************************************************/
void NetworkInit(void)
{
    pthread_t task;
    pthread_attr_t attr;
    int ret = 0;

    networkIsReady          = false;
    networkToReset          = false;
    networkQuit             = false;
	networkbindflg          = false;
    networkSocket           = -1;
    networkLastRecvTime     = time(NULL);	
	
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&task, &attr, NetworkTask, NULL);
}

void NetworkExit(void)
{
    networkQuit = true;
}

void NetworkReset(void)
{
    networkToReset  = true;
}

int NetworkGetInfo(void)
{
	ITPEthernetInfo info;
    info.index = 0;
    ioctl(ITP_DEVICE_ETHERNET, ITP_IOCTL_GET_INFO, &info);
	debug_log("info.address: %x, info.netmask: %x, info.displayName: %s\n", info.address, info.netmask, info.displayName);
}

/*************************************************
  Function:    		IPtoUlong
  Description:		��IP��ַ�ַ���ת��Ϊlong��IP��ַ
  Input:
  	1.ip			IP��ַ�ַ���
  Output:			
  Return:			ת�����unsigned long��IP��ַ
  Others:			
*************************************************/
uint32 IPtoUlong(char* ip)
{
	#if 1
	if (ip == NULL)
	{
		return FALSE;
	}
	
	struct in_addr sin_addr;
	if (!inet_aton(ip, &sin_addr))
	{
		err_log("bad ip addr !!!! \n");
		return FALSE;
	}
	uint32 address = ntohl(sin_addr.s_addr);
	return address;
	#else
	uint32 uIp=0;
	char *p, *q;
	char temp[10];
	int32 num;
	p = ip;
	for( ; ; )
	{
		q = (char *)strchr(p,'.');
		if (q == NULL)
		{
			break;
		}
		num = q - p;
		memset(temp, 0, 10);
		memcpy(temp, p, num);
		uIp = uIp << 8;
		uIp += atoi(temp);
		p=q+1;
	}
	uIp = uIp << 8;
	uIp += atoi(p);
	return uIp;
	#endif
}

/*************************************************
  Function:    		UlongtoIP
  Description:		��long��IP��ַת��ΪIP��ַ�ַ���
  Input:
  	1.uIp			unsigned long��IP��ַ
  Output:			
  Return:			ת������ַ���ָ��
  Others:			����NULL ��ʾIP��ַ����
*************************************************/
char* UlongtoIP(uint32 uIp)
{
	#if 1
	uint32 n_ip = htonl(uIp);
  	return inet_ntoa(n_ip);
	#else
	static char strIp[20];
	char temp[20];
	int32 i;
	uint32 t;
	memset(strIp, 0, 20);
	for(i = 3; i >= 0; i--)
	{
		t = uIp >> (i * 8);
		t = t & 0xFF;
		sprintf(temp, "%d", (uint32)t);
		strcat(strIp, temp);
		if(i != 0)
		{
			strcat(strIp, ".");
		}
	}
	return strIp;
	#endif
}

/*************************************************
  Function:    		IPIsCorrect
  Description:		IP�Ƿ�����
  Input:
  	1.ip			IP��ַ�ַ���
  Output:			
  Return:			
  Others:			
*************************************************/
int IPIsCorrect(char* ip)
{
	if (ip == NULL)
	{
		return FALSE;
	}

	int i = 0, num = 0;
	char *q = NULL;
	char *p = ip;
	
	for(i=0; i<4; i++)
	{
		q = (char *)strchr(p,'.');
		if (q != NULL)
		{
			num++;
			p=q+1;
		}		
	}

	if (num != 3)
	{
		return FALSE;
	}
	return IPtoUlong(ip);
}

/*************************************************
Function:   	 	storage_set_ipaddr
Description: 		��ʼ��ip
Input:              ��
Output:				��
Return:				�ɹ���ture ʧ�ܣ�false
Others:
*************************************************/
uint32 net_set_ipaddr(void)
{
	NetworkReset();
	return 0;
}

/*************************************************
Function:   	 	net_set_bind_ip
Description: 		���ú�������ǰ������IP
Input:              ��
Output:				��
Return:				�ɹ���ture ʧ�ܣ�false
Others:
*************************************************/
uint32 net_set_bind_ip(uint32 ip, uint32 netmack, uint32 gateway)
{
	memset(&g_BindNetParam, 0, sizeof(NET_PARAM));
	g_BindNetParam.IP = ip;
	g_BindNetParam.DefaultGateway = gateway;
	g_BindNetParam.SubNet = netmack;
	
	networkbindflg = true;
	NetworkReset();
}

/*************************************************
  Function:    		net_set_local_param
  Description:		���ñ����������
  Input: 			
  	1.NetParam		�������ָ��
  Output:			��
  Return:			��
  Others:
*************************************************/
void net_set_local_param(void)
{
	net_set_ipaddr();
}

/*************************************************
  Function:    		net_set_local_mac
  Description:		����MAC
  Input: 			
  	1.mac			MAC
  Output:			��
  Return:			��
  Others:			TRUE / FALSE
*************************************************/
uint32 net_set_local_mac(uint8 *mac)
{	
	uint32 ret = FALSE;

	// �洢����MAC
	ret = storage_set_mac(HOUSE_MAC, (char *)mac);
	if (TRUE == ret)
	{
		net_set_ipaddr();
	}
	return ret;
}



