/*************************************************
  Copyright (C), 2006-2016, Aurine
  File name: 	storage_message.c
  Author:    	luofl
  Version:   	2.0
  Date: 		2014-11-06
  Description:	��Ϣ�洢
  
  History:                    
    1. Date:		 
       Author:		 
       Modification: 
    2. ...
*********************************************************/
#include "storage_include.h"

#define MSGINFO_SIZE			(sizeof(MSGDATA))
#define INFO_MP3				0x20
#define INFO_WAV				0x21

/*************************************************
  Function:    		storage_set_msg_readed
  Description:		������Ϣ�Ѷ�������δ����־
  Input:
  	1.index			��Ϣ����
  Output:			
  Return:			���óɹ�TRUE,ʧ��FALSE
  Others:			
*************************************************/
ECHO_STORAGE storage_set_msg_readed(uint8 index)
{
	FILE* fPListFile = NULL;
	char path[50] = {0};
	MSGLIST infolist;
	//uint8 flag = 0;
	//uint8 i;
	//MSGLIST msg_list;
	//MSGLIST *pmsg_list;
	
	memcpy(path, MSG_MANAGE_PATH, sizeof(MSG_MANAGE_PATH));
	fPListFile = fopen(path, "r");
	if (fPListFile == NULL)
    {
		return ECHO_STORAGE_NOFILE;
    }
	else
	{
		fread((char*)&infolist, sizeof(MSGLIST), 1, fPListFile);
		infolist.pinfo_data[index].is_unread = 0;
		
		fclose(fPListFile);
		fPListFile = NULL;
		fPListFile = fopen(path, "w");
		if (NULL != fPListFile)
		{
			fwrite((char*)&infolist, sizeof(MSGLIST), 1, fPListFile);
		}
	}
	
	if (fPListFile)
	{
		fclose(fPListFile);
    	fPListFile = NULL;
		sync_data();
		DelayMs_nops(200);
	}
	// ����
	#if 0
	pmsg_list = storage_read_msg_list();
	for (i = 0; i < pmsg_list->ncount; i++)
	{
		dprintf("msg_list->pinfo_data[i].is_unread: %d \n", pmsg_list->pinfo_data[i].is_unread);
	}
	#endif
	return ECHO_STORAGE_OK;
}

/*************************************************
  Function:			storage_read_msg_voice_path
  Description: 		��ȡ��Ϣ����·��
  Input:
  	1.pmsg_data		��Ϣ�ṹ��
  Output:		
  Return:			���ֵ�Ϊ��Ϣ������,ͼƬ������ͼƬ�ļ���
  Others:
*************************************************/ 
void storage_read_msg_voice_path(MSGDATA *pmsg_data, uint8 path[50])
{
	memset(path, 0, 50);
	switch (pmsg_data->voice_type)
	{
		case INFO_MP3:
			sprintf(path, "%s/%d-%d-%d-%d-%d-%d-%d.mp3", MSG_VOICE_PATH, pmsg_data->time.year, pmsg_data->time.month, \
				pmsg_data->time.day, pmsg_data->time.hour, pmsg_data->time.min, pmsg_data->time.sec,pmsg_data->Head.ID);
			break;
			
		case INFO_WAV:
			sprintf(path, "%s/%d-%d-%d-%d-%d-%d-%d.wav", MSG_VOICE_PATH, pmsg_data->time.year, pmsg_data->time.month, \
				pmsg_data->time.day, pmsg_data->time.hour, pmsg_data->time.min, pmsg_data->time.sec, pmsg_data->Head.ID);
			break;
	}
}

