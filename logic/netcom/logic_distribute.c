/*************************************************
  Copyright (C), 2006-2016, Aurine
  File name: 	logic_distribute.c
  Author:    	chenbh
  Version:   	2.0
  Date: 		2014-12-11
  Description:  �������ݷַ�ģ��
				
  History:        
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#include "logic_distribute.h"
#include <semaphore.h>

typedef struct
{
	unsigned char code;								// ��ϵͳ����
	PFDISTRIBUTE distribute;						// ���ݷַ�����
	PFRESPONSION responsion;						// Ӧ����
}DISTRIBUTE_FUNC;

// ����ϵͳ���ݽ��շַ��ص�����
static DISTRIBUTE_FUNC g_distributefuncs[] = {
	{
		SSC_PUBLIC,									// ����
		NULL,
		NULL
	},
	{
		SSC_INTERPHONE,								// �Խ�
		NULL,
		NULL
	},
	{
		SSC_ALARM,									// ����
		NULL,
		NULL
	},
	{
		SSC_INFO,									// ��Ϣ
		NULL,
		NULL
	},
	{
		SSC_EHOME,									// �ҵ�
		NULL,
		NULL
	},
	{
		SSC_MULTIMEDIA,								// ��ý��
		NULL,
		NULL
	},
	{
		SSC_ELIVE,									// E����
		NULL,
		NULL
	},
	{
		SSC_OTHER,									// ��������
		NULL,
		NULL
	},
	{
		SSC_IPDOORPHONE,								// ����
		NULL,
		NULL
	},
	{
		SSC_VIDEOMONITORING,						// ��ý��
		NULL,
		NULL
	}	
};

static char g_SendBuf[NET_PACKBUF_SIZE];	  		// ����Buf
#ifndef _AU_PROTOCOL_
static PMAIN_NET_HEAD g_Main_NetHead = (PMAIN_NET_HEAD)g_SendBuf;  // ������������ͷ
#endif
static PNET_HEAD g_NetHead = (PNET_HEAD)(g_SendBuf+MAIN_NET_HEAD_SIZE);	// ���������ͷ
static uint16 g_CurPackID = 0;						// ���Ͱ�ID,�װ�Ϊ1
static PFULL_DEVICE_NO g_FullDeviceNo;				// �豸��Ų���
static uint8 g_setyet = 0;                          // ��־λ�Ƿ��Ѿ����ù�
static int g_DistributeRunning = 0;					//�ַ�ģ���߳�����״̬
static pthread_t g_DistributeThread;				//�ַ�ģ���߳�

static sem_t g_SendBufSem;							// �����ź���
static sem_t g_SendWaitSem;							// �����ź���

/*******************************��������ͷ�ʽ********************************************/
// ������������ڵ�ṹ����
typedef struct _WAIT_ECHO
{
	uint16 PackageID;								// ���������ID
	uint32 address;									// ���͵�ַ
	uint16 port;									// ���Ͷ˿�
	uint8 EchoValue;								// ����Ӧ����
	char * ReciData;								// ���ո�������
	int32 ReciSize;									// ���ո������ݴ�С
	struct _WAIT_ECHO * next;						// ��һ���ڵ�
}WAIT_ECHO, *PWAIT_ECHO;

static PWAIT_ECHO g_WaitFreeList = NULL;			// �������������������
static PWAIT_ECHO g_WaitEchoList = NULL;			// ����������������
//static ROCK_SEM_DATA g_SendWaitSem;					

/*******************************������մ������********************************************/
typedef struct
{
	uint32 ID;										// �ỰID
	int32 size;										// �������ݳ���
	char * data;									// ��������
	char * Recflags;								// ���ձ�־
	uint16 total;									// �ܰ���
	uint16 num;										// �Ѿ����հ���
	uint32 address;									// �Զ˵�ַ
	uint16 port;									// �Զ˶˿ں�
	PFMAX_RECIVE func; 								// ���ն���ص�����
	PFMAX_SEND_REC_STATE state;						// ����״̬�ص�����
	uint32 t0;										// ��ʼʱ��, ���㳬ʱ��
	uint8 TimeOut;									// ��ʱʱ��, ��λ��
}MULT_RECIVE;

#define	REC_MAX_DATA_NUM			5				// ͬʱ���ն��������
#define	MULT_SEND_PACKET_SIZE		1024			// �������ʱ,ûһ����С
#define	MULT_SEND_MAX_PACKET		(1024*20)		// ������
#define DEFAULT_MULT_RECIVE_TIMEOUT	60				// Ĭ�϶�����ճ�ʱʱ��	
static int32 g_CurReiveNum = 0;						// ��ǰ�����Ķ�����ս��̸���
static MULT_RECIVE g_MultRecList[REC_MAX_DATA_NUM];	// ���ն�����ƽṹ����

/*******************************������ʹ������********************************************/
typedef struct _MULT_SEND
{
	struct _MULT_SEND * next;			
	uint32 ID;										// �ỰID
	int32 size;										// ������������
	char * data;									// ��������
	uint32 address;									// ���͵�ַ
	uint16 port;									// ���Ͷ˿ں�
	PFMAX_SEND_ECHO func; 							// ����Ӧ��ص�����
	PFMAX_SEND_REC_STATE state;						// ����״̬�ص�����
}MULTI_SEND, *PMULTI_SEND;

static PMULTI_SEND g_MultSendList = NULL;			// ������Ͷ���
static PMULTI_SEND g_LastMultSend = NULL;			// ��β��
static PMULTI_SEND g_CurMultiSend = NULL;			// ��ǰ���Ͱ�
static char * g_SendFlags;							// ����Ӧ���־
static uint16 g_SendNum = 0;						// �ܷ��Ͱ���
static uint16 g_SendOkNum = 0;						// ���ͳɹ�����
static int32 g_SendSize = 0;						// ��ǰ�Ѿ������ֽ���
static int32 g_LastSendSize;						// ���һ���ֽ���

static uint32		DestAreaCode;					// Ŀ��С�����
static DEVICE_NO	DestDeviceNo;					// Ŀ���豸���
static uint16		DefaultPriry = PRIRY_DEFAULT;	// �����ȼ�

#ifndef _AU_PROTOCOL_
DEVICE_NO G_CENTER_DEVNO = {DEVICE_TYPE_CENTER, 0, 0, 0, 0};		// ���ķ�����
DEVICE_NO G_MANAGER_DEVNO = {DEVICE_TYPE_MANAGER, 0, 0, 0, 100};	// �������
DEVICE_NO G_BROADCAST_DEVNO = {0, 0, 0, 999999999, 999999999};		// �㲥���
DEVICE_NO G_MULTCMD_DEVNO = {0, 0, 0, 0, 0};						// �ְ������¼�Ŀ���豸���
#else
DEVICE_NO G_CENTER_DEVNO = {0, 0};									// ���ķ�����
DEVICE_NO G_MANAGER_DEVNO = {0, 100};								// �������
DEVICE_NO G_BROADCAST_DEVNO = {999999999, 999999999};				// �㲥���
DEVICE_NO G_MULTCMD_DEVNO = {0, 0};									// �ְ������¼�Ŀ���豸���
#endif

// �ϱ�����ƽ̨�¼������б�
NET_COMMAND g_CmdListReportAurine[] = 
	{
		//һ���¼�
		CMD_RP_SET_EVENT, CMD_RP_UNSET_EVENT, CMD_RP_PARTSET_EVENT, CMD_RP_BYPASS_EVENT,
		//�����¼�
		CMD_RP_FORCE_UNSET_ALARM, CMD_RP_SOS_ALARM, 
		CMD_RP_AREA_BREAK_ALARM, CMD_RP_AREA_FAULT_ALARM,
	}; 

// �ϱ������¼������б�
NET_COMMAND g_CmdListReportServer[] = 
	{
		//һ���¼�
		CMD_RP_SET_EVENT, CMD_RP_UNSET_EVENT, CMD_RP_PARTSET_EVENT, CMD_RP_BYPASS_EVENT,
		//�����¼�
		CMD_RP_FORCE_UNSET_ALARM, CMD_RP_SOS_ALARM, 
		CMD_RP_AREA_BREAK_ALARM, CMD_RP_AREA_FAULT_ALARM,
	}; 
	
// �ϱ�����Ա���¼������б�
NET_COMMAND g_CmdListReportManager[] = 
	{
		//һ���¼�
		CMD_RP_SET_EVENT, CMD_RP_UNSET_EVENT, CMD_RP_PARTSET_EVENT, CMD_RP_BYPASS_EVENT,
		//�����¼�
	    CMD_RP_FORCE_UNSET_ALARM, CMD_RP_SOS_ALARM,
    	CMD_RP_AREA_BREAK_ALARM, CMD_RP_AREA_FAULT_ALARM,CMD_RP_AREA_DEFINE,CMD_RP_ALARM_STATE,
	}; 
	
//static void mult_send_proc(void);
static void recive_multpacket(NET_COMMAND cmd, const PRECIVE_PACKET RecivePacket);

