/*************************************************
  Copyright (C), 2006-2010, Aurine
  File name:  	logic_interphone.h
  Author:     	xiewr
  Version:    	1.0
  Date: 
  Description:  ����ַ�ģ��ͷ�ļ�
  
  History:        
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#ifndef _LOGIC_INTERPHONE_H_
#define _LOGIC_INTERPHONE_H_

#include "logic_include.h"

#define CALL_AUDIO_PT			PT_G711A			// Ĭ�Ϻ�����Ƶ��ʽ
#define CALL_VIDEO_PT			PT_H264				// Ĭ�Ϻ�����Ƶ��ʽ

#define CALL_RTP_AUDIO_PT		8					// RTP��Ƶ����
#define CALL_RTP_VIDEO_PT		98					// RTP��Ƶ����

#ifdef _TFT_7_
#define SNAP_PIC_WIDTH			640
#define SNAP_PIC_HEIGHT			480
#else
#define SNAP_PIC_WIDTH			380
#define SNAP_PIC_HEIGHT			272
#endif

#if 0
#define	INTER_TPRI				TPRI_NORMAL		// �Խ������߳����ȼ�
#define	INTER_MB				MBGUI			// �Խ������߳�����
#endif

// ��Ƶ����Ƶ����ģʽ
typedef enum
{
    _NONE 		= 0x00,
	_SENDONLY	= 0x01,
	_RECVONLY	= 0x02,
	_SENDRECV	= 0x03
}INTER_MEDIA_SEND_STATE;

// Ӧ����
typedef enum
{
	TRC_UNKNOWN		=	0,							// δ֪
	TRC_TRYING		=	100,						// �����Ѿ����գ���ȴ� 
	TRC_RINGING		=	180,						// �Ѿ�����
	TRC_QUEUED		=	182,						// ���н���ȴ�����
	TRC_OK			=	200,						// ����ȷ��
	TRC_MOVE_TEMP	=	302,						// ��ʱת��
	TRC_NOT_FOUND	=	404,						// �Ҳ������û�
	TRC_BUSY		=	486,						// æ 
	TRC_TERMINATED	=	487 						// ������ֹ 
}INTER_RESPONSE_CODE_E;

/*************************************************
  Function:			is_fenji1_DeviceNo
  Description:		�ж��Ƿ�Ϊ�����ڷֻ�
  Input: 			��
  Output:			��
  Return:			�ɹ����
  Others:			�ֻ���Ϊ0 ��������
*************************************************/
uint8 is_fenji1_DeviceNo(void);

/*************************************************
  Function:			is_main_DeviceNo
  Description:		�ж��Ƿ�Ϊ�����ڷֻ�
  Input: 			��
  Output:			��
  Return:			�ɹ����
  Others:			�ֻ���Ϊ0 ��������
*************************************************/
uint8 is_main_DeviceNo(void);

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
char * get_stair_fullno(char * StairNo, char ExtNo);

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
void set_stair_address(int32 no, uint32 address);

/*************************************************
  Function:			get_stair_address
  Description:		���б��л���ݿڻ���ַ,δ�ҵ����㲥��ȡ
  Input: 			
  	1.no			��� 0-9, 0xFFΪ����
  Output:			��
  Return:			��ַ
  Others:
*************************************************/
uint32 get_stair_address_inlist(int32 no);

/*************************************************
  Function:			get_stair_address
  Description:		���ָ���ݿڻ���ַ
  Input: 			
  	1.no			��� 0-9, 0xFFΪ����
  Output:			��
  Return:			��ַ
  Others:
*************************************************/
uint32 get_stair_address(int32 no);

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
int32 get_stair_list(uint32 *index, uint32 *ipaddr);

/*************************************************
  Function:			set_stair0_addr
  Description:		����ݿڻ�0��ַ
  Input: 			��
  Output:			
  Return:			IPaddr
  Others:
*************************************************/
void set_stair0_addr(void);

/*************************************************
  Function:			get_stair0_addr
  Description:		����ݿڻ�0��ַ
  Input: 			��
  Output:			
  Return:			IPaddr
  Others:
*************************************************/
uint32 get_stair0_addr(void);

/*************************************************
  Function:			get_door_address
  Description:		�����ǰ����ַ
  Input: 			
  	1.ID			��� 1��2 
  Output:			��
  Return:			�ɹ�����ַ��ʧ�ܣ�0
  Others:			
*************************************************/
uint32 get_door_address(int32 ID);

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
int32 get_netdoor_list(uint32 *index, uint32 *ipaddr);

/*************************************************
  Function:			get_netdoor_address_inlist
  Description:		�����ǰ����ַ
  Input: 			
  	1.no			��� 1-2
  Output:			��
  Return:			��ַ
  Others:
*************************************************/
uint32 get_netdoor_address_inlist(int32 no);

/*************************************************
  Function:			get_phonedoor_address_inlist
  Description:		�����ǰ����ַ
  Input: 			
  	1.no			��� 1-2
  Output:			��
  Return:			��ַ
  Others:
*************************************************/
uint32 get_phonedoor_address_inlist(int32 no);

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
void set_area_address(int32 no, uint32 address);

/*************************************************
  Function:			get_stair_address
  Description:		������ڻ���ַ,δ�ҵ����㲥��ȡ
  Input: 			
  	1.no			��� 1-40
  Output:			��
  Return:			��ַ
  Others:
*************************************************/
uint32 get_area_address_inlist(int32 no);

/*************************************************
  Function:			get_area_address
  Description:		���ָ�����ڻ���ַ
  Input: 			
  	1.no			���
  Output:			��
  Return:			��ַ
  Others:
*************************************************/
uint32 get_area_address(int32 no);

/*************************************************
  Function:			search_stair_list
  Description:		����ݿڻ���ַ�б�
  Input: 			��
  Output:			
  Return:			�Ƿ��ȡ���豸IP: TRUE / FALSE
  Others:
*************************************************/
int32 search_stair_list(void);

/*************************************************
  Function:			search_area_list
  Description:		������ڻ���ַ�б�
  Input: 			��
  Output:			
  Return:			TRUE / FALSE
  Others:
*************************************************/
int32 search_area_list(void);

/*************************************************
  Function:			search_door_list
  Description:		���������ǰ����ַ�б�
  Input: 			��
  Output:			
  Return:			TRUE / FALSE
  Others:
*************************************************/
int32 search_door_list(void);

/*************************************************
  Function:			get_phonedoor_address
  Description:		�����ǰ����ַ
  Input: 			
  	1.ID			��� 1��2 
  Output:			��
  Return:			�ɹ�����ַ��ʧ�ܣ�0
  Others:			
*************************************************/
uint32 get_phonedoor_address(int32 ID);

/*************************************************
  Function:			init_list_address
  Description:		��ʼ���豸�б�
  Input: 	
  Output:			��
  Return:			�ɹ����, TRUE/FALSE
  Others:	
*************************************************/
int32 init_list_address(void);

/*************************************************
  Function:			inter_distribute
  Description:		�Խ����շ��Ͱ��ص�����
  Input: 	
  	1.recPacket		���հ�
  Output:			��
  Return:			�ɹ����, TRUE/FALSE
  Others:	
*************************************************/
int32 inter_distribute(const PRECIVE_PACKET recPacket);

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
void inter_responsion(const PRECIVE_PACKET recPacket, const PSEND_PACKET SendPacket);
#endif