/*************************************************
  Function:			storage_read_msg_data
  Description: 		��ȡ��Ϣ����
  Input:
  	1.pmsg_data		��Ϣ�ṹ��
  	2.index			��Ϣ������
  Output:		
  Return:			���ֵ�Ϊ��Ϣ������,ͼƬ������ͼƬ�ļ���
  Others:
*************************************************/ 
char * storage_read_msg_data(MSGDATA *pmsg_data, uint8 index)
{
	char path[50] = {0};
	FILE* fPListFile = NULL;
	char * data = NULL;
	uint32 size = 0;
	
	if (NULL == pmsg_data)
	{
		dprintf("storage_read_msg_data  NULL == pmsg_data \n");
		return NULL;
	}
	
	memset(path, 0, sizeof(path));
	if (0 != pmsg_data->Head.ImageLen)
	{
		sprintf(path, "%s/%d-%d-%d-%d-%d-%d-%d.jpg", MSG_DIR_PATH, pmsg_data->time.year, pmsg_data->time.month, \
								pmsg_data->time.day, pmsg_data->time.hour, pmsg_data->time.min, pmsg_data->time.sec, pmsg_data->Head.ID);
		data = malloc(sizeof(path));
		memcpy(data, path, sizeof(path));
		//dprintf("0 != pmsg_data->Head.ImageLen \n");
	}
	else
	{
		sprintf(path, "%s/%d-%d-%d-%d-%d-%d-%d.DATA", MSG_DIR_PATH, pmsg_data->time.year, pmsg_data->time.month, \
								pmsg_data->time.day, pmsg_data->time.hour, pmsg_data->time.min, pmsg_data->time.sec, pmsg_data->Head.ID);
		fPListFile = fopen(path, "r");
		if (NULL != fPListFile)
		{
			if (0 != pmsg_data->Head.CharLen)
			{
				size = pmsg_data->Head.CharLen;
				dprintf("Net size: %d\n", size);
			}
			//if (size != get_size(fPListFile))
			if (size != get_size(path))
			{
				dprintf("size !=  net size %d\n", size);
				if (fPListFile)
				{
					fclose(fPListFile);
			    	fPListFile = NULL;
				}
				return NULL;
			}
			data = malloc(size+1);
			if (NULL == data)
			{
				return NULL;
			}
			// �ַ���������
			data[size] = '\0';
			//if (size != fread(data, size, 1, fPListFile))
			if (size != fread(data, 1, size, fPListFile))
			{
				dprintf("size != fread(data, size, fPListFile)\n");
				free(data);
				data = NULL;
			}
		}
		else
		{
			dprintf("storage_read_msg_data  NULL == fPListFile \n");
		}
		if (fPListFile)
		{
			fclose(fPListFile);
	    	fPListFile = NULL;
		}
	}
	if (1 == pmsg_data->is_unread)
	{
		storage_set_msg_readed(index);
	}
	
	return data;
}

/*************************************************
  Function:			storage_read_msg_list
  Description: 		��ȡ��Ϣ�б�
  Input:			��
  Output:			��
  Return:			MSGLIST	�ṹָ��
  Others:			�ɵ������ͷ�
*************************************************/ 
MSGLIST * storage_read_msg_list(void)
{
	FILE* fPListFile = NULL;
	char path[50] = {0};
	
	MSGLIST * pinfolist = (MSGLIST *)malloc(sizeof(MSGLIST));
	if (NULL == pinfolist)
	{
		dprintf("storage_read_msg_list NULL == pinfolist \n");
		return NULL;
	}
	memcpy(path, MSG_MANAGE_PATH, sizeof(MSG_MANAGE_PATH));

	fPListFile = fopen(path, "r");
	if (NULL == fPListFile)
    {
    	free(pinfolist);
		return NULL;
    }
	else
	{
		fread(pinfolist, sizeof(MSGLIST), 1, fPListFile);
	}
	
	if (fPListFile)
	{
		fclose(fPListFile);
    	fPListFile = NULL;
	}

	return pinfolist;
}