/*************************************************
  Function:    		set_nethead
  Description: 		����Ŀ���豸�����ź��豸��, �����ȼ�
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
void set_nethead(DEVICE_NO DeviceNo, uint16 Priry)
{
	#ifndef _AU_PROTOCOL_
	if (DeviceNo.AreaNo == 0)
	{
		PFULL_DEVICE_NO fulldevno;
		fulldevno = storage_get_devparam();
		if (fulldevno->AreaNo == 0)
		{
			DestAreaCode = 1;
			DeviceNo.AreaNo = 1;
		}
		else
		{
			DestAreaCode = fulldevno->AreaNo;
			DeviceNo.AreaNo = fulldevno->AreaNo;
		}
	}
	else
	{
		DestAreaCode = DeviceNo.AreaNo;
	}
	//printf("set_nethead====== %d\n",DestAreaCode);
	DestDeviceNo = DeviceNo;
	DefaultPriry = Priry;
	#endif
}

/*************************************************
  Function:    		sendbuf_lock
  Description: 		���ͻ���������
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
static void sendbuf_lock(void)
{
	sem_wait(&g_SendBufSem);
}

/*************************************************
  Function:    		sendlbuf_unlock
  Description: 		���ͻ���������
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
static void sendbuf_unlock(void)
{
	sem_post(&g_SendBufSem);
}

/*************************************************
  Function:    		sendbuf_sem_init
  Description: 		�ź�����ʼ��
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
static void sendbuf_sem_init(void)
{
	sem_init(&g_SendBufSem, 0, 1);   					
}

/*************************************************
  Function:    		sendbuf_sem_del
  Description: 		�ź���ɾ��
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
static void sendbuf_sem_del(void)
{
	sem_destroy(&g_SendBufSem);   					
}

/*************************************************
  Function:    		sendbuf_lock
  Description: 		���ͻ���������
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
static void sendwait_lock(void)
{
	sem_wait(&g_SendWaitSem);
}

/*************************************************
  Function:    		sendlbuf_unlock
  Description: 		���ͻ���������
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
static void sendwait_unlock(void)
{
	sem_post(&g_SendWaitSem);
}

/*************************************************
  Function:    		sendbuf_sem_init
  Description: 		�ź�����ʼ��
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
static void sendwait_sem_init(void)
{
	sem_init(&g_SendWaitSem, 0, 1);   					
}

/*************************************************
  Function:    		sendwait_sem_del
  Description: 		�ź���ɾ��
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
static void sendwait_sem_del(void)
{
	sem_destroy(&g_SendWaitSem);   					
}

/*************************************************
  Function:    		wait_echo_list_init
  Description: 		��ʼ������,����ռ�
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
static void wait_echo_list_init(void)
{
	uint32 i = 0;
	PWAIT_ECHO WaitEchoNode = NULL;
	if (g_WaitFreeList == NULL)
	{
		for (i = 0; i < PACKET_NODE_MAX_NUM; i++)
		{
			// ����һ���ڵ�
			WaitEchoNode = (PWAIT_ECHO)malloc(sizeof(WAIT_ECHO));
			if (WaitEchoNode == NULL)
			{
				continue;
			}
			else
			{
				memset(WaitEchoNode, 0, sizeof(WAIT_ECHO));
				WaitEchoNode->ReciData = (char*)malloc(PACKET_NODE_DATA_SIZE);
				if (WaitEchoNode->ReciData == NULL)
				{
					free(WaitEchoNode);
					WaitEchoNode = NULL;
					continue;
				}
			}

			// �ѽڵ�ӵ�����
			if (g_WaitFreeList == NULL)
			{
				g_WaitFreeList = WaitEchoNode;
				g_WaitFreeList->next = NULL;
			}
			else
			{
				WaitEchoNode->next = g_WaitFreeList;
				g_WaitFreeList = WaitEchoNode;
			}
		}
	}
}

/*************************************************
  Function:    		get_waitnode_from_freelist
  Description: 		�ӿ��������л�ȡ�ڵ�
  Input: 			��
  Output:			��
  Return:			�ڵ�ָ��
  Others:
*************************************************/
static PWAIT_ECHO get_waitnode_from_freelist(void)
{
	PWAIT_ECHO tmp = NULL;
	if (g_WaitFreeList == NULL)
	{
		dprintf("free list is NULL\n");
		return NULL;
	}

	sendwait_lock();
	if (g_WaitFreeList->next == NULL)
	{
		tmp = g_WaitFreeList;
		g_WaitFreeList = NULL;
	}
	else
	{
		tmp = g_WaitFreeList;
		g_WaitFreeList = tmp->next;		
	}
	sendwait_unlock();
	return tmp;
}

/*************************************************
  Function:    		add_waitnode_to_freelist
  Description: 		�ѽڵ�����������
  Input: 			
  	1.node			�ڵ�ָ��
  Output:			��
  Return:			��
  Others:
*************************************************/
static void add_waitnode_to_freelist(PWAIT_ECHO node)
{
	if (node == NULL)
	{
		return;
	}

	sendwait_lock();
	if (g_WaitFreeList == NULL)
	{
		g_WaitFreeList = node;
		g_WaitFreeList->next = NULL;
	}
	else
	{
		node->next = g_WaitFreeList;
		g_WaitFreeList = node;
	}
	sendwait_unlock();
}

/*************************************************
  Function:    		get_package_ID
  Description:		��÷��Ͱ�ID
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
static uint16 get_package_ID(void)
{
	if (g_CurPackID==0xFFFF || g_CurPackID==0)
	{
		g_CurPackID = 1;
	}	
	else
	{
		g_CurPackID++;
	}
	return g_CurPackID;	
}

/*************************************************
  Function:    		encrypt_data
  Description:		����ʱ��������
  Input: 
	1.data			���ݲ���
	2.size			���ݴ�С
  Output:			��
  Return:			��
  Others:
*************************************************/
static void encrypt_data(char * data, int32 size)
{
	// �޼���
	g_NetHead->Encrypt = 1;							// �������� ������Ƶ�����־λ
	g_NetHead->DataLen = size;						// ���ݳ���
	memset(g_SendBuf+NET_HEAD_SIZE, 0, sizeof(g_SendBuf)-NET_HEAD_SIZE);
	memcpy(g_SendBuf+NET_HEAD_SIZE, data, size);	// ������������
	g_NetHead->DataLen = size;						// �������ݳ���

	#ifndef _AU_PROTOCOL_
	g_NetHead->DestDeviceNo = DestDeviceNo;			//�豸���
	g_NetHead->Priry = DefaultPriry;
	g_Main_NetHead->subPacketLen = sizeof(NET_HEAD) + g_NetHead->DataLen;
	#endif
}

/*************************************************
  Function:    		direct_send_package
  Description:		����ֱ�ӷ��ͣ����ӵ�������
  Input: 
	1.cmd			����
	2.data			��������
	3.size			�������ݴ�С
	4.address		�Է�IP��ַ
	5.port			�Է��˿ں�
  Output:			��
  Return:			��ID 
  Others:
*************************************************/
static int direct_send_package(NET_COMMAND cmd, char * data, int32 size, uint32 address, uint16 port)
{
	int32 ret = 0;
	uint16 PackageID = 0;

	sendbuf_lock();
	PackageID = get_package_ID();
	dprintf(" PackageID  : 0X%x \n", PackageID);
	#ifndef _AU_PROTOCOL_
	// ��Э���ͷ
	g_Main_NetHead->mainSeq = PackageID;
	g_Main_NetHead->mainPacketType = 0x0000;		// ��ͨ��
	#endif
	
	g_NetHead->PackageID = PackageID;				// ����ʶ
	g_NetHead->DirectFlag = DIR_SEND;				// �����־��������0xAA��Ӧ��0x00��
	g_NetHead->SubSysCode = (uint8)(cmd>>8);		// ��ϵͳ����
	g_NetHead->command = (uint8)(cmd & 0xFF);		// ����ֵ
	g_NetHead->EchoValue = 0;						// Ӧ����
	encrypt_data(data, size);
	ret = net_send_cmd_packet(g_SendBuf, g_NetHead->DataLen+NET_HEAD_SIZE, address, port);
	
	sendbuf_unlock();
	if (ret == FALSE)
	{
		return 0;
	}	
	return PackageID;
}

/*************************************************
  Function:    		net_direct_send
  Description:		����ֱ�ӷ��ͣ����ӵ�������
  Input: 
	1.cmd			����
	2.data			��������
	3.size			�������ݴ�С
	4.address		�Է�IP��ַ
	5.port			�Է��˿ں�
  Output:			��
  Return:			��ID 
  Others:
*************************************************/
uint16 net_direct_send(NET_COMMAND cmd, char * data, int32 size, uint32 address, uint16 port)
{
	//DECLARE_CUP_SR;
	uint16 PackageID = 0;
	
	//ENTER_CRITICAL(); �ر��ж�
	dprintf("address = %x\n",address);
	PackageID = direct_send_package(cmd, data, size, address, port);
	//EXIT_CRITICAL(); ���ж�
	return PackageID;
}

