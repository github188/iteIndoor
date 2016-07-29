/*************************************************
  Copyright (C), 2006-2010, Aurine
  File name: 	logic_send_event.c
  Author:   	xiewr	
  Version:  	1.0
  Date: 
  Description:  �����¼�ģ��
				�ϱ�һ���¼��򱨾��¼�������Է����������ȱ���
  History:        
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#include "logic_send_event.h"

#define	FILE_NAME		"D:\\nosend_event.dat"			// �¼������ļ��洢·��
#define	SENDTO_NUM		3							// ����Ŀ�ĵص�����������ƽ̨,���ķ�����,����Ա����

#pragma pack(push,1)  								// ��ʼ�������ݰ�, ʹ���ֽڶ��뷽ʽ
// �洢״̬
typedef struct _STORE_STATUS
{
	uint32 head;
	uint32 tail;
	uint32 count;
	char reserve[4];								// ����
}STORE_STATUS;

// �ļ�ͷ
typedef struct _FILE_HEAD
{
	uint16 RecordCount;								// �ɴ洢�ļ�¼��
	uint16 RecordLen;								// ÿ����¼��
	uint16 SendtoNum;								// �洢STORE_STATUS�ṹ��
	char reserve[10];								// ����
	STORE_STATUS StoreStatus[SENDTO_NUM];			// �洢״̬	
}FILE_HEAD;
#pragma pack(pop)  									// �����������ݰ�, �ָ�ԭ���뷽ʽ

typedef enum
{
	 STATE_NODATA	= 0,							// û�����ݷ���
	 STATE_READY	= 1,							// �ȴ�����(��������ԭ���޷�����)
	 STATE_SENDING	= 2,							// ���ڷ���
}STATE_E;

// ����״̬
typedef struct _SEND_STATUS
{
	char state;										// ״̬
	char SendTimes;									// ���ʹ���, 255�κ��ۼ���
	uint32 NextSendTime;							// �´��ط���ʱ��(��)
	uint16 SendPacketID[3];							// ������η��͵İ�ID
	NET_COMMAND cmd;								// ������
	char data[SEND_RECORD_DATA_LEN];
	char size;										// �������ݳ���
}SEND_STATUS;

#define FILE_HEAD_LEN				(sizeof(FILE_HEAD))

#define get_limit_up(index)			(FILE_HEAD_LEN + index * SEND_RECORD_MAXLEN*SEND_RECORD_STOR_COUNT)
#define get_limit_down(index)		(FILE_HEAD_LEN + (index+1)* SEND_RECORD_MAXLEN*SEND_RECORD_STOR_COUNT-SEND_RECORD_MAXLEN)
 
static FILE_HEAD g_FileHead; 						// �ļ�ͷ
static FILE* g_file;								// �ļ�
static SEND_STATUS g_SendStatus[SENDTO_NUM];		// ����״̬,˳��ͬ�洢˳��
static char *g_FileBuf = NULL;
static char g_UpdateFile = FALSE;					// �Ƿ�д�ļ�

static uint32 send_event_data(int32 index);
void net_update_event_file(void);

/*************************************************
  Function:			get_sento_index
  Description: 		��ô洢����ֵ
  Input: 			��
  Output:			��
  Return:			�洢����ֵ
*************************************************/
static int32 get_sento_index(EVENT_SENDTO_E SendTo)
{
	switch (SendTo)
	{
		case EST_AURINE:
			return 0;
			
		case EST_SERVER:
			return 1;
			
		case EST_MANAGER:
			return 2;
			
		default:
			return -1; 
	}
}

/*************************************************
  Function:			get_address
  Description: 		���ָ������ֵ��IP��ַ
  Input: 			��
  Output:			��
  Return:			IP��ַ
*************************************************/
static uint32 get_address(int32 index)
{
	switch (index)
	{
		case 0:
			return storage_get_aurine_ip();
			
		case 1:
			return storage_get_center_ip();
			
		case 2:
			return storage_get_manager_ip(1);
			
		default:
			return 0; 
	}	
}

/*************************************************
  Function:    		read_unsend
  Description:		�����緢�Ͳ��ɹ���һ����¼
  Input: 
	1.index			λ������
	2.data			��¼�б�ָ��
  Output:			��
  Return:			ʵ�ʶ���¼��
  Others:
*************************************************/
static int32 read_unsend(int32 index, char *data)
{
	if (g_file == NULL)
	{	
		net_ini_unsend_file();
		if (g_file == NULL) 
		{
			return 0;
		}	
	}
	if (g_FileHead.StoreStatus[index].count < 1)
	{
		return 0;
	}
	memcpy(data, g_FileBuf+g_FileHead.StoreStatus[index].head, SEND_RECORD_MAXLEN);
	return 1;
}

