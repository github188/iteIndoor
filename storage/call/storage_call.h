/*********************************************************
  Copyright (C), 2009-2012
  File name:	storage_call.h
  Author:   	������
  Version:   	1.0
  Date: 		09.4.7
  Description:  �洢ģ��--�Խ�
  History:                   
    1. Date:
       Author:
       Modification:
    2. ...
*********************************************************/
#ifndef __STORAGE_CALL_H__
#define __STORAGE_CALL_H__

#include "storage_include.h"

#define MAX_RECORD_NUM  		20					// ����¼
#define MAX_DEVNO_NUM			30
typedef enum
{
    MISSED = 0x00,									// δ������	
	INCOMING,										// �ѽӵ绰
	OUTGOING,										// �Ѳ��绰
	ALLCALL											// ȫ��ͨ��
}CALL_TYPE;

// ͨ����¼�Ľṹ
typedef struct
{
	CALL_TYPE Calltype;								// ��������
	uint8 UnRead;									// δ����־ 1 - δ�� 0 - �Ѷ� 
	uint8 LylyFlag;									// ��Ӱ����״̬
	char devno[MAX_DEVNO_NUM];						// �豸���
	DEVICE_TYPE_E Type;								// �豸����
	ZONE_DATE_TIME Time;		    				// ͨ����ʱ��	
}MCALLINFO, * PMCALLINFO;

// ͨ����¼�б�Ľṹ
typedef struct
{
	uint8 CallCount;								// ����
	PMCALLINFO CallInfo;			    			// ͨ����¼
}MCALLLISTINFO, * PMCALLLISTINFO;		

// ����ɾ��ʹ�ýṹ��
typedef struct 
{
	uint8 Counts;									// ɾ������
	uint8 DelFlg[MAX_RECORD_NUM];					// flg����ֵΪ1����Ҫɾ��
}DEL_LIST, *PDEL_LIST;
/**************************  ���¼����б���Ϣ  ***********************************/
#define MAX_MONITOR_NUM  		20					// ����¼

// ������Ϣ
typedef struct
{
	uint8 	DeviceType;                         	// �豸���� 
	uint8   index;									// �豸���
	uint32  IP;										// �豸IP
	
}MONITORINFO, *PMONITORINFO;

// ͨ����¼�б�Ľṹ
typedef struct
{
	uint8 MonitorCount;								// ����
	PMONITORINFO pMonitorInfo;			    		// ������Ϣ
}MONITORLISTINFO, *PMONITORLISTINFO;	

/**************************  ���ϼ����б���Ϣ  ***********************************/

/*************************************************
  Function:		storage_get_netdoor_ip
  Description: 	����IP��Ϣ
  Input:			
 	1.Index		����
  Output:		��
  Return:		��
  Others:
*************************************************/
uint32 storage_get_netdoor_ip(uint8 Index);

/*************************************************
  Function:		storage_get_monitorlist
  Description: 	 ��ȡ�����б���Ϣ
  Input:		
  	1.Calltype
  Output:		��
  Return:		��
  Others:		
  	1.��ָ���ⲿ�ͷ�
*************************************************/
PMONITORINFO storage_get_monitorinfo (DEVICE_TYPE_E devtype, int8 index);

/*************************************************
  Function:		storage_get_callrecord
  Description: 	���ͨ����¼
  Input:		
  	1.Calltype
  Output:		��
  Return:		��
  Others:		
  	1.��ָ���ⲿ�ͷ�
*************************************************/
PMCALLLISTINFO storage_get_callrecord (CALL_TYPE Calltype);

