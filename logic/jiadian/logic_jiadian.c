/*********************************************************
  Copyright (C), 2006-2010, Aurine
  File name:	logic_jiadian.c
  Author:   	txl
  Version:   	2.0
  Date: 		09.5.21
  Description:  �ҵ纯��
  History:                   
    1. Date:			 
       Author:				
       Modification:	
    2. ...
*********************************************************/
#include "logic_include.h"


#ifdef _JD_MODE_
static uint16 g_txnum = 0; 
static pthread_mutex_t g_SendBufCom;				// ���ͻ����ź���   
static pthread_mutex_t g_RecvBufCom;				// ���ջ����ź���
//static uint32  TimeId;								// ��ʱ��ID
static PJD_SEND_PACKET g_JDSendList = NULL;			// ���Ͷ���
static PJD_SEND_PACKET g_JDLoopSendList = NULL;		// ��ѯ���Ͷ���
static PJD_SEND_PACKET g_JDLoopLastList = NULL;		// ��ѯ����Ͷ���
static PJD_SEND_PACKET g_JDSendLastNode = NULL;		// ����������
static PJD_SEND_PACKET g_JDRecvList = NULL;			// ���ն���
static PJD_SEND_PACKET g_JDRecvLastNode = NULL;		// ����������
static uint8 g_InitFlag = 0;
static uint8 g_LoopBuf[7] = {0xAA, 0x82, 0x04, 0x00, 0x00, 0xFF, 0x00};

static uint8 LoopCnt = 0;
static PJD_DEV_LIST_INFO g_devlist = NULL;
static uint8 nextsendcmd = 0;

#ifdef _AURINE_ELEC_NEW_
JD_AURINE_DEV_UPDATE_STATE   g_jd_aurine_update_state = JD_AURINE_UPDATE_IDLE;

void jd_aurine_scene_open(uint8 scene_id,uint8 group_id,int index);
void jd_aurine_responsion(void);
void jd_aurine_dev_set_update_state(uint8 state);
void jd_aurine_net_distribute(char* data, int size);
static int jd_aurine_control_dev(uint16 cmd,uint8 dev,uint8 devindex,uint8 param1,uint8 param2);
static int jd_aurine_get_devinfo(uint8 dev,uint8 index,char* infodata);
static int jd_aurine_control_scene(uint8 sceneid,uint8 param);
static void jd_aurine_load_devinfo(AU_JD_DEV_TYPE devtype,AU_JD_STATE_LIST *statelist);
void jd_load_devinfo(void);

static AU_JD_STATE_LIST g_jd_state_dg_list[MAX_AURINE_JD_DG_NUM]={0};
static AU_JD_STATE_LIST g_jd_state_kt_list[MAX_AURINE_JD_KT_NUM]={0};
static AU_JD_STATE_LIST g_jd_state_cl_list[MAX_AURINE_JD_CL_NUM]={0};
static AU_JD_STATE_LIST g_jd_state_cz_list[MAX_AURINE_JD_CZ_NUM]={0};
static AU_JD_STATE_LIST g_jd_state_mq_list[MAX_AURINE_JD_MQ_NUM]={0};

static 	uint8 jd_aurine_buf[30] = {0xaa, 0x02, 0x09, 0, 0, 0x0c, JD_AU_ADDR_ID, JD_AURINE_SOURE_ADDR};

#define JD_AURINE_PACKET_LEN (jd_aurine_buf[2]+2)

static PAuJiaDianStateChanged  g_AuJiaDianStateChanged = NULL;
static PAuJiaDianStateChanged  g_AuKongtiaoStateChanged = NULL;
#endif

static JD_REPORT_CALLBACK JiaDianHook = NULL;
static uint8 g_loopsend = 0;

/*************************************************
  Function:		jiadian_tx_num
  Description:	�ҵ���ˮ
  Input: 		
  Output:		��
  Return:		��
  Others:		��
*************************************************/
static uint16 jiadian_tx_num(void)
{
	g_txnum++;
	return g_txnum;
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
	pthread_mutex_lock(&g_SendBufCom);
	//RKSemObtain(&g_SendBufCom);
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
	pthread_mutex_unlock(&g_SendBufCom);
	//RkSemRelease(&g_SendBufCom);
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
    //uint8 ret = 0;
    
	//ret = RockSemCreate(&g_SendBufCom, 1); 
	pthread_mutex_init(&g_SendBufCom, NULL);
}

/*************************************************
  Function:    		append_send_node
  Description: 		���뷢�ͽڵ�(����β��)
  Input: 
	1.node			����ڵ�
  Output:			��
  Return:			��
  Others:
*************************************************/
static void append_send_node(PJD_SEND_PACKET node)
{
	sendbuf_lock();
	if (g_JDSendList == NULL)
	{
		g_JDSendList = node;
		node->prev = NULL;
	}	
	else
	{
		g_JDSendLastNode->next = node;
		node->prev = g_JDSendLastNode;
	}
	g_JDSendLastNode = node;
	sendbuf_unlock();
}

/*************************************************
  Function:    		append_loop_send_node
  Description: 		������Ѳ���ͽڵ�(����β��)
  Input: 
	1.node			����ڵ�
  Output:			��
  Return:			��
  Others:
*************************************************/
static void append_loop_send_node(PJD_SEND_PACKET node)
{
	sendbuf_lock();
	if (g_JDLoopSendList == NULL)
	{
		g_JDLoopSendList = node;
		node->prev = NULL;
	}	
	else
	{
		g_JDLoopLastList->next = node;
		node->prev = g_JDLoopLastList;
	}
	g_JDLoopLastList = node;
	sendbuf_unlock();
}
/*************************************************
  Function:		add_send_node
  Description: 	��ӵ��������ݽ��
  Input:
  	1.type		�������� 1:Ϊ�������� 0:Ϊ��ѯ����
    2.data      ���� 
  	3.nlen		����
  Output:		��
  Return:		TRUE/FALSE
  Others:
*************************************************/
static uint32 add_send_node(JD_CMD_TYPE type, uint8 * data, uint8 nlen)
{
	PJD_SEND_PACKET node = NULL;	

	#if 0
	if (FALSE == storage_get_extmode(EXT_MODE_JD))
	{
		return FALSE;
	}
	#endif
	
	node = malloc(sizeof(JD_SEND_PACKET));
	if (NULL == node)
	{
		return FALSE;
	}

	node->SendState = SEND_STATE_READY;
	node->next = NULL;
	node->prev = NULL;			
	node->SendTimes = 0;
	node->size = nlen;
	#ifdef _AURINE_ELEC_NEW_
	if (storage_get_extmode(EXT_MODE_JD_FACTORY) == JD_FACTORY_ACBUS)
	{
	    node->SendMaxTimes = MAX_SEND_TIME;
	}
	else
	#endif
	{
	    node->SendMaxTimes = MAX_OLD_SEND_TIME;
	}
	
	
	if (MAX_DATA_LEN < nlen)
	{
		node->size = MAX_DATA_LEN;
	}
	memcpy(node->data, data, node->size);
	
	if (type == JD_CMD_COTROL)
	{
	    append_send_node(node);
	}
	else
	{
	    #ifdef _AURINE_ELEC_NEW_
	    if (storage_get_extmode(EXT_MODE_JD_FACTORY) == JD_FACTORY_ACBUS)
	    {
	        append_loop_send_node(node);            // ���ּҵ����ѯ�б�
	    }
	    #endif
	}

	return TRUE;
}

/*************************************************
  Function:		del_send_node
  Description: 	ɾ���������ݽ��
  Input: 		
  	1.data		����
  Output:		��
  Return:		��
  Others:
*************************************************/
static void del_send_node(void)
{
	PJD_SEND_PACKET node = NULL;
	
	sendbuf_lock();
	if (g_JDSendList != NULL)
	{
		node = g_JDSendList;
		if (NULL == node->next)
		{
			g_JDSendList = NULL;
			g_JDSendLastNode = NULL;
		}
		else
		{
			g_JDSendList = node->next;
		}

		if (node)
		{
			free(node);
			node = NULL;
		}
	}	
	sendbuf_unlock();
}

/*************************************************
  Function:		del_loop_send_node
  Description: 	ɾ����ѯ���ͽڵ�
  Input: 		
  	1.data		����
  Output:		��
  Return:		��
  Others:
*************************************************/
static void del_loop_send_node(void)
{
	PJD_SEND_PACKET node = NULL;
	
	sendbuf_lock();
	if (g_JDLoopSendList != NULL)
	{
		node = g_JDLoopSendList;
		if (NULL == node->next)
		{
			g_JDLoopSendList = NULL;
			g_JDLoopLastList = NULL;
		}
		else
		{
			g_JDLoopSendList = node->next;
		}

		if (node)
		{
			free(node);
			node = NULL;
		}
	}	
	sendbuf_unlock();
}

/*************************************************
  Function:		sendbuf_next
  Description: 	
  Input: 		
  	1.data		����
  Output:		��
  Return:		��
  Others:
*************************************************/
static void sendbuf_next(void)
{
    sendbuf_lock();
	if (g_JDSendList && SEND_STATE_WAIT == g_JDSendList->SendState)
    {
        g_JDSendList->SendState = SEND_STATE_OK;
    }
    else if (g_JDLoopSendList && SEND_STATE_WAIT == g_JDLoopSendList->SendState)
    {
        g_JDLoopSendList->SendTimes = LOOP_SEND_TIME;
    }
	sendbuf_unlock();
}

/*************************************************
  Function:    		init_loop_send_node
  Description: 		��ʼ����Ѳ���ͽڵ�
  Input: 
	1.data			����
	2.nlen			����
  Output:			��
  Return:			��
  Others:
*************************************************/
static void init_loop_send_node(char * data, uint8 nlen)
{
	g_JDLoopSendList = malloc(sizeof(JD_SEND_PACKET));
	if (NULL == g_JDLoopSendList)
	{
		return;
	}
	g_JDLoopSendList->next = NULL;
	g_JDLoopSendList->prev = NULL;			
	g_JDLoopSendList->SendTimes = 0;
	g_JDLoopSendList->size = nlen;
	if (MAX_DATA_LEN < nlen)
	{
		g_JDLoopSendList->size = MAX_DATA_LEN;
	}
	memcpy(g_JDLoopSendList->data, data, g_JDLoopSendList->size);
	g_JDLoopSendList->SendState = SEND_STATE_READY;
}

/*************************************************
  Function:    		recvbuf_lock
  Description: 		���ͻ���������
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
static void recvbuf_lock(void)
{
	//RKSemObtain(&g_RecvBufCom);
	pthread_mutex_lock(&g_RecvBufCom);
}

/*************************************************
  Function:    		recvbuf_unlock
  Description: 		���ͻ���������
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
static void recvbuf_unlock(void)
{
	//RkSemRelease(&g_RecvBufCom);
	pthread_mutex_unlock(&g_RecvBufCom);
}

/*************************************************
  Function:    		recvbuf_sem_init
  Description: 		�ź�����ʼ��
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
static void recvbuf_sem_init(void)
{
    //uint8 ret = 0;
	//ret = RockSemCreate(&g_RecvBufCom, 1);   	
	pthread_mutex_init(&g_RecvBufCom, NULL);
}

/*************************************************
  Function:    		append_recv_node
  Description: 		������սڵ�(����β��)
  Input: 
	1.node			����ڵ�
  Output:			��
  Return:			��
  Others:
*************************************************/
static void append_recv_node(PJD_SEND_PACKET node)
{
	recvbuf_lock();
	if (g_JDRecvList == NULL)
	{
		g_JDRecvList = node;
		node->prev = NULL;
	}	
	else
	{
		g_JDRecvLastNode->next = node;
		node->prev = g_JDRecvLastNode;
	}
	g_JDRecvLastNode = node;
	recvbuf_unlock();
}

/*************************************************
  Function:		add_recv_node
  Description: 	��ӵ��������ݽ��
  Input: 		
  	1.data		����
  Output:		��
  Return:		ECHO_VALUE 
  Others:
*************************************************/
static uint32 add_recv_node(uint8 *data, uint8 nlen)
{
	PJD_SEND_PACKET node = NULL;	

	node = malloc(sizeof(JD_SEND_PACKET));
	if (NULL == node)
	{
		return 0;
	}

	node->SendState = SEND_STATE_OK;
	node->next = NULL;
	node->prev = NULL;			
	node->SendTimes = 0;
	node->size = nlen;
	if (MAX_DATA_LEN < nlen)
	{
		node->size = MAX_DATA_LEN;
	}
	memcpy(node->data, data, node->size);
	append_recv_node(node);
}

/*************************************************
  Function:		del_recv_node
  Description: 	ɾ���������ݽ��
  Input: 		
  	1.data		����
  Output:		��
  Return:		��
  Others:
*************************************************/
static void del_recv_node(void)
{
	PJD_SEND_PACKET node = NULL;
	
	recvbuf_lock();
	if (g_JDRecvList != NULL)
	{
		node = g_JDRecvList;
		if (NULL == node->next)
		{
			g_JDRecvList = NULL;
			g_JDRecvLastNode = NULL;
		}
		else
		{
			g_JDRecvList = node->next;
		}

		if (node)
		{
			free(node);
			node = NULL;
		}
	}	
	recvbuf_unlock();
}

