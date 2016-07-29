/********************************************************
  Copyright (C), 2006-2010, Aurine
  File name:	logic_alarm_net.c
  Author:		wengjf
  Version:		V1.0
  Date:			2010-3-30
  Description:	����ģ��--����ַ�
  
  History:        
                  
    1. Date:
       Author:
       Modification:
    2. ...
*********************************************************/
#include "logic_include.h"

#define DATE_TIME_SIZE		sizeof(DATE_TIME)

/*************************************************
  Function:    		query_alarm_define_response
  Description: 		��λ����ѯ��������(����������)
  Input:       		
  	1.recPacket		���հ�
  Output:
  Return: 	   
  Others:      		��ѯ����CMD_GET_AREA_DEFINE
*************************************************/
static void query_alarm_define_response(PRECIVE_PACKET recPacket)
{
	// ������1B�����ò�����2B��+���ͣ�16B��
	ALARM_DEFINE_NET_DATA area_define;
	
	center_get_area_define(&area_define);
	net_send_echo_packet(recPacket, ECHO_OK, (char*)(&area_define), sizeof(ALARM_DEFINE_NET_DATA));
}

/*************************************************
  Function:    		alarm_define_report
  Description: 		�����ϱ�
  Input:        
  Output:       	��
  Return:			�ϱ��ɹ����
  Others:       	
*************************************************/
int32 alarm_define_report(void)
{
	ALARM_DEFINE_NET_DATA alarm_define;
	
	center_get_area_define(&alarm_define);
	return net_send_event(CMD_RP_AREA_DEFINE, (char*)&alarm_define, sizeof(ALARM_DEFINE_NET_DATA), EST_MANAGER);		
}

/*************************************************
  Function:    		query_alarm_status_response
  Description: 		����״̬��ѯ(���������)
  Input:       		
  	1.recPacket		���հ�
  Output:
  Return: 	   
  Others:      		CMD_GET_AREA_STATUS
*************************************************/
static void query_alarm_status_response(PRECIVE_PACKET recPacket)
{
	ALARM_STATUES_NET_DATA status;
	
	ipad_get_area_state(&status);
	net_send_echo_packet(recPacket, ECHO_OK, (char*)(&status), sizeof(ALARM_STATUES_NET_DATA));
}

/*************************************************
  Function:    		alarm_status_report
  Description: 		����״̬�ϱ�
  Input:        
  Output:       	��
  Return:			�ϱ��ɹ����
  Others:       	
*************************************************/
int32 alarm_status_report(void)
{
	ALARM_STATUES_NET_DATA status;
	
	center_get_area_state(&status);
	return net_send_event(CMD_RP_ALARM_STATE, (char*)&status, sizeof(ALARM_STATUES_NET_DATA), EST_MANAGER);		
}

/*************************************************
  Function:    		set_net_response
  Description: 		Զ�̲���
  Input:       		
  	1.recPacket		���հ�
  Output:
  Return: 	   
  Others:      		
*************************************************/
static int32 set_net_response(PRECIVE_PACKET recPacket)
{
	EXECUTOR_E executor;
	AF_RETURN_TYPE ret;

	executor = (EXECUTOR_E)(*(recPacket->data + NET_HEAD_SIZE));
	ret = check_defend_oper(SET_DEFEND, executor);
	switch (ret)
	{
		case AF_NO_REMOTE_CTRL:						// ��Ȩ��
			net_send_echo_packet(recPacket, ECHO_UNALLOWDD, NULL, 0);	
			break;
			
		case AF_SUCCESS:							// �ɹ�
			net_send_echo_packet(recPacket, ECHO_OK, NULL, 0);
			alarm_set_operator(executor);
			break;
			
		default:
			net_send_echo_packet(recPacket, ECHO_ERROR, NULL, 0);
			break;
	}
	return TRUE;	
}