/*************************************************
  Function:		storage_add_callrecord
  Description: 	���ͨ����¼
  Input:		  
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
ECHO_STORAGE storage_add_callrecord (PMCALLINFO pcallinfo);

/*************************************************
  Function:		storage_del_callrecords
  Description: 	ɾ������ͨ����¼
  Input:		
  	1.Calltype	��¼����
  	2.DelList	ɾ���б�
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
ECHO_STORAGE storage_del_callrecord (CALL_TYPE Calltype, PDEL_LIST DelList);

/*************************************************
  Function:		storage_clear_callrecord
  Description: 	���ͨ����¼
  Input:		
  	1.Calltype	��¼����
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
ECHO_STORAGE storage_clear_callrecord (CALL_TYPE Calltype);

/*************************************************
  Function:		storage_set_callrecord_lylyflag
  Description: 	���øü�¼�Ƿ��й�������Ӱ����
  Input:		
  	1.time
  	2.LylyFlag
  Output:		��
  Return:		��
  Others:
*************************************************/
FLAG_STORAGE storage_set_callrecord_lylyflag(ZONE_DATE_TIME time, uint8 LylyFlag);

/*************************************************
  Function:		storage_clear_all_callrecord
  Description:  ��նԽ�ģ������ͨ����¼
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void storage_clear_all_callrecord(void);

/*************************************************
  Function:		storage_get_callrecord_state
  Description:  ��ȡ�Ƿ���δ������
  Input:		��
  Output:		��
  Return:		TRUE -- ��δ������
  				FALSE -- û��δ������
  Others:		
*************************************************/
uint8 storage_get_callrecord_state(void);

/*************************************************
  Function:		storage_set_callrecord_state
  Description:  ��δ����������Ϊ�Ѷ�״̬
  Input:		��
  Output:		��
  Return:		��
  Others:		
*************************************************/
void storage_set_callrecord_state(PMCALLLISTINFO calllist);

/*********************************************************/
/*					�豸���豸��������					 */	
/*********************************************************/

/*************************************************
  Function:		storage_get_devparam
  Description: 	��ȡ�豸��Žṹ
  Input:		
  Output:		��
  Return:		�豸��Žṹ
  Others:
*************************************************/
PFULL_DEVICE_NO storage_get_devparam(void);

/*************************************************
  Function:    		storage_get_devno
  Description:		����豸���
  Input: 			��
  Output:			��
  Return:			�豸���
  Others:
*************************************************/
DEVICE_NO storage_get_devno(void);

/*************************************************
  Function:    		get_devno_str
  Description:		����豸����ַ���
  Input: 			��
  Output:			��
  Return:			�豸����ַ���
  Others:
*************************************************/
char * storage_get_devno_str(void);

/*************************************************
  Function:    		storage_get_use_cell
  Description:		��ȡ�Ƿ����õ�Ԫ��
  Input: 			��
  Output:			��
  Return:			1:����0:δ����
  Others:
*************************************************/
uint32 storage_get_use_cell (void);

/*************************************************
  Function:    		storage_set_areano
  Description:		��������
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
uint32 storage_set_areano (uint32 areano);

/*************************************************
  Function:    		storage_get_areano
  Description:		��ȡ����
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
uint32 storage_get_areano (void);

/*************************************************
  Function:    		set_devno_rule
  Description:		�����豸��Ź���
  Input: 
  	1.save			�Ƿ񱣴�
	2.rule			�豸��Ź���
  Output:			��
  Return:			0-�ɹ� 1-��Ԫ���ȷǷ� 2-�ݺų��ȷǷ� 3-���ų��ȷǷ�
  					4-�ܳ��Ȳ��ܴ���18
  Others:
*************************************************/
uint32 storage_set_devno_rule(uint8 save, DEVICENO_RULE Rule);

/*************************************************
  Function:    		storage_set_devno
  Description:		�����豸��
  Input: 
  	1.DEVICE_TYPE_E	�豸����
	2.devno			�豸���ַ���
  Output:			��
  Return:			
  	0-�ɹ� 
  	1-���ȷǷ� 
  	2-���Ŷ�ȫΪ0   
  	3-���ڻ���ű�����1-40֮�� 
  Others:
*************************************************/
uint32 storage_set_devno(uint8 save, DEVICE_TYPE_E DeviceType, char* DevNo);

