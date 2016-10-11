/*********************************************************
  Copyright (C), 2009-2012
  File name:	storage_call.c
  Author:   	������
  Version:   	1.0
  Date: 		09.4.7
  Description:  �洢ģ��--�Խ�
  History:            
*********************************************************/
#include "storage_include.h"

#define CALLINFO_SIZE			(sizeof(MCALLINFO))
#define MONITORINFO_SIZE			(sizeof(MONITORINFO))

/*************************************************
  Function:		free_call_memory
  Description: 	�ͷŶԽ��洢�ڴ�
  Input:		
  	1.CallList
  Output:		��
  Return:		��
  Others:
*************************************************/
static void free_call_memory(PMCALLLISTINFO* CallList)
{
	if ((*CallList))
	{
		if ((*CallList)->CallInfo)
		{
			free((*CallList)->CallInfo);
			(*CallList)->CallInfo = NULL;
		}
		free((*CallList));	
		(*CallList) = NULL;
	}
}

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
static void malloc_call_memory (PMCALLLISTINFO *CallList,uint32 MaxNum)
{
	*CallList = (PMCALLLISTINFO)malloc(sizeof(MCALLLISTINFO));	

	if (*CallList)
	{
		(*CallList)->CallCount = 0;
		(*CallList)->CallInfo = (PMCALLINFO)malloc(CALLINFO_SIZE * MaxNum);
	}
}		

/*************************************************
  Function:		get_callrecord_from_storage
  Description: 	���ͨ����¼
  Input:		
  	1.CallList
  Output:		��
  Return:		��
  Others:
*************************************************/
static void get_callrecord_from_storage(FLAG_STORAGE mode, PMCALLLISTINFO CallList)
{
	if (CallList)
	{
		switch (mode)
		{
			case FLAG_CALLIN:
				CallList->CallCount = Fread_common(CALL_IN_PATH, CallList->CallInfo, CALLINFO_SIZE, MAX_RECORD_NUM);
				break;

			case FLAG_CALLOUT:
				CallList->CallCount = Fread_common(CALL_OUT_PATH, CallList->CallInfo, CALLINFO_SIZE, MAX_RECORD_NUM);
				break;	

			case FLAG_CALLMISS:
				CallList->CallCount = Fread_common(CALL_MISS_PATH, CallList->CallInfo, CALLINFO_SIZE, MAX_RECORD_NUM);
				break;
				
			default:
				break;
		}
	}
}

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
static void save_call_storage(FLAG_STORAGE mode, PMCALLLISTINFO CallList)
{
	switch (mode)
	{
		case FLAG_CALLIN:
			Fwrite_common(CALL_IN_PATH, CallList->CallInfo, CALLINFO_SIZE, CallList->CallCount);
			break;

		case FLAG_CALLOUT:
			Fwrite_common(CALL_OUT_PATH, CallList->CallInfo, CALLINFO_SIZE, CallList->CallCount);
			break;	

		case FLAG_CALLMISS:
			Fwrite_common(CALL_MISS_PATH, CallList->CallInfo, CALLINFO_SIZE, CallList->CallCount);
			break;
			
		default:
			break;
	}
}

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
PMCALLLISTINFO storage_get_callrecord (CALL_TYPE Calltype)
{
	PMCALLLISTINFO info = NULL;
	FLAG_STORAGE mode = FLAG_CALLMISS;

	switch (Calltype)
	{
		case INCOMING:
			mode = FLAG_CALLIN;
			break;

		case OUTGOING:
			mode = FLAG_CALLOUT;
			break;	

		case MISSED:
			mode = FLAG_CALLMISS;
			break;
			
		default:
			dprintf (" Calltype is ERR !!! \n");
			return NULL;
			break;
	}
	
	malloc_call_memory(&info, MAX_RECORD_NUM);
	if (NULL == info)
	{
		return NULL;
	}
	get_callrecord_from_storage(mode, info);

	return info;
}