/*************************************************
  Function:    		unset_net_response
  Description: 		Զ�̳���
  Input:       		
  	1.recPacket		���հ�
  Output:
  Return: 	   
  Others:      		
*************************************************/
static int32 unset_net_response(PRECIVE_PACKET recPacket)
{
	EXECUTOR_E executor;
	AF_RETURN_TYPE ret;

	executor = (EXECUTOR_E)(*(recPacket->data + NET_HEAD_SIZE));
	ret = check_defend_oper(DIS_DEFEND, executor);

	switch (ret)
	{
		case AF_NO_REMOTE_CTRL:						// ��Ȩ��
			net_send_echo_packet(recPacket, ECHO_UNALLOWDD, NULL, 0);	
			break;
			
		case AF_SUCCESS:							// �ɹ�
			net_send_echo_packet(recPacket, ECHO_OK, NULL, 0);
			alarm_unset_operator(executor, 0);
			break;
			
		default:
			net_send_echo_packet(recPacket, ECHO_ERROR, NULL, 0);
			break;
	}
	return TRUE;	
}

/*************************************************
  Function:    		partset_net_response
  Description: 		Զ�ַ̾�
  Input:       		
  	1.recPacket		���հ�
  Output:
  Return: 	   
  Others:      		
*************************************************/
static int32 partset_net_response(PRECIVE_PACKET recPacket)
{
	EXECUTOR_E executor;
	AF_RETURN_TYPE ret;

	executor = (EXECUTOR_E)(*(recPacket->data + NET_HEAD_SIZE));
	ret = check_defend_oper(PART_DEFEND, executor);

	switch (ret)
	{
		case AF_NO_REMOTE_CTRL:						// ��Ȩ��
			net_send_echo_packet(recPacket, ECHO_UNALLOWDD, NULL, 0);	
			break;
			
		case AF_SUCCESS:							// �ɹ�
			net_send_echo_packet(recPacket, ECHO_OK, NULL, 0);
			alarm_partset_operator(executor);	
			break;
			
		default:
			net_send_echo_packet(recPacket, ECHO_ERROR, NULL, 0);
			break;
	}
	return TRUE;	
}

/*************************************************
  Function:    		get_alarm_net_time
  Description: 		��ȡ����ʱ��
  Input:   	    
  	1.command		���������¼�
    2.controlby		����ִ����
  Output:       	��
  Return:			�ϱ��ɹ����
  Others:
*************************************************/
static void get_alarm_net_time(PDATE_TIME pdata_time, char* timer)
{
	DATE_TIME datatime;
	net_get_utc_time(&datatime);

	pdata_time->year = datatime.year;
	pdata_time->month = datatime.month;
	pdata_time->day = datatime.day;
	pdata_time->week = datatime.week;
	pdata_time->hour = datatime.hour;
	pdata_time->min = datatime.min;
	pdata_time->sec = datatime.sec;

	if (timer)
	{
		sprintf(timer, "%04d-%02d-%02d %02d:%02d:%02d",datatime.year,datatime.month,datatime.day,datatime.hour,datatime.min,datatime.sec);
	}
}

/*************************************************
  Function:    		timetostr
  Description: 		��ȡ����ʱ���ַ���
  Input:   	    
  Output:       	��
   	1.str			�ַ���
  Return:			�ϱ��ɹ����
  Others:
*************************************************/
static void timetostr(char * str)
{
	DATE_TIME datatime;
	
	net_now_time(&datatime);
	sprintf(str,"%4d-%02d-%02d %02d:%02d:%02d",(datatime.year),(datatime.month),datatime.day, datatime.hour, datatime.min, datatime.sec);		
}

