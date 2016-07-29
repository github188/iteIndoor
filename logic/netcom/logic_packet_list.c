/*************************************************
  Copyright (C), 2006-2010, Aurine
  File name: 	logic_packet_list.c
  Author:    	xiewr
  Version:   	1.0
  Date: 
  Description:  ���緢�ͽ�������
				
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
#include "logic_packet_list.h"


static PSEND_PACKET g_SendFreeList = NULL;			// ���Ϳ��нڵ�����, ��������
static PRECIVE_PACKET g_RecvFreeList = NULL;		// ���տ��нڵ�����, ��������

static PSEND_PACKET g_SendList = NULL;				// ���Ͱ��б�
static PSEND_PACKET g_SendLastNode = NULL;			// ���Ͱ��б����ڵ�

static PRECIVE_PACKET g_ReciveList = NULL;  		// ���ն���
static PRECIVE_PACKET g_ReciveLastNode = NULL;  	// ���ն���β

static sem_t g_SendListSem;							// ���Ͷ����ź���
static sem_t g_RecvListSem;							// ���ն����ź���

static int g_SendRunning = 0;						// �����߳��Ƿ�����
static int g_ReciveRunning = 0;						// �����߳��Ƿ�����

static pthread_t SendThread;						// �����߳�
static pthread_t TimeOutThread;						// ���ͳ�ʱ�ط������߳�
static pthread_t ReciveThread;						// �����߳�
static pthread_t MulTimeOutThread;					// ������ճ�ʱ�ط������߳�

static PFSEND_TIMEOUT_ECHO g_SendTimeoutFunc = NULL;// ���ͳ�ʱӦ����ָ��

/*************************************************
  Function:    		recvlist_lock
  Description: 		�����б�����
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
static void recvlist_lock(void)
{
	sem_wait(&g_RecvListSem);
}

/*************************************************
  Function:    		recvlist_unlock
  Description: 		�����б����
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
static void recvlist_unlock(void)
{
	sem_post(&g_RecvListSem);
}

/*************************************************
  Function:    		sendlist_lock
  Description: 		�����б�����
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
static void sendlist_lock(void)
{
	sem_wait(&g_SendListSem);
}

/*************************************************
  Function:    		sendlist_unlock
  Description: 		�����б����
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
static void sendlist_unlock(void)
{
	sem_post(&g_SendListSem);
}

/*************************************************
  Function:    		list_sem_init
  Description: 		�ź�����ʼ��
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
static void list_sem_init(void)
{
	sem_init(&g_SendListSem, 0, 1);   			
	sem_init(&g_RecvListSem, 0, 1);   			
}

/*************************************************
  Function:    		list_sem_del
  Description: 		�ź���ɾ��
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
static void list_sem_del(void)
{
	sem_destroy(&g_SendListSem);   			
	sem_destroy(&g_RecvListSem);   			
}

/*************************************************
  Function:    		net_packet_list_init
  Description: 		��ʼ������,����ռ�
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
void net_packet_list_init(void)
{
	uint32 i = 0;
	PSEND_PACKET SendNode = NULL;
	PRECIVE_PACKET RecvNode = NULL;
	if (g_SendFreeList == NULL)
	{
		for (i = 0; i < PACKET_NODE_MAX_NUM; i++)
		{
			// ����һ���ڵ�
			SendNode = (PSEND_PACKET)malloc(sizeof(SEND_PACKET));
			if (SendNode == NULL)
			{
				continue;
			}
			else
			{
				memset(SendNode, 0, sizeof(SEND_PACKET));
				SendNode->data = (char*)malloc(PACKET_NODE_DATA_SIZE);
				if (SendNode->data == NULL)
				{
					free(SendNode);
					SendNode = NULL;
					continue;
				}
			}

			// �ѽڵ�ӵ�����
			if (g_SendFreeList == NULL)
			{
				g_SendFreeList = SendNode;
				g_SendFreeList->next = NULL;
			}
			else
			{
				SendNode->next = g_SendFreeList;
				g_SendFreeList = SendNode;
			}
		}
	}

	if (g_RecvFreeList == NULL)
	{
		for (i = 0; i < PACKET_NODE_MAX_NUM; i++)
		{
			// ����һ���ڵ�
			RecvNode = (PRECIVE_PACKET)malloc(sizeof(RECIVE_PACKET));
			if (RecvNode == NULL)
			{
				continue;
			}
			else
			{
				memset(RecvNode, 0, sizeof(RECIVE_PACKET));
				RecvNode->data = (char*)malloc(PACKET_NODE_DATA_SIZE);
				if (RecvNode->data == NULL)
				{
					free(RecvNode);
					RecvNode = NULL;
					continue;
				}
			}

			// �ѽڵ�ӵ�����
			if (g_RecvFreeList == NULL)
			{
				g_RecvFreeList = RecvNode;
				g_RecvFreeList->next = NULL;
			}
			else
			{
				RecvNode->next = g_RecvFreeList;
				g_RecvFreeList = RecvNode;
			}
		}
	}	
	list_sem_init();
}

/*************************************************
  Function:    		get_sendnode_from_freelist
  Description: 		�ӷ��Ϳ��������л�ȡ�ڵ�
  Input: 			��
  Output:			��
  Return:			�ڵ�ָ��
  Others:
*************************************************/
static PSEND_PACKET get_sendnode_from_freelist(void)
{
	PSEND_PACKET tmp = NULL;
	if (g_SendFreeList == NULL)
	{
		dprintf("send free list is null\n");
		return NULL;
	}

	sendlist_lock();
	if (g_SendFreeList->next == NULL)
	{
		tmp = g_SendFreeList;
		g_SendFreeList = NULL;
	}
	else
	{
		tmp = g_SendFreeList;
		g_SendFreeList = tmp->next;
	}
	sendlist_unlock();
	return tmp;
}