/*************************************************
  Function:		storage_add_callrecord
  Description: 	���ͨ����¼
  Input:		  
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
ECHO_STORAGE storage_add_callrecord (PMCALLINFO pcallinfo)
{
	PMCALLLISTINFO calllist = NULL;
	PMCALLLISTINFO calllistnew = NULL;
	ECHO_STORAGE ret = ECHO_STORAGE_ERR;
	FLAG_STORAGE flag;

	if (pcallinfo == NULL)
	{
		dprintf (" pcallinfo is NULL !!! \n");
		return ECHO_STORAGE_ERR;
	}
	
	switch(pcallinfo->Calltype)
	{
		case MISSED:
			flag = FLAG_CALLMISS;
			break;

		case OUTGOING:
			flag = FLAG_CALLOUT;
			break;
			
		case INCOMING:
			flag = FLAG_CALLIN;
			break;
			
		default:
			dprintf (" Calltype is ERR !!! \n");
			return ECHO_STORAGE_ERR;
			break;
	}

	calllist = storage_get_callrecord(pcallinfo->Calltype);

	// �¼�¼����ͷ��
	if (calllist && calllist->CallInfo)
	{
		calllistnew = (PMCALLLISTINFO)malloc(sizeof(MCALLLISTINFO));
		calllistnew->CallCount = 0;
		calllistnew->CallInfo = (PMCALLINFO)malloc(sizeof(MCALLINFO)*(calllist->CallCount+1));
		
		memset((char *)calllistnew->CallInfo, 0, sizeof(MCALLINFO)*(calllist->CallCount+1));

		pcallinfo->UnRead = TRUE;
		memcpy(&calllistnew->CallInfo[0], pcallinfo, sizeof(MCALLINFO));
		if (calllist->CallCount > 0)
		{
			memcpy(&calllistnew->CallInfo[1], calllist->CallInfo, (calllist->CallCount)*sizeof(MCALLINFO));
		}
		calllistnew->CallCount = (calllist->CallCount+1)>MAX_RECORD_NUM ? MAX_RECORD_NUM : (calllist->CallCount+1);				
		
		save_call_storage(flag, calllistnew);
		ret = ECHO_STORAGE_OK;
	}
	else
	{
		calllistnew = (PMCALLLISTINFO)malloc(sizeof(MCALLLISTINFO));
		calllistnew->CallCount = 0;
		calllistnew->CallInfo = (PMCALLINFO)malloc(sizeof(MCALLINFO));
		
		memset(calllistnew->CallInfo, 0, sizeof(MCALLINFO)*(calllist->CallCount+1));
		pcallinfo->UnRead = TRUE;
		memcpy(&calllistnew->CallInfo[0], pcallinfo, sizeof(MCALLINFO));
		calllistnew->CallCount = 1;
		
		save_call_storage(flag, calllistnew);
		ret = ECHO_STORAGE_OK;
	}
	
	free_call_memory(&calllist);
	free_call_memory(&calllistnew);
	return ret;
}

/*************************************************
  Function:		storage_del_callrecord
  Description: 	ɾ��ͨ����¼
  Input:		
  	1.Calltype	��¼����
  	2.DelList	ɾ���б�
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
ECHO_STORAGE storage_del_callrecord (CALL_TYPE Calltype, PDEL_LIST DelList)
{
	uint8 i;
	PMCALLLISTINFO calllist = NULL;
	PMCALLLISTINFO calllistnew = NULL;
	ECHO_STORAGE ret = ECHO_STORAGE_ERR;
	FLAG_STORAGE flag;

	if (DelList == NULL)
	{
		dprintf (" DelList is NULL !!! \n");
		return ECHO_STORAGE_ERR;
	}
	
	switch(Calltype)
	{
		case MISSED:
			flag = FLAG_CALLMISS;
			break;
			
		case OUTGOING:
			flag = FLAG_CALLOUT;
			break;
			
		case INCOMING:
			flag = FLAG_CALLIN;
			break;
			
		default:
			dprintf (" Calltype is ERR !!! \n");
			return ECHO_STORAGE_ERR;
			break;
	}

	calllist = storage_get_callrecord(Calltype);
	
	if (calllist && calllist->CallInfo)
	{
		calllistnew = (PMCALLLISTINFO)malloc(sizeof(MCALLLISTINFO));
		calllistnew->CallCount = 0;
		calllistnew->CallInfo = (PMCALLINFO)malloc(sizeof(MCALLINFO)*calllist->CallCount);
		memset(calllistnew->CallInfo, 0, sizeof(MCALLINFO)*calllist->CallCount);
		
		for (i=0; i<calllist->CallCount; i++)
		{
			if (DelList->DelFlg[i] == 0)			// flg == 1 ��ʾ��Ҫɾ����
			{
				memcpy(&calllistnew->CallInfo[calllistnew->CallCount], &calllist->CallInfo[i], sizeof(MCALLINFO));
				calllistnew->CallCount++;
			}
		}	
		
		save_call_storage(flag, calllistnew);
		ret = ECHO_STORAGE_OK;
	}

	free_call_memory(&calllist);
	free_call_memory(&calllistnew);
	return ret;
}

/*************************************************
  Function:		storage_clear_callrecord
  Description: 	���ͨ����¼
  Input:		
  	1.Calltype	��¼����
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
ECHO_STORAGE storage_clear_callrecord (CALL_TYPE Calltype)
{
	FLAG_STORAGE flag;
	PMCALLLISTINFO calllist = NULL;
	
	switch(Calltype)
	{
		case MISSED:
			flag = FLAG_CALLMISS;
			break;
			
		case OUTGOING:
			flag = FLAG_CALLOUT;
			break;
			
		case INCOMING:
			flag = FLAG_CALLIN;
			break;
			
		default:
			break;
	}

	calllist = storage_get_callrecord(Calltype);
	
	if (calllist)
	{	
		calllist->CallCount = 0;
		memset(calllist->CallInfo, 0, CALLINFO_SIZE*MAX_RECORD_NUM);
		save_call_storage(flag, calllist);
		free_call_memory(&calllist);
		return ECHO_STORAGE_OK;
	}
	else
	{
		free_call_memory(&calllist);
		return ECHO_STORAGE_ERR;
	}
}

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
FLAG_STORAGE storage_set_callrecord_lylyflag(DATE_TIME time, uint8 LylyFlag)
{	
	//FLAG_STORAGE flag;
	PMCALLLISTINFO calllist = NULL;
	uint8 i;
	
	calllist = storage_get_callrecord(MISSED);
	
	if (calllist && calllist->CallCount > 0)
	{	
		for (i = 0; i < calllist->CallCount; i++)
		{
			if (time.year == calllist->CallInfo[i].Time.year && time.month == calllist->CallInfo[i].Time.month
					&& time.day == calllist->CallInfo[i].Time.day && time.hour == calllist->CallInfo[i].Time.hour
					&& time.min == calllist->CallInfo[i].Time.min && time.sec == calllist->CallInfo[i].Time.sec)
			{
				calllist->CallInfo[i].LylyFlag = LylyFlag;
			}
		}
		
		save_call_storage(FLAG_CALLMISS, calllist);
		free_call_memory(&calllist);
		return ECHO_STORAGE_OK;
	}
	else
	{
		free_call_memory(&calllist);
		return ECHO_STORAGE_ERR;
	}
}

/*************************************************
  Function:		storage_clear_all_callrecord
  Description:  ��նԽ�ģ������ͨ����¼
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void storage_clear_all_callrecord(void)
{
	storage_clear_callrecord(MISSED);
	storage_clear_callrecord(OUTGOING);
	storage_clear_callrecord(INCOMING);
}

/*************************************************
  Function:		storage_get_callrecord_state
  Description:  ��ȡ�Ƿ���δ������
  Input:		��
  Output:		��
  Return:		  				
  Others:		
*************************************************/
uint8 storage_get_callrecord_state(void)
{
	uint8 i;
	uint8 ret = FALSE;
	
	PMCALLLISTINFO calllist = storage_get_callrecord(MISSED);
	if (calllist && calllist->CallCount > 0)
	{
		for(i = 0; i < calllist->CallCount; i++)
		{
			if (1 == calllist->CallInfo[i].UnRead)
			{
				ret = TRUE;
				break;
			}
		}
		free_call_memory(&calllist);
	}
	
	return ret;
}

