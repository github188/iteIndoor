/*********************************************************
  Copyright (C), 2009-2012
  File name:	storage_lyly.c
  Author:   	������
  Version:   	1.0
  Date: 		09.4.7
  Description:  �洢ģ��--��Ӱ����
  History:            
*********************************************************/
#include "storage_include.h"

#define LYLYINFO_SIZE			(sizeof(LYLY_INFO))
/*************************************************
  Function:		get_lylyrecord_from_storage
  Description: 	�����Ӱ���Լ�¼
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
static void get_lylyrecord_from_storage(PLYLYLIST_INFO list)
{
	if (list)
	{
		list->Count = Fread_common(LYLY_MANAGE_PATH, list->LylyInfo, LYLYINFO_SIZE, MAX_LYLY_NUM);
	}
}

/*************************************************
  Function:		save_lyly_storage
  Description: 	��ģ��洢
  Input:		
  	1.list
  Output:		��
  Return:		��
  Others:
*************************************************/
static ECHO_STORAGE save_lyly_storage(PLYLYLIST_INFO list)
{
	ECHO_STORAGE ret = ECHO_STORAGE_ERR;
	if (list)
	{
		ret =  Fwrite_common(LYLY_MANAGE_PATH, list->LylyInfo, LYLYINFO_SIZE, list->Count);
	}

	return ret;
}

/*************************************************
  Function:		storage_free_lyly_memory
  Description: 	�ͷŴ洢�ڴ�
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void storage_free_lyly_memory(PLYLYLIST_INFO *list)
{
	if (*list)
	{
		if ((*list)->LylyInfo)
		{
			free((*list)->LylyInfo);
			(*list)->LylyInfo = NULL;
		}
		free((*list));	
		(*list) = NULL;
	}
}

/*************************************************
  Function:		storage_malloc_lyly_memory
  Description: 	����洢�ڴ�
  Input:
  	1.MaxNum	����¼��
  Output:		��
  Return:		��
  Others:
*************************************************/
void storage_malloc_lyly_memory (PLYLYLIST_INFO *list,uint32 MaxNum)
{
	*list = (PLYLYLIST_INFO)malloc(sizeof(LYLYLIST_INFO));	
	if (*list)
	{
		(*list)->Count = 0;
		(*list)->LylyInfo = (PLYLY_INFO)malloc(LYLYINFO_SIZE * MaxNum);
	}
}		

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
char* get_lylyrecord_path(char *filename, PZONE_DATE_TIME Time) 
{	
	if (filename)
	{
		memset(filename, 0, sizeof(filename));
		sprintf(filename,"%s/%04d%02d%02d%02d%02d%02d",LYLY_DIR_PATH,Time->year,Time->month,Time->day
					,Time->hour,Time->min,Time->sec);
		return filename;
	}
	else
	{
		return NULL;
	}
}

