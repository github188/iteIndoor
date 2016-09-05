/*********************************************************
  Copyright (C), 2009-2012
  File name:	Storage_jrly.h
  Author:   	������
  Version:   	1.0
  Date: 		09.4.7
  Description:  �洢ģ��--��������
  History:                   
    1. Date:
       Author:
       Modification:
    2. ...
*********************************************************/
#ifndef __STORAGE_JRLY_H__
#define __STORAGE_JRLY_H__

#include "storage_include.h"
//#include "storage_types.h"


typedef struct
{
	uint8 UnRead;									// δ����־ 1 - δ�� 0 - �Ѷ� 
	DATE_TIME Time;		    						// ʱ��	           
}JRLY_INFO, * PJRLY_INFO;							// �������Խṹ

// ͨ����¼�б�Ľṹ
typedef struct
{
	uint8 Count;									// ����
	PJRLY_INFO JrlyInfo;				    		// �������Խṹ
}JRLYLIST_INFO, * PJRLYLIST_INFO;

/*************************************************
  Function:		get_jrlyrecord_file
  Description: 	�õ���ǰ����ļ���
  Input:		
  	1.info
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
char* get_jrlyrecord_file(char *filename, PDATE_TIME Time);

/*************************************************
  Function:		storage_free_jrly_memory
  Description: 	�ͷŴ洢�ڴ�
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void storage_free_jrly_memory(PJRLYLIST_INFO *list);

/*************************************************
  Function:		storage_malloc_jrly_memory
  Description: 	����洢�ڴ�
  Input:
  	1.MaxNum	����¼��
  Output:		��
  Return:		��
  Others:
*************************************************/
void storage_malloc_jrly_memory (PJRLYLIST_INFO *list,uint32 MaxNum);
		
/*************************************************
  Function:		get_jrlyrecord_path
  Description: 	�õ���ǰ����ļ���
  Input:		
  	1.info
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
char* get_jrlyrecord_path(char *filename, PDATE_TIME Time);

/*************************************************
  Function:		storage_get_jrlyrecord
  Description: 	��ü������Լ�¼
  Input:		
  	3.Time		ʱ��
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
void storage_get_jrlyrecord(PJRLYLIST_INFO *jrlylist);

/*************************************************
  Function:		storage_add_jrlyrecord
  Description: 	��Ӽ������Լ�¼
  Input:		
  	3.Time		ʱ��
  Output:		��
  Return:		
  Others:
*************************************************/
ECHO_STORAGE storage_add_jrlyrecord (DATE_TIME Time);

/*************************************************
  Function:		storage_del_jrlyrecord
  Description: 	ɾ���������Լ�¼
  Input:		
  	1.Index		����
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
ECHO_STORAGE storage_del_jrlyrecord (uint8 Index);

/*************************************************
  Function:		storage_clear_jrlyrecord
  Description: 	��ռ������Լ�¼
  Input:		
  	1.Index		����
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
ECHO_STORAGE storage_clear_jrlyrecord (void);

/*************************************************
  Function:		storage_set_jrlyrecord_flag
  Description: 	���øü�¼�Ƿ��Ѷ�
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
ECHO_STORAGE storage_set_jrlyrecord_flag(uint32 Index, uint8 State);

/*************************************************
  Function:		storage_set_jrlyrecord_flag
  Description:  ��ȡ�Ƿ���δ��
  Input:		��
  Output:		��
  Return:		TRUE --  ��
  				FALSE -- û��
  Others:		
*************************************************/
uint8 storage_get_jrlyrecord_flag(void);

/*************************************************
  Function:		storage_set_jrlyrecord_size
  Description:  ��ȡ�������Դ�С
  Input:		��
  Output:		��
  Return:		
  Others:		
*************************************************/
uint32 storage_set_jrlyrecord_size(void);

#endif 


