/*************************************************
  Copyright (C), 2006-2016, Aurine
  File name: 	logic_message.c
  Author:    	luofl
  Version:   	2.0
  Date: 		2014-11-06
  Description:	��Ϣ�����߼�
  				���ķ���ס���ĳ�Ϊ��Ϣ�����Ͱ������֡�ͼƬ����������ϣ�
  				�û�ͨ���ֻ�����ס���ĳ�Ϊ����, ����Ϊ������, һ�β�����70�����֡�
  History:                    
    1. Date:		 
       Author:		 
       Modification: 
    2. ...
*********************************************************/
#include "logic_include.h"


#define REC_SHORT_MSG_SIZE		(8 + 1 + 20 + 200)

/*************************************************
  Function:			save_msg
  Description: 		������Ϣ(�����ķ���)
  Input: 			
  	1.ID			�ỰID
  	2.data			����
  	3.size			��С
  Output:			��
  Return:			��
*************************************************/
static void save_msg(uint32 ID, int8 * data, int size)
{
	uint32 Len;	
	REC_BULLETIN_HEAD 	rec_bulletin_head;			// ��Ϣͷ	
	char des[14] = {0xff,0xff};
	char tmp_des[14] = {0};
	static uint32 sta_id = 0;
	
	if (NULL == data || 0 == size)
	{
		return;
	}
	memset(&rec_bulletin_head, 0, sizeof(REC_BULLETIN_HEAD));

	// ��ϢID��
	memcpy(&rec_bulletin_head.ID, data, 4);
	if (sta_id == rec_bulletin_head.ID)
	{
		dprintf("sta_id == rec_bulletin_head.ID: %d \n", sta_id);
		return;
	}
	sta_id = rec_bulletin_head.ID;
	dprintf("rec_bulletin_head.ID : %d\n", rec_bulletin_head.ID);

	// ����
	rec_bulletin_head.BulletinType = *(int8 *)((int8*)data + 4);
	dprintf("rec_bulletin_head.BulletinType : %d\n", rec_bulletin_head.BulletinType);
	
	// ����
	memcpy(rec_bulletin_head.title, (int8*)data + 4 + 4, 40);
	//dprintf("rec_bulletin_head.title : %s\n", rec_bulletin_head.title);
	
	// ���ֲ��ֳ���
	memcpy(&Len, (int8*)data+ 4 + 4 + 40, 4);	
	rec_bulletin_head.CharLen = Len;
	//dprintf("rec_bulletin_head.CharLen : %d\n", rec_bulletin_head.CharLen);
	
	// ͼƬ���ֳ���
	memcpy(&Len, (int8*)data+ 4 + 4 + 40 + 4, 4);
	rec_bulletin_head.ImageLen = Len;
    //dprintf("rec_bulletin_head.ImageLen : %d\n", rec_bulletin_head.ImageLen);

	// �������ֳ���
	memcpy(&Len, (int8*)data+ 4 + 4 + 40 + 4 + 4, 4);
	rec_bulletin_head.VoiceLen = Len;
	
	// �����ֲ���
	#if 0
	GuiDspGetIDString(SID_Info_Centre_Des, tmp_des, 10);
	Unicode2GBKtemp(tmp_des, des);
	#endif
	//dprintf("save_msg end \n");
	storage_write_msg(&rec_bulletin_head, (char*)data + sizeof(REC_BULLETIN_HEAD), des);
	show_sys_event_hint(AS_NEW_MSG);
	sys_sync_hint_state();
}

/*************************************************
  Function:			save_short_msg
  Description: 		�������(�û���������,������ת����ס��)
  Input: 			
  	1.rec_short_msg	���Žṹ
  Output:			��
  Return:			��
*************************************************/
static void save_short_msg(REC_SHORT_MSG *rec_short_msg)
{
	uint8 des[14];
	REC_BULLETIN_HEAD 	rec_bulletin_head;
	
	if (NULL == rec_short_msg)
	{
		return;
	}
	
	memset(&rec_bulletin_head, 0, sizeof(REC_BULLETIN_HEAD));
	rec_bulletin_head.CharLen = strlen(rec_short_msg->Content);
	memset(des, 0, sizeof(des));
	// �����ֲ���
	rec_bulletin_head.title[0] = 0xff;
	#if 0
	GuiDspGetIDString(SID_Info_Sms, des, 6);
	Unicode2GBKtemp(des, rec_bulletin_head.title);
	#endif
	memset(des, 0, sizeof(des));
	rec_bulletin_head.BulletinType = 0;
	memcpy(des, rec_short_msg->SendNo, 14);
	//Gbk2Unicode(des, rec_short_msg->SendNo);

	dprintf("des: %s, rec_short_msg->Content: %s,rec_short_msg->SendNo: %s\n", des, rec_short_msg->Content, rec_short_msg->SendNo);
	storage_write_msg(&rec_bulletin_head, rec_short_msg->Content, des);
	show_sys_event_hint(AS_NEW_MSG);
	sys_sync_hint_state();
}