/*************************************************
  Function:		storage_set_callrecord_state
  Description:  ��δ����������Ϊ�Ѷ�״̬
  Input:		��
  Output:		��
  Return:		��
  Others:		
*************************************************/
void storage_set_callrecord_state(void)
{
	uint8 i;
	PMCALLLISTINFO calllist = storage_get_callrecord(MISSED);

	if (calllist && calllist->CallCount > 0)
	{
		for (i = 0; i < calllist->CallCount; i++)
		{
			calllist->CallInfo[i].UnRead = 0;
		}
	}
	save_call_storage(FLAG_CALLMISS, calllist);
	free_call_memory(&calllist);
}


/*********************************************************/
/*					�豸���豸��������					 */	
/*********************************************************/

/*************************************************
  Function:    		str_to_deviceno
  Description:		���ַ���ת��Ϊ,�豸��
  Input: 
	1.devno			�豸�Ų���
  Output:			��
  Return:			�ɹ����, TRUE/FALSE
  Others:
*************************************************/
static uint32 str_to_deviceno(PFULL_DEVICE_NO devno)
{	
	int n;
	
	if (!devno)
	{
		return FALSE;
	}
	
	devno->DeviceNo.DeviceNo1 = 0;
	devno->DeviceNo.DeviceNo2 = 0;
	n = strlen(devno->DeviceNoStr);
	if (0 == n)
	{
		return FALSE;
	}

	if (n != devno->DevNoLen)
	{
		return FALSE;
	}
	if (devno->DevNoLen > 9)
	{
		char temp[10];
		strncpy(temp, devno->DeviceNoStr, devno->DevNoLen-9);
		devno->DeviceNo.DeviceNo1 = atoi(temp);
		strncpy(temp, devno->DeviceNoStr+devno->DevNoLen-9, 9);
		devno->DeviceNo.DeviceNo2 = atoi(temp);
	}
	else
	{
		devno->DeviceNo.DeviceNo2 = atoi(devno->DeviceNoStr);
	}
	#ifndef _AU_PROTOCOL_
	devno->DeviceNo.DeviceType = devno->DeviceType;
	#endif
	
	return TRUE;
}

