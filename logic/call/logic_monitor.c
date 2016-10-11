/*************************************************
  Copyright (C), 2006-2016, Aurine
  File name: 	logic_monitor.c
  Author:    	chenbh
  Version:   	2.0
  Date: 
  Description:  ����
				
  History:        
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#include "logic_include.h"

typedef struct
{
	int32 index;									// �����豸ID
	DEVICE_TYPE_E DevType;							// ���ӵ��豸����
	MONITOR_STATE_E state;							// ��ǰ����״̬
	uint16 TimeMax;									// ��ʱʱ��,�����ʱ��, ��, 0Ϊ���޳�
	uint16 TimeOut;									// ��ʱ,��ʱ�ж�
	uint16 HeartTime;								// ������ʱ
	uint32 address;									// �Է���ַ
	uint16 port;									// �Է��˿�
	uint16 LocalAudioPort;							// ������Ƶ�˿�
	uint16 LocalVideoPort;							// ������Ƶ�˿�
	uint16 RemoteAudioPort;							// Զ����Ƶ�˿�, ͨ����
	PFGuiNotify gui_notify;							// GUI ״̬֪ͨ����
	struct ThreadInfo mThread;						// �߳�
}MONITOR_INFO;

static uint32 g_ErrType;
static MONITOR_INFO g_MonitorInfo;
static MONITOR_INFO g_MonitorlistInfo;
static MONITOR_STATE_E g_PreMonitorState = MONITOR_END;   // �ı�ǰ��״̬
static DEVICE_NO	g_MoniDestDeviceNo;					  // ����Ŀ���豸���
static DATE_TIME g_SnapDateTime = {0};

char g_buf[3];
char g_data[66];
static uint8 g_Video_Start = 0;
static uint8 g_Audio_Start = 0;

static int16 g_RemainTime;
unsigned char g_mac[6] = {0};

#define GuiNotify(param1, param2)	if (g_MonitorInfo.gui_notify)\
	g_MonitorInfo.gui_notify(param1, param2)

#define MonitorListNotify(param1, param2)	if (g_MonitorlistInfo.gui_notify)\
	g_MonitorlistInfo.gui_notify(param1, param2)

/*************************************************
  Function:			InterSetThread
  Description:		
  Input:	
  Output:
  Return:
  Others:
*************************************************/
static void inter_SetThread(struct ThreadInfo *thread)
{	
	if (thread)
	{
		thread->running = 0;
		//thread->thread = -1;
	}
}

/*************************************************
  Function:				monitor_ini
  Description:			���ӳ�ʼ��
  Input: 	
  	1.GuiProc			GUI�ص�����
  Output:				��
  Return:				
  Others:
*************************************************/
void monitor_ini(PFGuiNotify MonitorListProc, PFGuiNotify GuiProc)
{
	g_MonitorlistInfo.gui_notify = MonitorListProc;
	g_MonitorInfo.gui_notify = GuiProc;
	g_MonitorInfo.port = NETCMD_UDP_PORT;
	g_MonitorInfo.LocalAudioPort = NETAUDIO_UDP_PORT;
	g_MonitorInfo.LocalVideoPort = NETVIDEO_UDP_PORT;
	g_MonitorInfo.RemoteAudioPort = NETAUDIO_UDP_PORT;
}

/*************************************************
  Function:			monitor_fill_destdevno
  Description: 		����Ŀ���豸������
  Input: 		  	
  Output:			��
  Return:			BOOLEAN
  Others:
*************************************************/
static void monitor_fill_destdevno(void)
{
	#ifndef _AU_PROTOCOL_
	// mody by caogw ��ֹ�豸��Ź������������
	char tmp[10];
	char pcallno[20];
	char pcallroom[20];
	uint8 calltype;
	uint32 index = 0;
	PFULL_DEVICE_NO pdevno;
	calltype = g_MonitorInfo.DevType;
	index = g_MonitorInfo.index;
	memset(pcallno, 0, sizeof(pcallno));
	if (calltype == DEVICE_TYPE_AREA)
	{
		sprintf(pcallno, "%d", index);
	}
	else if (calltype == DEVICE_TYPE_STAIR)
	{
		pdevno = storage_get_devparam();
		memset(pcallroom, 0, sizeof(pcallroom));
		memcpy(pcallroom, pdevno->DeviceNoStr, pdevno->Rule.StairNoLen);
		sprintf(pcallno, "%s%d", pcallroom, index);
	}
	else
	{
		pdevno = storage_get_devparam();
		memset(pcallroom, 0, sizeof(pcallroom));
		memcpy(pcallroom, pdevno->DeviceNoStr, pdevno->DevNoLen-1);
		if (calltype == DEVICE_TYPE_DOOR_PHONE) // ģ����ǰ��
		{		
			if (index == 1)
			{
				sprintf(pcallno, "%s%d", pcallroom, 6);
			}
			else
			{
				sprintf(pcallno, "%s%d", pcallroom, 7);
			}
		}
		else  // ������ǰ��
		{
			if (index == 1)
			{
				sprintf(pcallno, "%s%d", pcallroom, 8);
			}
			else
			{
				sprintf(pcallno, "%s%d", pcallroom, 9);
			}
		}
	}

	if (strlen(pcallno) > 9)
	{
		memcpy(tmp, pcallno, strlen(pcallno)-9);
		g_MoniDestDeviceNo.DeviceNo1 = atoi(tmp);
		memcpy(tmp, pcallno+strlen(pcallno)-9, 9);
		g_MoniDestDeviceNo.DeviceNo2 = atoi(tmp);
	}
	else
	{
		g_MoniDestDeviceNo.DeviceNo1 = 0;
		g_MoniDestDeviceNo.DeviceNo2 = atoi(pcallno);
	}
	memset(&g_MoniDestDeviceNo, 0, sizeof(DEVICE_NO));
	g_MoniDestDeviceNo.AreaNo = storage_get_areano();
	g_MoniDestDeviceNo.DeviceType =  g_MonitorInfo.DevType;
	#endif
}