/*************************************************
  Function:    		storage_delete_msg_data
  Description:		ɾ��ĳ����Ϣ������
  Input:
  	1.pmsg_data		��Ϣ�ṹ
  Output:			
  Return:			ɾ���ɹ�TRUE, ʧ��FALSE
  Others:			
*************************************************/
static void storage_delete_msg_data(MSGDATA *pmsg_data)
{
	char path[50];
	MSGDATA msg_data;
	msg_data = *pmsg_data;
	
	memset(path, 0, sizeof(path));
	// ɾ�����ֻ�ͼƬ�ļ�
	if (0 != pmsg_data->Head.CharLen)
	{
		sprintf(path, "%s/%d-%d-%d-%d-%d-%d-%d.DATA", MSG_DIR_PATH, msg_data.time.year, msg_data.time.month, \
			msg_data.time.day, msg_data.time.hour, msg_data.time.min, msg_data.time.sec,msg_data.Head.ID);
		dprintf("delete char content path:%s \n", path);
		if (TRUE == is_fileexist(path))
		{
			FSFileDelete(path);
			dprintf("after char FSFileDelete \n");
		}
	}
	else if (0 != pmsg_data->Head.ImageLen)
	{
		sprintf(path, "%s/%d-%d-%d-%d-%d-%d-%d.jpg", MSG_DIR_PATH, msg_data.time.year, msg_data.time.month, \
			msg_data.time.day, msg_data.time.hour, msg_data.time.min, msg_data.time.sec,msg_data.Head.ID);
		//dprintf("delete iamge content path:%s \n", path);
		if (TRUE == is_fileexist(path))
		{
			FSFileDelete(path);
			dprintf("after iamge FSFileDelete \n");
		}
	}
	
	// ɾ�������ļ�
	if (msg_data.Head.VoiceLen > 0)
	{
		memset(path, 0, sizeof(path));
		switch (msg_data.voice_type)
		{
			case INFO_MP3:
				sprintf(path, "%s/%d-%d-%d-%d-%d-%d-%d.mp3", MSG_VOICE_PATH, msg_data.time.year, msg_data.time.month, \
					msg_data.time.day, msg_data.time.hour, msg_data.time.min, msg_data.time.sec,msg_data.Head.ID);
				break;
			case INFO_WAV:
				sprintf(path, "%s/%d-%d-%d-%d-%d-%d-%d.wav", MSG_VOICE_PATH, msg_data.time.year, msg_data.time.month, \
					msg_data.time.day, msg_data.time.hour, msg_data.time.min, msg_data.time.sec,msg_data.Head.ID);
				break;

		}
		//dprintf("delete voice path:%d \n", path);
		if (TRUE == is_fileexist(path))
		{
			FSFileDelete(path);
			//dprintf("after voice FSFileDelete \n");
		}
	}
}