/*************************************************
  Function:			write_unsend
  Description:		д���緢�Ͳ��ɹ��ļ�¼
  Input: 
	1.index			λ������
	2.data			����ָ��
  Output:			��
  Return:			�ɹ����
*************************************************/
static int32 write_unsend(int32 index, char *data)
{
	//DECLARE_CUP_SR;
	int32 pos;
	int32 up;
	int32 down;
	
	if (g_file == NULL)
	{
		net_ini_unsend_file();
	}
	if (g_file == NULL)
	{
		return FALSE;
	}
	
	//ENTER_CRITICAL();

	// ��λ
	pos = g_FileHead.StoreStatus[index].tail;
	memcpy(g_FileBuf+pos, data, SEND_RECORD_MAXLEN);
	
	up = get_limit_up(index);
	down = get_limit_down(index);
		
	// д�ļ�ͷ
	if (g_FileHead.StoreStatus[index].count>=SEND_RECORD_STOR_COUNT)
	{
		if (g_FileHead.StoreStatus[index].head==down)
		{
			g_FileHead.StoreStatus[index].head = up;
		}	
		else
		{
			g_FileHead.StoreStatus[index].head += SEND_RECORD_MAXLEN;
		}	
		g_FileHead.StoreStatus[index].tail	= g_FileHead.StoreStatus[index].head; 
		g_FileHead.StoreStatus[index].count = SEND_RECORD_STOR_COUNT;
	}
	else
	{	
		g_FileHead.StoreStatus[index].count++;
		if (g_FileHead.StoreStatus[index].tail==down)
		{
			g_FileHead.StoreStatus[index].tail = up; 
		}	
		else
		{
			g_FileHead.StoreStatus[index].tail += SEND_RECORD_MAXLEN;
		}	
	}
	//EXIT_CRITICAL();
	return TRUE;
}

/*************************************************
  Function:			del_unsend
  Description: 		ɾ�����緢�Ͳ��ɹ���һ����¼
  Input: 
	1.index			λ������
  Output:			��
  Return:			�ɹ����
*************************************************/
static int32 del_unsend(int32 index)
{
	//DECLARE_CUP_SR;
	int32 up;
	int32 nextup;
	int32 head;
	
	if (g_file == NULL)
	{
		net_ini_unsend_file();
	}	
	if (g_file == NULL)
	{
		return FALSE;
	}	
	up = get_limit_up(index);
	nextup = get_limit_up(index+1);
	
	//�����
	//ENTER_CRITICAL();  ���ж�
	if (1 == g_FileHead.StoreStatus[index].count)
	{
		g_FileHead.StoreStatus[index].head = up;
		g_FileHead.StoreStatus[index].tail = up;
		g_FileHead.StoreStatus[index].count = 0;
	}	
	else
	{
		g_FileHead.StoreStatus[index].count -= 1;
		head = g_FileHead.StoreStatus[index].head + SEND_RECORD_MAXLEN;
		if (nextup == head)
		{
			head = up;
		}
		g_FileHead.StoreStatus[index].head = head;
	}	
	//EXIT_CRITICAL();  ���ж�
	return TRUE;
}

/*************************************************
  Function:    		send_event_data
  Description:		���ļ��м������ݷ���
  Input: 			��
	1.index			λ������
  Output:			��
  Return:			��ʱʱ��, ��, 0Ϊ����
  Others:
*************************************************/
static uint32 send_event_data(int32 index)
{
	uint32 address;
	int32 id;
	uint32 n;
	int32 i;
	
	if (g_SendStatus[index].state == STATE_NODATA)
	{
		return 0;
	}
	address = get_address(index);	
	if (0 == address) 
	{
		return 0;
	}
	if (index == 2)
	{
		set_nethead(G_MANAGER_DEVNO, PRIRY_HIGHEST);
	}
	else
	{
		set_nethead(G_CENTER_DEVNO, PRIRY_HIGHEST);
	}
	id = net_direct_send(g_SendStatus[index].cmd, g_SendStatus[index].data, 
					g_SendStatus[index].size, address, NETCMD_UDP_PORT);
	g_SendStatus[index].state = STATE_SENDING;
	
	//�����´��ط�ʱ��
	switch(g_SendStatus[index].SendTimes)
	{
		case 0:
		case 1:
		case 2:
			n = 2;
			break;
		case 3:
		case 4:
		case 5:
			n = 6;
			break;
		case 6:
		case 7:
		case 8:
			n = 12;
			break;
		default:	
			n = 60;
			break;
	}

	g_SendStatus[index].NextSendTime = n;

	// �������3����ID
	i = g_SendStatus[index].SendTimes%3;
	g_SendStatus[index].SendPacketID[i] = id; 
	if (g_SendStatus[index].SendTimes < 10)
	{
		g_SendStatus[index].SendTimes++;
	}	
	return n;
}

