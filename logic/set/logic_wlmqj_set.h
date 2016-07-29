/*********************************************************************
  COPYRIGHT (C), 2006-2010, AURINE
  FILE NAME: 		logic_wlmqj_set.h
  AUTHOR:   		xiewr
  VERSION:   		1.0 
  DATE:     		2010-06-04
  DESCRIPTION:		网络门前机设置

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

#define NETDOOR_ECHO_SUCESS		0								//操作成功

#define NETDOOR_ECHO_COMM		0x030000

#define NETDOOR_ERR_SEND		(NETDOOR_ECHO_COMM|0x00)		//网络数据发送错误
#define NETDOOR_ERR_ECHO		(NETDOOR_ECHO_COMM|0x01)		//应答一般错误
#define NETDOOR_ERR_MAC			(NETDOOR_ECHO_COMM|0x09)		//MAC吗错误

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
  Description:		往网络门前机发送设置命令
  					此接口主要用于无应答数据的发送
  					带MAC,MAC在数据前面
  Input: 
  	1.cmd:			发送的命令
  	2.data: 		数据内容
  	3.datelen:		数据的长度
  Output:			无
  Return:			ECHO_OK: 0
  					ECHO_ERROR:0x030001
  					ECHO_MAC:0x030009
  					其他:0x0300**
  Others:
*************************************************/
uint32 send_netdoor_cmd_noecho(NET_COMMAND cmd ,unsigned char * SendData, uint32 len);

/*************************************************
  Function:			logic_set_netdoor_netparam
  Description:		设置网络门前机网络参数
  Input: 				
    1.netparam		网络参数
  Output:			无
  Return:			ECHO_OK: 0
  					ECHO_ERROR:0x030001
  					ECHO_MAC:0x030009
  					数据发生错误:0x030000
  Others:
*************************************************/
int32 logic_set_netdoor_netparam(PNET_PARAM netparam);

/*************************************************
  Function:			logic_get_netdevice_netparam
  Description:		获得网络设备网络参数
  Input: 				
  Output:			
  	1.netparam		网络参数
  Return:			ECHO_OK: 0
  					ECHO_ERROR:0x030001
  					ECHO_MAC:0x030009
  					其他:0x0300**
  Others:			
*************************************************/
int32 logic_get_netdevice_netparam(PNET_PARAM netparam);

/*************************************************
  Function:			logic_set_netdoor_devnoinfo
  Description:		设置网络门前机设备编号以及规则
  Input: 				
    1.devno			设备编号
    2.rule 			编号规则
  Output:			无
  Return:			ECHO_OK: 0
  					ECHO_ERROR:0x030001
  					ECHO_MAC:0x030009
  					其他:0x0300**
  Others:
*************************************************/
int32 logic_set_netdoor_devnoinfo(PFULL_DEVICE_NO devno, DEVICENO_RULE * rule);

/*************************************************
  Function:			logic_get_netdevice_devnoinfo
  Description:		获取网络门前机设备编号以及规则
  Input: 			
  Output:			
    1.devno			设备编号
    2.rule 			编号规则
  Return:			ECHO_OK: 0
  					ECHO_ERROR:0x030001
  					ECHO_MAC:0x030009
  					其他:0x0300**
  Others:
*************************************************/
int32 logic_get_netdevice_devnoinfo(PFULL_DEVICE_NO devno, DEVICENO_RULE * rule);

/*************************************************
  Function:			logic_get_netdoor_version
  Description:		获得网络门前机内核和程序版本号
  Input: 			无
  Output:			
    1.KerVer		内核版本号
  	2.BuildTime		编译时间
  	3.ver			程序版本号
  Return:			ECHO_OK: 0
  					ECHO_ERROR:0x030001
  					ECHO_MAC:0x030009
  					其他:0x0300**
  Others:
*************************************************/
int32 logic_get_netdoor_version(FWM_VERSION_T * KerVer, char * BuildTime, char * ver);

/*************************************************
  Function:			logic_mng_netdoor_card
  Description:		管理卡信息
  Input: 	
  	1.OperType		操作类型
    2.CardNo		卡号指针
  Output:			无
  Return:			ECHO_OK: 0
  					ECHO_ERROR:0x030001/196609
  					ECHO_MAC:0x030009/196617
  					NETDOOR_ERR_SEND:0x030000/196608		
  Others:
*************************************************/
int32 logic_mng_netdoor_card(uint8 OperType, char * CardNo);

/*************************************************
  Function:			logic_get_netdevice_lockinfo
  Description:		获得锁信息
  Input: 			无
  Output:			
  	1.LockType		锁类型
  	2.LockTime		锁时长
  Return:			ECHO_OK: 0
  					ECHO_ERROR:0x030001/196609
  					ECHO_MAC:0x030009/196617
  					NETDOOR_ERR_SEND:0x030000/196608		
  Others:
*************************************************/
int32 logic_get_netdevice_lockinfo(uint8 * LockType, int32 * LockTime);

/*************************************************
  Function:			logic_set_netdevice_lockinfo
  Description:		设置锁信息
  Input: 	
  	1.LockType		锁类型
    2.LockTime		锁时长
  Output:			无
  Return:			ECHO_OK: 0
  					ECHO_ERROR:0x030001/196609
  					ECHO_MAC:0x030009/196617
  					NETDOOR_ERR_SEND:0x030000/196608		
  Others:
*************************************************/
int32 logic_set_netdevice_lockinfo(uint8 LockType, int32 LockTime);

/*************************************************
  Function:    		get_netdoor_mac
  Description:		获得网络门前机的MAC地址
  Input: 			
    1.index			网络门前机的ID
  Output:			
	1.HWaddr		获得的硬件地址
  Return:			成功与否, TRUE/FALSE
  Others:			在进入门前机设置时首先调用此函数
*************************************************/
uint32 logic_get_netdoor_mac(uint8 index, unsigned char * HWaddr);

/*************************************************
  Function:			logic_set_netdoor_other_settings
  Description:		网络设备其他设置
  Input: 				
    1.OtherSettings	设置值 0:不启用,1:启用
  Output:			无
  Return:			ECHO_OK: 0
  					ECHO_ERROR:0x030001
  					ECHO_MAC:0x030009
  					数据发生错误:0x030000
  Others:			数组大小为10,数组第一个元素为拍照设置的值,
  					第二个元素为监视设置的值,第三个元素为语言
  					第四个元素为视频格式
*************************************************/
uint32 logic_set_netdoor_other_settings(unsigned char * OtherSettings);

/*************************************************
  Function:			logic_get_netdoor_other_settings
  Description:		获得网络设备其他设置
  Input: 				
  Output:			
  	1.OtherSettings	其他设置值
  Return:			ECHO_OK: 0
  					ECHO_ERROR:0x030001
  					ECHO_MAC:0x030009
  					数据发生错误:0x030000
  					其他:0x0300**
  Others:			
*************************************************/
uint32 logic_get_netdoor_other_settings(unsigned char * OtherSettings);

/*************************************************
  Function:    		logic_netdoor_pub_ini
  Description:		初始化:获得网络门前机地址和MAC地址
  Input: 			无
  Output:			
  Return:			成功与否, TRUE/FALSE
  Others:			无线终端与网关交互操作之前要调用此函数
*************************************************/
uint32 logic_netdoor_pub_ini(uint8 index);

#endif