/*************************************************
  Function:    		storage_save_devno
  Description:		�����豸���
  Input: 
  	1.save			�Ƿ񱣴�
  	2.rule			�豸��Ź���
	3.devno			�豸���ַ���
  Output:			��
  Return:			��
  Others:			��
*************************************************/
void storage_save_devno(uint8 save, DEVICENO_RULE Rule, char* DevNo);

/*************************************************
  Function:    		storage_get_dev_desc
  Description:		��ȡ�豸������
  Input: 			��
  Output:			��
  Return:			�豸������ָ��
  Others:
*************************************************/
char * storage_get_dev_desc(void);

/*************************************************
  Function:    		storage_set_dev_desc
  Description:		��ȡ�豸������
  Input: 			��
  Output:			��
  Return:			�豸������ָ��
  Others:
*************************************************/
uint8 storage_set_dev_desc(char * str);

/*************************************************
  Function:    		storage_init_get_dev_desc
  Description:		��ʼ��ʱ��ȡ�豸����
  Input:
  	1.DevDesc		������ַ
  	2.language		��������
  Output:			��
  Return:			�豸������ָ��
  Others:
*************************************************/
void storage_init_get_dev_desc(char * DevDesc, LANGUAGE_E language);

/*************************************************
  Function:		storage_call_init
  Description: 	�Խ�ģ��洢��ʼ��
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void storage_call_init(void);

#if 0
/*************************************************
  Function:			stroage_sdp_param
  Description:		��ʼ����Ƶ�������
  Input: 	
  Output:			��
  Return:			�ɹ����, TRUE/FALSE
  Others:	
*************************************************/
void storage_sdp_param_init(void);

/*************************************************
  Function:			storage_init_sysparam
  Description:		��ʼ��ϵͳ����
  Input: 	
  Output:			��
  Return:			�ɹ����, TRUE/FALSE
  Others:	
*************************************************/
void storage_init_sysparam(void);
#endif

/*************************************************
  Function:		storage_get_monitorlist
  Description: 	 ��ȡ�����б���Ϣ
  Input:		
  	1.Calltype
  Output:		��
  Return:		��
  Others:		
  	1.��ָ���ⲿ�ͷ�
*************************************************/
PMONITORLISTINFO storage_get_monitorlist(DEVICE_TYPE_E devtype);

/*************************************************
  Function:		save_call_storage
  Description: 	��ģ��洢
  Input:	
  	1.mode
  	2.CallList	
  Output:		��
  Return:		��
  Others:
*************************************************/
void save_monitorlist_storage(DEVICE_TYPE_E Type, PMONITORLISTINFO monitorlist);

/*************************************************
  Function:		free_monitorlist_memory
  Description: 	�ͷŶԽ��洢�ڴ�
  Input:		
  	1.CallList
  Output:		��
  Return:		��
  Others:
*************************************************/
void free_monitorlist_memory(PMONITORLISTINFO* monitorlist);

/*************************************************
  Function:		malloc_call_memory
  Description: 	����Խ��洢�ڴ�
  Input:
  	1.CallList
  	2.MaxNum	����¼��
  Output:		��
  Return:		��
  Others:
*************************************************/
void malloc_monitorlist_memory (PMONITORLISTINFO *MonitorList,uint32 MaxNum);

/*************************************************
  Function:		storage_get_devrule
  Description: 	��ȡ�豸����ṹ
  Input:		
  Output:		��
  Return:		�豸��Žṹ
  Others:
*************************************************/
DEVICENO_RULE* storage_get_devrule(void);

/*************************************************
  Function:		storage_clear_monitorlist
  Description: 	��մ洢�ļ�����
  Input:		
  	1.Type		�豸����
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
void storage_clear_monitorlist(DEVICE_TYPE_E Type);

#endif 

