/*************************************************
  Copyright (C), 2006-2016, Aurine
  File name:  	logic_monitor.h
  Author:     	chenbh
  Version:    	2.0
  Date: 		2014-12-11
  Description:  ����ͷ�ļ�
  
  History:        
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#ifndef _LOGIC_MONITOR_H_
#define _LOGIC_MONITOR_H_
#include "logic_include.h"

#define MONITOR_TEMP			0XFF
// ���ӷ���ֵ
typedef enum
{	 
	MONITOR_OK,										// ��������
	MONITOR_ERR,									// һ�����
	MONITOR_ERR_ID,									// �豸��������
	MONITOR_SEARCH_ERR,								// �����豸ʧ��
	MONITOR_REQUEST_TIMEOUT,						// ���ӳ�ʱ
	MONITOR_MONITORING_TIMEOUT,
	MONITOR_TALKING_TIMEOUT,
	MONITOR_HEART_TIMEOUT,							// ������ʱ
	MONITOR_BUSY									// �Է�æ
}MONITOR_VALUE_E;

// ����״̬
typedef enum
{
	MONITOR_END,									// ���ӽ���
	MONITOR_SEARCH,									// �����豸��
	MONITOR_REQUEST,								// ������
	MONITOR_MONITORING,								// ������
	MONITOR_TALKING,								// ͨ����
	MONITOR_GETLIST,								// ��ȡ�б�
}MONITOR_STATE_E;

#define MONITOR_TIMER		(MONITOR_END+10) 		// ���Ӽ�ʱ
#define MONITOR_SNAP		(MONITOR_END+11) 		// ����ץ��

/*************************************************
  Function:			monitor_request
  Description:		��������
  Input: 	
   	1.ID			Ӧ��ID
  	2.DevType		�豸����
  Output:			��
  Return:			�ɹ����, TRUE / FALSE
  Others:
*************************************************/
int32 monitor_start(DEVICE_TYPE_E DevType, int8 index);

#define MONITOR_FAX_IP 1

#if MONITOR_FAX_IP
/*************************************************
  Function:			monitor_request
  Description:		��������
  Input: 	
   	1.ID			Ӧ��ID
  	2.DevType		�豸����
  Output:			��
  Return:			�ɹ����, TRUE / FALSE
  Others:
*************************************************/
int32 test_monitor_start(void);
#endif

/*************************************************
  Function:			monitor_stop
  Description:		�������ӻ�ͨ��
  Input: 	
  Output:			��
  Return:			�ɹ����, TRUE / FALSE
  Others:
*************************************************/
int32 monitor_stop(void);

/*************************************************
  Function:				monitor_ini
  Description:			���ӳ�ʼ��
  Input: 	
  	1.GuiProc			GUI�ص�����
  Output:				��
  Return:				
  Others:
*************************************************/
void monitor_ini(PFGuiNotify MonitorListProc, PFGuiNotify GuiProc);

/*************************************************
  Function:			monitor_distribute
  Description:		��������Ͱ�������
  Input: 	
  	1.recPacket		���������
  Output:			��
  Return:			�ɹ����,true/fasle
  Others:
*************************************************/
int32 monitor_distribute(const PRECIVE_PACKET recPacket);

/*************************************************
  Function:			monitor_responsion
  Description:		��������Ӧ���������
  Input: 	
  	1.recPacket		Ӧ���
  	2.SendPacket	���Ͱ�
  Output:			��
  Return:			��
  Others:
*************************************************/
void monitor_responsion(const PRECIVE_PACKET recPacket, const PSEND_PACKET SendPacket);

/*************************************************
  Function:			monitor_talk
  Description:		����תͨ��
  Input: 	
  Output:			��
  Return:			�ɹ����, TRUE / FALSE
  Others:
*************************************************/
int32 monitor_talk(void);

/*************************************************
  Function:			monitor_unlock
  Description:		���ڻ����ӿ���
  Input: 			��
  Output:			��
  Return:			�ɹ����
  Others:
*************************************************/
int32 monitor_unlock(void);

/*************************************************
  Function:			monitor_next
  Description:		�����л�
  Input: 	
  	1.direct		����: TRUE-��һ�� FALSE-��һ��
  Output:			��
  Return:			�ɹ����, TRUE / FALSE
  Others:
*************************************************/
int32 monitor_next(int32 direct);

/*************************************************
  Function:			monitor_video_snap
  Description:		ץ��
  Input: 	
  Output:			��
  Return:			�ɹ����, TRUE/FALSE
  Others:	
*************************************************/
int32 monitor_video_snap(void);

/*************************************************
Function:			ui_monitor_stop
Description:		�������ӻ�ͨ�� UI���һ����øýӿ�
Input:
Output:			��
Return:			�ɹ����, TRUE / FALSE
Others:
*************************************************/
int32 ui_monitor_stop(void);

/*************************************************
  Function:		rtsp_monitor_sync_devlist
  Description: 	��ȡRTSP�豸�б�
  Input: 		��
  Output:		��
  Return:		PMonitorDeviceList �豸�б�
  Others:		
*************************************************/
uint32 monitorlist_sync_devlist(DEVICE_TYPE_E DevType);

/*************************************************
  Function:		fill_devno_by_index
  Description: 	����豸���
  Input: 		
  	1.DevType   �豸����
  	2.index		�豸����
  Output:		��
  Return:		
  Others:		
*************************************************/
void fill_devno_by_index(DEVICE_TYPE_E DevType, int8 index, char *devno);

#endif