/*************************************************
  Function:		get_recv_node
  Description: 
  Input: 		
  	1.data		����
  Output:		��
  Return:		��
  Others:
*************************************************/
static int get_recv_node(uint8 * buf)
{
    int ret = 0;
	
	if (g_JDRecvList != NULL && g_JDRecvList->size > 0)
	{
		memcpy(buf, g_JDRecvList->data, g_JDRecvList->size);
		ret = g_JDRecvList->size;
	}	
	del_recv_node();

	return ret;
}

/*************************************************
  Function:		check_sum
  Description: 	���У���
  Input: 
	1.data		����ָ��
	2.len		����
  Output:		��
  Return:		0-��ȷ,����ֵʧ��
  Others:
*************************************************/
static int8 check_sum(uint8 *data, uint8 len)
{
	uint8 i, sum = 0;
	
	if (*data == 0xAA)
	{
		for (i = 1; i < len-1; i++)
		{
			sum += *(data+i);
		}

		sum += *(data+len-1);
		if (sum == 0)			// У�����ȷ
		{

			return 0;			
		}
	}
	return -1;
}

/*************************************************
  Function:		jiadian_distribute
  Description:	�ҵ��·������ģ��
  Input: 		
  	1.cmd		����
  	2.addr		�ڵ��ַ
  	3.param		���������Ĳ���
  Output:		��
  Return:		��
  Others:		��
*************************************************/
static void jiadian_distribute(uint8 cmd, uint16 addr, char *param)
{ 
	// �·������ʽ:
	// ����Ϊ���������ֽ�����(����������)
	// ͬ���� + �豸״̬/Ŀ���ַ + ���� + ��ˮ��(2�ֽ�) + ���� + �ڵ��ַ��λ + �ڵ��ַ��λ + ���������� + У���
	// buf[0]		buf[1]			buf[2]	buf[3] buf[4]	buf[5]	buf[6]			buf[7]		buf[8]
	uint8 buf[16] = {0xAA, JD, 0x06};
	uint8 nlen = 0;
	uint16 num = jiadian_tx_num();

	buf[3] = num >> 8;
	buf[4] = num & 0xff;
	switch (cmd)
	{
		case JD_ON:
			buf[5] = JD_ON;
			buf[6] = addr >> 8;
			buf[7] = addr & 0xff;
			if (param)
			{
				buf[2] = 7;
				buf[8] = param[0];
				nlen = 9;
			}
			else
			{
				nlen = 8;
			}
			break;

		case JD_OFF:
			buf[5] = JD_OFF;
			buf[6] = addr >> 8;
			buf[7] = addr & 0xff;
			nlen = 8;
			break;

		case JD_ADD:
			buf[2] = 7;
			buf[5] = JD_ADD;
			buf[6] = addr >> 8;
			buf[7] = addr & 0xff;
			buf[8] = param[0];
			nlen = 9;
			break;

		case JD_DEC:
			buf[2] = 7;
			buf[5] = JD_DEC;
			buf[6] = addr >> 8;
			buf[7] = addr & 0xff;
			buf[8] = param[0];
			nlen = 9;
			break;

		case JD_GET_LOGO_NUM:						// ȡ��Ʒ������
			buf[5] = JD_GET_LOGO_NUM;
			nlen = 6;
			break;

		case JD_GET_LOGO:							// ��ȡƷ������
			buf[2] = 7;
			buf[5] = JD_GET_LOGO;
			//buf[8] = param[0];					// Ʒ��������
			buf[8] = param[0];						// Ʒ��������(����Э�����Ʒ�ƺŴ�1��ʼ,ԭ����0��ʼ)
			nlen = 9;
			break;

		case JD_GET_VER:
			buf[5] = JD_GET_VER;
			nlen = 6;
			break;

		case JD_SET_LOGO:							// �趨ѡ�õ�Ʒ��
			buf[2] = 7;
			buf[5] = JD_SET_LOGO;
			//buf[8] = param[0]
			buf[8] = param[0];        				// Ʒ��������
			nlen = 9;
			break;

		default:
			return;
	}
	
	add_send_node(JD_CMD_COTROL, buf, (nlen+1));
}

/*************************************************
  Function:		jiadian_485_responsion
  Description: 	�ҵ�ģ��485����Ӧ�������
  Input: 
	1.mode		�龰ģʽ
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jiadian_485_responsion(uint8 * rcvbuf)
{
    uint8 ret = 0;

	#if 0
	uint8 i;
	printf("len = %d\n", len);
	for (i = 0; i < len; i++)
	{
		printf("  %02x", buf[i]);
	}
	printf("\n===================\n");
	#endif
		
    #ifdef _AURINE_ELEC_NEW_
    sendbuf_next();
    //ret = check_sum(rcvbuf, rcvbuf[2]+3);
    if (rcvbuf[9] == JD_AU_CMD_UP_STATE_REPORT || 
       (rcvbuf[9] == JD_AU_CMD_DW_CTRL_IR_REPORT && 
       rcvbuf[10] == JD_AU_IR_REPORT_MODE_LEARN))
    {
        add_recv_node(rcvbuf, (rcvbuf[2]+3));
    }
	#endif
}

/*************************************************
  Function:		device_all
  Description: 	����ͬһ�����豸
  Input: 
	1.devtype	�豸����
	2.cmd		����
  Output:		��
  Return:		��
  Others:		��
*************************************************/
static void operate_all(uint8 cmd, AU_JD_DEV_TYPE devtype)
{
	switch (devtype)
	{
		case JD_DEV_LIGHT:							// �ƹ��豸
			jiadian_distribute(cmd, 0xf0, NULL);
			jiadian_distribute(cmd, 0xf1, NULL);
			jiadian_distribute(cmd, 0xf2, NULL);
			break;

		case JD_DEV_KONGTIAO:						// �յ��豸
			jiadian_distribute(cmd, 0xf3, NULL);
			break;

		case JD_DEV_WINDOW:							// �����豸
			jiadian_distribute(cmd, 0xf4, NULL);
			jiadian_distribute(cmd, 0xf5, NULL);
			break;

		case JD_DEV_POWER:							// ��Դ�豸
			jiadian_distribute(cmd, 0xf6, NULL);
			jiadian_distribute(cmd, 0xf7, NULL);
			jiadian_distribute(cmd, 0xf8, NULL);
			break;

		case JD_DEV_GAS:							// ú���豸
			jiadian_distribute(cmd, 0xf9, NULL);
			break;

		default:									// �����豸
			jiadian_distribute(cmd, 0xfa, NULL);
			jiadian_distribute(cmd, 0xfb, NULL);
			jiadian_distribute(cmd, 0xfc, NULL);
			jiadian_distribute(cmd, 0xfd, NULL);
			jiadian_distribute(cmd, 0xfe, NULL);
			jiadian_distribute(cmd, 0xff, NULL);
			break;
	}
}

/*************************************************
  Function:		device_on
  Description: 	�豸��
  Input: 
	1.addr		�豸��ַ
	2.val		ֵ
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void device_on(uint16 addr, uint8 val)
{
	dprintf("device_on %d %d\n",addr,val);
	jiadian_distribute(JD_ON, addr, &val);
}

/*************************************************
  Function:		device_off
  Description: 	�豸��
  Input: 
	1.addr		�豸��ַ
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void device_off(uint16 addr)
{
	dprintf("device_off %d\n",addr);
	jiadian_distribute(JD_OFF, addr, NULL);
}

/*************************************************
  Function:		device_on_all
  Description: 	�豸ȫ��
  Input: 
	1.addr		�豸����
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void device_on_all(AU_JD_DEV_TYPE devtype)
{
	dprintf("device_on_all %d\n",devtype);
	operate_all(JD_ON, devtype);
}

/*************************************************
  Function:		device_off_all
  Description: 	�豸ȫ��
  Input: 
	1.addr		�豸��ַ
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void device_off_all(AU_JD_DEV_TYPE devtype)
{
	dprintf("device_off_all %d\n",devtype);
	operate_all(JD_OFF, devtype);
}

/*************************************************
  Function:		device_value_add
  Description: 	�豸ֵ����
  Input: 
	1.addr		�豸��ַ
	2.val		ֵ
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void device_value_add(uint16 addr, char *val)
{
	dprintf("device_value_add %d %d\n",addr,*val);
	jiadian_distribute(JD_ADD, addr, val);
}

/*************************************************
  Function:		device_value_dec
  Description: 	�豸ֵ����
  Input: 
	1.addr		�豸��ַ
	2.val		ֵ
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void device_value_dec(uint16 addr, char *val)
{
	dprintf("device_value_dec %d %d\n",addr,*val);	
	jiadian_distribute(JD_DEC, addr, val);
}

/*************************************************
  Function:		exec_jd_scene_mode
  Description: 	ִ�мҵ��龰ģʽ
  Input: 
	1.mode		�龰ģʽ
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void exec_jd_scene_mode(AU_JD_SCENE_MODE mode)
{
	uint8 i;
	char buf[16] = {0xAA, JD, 07};
	uint8 nlen = 0;
	uint16 num;
	
	#ifdef _AURINE_ELEC_NEW_
	if (JD_FACTORY_ACBUS == storage_get_extmode(EXT_MODE_JD_FACTORY))
	{
		dprintf("mode...........: %d\n",mode);
		switch(mode)
		{
		    //���ģʽ
		    case JD_SCENE_RECEPTION:
		        jd_aurine_scene_open(1,1, mode+1);
		        break;

		    case JD_SCENE_MEALS:
		        jd_aurine_scene_open(2,2, mode+1);
		        break;
			//�ڵ�ģʽ
			case JD_SCENE_POWERSAVING:
				jd_aurine_scene_open(4,4, mode+1);
				break;
			//ҹ��ģʽ
			case JD_SCENE_NIGHT:
				jd_aurine_scene_open(3,3, mode+1);
				break;
			//��ͨģʽ
			case JD_SCENE_GENERAL:
				jd_aurine_scene_open(5,5, mode+1);
				break;
		}
		return;
	}
	else
	{
	    PJD_DEV_LIST_INFO jdlist = storage_get_scene();
    	dprintf("exec_jd_scene_mode %d\n",mode);
		dprintf("jdlist->nCount %d\n",jdlist->nCount);
    	if (jdlist)
    	{
    		for (i = 0; i < jdlist->nCount; i++)
    		{
    			if (jdlist->pjd_dev_info[i].IsUsed)
    			{
    				dprintf("jdlist->pjd_dev_info[%d] %x,%d,%d\n",i,jdlist->pjd_dev_info[i].Address,
    				jdlist->pjd_dev_info[i].JdType,jdlist->pjd_dev_info[i].Commond[mode-JD_SCENE_RECEPTION]);		
    				if (JD_STATE_CLOSE == jdlist->pjd_dev_info[i].Commond[mode-JD_SCENE_RECEPTION])
    				{
    					buf[5] = JD_OFF;				// ����
    				}
    				else
    				{
    					buf[5] = JD_ON;					// ����					
    				} 
    				
    				num = jiadian_tx_num();
    				buf[3] = num >> 8;
    				buf[4] = num & 0xff;
    				buf[6] = jdlist->pjd_dev_info[i].Address >> 8;		// �豸��ַ��λ
    				buf[7] = jdlist->pjd_dev_info[i].Address & 0xff;	// �豸��ַ��λ
    				buf[8] = jdlist->pjd_dev_info[i].param[mode-JD_SCENE_RECEPTION][0];// ����
    				nlen = 10;
    				add_send_node(JD_CMD_COTROL, buf, nlen);
    			}
    		}
    		free_jd_memory(&jdlist);
    	}
	}
	#else // _IS_USE_AURINE_ELEC_NEW
    PJD_DEV_LIST_INFO jdlist = storage_get_scene();
	dprintf("exec_jd_scene_mode %d\n",mode);
	if (jdlist)
	{
		for (i = 0; i < jdlist->nCount; i++)
		{
			if (jdlist->pjd_dev_info[i].IsUsed)
			{
				dprintf("jdlist->pjd_dev_info[%d] %x,%d,%d\n",i,jdlist->pjd_dev_info[i].Address,
				jdlist->pjd_dev_info[i].JdType,jdlist->pjd_dev_info[i].Commond[mode-JD_SCENE_RECEPTION]);		
				if (JD_STATE_CLOSE == jdlist->pjd_dev_info[i].Commond[mode-JD_SCENE_RECEPTION])
				{
					buf[5] = JD_OFF;				// ����
				}
				else
				{
					buf[5] = JD_ON;					// ����					
				} 
				
				num = jiadian_tx_num();
				buf[3] = num >> 8;
				buf[4] = num & 0xff;
				buf[6] = jdlist->pjd_dev_info[i].Address >> 8;		// �豸��ַ��λ
				buf[7] = jdlist->pjd_dev_info[i].Address & 0xff;	// �豸��ַ��λ
				buf[8] = jdlist->pjd_dev_info[i].param[mode-JD_SCENE_RECEPTION][0];// ����
				nlen = 10;
				add_send_node(JD_CMD_COTROL, buf, nlen);
			}
		}
		free_jd_memory(&jdlist);
	}
	#endif
}

/*************************************************
  Function:		set_device_logo
  Description: 	���õ�ǰʹ���豸�̱�
  Input: 
	1.val		ֵ
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void set_device_logo(uint8 val)
{
	dprintf("set_device_logo %d\n",val);
	jiadian_distribute(JD_SET_LOGO, 0, &val);
}

/*************************************************
  Function:		jd_loop_thread
  Description: 	�ҵ��շ������߳�
  Input: 		��	
  Output:		��
  Return:		��
  Others:
*************************************************/
static void jd_loop_thread(void * param)
{   
	while (1)
	{
		// ����GUI���Ź�
		//hw_monitor_dog();
		
		// ���������
	    if (g_JDSendList && (nextsendcmd == 0 || nextsendcmd == 1))
		{    
			if (SEND_STATE_READY == g_JDSendList->SendState)
			{   
			    g_JDSendList->SendState = SEND_STATE_WAIT;
				send_485_pack(g_JDSendList->data, g_JDSendList->size);	
				nextsendcmd = 1;
			}
			else
			{
				if (SEND_STATE_WAIT == g_JDSendList->SendState)
				{
					g_JDSendList->SendTimes++;
					if (g_JDSendList->SendMaxTimes == g_JDSendList->SendTimes)
					{
						del_send_node();
						nextsendcmd = 0;
					}
				}
				else
				{
					del_send_node();
					nextsendcmd = 0;
				}
			}
	    }
	    else if (g_JDLoopSendList && (nextsendcmd == 0 || nextsendcmd == 2))
		{
			// del by luofl 2015-10-31 �ҵ�ģ�鲻����ҲҪ��Ѳι��
			#if 0
			if (FALSE == storage_get_extmode(EXT_MODE_JD))
			{
				nextsendcmd = 0;
				return;
			}
			#endif
			
			// del by luofl 2015-10-22 ���ʼҵ�Ҳ������Ѳ
			#if 0
			else if ((JD_FACTORY_ACBUS != storage_get_extmode(EXT_MODE_JD_FACTORY)))
			{
				nextsendcmd = 0;
				return;
			}
			#endif
			
			if (SEND_STATE_READY == g_JDLoopSendList->SendState)
			{ 
			    g_JDLoopSendList->SendState = SEND_STATE_WAIT;
				send_485_pack(g_JDLoopSendList->data, g_JDLoopSendList->size);
				nextsendcmd = 2;
			}
			else
			{
				g_JDLoopSendList->SendTimes++;
				if (MAX_SEND_TIME == g_JDLoopSendList->SendTimes)
				{
					g_JDLoopSendList->SendTimes = 0;
					g_JDLoopSendList->SendState = SEND_STATE_READY;
					nextsendcmd = 0;
				}
			}
		}

		// ���������
		if (g_JDRecvList)
		{	
			jd_aurine_responsion();
		}

		if ((JD_FACTORY_ACBUS == storage_get_extmode(EXT_MODE_JD_FACTORY)))
		{
			usleep(10*1000);
		}
		else
		{
			usleep(50*1000);
		}
	}
}