/*************************************************
  Function:    		net_send_by_list
  Description:		�������������������,������
  Input: 
	1.cmd			����
	2.data			��������
	3.size			�������ݴ�С
	4.address		�Է�IP��ַ
	5.port			�Է��˿ں�
	6.SendTimes		���ʹ���
	7.TimeOut		���η��ͳ�ʱʱ��
  Output:			��
  Return:			��ID
  Others:
*************************************************/
uint16 net_send_by_list(NET_COMMAND cmd, char * data, int32 size, uint32 address, uint16 port, 
				int32 SendTimes, int32 TimeOut)
{
	uint16 PackageID = 0;

	sendbuf_lock();
	PackageID = get_package_ID();
	#ifndef _AU_PROTOCOL_
	// ��Э���ͷ
	g_Main_NetHead->mainSeq = PackageID;
	g_Main_NetHead->mainPacketType = 0x0000;		// ��ͨ��
	#endif
	
	g_NetHead->PackageID = PackageID;				// ����ʶ
	g_NetHead->DirectFlag = DIR_SEND;				// �����־��������0xAA��Ӧ��0x00��
	g_NetHead->SubSysCode = (uint8)(cmd>>8);		// ��ϵͳ����
	g_NetHead->command = (uint8)(cmd & 0xFF);		// ����ֵ
	g_NetHead->EchoValue = 0;						// Ӧ����
	encrypt_data(data, size);

	if (SendTimes < 1)
	{
		SendTimes = 1;	
	}
	if (TimeOut < 1)
	{
		TimeOut = PACKET_TIMEOUT_VALUE;
	}
	
	net_add_send_packet(g_SendBuf, g_NetHead->DataLen+NET_HEAD_SIZE, address, port, SendTimes, TimeOut);
	
	sendbuf_unlock();
	return PackageID;
}

/*************************************************
  Function:    		add_wait_echo_list
  Description:		���ڵ����ȴ�Ӧ���б�
  Input: 
	1.node			����ڵ�
  Output:			��
  Return:			��
  Others:
*************************************************/
static void add_wait_echo_list(PWAIT_ECHO node)
{
	sendwait_lock();
	if (NULL == g_WaitEchoList)
	{
		node->next = NULL;
	}	
	else
	{
		node->next = g_WaitEchoList;
	}	
	g_WaitEchoList = node;
	sendwait_unlock();
}

/*************************************************
  Function:    		remove_wait_echo_list
  Description:		���ڵ�ӵȴ�Ӧ���б�ȡ��
  Input: 
	1.node			����ڵ�
  Output:			��
  Return:			��
  Others:
*************************************************/
static void remove_wait_echo_list(PWAIT_ECHO node)
{
	PWAIT_ECHO prev = NULL;
	PWAIT_ECHO temp = g_WaitEchoList;

	sendwait_lock();
	while(temp != NULL)
	{
		if (temp == node)
		{
			if (NULL == prev)
			{
				g_WaitEchoList = temp->next;
			}	
			else
			{
				prev->next = temp->next;
			}	
			break;	
		}
		else
		{
			prev = temp;
			temp = temp->next;
		}	
	}
	sendwait_unlock();
}

/*************************************************
  Function:    		check_is_wait_echo
  Description:		����Ƿ��ǵȴ�Ӧ�������е�Ӧ���
  Input: 
	1.RecPacket		������հ�
  Output:			��
  Return:			TRUE / FALSE
  Others:
*************************************************/
static int32 check_is_wait_echo(const PRECIVE_PACKET RecPacket)
{
	PNET_HEAD NetHead = (PNET_HEAD)(RecPacket->data + MAIN_NET_HEAD_SIZE);
	PWAIT_ECHO temp = g_WaitEchoList;
	sendwait_lock();
	sendwait_unlock();
	if (temp == NULL)
	{
		dprintf("g_WaitEchoList is NULL\n");
	}
	while(temp)
	{
		if (NetHead->PackageID==temp->PackageID &&
			(RecPacket->address==temp->address || (temp->address&0xFF)==0xFF) &&
			RecPacket->port==temp->port)
		{			
			if (temp->ReciData != NULL)
			{	
				temp->ReciSize = NetHead->DataLen;
				//��������
				memcpy(temp->ReciData, RecPacket->data+NET_HEAD_SIZE, NetHead->DataLen);
				temp->EchoValue = NetHead->EchoValue;
				return TRUE; 
			}
			break;
		}
		temp = temp->next;
	}
	return FALSE;
}

/*************************************************
  Function:    		net_send_command
  Description:		���������������ͣ������������߳��еȴ�Ӧ��
  Input: 
	1.cmd			����
	2.data			��������
	3.size			�������ݴ�С
	4.address		�Է�IP��ַ
	5.port			�Է��˿ں�
	6.TimeOut		���ͳ�ʱʱ��
  Output:			
	1.EchoValue		Ӧ����
	2.ReciData		����Ӧ������, ע��:���ⲿ����ռ�
	3.ReciSize		����Ӧ�����ݴ�С
  Return:			�ɹ����, TRUE/FALSE
  Others:
*************************************************/
int32 net_send_command(NET_COMMAND cmd, char * data, int32 size, uint32 address, uint16 port, int32 TimeOut,
				 uint8 * EchoValue, char * ReciData, int32 * ReciSize)
{	
	int32 ret = FALSE;
	int32 DelayCount = 0;
	
	PWAIT_ECHO node = get_waitnode_from_freelist();
	if (NULL == node)
	{
		dprintf("get node err\n");
		return FALSE;
	}

	add_wait_echo_list(node);
	node->address = address;						// ���͵�ַ
	node->port = port;								// ���Ͷ˿�			
	node->EchoValue = 0xFF;							// ��δӦ��
	sendwait_lock();
	node->PackageID = direct_send_package(cmd, data, size, address, port);
	if (0 == node->PackageID)						// ���Ͳ��ɹ�����
	{
		sendwait_unlock();
		remove_wait_echo_list(node);
		add_waitnode_to_freelist(node);
		return FALSE;
	}
	sendwait_unlock();

	if (TimeOut < 1)
	{
		TimeOut = 1;
	}	

	while(1)
	{
		// ��Ӧ��, �˳�
		if (node->EchoValue != 0xFF)
		{
			break;
		}	
		
		// �ж�ʱ��
		DelayCount++;
		if ((DelayCount*50) >= (TimeOut*1000))		// Ӧ��ʱ
		{
			break;
		}	
		else
		{
			DelayMs_nops(50);
		}	
	}
	
	if (node->EchoValue != 0xFF)
	{
		if (EchoValue)
		{
			*EchoValue = node->EchoValue;			// ����Ӧ����
		}	
		if (ReciData)
		{
			// ���ո�������
			memcpy(ReciData, node->ReciData, node->ReciSize);
		}	 
		if (ReciSize)	
		{
			*ReciSize =	node->ReciSize;				// ���ո������ݴ�С
		}	
		ret = TRUE;
	}
	else
	{
		ret = FALSE;
	}
	remove_wait_echo_list(node);
	add_waitnode_to_freelist(node);
	return ret;
}

/*************************************************
  Function:    		net_fill_main_nethead
  Description:		��Э������
  Input: 
	1.recMainNetHead		���հ�����ͷ
  Return:			��
  Others:
*************************************************/
void net_fill_main_nethead(void)
{
	#ifndef _AU_PROTOCOL_
	uint32 hwEncrypt = _HW_ENCRYPT_;
	memset(g_Main_NetHead->hwEncrypt, 0, sizeof(g_Main_NetHead->hwEncrypt));
	memcpy(&g_Main_NetHead->hwEncrypt[12], &hwEncrypt, 4);
	
	g_Main_NetHead->subProtocolType = SUBPROTOCALTYPE;
	g_Main_NetHead->DirectFlag = DIR_DONOTHING;
	g_Main_NetHead->mainProtocolType = MAINPROTOCOL_VER;
	g_Main_NetHead->sysChipType = SYYCHIPTYPE;
	g_Main_NetHead->subProtocolPackType = SUBPROTOCALPACKTYPE;
	g_Main_NetHead->subProtocolEncrypt = _SUB_PROTOCOL_ENCRYPT_; // ���ü���
	g_Main_NetHead->EchoValue = SUBPROTOCALECHO;
	#endif
}


/*************************************************
  Function:    		net_send_echo_packet
  Description:		����Ӧ���
  Input: 
	1.recPacket		���հ�
	2.echoValue		Ӧ����
	3.data			��������
	4.size			�������ݴ�С
  Output:			��
  Return:			��
  Others:
*************************************************/
void net_send_echo_packet(PRECIVE_PACKET recPacket, ECHO_VALUE_E echoValue, char * data, int32 size)
{
	PNET_HEAD recNetHead;
	PMAIN_NET_HEAD recMainNetHead;
	
	if (NULL == recPacket)
	{
		return;
	}
	recNetHead = (PNET_HEAD)(recPacket->data + MAIN_NET_HEAD_SIZE);
	recMainNetHead = (PMAIN_NET_HEAD)(recPacket->data);
	sendbuf_lock();
	g_NetHead->PackageID = recNetHead->PackageID;	// ����ʶ
	g_NetHead->DirectFlag = DIR_ACK;				// �����־��������0xAA��Ӧ��0x00��
	g_NetHead->SubSysCode = recNetHead->SubSysCode;	// ��ϵͳ����
	g_NetHead->command = recNetHead->command;		// ����ֵ
	g_NetHead->EchoValue = echoValue;				// Ӧ����	
	encrypt_data(data, size);
	
	#ifndef _AU_PROTOCOL_
	// ��������ͷ���
	g_Main_NetHead->mainSeq = recMainNetHead->mainSeq;
	g_Main_NetHead->mainPacketType = recMainNetHead->mainPacketType;
	g_NetHead->DestDeviceNo = recNetHead->SrcDeviceNo;
	g_NetHead->Priry = recNetHead->Priry;
	#endif
	
	net_send_cmd_packet(g_SendBuf, g_NetHead->DataLen+NET_HEAD_SIZE, recPacket->address, recPacket->port);
	
	sendbuf_unlock();
}