/*************************************************
  Function:    		load_send_data
  Description:		���ļ��м������ݷ���
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
static void load_send_data(void)
{
	int32 i;
	uint32 n = 0;									// ��С��ʱʱ��
	uint32 t;
	char RecordBuf[SEND_RECORD_MAXLEN];
	
	for (i=0; i<SENDTO_NUM; i++)
	{
		// ��������
		if (g_FileHead.StoreStatus[i].count>0 && g_SendStatus[i].state==STATE_NODATA)
		{
			// ������
			if (read_unsend(i, RecordBuf) != 1) 
			{
				continue;
			}	
			g_SendStatus[i].cmd = ((RecordBuf[1]<<8)&0xFF00) | RecordBuf[2];
			g_SendStatus[i].size = RecordBuf[0] - SEND_RECORD_HEAD_LEN;
			memcpy(g_SendStatus[i].data, RecordBuf+SEND_RECORD_HEAD_LEN, g_SendStatus[i].size); 
			g_SendStatus[i].SendPacketID[0] = 0;
			g_SendStatus[i].SendPacketID[1] = 0;
			g_SendStatus[i].SendPacketID[2] = 0;
			g_SendStatus[i].SendTimes = 0; 
			g_SendStatus[i].state = STATE_READY;
			t = send_event_data(i);
			if (n==0)
			{
				n = t;
			}	
			else	
			{
				n = (t>0 && t<n)? t : n;
			}	
		}	
	}
	// ��ʼ��ʱ��
	if (n > 0)
	{
		
	}	
}

/*************************************************
  Function:			send_ok_proc
  Description: 		�����¼��ɹ�Ӧ������
  Input: 
	1.index			����ֵ
  Output:			��
  Return:			��
*************************************************/
static void send_ok_proc(int32 index)
{
	uint32 n;
	char RecordBuf[SEND_RECORD_MAXLEN];
	
	g_SendStatus[index].state = STATE_NODATA;
	del_unsend(index);
	
	// ���ط���
	if (g_FileHead.StoreStatus[index].count > 0)
	{
		// ��������
		if (read_unsend(index, RecordBuf) != 1)
		{
			return;
		}
		g_SendStatus[index].cmd = ((RecordBuf[1]<<8)&0xFF00) | RecordBuf[2];
		g_SendStatus[index].size = RecordBuf[0] - SEND_RECORD_HEAD_LEN;
		memcpy(g_SendStatus[index].data, RecordBuf+SEND_RECORD_HEAD_LEN, g_SendStatus[index].size); 
		g_SendStatus[index].SendPacketID[0] = 0;
		g_SendStatus[index].SendPacketID[1] = 0;
		g_SendStatus[index].SendPacketID[2] = 0;
		g_SendStatus[index].state = STATE_READY;
		n = send_event_data(index);
		if (n > 0)
		{
			
		}
	}
}

/*************************************************
  Function:			net_ini_unsend_file
  Description: 		����ʱ,��ʼ���¼��ļ�
  Input: 			��
  Output:			��
  Return:			��
*************************************************/
void net_ini_unsend_file(void)
{
	int32 i;
	FILE * fp;
	int32 head;
	//char RecordBuf[SEND_RECORD_MAXLEN]; 			// ��¼buf
	
	if (g_file != NULL)
	{
		return;
	}
		
	memset(g_SendStatus, 0, sizeof(g_SendStatus));
	for(i=0; i<SENDTO_NUM; i++)
	{
		g_SendStatus[i].state = STATE_NODATA;		//û�����ݷ���״̬
	}

	if (g_FileBuf == NULL)
	{
		g_FileBuf = malloc(sizeof(FILE_HEAD)+SEND_RECORD_STOR_COUNT*SENDTO_NUM);
	}
	if (g_FileBuf)
	{
		memset(g_FileBuf, 0, sizeof(FILE_HEAD)+SEND_RECORD_STOR_COUNT*SENDTO_NUM);
	}
		
	// ���ļ���Ϣ
	fp = fopen(FILE_NAME, "r");
	if (fp != NULL)
	{
		// ���ļ�ͷ
		fread(g_FileBuf, 1, sizeof(FILE_HEAD)+SEND_RECORD_STOR_COUNT*SENDTO_NUM, fp);
		memcpy(&g_FileHead, g_FileBuf, sizeof(FILE_HEAD));
		
		// �ж��ļ��Ƿ�Ϸ�
		if (g_FileHead.RecordCount==SEND_RECORD_STOR_COUNT &&
			g_FileHead.RecordLen==SEND_RECORD_MAXLEN &&
			g_FileHead.SendtoNum==SENDTO_NUM)
		{
			g_file = fp;
			load_send_data();
			return;									// �ļ���Ч
		}
		// ��Ч,��Ҫ��ʼ��
		fclose(fp);
	}
	
	fp = fopen(FILE_NAME,"w");
	if(fp == NULL)
	{
		return;
	}
	// д�ļ�ͷ
	memset(&g_FileHead, 0, sizeof(g_FileHead));
	g_FileHead.RecordCount = SEND_RECORD_STOR_COUNT;
	g_FileHead.RecordLen = SEND_RECORD_MAXLEN;
	g_FileHead.SendtoNum = SENDTO_NUM;
	memset(g_FileHead.reserve, 0xFF, sizeof(g_FileHead.reserve));
	memcpy(g_FileHead.reserve, "EVENT.DAT", 9);
	for (i=0; i<SENDTO_NUM; i++)
	{
		head = get_limit_up(i);
		g_FileHead.StoreStatus[i].head = head;
		g_FileHead.StoreStatus[i].tail = head;
		g_FileHead.StoreStatus[i].count = 0;
		memset(g_FileHead.StoreStatus[i].reserve, 0xFF, sizeof(g_FileHead.StoreStatus[i].reserve));
	}	
	memcpy(g_FileBuf, &g_FileHead, sizeof(FILE_HEAD));
	g_file = fp;
}