#if 0
/*************************************************
  Function:		CommTimeBack
  Description: 	�շ���ʱ��
  Input: 		��	
  Output:		��
  Return:		��
  Others:
*************************************************/
static void CommTimeBack(void * param)
{ 
    if (g_JDSendList)
	{
		if (SEND_STATE_READY == g_JDSendList->SendState)
		{
			g_JDSendList->SendState = SEND_STATE_WAIT;
			send_485_pack(g_JDSendList->data, g_JDSendList->size);	
		}
		else
		{
			if (SEND_STATE_WAIT == g_JDSendList->SendState)
			{
				g_JDSendList->SendTimes++;
				if (MAX_SEND_NUM == g_JDSendList->SendTimes)
				{
					del_send_node();		
				}
			}
			else
			{
				del_send_node();	
			}
		}
	}
}
#endif

//#ifdef _AURINE_REG_
/*************************************************
  Function:		web_kz_jd_dev
  Description:	ִ�й���ƽ̨���͵ļҵ��豸��������
  Input: 	
  	1.cmd		ִ�еĶ�������/��
  	2.devtype 	�ҵ��豸����
  	3.index   	�ҵ��豸�����ŵ�=0xffʱ��ִ�д������豸ȫ��/�ض���
  Output:		��
  Return:		�ɹ����,true/fasle
  Others:
*************************************************/
static int web_kz_jd_dev(uint16 cmd, uint8 dev,uint8 index)
{
    uint8 devtype_id[5] = {JD_DEV_LIGHT, JD_DEV_KONGTIAO, JD_DEV_WINDOW, JD_DEV_POWER, JD_DEV_GAS};
	uint8 devtype = devtype_id[dev];
	
	int ret = FALSE;
	int i = 0;
  	
	dprintf("jd oper cmd %x devtype %x index %d\n",cmd,devtype,index);
	if (index == 0xff)
	{
	 	 switch(cmd)
	 	 {
	 	 	case CMD_JD_DEV_OPEN:
	 	 		device_on_all((AU_JD_DEV_TYPE)devtype);
				ret = TRUE;
	 	 		break;
				
	 	 	case CMD_JD_DEV_CLOSE:
	 	 		device_off_all((AU_JD_DEV_TYPE)devtype);
				ret = TRUE;
	 	 		break;
				
	 	 	default:
	 	 		break;
	 	 }
	 }
	 else
	 {
		PJD_DEV_LIST_INFO pdev = storage_get_jddev(devtype, FALSE);
	 	if (pdev) 
	 	{
	 		for(i = 0; i < pdev->nCount; i++)
	 		{
	 			if (pdev->pjd_dev_info[i].Index == (index-1) && pdev->pjd_dev_info[i].IsUsed)
	 			{
	 				switch(cmd)
		 			{
			 			case CMD_JD_DEV_OPEN:
			 				device_on(pdev->pjd_dev_info[i].Address,0xff);
							ret = TRUE;
			 				break;
							
			 			case CMD_JD_DEV_CLOSE:
			 				device_off(pdev->pjd_dev_info[i].Address);
							ret = TRUE;
			 				break;
							
			 			default:
			 				break;
		 			}
                    dprintf("pdev->pjd_dev_info[i].Address = %x\n",pdev->pjd_dev_info[i].Address);
					if (ret == TRUE)
					{
						break;
					}
	 			}
	 		}

			free_jd_memory(&pdev);
	 	}	
	 	else 
		{
			ret = FALSE;
	 	}
	 }	
	 return ret;	
}

/*************************************************
  Function:			jd_distribute
  Description:		�ҵ���շ��Ͱ��ص�����
  Input: 	
  	1.recPacket		���հ�
  Output:			��
  Return:			�ɹ����,true/fasle
  Others:
*************************************************/
int32 jd_net_distribute(const PRECIVE_PACKET recPacket)
{
	int ret = TRUE;
	uint8 jdType,jdIndex,jdParam,macOk, dev_param1, dev_param2;
	uint8 sMac[20]; 
	uint8 cexecutor = 0;
	uint8 localMac[30];  
	char* data_echo = NULL;
	int echo_size;
	PNET_HEAD head = (PNET_HEAD)(recPacket->data + MAIN_NET_HEAD_SIZE);
	int cmd = head->command | SSC_EHOME<<8;
	char * data = recPacket->data+NET_HEAD_SIZE;

	cexecutor = *(uint8*)data;
	memcpy(sMac,data+1,6);
	jdType = *(uint8*)(data+1+6);
	jdIndex =*(uint8*)(data+1+6+1); 
	jdParam =*(uint8*)(data+1+6+1+1);  
	macOk =0;

#ifdef _AURINE_ELEC_NEW_
	dev_param1 = *(uint8*)(data+1+6+1+1);
	dev_param2 = *(uint8*)(data+1+6+1+1+1);
	data_echo = malloc(1000);
#endif

	memcpy(localMac, storage_get_mac(HOUSE_MAC), 6);
	if ( localMac[0]==sMac[0] && localMac[1]==sMac[1] && localMac[2]==sMac[2]
		    && localMac[3]==sMac[3] && localMac[4]==sMac[4] && localMac[5]==sMac[5] )
    {
    	macOk = 1;
	}
    if (cexecutor != EXECUTOR_SHORT_MSG)
    {
    	if ( ( (head->DeviceType == DEVICE_TYPE_AURINE_SERVER_COMM  || head->DeviceType == DEVICE_TYPE_AURINE_SERVER_WEB ||
    		head->DeviceType == DEVICE_TYPE_AURINE_SERVER_SERVICE ) && !macOk ) || cexecutor != EXECUTOR_IE_USER )
    	{
            return 0;
    	}
	}
	
	switch (cmd)
	{
		case CMD_JD_DEV_OPEN:	  					// �ҵ��豸����
		case CMD_JD_DEV_CLOSE:    					// �ҵ��豸��			
		{
			net_send_echo_packet(recPacket, ECHO_OK, NULL, 0);	
			
			#ifdef _AURINE_ELEC_NEW_
			memcpy(&jdType,data+1+6,1);
			memcpy(&jdIndex,data+1+6+1,1);
			memcpy(&dev_param1,data+1+6+2,1);
			memcpy(&dev_param2,data+1+6+3,1);
			
			if(JD_FACTORY_ACBUS == storage_get_extmode(EXT_MODE_JD_FACTORY))
			{
			    if (jdType == JD_DEV_KONGTIAO-1)
			    {
			        if (recPacket->size == 34)         // ֻ��һ����������ԭ����Э�� ��Ϊ0��������Э��0ʱΪ����ģʽ
    			    {
    			        dev_param1 = 255;           
    			    }
    			}
				jd_aurine_control_dev(cmd,jdType,jdIndex,dev_param1,dev_param2);
			}
			else
			{
				ret = web_kz_jd_dev(cmd, jdType, jdIndex);
			}
    		#else
    			ret = web_kz_jd_dev(cmd, jdType, jdIndex);
    		#endif	
			
			break;
		}

		case CMD_JD_SCENE_MODE:
			net_send_echo_packet(recPacket, ECHO_OK, NULL, 0);
			#ifdef _AURINE_ELEC_NEW_
			memcpy(&jdType,data+1+6,1);
			memcpy(&jdIndex,data+1+6+1,1);
			if (JD_FACTORY_ACBUS == storage_get_extmode(EXT_MODE_JD_FACTORY))
			{
				jd_aurine_control_scene(jdType-0x80,jdIndex);
			}
			else
			{
				exec_jd_scene_mode(jdType-0x80);
			}
			#else
			exec_jd_scene_mode(jdType-0x80);
			#endif
			ret = TRUE;
			break;
			
        #ifdef _AURINE_ELEC_NEW_
		case  CMD_JD_STATE_QUERY:
			echo_size = jd_aurine_get_devinfo(jdType,jdIndex,data_echo);
			net_send_echo_packet(recPacket, ECHO_OK, data_echo, echo_size);
			if (data_echo)
			{
				free(data_echo);
				data_echo = NULL;
			}
			ret = TRUE;
			break;
	    #endif
	    
		default:
			 ret= FALSE;
			 break;	
	}

	return ret;
}

/*************************************************
  Function:			jd_responsion
  Description:		�ҵ�ģ�����Ӧ���������
  Input: 	
  	1.recPacket		Ӧ���
  	2.SendPacket	���Ͱ�
  Output:			��
  Return:			��
  Others:
*************************************************/
void jd_net_responsion(const PRECIVE_PACKET recPacket, const PSEND_PACKET SendPacket)
{
	PNET_HEAD head = (PNET_HEAD)(recPacket->data + MAIN_NET_HEAD_SIZE);
	int cmd = head->command | SSC_EHOME<<8;
	char * data = recPacket->data+NET_HEAD_SIZE;
	int size = (recPacket->size-NET_HEAD_SIZE);
	
	switch (cmd)
	{
		case CMD_JD_GET_JD_INFO:
			#ifdef _AURINE_ELEC_NEW_
			if (ECHO_OK == head->EchoValue)
			{
				jd_aurine_net_distribute(data, size);
			}
			#endif
			break;
	}
	return;
}
//#endif