#ifdef _DOOR_PHONE_
/*************************************************
  Function:			phone_monitor_proc
  Description: 		ģ����ǰ�������߳�
  Input: 			
  	1.param			����
  Output:			��
  Return:			BOOLEAN
  Others:
*************************************************/
static void* phone_monitor_proc(void *param)
{
	// ���÷����߳�
	pthread_detach(pthread_self());
	time_t t0;
	//int32 RequestTimes = 3;  
	int32 ret = FALSE;
	static int32 times = 10;
	
	
LabChange:

	t0 = time(0);
	g_Video_Start = 0;
	//RequestTimes = 3;
	if (MONITOR_REQUEST == g_MonitorInfo.state)			
	{
		g_PreMonitorState = MONITOR_REQUEST;
		GuiNotify(g_MonitorInfo.state, g_MonitorInfo.index);
		g_MonitorInfo.state = MONITOR_MONITORING;  // ģ����ǰ��ֱ�Ӽ��� 
		usleep(1000);
		#if 0
		//  3�β��ɹ����˳�
		while (RequestTimes)
		{
			// ���������ӽӿڡ������Ŵ���
			//ret = [�������ӽӿں���];
			if (ret == FALSE)
			{
				RequestTimes--;
			}
			else
			{
				g_MonitorInfo.state = MONITOR_MONITORING;
				break;
			}
		}
		if (RequestTimes <= 0)
		{
			g_MonitorInfo.state = MONITOR_END;
		}
		#endif
	}

	t0 = time(0);
	if(MONITOR_MONITORING == g_MonitorInfo.state)	
	{	
		dprintf(" *************** MONITOR_MONITORING ******************* \n");
		g_PreMonitorState = MONITOR_MONITORING;
		hw_mk_start();
		// [������Ƶ�ӿ�]
		ret = media_start_analog_video();
		if (ret == -1)
		{
			g_MonitorInfo.state = MONITOR_END;
			g_PreMonitorState = MONITOR_END;
		}
		else
		{
			GuiNotify(g_MonitorInfo.state, 0);	
			g_Video_Start = 1;
			
			g_MonitorInfo.TimeMax = MONITOR_TIME_MAX;	
			g_MonitorInfo.TimeOut = 0;
			g_RemainTime = 0;
		}
		
		while (MONITOR_MONITORING == g_MonitorInfo.state)
		{
			g_MonitorInfo.TimeOut = time(0) - t0;
			if (g_MonitorInfo.TimeMax > 0)
			{
				//ʣ��ʱ��
				g_RemainTime = g_MonitorInfo.TimeMax - g_MonitorInfo.TimeOut;
				dprintf("g_RemainTime : %d\n ", g_RemainTime);
				if (g_RemainTime <= 0)
				{
					g_ErrType = MONITOR_MONITORING_TIMEOUT;
					g_MonitorInfo.state = MONITOR_END;
					break;
				}
				else
				{
					//GuiNotify(MONITOR_TIMER, g_RemainTime);
				}

				times = 10;
				while ((times--) > 0 && MONITOR_MONITORING == g_MonitorInfo.state)
				{
					usleep(100*1000);
				}
			}
		}
	}

	t0 = time(0);
	if(MONITOR_TALKING == g_MonitorInfo.state)
	{
		dprintf("MONITOR_TALKING == g_MonitorInfo.state \n");

		// [���ӽ���ͨ���ӿ�]
		//hw_switch_analog();
		hw_mk_talk();
		
		g_PreMonitorState = MONITOR_TALKING;
		GuiNotify(g_MonitorInfo.state, 0);
		
		g_MonitorInfo.TimeMax = MONITOR_TALK_TIME_MAX;		
		g_MonitorInfo.TimeOut = 0;
		g_MonitorInfo.HeartTime = 0;
		
		while (MONITOR_TALKING == g_MonitorInfo.state)
		{
			if (g_MonitorInfo.TimeMax > 0)
			{
				g_RemainTime = 0;
				g_MonitorInfo.TimeOut = time(0) - t0;
				g_RemainTime = g_MonitorInfo.TimeMax - g_MonitorInfo.TimeOut;					
				if (g_RemainTime <= 0)
				{
					dprintf("monitor timer proc : talking time out\n");
					g_ErrType = MONITOR_TALKING_TIMEOUT;
					g_MonitorInfo.state = MONITOR_END;
					break;
				}
				else
				{
					//GuiNotify(MONITOR_TIMER, g_RemainTime);
				}
					
				times = 10;
				while ((times--) > 0 && MONITOR_TALKING == g_MonitorInfo.state)
				{
					usleep(100*1000);
				}
			}
		}
	}

	// ������һ����� 
	if (MONITOR_REQUEST == g_MonitorInfo.state)
	{
		// [�ر���Ƶ�ӿ�]
		media_stop_analog_video();
		goto LabChange;
	}
	
	dprintf("monitor proc : AS_MONITOR_END : g_ErrType : %d\n", g_ErrType);
	media_stop_analog_video();	
	g_Video_Start = 0;
	hw_switch_digit();  // [�رռ��ӽӿ�]
	sys_set_monitor_state(FALSE);
	dprintf("monitor proc end!\n");
	
	g_MonitorInfo.state = MONITOR_END;
	g_PreMonitorState = MONITOR_END;
	GuiNotify(g_MonitorInfo.state, g_ErrType);
	inter_SetThread(&g_MonitorInfo.mThread);

	pthread_exit(NULL);
	return NULL;             // ���غ���Դ��ϵͳ�ͷ�
}
#endif

