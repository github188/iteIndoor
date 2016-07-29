/*********************************************************************
  COPYRIGHT (C), 2006-2010, AURINE
  FILE NAME: 		logic_wlmqj_set.h
  AUTHOR:   		xiewr
  VERSION:   		1.0 
  DATE:     		2010-06-04
  DESCRIPTION:		������ǰ������

  History:        
    1. Date:
       Author:
       Modification:
    2. ...     
*********************************************************************/
#ifndef _LOGIC_WLMQJ_SET_H_
#define _LOGIC_WLMQJ_SET_H_

#include "logic_include.h"

#define NETDOOR_FAULT_IP		0x0A640104 						// 10.100.1.4

#define NETDOOR_ECHO_SUCESS		0								//�����ɹ�

#define NETDOOR_ECHO_COMM		0x030000

#define NETDOOR_ERR_SEND		(NETDOOR_ECHO_COMM|0x00)		//�������ݷ��ʹ���
#define NETDOOR_ERR_ECHO		(NETDOOR_ECHO_COMM|0x01)		//Ӧ��һ�����
#define NETDOOR_ERR_MAC			(NETDOOR_ECHO_COMM|0x09)		//MAC�����

typedef struct hiVERSION_INFO_S
{
    uint32 u32ProductVer;       /* The product version  */
    uint32 u32RootBoxVer;       /* RootBox version      */
    uint32 u32LinuxVer;         /* Linux kernel version */
    uint32 u32MspVer;           /* The MSP version      */
    uint32 u32AlgVer;           /* The codec alg version*/
    uint32 u32ChipVer;          /* The chip version     */
} VERSION_INFO_S, FWM_VERSION_T;

/*************************************************
  Function:    		send_netdoor_cmd_noecho
  Description:		��������ǰ��������������
  					�˽ӿ���Ҫ������Ӧ�����ݵķ���
  					��MAC,MAC������ǰ��
  Input: 
  	1.cmd:			���͵�����
  	2.data: 		��������
  	3.datelen:		���ݵĳ���
  Output:			��
  Return:			ECHO_OK: 0
  					ECHO_ERROR:0x030001
  					ECHO_MAC:0x030009
  					����:0x0300**
  Others:
*************************************************/
uint32 send_netdoor_cmd_noecho(NET_COMMAND cmd ,unsigned char * SendData, uint32 len);

/*************************************************
  Function:			logic_set_netdoor_netparam
  Description:		����������ǰ���������
  Input: 				
    1.netparam		�������
  Output:			��
  Return:			ECHO_OK: 0
  					ECHO_ERROR:0x030001
  					ECHO_MAC:0x030009
  					���ݷ�������:0x030000
  Others:
*************************************************/
int32 logic_set_netdoor_netparam(PNET_PARAM netparam);

/*************************************************
  Function:			logic_get_netdevice_netparam
  Description:		��������豸�������
  Input: 				
  Output:			
  	1.netparam		�������
  Return:			ECHO_OK: 0
  					ECHO_ERROR:0x030001
  					ECHO_MAC:0x030009
  					����:0x0300**
  Others:			
*************************************************/
int32 logic_get_netdevice_netparam(PNET_PARAM netparam);

/*************************************************
  Function:			logic_set_netdoor_devnoinfo
  Description:		����������ǰ���豸����Լ�����
  Input: 				
    1.devno			�豸���
    2.rule 			��Ź���
  Output:			��
  Return:			ECHO_OK: 0
  					ECHO_ERROR:0x030001
  					ECHO_MAC:0x030009
  					����:0x0300**
  Others:
*************************************************/
int32 logic_set_netdoor_devnoinfo(PFULL_DEVICE_NO devno, DEVICENO_RULE * rule);

/*************************************************
  Function:			logic_get_netdevice_devnoinfo
  Description:		��ȡ������ǰ���豸����Լ�����
  Input: 			
  Output:			
    1.devno			�豸���
    2.rule 			��Ź���
  Return:			ECHO_OK: 0
  					ECHO_ERROR:0x030001
  					ECHO_MAC:0x030009
  					����:0x0300**
  Others:
*************************************************/
int32 logic_get_netdevice_devnoinfo(PFULL_DEVICE_NO devno, DEVICENO_RULE * rule);

