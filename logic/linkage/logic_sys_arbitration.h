/*************************************************
  Copyright (C), 2006-2010, Aurine
  File name:  	logic_sys_arbitration.h
  Author:     	xiewr
  Version:    	1.0
  Date: 
  Description:  ϵͳ�ٲ�ͷ�ļ�
  
  History:        
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#ifndef _SYS_ARBITRATION_H_
#define _SYS_ARBITRATION_H_
#include "logic_include.h"

// ʱ�䡢���ȡ�״̬:TRUE-����/FALSE-����
typedef int32 (*MediaPlayCallback)(int32 param1,int32 param2,int32 state);
typedef void (*MediaStopedCallback)(void);

typedef enum
{
	SYS_MEDIA_NONE = 0x00,					
	SYS_MEDIA_ALARM,								// ������
	SYS_MEDIA_INTERCOM,								// �Խ�
	SYS_MEDIA_MONITOR,								// ����
	SYS_MEDIA_YUJING,								// Ԥ����
	SYS_MEDIA_OPER_HINT,							// ������ʾ��
	SYS_MEDIA_MUSIC,								// ���������ļ�,�������ż�������,������ʾ������
	SYS_MEDIA_FAMILY_RECORD,						// ��������¼��
	SYS_MEDIA_FAMILY_AUDITION,						// ��������¼�ƺ�����
	SYS_MEDIA_LEAVEWORD_PLAY,						// ������Ӱ����
	SYS_MEDIA_KEY,									// ���Ű�������
}SYS_MEDIA_TYPE;

#define	 SYS_MEDIA_ECHO_OK				0x00		
#define	 SYS_MEDIA_ECHO_ERR				0xFF		

typedef enum
{
	SYS_HINT_INFO,									// ����Ϣ
	SYS_HINT_LEAVEWORD,								// ����Ӱ����
	SYS_HINT_FAMILY,								// �¼�������
	SYS_HINT_ALARM_WARNING,							// �о���
	SYS_HINT_MISSED_CALLS,							// δ������
	SYS_HINT_NO_DISTURB,							// �����
	SYS_HINT_OPERATING,								// �ն˲���״̬ ������:TRUE ����:FALSE
	SYS_HINT_ALARM_STATE,							// ����״̬ ����:TRUE ����:FALSE
	SYS_HINT_MAX_NUM
}SYS_HINT_TYPE;

/*************************************************
  Function:			sys_continue_play_file
  Description: 		��������
  Input: 			
  Output:			��
  Return:			��
  Others:			
*************************************************/
int sys_continue_play_file(void);

/*************************************************
  Function:			sys_pause_play_file
  Description: 		��ͣ����ý���ļ�
  Input: 			
  Output:			��
  Return:			��
  Others:			
*************************************************/
int sys_pause_play_file(void);

/*************************************************
  Function:			sys_stop_play_audio
  Description: 		ֹͣ������Ƶ�ļ�
  Input: 			
  	1.AudioType		���ֲ�������
  Output:			��
  Return:			��
  Others:			
*************************************************/
void sys_stop_play_audio(SYS_MEDIA_TYPE AudioType);

/*************************************************
  Function:			sys_start_play_audio
  Description: 		��ʼ������Ƶ
  Input: 			
  	1.MediaType		ý���������
  	2.FileName		��Ҫ���ŵ�ý���ļ�·��
  	3.isRepeat		�Ƿ��ظ�����
  	4.value			���õ�����
  	5.ProgressProc	�������ص�����
  	6.StoppedProc	��ǿ����ֹ��ص�����
  Output:			��
  Return:			0-�ɹ� ��0-ʧ��
  Others:			
*************************************************/
int32 sys_start_play_audio(const SYS_MEDIA_TYPE MediaType, char *FileName, uint8 isRepeat, uint8 volume, 
							MediaPlayCallback ProgressProc, MediaStopedCallback StoppedProc);

/*************************************************
  Function:			sys_start_play_leaveword
  Description: 		��ʼ������Ӱ����
  Input: 			
  	1.FileName		�ļ���
  	2.ProcessProc	���Ȼص�
  	3.StoppedProc	��ǿ����ֹ��ص�����
  Output:			��
  Return:			0-�ɹ� ��0-ʧ��
  Others:			
*************************************************/
//int32 sys_start_play_leaveword(char *FileName, MediaPlayCallback ProcessProc, MediaStopedCallback StoppedProc);
int32 sys_start_play_leaveword(char *FileName, LYLY_TYPE type, uint8 volume, MediaPlayCallback ProcessProc, MediaStopedCallback StoppedProc);

