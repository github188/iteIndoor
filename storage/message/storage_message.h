/*************************************************
  Copyright (C), 2006-2016, Aurine
  File name: 	storage_message.h
  Author:    	luofl
  Version:   	2.0
  Date: 		2014-11-06
  Description:	信息存储
  
  History:                    
    1. Date:		 
       Author:		 
       Modification: 
    2. ...
*********************************************************/
#ifndef _STORAGE_MESSAGE_H_
#define _STORAGE_MESSAGE_H_

#include "storage_include.h"


// 信息结构体
typedef struct
{
	uint8 is_unread;								// 0 - 已读，1 - 未读标志	
	uint8 voice_type;								// 0x20:mp3 0x21:wav
	DATE_TIME time;									// 信息接收到的时间
	uint8 des[14];									// 执行者的描述或者电话号码
	REC_BULLETIN_HEAD Head;							// 信息头
}MSGDATA, * PMSGDATA;

// 信息列表
typedef struct
{
	uint8 ncount;
	MSGDATA pinfo_data[MAX_MSG_NUM];
}MSGLIST, * PMSGLIST;

/*************************************************
  Function:			storage_read_msg_voice_path
  Description: 		获取信息声音路径
  Input:
  	1.pmsg_data		信息结构体
  Output:		
  Return:			文字的为信息的数据,图片的则是图片文件名
  Others:
*************************************************/ 
void storage_read_msg_voice_path(MSGDATA *pmsg_data, uint8 path[50]);

/*************************************************
  Function:			storage_read_msg_voice_path
  Description: 		获取信息声音路径
  Input:
  	1.pmsg_data		信息结构体
  Output:		
  Return:			文字的为信息的数据,图片的则是图片文件名
  Others:
*************************************************/ 
void storage_read_msg_voice_path(MSGDATA *pmsg_data, uint8 path[50]);

/*************************************************
  Function:			storage_read_msg_data
  Description: 		获取信息数据
  Input:
  	1.pmsg_data		信息结构体
  	2.index			信息的索引
  Output:		
  Return:			信息的数据
  Others:
*************************************************/ 
char * storage_read_msg_data(MSGDATA *pmsg_data, uint8 index);

/*************************************************
  Function:			storage_read_msg_list
  Description: 		获取信息列表
  Input:		
  Output:		
  Return:			MSGLIST	结构
  Others:
*************************************************/ 
MSGLIST *storage_read_msg_list(void);

/*************************************************
  Function:			storage_write_msg
  Description: 		写入信息
  Input:		
  	1.pHead			信息头
  	2.data			数据
  	3.des			执行者的描述或者短信的电话号码
  Output:		   
  Return:		    
  Others:
*************************************************/
ECHO_STORAGE storage_write_msg(const REC_BULLETIN_HEAD *pHead,uint8 *data,uint8 des[14]);

/*************************************************
  Function:    		storage_delete_msg
  Description:		删除某条信息
  Input:
  	1.index			信息索引
  Output:			
  Return:			设置成功TRUE,失败FALSE
  Others:			
*************************************************/
ECHO_STORAGE storage_delete_msg(uint8 index);

/*************************************************
  Function:			storage_clear_msg
  Description: 		清空信息
  Input:		
  Output:		 
  Return:			TRUE:成功
  Others:
*************************************************/
ECHO_STORAGE storage_clear_msg (void);

/*************************************************
  Function:    		storage_set_msg_readed
  Description:		设置信息已读，或者未读标志
  Input:
  	1.index			信息索引
  Output:			
  Return:			设置成功TRUE,失败FALSE
  Others:			
*************************************************/
ECHO_STORAGE storage_set_msg_readed(uint8 index);

/*************************************************
  Function:			storage_get_msg_state
  Description: 		获取信息是否 有未读标示
  Input:		
  Output:		
  Return:			TRUE:有未读 FALSE:无未读
  Others:
*************************************************/
uint8 storage_get_msg_state(void);

/*************************************************
  Function:			storage_get_msg_size
  Description: 		获得信息的大小
  Input:		
  Output:		 
  Return:			TRUE:成功
  Others:
*************************************************/
int storage_get_msg_allsize(void);

/*************************************************
  Function:			storage_get_msg_unread_list
  Description: 		获取信息未读列表
  Input:			无
  Output:			无
  Return:			TRUE:有未读 FALSE:无未读
  Others:
*************************************************/
uint8 storage_get_msg_unread_list(MSGLIST **list);
#endif

