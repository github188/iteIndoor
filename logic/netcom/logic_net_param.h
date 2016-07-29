/*************************************************
  Copyright (C), 2006-2010, Aurine
  File name:  	logic_net_param.h
  Author:     	xiewr
  Version:    	1.0
  Date: 
  Description:  获取本地IP信息头文件
  
  History:        
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#ifndef _NET_GET_PARAM_H_
#define _NET_GET_PARAM_H_

#include "logic_include.h"


/*************************************************
  Function:    		NetworkInit
  Description:		网络初始化
  Input: 			
  Output:			无
  Return:			无
  Others:
*************************************************/
void NetworkInit(void);

/*************************************************
  Function:    		net_set_local_param
  Description:		设置本地网络参数
  Input: 			
  	1.NetParam		网络参数指针
  Output:			无
  Return:			无
  Others:
*************************************************/
void net_set_local_param(void);

/*************************************************
  Function:    		net_set_local_mac
  Description:		设置MAC
  Input: 			
  	1.mac			MAC
  Output:			无
  Return:			无
  Others:			TRUE / FALSE
*************************************************/
uint32 net_set_local_mac(uint8 *mac);

/*************************************************
  Function:    		IPtoUlong
  Description:		将IP地址字符串转化为long型IP地址
  Input:
  	1.ip			IP地址字符串
  Output:			
  Return:			转化后的unsigned long型IP地址
  Others:			
*************************************************/
uint32 IPtoUlong(char* ip);

/*************************************************
  Function:    		IPIsCorrect
  Description:		IP是否正常
  Input:
  	1.ip			IP地址字符串
  Output:			
  Return:			
  Others:			
*************************************************/
int IPIsCorrect(char* ip);

/*************************************************
  Function:    		UlongtoIP
  Description:		将long型IP地址转化为IP地址字符串
  Input:
  	1.uIp			unsigned long型IP地址
  Output:			
  Return:			转化后的字符串指针
  Others:			返回NULL 表示IP地址错误
*************************************************/
char* UlongtoIP(uint32 uIp);
#endif