/*************************************************
  Function:    		add_sendnode_to_freelist
  Description: 		�ѽڵ���ӵ����Ϳ�������
  Input: 			
  	1.node			���ͽڵ�
  Output:			��
  Return:			
  Others:
*************************************************/
static void add_sendnode_to_freelist(PSEND_PACKET node)
{
	if (node == NULL)
	{
		return;
	}

	sendlist_lock();
	if (g_SendFreeList == NULL)
	{
		g_SendFreeList = node;
		g_SendFreeList->next = NULL;
	}
	else
	{
		node->next = g_SendFreeList;
		g_SendFreeList = node;
	}
	sendlist_unlock();
}

/*************************************************
  Function:    		get_recvnode_from_freelist
  Description: 		�ӽ��տ��������л�ȡ�ڵ�
  Input: 			��
  Output:			��
  Return:			���հ��ڵ�ָ��
  Others:
*************************************************/
static PRECIVE_PACKET get_recvnode_from_freelist(void)
{
	PRECIVE_PACKET tmp = NULL;
	if (g_RecvFreeList == NULL)
	{
		return NULL;
	}

	recvlist_lock();
	if (g_RecvFreeList->next == NULL)
	{	
		tmp = g_RecvFreeList;
		g_RecvFreeList = NULL;
	}
	else
	{
		tmp = g_RecvFreeList;
		g_RecvFreeList = tmp->next;
	}
	recvlist_unlock();
	return tmp;
}

/*************************************************
  Function:    		add_recvnode_to_freelist
  Description: 		�ѽ��հ��ڵ���ӵ���������
  Input: 			
  	1.node			���հ�
  Output:			��
  Return:			��
  Others:
*************************************************/
static void add_recvnode_to_freelist(PRECIVE_PACKET node)
{
	if (node == NULL)
	{
		return;
	}

	recvlist_lock();
	if (g_RecvFreeList == NULL)
	{
		g_RecvFreeList = node;
		g_RecvFreeList->next = NULL;
	}
	else
	{
		node->next = g_RecvFreeList;
		g_RecvFreeList = node;
	}
	recvlist_unlock();
}

/*************************************************
  Function:    		append_send_packet
  Description: 		���뷢�ͽڵ㵽����
  Input: 
	1.node			����ڵ�
  Output:			��
  Return:			��
  Others:
*************************************************/
static void append_send_packet(PSEND_PACKET node)
{
	sendlist_lock();
	if (g_SendList == NULL)
	{
		g_SendList = node;
		node->prev = NULL;
	}	
	else
	{
		g_SendLastNode->next = node;
		node->prev = g_SendLastNode;
	}
	g_SendLastNode = node;
	sendlist_unlock();
}

/*************************************************
  Function:    		net_add_send_packet
  Description: 		����һ�����Ͱ�������
  Input: 
	1.data			��������
	2.size			�������ݴ�С
	3.address		���͵�ַ
	4.port			���Ͷ˿�
	5.SendTimes		���ʹ���, Ĭ�Ϸ��ʹ���Ϊ1
	6.TimeOut		���ͳ�ʱʱ��, Ĭ��ΪTIMEOUT_VALUE
  Output:			��
  Return:			���Ͱ�
  Others:
*************************************************/
PSEND_PACKET net_add_send_packet(char * data, int32 size, uint32 address, uint16 port, 
	int32 SendTimes, int32 TimeOut)
{
	PSEND_PACKET sendPacket = NULL;
	if (size > PACKET_NODE_DATA_SIZE)
	{
		dprintf("size err\n");
		return NULL;
	}
	
	sendPacket = get_sendnode_from_freelist();
	if (sendPacket == NULL)
	{
		dprintf("no free send node\n");
		return NULL;
	}	
	memset(sendPacket->data, 0, PACKET_NODE_DATA_SIZE);
	memcpy (sendPacket->data, data, size);
	sendPacket->address = address;					// �Է�IP��ַ
	sendPacket->port = port;						// �Է��˿ں�
	sendPacket->size = size;						// ���ݰ���С
	sendPacket->SendState = SEND_STATE_SEND_READY;	// ����״̬
	sendPacket->SendTimes = 0; 						// ��ǰ���ʹ���
	sendPacket->SendTimesMax = SendTimes; 			// ��෢�ʹ���
	sendPacket->TimeOut = TimeOut;
	sendPacket->WaitCount = 0; 						// �ȴ�ʱ�����
	sendPacket->next = NULL;
	append_send_packet(sendPacket);
	return sendPacket;
}