/*************************************************
  Function:    		check_cmd_inlist
  Description:		��������Ƿ����б���
  Input: 
	1.cmd			����
	2.list			�б�
	3.n				�б��С
  Output:			��
  Return:			�����, TRUE/FALSE
  Others:
*************************************************/
static int32 check_cmd_inlist(NET_COMMAND cmd, NET_COMMAND list[], int32 n)
{
	int32 i;
	
	for ( i= 0; i < n; i++)
	{
		if (list[i] == cmd)
		{
			return TRUE;
		}	
	}
	return FALSE;
}

/*************************************************
  Function:    		net_send_event
  Description:		�ϱ��¼����������¼�
  Input: 
	1.cmd			����
	2.data			��������
	3.size			�������ݴ�С
	4.SendTo		���ͷ�, ����ƽ̨/���ķ�����/����Ա�� 
  Output:			
  Return:			�Ƿ�ɷ���, TRUE/FALSE�� FALSE��������Ϊ�Ƕ�����¼���������ݳ��Ȳ�������
  Others:
*************************************************/
int32 net_send_event(NET_COMMAND cmd, char * data, int32 size, EVENT_SENDTO_E SendTo)
{
	uint32 ipaddr = 0;
	if (g_Event_Server.ip != 0)
	{
		set_nethead(G_MANAGER_DEVNO, PRIRY_DEFAULT);
		net_send_by_list(cmd, data, size, g_Event_Server.ip, g_Event_Server.port, 3, 3);
		return ;
	}
	
	// �ж������Ƿ����б���
	switch (SendTo)
	{
		case EST_AURINE:
			if (!check_cmd_inlist(cmd, g_CmdListReportAurine, sizeof(g_CmdListReportAurine)/sizeof(NET_COMMAND)))
			{
				return FALSE;
			}
			ipaddr = storage_get_aurine_ip();
			set_nethead(G_CENTER_DEVNO, PRIRY_DEFAULT);
			break;	
			
		case EST_SERVER:
			if (!check_cmd_inlist(cmd, g_CmdListReportServer, sizeof(g_CmdListReportServer)/sizeof(NET_COMMAND)))
			{
				return FALSE;
			}
			ipaddr = storage_get_center_ip();
			set_nethead(G_CENTER_DEVNO, PRIRY_DEFAULT);
			break;	
			
		case EST_MANAGER:
			if (!check_cmd_inlist(cmd, g_CmdListReportManager, sizeof(g_CmdListReportManager)/sizeof(NET_COMMAND)))
			{
				return FALSE;
			}
			set_nethead(G_MANAGER_DEVNO, PRIRY_DEFAULT);
			ipaddr = storage_get_manager_ip(1);
			if (ipaddr)
			{
				net_send_by_list(cmd, data, size, ipaddr, NETCMD_UDP_PORT, 3, 3);
			}
			ipaddr = storage_get_manager_ip(2);
			if (ipaddr)
			{
				net_send_by_list(cmd, data, size, ipaddr, NETCMD_UDP_PORT, 3, 3);
			}
			ipaddr = storage_get_manager_ip(3);
			if (ipaddr)
			{
				net_send_by_list(cmd, data, size, ipaddr, NETCMD_UDP_PORT, 3, 3);
			}
			return TRUE;
			
		default:
			return FALSE;	
	}
	
	dprintf("ipaddr = %x\n",ipaddr);
	return net_send_by_list(cmd, data, size, ipaddr, NETCMD_UDP_PORT, 3, 3);
	
}

/*************************************************
  Function:    		get_distribute_func
  Description:		���ָ����ϵͳ���շ��Ͱ�����ص�����
  Input: 
	1.subSysCode	��ϵͳ����
  Output:			��
  Return:			��ϵͳ���հ��ַ�����
  Others:
*************************************************/
static PFDISTRIBUTE get_distribute_func(unsigned char subSysCode)
{
	int32 i;
	for(i=0; i<(sizeof(g_distributefuncs)/sizeof(DISTRIBUTE_FUNC)); i++)
	{
		if (subSysCode == g_distributefuncs[i].code)
		{
			return g_distributefuncs[i].distribute;
		}
	}
	return NULL;
}

/*************************************************
  Function:    		get_distribute_func
  Description:		���ָ����ϵͳ����Ӧ�������ص�����
  Input: 
	1.subSysCode	��ϵͳ����
  Output:			��
  Return:			��ϵͳӦ����ַ�����
  Others:
*************************************************/
static PFRESPONSION get_responsion_func(unsigned char subSysCode)
{
	int32 i;
	for(i=0; i<(sizeof(g_distributefuncs)/sizeof(DISTRIBUTE_FUNC)); i++)
	{
		if (subSysCode == g_distributefuncs[i].code)
		{
			return g_distributefuncs[i].responsion;
		}
	}
	return NULL;	
}

/*************************************************
  Function:    		send_timeout_echo
  Description:		Ӧ��ʱ֪ͨ
  Input: 
	1.SendPacket	���Ͱ�
  Output:��
  Return:��
  Others:
*************************************************/
static void send_timeout_echo_func(PSEND_PACKET SendPacket)
{
	PNET_HEAD netHead = (PNET_HEAD)(SendPacket->data + MAIN_NET_HEAD_SIZE);
	PFRESPONSION responsion_func = get_responsion_func(netHead->SubSysCode);
	
	if (NULL != responsion_func)
	{
		responsion_func(NULL, SendPacket);
	}	
}

/*************************************************
  Function:    		net_set_recivedata_func
  Description:		���ú����ص�����
  Input: 
	1.subSysCode	��ϵͳ����
	2.distribute	���շ�������Ӧ������
	3.responsion	����Ӧ������Ӧ������
  Output:			��
  Return:			��
  Others:
*************************************************/
int32 net_set_recivedata_func(SUB_SYS_CODE_E SubSysCode, PFDISTRIBUTE distribute, PFRESPONSION responsion)
{
	int32 i; 
	for(i=0; i<(sizeof(g_distributefuncs)/sizeof(DISTRIBUTE_FUNC)); i++)
	{
		if (SubSysCode == g_distributefuncs[i].code)
		{
			g_distributefuncs[i].distribute = distribute;
			g_distributefuncs[i].responsion = responsion;
			return TRUE;
		}
	}
	return FALSE; 
}

/*************************************************
  Function:    		recive_dns_packet
  Description:		���ڻ��㲥DNS����ʱ�õ�
  Input: 			
  	1.RecivePacket	���հ�
  Output:			��
  Return:			��
  Others:
*************************************************/
static void recive_dns_packet(NET_COMMAND cmd, const PRECIVE_PACKET RecivePacket)
{
	uint32 address = 0; 
	uint8 DeviceType = 0;
	char devno[20];
	int32 pos = 0;
	int32 i = 0;
	char data[16];
	PNET_HEAD head = (PNET_HEAD)(RecivePacket->data + MAIN_NET_HEAD_SIZE);

	address = storage_get_local_ip();
	dprintf("remote address:0x%x, local address:0x%x\n", RecivePacket->address, address);
	if (RecivePacket->address == address)
	{
		return;
	}	

	if (DIR_SEND == head->DirectFlag)
	{
		DeviceType = RecivePacket->data[NET_HEAD_SIZE];
		dprintf("remote devtype:0x%x, local devtype:0x%x\n", DeviceType, g_FullDeviceNo->DeviceType);
		if (DeviceType != g_FullDeviceNo->DeviceType)
		{
			return;
		}
		memset(devno, 0, sizeof(devno));
		strncpy(devno, RecivePacket->data+NET_HEAD_SIZE+1, head->DataLen-1);

		// �滻ͨ���
		devno[g_FullDeviceNo->DevNoLen] = 0;
		if ('_'==devno[g_FullDeviceNo->DevNoLen-1] || '*'==devno[g_FullDeviceNo->DevNoLen-1])
		{
			devno[g_FullDeviceNo->DevNoLen-1] = g_FullDeviceNo->DeviceNoStr[g_FullDeviceNo->DevNoLen-1];
		}	
		pos = g_FullDeviceNo->Rule.StairNoLen - g_FullDeviceNo->Rule.CellNoLen;
		
		for (i=0; i<g_FullDeviceNo->Rule.CellNoLen; i++)
		{
			if (devno[pos+i] == '_')
			{
				devno[pos+i] = g_FullDeviceNo->DeviceNoStr[pos+i];
			}	
		}
		dprintf("request devstr:%s, local devstr:%s\n", devno, g_FullDeviceNo->DeviceNoStr);
		if (0 == strcmp(g_FullDeviceNo->DeviceNoStr, devno))
		{
			dprintf("remote address:0x%x, local address:0x%x\n", RecivePacket->address, address);
			memset(data, 0, sizeof(data));
			memcpy(data, &g_FullDeviceNo->DeviceNo, sizeof(DEVICE_NO));
			memcpy(data+sizeof(DEVICE_NO), (char *)(&address), 4);
			dprintf("send data len : sizeof(data):%d\n  ", sizeof(data));
			dprintf("send data : %s \n", data);
			//dprintf("send data DeviceType: %d   DeviceNo2: %d  addres:%0x \n ", g_FullDeviceNo->DeviceNo.DeviceType,g_FullDeviceNo->DeviceNo.DeviceNo2,address);
			// Ӧ������:�豸��ų���+4B��ַ
			net_send_echo_packet(RecivePacket, ECHO_OK, data, sizeof(DEVICE_NO)+4);
		}
	}
	else
	{
		//����Ƿ��ڵȴ�Ӧ�������
		if (FALSE == check_is_wait_echo(RecivePacket))
		{
			net_recive_dns_echo(RecivePacket->address, RecivePacket->data+NET_HEAD_SIZE, head->DataLen);
		}
	}
}