/*************************************************
  Function:			monitor_proc
  Description: 		�����߳�
  Input: 			
  	1.param			����
  Output:			��
  Return:			BOOLEAN
  Others:
*************************************************/
static void* monitor_proc(void *param)
{
	// ���÷����߳�
	pthread_detach(pthread_self());
	time_t t0;
	int32 size = 0;
	int32 ret = FALSE;
	static int32 times = 10;	
	
LabChange:
	g_Video_Start = 0;
	g_Audio_Start = 0;
	ret = media_start_net_video(g_MonitorInfo.address, _RECVONLY);
	if (ret == TRUE)
	{
		dprintf("monitor_proc : monitoring : start net video OK\n");
		g_Video_Start = 1;
	}
	
	t0 = time(0);
	if (MONITOR_REQUEST == g_MonitorInfo.state)			
	{
		g_PreMonitorState = MONITOR_REQUEST;
		monitor_fill_destdevno();
		GuiNotify(g_MonitorInfo.state, g_MonitorInfo.index);
		g_MonitorInfo.TimeOut = 0;
		g_MonitorInfo.HeartTime = 0;
		while (MONITOR_REQUEST == g_MonitorInfo.state)
		{
			memset(g_data, 0, sizeof(g_data));
			memcpy(g_data, &g_MonitorInfo.LocalVideoPort, 2);
			g_MonitorInfo.TimeOut = time(0) - t0;
			// ��������5s��û�л�Ӧ�˳�
			if (g_MonitorInfo.TimeOut >= 5)
			{
				dprintf("monitor_timer_proc : request time out \n");
				g_ErrType = MONITOR_REQUEST_TIMEOUT;
				g_MonitorInfo.state = MONITOR_END;
				break;
			}

			// ����ÿ�뷢��һ����������
			if (g_MonitorInfo.DevType == DEVICE_TYPE_DOOR_NET)							
			{
				// ����������ǰ��
				uint8 *tmp = NULL;
				MAC_TYPE MacType = DOOR1_MAC;
				if (g_MonitorInfo.index == 1)
				{
					MacType = DOOR1_MAC;
				}
				else
				{
					MacType = DOOR2_MAC;
				}
				
				tmp = storage_get_mac(MacType);
				memcpy(g_mac, tmp, 6);
				
				g_data[2] = DEVICE_TYPE_DOOR_NET;
				g_data[3] = (uint8)g_MonitorInfo.index;
				
				#ifdef _SEND_SDP_PARAM_
				memcpy(&g_data[4], (char *)(&g_venc_parm), 16);
				memcpy(&g_data[20], (char *)(&g_audio_parm), 8);
				memcpy(&g_data[28], g_mac, 6);
				size = 66;
				#else
				memcpy(&g_data[4], g_mac, 6);
				size = 10;
				#endif
				
				set_nethead(g_MoniDestDeviceNo, PRIRY_DEFAULT);
				net_direct_send(CMD_MONITOR, g_data, size, g_MonitorInfo.address, g_MonitorInfo.port);
			}
			else
			{
				#ifdef _TY_STAIR_
				if (storage_get_extmode(EXT_MODE_GENERAL_STAIR))
				{
					g_data[2] = (uint8)g_MonitorInfo.DevType;
					g_data[3] = (uint8)g_MonitorInfo.index;
					
					#ifdef _SEND_SDP_PARAM_
					memcpy(&g_data[4], (char *)(&g_venc_parm), 16);
					memcpy(&g_data[20], (char *)(&g_audio_parm), 8);
					size = 66;
					#else
					size = 4;
					#endif
					
					set_nethead(g_MoniDestDeviceNo, PRIRY_DEFAULT);
					net_direct_send(CMD_MONITOR, g_data, size, g_MonitorInfo.address, g_MonitorInfo.port);
				}
				else
				#endif
				{
					g_data[2] = (uint8)g_MonitorInfo.DevType;
					g_data[3] = (uint8)g_MonitorInfo.index;
					
					#ifdef _SEND_SDP_PARAM_
					memcpy(&g_data[4], (char *)(&g_venc_parm), 16);
					memcpy(&g_data[20], (char *)(&g_audio_parm), 8);
					size = 66;
					#else
					size = 4;
					#endif
					set_nethead(g_MoniDestDeviceNo, PRIRY_DEFAULT);
					net_direct_send(CMD_MONITOR, g_data, size, g_MonitorInfo.address, g_MonitorInfo.port);
				}		
			}	

			// ˯��ʱ���Ӱ����Ƶ��������
			times = 10;
			while ((times--) > 0 && MONITOR_REQUEST == g_MonitorInfo.state)
			{
				usleep(100*1000);
			}
		}

		// �������ת����̫�� UI�л�ʧ������
		if (time(0) - t0 < 2)
		{
			usleep(500*1000);
		}
	}

	t0 = time(0);
	if(MONITOR_MONITORING == g_MonitorInfo.state)	
	{
		g_MonitorInfo.TimeMax = MONITOR_TIME_MAX;	
		g_MonitorInfo.TimeOut = 0;
		g_MonitorInfo.HeartTime = 0;
		g_RemainTime = 0;
		while (MONITOR_MONITORING == g_MonitorInfo.state)
		{
			g_MonitorInfo.TimeOut = time(0) - t0;
			if (g_MonitorInfo.TimeMax > 0)
			{
				// ʣ��ʱ��
				g_RemainTime = g_MonitorInfo.TimeMax - g_MonitorInfo.TimeOut;
				if (g_RemainTime <= 0)
				{
					set_nethead(g_MoniDestDeviceNo, PRIRY_DEFAULT);
					net_direct_send(CMD_STOP_MONITOR, g_buf, 2, g_MonitorInfo.address, g_MonitorInfo.port);
					g_ErrType = MONITOR_MONITORING_TIMEOUT;
					g_MonitorInfo.state = MONITOR_END;
					break;
				}
				else
				{
					GuiNotify(MONITOR_TIMER, g_RemainTime);
					set_nethead(g_MoniDestDeviceNo, PRIRY_DEFAULT);
					net_direct_send(CMD_MONITOR_HEART, g_buf, 2, g_MonitorInfo.address, g_MonitorInfo.port);
				}

				if (g_MonitorInfo.HeartTime > HEART_TIMEOUT)
				{
					g_ErrType = MONITOR_HEART_TIMEOUT;
					g_MonitorInfo.state = MONITOR_END;
					break;
				}
				g_MonitorInfo.HeartTime++;

				times = 10;
				while ((times--) > 0 && (MONITOR_MONITORING == g_MonitorInfo.state))
				{
					usleep(100*1000);
				}
			}
		}
	}

	if (MONITOR_TALKING == g_MonitorInfo.state)
	{
		hw_switch_digit(); 				// �л������ֶԽ� 				
		media_set_talk_volume(g_MonitorInfo.DevType, storage_get_talkvolume());
		media_set_mic_volume(storage_get_micvolume());
		if (media_start_net_audio(g_MonitorInfo.address))
		{						
			dprintf("media_start_net_audio return ok\n ");
			g_Audio_Start = 1;			
			media_add_audio_sendaddr(g_MonitorInfo.address, g_MonitorInfo.RemoteAudioPort);
			// add by chenbh 2016-08-22
			media_enable_audio_send();
			media_enable_audio_recv();
		}	
		else
		{						
			dprintf(" media_start_net_audio return error\n ");
			g_MonitorInfo.state = MONITOR_END;
		}
	}
	
	t0 = time(0);
	if (MONITOR_TALKING == g_MonitorInfo.state)
	{				
		dprintf("MONITOR_TALKING == g_MonitorInfo.state \n");
		GuiNotify(g_MonitorInfo.state, 0);

		g_PreMonitorState = MONITOR_TALKING;
		g_MonitorInfo.TimeMax = MONITOR_TALK_TIME_MAX;		
		g_MonitorInfo.TimeOut = 0;
		g_MonitorInfo.HeartTime = 0;
		
		while (MONITOR_TALKING == g_MonitorInfo.state)
		{
			if (g_MonitorInfo.TimeMax > 0)
			{
				g_RemainTime = 0;
				g_MonitorInfo.TimeOut = time(0) - t0;
				// ʣ��ʱ��
				//g_RemainTime = g_MonitorInfo.TimeMax - g_MonitorInfo.TimeOut;					
				if (g_MonitorInfo.TimeOut >= g_MonitorInfo.TimeMax)
				{
					dprintf("monitor timer proc : talking time out\n");
					set_nethead(g_MoniDestDeviceNo, PRIRY_DEFAULT);
					net_direct_send(CMD_STOP_MONITOR, g_buf, 2, g_MonitorInfo.address, g_MonitorInfo.port);
					g_ErrType = MONITOR_TALKING_TIMEOUT;
					g_MonitorInfo.state = MONITOR_END;
					break;
				}
				else
				{
					g_RemainTime = g_MonitorInfo.TimeMax - g_MonitorInfo.TimeOut;
					GuiNotify(MONITOR_TIMER, g_RemainTime);
					set_nethead(g_MoniDestDeviceNo, PRIRY_DEFAULT);
					net_direct_send(CMD_MONITOR_HEART, g_buf, 2, g_MonitorInfo.address, g_MonitorInfo.port);
				}
				
				if (g_MonitorInfo.HeartTime > HEART_TIMEOUT)
				{
					g_ErrType = MONITOR_HEART_TIMEOUT;
					g_MonitorInfo.state = MONITOR_END;
					break;
				}
				g_MonitorInfo.HeartTime++;
				
				times = 10;
				while ((times--) > 0 && (MONITOR_TALKING == g_MonitorInfo.state))
				{
					usleep(100*1000);
				}
			}
		}
	}

	// ������һ����� �ûص���������
	if (MONITOR_REQUEST == g_MonitorInfo.state)
	{
		media_stop_net_video(_RECVONLY);
		goto LabChange;
	}

	dprintf("monitor proc : g_PreMonitorState : %d\n", g_PreMonitorState);
	if (g_Audio_Start == 1)
	{
		media_del_audio_send_addr(g_MonitorInfo.address, MEDIA_AUDIO_PORT);
		usleep(10*1000);
		media_stop_net_audio();		
	}

	// �ر���Ƶ�ӿ�
	if (g_Video_Start == 1)
	{
		g_Video_Start = 0;
		media_stop_net_video(_RECVONLY);	
	}
	
	dprintf("monitor proc : AS_MONITOR_END : g_ErrType : %d\n", g_ErrType);
	sys_set_monitor_state(FALSE);
	dprintf("monitor proc end!\n");
	g_MonitorInfo.state = MONITOR_END;
	g_PreMonitorState = MONITOR_END;
	GuiNotify(g_MonitorInfo.state, g_ErrType);
	g_ErrType = MONITOR_OK;
	inter_SetThread(&g_MonitorInfo.mThread);

	pthread_exit(NULL);
	return NULL;             	// ���غ���Դ��ϵͳ�ͷ�
}