/*************************************************
  Function:			storage_write_msg
  Description: 		д����Ϣ
  Input:		
  	1.pHead			��Ϣͷ
  	2.data			����
  	3.des			ִ���ߵ��������߶��ŵĵ绰����
  Output:		   
  Return:		    
  Others:
*************************************************/
ECHO_STORAGE storage_write_msg(const REC_BULLETIN_HEAD *pHead, uint8 * data, uint8 des[14])
{
	size_t count = 0;
	uint32 len;
	FILE* fPListFile = NULL;
	char path[100] = {0};
	int8 i;
	MSGDATA msg_data;
	uint8 *temp_data = NULL;
	uint8 voice_type = 0;
	uint8 size = 0;
	MSGLIST *pinfolist = (MSGLIST *)malloc(sizeof(MSGLIST));
	
	if (NULL == pinfolist)
	{
		dprintf("storage_write_msg NULL == pinfolist\n");
		return ECHO_STORAGE_ERR;
	}
	//dprintf("storage_write_msg begin\n");
	//PMU_EnterModule(PMU_HDMI); 
	memset(&msg_data, 0, sizeof(MSGDATA));
	//memset(&infolist_temp, 0, sizeof(MSGDATA));
	memcpy(path, MSG_MANAGE_PATH, sizeof(MSG_MANAGE_PATH));
	memcpy(msg_data.des, des, 14);
	memcpy((char*)&msg_data.Head, pHead, sizeof(REC_BULLETIN_HEAD));
	memcpy((char*)&msg_data.time, &size, sizeof(DATE_TIME));
	get_timer(&msg_data.time);
	msg_data.is_unread = 1;							// ��δ����־

	dprintf("end....pHead->VoiceLen .......%d\n",pHead->VoiceLen);
	dprintf("end.............000\n");
	if (pHead->VoiceLen)
	{
		dprintf("end.............111\n");
		msg_data.voice_type = *(data + pHead->CharLen + pHead->ImageLen);	
		dprintf("msg_data.voice_type........%0x\n",msg_data.voice_type);
	}
	#if 1
	fPListFile = fopen(path, "r");
	
	// һ����д��10�����ȵĽṹ
	if (fPListFile == NULL)
    {
        fPListFile = fopen(path, "w");
		//dprintf("first writh msg  \n");
		if (NULL == fPListFile)
		{
			free(pinfolist);
			dprintf("storage_write_msg NULL == fPListFile \n");
			return ECHO_STORAGE_NOFILE;
		}
		pinfolist->ncount = 1;
		pinfolist->pinfo_data[0] = msg_data;
		count = fwrite(pinfolist, sizeof(MSGLIST), 1, fPListFile);
    }
	else
	{
		fread((char*)pinfolist, sizeof(MSGLIST), 1, fPListFile);
		dprintf("infolist.ncount:%d \n", pinfolist->ncount);
		if (MAX_MSG_NUM >  pinfolist->ncount)
		{
			i = pinfolist->ncount-1;
			do 
			{
				//dprintf("infolist.ncount; %d why \n", pinfolist->ncount);
				memcpy((char *)&pinfolist->pinfo_data[i+1], (char *)&pinfolist->pinfo_data[i], sizeof(MSGDATA));
				i--;
			}while(i > -1);
			pinfolist->ncount = pinfolist->ncount+1;
		}
		else
		{
			pinfolist->ncount = MAX_MSG_NUM;
			// ɾ�����һ��
			storage_delete_msg_data(&pinfolist->pinfo_data[MAX_MSG_NUM-1]);
			for (i = pinfolist->ncount-2; i > -1; i--)
			{
				memcpy((char *)&pinfolist->pinfo_data[i+1], (char *)&pinfolist->pinfo_data[i], sizeof(MSGDATA));
			}
		}
		//memcpy((char*)&infolist.pinfo_data[1],(char*)&infolist_temp.pinfo_data[0], (MAX_MSG_NUM-1)*sizeof(MSGDATA));
		//msg_data.time.year += infolist.ncount;
		pinfolist->pinfo_data[0] = msg_data;
		//fseek(fPListFile, 0, SEEK_SET);
		fclose(fPListFile);
		fPListFile = NULL;
		fPListFile = fopen(path, "w");
		if (NULL != fPListFile)
		{
			count = fwrite((char*)pinfolist, sizeof(MSGLIST), 1, fPListFile);
			//dprintf("storage_write_msg count: %d, sizeof(MSGLIST): %d \n", count, sizeof(MSGLIST));
		}
	}
	free(pinfolist);
	if (fPListFile)
	{
		fclose(fPListFile);
    	fPListFile = NULL;
	}
	#endif
	
	#if 1
	// д�����ֻ�����ͼƬ������
	memset(path, 0, sizeof(path));
	dprintf("pHead->CharLen: %d,pHead->ImageLen:  %d \n", pHead->CharLen, pHead->ImageLen);
	if (0 != pHead->CharLen)
	{
		sprintf(path, "%s/%d-%d-%d-%d-%d-%d-%d.DATA", MSG_DIR_PATH, msg_data.time.year, \
			msg_data.time.month, msg_data.time.day, msg_data.time.hour, \
			msg_data.time.min, msg_data.time.sec, msg_data.Head.ID);
		len = pHead->CharLen;
		fPListFile = fopen(path, "w");
		if (NULL == fPListFile)
		{
			dprintf("NULL == fPListFile msg content\n");
			return ECHO_STORAGE_NOFILE;
		}
		fwrite(data, len, 1, fPListFile);
	}
	else if (0 != pHead->ImageLen)
	{
		sprintf(path, "%s/%d-%d-%d-%d-%d-%d-%d.jpg", MSG_DIR_PATH, msg_data.time.year, msg_data.time.month, \
						msg_data.time.day, msg_data.time.hour, msg_data.time.min, msg_data.time.sec,msg_data.Head.ID);
		len = pHead->ImageLen;
		fPListFile = fopen(path, "w");
		if (NULL == fPListFile)
		{
			dprintf("NULL == fPListFile msg content\n");
			return ECHO_STORAGE_NOFILE;
		}
		count = fwrite(data+1, len-1, 1, fPListFile);
		//dprintf("count: %d \n", count);
	}
	
	if (fPListFile)
	{
		fclose(fPListFile);
    	fPListFile = NULL;
	}
	
	// д����������
	if (pHead->VoiceLen)
	{
		temp_data = data;
		voice_type = *(temp_data + pHead->CharLen + pHead->ImageLen);		

		dprintf("voice_type type:0x%x,pHead->VoiceLen:0x%x,pHead->ImageLen:0x%x,pHead->CharLen:0x%x\n", voice_type,pHead->VoiceLen,pHead->ImageLen,pHead->CharLen);
		memset(path, 0, sizeof(MSG_VOICE_PATH));
		switch (voice_type)
		{
			case INFO_MP3:	
				sprintf(path, "%s/%d-%d-%d-%d-%d-%d-%d.mp3", MSG_VOICE_PATH, msg_data.time.year, msg_data.time.month, \
							msg_data.time.day, msg_data.time.hour, msg_data.time.min, msg_data.time.sec,msg_data.Head.ID);
				dprintf("write mp3 path:%s \n", path);
				break;
			case INFO_WAV:
				sprintf(path, "%s/%d-%d-%d-%d-%d-%d-%d.wav", MSG_VOICE_PATH, msg_data.time.year, msg_data.time.month, \
							msg_data.time.day, msg_data.time.hour, msg_data.time.min, msg_data.time.sec,msg_data.Head.ID);
				break;
			default:
				return ECHO_STORAGE_ERR;
		}
		
		fPListFile = fopen(path, "w");
		if (NULL == fPListFile)
		{
			return ECHO_STORAGE_NOFILE;
		}
		fwrite((temp_data + pHead->CharLen + pHead->ImageLen + 1) , pHead->VoiceLen - 1, 1, fPListFile);
		//dprintf("storage_write_msg fwrite voice \n");
		if (fPListFile)
		{
			fclose(fPListFile);
	    	fPListFile = NULL;
		}
	}
	#endif
	dprintf("storage_write_msg end\n");
	//sys_set_hint_state(SYS_HINT_INFO, 1);
	//PMU_ExitModule(PMU_HDMI);
	sync_data();
	DelayMs_nops(200);
	
	return ECHO_STORAGE_OK;
}

