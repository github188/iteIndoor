/*************************************************
  Copyright (C), 2006-2016, Aurine
  File name:  	logic_dianti.c
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
#include "logic_include.h"


#ifdef _USE_ELEVATOR_

// mody by caogw 15-05-08 Ϊ�˽���豸��Ź���ʱ
// ��ŷ��Ż�ȡ���������(�̶�4λ����)
/*************************************************
  Function:		get_floor
  Description:	��ȡ���
  Input:		��
  Output:		��	
  Return:		���
  Others:	
*************************************************/
static uint8 get_floor(void)
{
	char tmp[2];
	int floorno = -1;
	
	PFULL_DEVICE_NO dev = storage_get_devparam();
	uint8 StairNoLen = dev->Rule.StairNoLen;
	tmp[0] = dev->DeviceNoStr[StairNoLen];
	tmp[1] = dev->DeviceNoStr[StairNoLen+1];
	floorno = atoi(tmp);
	
	return floorno;
}

/*************************************************
  Function:		get_room
  Description:	��ȡ����
  Input:		��
  Output:		��	
  Return:		δ2λ����
  Others:	
*************************************************/
static uint8 get_room(void)
{
	char tmp[2];
	int roomno = -1;
	
	PFULL_DEVICE_NO dev = storage_get_devparam();
	uint8 StairNoLen = dev->Rule.StairNoLen;
	tmp[0] = dev->DeviceNoStr[StairNoLen+2];
	tmp[1] = dev->DeviceNoStr[StairNoLen+3];
	roomno = atoi(tmp);
	
	return roomno;
}

#ifdef _DIANTI_SHUANKAI_
/*************************************************
  Function:		get_room_flag
  Description:	�жϷ�����ż��־
  Input:		��
  Output:		��	
  Return:		δ2λ����
  Others:	
*************************************************/
static uint8 get_room_flag(void)
{
	uint8 flag;
	char tmp[2];
	int roomno = -1;
	
	PFULL_DEVICE_NO dev = storage_get_devparam();
	uint8 StairNoLen = dev->Rule.StairNoLen;
	tmp[0] = dev->DeviceNoStr[StairNoLen+2];
	tmp[1] = dev->DeviceNoStr[StairNoLen+3];
	roomno = atoi(tmp);
	flag = roomno % 2;
	
	return flag;
}
#endif

/*************************************************
  Function:		get_dianti_ip
  Description:	��ȡ���ݿ�����IP��ַ
  Input:		��
  Output:		��	
  Return:		IP��ַ
  Others:	
*************************************************/
uint32 get_dianti_ip(void)
{
	uint32 ip;

	ip = storage_get_netparam_bytype(DIANTI_IPADDR);
	return ip;
}

/*************************************************
  Function:		dianti_set_cmd
  Description:	������������ݿ�����
  Input:
  Output:
  Return:		��
  Others:		��	
*************************************************/
void dianti_set_cmd(uint8 direction)
{
	int32 sockFd;
	uint32 ipAddr;
	uint8 size;
	uint8 cmdbuf[5] = {0x02, 0x05, 0x01, 0x00, 0x00};
	struct sockaddr_in destAddr;
   	
	sockFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sockFd == -1)
	{
  		dprintf("socket err\n");
  		return;
	}

	#ifdef _DIANTI_SHUANKAI_						// add by luofl 2011-08-11 ����˫����
	if (get_room_flag())
	{
		cmdbuf[1] = ELEVATOR_DOWN;					
	}
	else
	{
		cmdbuf[1] = ELEVATOR_UP;
	}
	#else
	cmdbuf[1] = direction;
	#endif
	
	cmdbuf[2] = get_floor();						// ���
	cmdbuf[3] = get_room();							// ����

	// �������ӵĵ�ַ���˿�
	ipAddr = get_dianti_ip();
	destAddr.sin_family = AF_INET;
	destAddr.sin_addr.s_addr = htonl(ipAddr);		// Ҫ���������ֽ�˳��ת��, ���ֽ���ǰ,���ֽ��ں�
	destAddr.sin_port = htons(DIANTI_UDP_PORT);
	size = sendto(sockFd, cmdbuf, sizeof(cmdbuf), 0, (struct sockaddr *)&destAddr, sizeof(struct sockaddr));	
	if (size != sizeof(cmdbuf))
	{
		dprintf("sendto error!!! size: %d\n", size);
		// ���������ط�����
	}
	usleep(500*1000);
	sendto(sockFd, cmdbuf, sizeof(cmdbuf), 0, (struct sockaddr *)&destAddr, sizeof(struct sockaddr));
	close(sockFd);

	#if 0
	uint8 i;
	for (i = 0; i < sizeof(cmdbuf); i++)
	{
		dprintf("elevator cmdbuf[%d]:  %d\n", i, cmdbuf[i]);
	}
	dprintf("\n");
	#endif
	
  	return;
}