/*************************************************
  Function:		storage_get_devparam
  Description: 	��ȡ�豸��Žṹ
  Input:		
  Output:		��
  Return:		�豸��Žṹ
  Others:
*************************************************/
PFULL_DEVICE_NO storage_get_devparam(void)
{
	return &(gpSysParam->Devparam);
}

/*************************************************
  Function:		storage_get_devrule
  Description: 	��ȡ�豸����ṹ
  Input:		
  Output:		��
  Return:		�豸��Žṹ
  Others:
*************************************************/
DEVICENO_RULE* storage_get_devrule(void)
{
	return &(gpSysParam->Devparam.Rule);
}

/*************************************************
  Function:    		storage_get_devno
  Description:		����豸���
  Input: 			��
  Output:			��
  Return:			�豸���
  Others:
*************************************************/
DEVICE_NO storage_get_devno(void)
{
	return gpSysParam->Devparam.DeviceNo;
}

/*************************************************
  Function:    		get_devno_str
  Description:		����豸����ַ���
  Input: 			��
  Output:			��
  Return:			�豸����ַ���
  Others:
*************************************************/
char * storage_get_devno_str(void)
{
	return gpSysParam->Devparam.DeviceNoStr;
}

/*************************************************
  Function:    		storage_get_use_cell
  Description:		��ȡ�Ƿ����õ�Ԫ��
  Input: 			��
  Output:			��
  Return:			1:����0:δ����
  Others:
*************************************************/
uint32 storage_get_use_cell (void)
{
	return gpSysParam->Devparam.Rule.UseCellNo;
} 