/*************************************************
  Function:    		storage_delete_msg
  Description:		ɾ��������Ϣ
  Input:
  	1.index			��Ϣ����
  Output:			��
  Return:			ɾ���ɹ�TRUE, ʧ��FALSE
  Others:			
*************************************************/
ECHO_STORAGE storage_delete_msg(uint8 index)
{
	FILE* fPListFile = NULL;
	char path[50] = {0};
	//int ret = 0;
	MSGLIST infolist;
	MSGDATA msg_data;

	memset(&msg_data, 0, sizeof(MSGDATA));
	memcpy(path, MSG_MANAGE_PATH, sizeof(MSG_MANAGE_PATH));

	fPListFile = fopen(path, "r");
	if (NULL != fPListFile)
	{
		fread(&infolist, sizeof(MSGLIST), 1, fPListFile);
		msg_data = infolist.pinfo_data[index];
		if (index > (infolist.ncount - 1))
		{
			fclose(fPListFile);
			return ECHO_STORAGE_ERR;
		}
		if (0 != (infolist.ncount-index-1))
		{
			memcpy((char*)&infolist.pinfo_data[index], (char*)&infolist.pinfo_data[index+1], sizeof(MSGDATA)*(infolist.ncount-index-1));
		}
		else
		{
			memset((char*)&infolist.pinfo_data[index], 0, sizeof(MSGDATA));
		}
		//dprintf("storage_delete_msg infolist.ncount:%d \n", infolist.ncount);
		infolist.ncount--;
		fseek(fPListFile, 0, SEEK_SET);
		fwrite((char*)&infolist, sizeof(MSGLIST), 1, fPListFile);
	}
	else
	{
		return ECHO_STORAGE_NOFILE;
	}
	if (fPListFile)
	{
		fclose(fPListFile);
    	fPListFile = NULL;
	}
	storage_delete_msg_data(&msg_data);
	//DelayMs_nops(200);
	
	return ECHO_STORAGE_OK;
}

/*************************************************
  Function:			storage_clear_msg
  Description: 		�����Ϣ
  Input:			��
  Output:		 	��
  Return:			TRUE/FALSE
  Others:
*************************************************/
ECHO_STORAGE storage_clear_msg (void)
{
	FILE* fPListFile = NULL;
	char path[50] = {0};
	//int ret = 0;
	uint8 i;
	MSGLIST infolist;
	MSGDATA msg_data;

	memcpy(path, MSG_MANAGE_PATH, sizeof(MSG_MANAGE_PATH));
	fPListFile = fopen(path, "r");
	if (NULL != fPListFile)
	{
		fread(&infolist, sizeof(MSGLIST), 1, fPListFile);
	}
	else
	{
		return ECHO_STORAGE_NOFILE;
	}
	if (fPListFile)
	{
		fclose(fPListFile);
    	fPListFile = NULL;
	}
	memset(path, 0, sizeof(path));
	
	for (i = 0; i < infolist.ncount; i++)
	{
		//memset(&msg_data, 0, sizeof(MSGDATA));
		msg_data = infolist.pinfo_data[i];
		storage_delete_msg_data(&msg_data);
	}
	FSFileDelete(MSG_MANAGE_PATH);
	//sys_set_hint_state(SYS_HINT_INFO, 0);
	//DelayMs_nops(200);
		
	return ECHO_STORAGE_OK;
}

