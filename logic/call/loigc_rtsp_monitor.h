/*************************************************
  Copyright (C), 2006-2010, Aurine
  File name:  	loigc_rtsp_monitor.h
  Author:     	xiewr
  Version:    	1.0
  Date: 
  Description:  ����ͷ�ļ�
  
  History:        
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#ifndef _LOGIC_RTSP_MONITOR_H_
#define _LOGIC_RTSP_MONITOR_H_


// RTSP ����ID
#define AURINE_STR			"AURINE"	
#define HIKVISION_STR		"HIKVISION"	
#define DAHUA_STR			"DAHUA"	
#define SANLI_STR			"SANLI"

// RTSP ��Ƶ����
#define FMT_DEFAULT			0
#define H264				1
#define MPEG				2

#define H264_STR			"h264"
#define MPEG_STR			"mpeg4"

// RTSP ��Ƶ��ʽ
#define DEC_DEFAULT			0
#define QCIF				1
#define QVGA				2
#define CIF					3
#define D1					4
#define HalfD1				5
#define VGA                 6
#define _720P               7

#define QCIF_STR			"QCIF"
#define QVGA_STR			"QVGA"
#define CIF_STR				"CIF"
#define D1_STR				"D1"
#define HalfD1_STR			"HalfD1"
#define VGA_STR				"VGA"
#define _720P_STR			"720P"

/*************************************************
  Function:		rtsp_monitor_state
  Description: 	
  Input: 		��
  Output:		��
  Return:		
  Others:		
*************************************************/
uint32 rtsp_monitor_state (void);

/*************************************************
  Function:			monitor_stop
  Description:		�������ӻ�ͨ��
  Input: 	
  Output:			��
  Return:			�ɹ����, TRUE / FALSE
  Others:
*************************************************/
int32 rtsp_monitor_stop(void);

#ifdef _NEW_SELF_IPC_
/*************************************************
  Function:			init_ipc_state
  Description:		
  Input: 	
  Output:			��
  Return:			
  Others:			ipc״̬��ʼ�� Ĭ������״̬
*************************************************/
void init_ipc_state(void);

/*************************************************
  Function:			ipc_ontimer
  Description:		
  Input: 	
  Output:			��
  Return:			
  Others:			ipc״̬��ʱ��
*************************************************/
void ipc_ontimer(void);

/*************************************************
  Function:			ipc_ontimer
  Description:		
  Input: 	
  	index			�豸������� 0-9
  Output:			��
  Return:			0 ���� 1 ����
  Others:			���IPC����״̬
*************************************************/
uint8 get_ipc_state(uint8 index);

/*************************************************
  Function:			set_ipc_online
  Description:		
  Input: 	
  	index			�豸������� 0-9
  Output:			��
  Return:			
  Others:			����IPC����״̬
*************************************************/
void set_ipc_online(uint8 index, uint32 IP);

/*************************************************
  Function:			ipc_ontimer
  Description:		
  Input: 	
  	index			�豸������� 0-9
  Output:			��
  Return:			
  Others:			ipc״̬��ʱ��
*************************************************/
void set_ipc_outline(uint8 index);
#endif

/*************************************************
  Function:			rtsp_distribute
  Description:		
  Input: 	
  	1.recPacket		���հ�
  Output:			��
  Return:			�ɹ����, TRUE/FALSE
  Others:	
*************************************************/
int32 rtsp_distribute(const PRECIVE_PACKET recPacket);

/*************************************************
  Function:			rtsp_responsion
  Description:		
  Input: 	
  	1.recPacket		Ӧ���
  	2.SendPacket	���Ͱ�
  Output:			��
  Return:			��
  Others:
*************************************************/
void rtsp_responsion(const PRECIVE_PACKET recPacket, const PSEND_PACKET SendPacket);
#endif