#ifdef _AURINE_ELEC_NEW_
/*************************************************
  Function:		jd_aurine_net_distribute
  Description:	��ѯ���ּҵ��豸״̬Ӧ����
  Input: 		
  	2.recvbuf		Ӧ������
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_net_distribute(char* data, int size)
{
	int j,i;
	uint32 Devtype = 0;
	uint32 Count = size;
	uint32 Num = 0;
	AU_JDDEV_STATE JdDev[MAX_JD_LIGHT];
	uint32 newActionParam = 0;
	uint8 g_Devtype = 0;
	if (NULL == data && size < 8)
	{
		return;
	}

	memcpy(&g_Devtype, data, 4);
	memcpy(&Num, data+4, 4);
	Devtype = g_Devtype;
	
	switch (g_Devtype)
	{
		case JD_DEV_WINDOW:
			Devtype = 2;
			break;

		case JD_DEV_KONGTIAO:
			Devtype = 1;
			break;

		default:
			break;
	}
	
	Count -= 8;

	Num = Count/sizeof(AU_JDDEV_STATE);

	dprintf("jd_aurine_net_distribute size %d Devtype %d Num %d\n",size,Devtype,Num);
	
	if (Num == 0)
	{
		return;
	}
	memset(JdDev, 0, sizeof(AU_JDDEV_STATE)*MAX_JD_LIGHT);
	memcpy(JdDev, data+8, Num*sizeof(AU_JDDEV_STATE));

	for (i = 0; i < Num; i++)
	{
		switch (Devtype)
		{
			case JD_DEV_LIGHT:
				for (j = 0; j < MAX_AURINE_JD_DG_NUM; j++)
				{
					if (g_jd_state_dg_list[j].Index == (JdDev[i].Index-1) && g_jd_state_dg_list[j].IsUsed)
					{
						if (JdDev[i].State != 0)
						{
						    newActionParam = JdDev[i].State;
						}
						else
						{
						    newActionParam = 0;
						}
						if (g_AuJiaDianStateChanged)
						{
						    g_AuJiaDianStateChanged(JD_DEV_LIGHT, j, newActionParam);
						}
						
						g_jd_state_dg_list[j].Dev_State.ActiveParam= newActionParam;		
					}
				}
				break;

			case JD_DEV_KONGTIAO:
				for (j = 0; j < MAX_AURINE_JD_KT_NUM; j++)
				{
					if (g_jd_state_kt_list[j].Index == (JdDev[i].Index-1) && g_jd_state_kt_list[j].IsUsed)
					{
						if (g_AuJiaDianStateChanged)
						{
						    g_AuJiaDianStateChanged(JD_DEV_KONGTIAO, j, JdDev[i].State);
						}
						g_jd_state_kt_list[j].Dev_State.ActiveParam = JdDev[i].State;
					}
				}
				break;
				
			case JD_DEV_WINDOW:
				for (j = 0; j < MAX_AURINE_JD_CL_NUM; j++)
				{
					if (g_jd_state_cl_list[j].Index == (JdDev[i].Index-1) && g_jd_state_cl_list[j].IsUsed)
					{
						if (g_AuJiaDianStateChanged)
						{
						    g_AuJiaDianStateChanged(JD_DEV_WINDOW, j, JdDev[i].State);
						}
						g_jd_state_cl_list[j].Dev_State.ActiveParam = JdDev[i].State;
					}
				}
				break;

				
			case JD_DEV_POWER:
				for (j = 0; j < MAX_AURINE_JD_CZ_NUM; j++)
				{
					if (g_jd_state_cz_list[j].Index == (JdDev[i].Index-1) && g_jd_state_cz_list[j].IsUsed)
					{
						if (g_AuJiaDianStateChanged)
						{
							g_AuJiaDianStateChanged(JD_DEV_POWER, j, JdDev[i].State);
						}
						g_jd_state_cz_list[j].Dev_State.ActiveParam = JdDev[i].State;
					}
				}
				break;
			
			case JD_DEV_GAS:
				for (j = 0; j < MAX_AURINE_JD_MQ_NUM; j++)
				{
					if (g_jd_state_mq_list[j].Index == (JdDev[i].Index-1) && g_jd_state_mq_list[j].IsUsed)
					{
						if(g_AuJiaDianStateChanged)
						{
						    g_AuJiaDianStateChanged(JD_DEV_GAS, j, JdDev[i].State);
						}
						g_jd_state_mq_list[j].Dev_State.ActiveParam = JdDev[i].State;	
					}
				}
				break;
				
			default:
				break;
		}
	}
}

/*************************************************
  Function:		jd_aurine_485_distribute
  Description:	��ѯ���ּҵ��豸״̬Ӧ����
  Input: 		
  	2.recvbuf		Ӧ������
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_responsion(void)
{
    uint8 recvbuf[51];
    char data[30];
	uint8 i, j, x;
	uint8 flag = 0;
	uint8 len;
	uint8 dev_num = 0;
	uint8 dev_addr = 0;
	uint8 dev_state = 0;
	uint32 newActionParam = 0;
    
    len = get_recv_node(recvbuf);
    if (0 == len)
    {
        return;
    }

	#if 0
	printf("len = %d\n", len);
	for (i = 0; i < len; i++)
	{
		printf("  %02x", recvbuf[i]);
	}
	printf("\n===================\n");
	#endif
    
	if (recvbuf[9] == JD_AU_CMD_UP_STATE_REPORT)
	{
		dev_num = recvbuf[10];
		dprintf("-------%d-----------\n",dev_num);
		for (i = 0; i < dev_num; i++)
		{
			x = 11+(i*2);
			dev_addr = recvbuf[x];
			dev_state = recvbuf[x+1];
			dprintf("-------%d-----%d------\n",dev_addr,dev_state);
			
			for (j = 0; j < MAX_AURINE_JD_DG_NUM; j++)
			{
				if (g_jd_state_dg_list[j].Address == dev_addr && g_jd_state_dg_list[j].IsUsed)
				{
					if (dev_state!=0)
					{
					    newActionParam = ((dev_state+1)*100)/32;
					}
					else
					{
					    newActionParam = 0;
					}

					if (g_AuJiaDianStateChanged)
					{
					    g_AuJiaDianStateChanged(JD_DEV_LIGHT, j,newActionParam);
					}
					
					g_jd_state_dg_list[j].Dev_State.ActiveParam= newActionParam;		
				}
			}
			
			for (j = 0; j < MAX_AURINE_JD_CL_NUM; j++)
			{
				if (g_jd_state_cl_list[j].Address == dev_addr && g_jd_state_cl_list[j].IsUsed)
				{
					dprintf("---MAX_AURINE_JD_CL_NUM---%d------%d----%d----\n",newActionParam,dev_state,j);
					//if (dev_state !=g_jd_state_cl_list[j].Dev_State.ActiveParam )
					{
						if (g_AuJiaDianStateChanged)
						{
						    g_AuJiaDianStateChanged(JD_DEV_WINDOW, j, dev_state);
						}
						g_jd_state_cl_list[j].Dev_State.ActiveParam = dev_state;
					}	
				}
			}
			
			for (j = 0; j < MAX_AURINE_JD_CZ_NUM; j++)
			{
				if (g_jd_state_cz_list[j].Address == dev_addr && g_jd_state_cz_list[j].IsUsed)
				{
					dprintf("---MAX_AURINE_JD_CZ_NUM---%d------%d----%d----\n",newActionParam,dev_state,j);
					//if ( dev_state !=g_jd_state_cz_list[j].Dev_State.ActiveParam )
					{
						if (g_AuJiaDianStateChanged)
						{
							g_AuJiaDianStateChanged(JD_DEV_POWER, j, dev_state);
						}
						g_jd_state_cz_list[j].Dev_State.ActiveParam = dev_state;
					}	
				}
			}
			
			for (j = 0; j < MAX_AURINE_JD_MQ_NUM; j++)
			{
				if (g_jd_state_mq_list[j].Address == dev_addr && g_jd_state_mq_list[j].IsUsed)
				{
					dprintf("---MAX_AURINE_JD_MQ_NUM---%d------%d----%d----\n",newActionParam,dev_state,j);
					//if (dev_state !=g_jd_state_mq_list[j].Dev_State.ActiveParam )
					{
						if(g_AuJiaDianStateChanged)
						{
						    g_AuJiaDianStateChanged(JD_DEV_GAS, j, dev_state);
						}
						g_jd_state_mq_list[j].Dev_State.ActiveParam = dev_state;
					}	
				}
			}
		}
	}
	
	if (recvbuf[9] == JD_AU_CMD_DW_CTRL_IR_REPORT && recvbuf[10] == JD_AU_IR_REPORT_MODE_LEARN)
	{
		uint8 report_value=0;
		uint8 address = 0;
		uint8 learn_id = 0;
		learn_id = recvbuf[12];
		address = recvbuf[13];
		report_value = recvbuf[14];
		if (g_AuKongtiaoStateChanged)
		{
		    g_AuKongtiaoStateChanged(JD_DEV_KONGTIAO, learn_id, report_value);
		}
	}
}

/*************************************************
  Function:		jd_aurine_light_ctrlmode
  Description:	���ּҵ�ƹ���������
  Input: 		
  	1.mode		�ƹ����ģʽ
  	2.addr		�豸�ڵ��ַ
  	3.param	    ���������Ĳ���
  Output:		��
  Return:		��
  Others:		��
*************************************************/
static void jd_aurine_light_ctrlmode(uint8 Index, uint8 mode, uint16 addr, char *param)
{
	uint8 nlen = 0;
	
	jd_aurine_buf[6] = JD_AU_ADDR_ID;				// Ѱַ��ʽ
	jd_aurine_buf[7] = JD_AURINE_SOURE_ADDR;		// Դ��ַ
	jd_aurine_buf[8] = (addr&0xff);					// ��ַID 
	jd_aurine_buf[9] = JD_AU_CMD_DW_CTRL_LIGHT;		// ����
	jd_aurine_buf[10] = mode;
	
	switch (mode)
	{
		case JD_AU_LIGHT_MODE_ADD:
		case JD_AU_LIGHT_MODE_DEC:
		case JD_AU_LIGHT_MODE_STOP_TUNE:
			jd_aurine_buf[2] = 10;
			jd_aurine_buf[11] =	JD_AURINE_TUNE_SPEED;
			break;
			
		case JD_AU_LIGHT_MODE_TUNE:
			jd_aurine_buf[2] = 10;
			jd_aurine_buf[11] =	JD_AURINE_LIGHT_TUNE_SPEED;
			break;
			
		case JD_AU_LIGHT_MODE_CHANGE:
			jd_aurine_buf[2] = 10;
			jd_aurine_buf[11] =	JD_AURINE_TUNE_SPEED;
			break;
			
		case JD_AU_LIGHT_MODE_OPEN:
			jd_aurine_buf[2] = 11;
			if ( param==NULL)
				return ;
			jd_aurine_buf[11] = param[0];
			jd_aurine_buf[12] =	JD_AURINE_TUNE_SPEED;
			nlen = 1;
			break;
			
		case JD_AU_LIGHT_MODE_CLOSE:
		case JD_AU_LIGHT_MODE_OPEN_OLD:
			jd_aurine_buf[2] = 11;
			jd_aurine_buf[11] = JD_AURINE_TUNE_SPEED;
			nlen = 1;
			break;
	}

	#ifdef _IP_MODULE_JD_
	if (get_ipmodule())
	{
		switch (mode)
		{
			case JD_AU_LIGHT_MODE_OPEN:
				ipmodule_jd_net_open(JD_DEV_LIGHT, Index, param, nlen);
				break;

			case JD_AU_LIGHT_MODE_CLOSE:
				ipmodule_jd_net_close(JD_DEV_LIGHT, Index, param, nlen);
				break;	
		}
	}
	else
	#endif	
	{
		add_send_node(JD_CMD_COTROL, jd_aurine_buf, JD_AURINE_PACKET_LEN+1);
	}
}

/*************************************************
  Function:		jd_aurine_window_ctrlmode
  Description:	���ּҵ�ƴ����������
  Input: 		
  	1.mode		�ƹ����ģʽ
  	2.addr		�豸�ڵ��ַ
  	3.param	���������Ĳ���
  Output:		��
  Return:		��
  Others:		��
*************************************************/
static void jd_aurine_window_ctrlmode(uint8 Index, uint8 mode, uint16 addr, char *param)
{
	uint8 data[20]= {0};
	uint8 nlen = 0;
	
	jd_aurine_buf[6] = JD_AU_ADDR_ID;			//Ѱַ��ʽ
	jd_aurine_buf[7] = JD_AURINE_SOURE_ADDR;	//Դ��ַ
	jd_aurine_buf[8] = (addr&0xff);			//��ַID 
	jd_aurine_buf[9] = JD_AU_CMD_DW_CTRL_WINDOW;	//����
	jd_aurine_buf[10] = mode;
	jd_aurine_buf[11] =	JD_AURINE_TUNE_SPEED;
	
	switch ( mode)
	{
		case JD_AU_WINDOW_MODE_OPEN:
			jd_aurine_buf[2] =10;
			data[0] = 100;
			nlen = 1;
			break;
			
		case JD_AU_WINDOW_MODE_CLOSE:
			jd_aurine_buf[2] =10;
			data[0] = 0;
			nlen = 1;
			break;
			
		case JD_AU_WINDOW_MODE_STOP:
			jd_aurine_buf[2] =10;
			data[0] = 1;
			nlen = 1;
			break;
	}

	#ifdef _IP_MODULE_JD_
	if (get_ipmodule())
	{
		switch (mode)
		{
			case JD_AU_WINDOW_MODE_OPEN:
				ipmodule_jd_net_open(JD_DEV_WINDOW, Index, data, nlen);
				break;

			case JD_AU_WINDOW_MODE_STOP:
			case JD_AU_WINDOW_MODE_CLOSE:
				ipmodule_jd_net_close(JD_DEV_WINDOW, Index, data, nlen);
				break;	
		}
	}
	else
	#endif	
	{
		add_send_node(JD_CMD_COTROL, jd_aurine_buf, JD_AURINE_PACKET_LEN+1);
	}
}