/*************************************************
  Function:    		remove_send_packet
  Description: 		�Ƴ����Ͱ�, ����ӵ���������
  Input: 
	1.node			Ҫ�Ƴ��Ľڵ�
  Output:			��
  Return:			��
  Others:
*************************************************/
static void remove_send_packet(PSEND_PACKET node)
{
	if (node == NULL)
	{
		return;
	}

	sendlist_lock();
	if (g_SendList == node)
	{
		g_SendList = node->next;
		if (g_SendList != NULL)
		{
			g_SendList->prev = NULL;
		}	
		if (g_SendLastNode == node)
		{
			g_SendLastNode = NULL;
		}	
	}
	else if (g_SendLastNode == node)
	{
		g_SendLastNode = g_SendLastNode->prev;
		g_SendLastNode->next = NULL;
	}
	else	
	{
		node->prev->next = node->next; 
		node->next->prev = node->prev;
	}
	node->next = NULL;
	node->prev = NULL;
	sendlist_unlock();
}

/*************************************************
  Function:    		net_del_send_packet
  Description: 		�ӷ��������Ƴ�һ�������, �ӵ���������
  Input: 
	1.node			���Ͱ�
  Output:			��
  Return:			��
  Others:
*************************************************/
void net_del_send_packet(PSEND_PACKET node)
{
	if (node == NULL)
	{
		return;
	}
	add_sendnode_to_freelist(node);
}

/*************************************************
  Function:    		find_send_packet
  Description:		�ӷ����б�����һ�����Ͱ�
  Input: 			��
  Output:			��
  Return:			���Ͱ�
  Others:
*************************************************/
static PSEND_PACKET find_send_packet(void)
{
	PSEND_PACKET node = g_SendList;
	
	while (node != NULL)
	{
		if (node->SendState == SEND_STATE_SEND_READY)  // ��������
		{
			break;
		}
		node = node->next;
	}
	return node;
}

/*************************************************
  Function:    		net_send_proc
  Description:		�����̴߳�����
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
void* net_send_proc(void *arg)  
{  
	while (g_SendRunning)
	{
		// ����һ�����Է��͵İ�
		PSEND_PACKET sendPacket = find_send_packet();
		if (sendPacket != NULL)
		{
			sendPacket->SendState = SEND_STATE_SENDING;
			net_send_cmd_packet(sendPacket->data, sendPacket->size, sendPacket->address, sendPacket->port);
			sendPacket->SendTimes++;
			sendPacket->SendState = SEND_STATE_WAIT_ECHO;
			usleep(10*1000);
		}
		else
		{
			usleep(500*1000);
			// �˴������Ƿ�����ź����ж�
		}
	}	
	pthread_exit(NULL);
} 

/*************************************************
  Function:    		net_timeout_proc
  Description:		��ʱ�̴߳�����
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
void* net_timeout_proc(void *arg)
{
	PSEND_PACKET node;
	PSEND_PACKET temp;
	while (g_SendRunning)
	{
		node = g_SendList;
		
		if (node != NULL)
		{
			while (node != NULL)
			{
				if (node->SendState == SEND_STATE_WAIT_ECHO)  			// �ȴ�Ӧ��
				{
					node->WaitCount++; 
					if (node->WaitCount >= node->TimeOut) 				// Ӧ��ʱ
					{
						if (node->SendTimes < node->SendTimesMax)  		// ׼���ط�
						{
							node->WaitCount = 0;
							node->SendState = SEND_STATE_SEND_READY;
						}
						else
						{
							node->SendState = SEND_STATE_ECHO_TIMEOUT; 	// Ӧ��ʱ, ȡ������
							temp = node;
							node = node->next;
							if (NULL != g_SendTimeoutFunc)
							{
								g_SendTimeoutFunc(temp);				// ֪ͨ���ͷ�
							}	
							remove_send_packet(temp);
							net_del_send_packet(temp);
							continue; 
						}
					}
				}
				node = node->next;
			}

			sleep(1);
		}
		else
		{
			sleep(1);
			// �˴������Ƿ�����ź����ж�
		}
	}
	
	pthread_exit(NULL);
}

/*************************************************
  Function:    		net_add_recive_packet
  Description:		�����������ݰ��߳�ִ�к���
  Input: 			
  	1.data			����
  	2.size			��С
  	3.address		��ַ
  	4.port			�˿ں�
  Output:			��
  Return:			��
  Others:			���һ�����հ�����������
*************************************************/
void net_add_recive_packet(char *data, uint32 size, uint32 address, uint16 port)
{
	PRECIVE_PACKET recivePacket = NULL;

	if (0 == size || data == NULL || size > PACKET_NODE_DATA_SIZE)
	{
		return;
	}
	recivePacket = get_recvnode_from_freelist();
	if (NULL == recivePacket)
	{
		return;
	}
	
 	recivePacket->address = address;
 	recivePacket->port = port;
 	recivePacket->size = size;
 	memcpy (recivePacket->data, data, size);
 	recivePacket->next = NULL;

 	recvlist_lock();
 	if (NULL == g_ReciveList)
	{
		g_ReciveLastNode = recivePacket;
		g_ReciveList = recivePacket;
	}
	else
	{
		g_ReciveLastNode->next = recivePacket;
		g_ReciveLastNode = recivePacket;
	}
	recvlist_unlock();
}

