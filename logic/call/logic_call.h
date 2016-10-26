/*************************************************
  Copyright (C), 2006-2016, Aurine
  File name:  	logic_call.h
  Author:     	chenbh
  Version:    	2.0
  Date: 		2014-12-11
  Description:  呼叫逻辑头文件
  
  History:        
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#ifndef _LOGIC_CALL_H_
#define _LOGIC_CALL_H_

// 对讲页面GUI回调函数
typedef void (*PFGuiNotify)(uint32 param1, uint32 param2);

#define MANAGER_COUNTS_MAX		8					// 最大管理机个数
extern uint32 g_Manager_IP[];
extern uint32 g_Manager_Index[];

// 对讲类型
typedef enum
{
	INTER_CALLOUT_E, 								// 主叫
	INTER_CALLIN_E,									// 被叫
	INTER_MONITOR_E									// 监视
}INTER_OPER_TYPE;

typedef enum
{
	END_BY_OTHER_SIDE,								// 对方挂断
	END_BY_USER_HANDDOW,							// 本地挂断
	END_BY_CALL_BUSY, 								// 对方忙
	END_BY_CALL_TIMEOUT, 							// 呼叫/通话/留言录制超时
	END_BY_COMM_TIMEOUT,							// 心跳超时
	END_BY_REQUESET_ERR,							// 连接错误, 搜索不到IP
	END_BY_SELF_ERR									// 其他错误
}CALL_END_TYPE_E;



typedef struct
{
	uint8 InterType;
	uint8 DevType;
	char DevStr[30];
}INTER_INFO_S, *PINTER_INFO_S;

struct ThreadInfo
{
	uint8 running;
	pthread_t thread;	
	int (*exit)(struct ThreadInfo *thread);
	void *private;
};


/*************************************************
  Function:			call_get_media_ifover
  Description: 		音视频媒体是否已经关闭
  Input: 			
  Output:			无
  Return:			
  Others:
*************************************************/
uint8 call_get_media_ifover(void);

/*************************************************
  Function:			get_fenji_ipaddr
  Description:		获取分机IP
  Input: 	
  Output:			
  	1.devno		
  Return:			成功与否, TRUE/FALSE
  Others:	
*************************************************/
uint32 get_fenji_ipaddr(uint32 devno);

/*************************************************
  Function:			StartThread
  Description:		
  Input:	
  Output:
  Return:
  Others:
*************************************************/
int inter_start_thread(struct ThreadInfo* thread, void* proc, void* param, int ID);

/*************************************************
  Function:				inter_call_resident
  Description:			呼叫住户
  Input: 	
  	1.mode				0 区内 1 区间
  	2.InPutNo			输入号码
  Output:				无
  Return:				0 成功
  Others:
*************************************************/
int32 inter_call_resident(int32 mode, char * InPutNo);

/*************************************************
  Function:				inter_call_sub_terminal
  Description:			呼叫分机
  Input: 	
  	1.InPutNo			输入号码
  Output:				无
  Return:				0 成功
  Others:
*************************************************/
int32 inter_call_sub_terminal(char *InputNo);

/*************************************************
  Function:				inter_call_manager
  Description:			呼叫管理员机
  Input: 	
  	1.InPutNo			输入号码
  Output:				无
  Return:				0 成功
  Others:
*************************************************/
int32 inter_call_manager(char *InputNo);

/*************************************************
Function:			logic_call_sub_terminal
Description:		呼叫分机
Input:
1.InPutNo			输入号码
Output:				无
Return:				0 成功
Others:				无
*************************************************/
int32 logic_call_sub_terminal(char *InputNo);

/*************************************************
Function:			logic_call_center
Description:		呼叫管理员机
Input:
1.InPutNo			输入号码
Output:				无
Return:				0 成功
Others:
*************************************************/
int32 logic_call_center(char *InputNo);

/*************************************************
  Function:				inter_answer_call
  Description:			被叫接听
  Input: 			
  	1.VideoSendrecv		视频
  Output:				无
  Return:				成功与否 TRUE/FALSE
  Others:	
*************************************************/
int32 inter_answer_call(int32 * VideoSendrecv);

/*************************************************
  Function:			inter_hand_down
  Description:		挂断
  Input: 			无
  Output:			无
  Return:			成功与否 TRUE/FALSE
  Others:
*************************************************/
void inter_hand_down(void);

/*************************************************
  Function:			phone_hand_down
  Description:		门口挂断室内
  Input: 			无
  Output:			无
  Return:			
  Others:
*************************************************/
void phone_hand_down(void);

/*************************************************
  Function:				phone_call_in
  Description:			模拟门前机呼入
  Input: 	
  	1.index				门前机索引 1、2
  Output:				无
  Return:				0 成功
  Others:
*************************************************/
int32 phone_call_in(uint8 index);

/*************************************************
Function:				inter_call_ini
Description:			对讲初始化
Input:
1.CallRequestGuiNotify	请求时GUI回调函数
1.CallGuiNotify			呼叫GUI回调函数
2.BeCallGuiNotify		被叫GUI回调函数
Output:					无
Return:
Others:
*************************************************/
void inter_call_ini(PFGuiNotify CallRequestGuiNotify, PFGuiNotify CallGuiNotify, PFGuiNotify BeCallGuiNotify);

/*************************************************
  Function:			inter_video_snap
  Description:		抓拍
  Input: 	
  Output:			无
  Return:			成功与否, TRUE/FALSE
  Others:	
*************************************************/
int32 inter_video_snap(void);

/*************************************************
  Function:			inter_call_distribute
  Description:		对讲呼叫类发送包命令处理函数
  Input: 	
  	1.recPacket		接收包
  Output:			无
  Return:			成功与否, TRUE/FALSE
  Others:	
*************************************************/
int32 inter_call_distribute(const PRECIVE_PACKET recPacket);

/*************************************************
  Function:			inter_call_responsion
  Description:		对讲呼叫类应答包命令处理函数
  Input: 	
  	1.recPacket		应答包
  	2.SendPacket	发送包
  Output:			无
  Return:			无
  Others:
*************************************************/
void inter_call_responsion(const PRECIVE_PACKET recPacket, const PSEND_PACKET SendPacket);

/*************************************************
  Function:			get_callout_type
  Description: 		获取呼叫类型
  Output:			无
  Return:			
  Others:		
*************************************************/
uint8 get_callout_type(void);

/*************************************************
  Function:			inter_unlock
  Description:		室内机开锁
  Input: 			无
  Output:			无
  Return:			TRUE / FALSE
  Others:
*************************************************/
int32 inter_unlock(void);

/*************************************************
  Function:			call_out
  Description:		开始呼叫,呼叫参数设置完再调用该函数建立线程
  Input: 	
  Output:			无
  Return:			无
  Others:
*************************************************/
int32 call_out(void);
#endif

