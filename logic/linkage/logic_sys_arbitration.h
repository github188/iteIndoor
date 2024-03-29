/*************************************************
  Copyright (C), 2006-2010, Aurine
  File name:  	logic_sys_arbitration.h
  Author:     	xiewr
  Version:    	1.0
  Date: 
  Description:  系统仲裁头文件
  
  History:        
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#ifndef _SYS_ARBITRATION_H_
#define _SYS_ARBITRATION_H_
#include "logic_include.h"

// 时间、进度、状态:TRUE-正常/FALSE-出错
typedef int32 (*MediaPlayCallback)(int32 param1,int32 param2,int32 state);
typedef void (*MediaStopedCallback)(void);

typedef enum
{
	SYS_MEDIA_NONE = 0x00,					
	SYS_MEDIA_ALARM,								// 报警声
	SYS_MEDIA_INTERCOM,								// 对讲
	SYS_MEDIA_MONITOR,								// 监视
	SYS_MEDIA_YUJING,								// 预警声
	SYS_MEDIA_OPER_HINT,							// 操作提示音
	SYS_MEDIA_MUSIC,								// 播放音乐文件,包括播放家人留言,各种提示音试听
	SYS_MEDIA_FAMILY_RECORD,						// 家人留言录制
	SYS_MEDIA_FAMILY_AUDITION,						// 家人留言录制后试听
	SYS_MEDIA_LEAVEWORD_PLAY,						// 播放留影留言
	SYS_MEDIA_KEY,									// 播放按键声音
}SYS_MEDIA_TYPE;

#define	 SYS_MEDIA_ECHO_OK				0x00		
#define	 SYS_MEDIA_ECHO_ERR				0xFF		

typedef enum
{
	SYS_HINT_INFO,									// 新信息
	SYS_HINT_LEAVEWORD,								// 新留影留言
	SYS_HINT_FAMILY,								// 新家人留言
	SYS_HINT_ALARM_WARNING,							// 有警情
	SYS_HINT_MISSED_CALLS,							// 未接来电
	SYS_HINT_NO_DISTURB,							// 免打扰
	SYS_HINT_OPERATING,								// 终端操作状态 操作中:TRUE 待机:FALSE
	SYS_HINT_ALARM_STATE,							// 安防状态 布防:TRUE 撤防:FALSE
	SYS_HINT_MAX_NUM
}SYS_HINT_TYPE;

typedef struct _sys_hint_record
{
	uint8 syshint[SYS_HINT_MAX_NUM];				// 记录未读无处理状态 状态实时去获取
	uint8 syshintnum[SYS_HINT_MAX_NUM];				// 消息记录未读条数
	MSGLIST *MsgUnReadList;							// 未读信息列表
	LYLYLIST_INFO *LylyUnReadList;					// 未读留影留言列表
	JRLYLIST_INFO *JrlyUnReadList;					// 未读家人留言列表
	MCALLLISTINFO *MissUnReadList;					// 未读未接来电列表
	ALARM_TOUCH_INFO_LIST *AlarmUnReadList;			// 未读报警记录列表
}SysHintRecord, *PSysHintRecord;

/*************************************************
  Function:			sys_continue_play_file
  Description: 		继续播放
  Input: 			
  Output:			无
  Return:			无
  Others:			
*************************************************/
int sys_continue_play_file(void);

/*************************************************
  Function:			sys_pause_play_file
  Description: 		暂停播放媒体文件
  Input: 			
  Output:			无
  Return:			无
  Others:			
*************************************************/
int sys_pause_play_file(void);

/*************************************************
  Function:			sys_stop_play_audio
  Description: 		停止播放音频文件
  Input: 			
  	1.AudioType		音乐播放类型
  Output:			无
  Return:			无
  Others:			
*************************************************/
void sys_stop_play_audio(SYS_MEDIA_TYPE AudioType);

/*************************************************
  Function:			sys_start_play_audio
  Description: 		开始播放音频
  Input: 			
  	1.MediaType		媒体操作类型
  	2.FileName		需要播放的媒体文件路径
  	3.isRepeat		是否重复播放
  	4.value			设置的音量
  	5.ProgressProc	进度条回调函数
  	6.StoppedProc	被强制终止后回调函数
  Output:			无
  Return:			0-成功 非0-失败
  Others:			
*************************************************/
int32 sys_start_play_audio(const SYS_MEDIA_TYPE MediaType, char *FileName, uint8 isRepeat, uint8 volume, 
							MediaPlayCallback ProgressProc, MediaStopedCallback StoppedProc);