/*************************************************
  Function:		get_lylypic_path
  Description: 	�õ���ǰ���ͼƬ�ļ���
  Input:		
  	1.info
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
char* get_lylypic_path(char *filename, PZONE_DATE_TIME Time) 
{
	if (Time && filename)
	{
		memset(filename, 0, sizeof(filename));
		sprintf(filename,"%s/%04d%02d%02d%02d%02d%02d.jpg",LYLY_DIR_PATH,Time->year,Time->month,Time->day
				,Time->hour,Time->min,Time->sec);
		return filename;
	}
	else
	{
		return NULL;
	}
}

/*************************************************
  Function:		get_lylypic_path
  Description: 	�õ���ǰ���ͼƬ�ļ���
  Input:		
  	1.info
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
char* get_lylywav_path(char *filename, PZONE_DATE_TIME Time) 
{
	if (Time && filename)
	{
		memset(filename, 0, sizeof(filename));
		sprintf(filename,"%s/%04d%02d%02d%02d%02d%02d.wav",LYLY_DIR_PATH,Time->year,Time->month,Time->day
				,Time->hour,Time->min,Time->sec);
		return filename;
	}
	else
	{
		return NULL;
	}
}

/*************************************************
  Function:		get_lylypic_path
  Description: 	�õ���ǰ���ͼƬ�ļ���
  Input:		
  	1.info
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
char* get_lylyavi_path(char *filename, PZONE_DATE_TIME Time) 
{
	if (Time && filename)
	{
		memset(filename, 0, sizeof(filename));
		sprintf(filename,"%s/%04d%02d%02d%02d%02d%02d.avi",LYLY_DIR_PATH,Time->year,Time->month,Time->day
				,Time->hour,Time->min,Time->sec);
		return filename;
	}
	else
	{
		return NULL;
	}
}

/*************************************************
  Function:		del_lylyrecord_file
  Description: 	ɾ��������¼���ļ�
  Input:		
  	1.info
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
static ECHO_STORAGE del_lylyrecord_file(PLYLY_INFO info) 
{
	char filename[100];	
	memset(filename, 0, sizeof(filename));
	if (info)
	{
		dprintf("info.LyType77777........%d\n",info->LyType);
		if (info->LyType == LYLY_TYPE_VIDEO)
		{
			get_lylyrecord_path(filename,&info->Time);
			strcat(filename,".avi");
			FSFileDelete(filename);
			dprintf("filename9999999.................%s\n",filename);
		}
		else
		{
			if(info->LyType == LYLY_TYPE_PIC_AUDIO)
			{
				FSFileDelete(get_lylypic_path(filename,&info->Time));
			}
			get_lylyrecord_path(filename,&info->Time);
			strcat(filename,".wav");
			FSFileDelete(filename);
			dprintf("filename888888.................%s\n",filename);
		}
	
		storage_set_callrecord_lylyflag(info->Time, FALSE);

		return ECHO_STORAGE_OK;
	}
	else
	{
		return ECHO_STORAGE_ERR;
	}
}

/*************************************************
  Function:		storage_get_lylyrecord
  Description: 	�����Ӱ���Լ�¼
  Input:		
  	3.Time		ʱ��
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
void storage_get_lylyrecord(PLYLYLIST_INFO *lylylist)
{
	storage_malloc_lyly_memory(lylylist, MAX_LYLY_NUM);
	get_lylyrecord_from_storage(*lylylist);
}

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
ECHO_STORAGE storage_add_lylyrecord (LYLY_TYPE LyType, DEVICE_TYPE_E DevType, char* DevIndex, ZONE_DATE_TIME Time)
{
	PLYLYLIST_INFO lylylist = NULL;
	ECHO_STORAGE ret = ECHO_STORAGE_ERR;
	LYLY_INFO buf[MAX_LYLY_NUM];
	uint32 num = 0;
	int nlen = 0;
	//FLAG_STORAGE flag;

	storage_malloc_lyly_memory(&lylylist, MAX_LYLY_NUM);
	get_lylyrecord_from_storage(lylylist);
	
	// �¼�¼����ͷ��
	if (lylylist && lylylist->LylyInfo && DevIndex)
	{
		nlen = strlen(DevIndex);
		if (0 == lylylist->Count)
		{
			lylylist->LylyInfo[0].UnRead = TRUE;
			lylylist->LylyInfo[0].LyType = LyType;
			lylylist->LylyInfo[0].DevType = DevType;
			memcpy(lylylist->LylyInfo[0].DevNo,DevIndex,nlen);
			memcpy(&(lylylist->LylyInfo[0].Time), &Time, sizeof(ZONE_DATE_TIME));			
			lylylist->Count = 1;
		}
		else
		{
			memset(buf, 0, sizeof(buf));
			buf[0].UnRead = TRUE;
			buf[0].LyType = LyType;
			buf[0].DevType = DevType;
			memcpy(buf[0].DevNo,DevIndex,nlen);
			memcpy(&(buf[0].Time), &Time, sizeof(ZONE_DATE_TIME));	
			num = lylylist->Count;
			if (num >= MAX_LYLY_NUM)
			{
				num = MAX_LYLY_NUM-1;
				del_lylyrecord_file(&(lylylist->LylyInfo[num]));
			}
			// ��ԭ�м�¼�����¼�¼����
			memcpy(&(buf[1]), lylylist->LylyInfo, num*LYLYINFO_SIZE);
			lylylist->Count++;
			if (lylylist->Count > MAX_LYLY_NUM)
			{
				lylylist->Count = MAX_LYLY_NUM;
			}
			memset(lylylist->LylyInfo, 0, sizeof(lylylist->LylyInfo));
			memcpy(lylylist->LylyInfo, buf, LYLYINFO_SIZE*lylylist->Count);
		}
		ret = save_lyly_storage(lylylist);
	}

	storage_free_lyly_memory(&lylylist);
	return ret;
}

/*************************************************
  Function:		storage_del_lylyrecord
  Description: 	ɾ����Ӱ���Լ�¼
  Input:		
  	1.Index		����
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
ECHO_STORAGE storage_del_lylyrecord (uint8 Index)
{
	uint8 i, j;
	PLYLYLIST_INFO lylylist = NULL;
	ECHO_STORAGE ret = ECHO_STORAGE_ERR;
	LYLY_INFO buf[MAX_LYLY_NUM];
	uint32 num = 0;
	//FLAG_STORAGE flag;

	storage_malloc_lyly_memory(&lylylist, MAX_LYLY_NUM);
	get_lylyrecord_from_storage(lylylist);
	
	if (lylylist && lylylist->LylyInfo)
	{
		num = lylylist->Count;
		if (num > 0 && Index <= (num-1))
		{
			if (Index == (num-1))
			{
				del_lylyrecord_file(lylylist->LylyInfo+Index);
				memset((lylylist->LylyInfo+Index), 0, LYLYINFO_SIZE);
			}
			else
			{	
				memset(buf, 0, LYLYINFO_SIZE*MAX_LYLY_NUM);
				memcpy(buf, lylylist->LylyInfo, LYLYINFO_SIZE*num);
				memset(lylylist->LylyInfo, 0, LYLYINFO_SIZE*MAX_LYLY_NUM);
				j = 0;
				for(i = 0; i < num; i++)
				{
					if (Index != i)	
					{
						memcpy(lylylist->LylyInfo+j, buf+i, LYLYINFO_SIZE);
						j++;
					}
					else
					{
						del_lylyrecord_file(buf+i);
					}
				}
			}
			lylylist->Count--;
			ret = save_lyly_storage(lylylist);
			
		}
	}
	storage_free_lyly_memory(&lylylist);
	return ret;
}

/*************************************************
  Function:		storage_clear_lylyrecord
  Description: 	�����Ӱ���Լ�¼
  Input:		
  	1.Index		����
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
ECHO_STORAGE storage_clear_lylyrecord (void)
{
	uint8 i;
	PLYLYLIST_INFO lylylist = NULL;
	ECHO_STORAGE ret = ECHO_STORAGE_ERR;
	LYLY_INFO buf[MAX_LYLY_NUM];
	uint32 num = 0;
	//FLAG_STORAGE flag;

	storage_malloc_lyly_memory(&lylylist, MAX_LYLY_NUM);
	get_lylyrecord_from_storage(lylylist);
	if (lylylist && lylylist->LylyInfo)
	{
		num = lylylist->Count;
		if (num > 0)
		{
			memset(buf, 0, LYLYINFO_SIZE*MAX_LYLY_NUM);
			memcpy(buf, lylylist->LylyInfo, LYLYINFO_SIZE*num);
			memset(lylylist->LylyInfo, 0, LYLYINFO_SIZE*MAX_LYLY_NUM);
			for (i = 0; i < num; i++)
			{
				del_lylyrecord_file(buf+i);
			}
			lylylist->Count = 0;
			ret = save_lyly_storage(lylylist);
		}
		else
		{
			ret = ECHO_STORAGE_OK;
		}
	}
	storage_free_lyly_memory(&lylylist);
	//DelayMs_nops(200);
	return ret;
}

/*************************************************
  Function:		storage_set_lylyrecord_flag
  Description: 	���øü�¼�Ƿ��Ѷ�
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
ECHO_STORAGE storage_set_lylyrecord_flag(uint32 Index, uint8 State)
{
	uint8 i;
	PLYLYLIST_INFO lylylist = NULL;
	ECHO_STORAGE ret = ECHO_STORAGE_ERR;
	//LYLY_INFO buf[MAX_LYLY_NUM];
	uint32 num = 0;
	//FLAG_STORAGE flag;

	storage_malloc_lyly_memory(&lylylist, MAX_LYLY_NUM);
	get_lylyrecord_from_storage(lylylist);
	
	if (lylylist && lylylist->LylyInfo)
	{
		num = lylylist->Count;
		if (num > 0)
		{
			for(i = 0; i < num; i++)
			{
				if (Index == i)
				{
					lylylist->LylyInfo[i].UnRead = State;
				}
			}
			ret = save_lyly_storage(lylylist);
		}
	}
	storage_free_lyly_memory(&lylylist);
	return ret;
}

/*************************************************
  Function:		storage_set_lylyrecord_flag
  Description:  ��ȡ�Ƿ���δ��
  Input:		��
  Output:		��
  Return:		0-û��δ��
  				>0-�����м���δ��
  Others:		
*************************************************/
uint8 storage_get_lylyrecord_flag(void)
{
	uint8 i, unread_num = 0;
	PLYLYLIST_INFO lylylist = NULL;
	
	storage_malloc_lyly_memory(&lylylist, MAX_LYLY_NUM);
	get_lylyrecord_from_storage(lylylist);
	if (lylylist && lylylist->Count > 0)
	{
		for (i = 0; i < lylylist->Count; i++)
		{
			if (lylylist->LylyInfo[i].UnRead == TRUE)
			{
				unread_num ++;
			}
		}
	}
	storage_free_lyly_memory(&lylylist);
	return unread_num;
}

