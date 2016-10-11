/*********************************************************
  Copyright (C), 2009-2012
  File name:	storage_call.c
  Author:   	唐晓磊
  Version:   	1.0
  Date: 		09.4.7
  Description:  存储模块--对讲
  History:            
*********************************************************/
#include "storage_include.h"

#define CALLINFO_SIZE			(sizeof(MCALLINFO))
#define MONITORINFO_SIZE			(sizeof(MONITORINFO))

/*************************************************
  Function:		free_call_memory
  Description: 	释放对讲存储内存
  Input:		
  	1.CallList
  Output:		无
  Return:		无
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
  Description: 	申请对讲存储内存
  Input:
  	1.CallList
  	2.MaxNum	最大记录数
  Output:		无
  Return:		无
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
  Description: 	获得通话记录
  Input:		
  	1.CallList
  Output:		无
  Return:		无
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
  Description: 	按模块存储
  Input:	
  	1.mode
  	2.CallList	
  Output:		无
  Return:		无
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
  Description: 	获得通话记录
  Input:		
  	1.Calltype
  Output:		无
  Return:		无
  Others:		
  	1.该指针外部释放
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
  Description: 	添加通话记录
  Input:		  
  Output:		无
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

	// 新记录加在头部
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
  Description: 	删除通话记录
  Input:		
  	1.Calltype	记录类型
  	2.DelList	删除列表
  Output:		无
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
			if (DelList->DelFlg[i] == 0)			// flg == 1 表示需要删除的
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
  Description: 	清空通话记录
  Input:		
  	1.Calltype	记录类型
  Output:		无
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
  Description: 	设置该记录是否有关联的留影留言
  Input:		
  	1.time
  	2.LylyFlag
  Output:		无
  Return:		无
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
  Description:  清空对讲模块所有通话记录
  Input:		无
  Output:		无
  Return:		无
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
  Description:  获取是否有未接来电
  Input:		无
  Output:		无
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
  Description:  将未接来电设置为已读状态
  Input:		无
  Output:		无
  Return:		无
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
/*					设备号设备规则设置					 */	
/*********************************************************/

/*************************************************
  Function:    		str_to_deviceno
  Description:		将字符串转换为,设备号
  Input: 
	1.devno			设备号参数
  Output:			无
  Return:			成功与否, TRUE/FALSE
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
  Description: 	获取设备编号结构
  Input:		
  Output:		无
  Return:		设备编号结构
  Others:
*************************************************/
PFULL_DEVICE_NO storage_get_devparam(void)
{
	return &(gpSysParam->Devparam);
}

/*************************************************
  Function:		storage_get_devrule
  Description: 	获取设备规则结构
  Input:		
  Output:		无
  Return:		设备编号结构
  Others:
*************************************************/
DEVICENO_RULE* storage_get_devrule(void)
{
	return &(gpSysParam->Devparam.Rule);
}

/*************************************************
  Function:    		storage_get_devno
  Description:		获得设备编号
  Input: 			无
  Output:			无
  Return:			设备编号
  Others:
*************************************************/
DEVICE_NO storage_get_devno(void)
{
	return gpSysParam->Devparam.DeviceNo;
}

/*************************************************
  Function:    		get_devno_str
  Description:		获得设备编号字符串
  Input: 			无
  Output:			无
  Return:			设备编号字符串
  Others:
*************************************************/
char * storage_get_devno_str(void)
{
	return gpSysParam->Devparam.DeviceNoStr;
}

/*************************************************
  Function:    		storage_get_use_cell
  Description:		读取是否启用单元号
  Input: 			无
  Output:			无
  Return:			1:启用0:未启用
  Others:
*************************************************/
uint32 storage_get_use_cell (void)
{
	return gpSysParam->Devparam.Rule.UseCellNo;
} 

/*************************************************
  Function:    		storage_set_areano
  Description:		设置区号
  Input: 			无
  Output:			无
  Return:			无
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
  Description:		获取区号
  Input: 			无
  Output:			无
  Return:			无
  Others:
*************************************************/
uint32 storage_get_areano (void)
{
	return gpSysParam->Devparam.AreaNo;
}

/*************************************************
  Function:    		set_devno_rule
  Description:		保存设备编号规则
  Input: 
  	1.save			是否保存
	2.rule			设备编号规则
  Output:			无
  Return:			0-成功 1-单元长度非法 2-梯号长度非法 3-房号长度非法
  					4-总长度不能大于18
  Others:
*************************************************/
uint32 storage_set_devno_rule(uint8 save, DEVICENO_RULE Rule)
{
	//合法判断
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
  Description:		保存设备号
  Input: 
  	1.DEVICE_TYPE_E	设备类型
	2.devno			设备号字符串
  Output:			无
  Return:			
  	0-成功 
  	1-长度非法 
  	2-房号段全为0   
  	3-区口机编号必须在1-40之间 
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
  Description:		保存设备编号
  Input: 
  	1.save			是否保存
  	2.rule			设备编号规则
	3.devno			设备号字符串
  Output:			无
  Return:			无
  Others:			无
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
  Description:		读取设备描述符
  Input: 			无
  Output:			无
  Return:			设备描述符指针
  Others:
*************************************************/
char * storage_get_dev_desc(void)
{
	return gpSysParam->DevDesc;
}

/*************************************************
  Function:    		storage_set_dev_desc
  Description:		读取设备描述符
  Input: 			无
  Output:			无
  Return:			设备描述符指针
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
  Description:		初始化时读取设备描述
  Input:
  	1.DevDesc		描述地址
  	2.language		语言类型
  Output:			无
  Return:			设备描述符指针
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

	char DevDesc_China[7][10] = { "栋 ", "单元 ", "房 " };
	char DevDesc_Big5[7][10] = { "棟 ", "單元 ", "房 " };
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
  Description: 	对讲模块存储初始化
  Input:		无
  Output:		无
  Return:		无
  Others:
*************************************************/
void storage_call_init(void)
{
}

/*************************************************
  Function:		save_call_storage
  Description: 	按模块存储
  Input:	
  	1.mode
  	2.CallList	
  Output:		无
  Return:		无
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
  Description: 	释放对讲存储内存
  Input:		
  	1.CallList
  Output:		无
  Return:		无
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
  Description: 	获得通话记录
  Input:		
  	1.CallList
  Output:		无
  Return:		无
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
  Description: 	申请对讲存储内存
  Input:
  	1.CallList
  	2.MaxNum	最大记录数
  Output:		无
  Return:		无
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
  Description: 	 获取监视列表信息
  Input:		
  	1.Calltype
  Output:		无
  Return:		无
  Others:		
  	1.该指针外部释放
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

	// 记得使用完 内存释放
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
Description: 	获取监视列表个数
Input:
	1.devtype	类型
Output:			无
Return:			无
Others:
1.该指针外部释放
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
  Description: 	 获取监视列表信息
  Input:		
  	1.Calltype
  Output:		无
  Return:		无
  Others:		
  	1.该指针外部释放
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
  Description: 	增加监视设备
  Input:		
  	1.Type		设备类型
  	2.index		设备索引
  	3.ip		设备ip
  Output:		无
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

	// 新记录加在头部
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
			// 将原有记录加在新记录后面
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
  Description: 	清空存储文件内容
  Input:		
  	1.Type		设备类型
  Output:		无
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