/*************************************************
  Function:			storage_get_msg_state
  Description: 		��ȡ��Ϣ�Ƿ���δ����ʾ
  Input:			��
  Output:			��
  Return:			TRUE:��δ�� FALSE:��δ��
  Others:
*************************************************/
uint8 storage_get_msg_state(void)
{
	uint8 i,unread_num = 0;
	MSGLIST *pinfolist = storage_read_msg_list();
	
	if (NULL == pinfolist)
	{
		return unread_num;
	}
	for (i = 0; i < pinfolist->ncount; i++)
	{
		if (1 == pinfolist->pinfo_data[i].is_unread)
		{
			unread_num ++;
		}
	}
	free(pinfolist);
	return unread_num;
}

/*************************************************
  Function:    		storage_get_msg_onesize
  Description:		��ȡ������Ϣ�Ĵ�С
  Input:
  	1.pmsg_data		��Ϣ�ṹ
  Output:			��
  Return:			������Ϣ����
  Others:			
*************************************************/
static int storage_get_msg_onesize(MSGDATA * pmsg_data)
{
	char path[50];
	MSGDATA msg_data;
	int size = 0;
	msg_data = *pmsg_data;
	
	memset(path, 0, sizeof(path));
	// ���ֻ�ͼƬ�ļ���С
	if (0 != pmsg_data->Head.CharLen)
	{
		sprintf(path, "%s/%d-%d-%d-%d-%d-%d-%d.DATA", MSG_DIR_PATH, msg_data.time.year, msg_data.time.month, \
			msg_data.time.day, msg_data.time.hour, msg_data.time.min, msg_data.time.sec,msg_data.Head.ID);
		if (TRUE == is_fileexist(path))
		{
			size = get_size(path);
		}
	}
	else if (0 != pmsg_data->Head.ImageLen)
	{
		sprintf(path, "%s/%d-%d-%d-%d-%d-%d-%d.jpg", MSG_DIR_PATH, msg_data.time.year, msg_data.time.month, \
			msg_data.time.day, msg_data.time.hour, msg_data.time.min, msg_data.time.sec,msg_data.Head.ID);
		if (TRUE == is_fileexist(path))
		{
			size = get_size(path);
		}
	}
	//dprintf("path:%s, size: %d \n", path, size);

	// �����ļ���С
	if (msg_data.Head.VoiceLen > 0)
	{
		memset(path, 0, sizeof(path));
		switch (msg_data.voice_type)
		{
			case INFO_MP3:
				sprintf(path, "%s/%d-%d-%d-%d-%d-%d-%d.mp3", MSG_VOICE_PATH, msg_data.time.year, msg_data.time.month, \
					msg_data.time.day, msg_data.time.hour, msg_data.time.min, msg_data.time.sec,msg_data.Head.ID);
				break;
				
			case INFO_WAV:
				sprintf(path, "%s/%d-%d-%d-%d-%d-%d-%d.wav", MSG_VOICE_PATH, msg_data.time.year, msg_data.time.month, \
					msg_data.time.day, msg_data.time.hour, msg_data.time.min, msg_data.time.sec,msg_data.Head.ID);
				break;
		}
		if (TRUE == is_fileexist(path))
		{
			size += get_size(path);
			//dprintf("after voice FSFileDelete \n");
		}
	}
	
	//dprintf("path:%s, size: %d \n", path, size);
	return size;
}

/*************************************************
  Function:			storage_get_msg_size
  Description: 		��ȡ��Ϣ�ܴ�С
  Input:			��
  Output:		 	��
  Return:			������
  Others:
*************************************************/
int storage_get_msg_allsize(void)
{
	FILE* fPListFile = NULL;
	char path[50] = {0};
	//int ret = 0;
	uint8 i;
	int size = 0;
	MSGLIST infolist;
	MSGDATA msg_data;

	memcpy(path, MSG_MANAGE_PATH, sizeof(MSG_MANAGE_PATH));
	size = get_size(path);
	fPListFile = fopen(path, "r");
	
	if (NULL != fPListFile)
	{
		fread(&infolist, sizeof(MSGLIST), 1, fPListFile);
	}
	else
	{
		return ECHO_STORAGE_NOFILE;
	}
	if (fPListFile)
	{
		fclose(fPListFile);
    	fPListFile = NULL;
	}
	memset(path, 0, sizeof(path));
	
	for (i = 0; i < infolist.ncount; i++)
	{
		//memset(&msg_data, 0, sizeof(MSGDATA));
		msg_data = infolist.pinfo_data[i];
		size += storage_get_msg_onesize(&msg_data);
	}
	//dprintf("storage_get_msg_allsize size: %d \n", size);

	return size;
}