/*************************************************
  Function:    		sync_comm_event_send
  Description: 		ͬ�������¼��ϱ�ͨ�ú���
  Input:   	    
  	1.command		���������¼�
    2.controlby		����ִ����
  Output:       	��
  Return:			�ϱ��ɹ����
  Others:
*************************************************/
int32 sync_comm_event_send(uint16 command, EXECUTOR_E controlby)
{
	int32 ret = FALSE;
	char buf[DATE_TIME_SIZE + 1];
	
	memset(buf, 0, sizeof(buf));
	get_alarm_net_time((DATE_TIME *)buf, NULL);
	buf[DATE_TIME_SIZE] = controlby;
	ret = net_send_event(command, buf, DATE_TIME_SIZE + 1, EST_MANAGER);
	alarm_status_report();

	return ret;
}

/*************************************************
  Function:    		comm_event_send
  Description: 		�����¼��ϱ�ͨ�ú���
  Input:   	    
  	1.command		���������¼�
    2.controlby		����ִ����
  Output:       	��
  Return:			�ϱ��ɹ����
  Others:
*************************************************/
static int32 comm_event_send(uint16 command, EXECUTOR_E controlby)
{
	int32 ret = FALSE;
	char buf[DATE_TIME_SIZE + 1];
	
	memset(buf, 0, sizeof(buf));
	get_alarm_net_time((DATE_TIME *)buf, NULL);
	buf[DATE_TIME_SIZE] = controlby;
	ret = net_send_event(command, buf, DATE_TIME_SIZE + 1, EST_MANAGER);
	alarm_status_report();

	// �㲥�������������ַ������¼�. ʹ�ֻ��İ���״̬����ͬ�� add by luofl 2011-4-25
	#ifdef _ALARM_OPER_SYNC_
	switch (command)
	{
		case CMD_RP_SET_EVENT:
			command = CMD_SYNC_SET_EVENT;
			break;

		case CMD_RP_UNSET_EVENT:
			command = CMD_SYNC_UNSET_EVENT;
			break;

		case CMD_RP_PARTSET_EVENT:
			command = CMD_SYNC_PARTSET_EVENT;
			break;

		default:
			return ret;
	}
	buf[0] = EXECUTOR_REMOTE_DEVICE;				// ����ͬ����ִ����Ϊң����
	set_nethead(G_BROADCAST_DEVNO, PRIRY_DEFAULT);
	ret = net_direct_send(command, buf, 1, INADDR_BROADCAST, NETCMD_UDP_PORT);
	#endif
	
	return ret;
}

/*************************************************
  Function:    		set_event_report
  Description: 		���������¼��ϱ�
  Input:        	
  	1.controlby		����ִ����
  Output:      		��
  Return:			�ϱ��ɹ����
  Others:
*************************************************/
int32 set_event_report(EXECUTOR_E controlby)
{
	return comm_event_send(CMD_RP_SET_EVENT, controlby);				
}

/*************************************************
  Function:    		unset_event_report
  Description: 		���������¼��ϱ�
  Input:        	
  	1.controlby		����ִ����
  Output:       	��
  Return:			�ϱ��ɹ����
  Others:
*************************************************/
int32 unset_event_report(EXECUTOR_E controlby)
{
	return comm_event_send(CMD_RP_UNSET_EVENT, controlby);				
}

/*************************************************
  Function:    		partset_event_report
  Description: 		�ַ������¼��ϱ�
  Input:       
  	1.controlby		����ִ����
  Output:       	��
  Return:			�ϱ��ɹ����
  Others:
*************************************************/
int32 partset_event_report(EXECUTOR_E controlby)
{
	return comm_event_send(CMD_RP_PARTSET_EVENT, controlby);					
}

/*************************************************
  Function:    		bypass_event_report
  Description: 		��·�����¼�
  Input:        
  	1.bypass_num	��·������
    2.bypass_area	��·����
  Output:       	��
  Return:			�ϱ��ɹ����
  Others:       	bit0��15��1����·�ķ���
*************************************************/
int32 bypass_event_report(int8 bypass_num, uint16 bypass_area)
{
	char buf[DATE_TIME_SIZE + 3];
	
	get_alarm_net_time((DATE_TIME *)buf, NULL);

	buf[DATE_TIME_SIZE] = bypass_num;
	memcpy(&buf[DATE_TIME_SIZE + 1], &bypass_area, 2);
	
	return net_send_event(CMD_RP_BYPASS_EVENT, buf, DATE_TIME_SIZE + 3, EST_MANAGER);			
}