/*************************************************
  Function:    		distribute_proc
  Description:		���ݷַ��̴߳������
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
static void* distribute_proc(void* arg)
{
	PRECIVE_PACKET recPacket;
	PSEND_PACKET sendPacket;
	PFRESPONSION ResponsionFunc;
	PFDISTRIBUTE DistributeFunc;
	PNET_HEAD netHead;
	NET_COMMAND cmd;
	int32 ret = FALSE;

    while (g_DistributeRunning)
    {
		recPacket = net_get_recive_packet();
		if (NULL == recPacket)
		{
			usleep(1000);
			continue;
		}
		
		netHead = (PNET_HEAD)(recPacket->data + MAIN_NET_HEAD_SIZE);
		dprintf("have packet for distribute! DirectFlag : %x \n ", netHead->DirectFlag);
	// DNS��������շַ�
	#if 0					
		cmd = net_get_cmd_by_nethead(netHead);
		switch(cmd)
		{
			case CMD_SEND_MAXDATA:					// �������
			case CMD_STOP_SEND_MAXDATA: 			// �ж϶������
				recive_multpacket(cmd, recPacket);
				continue;
			case CMD_DNS_GETIP:						// DNS�㲥��
				recive_dns_packet(cmd, recPacket);
				continue;
				break;
			default:
				break;
		}
	#endif
	
		// ��λ���������͵ķ��Ͱ�	
		if (DIR_SEND == netHead->DirectFlag)	
		{
			DistributeFunc = get_distribute_func(netHead->SubSysCode);
			
			if (NULL != DistributeFunc)
			{
				ret = DistributeFunc(recPacket);
			}
			
			// �Ƿ�����,��û�лص�����ͳһӦ�������	
			if (ret == FALSE)
			{
				net_send_echo_packet(recPacket, ECHO_ERROR, NULL, 0);
			}	
		}
		// ��λ��Ӧ���
		else if (DIR_ACK == netHead->DirectFlag)	
		{
			cmd = net_get_cmd_by_nethead(netHead);

			// �ж��Ƿ��ǵȴ���Ӧ��
			if (check_is_wait_echo(recPacket))
			{
				net_del_recive_packet(recPacket);
				continue;
			}
			
			// �ж��Ƿ��ϱ��¼�Ӧ��
			/*
			if (check_is_report_event_echo(cmd, netHead->PackageID))
			{
				free_recive_node(recPacket);
				continue;
			}
			*/

			// ����Ӧ����ַ�
			sendPacket = net_get_sendpacket_by_responsion(netHead);
			ResponsionFunc = get_responsion_func(netHead->SubSysCode);
			if (NULL != ResponsionFunc)
			{
				ResponsionFunc(recPacket, sendPacket);
			}	
			if (NULL != sendPacket)
			{
				net_del_send_packet(sendPacket);
				sendPacket = NULL;
			}	
		}
		net_del_recive_packet(recPacket);
	}
	
  	pthread_exit(NULL);	
}

/*************************************************
  Function:    		start_distribute_thread
  Description:		�������ݷַ��߳�
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
static void start_distribute_thread(void)
{
    if (!g_DistributeRunning)
    {
        g_DistributeRunning = 1;
        pthread_create(&g_DistributeThread, NULL, distribute_proc, NULL);
    }
}

/*************************************************
  Function:    		stop_distribute_thread
  Description:		ֹͣ���ݷַ��߳�
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
static void stop_distribute_thread(void)
{   
	if (g_DistributeRunning)
    {
        g_DistributeRunning = 0 ;
        pthread_join(g_DistributeThread, NULL);
    }
}

/*************************************************
  Function:    		net_start_comm
  Description:		����ͨѶģ��
  Input: 
	1.FullDeviceNo	�豸���
	2.NetParam		�����������ָ��
  Output:			��
  Return:			��
  Others:
*************************************************/
void net_start_comm(PFULL_DEVICE_NO FullDeviceNo, PNET_PARAM NetParam)
{
	net_packet_list_init();
	sendbuf_sem_init();
	sendwait_sem_init();
	wait_echo_list_init();
	g_FullDeviceNo = FullDeviceNo;							// �����豸��
	g_NetHead->ProtocolVer = _PROTOCOL_VER_;				// Э��汾
	g_NetHead->DeviceType = g_FullDeviceNo->DeviceType;		// �豸����

	#ifndef _AU_PROTOCOL_
	g_NetHead->Priry = 0xFF;                                // Э�����ȼ�����ʱ��ΪĬ��
	g_NetHead->SrcDeviceNo = g_FullDeviceNo->DeviceNo;		// �豸���
	net_fill_main_nethead();                                // ��Э������
	#else
	g_NetHead->DeviceNo = g_FullDeviceNo->DeviceNo;			// �豸���
	g_NetHead->AreaCode = g_FullDeviceNo->AreaNo;			// С�����
	#endif
	net_set_send_timeout_echofunc(send_timeout_echo_func);	// ���ͳ�ʱӦ������
	net_set_dns_packet_recive(recive_dns_packet);			// ����DNS�����սӿ�
	net_set_dns_areacode(g_FullDeviceNo->AreaNo);			// ����DNSС����
	net_start_udp_comm();									// ����UDPͨѶ
	start_distribute_thread();								// �������ݷַ��߳�
}

/*************************************************
  Function:    		net_stop_comm
  Description:		ֹͣͨѶģ��
  Input: 			��		
  Output:			��
  Return:			��
  Others:
*************************************************/
void net_stop_comm(void)
{
	net_stop_udp_comm();									// ֹͣUDPͨѶ
	stop_distribute_thread();								// ֹͣ���ݷַ��߳�
	sendbuf_sem_del();
	sendwait_sem_del();
}

/*************************************************
  Function:    		net_get_comm_device_type
  Description:		�������ģ���豸����
  Input: 			��
  Output:			��
  Return:			�豸����
  Others:
*************************************************/
DEVICE_TYPE_E net_get_comm_device_type(void)
{
	return g_NetHead->DeviceType;
} 

/*************************************************
  Function:    		net_change_comm_deviceno
  Description:		��������ģ���豸����Ϣ����С����
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
void net_change_comm_deviceno(void)
{
	#ifdef _AU_PROTOCOL_
	g_NetHead->AreaCode = g_FullDeviceNo->AreaNo;	// С�����
	g_NetHead->DeviceNo = g_FullDeviceNo->DeviceNo;	// �豸���
	#else
	g_FullDeviceNo->DeviceNo.AreaNo = g_FullDeviceNo->AreaNo;
	g_FullDeviceNo->DeviceNo.DeviceType = g_FullDeviceNo->DeviceType;
	
	g_NetHead->SrcDeviceNo.AreaNo = g_FullDeviceNo->AreaNo;		    	// С�����
	g_NetHead->SrcDeviceNo.DeviceType = g_FullDeviceNo->DeviceType;  	// �豸����
	g_NetHead->SrcDeviceNo = g_FullDeviceNo->DeviceNo;		// �豸���
	g_NetHead->DeviceType = g_FullDeviceNo->DeviceType;  	// �豸����
	#endif
	
	net_set_dns_areacode(g_FullDeviceNo->AreaNo);
	storage_clear_subdev_ip();
	// ����ע�� �豸��Ÿ��ĺ� ��Ҫ����ע��
	#ifdef _USE_NEW_CENTER_
	logic_reg_center_ini_again();
	#endif
}

/*************************************************
  Function:    		free_recive_multipacket
  Description:		�ͷŶ�����հ�
  Input: 
	1.index			�����е�λ��
  Output:			��
  Return:			��
  Others:
*************************************************/
static void free_recive_multipacket(int32 index)
{
	if ((index>=REC_MAX_DATA_NUM)||(g_MultRecList[index].ID==0))
	{
		return;
	}
	
	// ������IDΪ0	
	g_MultRecList[index].ID = 0;
	//g_MultRecList[index].data = NULL;
	//g_MultRecList[index].Recflags = NULL;
	g_MultRecList[index].size = 0;		
	g_MultRecList[index].total = 0;		
	g_MultRecList[index].num = 0;		
	g_MultRecList[index].address = 0;	
	g_MultRecList[index].port = 0;		
	g_MultRecList[index].func = NULL; 		
	if (g_MultRecList[index].data)
	{
		free(g_MultRecList[index].data);
		g_MultRecList[index].data = NULL;
	}
	if (g_MultRecList[index].Recflags)
	{
		free(g_MultRecList[index].Recflags);
		g_MultRecList[index].Recflags = NULL;
	}	
	g_CurReiveNum--; 
	if ((g_MultSendList==NULL)&&(0==g_CurReiveNum))
	{
		net_set_mult_packet_recive(NULL);			//ȡ��������ͽӿ�
	}
}