/*************************************************
  Function:		jd_aurine_ir_ctrlmode
  Description:	���ּҵ�ƴ����������
  Input: 		
  	1.mode		�ƹ����ģʽ
  	2.addr		�豸�ڵ��ַ
  	3.param	���������Ĳ���
  Output:		��
  Return:		��
  Others:		��
*************************************************/
static void jd_aurine_ir_ctrlmode(uint8 index, uint8 mode,uint16 addr,char *param)
{
	uint8 data[20]= {0};
	uint8 nlen = 0;
	
	jd_aurine_buf[6] = JD_AU_ADDR_ID;			//Ѱַ��ʽ
	jd_aurine_buf[7] = JD_AURINE_SOURE_ADDR;	//Դ��ַ
	jd_aurine_buf[8] = (addr&0xff);			//��ַID 
	jd_aurine_buf[9] = JD_AU_CMD_DW_CTRL_IR;   //
	jd_aurine_buf[10] = mode;

	switch(mode)
	{
		case JD_AU_IR_MODE_SENDIR:
		case JD_AU_IR_MODE_LEARNIR:
			jd_aurine_buf[11] = 0;
			jd_aurine_buf[12] = (param[0]&0xff);
			jd_aurine_buf[2] =11;
			data[0] = jd_aurine_buf[12];	
			nlen = 1;
			break;
			
		case JD_AU_IR_MODE_LEARN_SCENE:	
			break;
	}
	
	#ifdef _IP_MODULE_JD_
	if (get_ipmodule())
	{
		if (mode == JD_AU_IR_MODE_SENDIR)
		{
			if (21 == jd_aurine_buf[12])
			{
				ipmodule_jd_net_close(JD_DEV_KONGTIAO, index, data, nlen);
			}
			else
			{
				ipmodule_jd_net_open(JD_DEV_KONGTIAO, index, data, nlen);
			}
		}
	}
	else
	#endif
	{
		add_send_node(JD_CMD_COTROL, jd_aurine_buf, JD_AURINE_PACKET_LEN+1);	
	}
}

/*************************************************
  Function:		jd_aurine_power_ctrlmode
  Description:	���ּҵ�ƴ����������
  Input: 		
  	1.mode		�ƹ����ģʽ
  	2.addr		�豸�ڵ��ַ
  	3.param	���������Ĳ���
  Output:		��
  Return:		��
  Others:		��
*************************************************/
static void jd_aurine_power_ctrlmode(uint8 index, uint8 mode,uint16 addr,char *param)
{
	uint8 data[20]= {0};
	uint8 nlen = 0;
	
	jd_aurine_buf[6] = JD_AU_ADDR_ID;			//Ѱַ��ʽ
	jd_aurine_buf[7] = JD_AURINE_SOURE_ADDR;	//Դ��ַ
	jd_aurine_buf[8] = (addr&0xff);			//��ַID 
	jd_aurine_buf[9] = JD_AU_CMD_DW_CTRL_LIGHT;	//����
	jd_aurine_buf[10] = mode;
	
	switch ( mode)
	{
		case JD_AU_LIGHT_MODE_OPEN:
			jd_aurine_buf[2] =11;
			if ( param==NULL)
				return ;
			jd_aurine_buf[11] = param[0];
			jd_aurine_buf[12] =	JD_AURINE_TUNE_SPEED;
			data[0] = param[0]*100/JD_AURINE_LIGHT_LEVEL;
			nlen = 1;
			break;
			
		case JD_AU_LIGHT_MODE_CLOSE:
			jd_aurine_buf[2] =11;
			jd_aurine_buf[11] = JD_AURINE_TUNE_SPEED;
			data[0] = 0;
			nlen = 1;
			break;
	}
	
	#ifdef _IP_MODULE_JD_
	if (get_ipmodule())
	{
		switch (mode)
		{
			case JD_AU_LIGHT_MODE_OPEN:
				ipmodule_jd_net_open(JD_DEV_POWER, index, data, nlen);
				break;

			case JD_AU_LIGHT_MODE_CLOSE:
				ipmodule_jd_net_close(JD_DEV_POWER, index, data, nlen);
				break;	
		}
	}
	else
	#endif	
	{
		add_send_node(JD_CMD_COTROL, jd_aurine_buf, JD_AURINE_PACKET_LEN+1);
	}
}

/*************************************************
  Function:		jd_aurine_gas_ctrlmode
  Description:	���ּҵ�ƴ����������
  Input: 		
  	1.mode		�ƹ����ģʽ
  	2.addr		�豸�ڵ��ַ
  	3.param	���������Ĳ���
  Output:		��
  Return:		��
  Others:		��
*************************************************/
static void jd_aurine_gas_ctrlmode(uint8 index, uint8 mode,uint16 addr,char *param)
{
	uint8 data[20]= {0};
	uint8 nlen = 0;
	
	jd_aurine_buf[6] = JD_AU_ADDR_ID;			//Ѱַ��ʽ
	jd_aurine_buf[7] = JD_AURINE_SOURE_ADDR;	//Դ��ַ
	jd_aurine_buf[8] = (addr&0xff);			//��ַID 
	jd_aurine_buf[9] = JD_AU_CMD_DW_CTRL_LIGHT;	//����
	jd_aurine_buf[10] = mode;
	
	switch ( mode)
	{
		case JD_AU_LIGHT_MODE_OPEN:
			jd_aurine_buf[2] =11;
			if ( param==NULL)
				return ;
			jd_aurine_buf[11] = param[0];
			jd_aurine_buf[12] =	JD_AURINE_TUNE_SPEED;
			data[0] = param[0]*100/JD_AURINE_LIGHT_LEVEL;
			nlen = 1;
			break;
			
		case JD_AU_LIGHT_MODE_CLOSE:
			jd_aurine_buf[2] =11;
			jd_aurine_buf[11] = JD_AURINE_TUNE_SPEED;
			data[0] = 0;
			nlen = 1;
			break;
			
	}

	#ifdef _IP_MODULE_JD_
	if (get_ipmodule())
	{
		switch (mode)
		{
			case JD_AU_LIGHT_MODE_OPEN:
				ipmodule_jd_net_open(JD_DEV_GAS, index, data, nlen);
				break;

			case JD_AU_LIGHT_MODE_CLOSE:
				ipmodule_jd_net_close(JD_DEV_GAS, index, data, nlen);
				break;	
		}
	}
	else
	#endif	
	{
		add_send_node(JD_CMD_COTROL, jd_aurine_buf, JD_AURINE_PACKET_LEN+1);
	}
}

/*************************************************
  Function:		jd_aurine_scene_ctrlmode
  Description:	���ּҵ��龰�����
  Input: 		
  	1.mode		�ƹ����ģʽ
  	2.addr		�豸�ڵ��ַ
  	3.param	���������Ĳ���
  Output:		��
  Return:		��
  Others:		��
*************************************************/
static void jd_aurine_scene_ctrlmode(uint8 mode, uint8 scene_id, uint8 group_id, int index)
{
	jd_aurine_buf[7] = JD_AURINE_SOURE_ADDR;			// Դ��ַ
	switch (mode)
	{
		case JD_AU_SCENE_MODE_OPEN:
		case JD_AU_SCENE_MODE_CLOSE:
			jd_aurine_buf[6] = JD_AU_GROUP_ID;			// Ѱַ��ʽ
			jd_aurine_buf[9] = JD_AU_CMD_DW_CTRL_SCENE;	// ����
			jd_aurine_buf[10] = mode;
			jd_aurine_buf[11] =	scene_id;
			jd_aurine_buf[8] = (group_id&0xff);			// ��ַID 
			jd_aurine_buf[2] = 10;
			break;
			
		case JD_AU_WINDOW_MODE_STOP:
			jd_aurine_buf[6] = JD_AU_ADDR_BROADCAST;	// Ѱַ��ʽ
			jd_aurine_buf[8] = JD_AU_CMD_DW_CTRL_SCENE;	// ����
			jd_aurine_buf[9] = mode;
			jd_aurine_buf[10] =	scene_id;
			jd_aurine_buf[11] = group_id;
			jd_aurine_buf[2] = 10;
			break;
	}

	#ifdef _IP_MODULE_JD_
	if (get_ipmodule())
	{
		ipmodule_jd_net_scene(index);
	}
	else
	#endif	
	{
		add_send_node(JD_CMD_COTROL, jd_aurine_buf, JD_AURINE_PACKET_LEN+1);
	}
}

