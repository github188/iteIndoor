/*********************************************************
  Copyright (C), 2006-2010, Aurine
  File name:	storage_alarm.h
  Author:   	�޷�»
  Version:   	2.0
  Date: 		2010-4-23
  Description:  �����洢ͷ�ļ�
  
  History:                   
    1. Date:			 
       Author:				
       Modification:	
    2. ...
*********************************************************/
#ifndef __AU_STORAGE_ALARM_H__
#define __AU_STORAGE_ALARM_H__

//#include "logic_include.h"

#define AREA_AMOUNT 			8
#define TELNUM_LEN          	15

#if 0
// �洢��ע����е�����
typedef struct
{
	uint8 		b_write;							// �жϰ�����Ϣ�Ƿ��ʼ����	
	uint8   	defend_state;						// ����״̬,�ڼң������ҹ��

	// ������ʾҳ���������ݣ�Ϊһ���ֽڱ�ʾ�˷���(����ʱ��,��ʱ����ʱ��,���ͳ���)
	uint8 		enable;		   						// ���� ��ر�   
	uint8       is_24_hour;							// �Ƿ�24Сʱ����
	uint8		finder_state;						// ̽ͷ״̬
	uint8       can_hear;                           // ����
	uint8       can_see;  							// �ɼ�
	uint8   	area_type[AREA_AMOUNT];				// ������Ӧ��ͼ������
	uint8       delay_time[AREA_AMOUNT];            // ��ʱ����ʱ��
	uint8   	alarm_time;							// ����ʱ��

	// �û�����ҳ������
	uint8		exit_alarm_time;				    // �˳�Ԥ��ʱ��
	uint8 		isolation;							// ��ʱ����	1 = ��ʱ����	
	uint8       part_valid;							// �Ƿ��Ǿַ���Ч 1 = ��Ч��0 = ��Ч
	uint8 		remote_set;							// Զ�̰������� bit0:Զ�̰�������,bit1:Զ�̳���,bit2:Զ�̲���,bit3:Զ�ַ̾�,bit4:�����Ƿ��Ͷ���,bit5:ס���ݿ�ˢ���Ƿ񳷷�
	uint8       link_param;							// ��������������
	uint8  		short_msg[2][TELNUM_LEN];  			// ���ź���

	// ������ʾҳ�棬ֻ������һ��������ʾ
	uint8 		show_state[AREA_AMOUNT];			// ����������ʾ״̬
}AF_FLASH_DATA, * PAF_FLASH_DATA;
#endif


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

typedef struct 
{
	uint8 			type;							// ����,����,�ַ�,��·,ȡ����·
	EXECUTOR_E 		executor;						// ������
	DATE_TIME	 	time;							// ����ʱ��
}AF_CZ_REC, * PAF_CZ_REC;							// ����������¼

typedef struct
{
	uint8 		nCount;
	PAF_CZ_REC	pAfCzRec;
}AF_CZ_LIST, * PAF_CZ_LIST;							// ����������¼�б�

typedef struct
{
	uint8			areaNum;		 				// ������
	uint8     		bReaded;		 				// �Ƿ��Ѷ�
	uint8     		type;			 				// ���������� ���ϡ�����
	DATE_TIME		time;	 						// ��������ʱ��
}AF_BJ_REC, * PAF_BJ_REC;							// ����������¼

typedef struct
{
	uint32 		nCount;
	PAF_BJ_REC	pAfBjRec;
}AF_BJ_LIST, * PAF_BJ_LIST;							// ����������¼�б�

typedef struct tagTOUCH_INFO{
	uint8 	 time[20];   							//��������ʱ�� yyyy-mm-dd hh:mm:ss 
	uint8     TouchNum;								//������
	uint8     bReaded;								//�Ƿ��Ѷ�
	uint8     bClean;								//�Ƿ������
	uint32     id;									//����������¼��ID
	uint8     type;									//����������
}ALARM_TOUCH_INFO,*PALARM_TOUCH_INFO;

typedef struct
{
	uint32 		nCount;
	PALARM_TOUCH_INFO	pAlarmRec;
}ALARM_TOUCH_INFO_LIST, *PALARM_TOUCH_INFO_LIST;							// ����������¼�б�


/*************************************************
  Function:		storage_get_afbj_unread_record
  Description:  ��հ���������¼
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
PALARM_TOUCH_INFO_LIST storage_get_afbj_unread_record(void);

/*************************************************
  Function:		storage_get_afcz_record
  Description: 	��ȡ����������¼�б�
  Input:		��
  Output:		��
  Return:		��
  Others:		
  	1.��ָ�벻Ҫ�ͷ�
*************************************************/
PAF_CZ_LIST storage_get_afcz_record (void);