/*************************************************
  Function:    		free_recive_multipacket_all
  Description:		�ͷ�ȫ��������հ�
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
static void free_recive_multipacket_all(void)
{
	int32 i;
	for (i=0; i<REC_MAX_DATA_NUM; i++)
	{
		free_recive_multipacket(i);
	}	
	g_CurReiveNum = 0;	
}

/*************************************************
  Function:    		recive_multipacket_send
  Description:		���յ����Ͱ�����
  Input: 
	1.recPacket		���շ��Ͱ�
  Output:			��
  Return:			��
  Others:
*************************************************/
static void recive_multipacket_send(PRECIVE_PACKET recPacket)
{
	int32 i;
	uint16 CurNum;
	int32 size;
	char data[6];
	PNET_HEAD net_head = (PNET_HEAD)(recPacket->data + MAIN_NET_HEAD_SIZE);
	uint32 ID = *(uint32 *)(recPacket->data+NET_HEAD_SIZE);
	uint16 total = *(uint16 *)(recPacket->data+NET_HEAD_SIZE+4);
	
	//����������,������
	if (total > MULT_SEND_MAX_PACKET)
	{
		dprintf("total package num is too big\n");
		return;
	}	
	CurNum = *(uint16 *)(recPacket->data+NET_HEAD_SIZE+6);
	size = net_head->DataLen - 8;
	//�Ƿ��ж�
	if (CurNum > total)
	{
		dprintf("cur tol num is bigger than total package\n");
		return;
	}	
	if (size != (recPacket->size-NET_HEAD_SIZE-8))
	{
		dprintf("size is err!\n");
		return;
	}	
	if (CurNum < total)
	{
		if (size!=MULT_SEND_PACKET_SIZE)
		{
			dprintf("package size is err!\n");
			return;
		}	
	}
	
	//Ӧ��	
	memcpy(data, recPacket->data+NET_HEAD_SIZE, 4);
	memcpy(data+4, recPacket->data+NET_HEAD_SIZE+6, 2);
	net_send_echo_packet(recPacket, ECHO_OK, data, 6);
	
	//���һỰID
	for (i=0; i<REC_MAX_DATA_NUM; i++)
	{
		if (ID==g_MultRecList[i].ID)
		{
			if (recPacket->address!=g_MultRecList[i].address )
			{
				dprintf("addr is err\n");
				return;
			}	
			if (recPacket->port!=g_MultRecList[i].port )
			{
				dprintf("port is err\n");
				return;
			}	
			g_MultRecList[i].t0 = 0;
			//��һ���յ���
			if (NULL==g_MultRecList[i].Recflags)
			{
				g_MultRecList[i].total = total;
				//����
				g_MultRecList[i].Recflags = malloc(total);
				//�ڴ����ʧ��
				if (NULL==g_MultRecList[i].Recflags)
				{
					dprintf("flags malloc err!\n");
					if (g_MultRecList[i].func)
					{
						g_MultRecList[i].func(g_MultRecList[i].ID, NULL, 0);
					}
					free_recive_multipacket(ID);
					return;
				}
				memset(g_MultRecList[i].Recflags, 0, total);
				
				//�������ݿռ�
				g_MultRecList[i].data = malloc(total*MULT_SEND_PACKET_SIZE);
				//�ڴ����ʧ��
				if (NULL==g_MultRecList[i].data)
				{	
					dprintf("data malloc err!\n");
					if (g_MultRecList[i].func)
					{
						g_MultRecList[i].func(g_MultRecList[i].ID, NULL, 0);
					}				
					free_recive_multipacket(ID);
					return;
				}
				memset(g_MultRecList[i].data, 0, total*MULT_SEND_PACKET_SIZE);
			}
			else
			{
				if (total!=g_MultRecList[i].total)
				{
					dprintf("total is err!\n");
					return;
				}	
			}
			if (g_MultRecList[i].Recflags[CurNum-1] != 1)
			{	
				memcpy(g_MultRecList[i].data+(CurNum-1)*MULT_SEND_PACKET_SIZE, 
					recPacket->data+NET_HEAD_SIZE+8,size);
				g_MultRecList[i].size += size;
				g_MultRecList[i].num++;

				//����OK��־
				g_MultRecList[i].Recflags[CurNum-1] = 1;
				
				if (g_MultRecList[i].state != NULL)
				{
					g_MultRecList[i].state(g_MultRecList[i].ID, total, g_MultRecList[i].num, g_MultRecList[i].size);
				}	
				//�������
				if (g_MultRecList[i].num==g_MultRecList[i].total)
				{
					if (g_MultRecList[i].func)
					{
						g_MultRecList[i].func(g_MultRecList[i].ID, g_MultRecList[i].data, g_MultRecList[i].size);
					}	
					//�ͷŷ�����ڴ�
					free_recive_multipacket(i);
				}
			}
			break;
		}
	}		
}

/*************************************************
  Function:    		stop_multipacket
  Description:		ֹͣĳ��������ͻ���չ���
  Input: 			
  	1.ID			�ػ�ID
  	2.flag			���ͻ���ձ�־, 0x01-�жϷ���(���ͷ�����) 0x02-�жϽ���(���շ�����)
  Output:			��
  Return:			��
  Others:
*************************************************/
static void stop_multipacket(uint32 ID, uint8 flag, uint32 address)
{
	int32 i;
	if (flag==0x01)
	{
		PMULTI_SEND prev = NULL; 
		PMULTI_SEND cur = g_MultSendList;
		while(cur)
		{
			if (cur->ID==ID && cur->address==address)
			{
				if (prev==NULL)
				{
					g_MultSendList = cur->next;
				}
				else
				{
					prev->next = cur->next;
				}
				if (cur == g_LastMultSend)
				{
					g_LastMultSend = prev; 
				}	
				if (cur == g_CurMultiSend)
				{
					g_CurMultiSend = NULL;
				}	
				free(cur);
				break;
			}
			prev = cur;
			cur = cur->next;
		}
	}
	else if (flag == 0x02)
	{
		for(i=0; i<REC_MAX_DATA_NUM; i++)
		{
			if (g_MultRecList[i].ID==ID && g_MultRecList[i].address==address)
			{
				free_recive_multipacket(i);
				break;
			}	
		}
	}
}

/*************************************************
  Function:    		recive_multipacket_echo
  Description:		���յ�Ӧ�������
  Input: 
	1.recPacket		����Ӧ���
  Output:			��
  Return:			��
  Others:
*************************************************/
static void recive_multipacket_echo(const PRECIVE_PACKET recPacket)
{
	uint32 * id = (uint32 *)(recPacket->data+NET_HEAD_SIZE);
	uint16 * pos = (uint16 *)(recPacket->data+NET_HEAD_SIZE+4);

	if (g_CurMultiSend!=NULL && g_SendOkNum<g_SendNum)
	{			
		if ((g_CurMultiSend->ID== *id) &&
			(g_CurMultiSend->address== recPacket->address) &&
			(g_CurMultiSend->port== recPacket->port))
		{
			// ��δӦ�����λ
			if (1!=g_SendFlags[*pos-1])
			{
				g_SendFlags[*pos-1] = 1;
				g_SendOkNum++;
				
				if (*pos==g_SendNum)
				{
					g_SendSize += g_LastSendSize;
				}	
				else
				{
					g_SendSize += MULT_SEND_PACKET_SIZE;
				}	
				if (g_CurMultiSend->state != NULL)
				{
					g_CurMultiSend->state(g_CurMultiSend->ID, g_SendNum, g_SendOkNum, g_SendSize);					
				}	
			}	
		}	
	}
}

/*************************************************
  Function:    		recive_multpacket
  Description:		���ն�����ʹ������
  Input: 			
  	1.RecivePacket	���հ�
  Output:			��
  Return:			��
  Others:
*************************************************/
static void recive_multpacket(NET_COMMAND cmd, const PRECIVE_PACKET RecivePacket)
{
	uint32 ID;
	uint8 flag;
	PNET_HEAD net_head = (PNET_HEAD)(RecivePacket->data + MAIN_NET_HEAD_SIZE);
	switch (cmd)
	{
		case CMD_SEND_MAXDATA:
			if (DIR_SEND == (net_head->DirectFlag))
			{
				recive_multipacket_send(RecivePacket);	
			}	
			else
			{
				recive_multipacket_echo(RecivePacket);
			}	
			break;
		case CMD_STOP_SEND_MAXDATA:
			if (DIR_SEND == (net_head->DirectFlag))
			{
				ID = *((uint32 *)(RecivePacket->data+NET_HEAD_SIZE));
				flag = RecivePacket->data[NET_HEAD_SIZE+4];
				if (flag!=0x01 && flag!=0x02)
				{
					net_send_echo_packet(RecivePacket, ECHO_ERROR, NULL, 0);
				}	
				else
				{	
					stop_multipacket(ID, flag, RecivePacket->address);
					net_send_echo_packet(RecivePacket, ECHO_OK, NULL, 0);
				}	
			}	
			break;
	}	
}