/*************************************************
  Function:    		shortmsg_trans_send
  Description: 		���������¼�
  Input:   	    	��������
  Output:       	��
  Return:			�ϱ��ɹ����
  Others:
*************************************************/
static int32 shortmsg_trans_send(int8 area_num)
{	
	uint32 code;
	uint32 Center_Ip;
	char stime[20];
	char transbuf[255];
	uint8 i;
	uint8 area_type[AREA_AMOUNT];
	char alarm_type[12];
	char area_des[10];
	char type_des[12];

	SEND_SHORT_MSG short_msg;
	uint8 tel_num1[TELNUM_LEN+1];
	uint8 tel_num2[TELNUM_LEN+1];
	uint8 *tel_num = NULL;
	memset(tel_num1, 0, TELNUM_LEN+1);
	memset(tel_num2, 0, TELNUM_LEN+1);
	storage_get_gsm_num(tel_num1, tel_num2);
   	code = storage_get_validate_code();
	Center_Ip = storage_get_center_ip();
	
	if (0 == code || 0 == Center_Ip || 0 == area_num)
	{ 
		return FALSE;
	}
	storage_get_area_type(area_type);
	//���Ͷ���
	short_msg.level = 2;
	timetostr(stime);
	for (i = 0; i < 2; i++)
	{
		if (0 == i)
		{
			tel_num = tel_num1;
		}
		else
		{
			tel_num = tel_num2;
		}
		memset(&short_msg, 0, sizeof(SEND_SHORT_MSG));		  		  
		memset(alarm_type, 0, sizeof(alarm_type));
		memset(area_des, 0, sizeof(area_des));
		memset(type_des, 0, sizeof(type_des));
		if(strcmp(tel_num, "") != 0)
		{ 
			memset(transbuf, 0, sizeof(transbuf));
			memcpy(&short_msg.ReceiveNo,tel_num,sizeof(short_msg.ReceiveNo));
			memcpy(transbuf,(char*)(&code),sizeof(uint32));
			strcpy(alarm_type,get_str(SID_Bj_Report_Type_Chufa));
			strcpy(area_des,get_str(SID_Bj_Fangqu));
			strcpy(type_des,get_str(SID_Bj_SOS+area_type[area_num-1]));
			sprintf(short_msg.Content,"%s    %s    %s:%d    %s", alarm_type,stime,\
				area_des,area_num, type_des);
			memcpy(transbuf+4,&short_msg,sizeof(short_msg));  

			set_nethead(G_CENTER_DEVNO, PRIRY_HIGHEST);
	    	net_direct_send(CMD_SHORT_MSG_TRANSMIT, transbuf, sizeof(int) + sizeof(short_msg), Center_Ip, NETCMD_UDP_PORT);			
		}
	}
	return TRUE;
}

/*************************************************
  Function:    		force_alarm_report
  Description: 		Ю�ֳ��������¼��ϱ�
  Input:   	    	
  Output:       	��
  Return:			�ϱ��ɹ����
  Others:
*************************************************/
int force_alarm_report(void)
{
	char buf[DATE_TIME_SIZE + 1];
	get_alarm_net_time((DATE_TIME *)buf, NULL);

	buf[DATE_TIME_SIZE] = 0x00;
	
	return net_send_event(CMD_RP_FORCE_UNSET_ALARM, buf, DATE_TIME_SIZE + 1, EST_MANAGER);		
}

