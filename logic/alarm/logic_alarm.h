/*********************************************************
  Copyright (C), 2006-2010, Aurine
  File name:	logic_alarm.h
  Author:   	�޷�»
  Version:   	2.0
  Date: 		2010-4-29
  Description:  �����߼�����ͷ�ļ�
  
  History:                   
    1. Date:			 
       Author:				
       Modification:	
    2. ...
*********************************************************/
#ifndef __LOGIC_ALARM_H__
#define __LOGIC_ALARM_H__

#include "logic_alarm_net.h"

typedef int32 (*ALARMGUI_CALLBACK)(int32 Param1, int32 Param2);
typedef void  (*SHOW_SYSEVENHIT)(uint16 EventType);

typedef enum 
{
	URGENCY_ALARM = 0x00,							// ������Ȱ�ť
	GAS_DETECTOR,									// ��˹����̽����
	DEF_DOOR_INBREAK_DETECTOR,						// ���ŷ�����̽����
	DEF_SPACE_INBREAK_DETECTOR,						// �ռ����̽����
	DEF_WINDOW_INBREAK_DETECTOR1,					// ����������̽����1
	DEF_WINDOW_INBREAK_DETECTOR2,					// ����������̽����2
	DEF_BALCONY_INBREAK_DETECTOR,					// ��̨������̽����
	USER_DEFINED									// �Զ�������	
}AREA_TYPE;

#if 0
// ����״̬
typedef enum
{
   DIS_DEFEND = 0x00,								// ����
   SET_DEFEND,								    	// ����
   PART_DEFEND,										// �ַ�
   RESERVE                            				// ����
}DEFEND_STATE;

// �����ȼ����У����ܸ���˳��,��·���ϲ���ԶԵ�������
typedef enum
{
   UNENABLE_SHOW_STATE = 0x00,						// δ����
   ISO_SHOW_STATE,								   	// ����
   ALARM_SHOW_STATE,								// ����
   WORK_SHOW_STATE,                            		// ������������״̬
   NORM_SHOW_STATE									// ������������״̬
}SHOW_STATE;
#endif

// ������ť����ķ���ֵ����
typedef enum
{
	AF_SUCCESS = 0x00,								// �ɹ�
	AF_NO_REMOTE_CTRL,								// ���������������Զ�̿���
	AF_SETTING_ERR,									// �������ڳ���״̬����ʧ��
	AF_BAOJINGNOBUFANG,								// ���ڱ������ܲ���
	AF_FQCHUFA,										// ����ʱ�з�����������
	AF_FQGUZHANG,									// ����ʱ�з������ϴ���
	AF_STATE_SAME,									// ��ǰ�����ò���״̬��������ǰ״̬��ͬ
	AF_BAOJING,										// ��ǰ���ڱ���״̬
	AF_CANNOT_OVERLEAP								// ����Խ����ҹ��ģʽ(�ַ�)����ֱ�ӽ������ģʽ(����)�����ģʽ����ֱ�ӽ����ڼ�ģʽ(����)	
}AF_RETURN_TYPE; 

// �����ص�����
typedef enum
{
	BAOJING_SHOW = 0x00,							// ǿ����ʾ��������
	BAOJING_FRESH,									// ����״̬��ʾ����ʱ����Ҫˢ�½��������
	BAOJING_DUIJIANG,								// ����ʱ�����ڶԽ�״̬
	BAOJING_KEY										// �����ı�������
}AF_CALLBACK_TYPE;

typedef enum 
{
	NO_SOUND_TYPE = 0x00,							// ������״̬
	OUT_SOUND_TYPE,									// �˳�Ԥ������״̬
	IN_SOUND_TYPE,									// ����Ԥ������״̬
	ALARM_SOUND_TYPE,								// ��������״̬
	STOP_ALL_SOUND_TYPE								// �ر�����������������
}SOUND_TYPE;

#define SET_ONE_VALUE(val,state,pos) ((0==state)?(val&=(~(1<<pos))):(val|=(1<<pos)))

/*************************************************
  Function:    		alarm_get_area_define
  Description: 		�������ҳ����ʾ��Ϣ
  Input:
  Output:    		
  	1.define_info	����ҳ����ʾ����Ϣ
  Return:				
  Others:    		       
  *************************************************/