/*************************************************
  Function:    		net_start_multipacket_recive
  Description:		��ʼ�ְ���������
  Input: 
	1.address		�Է�IP��ַ
	2.port			�Է��˿ں�
	3.TimeOut		��ʱʱ��, ��, 0ʱ,ΪĬ�ϳ�ʱʱ��
	4.func			���ճɹ���Ӧ��ص�����
	5.state			����״̬�ص�����
  Output:			��
  Return:			���ջỰID, 0ΪæӦ��
  Others:
*************************************************/
uint32 net_start_multipacket_recive(uint32 address, uint16 port, uint8 TimeOut, PFMAX_RECIVE func, PFMAX_SEND_REC_STATE state)
{
	static int32 ID = 0;
	int32 i;
	int32 index = -1;
	if (NULL == func)
	{
		return 0;
	}
	
	if (g_CurReiveNum == REC_MAX_DATA_NUM)
	{
		return 0;
	}
	
	for (i=0; i<REC_MAX_DATA_NUM; i++)
	{
		if (0==g_MultRecList[i].ID)
		{
			
			index = i;
			break;
		}
	}		

	if (-1==index)
	{
		return 0;
	}
	if (ID==0 || ID==0xFFFFFFFF )
	{
		ID = 1;
	}	
	else
	{
		ID++;	
	}	
	g_MultRecList[index].data = NULL;					//��������
	g_MultRecList[index].Recflags = NULL;				//���ձ�־
	g_MultRecList[index].size = 0;						//�������ݳ���
	g_MultRecList[index].total = 0;						//�ܰ���
	g_MultRecList[index].num = 0;						//�Ѿ����հ���
	g_MultRecList[index].address = address;				//�Զ˵�ַ
	g_MultRecList[index].port = port;					//�Զ˶˿ں�
	g_MultRecList[index].func = func; 					//���ն���ص�����
	g_MultRecList[index].state = state;					//����״̬�ص�����
	g_MultRecList[index].t0 = 0;						//��ʼʱ��
	if (TimeOut==0)
	{
		g_MultRecList[index].TimeOut = DEFAULT_MULT_RECIVE_TIMEOUT;
	}	
	else	
	{
		g_MultRecList[index].TimeOut = TimeOut;			//��ʱ����
	}	
	//���ֵID
	g_MultRecList[index].ID = ID;						//�ỰID
	g_CurReiveNum++;
	net_set_mult_packet_recive(recive_multpacket); 		// ���ö�����ͽӿ�
	
	return ID;
}

/*************************************************
  Function:    		net_stop_multipacket_recive
  Description:		�жϷְ���������, (�û������жϷְ�����)
  Input: 
	1.ID			���ջỰID
  Output:			��
  Return:			��
  Others:
*************************************************/
void net_stop_multipacket_recive(uint32 ID)
{
	int32 i;
	for (i=0; i<REC_MAX_DATA_NUM; i++)
	{
		if (ID==g_MultRecList[i].ID)
		{
			//�����жϽ�������
			char data[5];
			memcpy(data, &ID, 4);
			data[4] = 0x02;
			set_nethead(G_MULTCMD_DEVNO, PRIRY_DEFAULT);
			net_direct_send(CMD_STOP_SEND_MAXDATA, data, 5, g_MultRecList[i].address, g_MultRecList[i].port);
			free_recive_multipacket(i);
			break;
		}
	}		
}

/*************************************************
  Function:    		net_recive_mult_timeout_proc
  Description:		������ճ�ʱ���
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
void* net_recive_mult_timeout_proc(void *arg)
{
	int32 i;

	while (1)
	{
		if (g_CurReiveNum == 0)
		{
			usleep(500*1000);
		}
		
		for (i=0; i<REC_MAX_DATA_NUM; i++)
		{
			if (g_MultRecList[i].ID>0 && g_MultRecList[i].TimeOut>0)
			{
				g_MultRecList[i].t0++;
				
				//���ճ�ʱ�� 500ms ��ʱ��
				if (g_MultRecList[i].t0*NET_MULTI_RECV_CHECK_TICKS*5 > g_MultRecList[i].TimeOut*1000)
				{
					if (g_MultRecList[i].func)
					{
						g_MultRecList[i].func(g_MultRecList[i].ID, NULL, 0);
					}	
					free_recive_multipacket(i);
				}
			}
		}
		usleep(500*1000);
	}
}

/*************************************************
  Function:    		mult_send
  Description:		�������һ��
  Input: 			
  	1.id			ip��ַ
  	2.num			������
  	3.curNo			��ǰ����, ��1��ʼ
  	4.data			������
  	5.size			���ݴ�С
  	6.address		���͵�ַ
  	7.port			���Ͷ˿�
  Output:			��
  Return:			��
  Others:
*************************************************/
static void mult_send(uint32 id, uint16 num, uint16 curNo, char * data, int32 size, 
						uint32 address, uint16 port)
{
	uint16 PackageID = get_package_ID();
	
	g_NetHead->PackageID = PackageID;						//����ʶ
	g_NetHead->DirectFlag = DIR_SEND;						//�����־��������0xAA��Ӧ��0x00��
	g_NetHead->SubSysCode = (uint8)(CMD_SEND_MAXDATA>>8);	//��ϵͳ����
	g_NetHead->command = (uint8)(CMD_SEND_MAXDATA & 0xFF);	//����ֵ
	g_NetHead->EchoValue = 0;								//Ӧ����
	g_NetHead->Encrypt = 0;									//��������
	g_NetHead->DataLen = size + 8;							//���ݳ���

	#ifndef _AU_PROTOCOL_
	// ��Э���ͷ
	set_nethead(G_MULTCMD_DEVNO, PRIRY_DEFAULT);
	g_Main_NetHead->mainSeq = PackageID;
	g_Main_NetHead->mainPacketType = 0x0000;				// ��ͨ��
	g_Main_NetHead->subPacketLen = sizeof(NET_HEAD) + g_NetHead->DataLen;
	g_NetHead->Priry = 0xFF;
	g_NetHead->DestDeviceNo = DestDeviceNo;
	g_NetHead->Priry = DefaultPriry;
	#endif
	
	memcpy(g_SendBuf+NET_HEAD_SIZE, &id, 4);				//�ỰID
	memcpy(g_SendBuf+NET_HEAD_SIZE+4, &num, 2);				//������
	memcpy(g_SendBuf+NET_HEAD_SIZE+6, &curNo, 2);			//��ǰ����
	memcpy(g_SendBuf+NET_HEAD_SIZE+8, data, size);			//������������

	net_send_cmd_packet(g_SendBuf, g_NetHead->DataLen+NET_HEAD_SIZE, address, port);
}

