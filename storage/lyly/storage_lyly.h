/*********************************************************
  Copyright (C), 2009-2012
  File name:	Storage_Lyly.h
  Author:   	������
  Version:   	1.0
  Date: 		09.4.7
  Description:  �洢ģ��--��Ӱ����
  History:                   
    1. Date:
       Author:
       Modification:
    2. ...
*********************************************************/
#ifndef __STORAGE_LYLY_H__
#define __STORAGE_LYLY_H__

#include "storage_include.h"
//#include "storage_types.h"


typedef struct
{
	uint8 UnRead;									// δ����־ 1 - δ�� 0 - �Ѷ� 
	char DevNo[20];									// �豸��
	DEVICE_TYPE_E DevType;							// �豸����
	LYLY_TYPE LyType;								// ��������
	DATE_TIME Time;		    						// ʱ��	           
}LYLY_INFO, * PLYLY_INFO;							// ��Ӱ���Խṹ

// ͨ����¼�б�Ľṹ
typedef struct
{
	uint8 Count;									// ����
	PLYLY_INFO LylyInfo;				    		// ��Ӱ���Խṹ
}LYLYLIST_INFO, * PLYLYLIST_INFO;

/*************************************************
  Function:		storage_free_lyly_memory
  Description: 	�ͷŴ洢�ڴ�
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void storage_free_lyly_memory(PLYLYLIST_INFO *list);

/*************************************************
  Function:		storage_malloc_lyly_memory
  Description: 	����洢�ڴ�
  Input:
  	1.MaxNum	����¼��
  Output:		��
  Return:		��
  Others:
*************************************************/
void storage_malloc_lyly_memory (PLYLYLIST_INFO *list,uint32 MaxNum);

/*************************************************
  Function:		get_lylyrecord_path
  Description: 	�õ���ǰ����ļ���
  Input:
  	1.filename
  	2.info
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
char* get_lylyrecord_path(char *filename, PDATE_TIME Time) ;

/*************************************************
  Function:		get_lylypic_path
  Description: 	�õ���ǰ���ͼƬ�ļ���
  Input:		
  	1.info
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
char* get_lylypic_path(char *filename, PDATE_TIME Time);

/*************************************************
  Function:		storage_get_lylyrecord
  Description: 	�����Ӱ���Լ�¼
  Input:		
  	3.Time		ʱ��
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
void storage_get_lylyrecord(PLYLYLIST_INFO *lylylist);

/*************************************************
  Function:		storage_add_lylyrecord
  Description: 	�����Ӱ���Լ�¼
  Input:
  	1.LyType 	��������
  	2.DevType	�豸����
  	3.DevIndex	�豸����
  	4.Time		ʱ��
  Output:		��
  Return:		
  Others:
*************************************************/
ECHO_STORAGE storage_add_lylyrecord (LYLY_TYPE LyType, DEVICE_TYPE_E DevType, char* DevIndex, DATE_TIME Time);

/*************************************************
  Function:		storage_del_lylyrecord
  Description: 	ɾ����Ӱ���Լ�¼
  Input:		
  	1.Index		����
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
ECHO_STORAGE storage_del_lylyrecord (uint8 Index);

/*************************************************
  Function:		storage_clear_lylyrecord
  Description: 	�����Ӱ���Լ�¼
  Input:		
  	1.Index		����
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
ECHO_STORAGE storage_clear_lylyrecord (void);

/*************************************************
  Function:		storage_set_lylyrecord_flag
  Description: 	���øü�¼�Ƿ��Ѷ�
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
ECHO_STORAGE storage_set_lylyrecord_flag(uint32 Index, uint8 State);

/*************************************************
  Function:		storage_set_lylyrecord_flag
  Description:  ��ȡ�Ƿ���δ��
  Input:		��
  Output:		��
  Return:		TRUE --  ��
  				FALSE -- û��
  Others:		
*************************************************/
uint8 storage_get_lylyrecord_flag(void);

/*************************************************
  Function:		storage_set_lylyrecord_size
  Description:  ��ȡ��Ӱ���Դ�С
  Input:		��
  Output:		��
  Return:		
  Others:		
*************************************************/
uint32 storage_set_lylyrecord_size(void);

#endif 