void alarm_get_area_define(uint8 * define_info);

/*************************************************
  Function:    		alarm_set_area_define
  Description: 		���ñ�������ҳ����ʾ��Ϣ
  Input:
   	1.define_info	���Ե�����
  Output:    		
  Return:			false:ʧ�� true:�ɹ�				
  Others:           
*************************************************/
uint8 alarm_set_area_define(uint8 *define_info);

/*************************************************
  Function:    		alarm_get_area_isolation
  Description: 		��ȡ�����������
  Input:
  Output:   
  Return:			��ʱ�������
  Others:           
*************************************************/
uint8 alarm_get_area_isolation(void);

/*************************************************
  Function:    		alarm_set_area_isolation
  Description: 		���ñ�����ʱ������ʾ��Ϣ
  Input:
  	1.iso_info		��ʱ������ʾ��Ϣ
  Output:    		
  Return:			false:ʧ�� true:�ɹ�
  Others:           
*************************************************/
uint8 alarm_set_area_isolation(uint8 iso_info);

/*************************************************
  Function:    		alarm_get_part_valid
  Description: 		��ȡ�����ַ���Ч����
  Input:
  Output:   
  Return:			�ַ���Ч����	
  Others:           
*************************************************/
uint8 alarm_get_part_valid(void);

/*************************************************
  Function:    		alarm_set_area_part_valid
  Description: 		���ñ����ַ���Чҳ����ʾ��Ϣ
  Input:
  	1.valid			�ַ���Чҳ����ʾ��Ϣ
  Output:    		
  Return:			false:ʧ�� true:�ɹ�
  Others:           
*************************************************/
uint8 alarm_set_area_part_valid(uint8 valid);

/*************************************************
  Function:    		center_get_area_define
  Description: 		���Ĺ������ȡ�ķ�������
  Input:
  Output:  
   	1.define 		���Ĺ������ȡ������
  Return:				
  Others:         	
  					���Ĺ������ȡ�ķ���������
  					�����������ò�������������
*************************************************/
void center_get_area_define(ALARM_DEFINE_NET_DATA * alarm_define);

/*************************************************
  Function:    		center_get_area_state
  Description: 		���Ĺ�����Ļ�ȡ�İ���״̬
  Input:			��
  Output:		
  	1.status 		���Ĺ������ȡ�İ���״̬�ṹ��
  Return:			��	
  Others:           
*************************************************/
void center_get_area_state(ALARM_STATUES_NET_DATA * status);

/*************************************************
  Function:    		ipad_get_area_state
  Description: 		ipad��ȡ����״̬
  Input:			��
  Output:		
  	1.status 		ipad��ȡ�İ���״̬�ṹ��
  Return:			��	
  Others:           
*************************************************/
void ipad_get_area_state(ALARM_STATUES_NET_DATA * status);

/*************************************************
Function:    		get_time_for_timetype
Description: 		����ʱ�����ͻ�ȡʱ��
Input:
1.sound_type	��������
2.time_type     ʱ������
Output:    		��
Return:
Others:
*************************************************/
uint16 get_time_for_timetype(SOUND_TYPE sound_type, uint8 time_type);

/*************************************************
  Function:			check_defend_oper
  Description:		����״̬����
  Input: 
  	1.state			����״̬
  	2.exec			ִ����
  Output:			��
  Return:			��
  Others:			��
*************************************************/
AF_RETURN_TYPE check_defend_oper(DEFEND_STATE state, EXECUTOR_E exec);

/*************************************************
  Function:			alarm_clear_alerts_operator
  Description:		�����ʾ����
  Input: 		
  Output:			��
  Return:			
  Others:			��
*************************************************/
void alarm_clear_alerts_operator(void);

/*************************************************
  Function:			alarm_set_operator
  Description:		��������
  Input: 
   	1.exec			ִ����
  Output:			��
  Return:			��
  Others:			��
*************************************************/
AF_RETURN_TYPE alarm_set_operator(EXECUTOR_E exec);

/*************************************************
  Function:			alarm_single_set_operator
  Description:		��������������
  Input: 		
  	1.num			������
  Output:			��
  Return:			��
  Others:			��
*************************************************/
AF_RETURN_TYPE alarm_single_set_operator(uint8 num);

