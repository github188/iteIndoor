/*************************************************
  Copyright (C), 2006-2010, Aurine
  File name: 	logic_interphone.c
  Author:    	xiewr
  Version:   	1.0
  Date: 
  Description:  �Խ�/���ӹ��ýӿ�
				
  History:        
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#include "logic_include.h"


//�ݿڻ���ַ:20���ݿڻ�,���0-19
static uint32 g_StairAddress[STAIR_DEVICE_NUM]  =  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

//���ڻ���ַ:99�����ڻ�,���1-99
static uint32 g_AreaAddress[AREA_DEVICE_NUM + 1] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
												  	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										 		  	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
										 		  	0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
										 		  	
// ������ǰ����ַ:2����ǰ��,���1-2
static uint32 g_NetDoorAddress[DOOR_DEVICE_NUM + 1] = {0, 0, 0};

// ģ����ǰ����ַ: 1 2
static uint32 g_PhoneDoorAddress[DOOR_DEVICE_NUM + 1] = {0, 0, 0};

static uint32 g_Stair0Address = 0;
static uint32 g_Stair0AddressFlg = FALSE;

static PMONITORLISTINFO g_monitorlist = NULL;


/*************************************************
  Function:			get_stair_fullno
  Description:		����ݺ�
  Input: 			
  	1.StairNo		�ݿں�
  	2.ExtNo			�ֻ���
  Output:			��
  	1.StairNo		�ݿں� 
  Return:			�ݺ��ַ���
  Others:
*************************************************/
char * get_stair_fullno(char * StairNo, char ExtNo)
{
	PFULL_DEVICE_NO fulldevno;
	if (!StairNo)
	{
		return NULL;
	}
	
	fulldevno = storage_get_devparam();
	strncpy(StairNo, fulldevno->DeviceNoStr, fulldevno->Rule.StairNoLen);
	
	StairNo[fulldevno->Rule.StairNoLen] = ExtNo;	
	StairNo[fulldevno->Rule.StairNoLen + 1] = 0;	
	return StairNo;
}

/*************************************************
  Function:			set_stair_address
  Description:		�����ݿڻ���ַ
  Input: 			
  	1.no			��� 0-9
  	2.address		��ַ
  Output:			��
  Return:			��
  Others:
*************************************************/
void set_stair_address(int32 no, uint32 address)
{
	if (no >= 0 && no < STAIR_DEVICE_NUM)
	{
		g_StairAddress[no] = address;
	}
}

/*************************************************
  Function:			get_stair_address
  Description:		���б��л���ݿڻ���ַ,δ�ҵ����㲥��ȡ
  Input: 			
  	1.no			��� 0-9, 0xFFΪ����
  Output:			��
  Return:			��ַ
  Others:
*************************************************/
uint32 get_stair_address_inlist(int32 no)
{
	if (no < 0 || no >= STAIR_DEVICE_NUM)
	{
		return 0;
	}
	
	return g_StairAddress[no];
}

/*************************************************
  Function:			get_stair_address
  Description:		���ָ���ݿڻ���ַ
  Input: 			
  	1.no			��� 0-9, 0xFFΪ����
  Output:			��
  Return:			��ַ
  Others:
*************************************************/
uint32 get_stair_address(int32 no)
{
	if (no < 0 || no >= STAIR_DEVICE_NUM)
	{
		return 0;
	}
	
	if (0 == g_StairAddress[no])
	{
		char DeviceNo[20] = {0};
		uint32 *IPs = NULL;
		int32 n;
		get_stair_fullno(DeviceNo, '0' + no);
		
		n = net_get_ips(storage_get_areano(), DEVICE_TYPE_STAIR, DeviceNo, &IPs);
		if (n)
		{
			g_StairAddress[no] = IPs[0];
		}
	}
	
	return g_StairAddress[no];
}