/*************************************************
  Function:			sys_stop_play_leaveword
  Description: 		ֹͣ������Ӱ����
  Input: 			��
  Output:			��
  Return:			0-�ɹ� ��0-ʧ��
  Others:			
*************************************************/
int32 sys_stop_play_leaveword(void);

/*************************************************
  Function:			sys_start_family_record
  Description: 		��ʼ��������¼��
  Input: 			
  	1.FileName		�ļ�·��
  	2.ProcessProc	���Ȼص�����
  	3.StoppedProc	��ǿ����ֹ��ص�����
  Output:			��
  Return:			0-�ɹ� ��0-ʧ��
  Others:			
*************************************************/
int32 sys_start_family_record(char *FileName, MediaPlayCallback ProcessProc, MediaStopedCallback StoppedProc);

/*************************************************
  Function:			sys_stop_family_record
  Description: 		ֹͣ��������¼��
  Input: 			��
  Output:			��
  Return:			0-�ɹ� ��0-ʧ��
  Others:			
*************************************************/
int32 sys_stop_family_record(void);

/*************************************************
  Function:			sys_set_intercomm_state
  Description: 		���õ�ǰ�Խ�״̬
  Input: 			
  	1.state			TRUE-���ڶԽ��� FALSE-�����ڶԽ���
  Output:			��
  Return:			0-�ɹ� ��0-ʧ��
  Others:			
*************************************************/
int32 sys_set_intercomm_state(uint8 state);

/*************************************************
  Function:			sys_set_monitor_state
  Description: 		���õ�ǰ����״̬
  Input: 			
  	1.state			TRUE-���ڼ����� FALSE-�����ڼ�����
  Output:			��
  Return:			0-�ɹ� ��0-ʧ��
  Others:			
*************************************************/
int32 sys_set_monitor_state(uint8 state);

/*************************************************
  Function:			sys_get_media_state
  Description: 		��ȡϵͳý��״̬
  Input: 		
  Output:			��
  Return:			ϵͳý��״̬
  Others:			
*************************************************/
SYS_MEDIA_TYPE sys_get_media_state(void);

/*************************************************
  Function:			sys_stop_cur_media
  Description: 		ֹͣ��ǰý��
  Input: 		
  Output:			��
  Return:			
  Others:			
*************************************************/
void sys_stop_cur_media(void);

/*************************************************
  Function:			sys_set_hint_state
  Description: 		����ϵͳ״̬
  Input: 		
  	1.HintType		ϵͳ״̬����
  	2.flg			��־: TRUE / FALSE
  Output:			��
  Return:			��
  Others:			
*************************************************/
void sys_set_hint_state(SYS_HINT_TYPE HintType, uint8 flg);

/*************************************************
  Function:			sys_get_hint_state
  Description: 		���ϵͳ״̬
  Input: 		
  	1.HintType		ϵͳ״̬����
  Output:			��
  Return:			TRUE / FALSE
  Others:			
*************************************************/
uint8 sys_get_hint_state(SYS_HINT_TYPE HintType);

/*************************************************
  Function:			sys_get_hint_num
  Description: 		���ÿ��״̬����Ϣ����
  Input: 		
  	1.HintType		ϵͳ״̬����
  Output:			��
  Return:			
  Others:			
*************************************************/
uint8 sys_get_hint_num(SYS_HINT_TYPE HintType);

/*************************************************
  Function:			sys_sync_hint_state
  Description: 		ͬ��ϵͳ״̬
  Input: 		
  Output:			��
  Return:			��
  Others:			
*************************************************/
void sys_sync_hint_state(void);

/*************************************************
  Function:			sys_init_hint_state
  Description: 		ϵͳ�ϵ�ʱ��ʼ��ϵͳ״̬
  Input: 		
  Output:			��
  Return:			��
  Others:			
*************************************************/
void sys_init_hint_state(void);

/*************************************************
  Function:			sys_key_beep
  Description: 		������
  Input: 		
  Output:			��
  Return:			��
  Others:			
*************************************************/
void sys_key_beep(void);

#endif
