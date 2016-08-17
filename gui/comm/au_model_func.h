/*********************************************************
  Copyright (C), 2006-2010, Aurine
  File name:	au_model_func.h
  Author:   	唐晓磊
  Version:   	2.0
  Date: 		08.3.26
  Description:  公共函数头文件
  History:                   
    1. Date:
       Author:
       Modification:
    2. ...
*********************************************************/
#ifndef __AU_MODEL_FUNC_H__
#define __AU_MODEL_FUNC_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "gui_include.h"

#define ip_addr1(ipaddr) ((uint8)(ipaddr >> 24) & 0xff)
#define ip_addr2(ipaddr) ((uint8)(ipaddr >> 16) & 0xff)
#define ip_addr3(ipaddr) ((uint8)(ipaddr >> 8) & 0xff)
#define ip_addr4(ipaddr) ((uint8)(ipaddr) & 0xff)

typedef enum
{
	STYLE_MAIN_NEWMESSAGE, 
	STYLE_MAIN_LYLY,
	STYLE_MAIN_NOFAZE,
	STYLE_MAIN_COUNT
} MAIN_ICON_STYLE;							// 更改小图标类型

typedef enum
{
	SHOW_MSG_WIN, 
	SHOW_SAFETY_WIN,
	SHOW_BECALL_WIN,
	SHOW_MAX_WIN
} SHOW_WIN_TYPE;							// 显示窗口

// 异步操作仲裁: IO触发、感应按键、网络触发、界面入口
// 主要处理页面跳转仲裁, 不处理媒体之间的仲裁
typedef enum
{
	SYS_OPER_ALARMING,					  // 报警触发-跳转页面					
	SYS_OPER_ALARM_SOS,						// 报警感应按键
	SYS_OPER_CALLOUT,						  // 呼叫中心、住户、分机、回拨记录
	SYS_OPER_CALLIN,							// 被叫
	SYS_OPER_MONITOR							// 监视
}SYS_ASYN_OPER_TYPE;

/*************************************************
  Function:			ui_show_win_arbitration
  Description: 		显示页面时仲裁
  Input: 		
  	1.hDlg			当前页面
  	2.OperType		操作类型
  Output:			无
  Return:			TRUE / FALSE
  Others:			
*************************************************/
int32 ui_show_win_arbitration(SYS_ASYN_OPER_TYPE OperType);

/*************************************************
  Function:			get_houseno_desc
  Description: 		获得房号描述
  Input: 			
  	1.numtext		设备编号
  	2.temp			设备描述
  Output:			无
  Return:			无
  Others:		
*************************************************/
char * get_houseno_desc(char * numtext, char * temp);

/*************************************************
  Function:			get_dev_description
  Description: 		获得设备描述
  Input: 			
  	1.DevType		设备类型
  	2.DevStr		设备编号
 	3.StrOutLen		输出缓冲大小
  Output:			
  	1.DevStrOut		设备描述
  Return:			无
  Others:		
*************************************************/
uint32 get_dev_description(DEVICE_TYPE_E DevType, char * DevStr, char * DevStrOut, uint32 StrOutLen);

/*************************************************
Function:    		toXiechiPwd
Description:		转换成挟持密码
Input:
1.oldPwd		要转换的密码
2.bAdd			挟持密码是否+1：true:+1；false: -1;
Output:
1.newPwd		转换后的密码
Return:
*************************************************/
void toXiechiPwd(char * oldPwd, uint8 bAdd, char * newPwd);

/*************************************************
Function:		change_ip_to_char
Description:  切换IP地址4个字节
Input:
1.IP		IP地址
2.ipaddr	地址
Output:		无
Return:		无
Others:
*************************************************/
void change_ip_to_char(uint32 IP, uint8* ipaddr);

/*************************************************
Function:		change_char_to_ip
Description:  切换4个字节为IP地址
Input:
1.ipaddr	ip地址
Output:		无
Return:		无
Others:
*************************************************/
uint32 change_char_to_ip(uint8* ipaddr);

/*************************************************
Function:		change_ip_to_str
Description:  切换IP地址字符串
Input:
1.IP		IP地址
2.ipaddr	地址
Output:		无
Return:		无
Others:
*************************************************/
void change_ip_to_str(uint32 IP, char *ip);

/*************************************************
Function:		change_char_to_ip
Description:  切换4个字节为IP地址
Input:
1.ipaddr	ip地址
Output:		无
Return:		无
Others:
*************************************************/
uint32 change_char_to_ip(uint8* ipaddr);

/*************************************************
Function:		LogicShowWin
Description:	逻辑回调显示窗口
Input:
1.type			显示类型
Output:			无
Return:			无
Others:
*************************************************/
void LogicShowWin(SHOW_WIN_TYPE type, char* param);

#ifdef __cplusplus 
}
#endif

#endif /* __AU_MODEL_FUNC_H__ */