/*************************************************
  Function:		storage_get_afbj_record
  Description: 	��ȡ����������¼�б�
  Input:		��
  Output:		��
  Return:		��
  Others:		
  	1.��ָ�벻Ҫ�ͷ�
*************************************************/
PAF_BJ_LIST storage_get_afbj_record (void);

/*************************************************
  Function:		storage_add_afcz_record
  Description: 	���Ӱ���������¼
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
ECHO_STORAGE storage_add_afcz_record(uint8 type, EXECUTOR_E executor);

/*************************************************
  Function:		storage_add_afbj_record
  Description: 	���Ӱ���������¼
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
ECHO_STORAGE storage_add_afbj_record(uint8 areaNum, uint8 type);

/*************************************************
  Function:		storage_clear_afcz_record
  Description:  ��հ���������¼
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void storage_clear_afcz_record(void);

/*************************************************
  Function:		storage_clear_afbj_record
  Description:  ��հ���������¼
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void storage_clear_afbj_record(void);

/*************************************************
  Function:		storage_clear_afbj_unread_state
  Description:  ��հ���������¼δ��״̬
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void storage_clear_afbj_unread_state(void);

/*************************************************
  Function:		storage_af_timer_save
  Description: 	��ʱ�洢����ģ������
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void storage_af_timer_save(void);

/*************************************************
  Function:		storage_af_init
  Description: 	����ģ��洢��ʼ��
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void storage_af_init(void);

/*************************************************
  Function:    		storage_get_yj_wc_path
  Description: 		��ȡ"�����غ���,�ټ�"������·��
  Input:			��
  Output:    		��
  Return:			·��
  Others:    		       
*************************************************/
char * storage_get_yj_wc_path(void);

/*************************************************
  Function:    		storage_get_yjq_path
  Description: 		��ȡ"��ע���о���"������·��
  Input:			��
  Output:    		��
  Return:			·��
  Others:    		       
*************************************************/
char * storage_get_yjq_path(void);

/*************************************************
  Function:    		storage_get_fqcf_path
  Description: 		��ȡ"�����������ܲ���"������·��
  Input:			��
  Output:    		��
  Return:			·��
  Others:    		       
*************************************************/
char * storage_get_fqcf_path(void);

/*************************************************
  Function:    		storage_get_jjyjc_path
  Description: 		��ȡ"��ã������ѽ��"������·��
  Input:			��
  Output:    		��
  Return:			·��
  Others:    		       
*************************************************/
char * storage_get_jjyjc_path(void);

/*************************************************
  Function:    		storage_get_wanan_path
  Description: 		��ȡ"��"������·��
  Input:			��
  Output:    		��
  Return:			·��
  Others:    		       
*************************************************/
char * storage_get_wanan_path(void);

#if 0
/*************************************************
  Function:    		storage_get_yj_path
  Description: 		��ȡԤ��������·��
  Input:			��
  Output:    		��
  Return:			·��
  Others:    		       
*************************************************/
char * storage_get_yj_path(void);
#endif

/*************************************************
  Function:    		storage_get_bj_path
  Description: 		��ȡ����������·��
  Input:			��
  Output:    		��
  Return:			·��
  Others:    		       
*************************************************/
char * storage_get_bj_path(void);

/*************************************************
  Function:    		storage_get_area_enable
  Description: 		��÷������ò���
  Input:			��
  Output:    		��
  Return:			�������ò���
  Others:    		       
*************************************************/
uint8 storage_get_area_enable(void);

/*************************************************
  Function:    		storage_set_area_enable
  Description: 		���÷������ò���
  Input:
  	1.time 			����ʱ��
  Output:    		
  Return:			false:ʧ�� true:�ɹ�			
  Others:    		       
*************************************************/
uint32 storage_set_area_enable(uint8 time);
/*************************************************
  Function:    		storage_get_gsm_num
  Description: 		��ö��ź���
  Input:
  Output:
   	1.num1 			�绰����1
   	2.num2			�绰����2
  Return:			
  Others:           
*************************************************/
void stroage_get_gsm_num(uint8 *num1, uint8 *num2);

/*************************************************
  Function:    		storage_set_gsm_num
  Description: 		���ö��ź���
  Input:
  	1.num1         	1:���ź���1 
  	2.num2			2:���ź���2
  Output:    		
  Return:			TRUE - �ɹ�, FALSE - ʧ��
  Others:           
*************************************************/
uint32 storage_set_gsm_num(uint8 *num1, uint8 *num2);