/*************************************************
  Function:			logic_get_netdoor_version
  Description:		���������ǰ���ں˺ͳ���汾��
  Input: 			��
  Output:			
    1.KerVer		�ں˰汾��
  	2.BuildTime		����ʱ��
  	3.ver			����汾��
  Return:			ECHO_OK: 0
  					ECHO_ERROR:0x030001
  					ECHO_MAC:0x030009
  					����:0x0300**
  Others:
*************************************************/
int32 logic_get_netdoor_version(FWM_VERSION_T * KerVer, char * BuildTime, char * ver);

/*************************************************
  Function:			logic_mng_netdoor_card
  Description:		������Ϣ
  Input: 	
  	1.OperType		��������
    2.CardNo		����ָ��
  Output:			��
  Return:			ECHO_OK: 0
  					ECHO_ERROR:0x030001/196609
  					ECHO_MAC:0x030009/196617
  					NETDOOR_ERR_SEND:0x030000/196608		
  Others:
*************************************************/
int32 logic_mng_netdoor_card(uint8 OperType, char * CardNo);

/*************************************************
  Function:			logic_get_netdevice_lockinfo
  Description:		�������Ϣ
  Input: 			��
  Output:			
  	1.LockType		������
  	2.LockTime		��ʱ��
  Return:			ECHO_OK: 0
  					ECHO_ERROR:0x030001/196609
  					ECHO_MAC:0x030009/196617
  					NETDOOR_ERR_SEND:0x030000/196608		
  Others:
*************************************************/
int32 logic_get_netdevice_lockinfo(uint8 * LockType, int32 * LockTime);

/*************************************************
  Function:			logic_set_netdevice_lockinfo
  Description:		��������Ϣ
  Input: 	
  	1.LockType		������
    2.LockTime		��ʱ��
  Output:			��
  Return:			ECHO_OK: 0
  					ECHO_ERROR:0x030001/196609
  					ECHO_MAC:0x030009/196617
  					NETDOOR_ERR_SEND:0x030000/196608		
  Others:
*************************************************/
int32 logic_set_netdevice_lockinfo(uint8 LockType, int32 LockTime);

/*************************************************
  Function:    		get_netdoor_mac
  Description:		���������ǰ����MAC��ַ
  Input: 			
    1.index			������ǰ����ID
  Output:			
	1.HWaddr		��õ�Ӳ����ַ
  Return:			�ɹ����, TRUE/FALSE
  Others:			�ڽ�����ǰ������ʱ���ȵ��ô˺���
*************************************************/
uint32 logic_get_netdoor_mac(uint8 index, unsigned char * HWaddr);

/*************************************************
  Function:			logic_set_netdoor_other_settings
  Description:		�����豸��������
  Input: 				
    1.OtherSettings	����ֵ 0:������,1:����
  Output:			��
  Return:			ECHO_OK: 0
  					ECHO_ERROR:0x030001
  					ECHO_MAC:0x030009
  					���ݷ�������:0x030000
  Others:			�����СΪ10,�����һ��Ԫ��Ϊ�������õ�ֵ,
  					�ڶ���Ԫ��Ϊ�������õ�ֵ,������Ԫ��Ϊ����
  					���ĸ�Ԫ��Ϊ��Ƶ��ʽ
*************************************************/
uint32 logic_set_netdoor_other_settings(unsigned char * OtherSettings);

/*************************************************
  Function:			logic_get_netdoor_other_settings
  Description:		��������豸��������
  Input: 				
  Output:			
  	1.OtherSettings	��������ֵ
  Return:			ECHO_OK: 0
  					ECHO_ERROR:0x030001
  					ECHO_MAC:0x030009
  					���ݷ�������:0x030000
  					����:0x0300**
  Others:			
*************************************************/
uint32 logic_get_netdoor_other_settings(unsigned char * OtherSettings);

/*************************************************
  Function:    		logic_netdoor_pub_ini
  Description:		��ʼ��:���������ǰ����ַ��MAC��ַ
  Input: 			��
  Output:			
  Return:			�ɹ����, TRUE/FALSE
  Others:			�����ն������ؽ�������֮ǰҪ���ô˺���
*************************************************/
uint32 logic_netdoor_pub_ini(uint8 index);

#endif