/*************************************************
  Function:		jd_aurine_light_ctrlmode
  Description:	���ƹ��ּҵ�ƹ⿪
  Input: 		
  	2.addr		�豸�ڵ��ַ
  	3.param	    ���ȼ���:0-100%��0xff: ����ԭ�������ȼ���
  			
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_light_open(uint8 Index, uint16 addr, uint8 param)
{
	//param :0-100%  �谴�յƹ����ȼ���32������ת��
	#ifdef _IP_MODULE_JD_
	uint8 level = param;

	if (1 != get_ipmodule())
	{
		level = param*JD_AURINE_LIGHT_LEVEL/100;
		dprintf("jd_aurine_light_open 1\n");
	}

	dprintf("jd_aurine_light_open %d %d\n",param,level);
	#else
	uint8 level = param*JD_AURINE_LIGHT_LEVEL/100;
	#endif
	if (param == 0xFF )
	{
		// ����ԭ�������ȼ���
		jd_aurine_light_ctrlmode(Index, JD_AU_LIGHT_MODE_OPEN_OLD,addr,0);
	}
	else
	{
		// ����ĳ�����ȼ���
		jd_aurine_light_ctrlmode(Index, JD_AU_LIGHT_MODE_OPEN,addr,&level);
	}
}

/*************************************************
  Function:		jd_aurine_light_open_by_speed
  Description:	���ƹ��ּҵ�ƹ⿪
  Input: 		
  	2.addr		�豸�ڵ��ַ
  	3.param	    ���ȼ���:0-100%��0xff: ����ԭ�������ȼ���
  			
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_light_open_by_speed(uint8 Index, uint16 addr,uint8 param)
{
	uint8 level = param*JD_AURINE_LIGHT_LEVEL/100;
	uint8 data = param;
	
	jd_aurine_buf[6] = JD_AU_ADDR_ID;			//Ѱַ��ʽ
	jd_aurine_buf[7] = JD_AURINE_SOURE_ADDR;	//Դ��ַ
	jd_aurine_buf[8] = (addr&0xff);			//��ַID 
	jd_aurine_buf[9] = JD_AU_CMD_DW_CTRL_LIGHT;	//����
	jd_aurine_buf[10] = JD_AU_LIGHT_MODE_OPEN;
	jd_aurine_buf[2] = 11;
	jd_aurine_buf[11] = level;
	jd_aurine_buf[12] =	JD_AURINE_TUNE_SPEED_FAST;

	#ifdef _IP_MODULE_JD_
	if (get_ipmodule())
	{
		ipmodule_jd_net_open(JD_DEV_LIGHT, Index, &data, 1);
	}
	else
	#endif	
	{
		add_send_node(JD_CMD_COTROL, jd_aurine_buf, JD_AURINE_PACKET_LEN+1);
	}
}

/*************************************************
  Function:		jd_auine_light_close
  Description:	���ƹ��ּҵ�ƹ��
  Input: 		
  	1.addr		�豸�ڵ��ַ		
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_light_close(uint8 Index, uint16 addr)
{
	jd_aurine_light_ctrlmode(Index, JD_AU_LIGHT_MODE_CLOSE,addr, NULL);
}

/*************************************************
  Function:		jd_auine_window_open
  Description:	���ƹ��ּҵ�ƹ��
  Input: 		
  	1.addr		�豸�ڵ��ַ		
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_window_open(uint8 Index, uint16 addr )
{
	jd_aurine_window_ctrlmode(Index,JD_AU_WINDOW_MODE_OPEN,addr, NULL);
}

/*************************************************
  Function:		jd_auine_light_close
  Description:	���ƹ��ּҵ�ƹ��
  Input: 		
  	1.addr		�豸�ڵ��ַ		
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_window_close(uint8 Index,uint16 addr )
{
	jd_aurine_window_ctrlmode(Index,JD_AU_WINDOW_MODE_CLOSE,addr, NULL);
}

/*************************************************
  Function:		jd_aurine_window_stop
  Description:	���ƹ��ּҵ�ƹ��
  Input: 		
  	1.addr		�豸�ڵ��ַ		
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_window_stop(uint8 Index,uint16 addr )
{
	jd_aurine_window_ctrlmode(Index,JD_AU_WINDOW_MODE_STOP,addr, NULL);
}

/*************************************************
  Function:		jd_auine_gas_open
  Description:	
  Input: 		
  	1.addr		�豸�ڵ��ַ		
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_gas_open(uint8 index, uint16 addr , uint8 param)
{
	uint8 level = param*JD_AURINE_LIGHT_LEVEL/100;
	
	jd_aurine_gas_ctrlmode(index, JD_AU_LIGHT_MODE_OPEN, addr, &level);
}

/*************************************************
  Function:		jd_aurine_gas_close
  Description:	
  Input: 		
  	1.addr		�豸�ڵ��ַ		
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_gas_close(uint8 index,uint16 addr)
{
	jd_aurine_gas_ctrlmode(index,JD_AU_LIGHT_MODE_CLOSE, addr, NULL);
}

/*************************************************
  Function:		jd_auine_power_open
  Description:	
  Input: 		
  	1.addr		�豸�ڵ��ַ		
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_power_open(uint8 index, uint16 addr , uint8 param)
{
	uint8 level = param*JD_AURINE_LIGHT_LEVEL/100;
	jd_aurine_power_ctrlmode(index, JD_AU_LIGHT_MODE_OPEN, addr, &level);
}

/*************************************************
  Function:		jd_aurine_power_close
  Description:	
  Input: 		
  	1.addr		�豸�ڵ��ַ		
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_power_close(uint8 index, uint16 addr)
{
	jd_aurine_power_ctrlmode(index, JD_AU_LIGHT_MODE_CLOSE, addr, NULL);
}

/*************************************************
  Function:		jd_aurine_dev_all_open
  Description:	���ּҵ��豸ȫ��
  Input: 		
  	1.inedx		�豸����		
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_dev_all_open(uint8 devtype)
{
	uint8 data[20] = {0};
	uint8 nlen = 0;
	
	#ifdef _IP_MODULE_JD_
	if (get_ipmodule())
	{
		data[0] = 100;
		nlen = 1;
		ipmodule_jd_net_open(devtype, 0xFE, data, nlen);
	}
	else
	#endif	
	{
		switch (devtype)
	  	{
		   	case JD_DEV_LIGHT:						// �ƹ��豸
				jd_aurine_scene_ctrlmode(JD_AU_SCENE_MODE_OPEN, 6, 6, 0);
				break;
				
			case JD_DEV_KONGTIAO:					// �յ��豸
				jd_aurine_scene_ctrlmode(JD_AU_SCENE_MODE_OPEN, 12, 12, 0);
				break;
				
			case JD_DEV_WINDOW:						// �����豸
				jd_aurine_scene_ctrlmode(JD_AU_SCENE_MODE_OPEN, 8, 8, 0);
				break;
				
			case JD_DEV_POWER:						// ��Դ�豸
				jd_aurine_scene_ctrlmode(JD_AU_SCENE_MODE_OPEN, 10, 10, 0);
				break;
				
			case JD_DEV_GAS:					 	// ú���豸
				//jd_aurine_scene_ctrlmode(JD_AU_SCENE_MODE_OPEN, 14, 14, 0);
				break;	
	  	}
	}
}

/*************************************************
  Function:		jd_aurine_dev_all_close
  Description:	���ּҵ��豸ȫ��
  Input: 		
  	1.inedx		�豸����		
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_dev_all_close(uint8 devtype)
{
	uint8 data[20]={0};
	uint8 nlen = 0;
	#ifdef _IP_MODULE_JD_
	if (get_ipmodule())
	{
		data[0] = 0;
		nlen = 1;
		ipmodule_jd_net_close(devtype, 0xFE, data, nlen);
	}
	else
	#endif	
	{
		 switch(devtype)
		 {
		   	case JD_DEV_LIGHT:					 //�ƹ��豸
				jd_aurine_scene_ctrlmode(JD_AU_SCENE_MODE_OPEN,7,7,0);
				break;
				
			case JD_DEV_KONGTIAO:				// �յ��豸
				jd_aurine_scene_ctrlmode(JD_AU_SCENE_MODE_OPEN,13,13,0);
				break;
				
			case JD_DEV_WINDOW:					// �����豸
				jd_aurine_scene_ctrlmode(JD_AU_SCENE_MODE_OPEN,9,9,0);
				break;
				
			case JD_DEV_POWER:					// ��Դ�豸
				jd_aurine_scene_ctrlmode(JD_AU_SCENE_MODE_OPEN,11,11,0);
				break;
				
			case JD_DEV_GAS:					 //ú���豸
				//jd_aurine_scene_ctrlmode(JD_AU_SCENE_MODE_OPEN,15,15,0);
				break;	
		  }
	}
}

/*************************************************
  Function:		jd_aurine_send_ir_to_dev_open
  Description:	���ƹ��ּҵ��豸ȫ��ȫ��
  Input: 		
  	2.addr		�豸�ڵ��ַ
  	3.param	>0:���� ��<0:����
  			
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_send_ir_to_dev_oper(uint8 oper, uint8 index, uint8 addr,uint16 param)
{
	uint8 data[20]= {0};
	uint8 nlen = 0;
	
	jd_aurine_buf[6] = JD_AU_ADDR_ID;			//Ѱַ��ʽ
	jd_aurine_buf[7] = JD_AURINE_SOURE_ADDR;	//Դ��ַ
	jd_aurine_buf[8] = (addr&0xff);			//��ַID 
	jd_aurine_buf[9] = JD_AU_CMD_DW_CTRL_IR;   //
	jd_aurine_buf[10] = JD_AU_IR_MODE_SENDIR;

	jd_aurine_buf[11] = 0;
	jd_aurine_buf[12] = (param&0xff);
	jd_aurine_buf[2] =11;
	data[0] = jd_aurine_buf[12];	
	nlen = 1;
	
	#ifdef _IP_MODULE_JD_
	if (get_ipmodule())
	{
		if (0 == oper)
		{
			ipmodule_jd_net_close(JD_DEV_KONGTIAO, index, data, nlen);
		}
		else
		{
			ipmodule_jd_net_open(JD_DEV_KONGTIAO, index, data, nlen);
		}
	}
	else
	#endif
	{
		add_send_node(JD_CMD_COTROL, jd_aurine_buf, JD_AURINE_PACKET_LEN+1);	
	}
}

/*************************************************
  Function:		jd_aurine_send_ir_to_dev
  Description:	���ƹ��ּҵ��豸ȫ��ȫ��
  Input: 		
  	2.addr		�豸�ڵ��ַ
  	3.param	>0:���� ��<0:����
  			
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_send_ir_to_dev(uint8 index, uint8 addr,uint16 param)
{
	jd_aurine_ir_ctrlmode(index, JD_AU_IR_MODE_SENDIR, addr, (char *)&param);
}

/*************************************************
  Function:		jd_aurine_learn_ir
  Description:	���ƹ��ּҵ��豸ȫ��ȫ��
  Input: 		
  	2.addr		�豸�ڵ��ַ
  	3.param	>0:���� ��<0:����
  			
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_learn_ir(uint8 index,uint8 addr,uint16 param)
{
	jd_aurine_ir_ctrlmode(index,JD_AU_IR_MODE_LEARNIR,addr, (char *)&param);
}

/*************************************************
  Function:		jd_aurine_scene_open
  Description:	���ƹ��ּҵ��龰ģʽ��
  Input: 		
  	1.addr		�豸�ڵ��ַ		
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_scene_open(uint8 scene_id,uint8 group_id, int index)
{
	jd_aurine_scene_ctrlmode(JD_AU_SCENE_MODE_OPEN, scene_id, group_id,index);
}

/*************************************************
  Function:		jd_aurine_dev_set_update_state
  Description:	���üҵ��豸��ѯ���´���״̬
  Input: 		
  	2.state		
  		0:     ��ͣ����
  		1:    �ָ�����
  		2:   ǿ�Ƹ���
  Output:		��
  Return:		��
  Others:		��
*************************************************/
static int jd_aurine_control_scene(uint8 sceneid,uint8 param)
{
	PJD_SCENE_INFO_LIST scene_param;
	
	jd_aurine_dev_set_update_state(0);
	scene_param = storage_get_aurine_scene_info(0);
	jd_aurine_scene_open(scene_param->pjd_scene_info[sceneid].Address, scene_param->pjd_scene_info[sceneid].Address,sceneid+1);
}

/*************************************************
  Function:		jd_aurine_yuyue_oper
  Description: 	
  Input:		��
  Output:		��
  Return:		
  Others:		
*************************************************/
void jd_aurine_yuyue_oper(uint16 cmd,uint8 devtype, uint8 index, uint16 address,uint8 param)
{
	int i,j;
	uint8 stop = 0 ;
	dprintf("-------jd_aurine_yuyue_oper----address---%d---param---%d------\n",address,param);
	switch(devtype)
	{
		case JD_DEV_LIGHT:	
			if(cmd == JD_ON)
				jd_aurine_light_open_by_speed(index,address,100);
			if(cmd == JD_OFF)
				jd_aurine_light_close(index,address);
			break;
			
		case JD_DEV_KONGTIAO:
			if(cmd == JD_ON)
				jd_aurine_send_ir_to_dev(index,address,5);
			if(cmd == JD_OFF)
				jd_aurine_send_ir_to_dev(index,address,21);
			break;
			
		case JD_DEV_WINDOW:
			if(cmd == JD_ON)
				jd_aurine_window_open(index,address);
			if(cmd == JD_OFF)
				jd_aurine_window_close(index,address);
			break;
			
		case JD_DEV_POWER:
			if(cmd == JD_ON)
				jd_aurine_power_open(index,address,param);
			if(cmd == JD_OFF)
				jd_aurine_power_close(index,address);
			break;
			
		case JD_DEV_GAS:
			if(cmd == JD_ON)
				jd_aurine_gas_open(index,address,param);
			if(cmd == JD_OFF)
				jd_aurine_gas_close(index,address);
			break;
	}
}

/*************************************************
  Function:		jd_aurine_control_dev
  Description:	���Ƽҵ��豸����
  Input: 		
  	1.inedx		�豸����
  	2.param		����
  Output:		��
  Return:		��
  Others:		��
*************************************************/
static int jd_aurine_control_dev(uint16 cmd,uint8 dev,uint8 devindex,uint8 param1,uint8 param2)
{       
    uint8 devtype_id[5] = {JD_DEV_LIGHT, JD_DEV_KONGTIAO, JD_DEV_WINDOW,JD_DEV_POWER, JD_DEV_GAS};
	uint8 devtype = devtype_id[dev];
	uint8 param;
	jd_aurine_dev_set_update_state(0);
	
	//dprintf("--------jd_aurine_control_dev---%d--%d--%d----------\n",devtype,devindex,param1);
	if (devindex == 0xff)
	{
		switch(cmd)
		{
			case CMD_JD_DEV_OPEN:
				jd_aurine_dev_all_open((AU_JD_DEV_TYPE)devtype);
				break;
				
			case CMD_JD_DEV_CLOSE:
				jd_aurine_dev_all_close((AU_JD_DEV_TYPE)devtype);
				break;
		}
	}
	else
	{   
	    jd_load_devinfo();
		devindex = devindex-1;
		switch (devtype)
		{
			case JD_DEV_LIGHT:
    			if (cmd == CMD_JD_DEV_OPEN)
    			{
    			    if (param1 == 0xFF)
    			    {
    			       jd_aurine_light_open(g_jd_state_dg_list[devindex].Index,g_jd_state_dg_list[devindex].Address,param1); 
    			    }
    				else 
    				{
    				    if (param1 == 0)
    				    {
    				        param1 = 100;
    				    }
    				    jd_aurine_light_open_by_speed(g_jd_state_dg_list[devindex].Index,g_jd_state_dg_list[devindex].Address,param1);
    				}
    			}
    			else
    			{
    				jd_aurine_light_close(g_jd_state_dg_list[devindex].Index,g_jd_state_dg_list[devindex].Address);
    			}
    			
			    break;
			
			case JD_DEV_KONGTIAO:
    			if (cmd == CMD_JD_DEV_OPEN)
    			{
    			    if (param1 == 255)
    			    {
    			        param = 5;
    			    }
    			    else                            // Ϊ���ֻ������п��ƿյ�������ģʽ
    			    {
    			        if (param1 < 2)             // ������ůģʽ
    			        {
    			            param = (param1*10)+param2;
    			        }
    			        if (param1 == 2)
    			        {
    			            param = (param1*10)+param2+1;  // �յ�����ģʽ
    			        }
    			    }
    				jd_aurine_send_ir_to_dev(g_jd_state_kt_list[devindex].Index,g_jd_state_kt_list[devindex].Address,param);
    			}
    			else
    			{
    				jd_aurine_send_ir_to_dev(g_jd_state_kt_list[devindex].Index,g_jd_state_kt_list[devindex].Address,21);
    			}
    			
			    break;
			
			case JD_DEV_WINDOW:
    			if (cmd == CMD_JD_DEV_OPEN)
    			{
    				jd_aurine_window_open(g_jd_state_cl_list[devindex].Index,g_jd_state_cl_list[devindex].Address);
    			}
    			else
    			{
    				if(param1 == 1)
    				{
    				    jd_aurine_window_stop(g_jd_state_cl_list[devindex].Index,g_jd_state_cl_list[devindex].Address);
    				}
    				else
    				{
    				    jd_aurine_window_close(g_jd_state_cl_list[devindex].Index,g_jd_state_cl_list[devindex].Address);
    				}
    			}
    			
    		    break;
			
			case JD_DEV_POWER:
    			if (cmd == CMD_JD_DEV_OPEN)
    			{
    				if(param1 == 0)
    				{
    				    param1 = 100;
    				}
    				jd_aurine_power_open(g_jd_state_cz_list[devindex].Index,g_jd_state_cz_list[devindex].Address,100);
    			}
    			else
    			{
    				jd_aurine_power_open(g_jd_state_cz_list[devindex].Index,g_jd_state_cz_list[devindex].Address,0);
    			}
    			
    		    break;
			
			case JD_DEV_GAS:
    			if (cmd == CMD_JD_DEV_OPEN)
    			{
    				if(param1 == 0)
    				{
    				    param1 = 100;
    				}
    				jd_aurine_gas_open(g_jd_state_mq_list[devindex].Index,g_jd_state_mq_list[devindex].Address,100);
    			}
    			else
    			{
    				jd_aurine_gas_open(g_jd_state_mq_list[devindex].Index,g_jd_state_mq_list[devindex].Address,0);
    			}
			    break;
		}
	}
	return 1;
}