/*************************************************
  Function:    		sos_alarm_report
  Description: 		������ȱ����¼��ϱ�
  Input:   	   		
  Output:       	��
  Return:			�ϱ��ɹ����
  Others:
*************************************************/
int sos_alarm_report(void)
{
	char time[30];
	char buf[DATE_TIME_SIZE + 1];
	uint32 type = storage_get_area_type_byindex(0x00);

	memset(time, 0, 30);
	get_alarm_net_time((DATE_TIME *)buf, time);
	buf[DATE_TIME_SIZE] = 0x01;
	shortmsg_trans_send(1);
	net_send_event(CMD_RP_SOS_ALARM, buf, DATE_TIME_SIZE + 1, EST_MANAGER);

	#ifdef _IP_MODULE_ALARM
	//ipmodule_transmit_alarmrecord(type, 0x01, time);
	#endif
	return 0;	
}

/*************************************************
  Function:    		break_alarm_report
  Description: 		�������������¼��ϱ�
  Input:   	    	
  	1. area_num     ��������
  Output:       	��
  Return:			�ϱ��ɹ����
  Others:
*************************************************/
int32 break_alarm_report(uint8 area_num)
{
	char time[30];
	char buf[DATE_TIME_SIZE + 1];
	uint32 type = storage_get_area_type_byindex(area_num-1);

	memset(time, 0, 30);
	get_alarm_net_time((DATE_TIME *)buf, time);
	buf[DATE_TIME_SIZE] = area_num;
	shortmsg_trans_send(area_num);

	#ifdef _IP_MODULE_ALARM
	ipmodule_transmit_alarmrecord(type, area_num, time);
	#endif
		
	return net_send_event(CMD_RP_AREA_BREAK_ALARM, buf, DATE_TIME_SIZE + 1, EST_MANAGER);		
}

/*************************************************
  Function:    		alarm_distribute
  Description: 		������ķַ�
  Input:       		
  	1.recPacket		���հ�
  Output:
  Return: 	   
  Others:      		
*************************************************/
int32 alarm_distribute(const PRECIVE_PACKET recPacket)
{
	PNET_HEAD head = (PNET_HEAD)(recPacket->data + MAIN_NET_HEAD_SIZE);
	int cmd = head->command | SSC_ALARM<<8;

	dprintf("alarm_distribute cmd: %x \n", cmd);
	switch(cmd)
	{
		case CMD_ALARM_SET:							// ����	
			set_net_response(recPacket);
			break;	
			
		case CMD_ALARM_UNSET:						// ��������
			unset_net_response(recPacket);
			break;	
			
		case CMD_ALARM_PARTSET:						// �ַ�����
			partset_net_response(recPacket);
			break;	
			
		case CMD_QUERY_AREA_DEFINE:					// ��ѯ��������
			query_alarm_define_response(recPacket);
			break;
			
		case CMD_QUERY_ALARM_STATUS:				// ����״̬��ѯ
			query_alarm_status_response(recPacket);
			break;

		#ifdef _IP_MODULE_ALARM
		case CMD_CLEAR_ALARM:
			dprintf("CMD_CLEAR_ALARM\n");
			ipmodule_clear_alarm(recPacket);
			break;

		case CMD_SOS_REMOTECTRL:
			dprintf("CMD_SOS_REMOTECTRL\n");
			ipmodule_sos_remotectrl(recPacket);
			break;

		case CMD_QUERY_ALARMRECORD:
			dprintf("CMD_QUERY_ALARMRECORD\n");
			ipmodule_query_alarmrecord(recPacket);
			break;
		#endif
		
		default:
			break;
	}
	return TRUE;
}

/*************************************************
  Function:			af_responsion
  Description:		��������Ӧ���������
  Input: 	
  	1.recPacket		Ӧ���
  	2.SendPacket	���Ͱ�
  Output:			��
  Return:			��
  Others:
*************************************************/
void alarm_responsion(const PRECIVE_PACKET recPacket, const PSEND_PACKET SendPacket)
{
	//�����ϱ���������¼����ͣ����û��Ӧ����
}

