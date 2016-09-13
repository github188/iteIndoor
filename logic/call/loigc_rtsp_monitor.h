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

#define PTZ_START 			0						// ��ʼ����
#define PTZ_END				1						// ��������
#define ZOOM_IN				11						// �������ٶ�SS ���(���ʱ��)
#define ZOOM_OUT			12						// �������ٶ�SS ��С(���ʱ�С)
#define TILT_UP				21						// ��̨��SS ���ٶ�����
#define TILT_DOWN			22						// ��̨��SS ���ٶ��¸�
#define PAN_LEFT			23						// ��̨��SS ���ٶ���ת
#define TILT_RIGHT			24						// ��̨��SS ���ٶ���ת
#define GOTO_PRESET 		39 						// ����ת��Ԥ�õ�


/*************************************************
  Function:		rtsp_monitor_sync_devlist
  Description: 	��ȡRTSP�豸�б�
  Input: 		��
  Output:		��
  Return:		PMonitorDeviceList �豸�б�
  Others:		
*************************************************/
uint32 rtsp_monitor_sync_devlist (void);

/*************************************************
  Function:			monitor_start
  Description:		��ʼ����
  Input: 	
  	1.ID			Ӧ��ID
  	2.DevType		�豸����
  	3.index			�豸����
  Output:			��
  Return:			�ɹ����, TRUE / FALSE
  Others:
*************************************************/
int32 rtsp_monitor_start(uint32 index, uint32 type);

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
  Function:			get_ipc_state
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
  Function:			set_control_ptz
  Description:		��̨����ͽ������
  Input:			
  	1.cmd			��̨��������
  	2.action 		0 �������� 1 ֹͣ����
  Output:			��
  Return:			��
  Others:			��
*************************************************/
void set_control_ptz(uint8 cmd, uint8 action);

/*************************************************
  Function:			preset_control_ptz
  Description:		��̨Ԥ�õ����
  Input:			
  	1.index			Ԥ�õ�����
  Output:			��
  Return:			��
  Others:			��
*************************************************/
void preset_control_ptz(uint8 index);

/*************************************************
  Function:				rtsp_ini
  Description:			rtsp��ʼ��
  Input:
  	1.RtspListProc		list�ص�����
  	1.GuiProc			GUI�ص�����
  Output:				��
  Return:				
  Others:
*************************************************/
void rtsp_ini(PFGuiNotify RtspListProc, PFGuiNotify GuiProc);


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
