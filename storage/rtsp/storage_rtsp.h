/*********************************************************
  Copyright (C), 2009-2012
  File name:	storage_rtsp.h
  Author:   	������
  Version:   	1.0
  Date: 		09.4.7
  Description:  
  History:                   
    1. Date:
       Author:
       Modification:
    2. ...
*********************************************************/
#ifndef __STORAGE_RTSP_H__
#define __STORAGE_RTSP_H__

#include "storage_include.h"
//#include "storage_types.h"

#define MAX_HOME_NUM  			16					

#define HOME_CAMERA				0
#define COMMUNITY_CAMERA		1

typedef enum
{
    RTSP_IP = 0x00,		
	RTSP_PORT,	
	RTSP_CHANNEL,
	RTSP_USER,
	RTSP_PASSWORD,
	
	RTSP_MAX
}RTSP_TYPE;				

typedef struct
{
	uint32 index;									// IPC ��ţ�0��9
	uint32 address;									// IPC ��ַ
} SelfIPCamera;										// ���� IPC ��Ϣ�ṹ

typedef struct 
{
   	char DeviceName[32];							// �豸����	
	uint32 DeviceIP;								// �豸IP		
	uint16 DevPort;									// �豸�˿�
	uint16 ChannelNumber;							// ͨ������
	char FactoryName[12];               			// �豸��������
	uint32 CanControlPTZ;		                    // ��̨�Ƿ����
}COMMUNITYDEVICE,*PCOMMUNITYDEVICE;

//�����������Ϣ�ṹ
typedef struct 
{
   	char DeviceName[32];							// �豸����	
	uint32 DeviceIP;								// �豸IP		
	uint16 DevPort;									// �豸�˿�
	uint16 ChannelNumber;							// ͨ������
	char FactoryName[12];               			// �豸��������
	uint32 CanControlPTZ;		                    // ��̨�Ƿ����
	int isOnLine;									// �Ƿ����� (�޷��ж��Ƿ����ߵ�Ĭ��Ϊ����) <0 �޷�ȷ�� 0:���� 1:����
}NEWCOMMUNITYDEVICE,*PNEWCOMMUNITYDEVICE;

//�����������Ϣ�ṹ
typedef struct 
{
    uint32 	EnableOpen;         					// �Ƿ񿪷����
	char 	DeviceName[32];							// �豸����
	uint32 	DeviceIP;								// �豸IP
	uint16 	DevPort	;								// �豸�˿�
	uint16 	ChannelNumber;							// ͨ������
	char 	FactoryName[12];    					// �豸��������
	char 	UserName[16];							// �豸��½�û���
	char 	Password[16];							// �豸��½����
	uint32  CanControlPTZ;          				// ��̨�ɿر�־,0: ��̨���ɿ��ƣ�1:��̨���Կ���
}HOMEDEVICE, *PHOMEDEVICE;

typedef struct 
{
    uint32 	EnableOpen;         					// �Ƿ񿪷����
	char 	DeviceName[32];							// �豸����
	uint32 	DeviceIP;								// �豸IP
	uint16 	DevPort	;								// �豸�˿�
	uint16 	ChannelNumber;							// ͨ������
	char 	FactoryName[12];    					// �豸��������
	char 	UserName[16];							// �豸��½�û���
	char 	Password[16];							// �豸��½����
	uint32  CanControlPTZ;          				// ��̨�ɿر�־,0: ��̨���ɿ��ƣ�1:��̨���Կ���
	int isOnLine;									// �Ƿ����� (�޷��ж��Ƿ����ߵ�Ĭ��Ϊ����) <0 �޷�ȷ�� 0:���� 1:����
}NEWHOMEDEVICE, *PNEWHOMEDEVICE;


typedef struct 
{
	uint32 Comnum;									
	uint32 Homenum;
   	COMMUNITYDEVICE *Comdev;	
   	HOMEDEVICE* Homedev;
}MonitorDeviceList,*PMonitorDeviceList;		

typedef struct 
{
	uint32 DevNum;									
   	HOMEDEVICE* Devinfo;
}RtspDeviceList,*PRtspDeviceList;	

typedef struct 
{
	uint32 Comnum;									
	uint32 Homenum;
   	NEWCOMMUNITYDEVICE *Comdev;	
   	NEWHOMEDEVICE* Homedev;
}NewMonitorDeviceList,*PNewMonitorDeviceList;	


/*************************************************
  Function:		storage_get_monitordev
  Description: 	
  Input:		
  	3.Time		ʱ��
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
void storage_get_monitordev(PMonitorDeviceList *Devlist);

/*************************************************
  Function:		storage_get_home_monitordev
  Description: 	
  Input:		
  Output:		��
  Return:		ECHO_STORAGE
  Others:		��ȡ�ҾӼ����б�
*************************************************/
void storage_get_home_monitordev(PMonitorDeviceList *Devlist);

/*************************************************
  Function:		storage_get_comm_monitordev
  Description: 	
  Input:		
  Output:		��
  Return:		ECHO_STORAGE
  Others:		��ȡ���������б�
*************************************************/
void storage_get_comm_monitordev(PMonitorDeviceList *Devlist);

/*************************************************
  Function:		storage_malloc_new_monitordev_memory
  Description: 	����洢�ڴ�
  Input:
  	1.MaxNum	����¼��
  Output:		��
  Return:		��
  Others:
*************************************************/
void storage_malloc_new_monitordev_memory (uint32 HomeMaxNum, uint32 CommMaxNum, PNewMonitorDeviceList *list);

/*************************************************
  Function:		storage_get_index
  Description: 	��ȡ�豸�ı��
  Input:		��
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
uint8 storage_get_index(uint8 index);
#endif 