/*************************************************
  Function:		get_next_stair
  Description: 	�����¸��ݿ�
  Input: 		
  	1.direct	������һ������һ��
  Output:		
  	1.ipaddr	�ݿ�ip
  	2.index		�ݿں�����
  Return:		��
  Others:
*************************************************/
static int32 get_next_stair(int32 direct, uint32 *ipaddr, int32 *index)
{
	int32 i, ID = -1;
	uint32 Address = 0;

	// ������һ��
	if (direct)
	{
		if (g_MonitorInfo.index < (STAIR_DEVICE_NUM-1))
		{
			for (i = g_MonitorInfo.index + 1; i < STAIR_DEVICE_NUM; i++)
			{
				Address = get_stair_address_inlist(i);
				if (0 != Address)
				{
					ID = i;
					break;
				}
			}
		}
		
		// not fount
		if (-1 == ID)
		{
			for (i = 0; i < g_MonitorInfo.index; i++)
			{
				Address = get_stair_address_inlist(i);
				if (0 != Address)
				{
					ID = i;
					break;
				}
			}
		}
	}
	// ������һ��
	else
	{
		if (g_MonitorInfo.index > 0)
		{
			for (i = g_MonitorInfo.index-1; i > -1; i--)
			{
				Address = get_stair_address_inlist(i);
				if (0 != Address)
				{
					ID = i;
					break;
				}
			}
		}
		
		// not found
		if (-1 == ID)
		{
			for (i = STAIR_DEVICE_NUM - 1; i > g_MonitorInfo.index; i--)
			{
				//Address = get_stair_address_inlist(ID);
				Address = get_stair_address_inlist(i);
				if (0 != Address)
				{
					ID = i;
					break;
				}
			}
		}
	}
	
	if (-1 == ID || Address == 0)
	{
		dprintf("get next stiar : don't get next stair\n");
		return FALSE;
	}
	else
	{
		*ipaddr = Address;
		*index = ID;
		dprintf("get next stiar : ipaddr:0x%x, index:%d\n", *ipaddr, *index);
		return TRUE;
	}
}