/*************************************************
  Function:    		net_close_unsend_file
  Description:		�ر�δ���ͳɹ��ļ�(�ڳ������ʱ�ر�)
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
void net_close_unsend_file(void)
{
	if (NULL != g_file)
	{
		net_update_event_file();
		fclose(g_file);
		g_file = NULL;
	}
}

/*************************************************
  Function:			net_send_event_from_file
  Description: 		�Ӵ洢���ļ��з���ָ�����ͷ����¼�
  Input: 
	1.cmd			����
	2.data			��������
	3.size			�������ݴ�С
	4.SendTo		���ͷ�, ����ƽ̨/���ķ�����/����Ա�� 
  Output:			��
  Return:			�Ƿ�ɹ�
*************************************************/
int32 net_send_event_from_file(NET_COMMAND cmd, char * data, int32 size, EVENT_SENDTO_E SendTo)
{
	int32 index;
	char EventBuf[SEND_RECORD_MAXLEN];
	
	// �ж����ݴ�С�Ƿ񳬹�����¼����, ��ȥ2������Ⱥ�һ���ֽڴ洢����
	if (size > SEND_RECORD_DATA_LEN)
	{
		return FALSE;
	}
	index = get_sento_index(SendTo);
	if (-1 == index)
	{
		return FALSE; 	
	}
	
	memset(EventBuf, 0, SEND_RECORD_MAXLEN);
	EventBuf[0] = size + SEND_RECORD_HEAD_LEN; 		// ���ݳ���
	EventBuf[1] = (cmd >> 8) & 0xFF;
	EventBuf[2] = cmd & 0xFF;
	memcpy(EventBuf+SEND_RECORD_HEAD_LEN, data, size);
	
	// �ȴ洢
	write_unsend(index, EventBuf);

	// ���ط���
	load_send_data();
	g_UpdateFile = TRUE;
	return TRUE;
}

/*************************************************
  Function:			net_check_is_report_event_echo
  Description: 		�жϸ�Ӧ����Ƿ��ϱ��¼���Ӧ����
  Input: 
	1.ID			Ӧ�������
  Output:			��
  Return:			�Ƿ�ɹ�
*************************************************/
int32 net_check_is_report_event_echo(NET_COMMAND cmd, uint16 ID)
{
	int32 i, j;
	if (ID == 0)
	{
		return FALSE;
	}
	
	for(i=0; i<SENDTO_NUM; i++)
	{ 
		if (cmd != g_SendStatus[i].cmd)
		{
			continue;
		}
		for (j=0; j<3; j++)							// ������������ID��
		{
			if (ID==g_SendStatus[i].SendPacketID[j])
			{
				send_ok_proc(i);
				return TRUE;
			}	
		}	
	}
	return FALSE;
}

/*************************************************
  Function:			net_update_event_file
  Description: 		��ʱ�����¼������ļ�
  Input: 			��
  Output:			��
  Return:			��
*************************************************/
void net_update_event_file(void)
{
	if (g_UpdateFile)
	{
		if (g_file)
		{
			fseek(g_file, 0, SEEK_SET);
			fwrite(g_FileBuf, 1, sizeof(FILE_HEAD)+SEND_RECORD_STOR_COUNT*SENDTO_NUM, g_file);
		}
	}
	g_UpdateFile = FALSE;
}