/*************************************************
  Function:    		mult_send_proc
  Description:		��������̴߳�����
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
static void mult_send_proc(void)
{	
	static int32 sendtimes = 0;						// ���ʹ���
	static int32 GroupNum = 28;						// ÿ�����
	static int32 SleepValue = 10;					// ֹͣʱ��			

	if (g_MultSendList == NULL)
	{
		return;
	}
	
	if (g_CurMultiSend == NULL)
	{
		g_CurMultiSend = g_MultSendList;
		
		// ���Ͱ��������һ���ֽ���
		g_SendNum = g_CurMultiSend->size/MULT_SEND_PACKET_SIZE;		
		g_LastSendSize = MULT_SEND_PACKET_SIZE;
		if ((g_CurMultiSend->size%MULT_SEND_PACKET_SIZE) > 0)
		{	
			g_LastSendSize = g_CurMultiSend->size - g_SendNum*MULT_SEND_PACKET_SIZE;
			g_SendNum++;
		}	
		
		g_SendOkNum = 0;
		g_SendSize = 0;
		g_SendFlags = malloc(g_SendNum);					//����Ӧ���־
		if (g_SendFlags)
		{
			memset(g_SendFlags, 0, g_SendNum);
		}
		else
		{
			//Ӧ����ʧ��
			if (g_CurMultiSend->func != NULL)	
			{
				g_CurMultiSend->func(g_CurMultiSend->ID, 1);
			}
			g_MultSendList = g_MultSendList->next;
			free(g_CurMultiSend);
			g_CurMultiSend = NULL;
			if ((g_MultSendList==NULL)&&(0==g_CurReiveNum))
			{
				net_set_mult_packet_recive(NULL);	// ȡ��������ͽӿ�
			}
			return;
		}
		
		sendtimes = 0;
		GroupNum = 28;
		SleepValue = 10;
	}
	else
	{
		int32 i = 0;
		int32 LastSendOK = g_SendOkNum; 			// �ϴη��ͳɹ���
		char * data = g_CurMultiSend->data;
		int32 SendNum = 0;							// ��һ��, ���η��͵�����

		// ���ͳ����һ����δ���ͳɹ��İ�
		for(i = 0; i < g_SendNum-1; i++)
		{
			if (g_SendFlags[i]==0)
			{	
				mult_send(g_CurMultiSend->ID, g_SendNum, i+1, data, MULT_SEND_PACKET_SIZE, 
					g_CurMultiSend->address, g_CurMultiSend->port);
				SendNum++;
				DelayMs_nops(5);
				if (SendNum%GroupNum==0)
				{
					DelayMs_nops(SleepValue);	
				}
			}
			data += MULT_SEND_PACKET_SIZE;
		}

		// �������һ��.������һ��,���ʹ�������
		if (g_SendFlags[g_SendNum-1]==0)
		{	
			mult_send(g_CurMultiSend->ID, g_SendNum, i+1, data, 
				g_LastSendSize, g_CurMultiSend->address, g_CurMultiSend->port);
			DelayMs_nops(10);	
			data += g_LastSendSize;
		}
		sendtimes++;

		// �ȴ�Ӧ��
		i = 5;
		while(g_SendOkNum<g_SendNum && i>0)
		{
			// ���η��Ͷ��Ѿ�����
			if ((g_SendOkNum-LastSendOK)==SendNum)
			{
				break;
			}	
			DelayMs_nops(100);
			i--;
		}
			
		if (g_SendOkNum == g_SendNum)
		{
			// Ӧ���ͳɹ�
			if (g_CurMultiSend->func!=NULL)	
			{
				g_CurMultiSend->func(g_CurMultiSend->ID, 0);
			}	
			g_MultSendList = g_MultSendList->next;
			free(g_CurMultiSend);
			g_CurMultiSend = NULL;
			free(g_SendFlags);
			g_SendFlags = NULL;
			if ((g_MultSendList==NULL)&&(0==g_CurReiveNum))
			{
				net_set_mult_packet_recive(NULL);			//ȡ��������ͽӿ�
			}
			return;
		}

		// ��Ӧ����ʹ�������100��,����ʧ��
		if(LastSendOK == g_SendOkNum || sendtimes>100)
		{
			//Ӧ����ʧ��
			if (g_CurMultiSend->func!=NULL)	
			{
				g_CurMultiSend->func(g_CurMultiSend->ID, 1);
			}	
			g_MultSendList = g_MultSendList->next;
			free(g_CurMultiSend);
			g_CurMultiSend = NULL;
			free(g_SendFlags);
			g_SendFlags = NULL;
			if ((g_MultSendList==NULL)&&(0==g_CurReiveNum))
			{
				net_set_mult_packet_recive(NULL);			//ȡ��������ͽӿ�
			}
			return;
		}
		else //���㶪����, ���ٷ��ͼ�����Ͱ��������ӷ��ͼ��ʱ��
		{
			if (1==GroupNum)
			{
				SleepValue += 10; 					//����ʱ��
			}
			else
			{	
				int32 LostRate =100*(SendNum-(g_SendOkNum-LastSendOK))/SendNum;
				if (LostRate>0)
				{
					int32 n = LostRate/10;

					//����GroupNumֵ
					if (n==0)
					{
						GroupNum--;
					}	
					else	
					{
						GroupNum -= n;
					}	
					if (GroupNum<1)
					{
						GroupNum = 1;
						SleepValue += 10;
					}	
				}	
			}
		}
	}	
}	

/*************************************************
  Function:    		net_multi_send_check
  Description:		����Ƿ��ж������������Ҫ����
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
void net_multi_send_check(void)
{
	if (g_MultSendList)
	{
	#if USE_DISTRIBUTE_THREAD
		//RockOSSendMsg(MB_NETDIST, AS_NET_MULTI_SEND_NOTIFY, NULL);
	#endif	
	}
	else
	{
		return;
	}
}

/*************************************************
  Function:    		net_start_multi_packet_send
  Description:		�ְ����ݷ���
  Input: 
	1.ID			�ỰID
	2.data			����
	3.size			���ݴ�С
	4.address		�Է�IP��ַ
	5.port			�Է��˿ں�
	6.func			���ͳɹ���Ӧ��ص�����
	7.state			����״̬�ص�����
  Output:			��
  Return:			�Ƿ�ɹ�, TRUE / FALSE
  Others:
*************************************************/
int32 net_start_multi_packet_send(uint32 ID, char * data, int32 size, uint32 address, uint16 port, PFMAX_SEND_ECHO func, PFMAX_SEND_REC_STATE state)
{
	int32 num;
	PMULTI_SEND node = NULL;
	if (data == NULL)
	{
		return FALSE;
	}
	if (size < 1) 
	{
		return FALSE;
	}	
	num = size/MULT_SEND_PACKET_SIZE;
	if (size%MULT_SEND_PACKET_SIZE != 0)
	{
		num++;
	}	
	if (num > MULT_SEND_MAX_PACKET)
	{
		return FALSE;
	}
		
	node = (PMULTI_SEND)calloc(sizeof(MULTI_SEND), 1);
	if (node==NULL)
	{
		return FALSE;
	}	
	node->ID = ID;
	node->size = size;
	node->data = data;
	node->address = address;
	node->port = port;
	node->func = func;
	node->state = state;
	node->next = NULL;
	if (NULL==g_MultSendList)
	{	
		g_MultSendList = node;
		g_LastMultSend = node;
	}	
	else
	{
		g_LastMultSend->next = node;
		g_LastMultSend = node;
	}
	net_set_mult_packet_recive(recive_multpacket); 		// ���ö�����ͽӿ�
#if USE_DISTRIBUTE_THREAD	
	//RockOSSendMsg(MB_NETDIST, AS_NET_MULTI_SEND_NOTIFY, NULL);
#endif	
	return TRUE;
}

/*************************************************
  Function:    		net_stop_multipacket_send
  Description:		�жϷְ���������, (�û������жϷְ�����)
  Input: 
	1.ID			���ͻỰID
	2.address		���͵�ַ
  Output:			��
  Return:			��
  Others:
*************************************************/
void net_stop_multipacket_send(uint32 ID, uint32 address)
{
	PMULTI_SEND prev = NULL; 
	PMULTI_SEND cur = g_MultSendList;
	char data[5];
	while(cur)
	{
		if (cur->ID==ID && cur->address==address)
		{
			if (prev==NULL)
			{
				g_MultSendList = cur->next;
			}
			else
			{
				prev->next = cur->next;
			}
			if (cur==g_LastMultSend)
			{
				g_LastMultSend = prev; 
			}	
			if (cur==g_CurMultiSend)
			{
				g_CurMultiSend = NULL;
			}	
			//�����жϷ�������
			memcpy(data, &ID, 4);
			data[4] = 0x01;
			set_nethead(G_MULTCMD_DEVNO, PRIRY_DEFAULT);
			net_direct_send(CMD_STOP_SEND_MAXDATA, data, 5, cur->address, cur->port);
			free(cur);
			break;
		}
		prev = cur;
		cur = cur->next;
	}
}

/*************************************************
  Function:    		net_get_utc_time
  Description:		��ȡUTCʱ��
  Input: 
  Output:			
  	1.DateTime		ʱ��ṹ��
  Return:			��
  Others:			Ĭ��ʱ��Ϊ8
*************************************************/
void net_now_time(DATE_TIME * DateTime)
{
	DATE_TIME LocalTime;
	get_timer(&LocalTime);
	memset(DateTime, 0, sizeof(DATE_TIME));
	DateTime->year = LocalTime.year;
	DateTime->month = LocalTime.month;
	DateTime->day = LocalTime.day;
	DateTime->hour = LocalTime.hour;
	DateTime->min = LocalTime.min;
	DateTime->sec = LocalTime.sec;
}

/*************************************************
  Function:    		net_get_utc_time
  Description:		��ȡUTCʱ��
  Input: 
  Output:			
  	1.DateTime		ʱ��ṹ��
  Return:			��
  Others:			Ĭ��ʱ��Ϊ8
*************************************************/
void net_get_utc_time(DATE_TIME * DateTime)
{
	DATE_TIME LocalTime;
	get_timer(&LocalTime);

#if 0
	// 8 ʱ��Ĭ��
	if (LocalTime.hour >= 8)
	{
		LocalTime.hour -= 8;
	}
	else
	{
		// ʱ��
		LocalTime.hour = (LocalTime.hour+24) - 8;

		// day > 1
		if (LocalTime.day > 1)
		{
			LocalTime.day--;
		}
		// day == 1
		else
		{
			// month > 1
			if (LocalTime.month > 1)
			{
				LocalTime.month--;
				LocalTime.day = MonthDays(LocalTime.year, LocalTime.month);
			}
			// month == 1
			else
			{
				LocalTime.year--;
				LocalTime.month = 12;
				LocalTime.day = MonthDays(LocalTime.year, LocalTime.month);
			}
		}
	}
#else
	get_utc_time(&LocalTime, 8);
#endif

	memset(DateTime, 0, sizeof(DATE_TIME));
	DateTime->year = LocalTime.year;
	DateTime->month = LocalTime.month;
	DateTime->day = LocalTime.day;
	DateTime->hour = LocalTime.hour;
	DateTime->min = LocalTime.min;
	DateTime->sec = LocalTime.sec;
}