/*************************************************
  Function:		get_next_stair
  Description: 	�����¸�����
  Input: 		
  	1.direct	������һ������һ��
  Output:		
  	1.ipaddr	�ݿ�ip
  	2.index		�ݿں�����
  Return:		��
  Others:
*************************************************/
static int32 get_next_area(int32 direct, uint32 *ipaddr, int32 *index)
{
	int32 i, ID = -1;
	uint32 Address = 0;

	if (direct)
	{
		if (g_MonitorInfo.index < AREA_DEVICE_NUM)
		{
			for (i = g_MonitorInfo.index + 1; i <= AREA_DEVICE_NUM; i++)
			{
				Address = get_area_address_inlist(i);
				if (0 != Address)
				{
					ID = i;
					break;
				}
			}
		}
		if (-1 == ID)
		{
			for (i = 1; i < g_MonitorInfo.index; i++)
			{
				Address = get_area_address_inlist(i);
				if (0 != Address)
				{
					ID = i;
					break;
				}
			}
		}
	}
	else
	{
		if (g_MonitorInfo.index > 1)
		{
			for (i = g_MonitorInfo.index - 1; i > 0; i--)
			{
				Address = get_area_address_inlist(i);
				if (0 != Address)
				{
					ID = i;
					break;
				}
			}
		}
		if (-1 == ID)
		{
			for (i = AREA_DEVICE_NUM; i > g_MonitorInfo.index; i--)
			{
				Address = get_area_address_inlist(ID);
				if (0 != Address)
				{
					ID = i;
					break;
				}
			}
		}
	}
	
	if (-1 == ID || Address == 0)
	{
		dprintf("get next area : don't get next area\n");
		return FALSE;
	}
	else
	{
		*ipaddr = Address;
		*index = ID;
		dprintf("get next area : ipaddr:0x%x, index:%d\n", *ipaddr, *index);
		return TRUE;
	}
}

/*************************************************
  Function:		get_next_netdoor
  Description: 	�����¸��ݿ�
  Input: 		
  	1.direct	������һ������һ��
  Output:		
  	1.ipaddr	�ݿ�ip
  	2.index		�ݿں�����
  Return:		��
  Others:
*************************************************/
static int32 get_next_netdoor(int32 direct, uint32 *ipaddr, int32 *index)
{
	int32 ID = -1;
	uint32 Address = 0;

	if (g_MonitorInfo.index == 1)
	{
		Address = get_netdoor_address_inlist(2);
		#ifdef _IP_MODULE_DJ_
		set_netdoor_ip(EXT_MODE_NETDOOR2, Address);
		#endif
		if (Address != 0)
		{	
			ID = 2;
		}
	}
	else
	{
		Address = get_netdoor_address_inlist(1);
		#ifdef _IP_MODULE_DJ_
		set_netdoor_ip(EXT_MODE_NETDOOR1, Address);
		#endif
		if (Address != 0)
		{	
			ID = 1;
		}
	}
	
	if (-1 == ID || Address == 0)
	{
		dprintf("get next netdoor : don't get next netdoor\n");
		return FALSE;
	}
	else
	{
		*ipaddr = Address;
		*index = ID;
		dprintf("get next netdoor : ipaddr:0x%x, index:%d\n", *ipaddr, *index);
		return TRUE;
	}
}

/*************************************************
  Function:		get_next_phonedoor
  Description: 	�����¸�ģ����ǰ��
  Input: 		
  	1.direct	������һ������һ��
  Output:		
  	1.ipaddr	�ݿ�ip
  	2.index		�ݿں�����
  Return:		��
  Others:
*************************************************/
static int32 get_next_phonedoor(int32 direct, uint32 *ipaddr, int32 *index)
{
	int32 ret = 0;
	if (g_MonitorInfo.index == 1)
	{
		ret = get_phonedoor_address_inlist(2);
		if (ret == 0)
		{
			return FALSE;
		}
		else
			*index = 2;
	}
	else
	{
		ret = get_phonedoor_address_inlist(1);
		if (ret == 0)
		{
			return FALSE;
		}
		else
			*index = 1;
	}
	
	return TRUE;
}

/*************************************************
  Function:			monitor_start
  Description:		��������
  Input: 	
   	1.DevType		�豸����
  	2.index			�豸���� ��0��ʼ
  Output:			��
  Return:			�ɹ����, TRUE / FALSE
  Others:
*************************************************/
int32 monitor_start(DEVICE_TYPE_E DevType, int8 index)
{
	PMONITORINFO info = NULL;
	dprintf(" devtype :%d , index : %d \n", DevType, index);
	int32 ret = sys_set_monitor_state(TRUE);
	if (ret != 0)
	{
		dprintf("monitor search dev : sys is busy : state:%d\n", ret);
		return FALSE;
	}
	
	info = storage_get_monitorinfo(DevType, index);
	if (info == NULL)
	{
	 	dprintf(" storage_get_monitorinfo return NULL \n");
		sys_set_monitor_state(FALSE);	// add by caogw 2015-9-25 ��������б�ʧ��ʱ������ý��״̬
		return FALSE;
	}
	
	g_MonitorInfo.DevType = info->DeviceType;
	g_MonitorInfo.address = info->IP;
	g_MonitorInfo.index = info->index;
	free(info);	
	info = NULL;
	dprintf("DevType : %x, address: %x, index: %d\n", g_MonitorInfo.DevType, g_MonitorInfo.address, g_MonitorInfo.index);

	// ����״̬
	g_MonitorInfo.state = MONITOR_REQUEST;

	switch (g_MonitorInfo.DevType)
	{
		#ifdef _DOOR_PHONE_
		case DEVICE_TYPE_DOOR_PHONE:
		{ 
			if (0 != inter_start_thread(&g_MonitorInfo.mThread, phone_monitor_proc, (void*)&g_MonitorInfo, index))
			{
				g_MonitorInfo.state = MONITOR_END;
				return FALSE;
			}
			break;
		}
		#endif

		case DEVICE_TYPE_DOOR_NET:
		case DEVICE_TYPE_AREA:
		case DEVICE_TYPE_STAIR:
		{
			if (0 != inter_start_thread(&g_MonitorInfo.mThread, monitor_proc, (void*)&g_MonitorInfo, index))
			{
				g_MonitorInfo.state = MONITOR_END;
				return FALSE;
			}
			break;
		}

		default:
			dprintf(" monitor devtype is not support!!!!!! \n");
			return FALSE;
	}
	return TRUE;
}