/*************************************************
  Function:		jd_aurine_dev_set_update_state
  Description:	���üҵ��豸��ѯ���´���״̬
  Input: 		
  	2.state		
  		0:     ��ͣ����
  		1:    �ָ�����
  		2:   ǿ�Ƹ���
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_dev_set_update_state(uint8 state)
{
	JD_AURINE_DEV_UPDATE_STATE updatestate ; 
	switch (state )
	{
		case 0:
			updatestate = JD_AURINE_UPDATE_PAUSE;
			break;
		case 1:
			updatestate = JD_AURINE_UPDATE_IDLE;
			break;
		case 2:
			updatestate = JD_AURINE_UPDATE_RUNNING;
			break;
		case 3:
			updatestate = JD_AURINE_UPDATE_SINGLE_DEV;
			break;
		case 4:
			updatestate = JD_AURINE_UPDATE_STOP;
			break;
	}	

	g_jd_aurine_update_state = updatestate;
}

/*************************************************
  Function:		jd_aurine_dev_Batchupdate
  Description:	������ѯ���ּҵ��豸״̬
  Input: 		
  	2.addr		�豸�ڵ��ַ,=0xFF:��㲥��ѯ�������Ե��ѯ
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_dev_Batchupdate(AU_JD_DEV_TYPE JDType,uint32 Check,uint8 num,uint16 *ID_list, uint8 *index)
{
	int i;
	
	jd_aurine_buf[6] = JD_AU_ADDR_BROADCAST;
	jd_aurine_buf[7] = JD_AURINE_SOURE_ADDR;
	jd_aurine_buf[8] = JD_AU_CMD_DEV_BATCH_QUERY_STATE;
	jd_aurine_buf[9] = num;
	
	for (i = 0; i < num; i++)
	{
		jd_aurine_buf[10+i] = ID_list[i];
	}
	jd_aurine_buf[2] = 8+num;
	
	#ifdef _IP_MODULE_JD_
	if (get_ipmodule())
	{
		ipmodule_jd_net_get_state(JDType, Check, index, num);
	}
	else
	#endif	
	{
		add_send_node(JD_CMD_COTROL, jd_aurine_buf, JD_AURINE_PACKET_LEN+1);
	}
}

/*************************************************
  Function:		jd_aurine_dev_update
  Description:	��ѯ���ּҵ��豸״̬
  Input: 		
  	2.addr		�豸�ڵ��ַ,=0xFF:��㲥��ѯ�������Ե��ѯ
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_dev_update(AU_JD_DEV_TYPE JDType,uint32 Check,uint16 index, uint16 addr)
{
	int len = 8;
	int i=0;
	uint16 Index = index;
	
	if (addr == 0xFF )
	{
		//�㲥
		jd_aurine_buf[6] = JD_AU_ADDR_BROADCAST;			//Ѱַ��ʽ
		jd_aurine_buf[2] = 7;
		i= 0;
	}
	else
	{
		jd_aurine_buf[6] = JD_AU_ADDR_ID;					//Դ��ַ
		jd_aurine_buf[8] = addr&0xff;					
		i=1;
		jd_aurine_buf[2] = 8;
	}
	jd_aurine_buf[7] = JD_AURINE_SOURE_ADDR;
	
	jd_aurine_buf[8+i] = JD_AU_CMD_DW_QUERY_STATE;	//����

	#ifdef _IP_MODULE_JD_
	if (get_ipmodule())
	{	
		ipmodule_jd_net_get_state(JDType, Check, &Index, 1);
	}
	else
	#endif	
	{
		add_send_node(JD_CMD_COTROL, jd_aurine_buf, JD_AURINE_PACKET_LEN+1);
	}
}

/*************************************************
  Function:    	jd_state_report_init
  Description: 	�ҵ����ݴ���ص��ҵ����	
  Input:
  	1.			�ص�
  Output: 		��   		
  Return:		��
  Others:           
*************************************************/
void jd_state_report_init(PAuJiaDianStateChanged proc)
{
	 g_AuJiaDianStateChanged = proc;	    
}

/*************************************************
  Function:		init_kt_learn_state_report
  Description:	�յ�ѧϰ״̬�ϱ�
  Input: 		
  	1.inedx		�豸����		
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void init_kt_learn_state_report(PAuJiaDianStateChanged proc)
{
    g_AuKongtiaoStateChanged = proc;
}

/*************************************************
  Function:		jd_aurine_get_dev_state
  Description:	��ȡ���ּҵ��豸״̬
  Input: 		
  	1.inedx		�豸����		
  Output:		��
  Return:		��
  Others:		��
*************************************************/
uint8 jd_aurine_get_dev_state(AU_JD_DEV_TYPE devtype,uint8 index)
{
	switch (devtype)
	{
    	case JD_DEV_LIGHT:
    		return g_jd_state_dg_list[index].Dev_State.Active;
    		
    	case JD_DEV_KONGTIAO:
    		return g_jd_state_kt_list[index].Dev_State.Active;
    		
    	case JD_DEV_WINDOW:
    		return g_jd_state_cl_list[index].Dev_State.Active;
    		
    	case JD_DEV_POWER:
    		return g_jd_state_cz_list[index].Dev_State.Active;
    		
    	case JD_DEV_GAS:
    		return g_jd_state_mq_list[index].Dev_State.Active;	
	}
}

/*************************************************
  Function:		jd_aurine_set_light_state
  Description:	������ּҵ�ƹ�״̬
  Input: 		
  	1.inedx		�豸����
  	2.param		����
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_set_dev_state(AU_JD_DEV_TYPE devtype,uint8 index,uint8 param)
{
	switch(devtype)
	{
    	case JD_DEV_LIGHT:
    		g_jd_state_dg_list[index].Dev_State.Active = param;
    		return;
    		
    	case JD_DEV_KONGTIAO:
    		g_jd_state_kt_list[index].Dev_State.Active = param;
    		return;
    		
    	case JD_DEV_WINDOW:
    		g_jd_state_cl_list[index].Dev_State.Active = param;
    		return;
    		
    	case JD_DEV_POWER:
    		g_jd_state_cz_list[index].Dev_State.Active = param;
    		return;
    		
    	case JD_DEV_GAS:
    		g_jd_state_mq_list[index].Dev_State.Active = param;	
    		return;
    		
	}
}

/*************************************************
  Function:		jd_aurine_get_dev_state_param
  Description:	��ȡ���ּҵ����õĲ���
  Input: 		
  	1.inedx		�豸����		
  Output:		��
  Return:		��
  Others:		��
*************************************************/
uint8 jd_aurine_get_dev_state_param(AU_JD_DEV_TYPE devtype,uint8 index)
{
	switch(devtype)
	{
    	case JD_DEV_LIGHT:
    		return g_jd_state_dg_list[index].Dev_State.ActiveParam;
    		
    	case JD_DEV_KONGTIAO:
    		return g_jd_state_kt_list[index].Dev_State.ActiveParam;
    		
    	case JD_DEV_WINDOW:
    		return g_jd_state_cl_list[index].Dev_State.ActiveParam;
    		
    	case JD_DEV_POWER:
    		return g_jd_state_cz_list[index].Dev_State.ActiveParam;
    		
    	case JD_DEV_GAS:
    		return g_jd_state_mq_list[index].Dev_State.ActiveParam;		
	}
}

/*************************************************
  Function:		jd_aurine_set_dev_state_param
  Description:	������ּҵ����õĲ���
  Input: 		
  	1.inedx		�豸����
  	2.param		����
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_set_dev_state_param(AU_JD_DEV_TYPE devtype, uint8 index, uint8 param)
{
	switch (devtype)
	{
    	case JD_DEV_LIGHT:
    		g_jd_state_dg_list[index].Dev_State.ActiveParam = param;
    		return;
    		
    	case JD_DEV_KONGTIAO:
    		g_jd_state_kt_list[index].Dev_State.ActiveParam = param;
    		return;
    		
    	case JD_DEV_WINDOW:
    		g_jd_state_cl_list[index].Dev_State.ActiveParam = param;
    		return;
    		
    	case JD_DEV_POWER:
    		g_jd_state_cz_list[index].Dev_State.ActiveParam = param;
    		return;
    		
    	case JD_DEV_GAS:
    		g_jd_state_mq_list[index].Dev_State.ActiveParam = param;	
    		return;
	}
}

/*************************************************
  Function:		jd_aurine_get_dev_address
  Description:	��ȡ���ּҵ�ƹ�״̬
  Input: 		
  	1.inedx		�豸����		
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_set_dev_address(AU_JD_DEV_TYPE devtype,uint8 index, uint16 address)
{
	switch(devtype)
	{
    	case JD_DEV_LIGHT:
    		g_jd_state_dg_list[index].Address = address;
    		return;
    		
    	case JD_DEV_KONGTIAO:
    		g_jd_state_kt_list[index].Address = address;
    		return;
    		
    	case JD_DEV_WINDOW:
    		g_jd_state_cl_list[index].Address = address;
    		return;
    		
    	case JD_DEV_POWER:
    		g_jd_state_cz_list[index].Address = address;
    		return;
    		
    	case JD_DEV_GAS:
    		g_jd_state_mq_list[index].Address = address;	
    		return;
	}
}

/*************************************************
  Function:		jd_aurine_get_dev_address
  Description:	��ȡ���ּҵ�ƹ�״̬
  Input: 		
  	1.inedx		�豸����		
  Output:		��
  Return:		��
  Others:		��
*************************************************/
uint16 jd_aurine_get_dev_address(AU_JD_DEV_TYPE devtype,uint8 index)
{
	switch(devtype)
	{
    	case JD_DEV_LIGHT:
    		return g_jd_state_dg_list[index].Address;
    		
    	case JD_DEV_KONGTIAO:
    		return g_jd_state_kt_list[index].Address;
    		
    	case JD_DEV_WINDOW:
    		return g_jd_state_cl_list[index].Address;
    		
    	case JD_DEV_POWER:
    		return g_jd_state_cz_list[index].Address;
    		
    	case JD_DEV_GAS:
    		return g_jd_state_mq_list[index].Address;	
	}
}