/*************************************************
  Function:		dianti_rec_cmd
  Description:	���յ��ݿ�����������
  Input:		��
  Output:		��	
  Return:		��
  Others:	
*************************************************/
void dianti_rec_cmd(void)
{	
	#if 0
	uint8 recbuf[50] = {0};
	int32 sock;
  	struct sockaddr_in * name;
  	int32 recvlen, sendlen;
  
  	// ����socket������ΪUDP���ӣ�����ΪSOCK_DGRAM
  	// ע��: ���������SOCK_DGRAM����TCP socket������SOCK_DGRAM��ͬ!
  	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  	if(sock == -1) 
	{
    	dprintf("socket err\n");
    	return;
  	}

  	// Ϊbind��׼��
  	name = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
  	if(name == NULL) 
	{
    	dprintf("malloc name fail\n");
    	return;
  	}
  	name->sin_family = AF_INET;
  	name->sin_addr.s_addr = 0x180d6e0a;
  	name->sin_port = 0x0700;

  	// �󶨶˿�
  	bind(sock, (struct sockaddr*)name, sizeof(struct sockaddr));

  	while(1) 
	{
    	dprintf("udpecho_server : recv begin !!\n");
    	recvlen = recv(sock, recbuf, 256, 0);
		dprintf("Recvlen=%d,str:%s\n", recvlen, recbuf);
    	if (recvlen > 0) 
		{
        	do 
			{
          		sendlen=send(sock, recbuf, recvlen, 0);
          		if (sendlen < 0)
          		{
            		dprintf("sendlen == -1,error\n");
           			break;
          		}
          		recvlen -= sendlen;
    		} while(recvlen > 0);
    	}
    	else 
		{
		}
  	}
  	dprintf("Got EOF, looping\n");
	#endif
}

// �Ϻ��쵼�Ƶ��ݿ���
#ifdef _ALAV_LINGKE_
/*************************************************
  Function:		dianti_set_cmd_custom
  Description:	������������ݿ�����
  Input:
  Output:
  Return:		��
  Others:		��	
*************************************************/
void dianti_set_call_custom(char* stairno)
{
	int32 sockFd;
	uint32 ipAddr;
	uint8 floor, size;
	uint8 cmdbuf[5] = {0x02, 0x05, 0x01, 0x00, 0x00};
	struct sockaddr_in destAddr;

	PFULL_DEVICE_NO fulldevno;

	uint8 StairNoLen;   
	fulldevno = storage_get_devparam();
	StairNoLen = fulldevno->Rule.StairNoLen;

	dprintf("dianti_set_call_custom: %s\n", stairno);
	sockFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sockFd == -1)
	{
  		dprintf("socket err\n");
  		return;
	}
	
	cmdbuf[1] = ELEVATOR_CALL;
	
	cmdbuf[2] = get_floor();
	if (stairno != NULL)
	{
		cmdbuf[3] = atoi(stairno+StairNoLen);
	}
	else
	{
		cmdbuf[3] = 0;
	}
	
	ipAddr = get_dianti_ip();
	// �������ӵĵ�ַ���˿�
	destAddr.sin_family = AF_INET;
	destAddr.sin_addr.s_addr = htonl(ipAddr);		// Ҫ���������ֽ�˳��ת��, ���ֽ���ǰ,���ֽ��ں�
	destAddr.sin_port = htons(DIANTI_UDP_PORT);
	size = sendto(sockFd, cmdbuf, sizeof(cmdbuf), 0, (struct sockaddr *)&destAddr, sizeof(struct sockaddr));	
	if (size != sizeof(cmdbuf))
	{
		dprintf("sendto error!!! size: %d\n", size);
		// ���������ط�����
	}
	usleep(500*1000);
	sendto(sockFd, cmdbuf, sizeof(cmdbuf), 0, (struct sockaddr *)&destAddr, sizeof(struct sockaddr));
	close(sockFd);

	dprintf("elevator direct: %d\n", cmdbuf[1]);
	dprintf("elevator floor: %d\n", cmdbuf[2]);
	dprintf("elevator room: %d\n", cmdbuf[3]);
  	return;
}
#endif
#endif