/*************************************************
  Function:			monitor_next
  Description:		�����л�
  Input: 	
  	1.direct		����: TRUE-��һ�� FALSE-��һ��
  Output:			��
  Return:			�ɹ����, TRUE / FALSE
  Others:
*************************************************/
int32 monitor_next(int32 direct)
{
	uint32 ipaddr = 0;
	int32 index = 0;
	int32 ret = FALSE;
	
	if (g_MonitorInfo.state != MONITOR_MONITORING)
	{
		dprintf("monitor next : state err : %d\n", g_MonitorInfo.state);
		return FALSE;
	}

	switch (g_MonitorInfo.DevType)
	{
		case DEVICE_TYPE_STAIR:
			ret = get_next_stair(direct, &ipaddr, &index);			
			break;
			
		case DEVICE_TYPE_AREA:
			ret = get_next_area(direct, &ipaddr, &index);
			break;
			
		case DEVICE_TYPE_DOOR_NET:
			ret = get_next_netdoor(direct, &ipaddr, &index);
			break;

		case DEVICE_TYPE_DOOR_PHONE:
			ret = get_next_phonedoor(direct, NULL, &index);
			break; 

		default:
			dprintf("DevType[ %x] is not true\n", g_MonitorInfo.DevType);
			break;
	}

	// ģ����ǰ�� ���⴦��
	if (DEVICE_TYPE_DOOR_PHONE == g_MonitorInfo.DevType)
	{
		if (ret == TRUE)
		{
			// �����¸�ģ����ǰ��
			g_MonitorInfo.index = index;
			g_MonitorInfo.state = MONITOR_REQUEST;  // ģ����ǰ��ֱ�Ӽ��� 
			return TRUE;
		}
		return FALSE;
	}

	if (ipaddr == 0)
	{
		return FALSE;
	}
	else
	{
		set_nethead(g_MoniDestDeviceNo, PRIRY_DEFAULT);
		net_direct_send(CMD_STOP_MONITOR, NULL, 0, g_MonitorInfo.address, g_MonitorInfo.port);
		
		g_MonitorInfo.address = ipaddr;
		g_MonitorInfo.index = index;
		g_MonitorInfo.state = MONITOR_REQUEST;
		return TRUE;
	}
}