/*************************************************
  Function:    		net_get_recive_packet
  Description:		��ȡһ�����հ�, �ӽ����������Ƴ��ýڵ�
  Input: 			��
  Output:			��
  Return:			���հ�
  Others:
*************************************************/
PRECIVE_PACKET net_get_recive_packet(void)
{
	PRECIVE_PACKET node;
	if (NULL == g_ReciveList)
	{
		return NULL;
	}

	recvlist_lock();
	node = g_ReciveList;
	if (g_ReciveLastNode == g_ReciveList)
	{
		g_ReciveList = NULL;
		g_ReciveLastNode = NULL;
	}
	else
	{
		g_ReciveList = g_ReciveList->next;
	}
	recvlist_unlock();
	node->next = NULL;
	return node;
}

/*************************************************
  Function:    		net_del_recive_packet
  Description:		��һ�����հ���ӵ���������
  Input: 			
	1.node			���ڵ�
  Output:			��
  Return:			��
  Others:
*************************************************/
void net_del_recive_packet(PRECIVE_PACKET node)
{
	if (NULL == node)
	{
		return;
	}
	 
	add_recvnode_to_freelist(node);
}

/*************************************************
  Function:    		net_get_sendpacket_by_responsion
  Description:		����Ӧ���ͷ,��÷��Ͱ�
  Input: 
	1.netHead		Ӧ���ͷ
  Output:			��
  Return:			���Ͱ�
  Others:
*************************************************/
PSEND_PACKET net_get_sendpacket_by_responsion(PNET_HEAD netHead)
{
	PSEND_PACKET node = g_SendList;
	
	while (node != NULL)
	{
		if (node->SendState == SEND_STATE_WAIT_ECHO && 
			((PNET_HEAD)((node->data) + MAIN_NET_HEAD_SIZE))->PackageID == netHead->PackageID)  // ��������
		{
			remove_send_packet(node);	
			return node;
		}
		node = node->next;
	}
	return NULL;	
}

/*************************************************
  Function:    		net_set_send_timeout_echofunc
  Description:		���÷��ͳ�ʱӦ��ص�����
  Input: 
	1.func			���ͳ�ʱӦ����ָ��
  Output:			��
  Return:			��
  Others:
*************************************************/
void net_set_send_timeout_echofunc(PFSEND_TIMEOUT_ECHO func)
{
	g_SendTimeoutFunc = func;
}

/*************************************************
  Function:    		start_comm_thread
  Description:		���������߳�
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
void net_start_comm_thread(void)
{
 	g_SendRunning = TRUE ;
 	g_ReciveRunning = TRUE ;
 	pthread_create(&SendThread, NULL, net_send_proc, NULL);
 	pthread_create(&TimeOutThread, NULL, net_timeout_proc, NULL);
 	pthread_create(&ReciveThread, NULL, recive_cmd_data_proc, NULL);
	pthread_create(&MulTimeOutThread, NULL, net_recive_mult_timeout_proc, NULL);
}

/*************************************************
  Function:    		stop_comm_thread
  Description:		ֹͣ�����߳�
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
void net_stop_comm_thread(void)
{
	g_SendRunning = FALSE ;
	g_ReciveRunning = FALSE ;

	pthread_join(ReciveThread, NULL);
	pthread_join(TimeOutThread, NULL);
	pthread_join(SendThread, NULL);
	list_sem_del();
}