/*************************************************
  Function:    		storage_set_areano
  Description:		��������
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
uint32 storage_set_areano (uint32 areano)
{
	if (gpSysParam->Devparam.AreaNo != areano)
	{
		gpSysParam->Devparam.AreaNo = areano;
		#ifndef _AU_PROTOCOL_
		gpSysParam->Devparam.DeviceNo.AreaNo = areano;
		#endif
		SaveRegInfo();
	}
	return TRUE;
}

/*************************************************
  Function:    		storage_get_areano
  Description:		��ȡ����
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
uint32 storage_get_areano (void)
{
	return gpSysParam->Devparam.AreaNo;
}

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
uint32 storage_set_devno_rule(uint8 save, DEVICENO_RULE Rule)
{
	//�Ϸ��ж�
	if (Rule.CellNoLen > 2)
	{
		return 1;
	}
	if (Rule.StairNoLen < Rule.CellNoLen || Rule.StairNoLen > 9)
	{
		return 2;
	}
	if (Rule.RoomNoLen < 3 || Rule.RoomNoLen > 9)
	{
		return 3;
	}
	if ((Rule.StairNoLen+Rule.RoomNoLen) > 17) 
	{
		return 4;
	}
	if (Rule.CellNoLen == 0)
	{
		Rule.UseCellNo = FALSE;
	}
	memcpy(&gpSysParam->Devparam.Rule, &Rule, sizeof(DEVICENO_RULE));
	//gpSysParam->Devparam.Rule.Subsection = (Rule.StairNoLen -Rule.CellNoLen)*100+Rule.CellNoLen*10+Rule.RoomNoLen;
	gpSysParam->Devparam.DevNoLen = Rule.StairNoLen + Rule.RoomNoLen + 1;
	gpSysParam->Devparam.IsRight = FALSE;
	if (save)
	{
		SaveRegInfo();
	}
	return 0;
}

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
uint32 storage_set_devno(uint8 save, DEVICE_TYPE_E DeviceType, char* DevNo)
{
	char temp[20];
	int nlen = 0;

	if (NULL == DevNo)
	{
		return 1;
	}
	
	nlen = strlen(DevNo);
	if (nlen != gpSysParam->Devparam.DevNoLen)
	{
		return 2;
	}

	strncpy(temp, DevNo+gpSysParam->Devparam.Rule.StairNoLen, gpSysParam->Devparam.Rule.RoomNoLen);
	temp[gpSysParam->Devparam.Rule.RoomNoLen] = 0;
	if (0 == atoi(temp))
	{
		return 3;
	}
	gpSysParam->Devparam.DeviceType = DeviceType;
	memset(gpSysParam->Devparam.DeviceNoStr,0,sizeof(gpSysParam->Devparam.DeviceNoStr));
	strcpy(gpSysParam->Devparam.DeviceNoStr, DevNo); 
	gpSysParam->Devparam.IsRight = str_to_deviceno(storage_get_devparam());
	if (save)
	{
		SaveRegInfo();
	}
	return 0;
}

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
void storage_save_devno(uint8 save, DEVICENO_RULE Rule, char* DevNo)
{
	memcpy(&gpSysParam->Devparam.Rule, &Rule, sizeof(DEVICENO_RULE));
	gpSysParam->Devparam.DevNoLen = Rule.StairNoLen + Rule.RoomNoLen + 1;
	gpSysParam->Devparam.IsRight = FALSE;

	gpSysParam->Devparam.DeviceType = DEVICE_TYPE_ROOM;
	memset(gpSysParam->Devparam.DeviceNoStr, 0, sizeof(gpSysParam->Devparam.DeviceNoStr));
	strcpy(gpSysParam->Devparam.DeviceNoStr, DevNo); 
	gpSysParam->Devparam.IsRight = str_to_deviceno(storage_get_devparam());
	if (save)
	{
		SaveRegInfo();
	}
}

/*************************************************
  Function:    		storage_get_dev_desc
  Description:		��ȡ�豸������
  Input: 			��
  Output:			��
  Return:			�豸������ָ��
  Others:
*************************************************/
char * storage_get_dev_desc(void)
{
	return gpSysParam->DevDesc;
}