/*************************************************
  Function:		storage_set_lylyrecord_size
  Description:  ��ȡ��Ӱ���Դ�С
  Input:		��
  Output:		��
  Return:		
  Others:		
*************************************************/
uint32 storage_set_lylyrecord_size(void)
{
	PLYLYLIST_INFO lylylist = NULL;
	uint32 i,ret = 0;
	//LYLY_INFO buf[MAX_LYLY_NUM];
	char filename[100];
	
	storage_malloc_lyly_memory(&lylylist, MAX_LYLY_NUM);
	get_lylyrecord_from_storage(lylylist);
	
	// �¼�¼����ͷ��
	if (lylylist && lylylist->LylyInfo)
	{
		if (lylylist->Count > 0)
		{
			for (i = 0; i < lylylist->Count; i++)
			{
				if (lylylist->LylyInfo[i].LyType == LYLY_TYPE_PIC_AUDIO)
				{
					ret += get_size(get_lylypic_path(filename,&(lylylist->LylyInfo[i].Time)));
				}

				if (lylylist->LylyInfo[i].LyType == LYLY_TYPE_VIDEO)
				{
					ret += get_size(get_lylyavi_path(filename,&(lylylist->LylyInfo[i].Time)));
				}
				else 
				{
					ret += get_size(get_lylywav_path(filename,&(lylylist->LylyInfo[i].Time)));
				}
			}
		}
	}
	storage_free_lyly_memory(&lylylist);
	return ret;
}