/*************************************************
  Function:		jd_aurine_get_devinfo
  Description:	��ȡ�ҵ�״̬��Ϣ
  Input: 		
  	1.inedx		�豸����
  	2.param		����
  Output:		��
  Return:		��
  Others:		��
*************************************************/
static int jd_aurine_get_devinfo(uint8 dev,uint8 index,char* infodata)
{
	int size;
	int number;
	char * data;
	int i;
	AU_JD_STATE state;
	int state_size = sizeof(AU_JD_STATE);
	
	uint8 devtype_id[5] = {JD_DEV_LIGHT, JD_DEV_KONGTIAO, JD_DEV_WINDOW,JD_DEV_POWER, JD_DEV_GAS};
	uint8 devtype = 0;
	
	if (dev == 0xff)
	{
		number = MAX_AURINE_JD_DG_NUM+MAX_AURINE_JD_KT_NUM+MAX_AURINE_JD_CL_NUM+MAX_AURINE_JD_CZ_NUM+MAX_AURINE_JD_MQ_NUM;
		size = state_size*number;
		data = malloc(size);
		for(i = 0; i < MAX_AURINE_JD_DG_NUM; i++)
		{
			state = g_jd_state_dg_list[i].Dev_State;
			memcpy(data+(i*state_size),&state,state_size);
		}
		for(i= 0 ;i < MAX_AURINE_JD_KT_NUM; i++)
		{
			state = g_jd_state_kt_list[i].Dev_State;
			memcpy(data+((MAX_AURINE_JD_DG_NUM+i)*state_size),&state,state_size);
		}
		for(i = 0; i < MAX_AURINE_JD_CL_NUM; i++)
		{
			state = g_jd_state_cl_list[i].Dev_State;
			memcpy(data+((MAX_AURINE_JD_DG_NUM+MAX_AURINE_JD_KT_NUM+i)*state_size),&state,state_size);
		}
		for(i = 0;i < MAX_AURINE_JD_CZ_NUM; i++)
		{
			state = g_jd_state_cz_list[i].Dev_State;
			memcpy(data+((MAX_AURINE_JD_CL_NUM+MAX_AURINE_JD_DG_NUM+MAX_AURINE_JD_KT_NUM+i)*state_size),&state,state_size);
		}
		for(i = 0;i < MAX_AURINE_JD_MQ_NUM; i++)
		{
			state = g_jd_state_mq_list[i].Dev_State;
			memcpy(data+((MAX_AURINE_JD_CZ_NUM+MAX_AURINE_JD_CL_NUM+MAX_AURINE_JD_DG_NUM+MAX_AURINE_JD_KT_NUM+i)*state_size),&state,state_size);
		}
	}
	else
	{
	    devtype = devtype_id[dev];
		if (index == 0xff)
		{
			switch (devtype)
			{
				case JD_DEV_LIGHT:
					number = MAX_AURINE_JD_DG_NUM;
					size = state_size*number;
					data = malloc(size);
					for(i = 0; i < number; i++)
					{
						state = g_jd_state_dg_list[i].Dev_State;
						memcpy(data+(i*state_size),&state,state_size);
					}
					break;
					
				case JD_DEV_KONGTIAO:
					number = MAX_AURINE_JD_KT_NUM;
					size = state_size*number;
					data = malloc(size);
					for(i = 0 ;i < number; i++)
					{
						state = g_jd_state_kt_list[i].Dev_State;
						memcpy(data+(i*state_size),&state,state_size);
					}
					break;
					
				case JD_DEV_WINDOW:
					number = MAX_AURINE_JD_CL_NUM;
					size = state_size*number;
					data = malloc(size);
					for(i = 0; i < number; i++)
					{
						state = g_jd_state_cl_list[i].Dev_State;
						memcpy(data+(i*state_size),&state,state_size);
					}
					break;
					
				case JD_DEV_POWER:
					number = MAX_AURINE_JD_CZ_NUM;
					size = state_size*number;
					data = malloc(size);
					for(i = 0; i < number; i++)
					{
						state = g_jd_state_cz_list[i].Dev_State;
						memcpy(data+(i*state_size),&state,state_size);
					}
					break;
					
				case JD_DEV_GAS:
					number = MAX_AURINE_JD_MQ_NUM;
					size = state_size*number;
					data = malloc(size);
					for(i = 0; i < number; i++)
					{
						state = g_jd_state_mq_list[i].Dev_State;
						memcpy(data+(i*state_size),&state,state_size);
					}
					break;
			}
		}
		else
		{
			size = state_size;
			data = malloc(size);
			switch(devtype)
			{
				case JD_DEV_LIGHT:
					state = g_jd_state_cz_list[index].Dev_State;
					memcpy(data,&state,state_size);
					break;
					
				case JD_DEV_KONGTIAO:
					state = g_jd_state_cz_list[index].Dev_State;
					memcpy(data,&state,state_size);
					break;
					
				case JD_DEV_WINDOW:
					state = g_jd_state_cz_list[index].Dev_State;
					memcpy(data,&state,state_size);
					break;
					
				case JD_DEV_POWER:
					state = g_jd_state_cz_list[index].Dev_State;
					memcpy(data,&state,state_size);
					break;
					
				case JD_DEV_GAS:
					state = g_jd_state_cz_list[index].Dev_State;
					memcpy(data,&state,state_size);
					break;
			}
		}
	}
	infodata = data;
	free(data);
	return size;
}

/*************************************************
  Function:		jd_aurine_init_statelist
  Description: 	
  Input: 		
  Return:		��  
*************************************************/
static void jd_aurine_init_statelist(AU_JD_DEV_TYPE devtype,AU_JD_STATE_LIST *statelist)
{
	int i,num;
	switch(devtype)
	{
		case JD_DEV_LIGHT:
			num = MAX_AURINE_JD_DG_NUM;
			break;
		case JD_DEV_KONGTIAO:
			num = MAX_AURINE_JD_KT_NUM;
			break;
		case JD_DEV_WINDOW:
			num = MAX_AURINE_JD_CL_NUM;
			break;
		case JD_DEV_POWER:
			num = MAX_AURINE_JD_CZ_NUM;
			break;
		case JD_DEV_GAS:
			num = MAX_AURINE_JD_MQ_NUM;
			break;
	}
	for (i=0;i<num;i++)
	{
		statelist[i].Address =0;
		statelist[i].Ischange =0;
		statelist[i].IsUsed = 0;
		statelist[i].Dev_State.DevID =i;
		statelist[i].Dev_State.Active =0;
		statelist[i].Dev_State.ActiveParam =0;
		statelist[i].Dev_State.DevType = devtype;
	}
}

/*************************************************
  Function:		jd_aurine_init
  Description: 	
  Input: 		
  Return:		��  
*************************************************/
void jd_set_state_list(AU_JD_DEV_TYPE devtype, PJD_DEV_LIST_INFO statelist)
{
    uint8 i, index;
	
    for(i = 0; i < statelist->nCount; i++)
	{
	    if (devtype == JD_DEV_LIGHT)
		{  
			g_jd_state_dg_list[i].Index = statelist->pjd_dev_info[i].Index;
		    g_jd_state_dg_list[i].Address = statelist->pjd_dev_info[i].Address;
		    g_jd_state_dg_list[i].IsUsed = statelist->pjd_dev_info[i].IsUsed;
		    g_jd_state_dg_list[i].IsTune = statelist->pjd_dev_info[i].IsTune;
		 }
		 else if (devtype == JD_DEV_WINDOW)
		 {
		 	g_jd_state_cl_list[i].Index = statelist->pjd_dev_info[i].Index;
		    g_jd_state_cl_list[i].Address = statelist->pjd_dev_info[i].Address;
		    g_jd_state_cl_list[i].IsUsed = statelist->pjd_dev_info[i].IsUsed;
		    g_jd_state_cl_list[i].IsTune = statelist->pjd_dev_info[i].IsTune;
		 }
		 else if (devtype == JD_DEV_KONGTIAO)
		 {
		 	g_jd_state_kt_list[i].Index =  statelist->pjd_dev_info[i].Index;
		    g_jd_state_kt_list[i].Address =  statelist->pjd_dev_info[i].Address;
		    g_jd_state_kt_list[i].IsUsed = statelist->pjd_dev_info[i].IsUsed;
		    g_jd_state_kt_list[i].IsTune = statelist->pjd_dev_info[i].IsTune;
		 }
		 else if (devtype == JD_DEV_POWER)
		 {
		 	g_jd_state_cz_list[i].Index = statelist->pjd_dev_info[i].Index;
		    g_jd_state_cz_list[i].Address = statelist->pjd_dev_info[i].Address;
		    g_jd_state_cz_list[i].IsUsed = statelist->pjd_dev_info[i].IsUsed;
		    g_jd_state_cz_list[i].IsTune = statelist->pjd_dev_info[i].IsTune;
		 }
		 else if (devtype == JD_DEV_GAS)
		 {
		 	g_jd_state_mq_list[i].Index = statelist->pjd_dev_info[i].Index;
		    g_jd_state_mq_list[i].Address = statelist->pjd_dev_info[i].Address;
		    g_jd_state_mq_list[i].IsUsed = statelist->pjd_dev_info[i].IsUsed;
		    g_jd_state_mq_list[i].IsTune = statelist->pjd_dev_info[i].IsTune;
		 }
	}
}

/*************************************************
  Function:		jd_aurine_load_devinfo_from_db
  Description: 	
  	�Ӽҵ����ݱ��л�ȡ�ҵ������Ϣ  ��
  	�������豸״̬�б���
  Input: 		
  Return:		��  
*************************************************/
static void jd_aurine_load_devinfo(AU_JD_DEV_TYPE devtype,AU_JD_STATE_LIST *statelist)
{
	int i, max;
	int index;
	PJD_DEV_LIST_INFO pdevInfolist = NULL;
	
	max = get_dev_max(devtype);
	malloc_jd_memory(&pdevInfolist, max);
	pdevInfolist = storage_get_jddev((AU_JD_DEV_TYPE)devtype, 1);
	if (pdevInfolist == NULL)
	{
		return;
	}
	for (i = 0; i < pdevInfolist->nCount; i++)
	{
		statelist[i].Address =  pdevInfolist->pjd_dev_info[i].Address;
		statelist[i].IsUsed = pdevInfolist->pjd_dev_info[i].IsUsed;
		statelist[i].IsTune = pdevInfolist->pjd_dev_info[i].IsTune;
	}
	free_jd_memory(&pdevInfolist);
}

/*************************************************
  Function:		jd_load_devinfo
  Description: 	�����豸��Ϣ
  Input: 		
  Return:		��  
*************************************************/
void jd_load_devinfo(void)
{
     // �����б�
	jd_aurine_load_devinfo(JD_DEV_LIGHT,  g_jd_state_dg_list);
	jd_aurine_load_devinfo(JD_DEV_WINDOW, g_jd_state_cl_list);
	jd_aurine_load_devinfo(JD_DEV_KONGTIAO, g_jd_state_kt_list);
	jd_aurine_load_devinfo(JD_DEV_POWER, g_jd_state_cz_list);
	jd_aurine_load_devinfo(JD_DEV_GAS, g_jd_state_mq_list);
}

/*************************************************
  Function:		jd_aurine_init
  Description: 	
  Input: 		
  Return:		��  
*************************************************/
void jd_aurine_init(void)
{
    // ��ʼ���б�
    jd_aurine_init_statelist(JD_DEV_LIGHT,  g_jd_state_dg_list);
    jd_aurine_init_statelist(JD_DEV_WINDOW, g_jd_state_cl_list);
    jd_aurine_init_statelist(JD_DEV_KONGTIAO, g_jd_state_kt_list);
    jd_aurine_init_statelist(JD_DEV_POWER, g_jd_state_cz_list);
    jd_aurine_init_statelist(JD_DEV_GAS, g_jd_state_mq_list);

    // �����б�
    jd_load_devinfo();
}
#endif

#if 0
/*************************************************
  Function:    	jd_data_callback
  Description: 	�ҵ����ݴ���ص��ҵ����	
  Input:
  	1.id		��ϢID		
  	2.data		��������
  Output: 			��   		
  Return:			��
  Others:           
*************************************************/
uint8 jd_data_callback(SYS_MSG_ID id, uint32 * data)
{
    uint8 ret = 0;
	
    switch (id)
	{
		case AS_UART0_REV_DATA:
		{
		    #ifdef _AURINE_ELEC_NEW_
            jd_aurine_responsion();
            #endif
        }
        break;

        default:
            break;
    }
    dprintf("jd_data_callback \n", jd_data_callback);
}

/*************************************************
  Function:		init_jiadian_callback
  Description:	��ʼ���ҵ�ģ��ַ��ص�����
  Input: 
	1.func		���ݴ�����ָ��
	2.timeout	��ʱ������ָ��
  Output:		��
  Return:		��
  Others:
*************************************************/
int32 init_jiadian_callback(JD_REPORT_CALLBACK func)
{
	JiaDianHook = func;	
}
#endif

/*************************************************
  Function:		jd_stop_feet_dog
  Description:  �ҵ�ģ����ֹͣ���Ź�
  Input:
  	1.buf		����
  	2.len		����
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_stop_feet_dog(uint8 * buf, uint8 len)
{
	add_send_node(JD_CMD_COTROL, buf, len);
}

/*************************************************
  Function:		start_jd_loop
  Description: 	�����ҵ��շ������߳�
  Input: 		��
  Output:		��
  Return:		0: �ɹ� ����ֵ: ʧ��
  Others:
*************************************************/
static int32 start_jd_loop(void)
{
	pthread_t th;
	pthread_attr_t new_attr;

	pthread_attr_init(&new_attr);
	pthread_attr_setdetachstate(&new_attr, PTHREAD_CREATE_DETACHED);
	//pthread_attr_setschedpolicy(&new_attr, SCHED_FIFO);
	pthread_attr_setscope(&new_attr, PTHREAD_SCOPE_SYSTEM);
	//pthread_attr_setstacksize(&new_attr, 48*1024);
	pthread_create(&th, &new_attr, jd_loop_thread, NULL);
	
	return 0;
}

/*************************************************
  Function:		jd_logic_init
  Description:  �ҵ��߼���ʼ��
  Input: 
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_logic_init(void)
{
	uint8 factory;

	#if 0
	if (FALSE == storage_get_extmode(EXT_MODE_JD))
	{
		return FALSE;
	}
	#endif
	
	nextsendcmd = 0;
	if (g_InitFlag == 0)
	{
		g_InitFlag = 1;
		sendbuf_sem_init();
		recvbuf_sem_init();
		init_loop_send_node(g_LoopBuf, sizeof(g_LoopBuf));
		//TimeId = add_aurine_realtimer(AURINE_COMMTIME_NUM, AurineCommTimeBack, NULL);
		start_jd_loop();
		#ifndef WIN32
		init_jd_callbackt(jiadian_485_responsion);
		#endif
	}
	
	factory = storage_get_extmode(EXT_MODE_JD_FACTORY);
	if (factory == JD_FACTORY_ACBUS)
	{
	    factory = factory + 1;
	}
	set_device_logo(factory);
}
#endif