/*************************************************
  Function:    		storage_get_remote_set
  Description: 		��ȡԶ�����ò���
  Input:			��
  Output:    		��
  Return:			Զ�����ò���	
  Others:           bit0 = 1: Զ�̰���������Ч
  					bit1 = 1: ����Զ�̳���
  					bit2 = 1: ����Զ�̲���
  					bit3 = 1: ����Զ�ַ̾�
*************************************************/
uint8 storage_get_remote_set(void);

/*************************************************
  Function:    		storage_set_remote_set
  Description: 		����Զ�����ò���
  Input:
  	1.remote		Զ�����ò���
  Output:    		
  Return:			false:ʧ�� true:�ɹ�	
  Others:           
  					bit0 = 1: Զ�̰���������Ч
  					bit1 = 1: ����Զ�̳���
  					bit2 = 1: ����Զ�̲���
  					bit3 = 1: ����Զ�ַ̾�
*************************************************/
uint8 storage_set_remote_set(uint8 remote);

/*************************************************
  Function:    		storage_get_link_param
  Description: 		��ȡ��������
  Input:
  Output:    		
  Return:			��������	
  Others:           bit0 = 1 �ݿ�ˢ������
  					bit1 = 1 �ſ�ˢ������
  					bit3 = 1 ���ò��������龰ģʽ
  					bit5 = 1 ���ó��������龰ģʽ
*************************************************/
uint8 storage_get_link_param(void);

/*************************************************
  Function:    		storage_set_link_param
  Description: 		������������
  Input:
  	1.link			��������
  Output:    		
  Return:			false:ʧ�� true:�ɹ�	
  Others:           bit0 = 1 �ݿ�ˢ������
  					bit1 = 1 �ſ�ˢ������
  					bit3 = 1 ���ò��������龰ģʽ
  					bit5 = 1 ���ó��������龰ģʽ
*************************************************/
uint8 storage_set_link_param(uint8 link);

/*************************************************
  Function:    		storage_get_link_scene
  Description: 		��ȡ���������龰ģʽ
  Input:
  Output:  
   	1.type			�����龰ģʽ������
  Return:				
  Others:           
*************************************************/
void storage_get_link_scene(uint8 * type);

/*************************************************
  Function:    		storage_set_link_scene
  Description: 		���ð��������龰ģʽ
  Input:
  Output:  
   	1.type			�����龰ģʽ������
  Return:				
  Others:           
*************************************************/
uint8 storage_set_link_scene(uint8 * type);

/*************************************************
  Function:    		storage_get_area_type
  Description: 		��ȡ����ͼ������
  Input:
  Output:  
   	1.area_type		����ͼ������,ÿ������һ���ֽ�
  Return:				
  Others:           
*************************************************/
void storage_get_area_type(uint8 * area_type);

/*************************************************
  Function:    		storage_get_defend_state
  Description: 		��ȡ��������״̬
  Input:
  Output:    		
  Return:			����״̬
  Others:           
*************************************************/
DEFEND_STATE storage_get_defend_state(void);

/*************************************************
  Function:    		storage_get_time_param
  Description: 		��ȡ�����Ĳ���
  Input:
  Output: 
  	1.para			���� 1;����ʱ�� 2:�˳�Ԥ��ʱ�� 3:�Ƿ�����Ԥ����
  Return:			�Ƿ������˳�Ԥ����
  Others:           
*************************************************/
void storage_get_time_param(uint8 param[3]);

/*************************************************
  Function:    		storage_set_time_param
  Description: 		���������Ĳ���
  Input:
  	1.para			���� 1;����ʱ�� 2:�˳�Ԥ��ʱ�� 3:�Ƿ�����Ԥ����
  Output: 
  Return:			�Ƿ������˳�Ԥ����
  Others:           
*************************************************/
uint8 storage_set_time_param(uint8 param[3]);

/*************************************************
  Function:    		storage_get_validate_code
  Description: 		��֤��
  Input:
  Output: 
  Return:			��֤��
  Others:           
*************************************************/
uint32 storage_get_validate_code(void);

/*************************************************
  Function:    		storage_set_validate_code
  Description: 		������֤��
  Input:
  	1.validate_code	��֤��
  Output: 
  Return:			
  Others:           
*************************************************/
uint8 storage_set_validate_code(uint32 validate_code);

/*************************************************
  Function:    		storage_get_alarm_state
  Description: 		��ȡ����״̬
  Input:			��
  Output:    		��
  Return:			0: ���� 1:���� 2:����
  Others:           
*************************************************/
uint8 storage_get_alarm_state(void);

/*************************************************
  Function:    		storage_get_alarm_undeal_num
  Description: 		������������
  Input:			��
  Output:    		��
  Return:			
  Others:           
*************************************************/
uint8 storage_get_alarm_undeal_num(void);
#endif

