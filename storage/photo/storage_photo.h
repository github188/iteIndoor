/*********************************************************
  Copyright (C), 2009-2012
  File name:	storage_photo.h
  Author:   	������
  Version:   	1.0
  Date: 		09.4.7
  Description:  �洢ģ��--��Ƭ�鿴
  History:                   
    1. Date:
       Author:
       Modification:
    2. ...
*********************************************************/
#ifndef __STORAGE_PHOTO_H__
#define __STORAGE_PHOTO_H__

#include "storage_include.h"
//#include "storage_types.h"

#define MAX_PHOTO_NUM  			10					// ����Զ�ץ�ļ�¼

typedef struct
{
	char DevNo[20];									// �豸��
	DEVICE_TYPE_E Type;								// �豸����
	ZONE_DATE_TIME Time;		    				// ʱ��	           
}PHOTO_INFO, * PPHOTO_INFO;							// ��Ƭ�ṹ

// ͨ����¼�б�Ľṹ
typedef struct
{
	uint8 Count;									// ����
	PPHOTO_INFO PhotoInfo;				    		// ��Ƭ�ṹ
}PHOTOLIST_INFO, * PPHOTOLIST_INFO;

/*************************************************
  Function:		storage_free_photo_memory
  Description: 	�ͷŴ洢�ڴ�
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void storage_free_photo_memory(PPHOTOLIST_INFO *list);

/*************************************************
  Function:		storage_malloc_photo_memory
  Description: 	����洢�ڴ�
  Input:
  	1.MaxNum	����¼��
  Output:		��
  Return:		��
  Others:
*************************************************/
void storage_malloc_photo_memory (PPHOTOLIST_INFO *list,uint32 MaxNum);

/*************************************************
  Function:		get_photo_path
  Description: 	�õ���ǰ����ļ���
  Input:		
  	1.info
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
char* get_photo_path(char *filename, PZONE_DATE_TIME Time);

/*************************************************
  Function:		storage_add_photo
  Description: 	��ȡ��Ƭ�б�
  Input:		
  	3.Time		ʱ��
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
void storage_get_photo(PPHOTOLIST_INFO *photolist);

/*************************************************
  Function:		storage_add_photo
  Description: 	�����Ƭ��¼
  Input:		
  	1.Type		�豸����
  	2.Index		�豸��
  	3.Time		ʱ��
  Output:		��
  Return:		
  Others:
*************************************************/
ECHO_STORAGE storage_add_photo (DEVICE_TYPE_E Type, char* DevNo, ZONE_DATE_TIME Time);

/*************************************************
  Function:		storage_del_photo
  Description: 	ɾ����Ƭ��¼
  Input:		
  	1.Index		����
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
ECHO_STORAGE storage_del_photo (uint8 Index);

/*************************************************
  Function:		storage_clear_photo
  Description: 	�����Ƭ��¼
  Input:		
  	1.Index		����
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
ECHO_STORAGE storage_clear_photo (void);

/*************************************************
  Function:		storage_set_photo_size
  Description:  ��ȡ��Ƭ��С
  Input:		��
  Output:		��
  Return:		
  Others:		
*************************************************/
uint32 storage_set_photo_size(void);
#endif 