/*************************************************
  Function:    		storage_set_dev_desc
  Description:		��ȡ�豸������
  Input: 			��
  Output:			��
  Return:			�豸������ָ��
  Others:
*************************************************/
uint8 storage_set_dev_desc(char * str)
{
	if (str == NULL)
	{
		return FALSE;
	}

	memset(gpSysParam->DevDesc, 0, sizeof(gpSysParam->DevDesc));
	memcpy(gpSysParam->DevDesc, str, sizeof(gpSysParam->DevDesc));
	SaveRegInfo();
	return TRUE;
}

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
void storage_init_get_dev_desc(char * DevDesc, LANGUAGE_E language)
{
#if 0
	char DevDesc_China[70] = { 0xb6, 0xb0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xb5, 0xa5, 0xd4, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb7,
		0xbf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	char DevDesc_Big5[70] = { 0xb4, 0xc9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xb3, 0xe6, 0xa4, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa9,
		0xd0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
#endif

	char DevDesc_China[7][10] = { "�� ", "��Ԫ ", "�� " };
	char DevDesc_Big5[7][10] = { "�� ", "��Ԫ ", "�� " };
	char DevDesc_En[7][10] = { "Build ", "Unit ", "Room " };
	

	if (language == CHNBIG5)
	{
		memcpy(DevDesc, DevDesc_Big5, 70);
	}
	else if (language == ENGLISH)
	{
		memcpy(DevDesc, DevDesc_En, 70);
	}
	else
	{
		memcpy(DevDesc, DevDesc_China, 70);
	}
}

/*************************************************
  Function:		storage_call_init
  Description: 	�Խ�ģ��洢��ʼ��
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void storage_call_init(void)
{
}

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
void save_monitorlist_storage(DEVICE_TYPE_E Type, PMONITORLISTINFO monitorlist)
{
	switch (Type)
	{
		case DEVICE_TYPE_DOOR_NET:
		case DEVICE_TYPE_DOOR_PHONE:
			Fwrite_common(MONITOR_DOOR_PATH, monitorlist->pMonitorInfo, MONITORINFO_SIZE, monitorlist->MonitorCount);
			break;

		case DEVICE_TYPE_STAIR:
			Fwrite_common(MONITOR_STAIR_PATH, monitorlist->pMonitorInfo, MONITORINFO_SIZE, monitorlist->MonitorCount);
			break;	

		case DEVICE_TYPE_AREA:
			Fwrite_common(MONITOR_AREA_PATH, monitorlist->pMonitorInfo, MONITORINFO_SIZE, monitorlist->MonitorCount);
			break;
			
		default:
			break;
	}
}

/*************************************************
  Function:		free_monitorlist_memory
  Description: 	�ͷŶԽ��洢�ڴ�
  Input:		
  	1.CallList
  Output:		��
  Return:		��
  Others:
*************************************************/
void free_monitorlist_memory(PMONITORLISTINFO* monitorlist)
{
	if ((*monitorlist))
	{
		if ((*monitorlist)->pMonitorInfo)
		{
			free((*monitorlist)->pMonitorInfo);
			(*monitorlist)->pMonitorInfo = NULL;
		}
		free((*monitorlist));	
		(*monitorlist) = NULL;
	}
}

/*************************************************
  Function:		get_callrecord_from_storage
  Description: 	���ͨ����¼
  Input:		
  	1.CallList
  Output:		��
  Return:		��
  Others:
*************************************************/
static void get_monitorinfo_from_storage(DEVICE_TYPE_E devtype, PMONITORLISTINFO MonitorList)
{
	if (MonitorList)
	{
		switch (devtype)
		{
			case DEVICE_TYPE_DOOR_NET:
			case DEVICE_TYPE_DOOR_PHONE:
				MonitorList->MonitorCount = Fread_common(MONITOR_DOOR_PATH, MonitorList->pMonitorInfo, MONITORINFO_SIZE, MAX_MONITOR_NUM);
				break;

			case DEVICE_TYPE_STAIR:
				MonitorList->MonitorCount = Fread_common(MONITOR_STAIR_PATH, MonitorList->pMonitorInfo, MONITORINFO_SIZE, MAX_MONITOR_NUM);
				break;	

			case DEVICE_TYPE_AREA:
				MonitorList->MonitorCount = Fread_common(MONITOR_AREA_PATH, MonitorList->pMonitorInfo, MONITORINFO_SIZE, MAX_MONITOR_NUM);
				break;
				
			default:
				break;
		}
		
	}
}

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
void malloc_monitorlist_memory (PMONITORLISTINFO *MonitorList,uint32 MaxNum)
{
	*MonitorList = (PMONITORLISTINFO)malloc(sizeof(MONITORLISTINFO));	

	if (*MonitorList)
	{
		(*MonitorList)->MonitorCount = 0;
		(*MonitorList)->pMonitorInfo = (PMONITORINFO)malloc(MONITORINFO_SIZE * MaxNum);
	}
}	

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
PMONITORINFO storage_get_monitorinfo (DEVICE_TYPE_E devtype, int8 index)
{
	PMONITORINFO info = NULL;
	PMONITORLISTINFO monitorlist = NULL;
	monitorlist = storage_get_monitorlist(devtype);
	if (monitorlist->MonitorCount == 0)
	{
		dprintf(" storage_get_monitorlist return 0 \n");
		free_monitorlist_memory(&monitorlist);
		return NULL;
	}

	// �ǵ�ʹ���� �ڴ��ͷ�
	info = (PMONITORINFO)malloc(MONITORINFO_SIZE);
	if (NULL == info)
	{
		free_monitorlist_memory(&monitorlist);
		return NULL;
	}
	info->DeviceType = monitorlist->pMonitorInfo[index].DeviceType;
	info->index = monitorlist->pMonitorInfo[index].index;
	info->IP = monitorlist->pMonitorInfo[index].IP;
	
	free_monitorlist_memory(&monitorlist);
	return info;
}

/*************************************************
Function:		storage_get_monitocount
Description: 	��ȡ�����б����
Input:
	1.devtype	����
Output:			��
Return:			��
Others:
1.��ָ���ⲿ�ͷ�
*************************************************/
uint8 storage_get_monitocount(DEVICE_TYPE_E devtype)
{
	uint8 count = 0;
	PMONITORLISTINFO monitorlist = NULL;
	monitorlist = storage_get_monitorlist(devtype);
	if (monitorlist->MonitorCount == 0)
	{
		dprintf(" storage_get_monitorlist return 0 \n");
		free_monitorlist_memory(&monitorlist);
		return NULL;
	}
	count = monitorlist->MonitorCount;

	free_monitorlist_memory(&monitorlist);

	return count;
}

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
PMONITORLISTINFO storage_get_monitorlist(DEVICE_TYPE_E devtype)
{
	PMONITORLISTINFO info = NULL;
		
	malloc_monitorlist_memory(&info, MAX_MONITOR_NUM);
	if (NULL == info)
	{
		return NULL;
	}
	get_monitorinfo_from_storage(devtype,info);

	return info;
}

/*************************************************
  Function:		storage_add_monitorlist
  Description: 	���Ӽ����豸
  Input:		
  	1.Type		�豸����
  	2.index		�豸����
  	3.ip		�豸ip
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
ECHO_STORAGE storage_add_monitorlist (DEVICE_TYPE_E Type, int8 index, uint32 ip)
{
	PMONITORLISTINFO monitorlist = NULL;
	ECHO_STORAGE ret = ECHO_STORAGE_ERR;
	MONITORINFO buf[MAX_MONITOR_NUM];
	uint32 num = 0;

	monitorlist = storage_get_monitorlist(Type);

	// �¼�¼����ͷ��
	if (monitorlist && monitorlist->pMonitorInfo)
	{
		if (0 == monitorlist->MonitorCount)
		{
			monitorlist->pMonitorInfo[0].IP = ip;
			monitorlist->pMonitorInfo[0].index = index;
			monitorlist->pMonitorInfo[0].DeviceType = Type;	
			monitorlist->MonitorCount = 1;
		}
		else
		{
			memset(buf, 0, sizeof(buf));
			buf[0].IP = ip;
			buf[0].index= index;
			buf[0].DeviceType= Type;
			
			num = monitorlist->MonitorCount;
			if (num >= MAX_MONITOR_NUM)
			{
				num = MAX_MONITOR_NUM-1;
			}
			// ��ԭ�м�¼�����¼�¼����
			memcpy(&(buf[1]), monitorlist->pMonitorInfo, num*MONITORINFO_SIZE);
			monitorlist->MonitorCount++;
			if (monitorlist->MonitorCount > MONITORINFO_SIZE)
			{
				monitorlist->MonitorCount = MONITORINFO_SIZE;
			}

			memset(monitorlist->pMonitorInfo, 0, sizeof(monitorlist->pMonitorInfo));
			memcpy(monitorlist->pMonitorInfo, buf, MONITORINFO_SIZE*monitorlist->MonitorCount);
		}
		
		save_monitorlist_storage(Type, monitorlist);
		ret = ECHO_STORAGE_OK;
	}

	free_monitorlist_memory(&monitorlist);
	return ret;
}

/*************************************************
  Function:		storage_clear_monitorlist
  Description: 	��մ洢�ļ�����
  Input:		
  	1.Type		�豸����
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
void storage_clear_monitorlist(DEVICE_TYPE_E Type)
{
	FILE* fPListFile = NULL;

	switch (Type)
	{
		case DEVICE_TYPE_DOOR_NET:
		case DEVICE_TYPE_DOOR_PHONE:	
			fPListFile = fopen(MONITOR_DOOR_PATH, "w");
			//Fwrite_common(MONITOR_DOOR_PATH, monitorlist->pMonitorInfo, MONITORINFO_SIZE, monitorlist->MonitorCount);
			break;

		case DEVICE_TYPE_STAIR:
			fPListFile = fopen(MONITOR_STAIR_PATH, "w");
			//Fwrite_common(MONITOR_STAIR_PATH, monitorlist->pMonitorInfo, MONITORINFO_SIZE, monitorlist->MonitorCount);
			break;	

		case DEVICE_TYPE_AREA:
			fPListFile = fopen(MONITOR_AREA_PATH, "w");
			//Fwrite_common(MONITOR_AREA_PATH, monitorlist->pMonitorInfo, MONITORINFO_SIZE, monitorlist->MonitorCount);
			break;
			
		default:
			break;
	}
    
	fclose(fPListFile);
	sync_data();
}