/*************************************************
  Function:			alarm_unset_operator
  Description:		��������
  Input: 
  	1.exec			ִ����
  	2.force_type    1:Ю�� 0:����
  Output:			��
  Return:			��
  Others:			��
*************************************************/
AF_RETURN_TYPE alarm_unset_operator(EXECUTOR_E exec, uint8 force_type);

/*************************************************
  Function:			alarm_partset_operator
  Description:		�ַ�����
  Input: 	
  	1.exec			ִ����
  Output:			��
  Return:			��
  Others:			��
*************************************************/
AF_RETURN_TYPE alarm_partset_operator(EXECUTOR_E exec);

/*************************************************
  Function:			alarm_logic_init
  Description:		�����߼���ʼ��
  Input: 		
  Output:			��
  Return:			��
  Others:			��
*************************************************/
void alarm_logic_init(void);

/*************************************************
  Function:    		alarm_init_gui_callback
  Description: 		����GUI �ص�����
  Input:
  	1.func			�ص�����
  Output:    		��
  Return:			��
  Others:           
*************************************************/
//void alarm_init_gui_callback(ALARMGUI_CALLBACK func);
void alarm_init_gui_callback(ALARMGUI_CALLBACK func, SHOW_SYSEVENHIT func1);

/*************************************************
  Function:    		logic_get_area_isolation
  Description: 		��ȡ�����������
  Input:
  Output:   
  Return:			��ʱ�������
  Others:           
*************************************************/
uint8 logic_get_area_isolation(void);

/*************************************************
  Function:    		logic_set_area_isolation
  Description: 		���÷�����ʱ�������
  Input:
  	1.iso_info		1- ��ʱ����
  Output:    		
  Return:			false:ʧ�� true:�ɹ�
  Others:           ÿһλ����һ������
*************************************************/
uint8 logic_set_area_isolation(uint8 iso_info);

/*************************************************
  Function:    		logic_get_part_valid
  Description: 		��ȡ�����ַ���Ч����
  Input:
  Output:   
  Return:			�ַ���Ч����	
  Others:           
*************************************************/
uint8 logic_get_part_valid(void);

/*************************************************
  Function:    		logic_set_part_valid
  Description: 		���÷����ַ���Ч����
  Input:
  	1.valid			�ַ���Ч����
  Output:    		
  Return:			false:ʧ�� true:�ɹ�
  Others:           
*************************************************/
uint8 logic_set_part_valid(uint8 valid);

/*************************************************
  Function:    		logic_get_alarm_param
  Description: 		��ð�������
  Input:
  Output:    		
  	1.param_info	��������
  Return:				
  Others:    		       
*************************************************/
void logic_get_alarm_param(uint8 *param_info);

/*************************************************
  Function:    		logic_set_alarm_param
  Description: 		���ð�������
  Input:
   	1.param_info	��������
  Output:    		
  Return:			false:ʧ�� true:�ɹ�				
  Others:           
*************************************************/
uint8 logic_set_alarm_param(uint8 *param_info);

/*************************************************
  Function:    		alarm_gui_callback
  Description: 		ִ�а����ص�
  Input:
  	1.Param1		����1		
  	2.Param2		����2
  Output: 			��   		
  Return:			��
  Others:           
*************************************************/
void alarm_gui_callback(int32 Param1, int32 Param2);

/*************************************************
  Function:    		alarm_get_24clear_param  
  Description: 		��÷�24Сʱ����������־
  Input:
  Output:    		
  	1.param	��24Сʱ����������־����
  Return:				
  Others:    add by wufn		       
*************************************************/
uint8 alarm_get_no24touch_param(void);

/*************************************************
  Function:    		alarm_get_24clear_param  
  Description: 		��÷�24Сʱ��������
  Input:
  Output:    		
  	1.param	��24Сʱ����������־����
  Return:				
  Others:    add by wufn		       
*************************************************/
uint8 alarm_get_no24touch_param(void);

/*************************************************
  Function:    	logic_get_g_whole_alarm_state_param  
  Description: 		��ȡ����������־
  Input:
  Output:    		
  	1.param	����������־
  Return:				
  Others:    add by wufn		       
*************************************************/
uint8 logic_get_g_whole_alarm_state_param(void);

#endif