/*************************************************
  Function:			update_msg_recv_state
  Description: 		������Ϣ��״̬
  Input: 			
  	1.ID			�ỰID
  	2.Total			�ܰ���
  	3.CurNum		��ǰ����
  	4.size			��ǰ�������ݴ�С
  Output:			��
  Return:			��
*************************************************/
static void* update_msg_recv_state(uint32 ID, int Total, int CurNum, int size)
{	
	if (Total == CurNum)
	{
		//set_public_state(PUB_STATE_XINXI, 1);
	}
}

/*************************************************
  Function:			recv_msg_start
  Description:		������Ϣ��ʼ(���ö������)
  Input: 	
  	1.recPacket		���հ�
  Output:			��
  Return:			��
  Others:
*************************************************/
static void recv_msg_start(const PRECIVE_PACKET recPacket)
{
	uint32 ID = 0;									// �ỰID
	char * data = (char*)&ID;						// �ỰID
	ID = net_start_multipacket_recive(recPacket->address, recPacket->port, 0, save_msg, update_msg_recv_state);
	if (ID == 0)
	{
		//dprintf("net_start_multipacket_recive = 0\n");
		net_send_echo_packet(recPacket, ECHO_BUSY, data, 4);	
		return;
	} 
	//dprintf("net_start_multipacket_recive = id:%d\n", ID);
	net_send_echo_packet(recPacket, ECHO_OK, data, 4);
	return;
}

/*************************************************
  Function:			msg_distribute
  Description:		���ն��ŷ�������Ӧ������
  Input:
  	1.packet		���հ�
  Output:			��
  Return:			�Ϸ�����TRUE, �Ƿ�����FALSE
  Others:
**************************************************/
int msg_distribute(const PRECIVE_PACKET recPacket)
{
	char net_cmd[2];								// ����ת����������
	REC_SHORT_MSG 		rec_short_msg;				// ���ܶ��Žṹ
	uint32 				validate_code;				// ��֤��
	
	PNET_HEAD head = (PNET_HEAD)(recPacket->data + MAIN_NET_HEAD_SIZE);	// ������
	int cmd = head->command | SSC_INFO << 8;
	net_cmd[0] = SSC_INFO;							// ��������(��ϵͳ��+����)
	net_cmd[1] = head->command;
	
	dprintf("msg_distribute cmd: %x \n", cmd);
	switch (cmd)
	{
		case CMD_SEND_INFO:							// ������Ϣ				
			recv_msg_start(recPacket);
			break;				
			
		case CMD_SEND_SHORT_MSG:					// ���ŷ��ͣ���Ӧ��			
			memcpy(&rec_short_msg, recPacket->data + NET_HEAD_SIZE, REC_SHORT_MSG_SIZE);
			net_send_echo_packet(recPacket, ECHO_OK, NULL, 0);	
			save_short_msg(&rec_short_msg);
			break;	
			
		case CMD_SEND_MSG_VALIDATE:					// ��Ϣ������֤�룬��Ӧ��
			memcpy(&validate_code, recPacket->data + NET_HEAD_SIZE, 4);
			dprintf("validate_code : 0x%x\n", validate_code);
			net_send_echo_packet(recPacket, ECHO_OK, NULL,0);
			//save_validate_code(validate_code);
			storage_set_validate_code(validate_code);
			break;	
			
		default:
			return FALSE;							// ��Ч����
	}
	
	return TRUE;
}

/*************************************************
  Function:			msg_responsion
  Description:		���ն���Ӧ������Ӧ������
  Input:
  	1.recPacket		������հ�
  	2.SendPacket	���緢�Ͱ�
  Output:			��
  Return:			��
  Others:
**************************************************/
void  msg_responsion(const PRECIVE_PACKET recPacket, const PSEND_PACKET SendPacket)
{
	return;
}