/*************************************************
  Function:			get_stair_list
  Description:		����ݿڻ���ַ�б�
  Input: 			��
  Output:			
  	1.FirstIndex	��һ���豸����: 0-~
  	2.ipaddr		��һ���豸IP
  Return:			�Ƿ��ȡ���豸IP: TRUE / FALSE
  Others:
*************************************************/
int32 get_stair_list(uint32 *index, uint32 *ipaddr)
{
	PDEVICE_NO DevNoList;
	uint32 * IPs;
	int32 i;
	int32 count = 0;
	uint8 n = 0, temp = 0;
	char DeviceNo[20] = {0};
	
	get_stair_fullno(DeviceNo, '*');
	//get_stair_fullno(DeviceNo, '_');
	dprintf("get_stair_list  DeviceNo: %s\n", DeviceNo);
	memset(g_StairAddress, 0, sizeof(g_StairAddress));			// �����ԭ�ȵ�����
	count = net_get_devices_and_ips(storage_get_areano(), DEVICE_TYPE_STAIR, DeviceNo, &DevNoList, &IPs);
	for (i = 0; i < count; i++)
	{
		#if 0
		sprintf(DeviceNo, "%u", DevNoList[i].DeviceNo2);
		n = strlen(DeviceNo);	
		n = DeviceNo[n-1] - '0';									// �ֻ���
		#else
		temp = DevNoList[i].DeviceNo2 % STAIR_DEVICE_NUM;							// �ֻ���
		if (temp > n)
		{
			n = temp;
		}
		#endif
		g_StairAddress[temp] = IPs[i];
	}

	// ֧��ͨ���ݿڻ� add by luofl 2011-11-03
	#ifdef _TY_STAIR_
	if (storage_get_extmode(EXT_MODE_GENERAL_STAIR))
	{
		for (i = 0; i <= n; i++)
		{
			if ((i%2) == 0)
			{
				if (g_StairAddress[i] > 0 && g_StairAddress[i+1] == 0)
				{
					g_StairAddress[i+1] = g_StairAddress[i];
				}
			}
		}
	}
	#endif
	
	// ��ȡ��һ����Ч�豸��������IP
	for (i = 0; i < STAIR_DEVICE_NUM; i++)
	{
		if (g_StairAddress[i] != 0)
		{
			*index = i;
			*ipaddr = g_StairAddress[i];
			break;
		}
	}
	
	if (count > 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*************************************************
  Function:			set_stair0_addr
  Description:		����ݿڻ�0��ַ
  Input: 			��
  Output:			
  Return:			IPaddr
  Others:
*************************************************/
void set_stair0_addr(void)
{
	PDEVICE_NO DevNoList;
	uint32 * IPs;
	int32 count;
	char DeviceNo[20] = {0};

	dprintf("set_stair0_addr\n"); 
	
	#if (SYS_TYPE == E81_GZBL_VER)
	if(g_Stair0Address <= 0)
	#else
	if (g_Stair0Address <= 0 && !g_Stair0AddressFlg)
	#endif
	{
		get_stair_fullno(DeviceNo, '0');
		dprintf("get_stair_addr : stair no : %s\n", DeviceNo); 
		count = net_get_devices_and_ips(storage_get_areano(), DEVICE_TYPE_STAIR, DeviceNo, &DevNoList, &IPs);
		
		if (count > 0)
		{
			dprintf("get_stair_addr : ipaddr : 0x%x\n", IPs[0]); 
			g_Stair0Address = IPs[0];
		}
		else
		{
			dprintf("get_stair_addr : can not get ip\n"); 
			g_Stair0Address = 0;
		}
		#ifndef _GZBL_MODE_
		g_Stair0AddressFlg = TRUE;
		#endif
	}
}

/*************************************************
  Function:			get_stair0_addr
  Description:		����ݿڻ�0��ַ
  Input: 			��
  Output:			
  Return:			IPaddr
  Others:
*************************************************/
uint32 get_stair0_addr(void)
{
	if (g_StairAddress[0] > 0)
	{
		g_Stair0Address = g_StairAddress[0];
	}

	/*
	if (g_Stair0Address <= 0)
	{
		get_stair_fullno(DeviceNo, '0');
		dprintf("get_stair_addr : stair no : %s\n", DeviceNo); 
		count = net_get_devices_and_ips(storage_get_areano(), DEVICE_TYPE_STAIR, DeviceNo, &DevNoList, &IPs);
		
		if (count > 0)
		{
			dprintf("get_stair_addr : ipaddr : 0x%x\n", IPs[0]); 
			g_Stair0Address = IPs[0];
		}
		else
		{
			dprintf("get_stair_addr : can not get ip\n"); 
			g_Stair0Address = 0;
		}
	}
	*/
	#ifdef _GZBL_MODE_
	else
	{
		set_stair0_addr();
	}
	#endif
	
	return g_Stair0Address;
}



/*************************************************
  Function:			get_door_address
  Description:		�����ǰ����ַ
  Input: 			
  	1.ID			��� 1��2 
  Output:			��
  Return:			�ɹ�����ַ��ʧ�ܣ�0
  Others:			
*************************************************/
uint32 get_door_address(int32 ID)
{
	int32 count = 0;
	uint32 * IPs = NULL;
	uint32 Address = 0;
	PFULL_DEVICE_NO myDev = (PFULL_DEVICE_NO)storage_get_devparam();
	char no[50] = {0};

	if (ID < 1 || ID > DOOR_DEVICE_NUM)
	{
		return 0;
	}	
	else
	{
		uint8 index = 0;
		if (ID == 1)
		{
			index = EXT_MODE_NETDOOR1;
		}
		else if (ID == 2)
		{
			index = EXT_MODE_NETDOOR2;
		}
		if (storage_get_extmode(index) == FALSE)
		{
			return 0;
		}
	}
	
	strncpy(no, myDev->DeviceNoStr, myDev->DevNoLen-1);
	
	if (ID == 1)
	{
		no[myDev->DevNoLen-1] = '8';
	}
	else
	{
		no[myDev->DevNoLen-1] = '9';
	}
	no[myDev->DevNoLen] = 0;

	count = net_get_ips(storage_get_areano(), DEVICE_TYPE_ROOM, no, &IPs);
	if (count)
	{
		Address = IPs[0];
	}
	return Address;
}

/*************************************************
  Function:			get_phonedoor_address
  Description:		�����ǰ����ַ
  Input: 			
  	1.ID			��� 1��2 
  Output:			��
  Return:			�ɹ�����ַ��ʧ�ܣ�0
  Others:			
*************************************************/
uint32 get_phonedoor_address(int32 ID)
{
	if (ID < 1 || ID > DOOR_DEVICE_NUM)
	{
		return FALSE;
	}	
	else
	{
		uint8 index = 0;
		if (ID == 1)
		{
			index = EXT_MODE_PHONEDOOR1;
		}
		else if (ID == 2)
		{
			index = EXT_MODE_PHONEDOOR2;
		}
		if (storage_get_extmode(index) == TRUE)
		{
			return TRUE;
		}
	}

	return FALSE;		
}

/*************************************************
  Function:			get_netdoor_list
  Description:		���������ǰ����ַ�б�
  Input: 			��
  Output:			
  	1.index			��һ�������豸������:1-2
  	2.ipaddr		��һ�������豸��IP
  Return:			TRUE / FALSE
  Others:
*************************************************/
int32 get_netdoor_list(uint32 *index, uint32 *ipaddr)
{
	int32 i;
	int32 count = 0;
	int32 ret = FALSE;
	uint32 * IPs = NULL;
	PFULL_DEVICE_NO myDev = (PFULL_DEVICE_NO)storage_get_devparam();
	char no[50] = {0};

	strncpy(no, myDev->DeviceNoStr, myDev->DevNoLen-1);
	memset(g_NetDoorAddress, 0, sizeof(g_NetDoorAddress));

	for (i = 0; i < 2; i++)
	{
		if (i == 0)
		{
			if (storage_get_extmode(EXT_MODE_NETDOOR1) == FALSE)
			{
				g_NetDoorAddress[i+1] = 0;
				continue;
			}
			else
			{
				no[myDev->DevNoLen-1] = '8';
			}
		}
		else
		{
			if (storage_get_extmode(EXT_MODE_NETDOOR2) == FALSE)
			{
				g_NetDoorAddress[i+1] = 0;
				continue;
			}
			else
			{
				no[myDev->DevNoLen-1] = '9';
			}
		}
		no[myDev->DevNoLen] = 0;
		count = net_get_ips(storage_get_areano(), DEVICE_TYPE_DOOR_NET, no, &IPs);
		if (count)
		{
			g_NetDoorAddress[i+1] = IPs[0];
			ret = TRUE;
		}
	}

	// ��ȡ��һ����Ч�豸��������IP
	for (i = 1; i < DOOR_DEVICE_NUM+1; i++)
	{
		if (g_NetDoorAddress[i] != 0)
		{
			*index = i;
			*ipaddr = g_NetDoorAddress[i];
			break;
		}
	}
	
	return ret;
}

/*************************************************
  Function:			get_netdoor_address_inlist
  Description:		�����ǰ����ַ
  Input: 			
  	1.no			��� 1-2
  Output:			��
  Return:			��ַ
  Others:
*************************************************/
uint32 get_netdoor_address_inlist(int32 no)
{
	if (no < 1 || no > DOOR_DEVICE_NUM)
	{
		return 0;
	}
	return g_NetDoorAddress[no];
}

/*************************************************
  Function:			get_phonedoor_address_inlist
  Description:		�����ǰ����ַ
  Input: 			
  	1.no			��� 1-2
  Output:			��
  Return:			��ַ
  Others:
*************************************************/
uint32 get_phonedoor_address_inlist(int32 no)
{
	if (no < 1 || no > DOOR_DEVICE_NUM)
	{
		return 0;
	}
	return g_PhoneDoorAddress[no];
}

/*************************************************
  Function:			set_area_address
  Description:		������ڻ���ַ
  Input: 			
  	1.no			���: 1-40
  	2.address		��ַ
  Output:			��
  Return:			��
  Others:
*************************************************/
void set_area_address(int32 no, uint32 address)
{
	if (no > 0 && no <= AREA_DEVICE_NUM)
	{
		g_AreaAddress[no] = address;
	}
}

/*************************************************
  Function:			get_stair_address
  Description:		������ڻ���ַ,δ�ҵ����㲥��ȡ
  Input: 			
  	1.no			��� 1-40
  Output:			��
  Return:			��ַ
  Others:
*************************************************/
uint32 get_area_address_inlist(int32 no)
{
	if (no < 1 || no > AREA_DEVICE_NUM)
	{
		return 0;
	}
	return g_AreaAddress[no];
}

/*************************************************
  Function:			get_area_address
  Description:		���ָ�����ڻ���ַ
  Input: 			
  	1.no			���
  Output:			��
  Return:			��ַ
  Others:
*************************************************/
uint32 get_area_address(int32 no)
{
	if (no < 1 || no > AREA_DEVICE_NUM)
	{
		return 0;
	}
	
	if (0 == g_AreaAddress[no])
	{
		char temp[3] = {0};
		uint32 *IPs = NULL;
		int32 n;
		sprintf(temp, "%02d", no);
		n = net_get_ips(storage_get_areano(), DEVICE_TYPE_AREA, temp, &IPs);
		if (n)
		{
			g_AreaAddress[no] = IPs[0];
		}
	}  
	
	return g_AreaAddress[no];
}

/*************************************************
  Function:			get_area_list
  Description:		������ڻ���ַ�б�
  Input: 			��
  Output:			
  	1.index 		��һ�������豸������: 1-40
  	2.ipaddr		��һ�������豸��IP
  Return:			TRUE / FALSE
  Others:
*************************************************/
int32 get_area_list(uint32 *index, uint32 *ipaddr)
{
	PDEVICE_NO DevNoList;
	uint32 * IPs;
	int32 i;
	int32 count;
	
	memset(g_AreaAddress, 0, sizeof(g_AreaAddress));
	count = net_get_devices_and_ips(storage_get_areano(), DEVICE_TYPE_AREA, "__", &DevNoList, &IPs);
	for (i = 0; i < count; i++)
	{
		if (DevNoList[i].DeviceNo2 > 0 && DevNoList[i].DeviceNo2 <= AREA_DEVICE_NUM)
		{
			g_AreaAddress[DevNoList[i].DeviceNo2] = IPs[i];				
		}
		dprintf("get_area_list : g_AreaAddress[%d]=%d\n",i,IPs[i]);
	}

	for (i = 1; i <= AREA_DEVICE_NUM; i++)
	{
		if (g_AreaAddress[i] != 0)
		{
			*index = i;
			*ipaddr = g_AreaAddress[i];
			break;
		}
	}
	
	if (count > 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*************************************************
  Function:			is_main_DeviceNo
  Description:		�ж��Ƿ�Ϊ�����ڷֻ�
  Input: 			��
  Output:			��
  Return:			�ɹ����
  Others:			�ֻ���Ϊ0 ��������
*************************************************/
uint8 is_main_DeviceNo(void)
{
	PFULL_DEVICE_NO dev = storage_get_devparam();
	if (dev->DeviceNoStr[dev->DevNoLen - 1] == '0')
	{
		return TRUE;
	}
	else
	{
		//dprintf("is_main_DeviceNo FALSE\n");		
		return FALSE;
	}
	
}

/*************************************************
  Function:			is_fenji1_DeviceNo
  Description:		�ж��Ƿ�Ϊ�����ڷֻ�
  Input: 			��
  Output:			��
  Return:			�ɹ����
  Others:			�ֻ���Ϊ0 ��������
*************************************************/
uint8 is_fenji1_DeviceNo(void)
{
	PFULL_DEVICE_NO dev = storage_get_devparam();
	if (dev->DeviceNoStr[dev->DevNoLen - 1] == '1')
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


/*************************************************
  Function:			check_access_pass
  Description:		��鿪������
  Input: 	
  	1.recPacket		���հ�
  Output:			��
  Return:			��
  Others:
*************************************************/
void check_access_pass(const PRECIVE_PACKET recPacket)
{
	char pass[6];
	char mypass[6];
	int32 n;
	uint32 echo = 0;
	memset(pass, 0, 6);
	memset(mypass, 0, 6);
	memcpy(pass, recPacket->data + NET_HEAD_SIZE, 5);
	
	//ס������
	strcpy(mypass, (char *)storage_get_pass(PASS_TYPE_DOOR_USER));
	dprintf("check_access_pass : user pwd: %s recive:%s\n", mypass, pass);
	n = strlen(mypass);
	if (0 == strlen(mypass))
	{
		// ���ò��ҳ��Ȳ�Ϊ0
		if (strlen((char *)storage_get_pass(PASS_TYPE_DOOR_SERVER)) && storage_get_doorserver(1))
		{
			goto labelpass2; 
		}
		echo = 2;											//������
		goto labelexit;
	} 
	if (0 == strcmp(mypass, pass)) 
	{
		echo = 1;											//��ȷ
		goto labelexit;
	}
	if (mypass[n-1] == '9')
	{
		mypass[n-1] = '0';
	}
	else
	{
		mypass[n-1] = mypass[n-1] + 1;
	}
	if (0 == strcmp(mypass, pass)) 
	{
		echo = 3;											//Ю��
		dprintf("check_access_pass : xiechi user pwd: %s recive:%s\n", mypass, pass);
		goto labelexit;
	}
	
labelpass2:	
	//��ķ����
	memset(mypass, 0, 6);
	strcpy(mypass, (char *)storage_get_pass(PASS_TYPE_DOOR_SERVER));
	dprintf("check_access_pass : user server pwd: %s recive:%s\n", mypass, pass);
	n = strlen(mypass);
	if (0 == strlen(mypass) || 0 == storage_get_doorserver(1))
	{
		echo = 0;//����
		goto labelexit;
	}
	if (0 == strcmp(mypass, pass)) 
	{
		echo = 1;//��ȷ
		goto labelexit;
	}
	if (mypass[n-1] == '9')
	{
		mypass[n-1] = '0';
	}
	else
	{
		mypass[n-1] = mypass[n-1] + 1;
	}
	dprintf("check_access_pass : xiechi server pwd: %s recive:%s\n", mypass, pass);
	if (0 == strcmp(mypass, pass)) 
	{
		echo = 3;//Ю��
		goto labelexit;
	}
	
labelexit:
	dprintf("check_access_pass : echo:%d\n", echo);
	net_send_echo_packet(recPacket, echo, NULL, 0);

}

/*************************************************
  Function:			search_stair_list
  Description:		����ݿڻ���ַ�б�
  Input: 			��
  Output:			
  Return:			�Ƿ��ȡ���豸IP: TRUE / FALSE
  Others:
*************************************************/
int32 search_stair_list(void)
{
	PDEVICE_NO DevNoList;
	uint32 * IPs;
	int32 i;
	int32 count = 0;
	uint8 n = 0, temp = 0;
	char DeviceNo[20] = {0};
	int8 ret = FALSE;
	
	#ifdef _AU_PROTOCOL_
	get_stair_fullno(DeviceNo, '_');
	#else
	get_stair_fullno(DeviceNo, '*');
	#endif
	dprintf("get_stair_list  DeviceNo: %s\n", DeviceNo);
	memset(g_StairAddress, 0, sizeof(g_StairAddress));			// �����ԭ�ȵ�����
	count = net_get_devices_and_ips(storage_get_areano(), DEVICE_TYPE_STAIR, DeviceNo, &DevNoList, &IPs);
	for (i = 0; i < count; i++)
	{
		// �����Ŵ� 0��ʼ
		temp = DevNoList[i].DeviceNo2 % STAIR_DEVICE_NUM;							// �ֻ���
		if (temp > n)
		{
			n = temp;
		}
		g_StairAddress[temp] = IPs[i];
		dprintf("g_StairAddress[%d]: %x\n",temp, g_StairAddress[temp]);
	}

	// ֧��ͨ���ݿڻ� add by luofl 2011-11-03
	#ifdef _TY_STAIR_
	if (storage_get_extmode(EXT_MODE_GENERAL_STAIR))
	{
		for (i = 0; i <= n; i++)
		{
			if ((i%2) == 0)
			{
				if (g_StairAddress[i] > 0 && g_StairAddress[i+1] == 0)
				{
					g_StairAddress[i+1] = g_StairAddress[i];
				}
			}
		}
	}
	#endif

	//storage_clear_monitorlist(DEVICE_TYPE_STAIR);
	malloc_monitorlist_memory(&g_monitorlist, MAX_MONITOR_NUM);
	for (i = 0; i < STAIR_DEVICE_NUM; i++)
	{
		if (g_StairAddress[i] != 0)
		{
			// �洢���������б�
			//storage_add_monitorlist(DEVICE_TYPE_STAIR, i, g_StairAddress[i]);
			g_monitorlist->pMonitorInfo[g_monitorlist->MonitorCount].DeviceType = DEVICE_TYPE_STAIR;
			g_monitorlist->pMonitorInfo[g_monitorlist->MonitorCount].index = i;
			g_monitorlist->pMonitorInfo[g_monitorlist->MonitorCount].IP = g_StairAddress[i];
			g_monitorlist->MonitorCount++;
			
		}
	}
		
	if (count > 0)
	{
		save_monitorlist_storage(DEVICE_TYPE_STAIR, g_monitorlist);
		ret = TRUE;
	}
	else
	{
		ret = FALSE;
	}
	
	free_monitorlist_memory(&g_monitorlist);
	return ret;
}

/*************************************************
  Function:			search_area_list
  Description:		������ڻ���ַ�б�
  Input: 			��
  Output:			
  Return:			TRUE / FALSE
  Others:
*************************************************/
int32 search_area_list(void)
{
	PDEVICE_NO DevNoList;
	uint32 * IPs;
	int32 i;
	int32 count;
	int8 ret = FALSE;
	memset(g_AreaAddress, 0, sizeof(g_AreaAddress));
	count = net_get_devices_and_ips(storage_get_areano(), DEVICE_TYPE_AREA, "__", &DevNoList, &IPs);
	for (i = 0; i < count; i++)
	{
		// �����Ŵ� 1��ʼ
		if (DevNoList[i].DeviceNo2 > 0 && DevNoList[i].DeviceNo2 <= AREA_DEVICE_NUM)
		{
			g_AreaAddress[DevNoList[i].DeviceNo2] = IPs[i];				
		}
		dprintf("get_area_list : g_AreaAddress[%d]=%x\n",DevNoList[i].DeviceNo2,IPs[i]);
	}

	//storage_clear_monitorlist(DEVICE_TYPE_AREA);
	malloc_monitorlist_memory(&g_monitorlist, MAX_MONITOR_NUM);
	for (i = 0; i < AREA_DEVICE_NUM+1; i++)
	{
		if (g_AreaAddress[i] != 0)
		{
			// �洢���������б�
			//storage_add_monitorlist(DEVICE_TYPE_STAIR, i, g_StairAddress[i]);
			g_monitorlist->pMonitorInfo[g_monitorlist->MonitorCount].DeviceType = DEVICE_TYPE_AREA;
			g_monitorlist->pMonitorInfo[g_monitorlist->MonitorCount].index = i;
			g_monitorlist->pMonitorInfo[g_monitorlist->MonitorCount].IP = g_AreaAddress[i];
			g_monitorlist->MonitorCount++;
			
		}
	}
		
	if (count > 0)
	{
		save_monitorlist_storage(DEVICE_TYPE_AREA, g_monitorlist);
		ret = TRUE;
	}
	else
	{
		ret = FALSE;
	}
	
	free_monitorlist_memory(&g_monitorlist);
	return ret;
}

/*************************************************
  Function:			search_door_list
  Description:		��ȡ��ǰ����ַ�б�
  Input: 			��
  Output:			
  Return:			TRUE / FALSE
  Others:
*************************************************/
int32 search_door_list(void)
{
	int32 i;
	int32 count = 0;
	int32 phonecount = 0;
	int32 ret = FALSE;
	uint32 * IPs = NULL;
	PFULL_DEVICE_NO myDev = (PFULL_DEVICE_NO)storage_get_devparam();
	char no[50] = {0};

	strncpy(no, myDev->DeviceNoStr, myDev->DevNoLen-1);
	memset(g_NetDoorAddress, 0, sizeof(g_NetDoorAddress));
	for (i = 0; i < DOOR_DEVICE_NUM; i++)
	{
		if (i == 0)
		{
			if (storage_get_extmode(EXT_MODE_NETDOOR1) == FALSE)
			{
				g_NetDoorAddress[i+1] = 0;
				continue;
			}
			else
			{
				no[myDev->DevNoLen-1] = '8';
			}
		}
		else
		{
			if (storage_get_extmode(EXT_MODE_NETDOOR2) == FALSE)
			{
				g_NetDoorAddress[i+1] = 0;
				continue;
			}
			else
			{
				no[myDev->DevNoLen-1] = '9';
			}
		}
		no[myDev->DevNoLen] = 0;
		// Ϊ����GM8126��ǰ��DEVICE_TYPE_DOOR_NET ���͸�ΪDEVICE_TYPE_ROOM
		count = net_get_ips(storage_get_areano(), DEVICE_TYPE_ROOM, no, &IPs);
		if (count)
		{
			g_NetDoorAddress[i+1] = IPs[0];
			ret = TRUE;
		}
	}

	// ģ����ǰ������
	for (i = 0; i < DOOR_DEVICE_NUM; i++)
	{
		if (i == 0)
		{
			if (storage_get_extmode(EXT_MODE_PHONEDOOR1) == FALSE)
			{
				g_PhoneDoorAddress[i+1] = 0;
				continue;
			}
			else
			{
				no[myDev->DevNoLen-1] = '6';
				g_PhoneDoorAddress[i+1] = 1;
				phonecount ++;
			}
		}
		else
		{
			if (storage_get_extmode(EXT_MODE_PHONEDOOR2) == FALSE)
			{
				g_PhoneDoorAddress[i+1] = 0;
				continue;
			}
			else
			{
				no[myDev->DevNoLen-1] = '7';
				g_PhoneDoorAddress[i+1] = 2;
				phonecount++;
			}
		}
	}

 	if (phonecount > 0)
 	{
		ret = TRUE;
	}
	
	//storage_clear_monitorlist(DEVICE_TYPE_DOOR_NET);
	malloc_monitorlist_memory(&g_monitorlist, DOOR_DEVICE_NUM*2); 
	for (i = 1; i < DOOR_DEVICE_NUM+1; i++) // �ȴ洢ģ����ǰ��
	{
		if (g_PhoneDoorAddress[i] != 0)
		{
			g_monitorlist->pMonitorInfo[g_monitorlist->MonitorCount].DeviceType = DEVICE_TYPE_DOOR_PHONE;
			g_monitorlist->pMonitorInfo[g_monitorlist->MonitorCount].index = i;
			g_monitorlist->pMonitorInfo[g_monitorlist->MonitorCount].IP = g_PhoneDoorAddress[i];
			g_monitorlist->MonitorCount++;
		}
	}
	
	for (i = 1; i < DOOR_DEVICE_NUM+1; i++)
	{
		if (g_NetDoorAddress[i] != 0)
		{
			//storage_add_monitorlist(DEVICE_TYPE_DOOR_NET, i, g_NetDoorAddress[i]);
			g_monitorlist->pMonitorInfo[g_monitorlist->MonitorCount].DeviceType = DEVICE_TYPE_DOOR_NET;
			g_monitorlist->pMonitorInfo[g_monitorlist->MonitorCount].index = i;
			g_monitorlist->pMonitorInfo[g_monitorlist->MonitorCount].IP = g_NetDoorAddress[i];
			g_monitorlist->MonitorCount++;
		}
	}
	if (g_monitorlist->MonitorCount)
	{
		// ģ����ǰ��Ҳ�洢�� DEVICE_TYPE_DOOR_NET ��������ļ���
		save_monitorlist_storage(DEVICE_TYPE_DOOR_NET, g_monitorlist);
	}
	free_monitorlist_memory(&g_monitorlist);
	return ret;
}

/*************************************************
  Function:			init_list_address
  Description:		��ʼ���豸�б�
  Input: 	
  Output:			��
  Return:			�ɹ����, TRUE/FALSE
  Others:	
*************************************************/
int32 init_list_address(void)
{
	int32 index = 0;
	uint8 i = 0, j = 0;
	PMONITORLISTINFO monitorlist = NULL;
	DEVICE_TYPE_E devtype[4] = {DEVICE_TYPE_AREA, DEVICE_TYPE_STAIR, DEVICE_TYPE_DOOR_NET, DEVICE_TYPE_DOOR_PHONE};

	for (i = 0; i < 4; i++)
	{
		monitorlist = storage_get_monitorlist(devtype[i]);
		if (monitorlist->MonitorCount == 0)
		{
			dprintf(" storage_get_monitorlist return 0 \n");
			free_monitorlist_memory(&monitorlist);
			monitorlist = NULL;
			continue;
		}

		dprintf("monitorlist devtype; %x  MonitorCount : %d\n", devtype[i], monitorlist->MonitorCount);
		index = 0;
		for (j = 0; j < monitorlist->MonitorCount; j++)
		{
			switch (monitorlist->pMonitorInfo[j].DeviceType)
			{
				case DEVICE_TYPE_AREA:
					index = monitorlist->pMonitorInfo[j].index;
					g_AreaAddress[index] = monitorlist->pMonitorInfo[j].IP;
					break;

				case DEVICE_TYPE_STAIR:
					index = monitorlist->pMonitorInfo[j].index;
					g_StairAddress[index] = monitorlist->pMonitorInfo[j].IP;
					break;

				case DEVICE_TYPE_DOOR_NET:
					index = monitorlist->pMonitorInfo[j].index;
					g_NetDoorAddress[index] = monitorlist->pMonitorInfo[j].IP;
					break;

				case DEVICE_TYPE_DOOR_PHONE:
					index = monitorlist->pMonitorInfo[j].index;
					g_PhoneDoorAddress[index] = monitorlist->pMonitorInfo[j].IP;
					break;

				default:
					break;
			}
			dprintf("monitorlist->pMonitorInfo[%d].index : %x\n", j, monitorlist->pMonitorInfo[j].index);
			dprintf("monitorlist->pMonitorInfo[%d].IP    : %x\n", j, monitorlist->pMonitorInfo[j].IP);
			dprintf("monitorlist->pMonitorInfo[%d].DeviceType : %x\n", j, monitorlist->pMonitorInfo[j].DeviceType);
		}
	
		free_monitorlist_memory(&monitorlist);
		monitorlist = NULL;
	}
}

/*************************************************
  Function:			inter_distribute
  Description:		�Խ����շ��Ͱ��ص�����
  Input: 	
  	1.recPacket		���հ�
  Output:			��
  Return:			�ɹ����, TRUE/FALSE
  Others:	
*************************************************/
int32 inter_distribute(const PRECIVE_PACKET recPacket)
{
	PNET_HEAD head = (PNET_HEAD)(recPacket->data + MAIN_NET_HEAD_SIZE);
	int32 cmd = head->command | SSC_INTERPHONE << 8;
	
	dprintf("cmd : 0X%04x  addr: 0X%x \n", cmd, recPacket->address);
	switch (cmd)
	{											
		case CMD_QUERY_ACCESS_PASS:								// ��֤����,������֤���
		{
			if (is_main_DeviceNo())
			{
				check_access_pass(recPacket);
			}
			return TRUE;
		}
		
		case CMD_CLEAR_ACCESS_PASS:								// ��λ��������
		{		
			storage_set_pass(PASS_TYPE_DOOR_USER, "");
			storage_set_pass(PASS_TYPE_DOOR_SERVER, "");
			return TRUE;
		}
		 
		case CMD_MONITOR:										// ����
		case CMD_MONITOR_TO_TALK:								// ���ӽ���ͨ��
		case CMD_STOP_MONITOR:									// �жϼ���
		case CMD_MONITOR_HEART:									// �������֣�������
			return monitor_distribute(recPacket);
			
		case CMD_CALL_CALLING:									// ��������	0x50
		case CMD_CALL_ANSWER:									// ȷ������	0x51
		case CMD_CALL_HANDDOWN:									// �Ҷ�����	0x53
		case CMD_CALL_HEART:									// ͨ������	0x5F
			return inter_call_distribute(recPacket);
			
		case CMD_RP_CARD_ACCESS_EVENT:
			net_send_echo_packet(recPacket, ECHO_OK, NULL, 0);
			if (DEVICE_TYPE_ROOM == head->DeviceType || DEVICE_TYPE_DOOR_NET== head->DeviceType)
			{
				linkage_alarm_card(1);							// ��ǰ��ˢ������
			}
			else
			{
				linkage_alarm_card(0);							// �ݿڻ�ˢ������
			}
			return TRUE;
			
		default:
			break;
	}
	return FALSE;
}

/*************************************************
  Function:			inter_responsion
  Description:		�Խ�����Ӧ���������
  Input: 	
  	1.recPacket		Ӧ���
  	2.SendPacket	���Ͱ�
  Output:			��
  Return:			��
  Others:
*************************************************/
void inter_responsion(const PRECIVE_PACKET recPacket, const PSEND_PACKET SendPacket)
{
	PNET_HEAD head = (PNET_HEAD)(recPacket->data + MAIN_NET_HEAD_SIZE);
	int32 cmd = head->command | SSC_INTERPHONE << 8;
	
	switch (cmd)
	{	
		case CMD_MONITOR:										// ����
		case CMD_MONITOR_TO_TALK:
		case CMD_STOP_MONITOR:									// �жϼ���
			monitor_responsion(recPacket, SendPacket);
			break;
		case CMD_CALL_CALLING:									// ��������	0x50
		case CMD_CALL_ANSWER:									// ȷ������	0x51
		case CMD_CALL_HANDDOWN:									// �Ҷ�����	0x53
		case CMD_CALL_HEART:									// ͨ������	0x5F
			inter_call_responsion(recPacket, SendPacket);
			break;
		default:
			break;
	}
}