/*************************************************
  Function:			sys_start_play_leaveword
  Description: 		开始播放留影留言
  Input: 			
  	1.FileName		文件名
  	2.ProcessProc	进度回调
  	3.StoppedProc	被强制终止后回调函数
  Output:			无
  Return:			0-成功 非0-失败
  Others:			
*************************************************/
//int32 sys_start_play_leaveword(char *FileName, MediaPlayCallback ProcessProc, MediaStopedCallback StoppedProc);
int32 sys_start_play_leaveword(char *FileName, LYLY_TYPE type, uint8 volume, MediaPlayCallback ProcessProc, MediaStopedCallback StoppedProc);

/*************************************************
  Function:			sys_stop_play_leaveword
  Description: 		停止播放留影留言
  Input: 			无
  Output:			无
  Return:			0-成功 非0-失败
  Others:			
*************************************************/
int32 sys_stop_play_leaveword(void);

/*************************************************
  Function:			sys_start_family_record
  Description: 		开始家人留言录制
  Input: 			
  	1.FileName		文件路径
  	2.ProcessProc	进度回调函数
  	3.StoppedProc	被强制终止后回调函数
  Output:			无
  Return:			0-成功 非0-失败
  Others:			
*************************************************/
int32 sys_start_family_record(char *FileName, MediaPlayCallback ProcessProc, MediaStopedCallback StoppedProc);

/*************************************************
  Function:			sys_stop_family_record
  Description: 		停止家人留言录制
  Input: 			无
  Output:			无
  Return:			0-成功 非0-失败
  Others:			
*************************************************/
int32 sys_stop_family_record(void);

/*************************************************
  Function:			sys_set_intercomm_state
  Description: 		设置当前对讲状态
  Input: 			
  	1.state			TRUE-处于对讲中 FALSE-不处于对讲中
  Output:			无
  Return:			0-成功 非0-失败
  Others:			
*************************************************/
int32 sys_set_intercomm_state(uint8 state);

/*************************************************
  Function:			sys_set_monitor_state
  Description: 		设置当前监视状态
  Input: 			
  	1.state			TRUE-处于监视中 FALSE-不处于监视中
  Output:			无
  Return:			0-成功 非0-失败
  Others:			
*************************************************/
int32 sys_set_monitor_state(uint8 state);

/*************************************************
  Function:			sys_get_media_state
  Description: 		获取系统媒体状态
  Input: 		
  Output:			无
  Return:			系统媒体状态
  Others:			
*************************************************/
SYS_MEDIA_TYPE sys_get_media_state(void);

/*************************************************
  Function:			sys_stop_cur_media
  Description: 		停止当前媒体
  Input: 		
  Output:			无
  Return:			
  Others:			
*************************************************/
void sys_stop_cur_media(void);

/*************************************************
  Function:			sys_set_hint_state
  Description: 		设置系统状态
  Input: 		
  	1.HintType		系统状态类型
  	2.flg			标志: TRUE / FALSE
  Output:			无
  Return:			无
  Others:			
*************************************************/
void sys_set_hint_state(SYS_HINT_TYPE HintType, uint8 flg);

/*************************************************
  Function:			sys_get_hint_state
  Description: 		获得系统状态
  Input: 		
  	1.HintType		系统状态类型
  Output:			无
  Return:			TRUE / FALSE
  Others:			
*************************************************/
uint8 sys_get_hint_state(SYS_HINT_TYPE HintType);

/*************************************************
  Function:			sys_get_hint_list
  Description: 		
  Input: 		
  Output:			无
  Return:			
  Others:			
*************************************************/
SysHintRecord *sys_get_hint_list(void);

/*************************************************
  Function:			sys_sync_hint_state
  Description: 		同步系统状态
  Input: 		
  Output:			无
  Return:			无
  Others:			
*************************************************/
void sys_sync_hint_state(void);

/*************************************************
  Function:			sys_sync_hint_state
  Description: 		同步系统状态
  Input: 		
  Output:			无
  Return:			无
  Others:			
*************************************************/
void sys_sync_hint_state_ext(SYS_HINT_TYPE HintType);

/*************************************************
  Function:			sys_init_hint_state
  Description: 		系统上电时初始化系统状态
  Input: 		
  Output:			无
  Return:			无
  Others:			
*************************************************/
void sys_init_hint_state(void);

/*************************************************
  Function:			sys_key_beep
  Description: 		按键音
  Input: 		
  Output:			无
  Return:			无
  Others:			
*************************************************/
void sys_key_beep(void);

#endif