/*************************************************
  Function:			monitor_change
  Description:		�л�������ĳһ���豸
  Input: 	
  	1.index			�豸����
  Output:			��
  Return:			�ɹ����, TRUE / FALSE
  Others:
*************************************************/
int32 monitor_change(int32 index)
{
	uint32 ipaddr = 0;
	
	if (g_MonitorInfo.state != MONITOR_MONITORING)
	{
		return FALSE;
	}

	switch (g_MonitorInfo.DevType)
	{
		case DEVICE_TYPE_STAIR:
			ipaddr = get_stair_address(index);
			break;
			
		case DEVICE_TYPE_AREA:
			ipaddr = get_area_address(index);
			break;
			
		case DEVICE_TYPE_DOOR_NET:
			ipaddr = get_door_address(index);
			break;
			
		case DEVICE_TYPE_DOOR_PHONE:
			ipaddr = get_phonedoor_address(index);
			break;
			
		default:
			dprintf("monitor change : dev type err\n");
			return FALSE;
	}

	// ģ����ǰ�����⴦��
	if (DEVICE_TYPE_DOOR_PHONE == g_MonitorInfo.DevType)
	{
		if (ipaddr == TRUE)
		{
			// �����¸�ģ����ǰ��
			g_MonitorInfo.index = index;
			g_MonitorInfo.state = MONITOR_REQUEST;
			return TRUE;
		}
		return FALSE;
	}

	if (ipaddr != 0)
	{
		set_nethead(g_MoniDestDeviceNo, PRIRY_DEFAULT);
		net_direct_send(CMD_STOP_MONITOR, NULL, 0, g_MonitorInfo.address, g_MonitorInfo.port);
		
		g_MonitorInfo.index = index;
		g_MonitorInfo.address = ipaddr;
		g_MonitorInfo.state = MONITOR_REQUEST;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*************************************************
  Function:			monitor_talk
  Description:		����תͨ��
  Input: 	
  Output:			��
  Return:			�ɹ����, TRUE / FALSE
  Others:
*************************************************/
int32 monitor_talk(void)
{
	char * data = (char *)&g_MonitorInfo.LocalAudioPort;			
	if (g_MonitorInfo.state != MONITOR_MONITORING)
	{
		return FALSE;
	}

	if (g_MonitorInfo.DevType == DEVICE_TYPE_DOOR_PHONE)
	{
		g_MonitorInfo.state = MONITOR_TALKING;
	}
	else
	{
		set_nethead(g_MoniDestDeviceNo, PRIRY_DEFAULT);
		net_direct_send(CMD_MONITOR_TO_TALK, data, 2, g_MonitorInfo.address, g_MonitorInfo.port);
	}
	return TRUE;
}

/*************************************************
  Function:			ui_monitor_stop
  Description:		�������ӻ�ͨ�� UI���һ����øýӿ�
  Input: 	
  Output:			��
  Return:			�ɹ����, TRUE / FALSE
  Others:
*************************************************/
int32 ui_monitor_stop(void)
{
	if (g_MonitorInfo.state == MONITOR_END)
	{
		if (g_MonitorInfo.mThread.running == 0)
		{
			sys_set_monitor_state(FALSE);
			GuiNotify(g_MonitorInfo.state, MONITOR_OK);
		}
	}
	else if (g_MonitorInfo.state != MONITOR_END)
	{
		g_ErrType = MONITOR_OK;

		if (g_MonitorInfo.DevType == DEVICE_TYPE_DOOR_PHONE)
		{
			g_MonitorInfo.state = MONITOR_END;
		}
		else
		{
			set_nethead(g_MoniDestDeviceNo, PRIRY_DEFAULT);
			net_direct_send(CMD_STOP_MONITOR, NULL, 0, g_MonitorInfo.address, g_MonitorInfo.port);		
		}
		
		g_MonitorInfo.state = MONITOR_END;
	}
	return TRUE;
}

/*************************************************
  Function:			monitor_stop
  Description:		�������ӻ�ͨ��
  Input: 	
  Output:			��
  Return:			�ɹ����, TRUE / FALSE
  Others:
*************************************************/
int32 monitor_stop(void)
{
	if (g_MonitorInfo.state != MONITOR_END)
	{
		g_ErrType = MONITOR_OK;

		if (g_MonitorInfo.DevType == DEVICE_TYPE_DOOR_PHONE)
		{
			g_MonitorInfo.state = MONITOR_END;
		}
		else
		{
			set_nethead(g_MoniDestDeviceNo, PRIRY_DEFAULT);
			net_direct_send(CMD_STOP_MONITOR, NULL, 0, g_MonitorInfo.address, g_MonitorInfo.port);		
		}
		
		g_MonitorInfo.state = MONITOR_END;
	}
	return TRUE;
}


/*************************************************
  Function:			monitor_video_snap_callback
  Description:		ץ�Ļص�����
  Input: 	
  Output:			��
  Return:			�ɹ����, TRUE/FALSE
  Others:	
*************************************************/
static void monitor_video_snap_callback(uint8 state)
{
	if (state == TRUE)
	{
		char DevStr[20] = {0};
		if (g_MonitorInfo.DevType == DEVICE_TYPE_STAIR)
		{
			char StairNo[5] = {0};
			sprintf(StairNo, "%d", g_MonitorInfo.index);
			get_stair_fullno(DevStr, StairNo[0]);
		}
		else
		{
			sprintf(DevStr, "%d", g_MonitorInfo.index);
		}	
		storage_add_photo(g_MonitorInfo.DevType, DevStr, g_SnapDateTime);
	}

	GuiNotify(MONITOR_SNAP, state);
}

/*************************************************
  Function:			monitor_video_snap
  Description:		ץ��
  Input: 	
  Output:			��
  Return:			�ɹ����, TRUE/FALSE
  Others:	
*************************************************/
int32 monitor_video_snap(void)
{
	uint32 ret;
	char FileName[50] = {0};
	
	dprintf("[%d]\n", __LINE__);	
	get_timer(&g_SnapDateTime);
	get_photo_path(FileName, &g_SnapDateTime);
	if (g_MonitorInfo.state == MONITOR_MONITORING || g_MonitorInfo.state == MONITOR_TALKING)
	{
		ret = media_snapshot(FileName, monitor_video_snap_callback);			
	}	
	else
	{
		ret = FALSE;
	}
	return ret;
}

/*************************************************
  Function:			monitor_unlock
  Description:		���ڻ����ӿ���
  Input: 			��
  Output:			��
  Return:			�ɹ����
  Others:
*************************************************/
int32 monitor_unlock(void)
{
	int32 ret = 0;
	switch (g_MonitorInfo.state)
	{
		case MONITOR_MONITORING:
		case MONITOR_TALKING:
		{	
			#ifdef _DOOR_PHONE_
			if (g_MonitorInfo.DevType == DEVICE_TYPE_DOOR_PHONE)
			{
				int32 ret1 = hw_mk_lock();
				if (ret1 == 0)
				{
					ret = TRUE;
				}
				else
				{
					ret = FALSE;
				}
			}
			else
			#endif
			{
				uint8 EchoValue;
				#if 0     // modi by chenbh �ͱ���һ�¿��������Ӧ��
				int32 ret1 = net_send_command(CMD_UNLOCK, NULL, 0, g_MonitorInfo.address, g_MonitorInfo.port, 2, &EchoValue, NULL, NULL);
				if (ret1 && EchoValue == ECHO_OK)
				{
					ret = TRUE;
				}
				else
				{
					ret = FALSE;
				}
				#else
				set_nethead(g_MoniDestDeviceNo, PRIRY_REALTIME);
				net_direct_send(CMD_UNLOCK, NULL, 0, g_MonitorInfo.address, g_MonitorInfo.port);	
				ret = TRUE;
				#endif
			}
			break;
		}
		default:
			ret = FALSE;
			break;
	}
	return ret;
}

/*************************************************
  Function:		fill_devno_by_index
  Description: 	����豸���
  Input: 		
  	1.DevType   �豸����
  	2.index		�豸����
  Output:		��
  Return:		
  Others:		
*************************************************/
void fill_devno_by_index(DEVICE_TYPE_E DevType, int8 index, char *devno)
{
	if (DEVICE_TYPE_AREA == DevType || DEVICE_TYPE_DOOR_NET == DevType || DEVICE_TYPE_DOOR_PHONE == DevType)
	{
		sprintf(devno, "%d", index);
	}
	else if (DEVICE_TYPE_STAIR == DevType)
	{
		PFULL_DEVICE_NO pdevno;
		// mody by caogw ��������ռ䣬Ԥ���������
		char stairno[10] = {0};
		pdevno = storage_get_devparam();
		memset(stairno, 0, sizeof(stairno));
		memcpy(stairno, pdevno->DeviceNoStr, pdevno->Rule.StairNoLen);
		sprintf(devno, "%s%05d", stairno, index);
	}
	dprintf("fill_devno_by_index  devno :%s \n", devno);
}

/*************************************************
  Function:		get_monitor_sync_devlist
  Description: 	��ȡ�豸�б�
  Input: 		��
  Output:		��
  Return:		
  Others:		
*************************************************/
static void* get_monitor_sync_devlist (void *param)
{
	int32 ret = FALSE;

	dprintf("monitor proc : search dev start\n");
	
	g_MonitorInfo.state = MONITOR_SEARCH;

	switch (g_MonitorInfo.DevType)
	{
		case DEVICE_TYPE_STAIR:
			ret = search_stair_list();
			break;
		case DEVICE_TYPE_AREA:
			ret = search_area_list();
			break;
		case DEVICE_TYPE_DOOR_NET:
		case DEVICE_TYPE_DOOR_PHONE:
			ret = search_door_list();
			break;
		default:	
			MonitorListNotify(MONITOR_GETLIST, FALSE);
			g_MonitorInfo.state = MONITOR_END;
			return NULL;
	}

	if (ret == TRUE)
	{		
		MonitorListNotify(MONITOR_GETLIST, TRUE);
	}
	else
	{
		MonitorListNotify(MONITOR_GETLIST, FALSE);
	}
	
	g_MonitorInfo.state = MONITOR_END;
	return NULL;
}

/*************************************************
  Function:		rtsp_monitor_sync_devlist
  Description: 	��ȡRTSP�豸�б�
  Input: 		��
  Output:		��
  Return:		PMonitorDeviceList �豸�б�
  Others:		
*************************************************/
uint32 monitorlist_sync_devlist(DEVICE_TYPE_E DevType)
{
	g_MonitorInfo.DevType = DevType;
	struct ThreadInfo mThread;	
	if (0 != inter_start_thread(&mThread, get_monitor_sync_devlist, NULL, 0))
	{
		g_MonitorInfo.state = MONITOR_END;
		return FALSE;
	}
	
	return TRUE;
}


/*************************************************
  Function:			monitor_distribute
  Description:		��������Ͱ�������
  Input: 	
  	1.recPacket		���������
  Output:			��
  Return:			�ɹ����,true/fasle
  Others:
*************************************************/
int32 monitor_distribute(const PRECIVE_PACKET recPacket)
{
	PNET_HEAD head = (PNET_HEAD)(recPacket->data + MAIN_NET_HEAD_SIZE);
	uint16 cmd = head->command | SSC_INTERPHONE << 8;
	switch (cmd)
	{	
		case CMD_STOP_MONITOR:											//�жϼ���
			if (g_MonitorInfo.address == recPacket->address)	
			{	
				// ֹͣ���ӽӿ�
				dprintf(" recv cmd:  CMD_STOP_MONITOR\n");
				net_send_echo_packet(recPacket, ECHO_OK, NULL, 0);
				g_MonitorInfo.state = MONITOR_END;
				//monitor_stop();
				
			}
			break;
			
		case CMD_MONITOR_HEART:											//�������֣�������
			if (g_MonitorInfo.address == recPacket->address)
			{
				g_MonitorInfo.HeartTime = 0;
			}
			break;
			
		default:
			return FALSE;
	}
	return TRUE;
}

/*************************************************
  Function:			monitor_responsion
  Description:		��������Ӧ���������
  Input: 	
  	1.recPacket		Ӧ���
  	2.SendPacket	���Ͱ�
  Output:			��
  Return:			��
  Others:
*************************************************/
void monitor_responsion(const PRECIVE_PACKET recPacket, const PSEND_PACKET SendPacket)
{
	PNET_HEAD head = (PNET_HEAD)(recPacket->data + MAIN_NET_HEAD_SIZE);
	uint16 cmd = head->command | SSC_INTERPHONE << 8;
	switch (cmd)
	{	
		case CMD_MONITOR:							// ����
			if (g_MonitorInfo.state == MONITOR_REQUEST)
			{
				dprintf("monitor net response : CMD_MONITOR : EchoValue : %d\n", head->EchoValue);
				if (head->EchoValue == ECHO_OK)
				{
					g_MonitorInfo.state = MONITOR_MONITORING;
					g_PreMonitorState = MONITOR_MONITORING;
					GuiNotify(g_MonitorInfo.state, 0);
					usleep(20*1000);
				}
				else if(head->EchoValue == ECHO_BUSY)
				{
					g_ErrType = MONITOR_BUSY;
					g_MonitorInfo.state = MONITOR_END;
				}
				else
				{
					g_ErrType = MONITOR_REQUEST_TIMEOUT;
					g_MonitorInfo.state = MONITOR_END;
				}
			}
			break;
			
		case CMD_MONITOR_TO_TALK:					// ����תͨ��
			dprintf("monitor net response : CMD_MONITOR_TO_TALK : echo:%d, curstate:%d\n", head->EchoValue, g_MonitorInfo.state);
			if (g_MonitorInfo.state == MONITOR_MONITORING)
			{
				if (head->EchoValue == ECHO_OK)
				{
					if (DEVICE_TYPE_DOOR_NET == g_MonitorInfo.DevType)
					{
						media_Ad_set_audio_PackMode(1);		// GM8126��ǰ��Ӧ���ǵ���,ʵ����Ҫ�������ȥ,�����������
					}
					else
					{
						media_Ad_set_audio_PackMode(head->Encrypt);
					}

					#if 0
					hw_switch_digit(); 				// �л������ֶԽ� 				
					if (media_start_net_audio(g_MonitorInfo.address))
					{						
						if (g_MonitorInfo.state == MONITOR_MONITORING)
						{
							dprintf("media_start_net_audio return ok\n ");
							g_MonitorInfo.state = MONITOR_TALKING;
							GuiNotify(g_MonitorInfo.state, 0);
							uint8 volume = storage_get_talkvolume();
							//media_set_talk_volume(volume);
							media_set_talk_volume(g_MonitorInfo.DevType, volume);
							// add by luofl 2011-12-07 ������ͷ��������
							media_set_mic_volume(storage_get_micvolume());
							g_MonitorInfo.RemoteAudioPort = *((uint16 *)(recPacket->data + NET_HEAD_SIZE));
							media_add_audio_sendaddr(g_MonitorInfo.address, g_MonitorInfo.RemoteAudioPort);
						}
						else
						{
							usleep(500*1000);
							media_stop_net_audio();
						}
						
					}	
					else
					{						
						dprintf(" media_start_net_audio return error\n ");
					}
					#else
					g_MonitorInfo.state = MONITOR_TALKING;
					g_MonitorInfo.address = recPacket->address;
					g_MonitorInfo.RemoteAudioPort = *((uint16 *)(recPacket->data + NET_HEAD_SIZE));
					#endif
				}
				else if (head->EchoValue == ECHO_BUSY)
				{
					GuiNotify(g_MonitorInfo.state, MONITOR_BUSY);
				}
			}
			break;
			
		case CMD_STOP_MONITOR:						// �жϼ���
			break;
			
		default:
			break;
	}
}

#if MONITOR_FAX_IP
/*************************************************
  Function:			monitor_request
  Description:		��������
  Input: 	
   	1.ID			Ӧ��ID
  	2.DevType		�豸����
  Output:			��
  Return:			�ɹ����, TRUE / FALSE
  Others:
*************************************************/
int32 test_monitor_start(void)
{
	//dprintf("test_monitor_start 11111111111111111111\n ");
	int32 ret = sys_set_monitor_state(TRUE);
	if (ret != 0)
	{
		dprintf("monitor search dev : sys is busy : state:%d\n", ret);
		return FALSE;
	}
	
	g_MonitorInfo.DevType = DEVICE_TYPE_STAIR;
	g_MonitorInfo.address = 0XA6E1090;      // ip 10.110.16.176
	g_MonitorInfo.index = 0;

	g_MonitorInfo.state = MONITOR_REQUEST;
	if (0 != inter_start_thread(&g_MonitorInfo.mThread, monitor_proc, (void*)&g_MonitorInfo, 1))
	{
		g_MonitorInfo.state = MONITOR_END;
		return false;
	}
	return TRUE;
}
#endif

