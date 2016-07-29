/*************************************************
  Copyright (C), 2006-2010, Aurine
  File name: 	loigc_rtsp_monitor.c
  Author:    	
  Version:   	1.0
  Date: 
  Description:  RTSP����
				
  History:        
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#include "logic_include.h"


#define DEFAULT_VIDEO_FMT						H264			// ������ʽ
#define DEFAULT_VIDEO_IMGSIZE					CIF				// ��Ƶ�ֱ���
#define DEFAULT_VIDEO_FRAMERATE					15				// ֡��
#define DEFAULT_VIDEO_BITRATE					512				// ����

#define DEFAULT_SELFIPC_VIDEO_FMT				H264
#define DEFAULT_SELFIPC_VIDEO_IMGSIZE			VGA
#define DEFAULT_SELFIPC_VIDEO_FRAMERATE			15
#define DEFAULT_SELFIPC_VIDEO_BITRATE			768//512

#define DEFAULT_HIKVISION_VIDEO_FMT				H264
#define DEFAULT_HIKVISION_VIDEO_IMGSIZE			CIF
#define DEFAULT_HIKVISION_VIDEO_FRAMERATE		15
#define DEFAULT_HIKVISION_VIDEO_BITRATE			512

#define MONITOR_TIME_MAX						60				// �����ʱ��
#define MONITOR_REQUEST_TIME					5				// ����ʱ��10��
#define MONITOR_HEART_TIME						5				// 5��һ������
#define MONIROT_HEART_MAX						3				// 3������

typedef struct
{
	int32 index;									// �����豸ID
	int32 type;
	MONITOR_STATE_E state;							// ��ǰ����״̬
	uint16 TimeMax;									// ��ʱʱ��,�����ʱ��, ��, 0Ϊ���޳�
	uint16 TimeOut;									// ��ʱ,��ʱ�ж�
	uint16 HeartTime;								// ������ʱ
	uint16 SendCmd;									// �Ƿ������� 0 ����Ҫ�� 1 ���� 
	uint16 Recv;
	uint16 SPS;							
	PFGuiNotify gui_notify;							// GUI ״̬֪ͨ����
	struct ThreadInfo mThread;						// �߳�
}RTSP_MONITOR_INFO;

static uint32 g_ErrType;
static struct ThreadInfo g_mThread;					// ��ȡ�б��߳�
static RTSP_MONITOR_INFO g_RtspMonItorInfo;
static PRtspDeviceList g_RtspMonItoRDevList = NULL;
static MONITOR_STATE_E g_PreRtspMonitorState = MONITOR_END;   	

#define RtspGuiNotify(param1, param2)	if (g_RtspMonItorInfo.gui_notify)\
	g_RtspMonItorInfo.gui_notify(param1, param2)

#ifdef _NEW_SELF_IPC_
#define IPC_HEART_TIME			90					// ������ʱʱ��s

typedef struct
{
	SelfIPCamera IPList;
	uint32 TimeOut;
}SelfIPCameraList, *PSelfIPCameraList;

static SelfIPCameraList g_ipc_list[MAX_HOME_NUM];	// IPC�ѱ����б�
static PNewMonitorDeviceList g_CommDevlist = NULL;	// �����б�
static uint32 g_MainDevIP = 0;						// ��õ�����IP
#endif

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
  Function:		rtsp_monitor_state
  Description: 	
  Input: 		��
  Output:		��
  Return:		
  Others:		
*************************************************/
uint32 rtsp_monitor_state (void)
{
	if (MONITOR_END != g_RtspMonItorInfo.state)
	{
		return TRUE;
	}

	return FALSE;
}

/*************************************************
  Function:		rtsp_monitor_sync_devlist
  Description: 	��ȡRTSP�豸�б�
  Input: 		��
  Output:		��
  Return:		
  Others:		
*************************************************/
static void* rtsp_get_monitor_sync_devlist (void *param)
{
	pthread_detach(pthread_self());
	int i;
    int ret = 0;
    char RecData[10000] = {0};
	int ReciSize, ComDevNum;
	uint8 EchoValue;
    char data[10] = {0};
	uint32 rtsp_ip = storage_get_netparam_bytype(RTSP_IPADDR); 
	PFULL_DEVICE_NO dev = storage_get_devparam();
	PMonitorDeviceList g_list;
	
    memcpy(data, &dev->AreaNo, 4);
	ReciSize = 0;
	EchoValue = ECHO_ERROR;

	// add by chenbh 2016-03-09 �����豸ʱ ��ԭ���б��������
	storage_free_monitordev(COMMUNITY_CAMERA);
	
	// modi by chenbh 2016-06-23 ���û������rtsp ip��ֱ�Ӵ����ķ�������ȡ�豸�б�
	#if 1
	if (0 == rtsp_ip)
	{
		rtsp_ip = storage_get_center_ip();
	}	
	#else
	// add by chenbh 2015-09-28 ����¿���λ�� ��Ҫ�ȴӷ�������ȡ���ŷ��������ٴ������������ȡ�����б�
	if (0 == rtsp_ip)
	{
		// add by chenbh 2015-10-09 ��ȡ����RTSP������
		uint32 SendData = 0x01;  // ��ȡRTSP ������
		uint32 CenterIP = storage_get_center_ip();
		set_nethead(G_CENTER_DEVNO, PRIRY_DEFAULT);
		ret = net_send_command(CMD_GET_SUIT_SERVER, &SendData, 4, CenterIP, NETCMD_UDP_PORT, 2, &EchoValue, RecData, &ReciSize);
		if (TRUE == ret)
		{
			if (EchoValue == ECHO_OK)
			{
				memcpy(&g_Rtsp_ServerInfo.deviceno, (PDEVICE_NO)RecData, sizeof(DEVICE_NO));
				g_Rtsp_ServerInfo.ip = *(uint32 *)(RecData+12);
				g_Rtsp_ServerInfo.comm_port = *(uint32 *)(RecData+16);
				g_Rtsp_ServerInfo.rtsp_port = *(uint32 *)(RecData+20);					
			}
		}
		else
		{
			memset(&g_Rtsp_ServerInfo, 0, sizeof(RTSP_SERVER_INFO));
		}

		dprintf("g_Rtsp_ServerInfo.ip :%x, g_Rtsp_ServerInfo.rtsp_port : %d\n", g_Rtsp_ServerInfo.ip, g_Rtsp_ServerInfo.rtsp_port);
		rtsp_ip = g_Rtsp_ServerInfo.ip;
	}
	#endif

	memset(RecData, 0, sizeof(RecData));

	// add by chenbh 2016-03-09 SELF_IPC�ж���һ������״̬
	#ifdef _NEW_SELF_IPC_ 
	storage_free_monitordev_memory(&g_CommDevlist);		// �������ʱ�洢������
	ret = net_send_command(CMD_GET_AREA_CAMERALIST_EXT, data, 4, rtsp_ip, NETCMD_UDP_PORT, 3, &EchoValue, RecData, &ReciSize);		
	if (TRUE == ret)
	{
		if (EchoValue == ECHO_OK && ReciSize > 4)
		{
			ComDevNum = (ReciSize-4)/sizeof(NEWCOMMUNITYDEVICE);
			if (ComDevNum == 0)
			{
				storage_malloc_monitordev_memory(0, ComDevNum, &g_list);
				if (ECHO_STORAGE_OK ==  storage_save_monitordev(COMMUNITY_CAMERA, g_list))
				{
					ret = TRUE;
				}
			}
			else
			{
				if ((ReciSize-4) == ComDevNum*sizeof(NEWCOMMUNITYDEVICE))
				{
					storage_malloc_monitordev_memory(0, ComDevNum, &g_list);
					if(NULL == g_list || NULL == g_list->Comdev)
					{
						ret = FALSE;
						goto err;
					}

					// add by chenbh 2016-03-09 �����ݴ浽��ʱ�� UI��ȡ����״̬ʱ��
					storage_free_monitordev_memory(&g_CommDevlist);		
					storage_malloc_new_monitordev_memory(0, ComDevNum, &g_CommDevlist);
					if(NULL != g_CommDevlist && NULL != g_CommDevlist->Comdev)
					{
						memcpy(g_CommDevlist->Comdev, RecData+4, sizeof(NEWCOMMUNITYDEVICE)*ComDevNum);
						g_CommDevlist->Comnum = ComDevNum;
					}
					
					g_list->Comnum = ComDevNum;
					for (i = 0; i < ComDevNum; i++)
					{
						// �ṹ�廹������ǰ�� �����ǵ�ƫ�Ƴ���Ϊ�½ṹ�峤��
						NEWCOMMUNITYDEVICE* PNewComdev = (NEWCOMMUNITYDEVICE *)(RecData+4+sizeof(NEWCOMMUNITYDEVICE)*i);
						g_list->Comdev[i].DeviceIP = PNewComdev->DeviceIP;
						g_list->Comdev[i].DevPort = PNewComdev->DevPort;
						g_list->Comdev[i].CanControlPTZ = PNewComdev->CanControlPTZ;
						g_list->Comdev[i].ChannelNumber = PNewComdev->ChannelNumber;
						memset(g_list->Comdev[i].DeviceName, 0, sizeof(g_list->Comdev[i].DeviceName));
						memset(g_list->Comdev[i].FactoryName, 0, sizeof(g_list->Comdev[i].FactoryName));
						memcpy(g_list->Comdev[i].DeviceName, PNewComdev->DeviceName, sizeof(g_list->Comdev[i].DeviceName));
						memcpy(g_list->Comdev[i].FactoryName, PNewComdev->FactoryName, sizeof(g_list->Comdev[i].FactoryName));
						
						dprintf("***********************************************\n");
						dprintf("g_index %x\n",i);
						dprintf("g_list->Comdev[i].DeviceName %s \n",g_list->Comdev[i].DeviceName);
						dprintf("g_list->Comdev[i].DeviceIP %x \n",g_list->Comdev[i].DeviceIP);
						dprintf("g_list->Comdev[i].DevPort %x \n",g_list->Comdev[i].DevPort);
						dprintf("g_list->Comdev[i].FactoryName %s \n",g_list->Comdev[i].FactoryName);	
						dprintf("g_list->Comdev[i].isOnLine %d \n",PNewComdev->isOnLine);	
						dprintf("***********************************************\n");
					}
					
					if (ECHO_STORAGE_OK ==  storage_save_monitordev(COMMUNITY_CAMERA, g_list))
					{
						ret = TRUE;
					}					
				}
				else
				{
					ret = FALSE;
				}
			}
		}
        else
        {
            ret	= FALSE;
        }        
	}	
	else
	#endif
	{
		ret = net_send_command(CMD_GET_AREA_CAMERALIST, data, 4, rtsp_ip, NETCMD_UDP_PORT, 3, &EchoValue, RecData, &ReciSize);		
		if (TRUE == ret)
		{
			if (EchoValue == ECHO_OK && ReciSize > 4)
			{
				ComDevNum = (ReciSize-4)/sizeof(COMMUNITYDEVICE);
				if (ComDevNum == 0)
				{
					storage_malloc_monitordev_memory(0, ComDevNum, &g_list);
					if (ECHO_STORAGE_OK ==  storage_save_monitordev(COMMUNITY_CAMERA, g_list))
					{
						ret = TRUE;
					}
				}
				else
				{
					if ((ReciSize-4) == ComDevNum*sizeof(COMMUNITYDEVICE))
					{
						storage_malloc_monitordev_memory(0, ComDevNum, &g_list);
						if(NULL == g_list || NULL == g_list->Comdev)
						{
							ret = FALSE;
							goto err;
						}

						g_list->Comnum = ComDevNum;
						memcpy(g_list->Comdev, RecData+4, sizeof(COMMUNITYDEVICE)*g_list->Comnum);

						for (i = 0; i < ComDevNum; i++)
						{
							dprintf("***********************************************\n");
							dprintf("g_index %x\n",i);
							dprintf("g_list->Comdev[i].DeviceName %s \n",g_list->Comdev[i].DeviceName);
							dprintf("g_list->Comdev[i].DeviceIP %x \n",g_list->Comdev[i].DeviceIP);
							dprintf("g_list->Comdev[i].DevPort %x \n",g_list->Comdev[i].DevPort);
							dprintf("g_list->Comdev[i].FactoryName %s \n",g_list->Comdev[i].FactoryName);	
							dprintf("***********************************************\n");
						}
						
						if (ECHO_STORAGE_OK ==  storage_save_monitordev(COMMUNITY_CAMERA, g_list))
						{
							ret = TRUE;
						}					
					}
					else
					{
						ret = FALSE;
					}
				}
			}
	        else
	        {
	            ret	= FALSE;
	        }        
		}
	}
	
	
	
err:

	if (ret)
	{
		storage_free_monitordev_memory(&g_list);
	}

	if (ret)
	{
		RtspGuiNotify(MONITOR_GETLIST, TRUE);
	}
	else
	{
		RtspGuiNotify(MONITOR_GETLIST, FALSE);
	}
	
	inter_SetThread(&g_mThread);
	pthread_exit(NULL);
	return NULL;
}

/*************************************************
  Function:		recive_rtp_data
  Description: 	
  Input: 		
  	1.data		Ӧ��״̬
  	2.param		����
  Output:		��
  Return:		��
  Others:		
*************************************************/
void recive_rtp_data(void)
{
	if (g_RtspMonItorInfo.Recv)
	{
		//rtsp_recv_steams_loop();
	}
}

/*************************************************
  Function:		rtsp_monitor_callback
  Description: 	RTSP ���ݻص�
  Input: 		
  	1.data		Ӧ��״̬
  	2.param		����
  Output:		��
  Return:		��
  Others:		
*************************************************/
static void rtsp_monitor_callback(RTSP_STATE_E echo, int param)
{	
	switch (echo)
	{
		case RTSP_STATE_NONE:
			break;
			
		case RTSP_STATE_CONNECT:
			break;
		
		case RTSP_STATE_PLAY:
			g_RtspMonItorInfo.state = MONITOR_MONITORING;
			break;
			
		case RTSP_STATE_PAUSE:
			break;			
			
		case RTSP_STATE_STOP:
			{
				switch (param)
				{
					case -1:
						g_ErrType = MONITOR_REQUEST_TIMEOUT;
						break;
						
					default:
						g_ErrType = MONITOR_ERR;
						break;
				}
				g_RtspMonItorInfo.SendCmd = 0;
				g_RtspMonItorInfo.state = MONITOR_END;
			}
			break;

		case RTSP_STATE_HEART:
			g_RtspMonItorInfo.HeartTime = 0;
			break;
		
		default:
			break;
			
	}
}

/*************************************************
  Function:    		get_videofmt
  Description:		��ȡ��Ƶ�����ַ���
  Input:
  	1.ID			��Ƶ��ʽID
  Output:			
  Return:			ת������ַ���ָ��
  Others:			
*************************************************/
static char* get_videofmt(unsigned long ID)
{
	switch (ID)
	{
		case H264:
			return H264_STR;

		case MPEG:
			return MPEG_STR;

		default:
			return NULL;
	}
}

/*************************************************
  Function:    		get_videodec
  Description:		��ȡ��Ƶ��ʽ�ַ���
  Input:
  	1.ID			��Ƶ��ʽID
  Output:			
  Return:			ת������ַ���ָ��
  Others:			
*************************************************/
static char* get_videodec(unsigned long ID)
{
	switch (ID)
	{
		case QCIF:
			return QCIF_STR;

		case QVGA:
			return QVGA_STR;
			
		case CIF:
			return CIF_STR;
			
		case D1:
			return D1_STR;
			
		case HalfD1:
			return HalfD1_STR;

		case VGA:
			return VGA_STR;

		case _720P:
			return _720P_STR;
			
		default:
			return NULL;
	}
}

/*************************************************
  Function:		create_sanli_url_name
  Description: 	����RTSP ת��url
  Input: 		
  	1.ServerIP		������IP
  	2.FactoryName 	��������
  	3.IP			IP��ַ
  	4.Port 			�豸�˿ں�
  	5.ChannelNumber	ͨ����
  	6.outputformat	��Ƶ����	
  	7.resolution	��Ƶ��ʽ
  	8.framerate		֡��
  	9.bitrate		����
  	10.user			�û���
  	11.password		����
  	12.AreaNo		С�����
  	13.DevNo		�豸��
  	12.urlname		URL����
  Output:		��
  Return:		��
  Others:
MOBILELIVECAST_FactoryName_IP_Port_ChannelNumber_OutputFormat_Resolution_Bitrate_Framerate_areano_deviceno.sdp
MOBILELIVECAST_FactoryName_IP_Port_ChannelNumber_OutputFormat_Resolution_Bitrate_Framerate_areano_deviceno.sdp?ACCOUNT@user=admin&password=12345
*************************************************/
void create_sanli_url_name(char* ServerIP, HOMEDEVICE* Devinfo, char* urlname)
{
	char tmp[50];
	char fmt[150];
	char userinfo[50];
	char devinfo[50];
	int fmtlen = 0;
	int userinfolen = 0;
	int devinfolen = 0;
	
	if (urlname && ServerIP)
	{
		memset(fmt,0,sizeof(fmt));
		memset(tmp,0,sizeof(tmp));
		memset(userinfo,0,sizeof(userinfo));		
		if (strlen(Devinfo->Password) > 0 && strlen(Devinfo->UserName) > 0)
		{	
			sprintf(userinfo, "?ACCOUNT@user=%s&password=%s", Devinfo->UserName, Devinfo->Password);
			userinfolen = strlen(userinfo);
		}
		sprintf(urlname,"rtsp://%s/LIVECAST_%s_%s_%d_%d.sdp",
				ServerIP, Devinfo->FactoryName, UlongtoIP(Devinfo->DeviceIP), Devinfo->DevPort, Devinfo->ChannelNumber);
		if (userinfolen > 0)
		{
			strcat(urlname,userinfo);
		}
	}
}

/*************************************************
  Function:		create_url_name
  Description: 	����RTSP ת��url
  Input: 	
  	1.ServerIP		������IP
  	2.FactoryName 	��������
  	3.IP			IP��ַ
  	4.Port 			�豸�˿ں�
  	5.ChannelNumber	ͨ����
  	6.AreaNo		����	
  	7.DevNo			�豸��
  	8.urlname		URL����
  Output:		��
  Return:		��
  Others:
LIVECAST_FactoryName_IP_Port_ChannelNumber.sdp
LIVECAST_FactoryName_IP_Port_ChannelNumber_areano_deviceno.sdp
*************************************************/
void create_url_name(char* ServerIP,char* FactoryName, char* IP, unsigned int Port, char* ChannelNumber, char* AreaNo, char* DevNo, char* urlname)
{
	if (urlname && FactoryName && ServerIP)
	{
		if (NULL != AreaNo && NULL != DevNo)
		{	
			sprintf(urlname,"rtsp://%s/LIVECAST_%s_%s_%d_%s_%s_%s.sdp",ServerIP,FactoryName,IP,Port,ChannelNumber,AreaNo,DevNo);
		}
		else
		{
			sprintf(urlname,"rtsp://%s/LIVECAST_%s_%s_%d_%s.sdp",ServerIP,FactoryName,IP,Port,ChannelNumber);
		}
	}
}

/*************************************************
  Function:		create_mobile_url_name
  Description: 	����RTSP ת��url
  Input: 		
  	1.ServerIP		������IP
  	2.FactoryName 	��������
  	3.IP			IP��ַ
  	4.Port 			�豸�˿ں�
  	5.ChannelNumber	ͨ����
  	6.outputformat	��Ƶ����	
  	7.resolution	��Ƶ��ʽ
  	8.framerate		֡��
  	9.bitrate		����
  	10.user			�û���
  	11.password		����
  	12.AreaNo		С�����
  	13.DevNo		�豸��
  	12.urlname		URL����
  Output:		��
  Return:		��
  Others:
MOBILELIVECAST_FactoryName_IP_Port_ChannelNumber_OutputFormat_Resolution_Bitrat
e_Framerate_areano_deviceno.sdp
MOBILELIVECAST_FactoryName_IP_Port_ChannelNumber_OutputFormat_Resolution_Bitrat
e_Framerate_areano_deviceno.sdp?ACCOUNT@user=admin&password=12345
*************************************************/
void create_mobile_url_name(char* ServerIP, char* ServerPort, HOMEDEVICE* Devinfo, char* AreaNo, char* DevNo, char* urlname)
{
	char tmp[50];
	char fmt[150];
	char userinfo[50];
	char devinfo[50];
	int fmtlen = 0;
	int userinfolen = 0;
	int devinfolen = 0;

	if (urlname && ServerIP)
	{
		memset(fmt, 0, sizeof(fmt));
		memset(tmp, 0, sizeof(tmp));
		memset(userinfo, 0, sizeof(userinfo));		

		sprintf(fmt, "%s", get_videofmt(DEFAULT_VIDEO_FMT));
		fmtlen = strlen(fmt);

		sprintf(tmp, "_%s", get_videodec(DEFAULT_VIDEO_IMGSIZE));
		strcat(fmt, tmp);
		fmtlen += strlen(tmp);

		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "_%d", DEFAULT_VIDEO_BITRATE);
		strcat(fmt, tmp);
		fmtlen += strlen(tmp);
		
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "_%d", DEFAULT_VIDEO_FRAMERATE);
		strcat(fmt, tmp);
		fmtlen += strlen(tmp);

		if (NULL != AreaNo && NULL != DevNo)
		{	
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s_%s", AreaNo, DevNo);
			strcat(fmt, tmp);
			fmtlen += strlen(tmp);
		}

		if (strlen(Devinfo->Password) > 0 && strlen(Devinfo->UserName) > 0)
		{	
			sprintf(userinfo, "?ACCOUNT@user=%s&password=%s", Devinfo->UserName, Devinfo->Password);
			userinfolen = strlen(userinfo);
		}

		sprintf(urlname,"rtsp://%s%s/MOBILELIVECAST_%s_%s_%d_%d_%s.sdp",
				ServerIP, ServerPort, Devinfo->FactoryName, UlongtoIP(Devinfo->DeviceIP), Devinfo->DevPort, Devinfo->ChannelNumber, fmt);
		
		if (userinfolen > 0)
		{
			strcat(urlname,userinfo);
		}
	}
}

/*************************************************
  Function:		create_selfipc_url_name
  Description: 	����RTSP ת��url
  Input: 		
  	1.ServerIP		������IP
  	2.FactoryName 	��������
  	3.IP			IP��ַ
  	4.Port 			�豸�˿ں�
  	5.ChannelNumber	ͨ����
  	6.outputformat	��Ƶ����	
  	7.resolution	��Ƶ��ʽ
  	8.framerate		֡��
  	9.bitrate		����
  	10.user			�û���
  	11.password		����
  	12.AreaNo		С�����
  	13.DevNo		�豸��
  	12.urlname		URL����
  Output:		��
  Return:		��
  Others:
MOBILELIVECAST_FactoryName_IP_Port_ChannelNumber_OutputFormat_Resolution_Bitrat
e_Framerate_areano_deviceno.sdp
MOBILELIVECAST_FactoryName_IP_Port_ChannelNumber_OutputFormat_Resolution_Bitrat
e_Framerate_areano_deviceno.sdp?ACCOUNT@user=admin&password=12345
*************************************************/
void create_selfipc_url_name(char* ServerIP, char* ServerPort, HOMEDEVICE* Devinfo, char* AreaNo, char* DevNo, char* urlname)
{
	char tmp[50];
	char fmt[150];
	char userinfo[50];
	char devinfo[50];
	int fmtlen = 0;
	int userinfolen = 0;
	int devinfolen = 0;

	if (urlname && ServerIP)
	{
		memset(fmt, 0, sizeof(fmt));
		memset(tmp, 0, sizeof(tmp));
		memset(userinfo, 0, sizeof(userinfo));		

		sprintf(fmt, "%s", get_videofmt(DEFAULT_SELFIPC_VIDEO_FMT));
		fmtlen = strlen(fmt);

		sprintf(tmp, "_%s", get_videodec(DEFAULT_SELFIPC_VIDEO_IMGSIZE));
		strcat(fmt, tmp);
		fmtlen += strlen(tmp);

		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "_%d", DEFAULT_SELFIPC_VIDEO_BITRATE);
		strcat(fmt, tmp);
		fmtlen += strlen(tmp);
		
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "_%d", DEFAULT_SELFIPC_VIDEO_FRAMERATE);
		strcat(fmt, tmp);
		fmtlen += strlen(tmp);

		if (NULL != AreaNo && NULL != DevNo)
		{	
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s_%s", AreaNo, DevNo);
			strcat(fmt, tmp);
			fmtlen += strlen(tmp);
		}

		if (strlen(Devinfo->Password) > 0 && strlen(Devinfo->UserName) > 0)
		{	
			sprintf(userinfo, "?ACCOUNT@user=%s&password=%s", Devinfo->UserName, Devinfo->Password);
			userinfolen = strlen(userinfo);
		}

		sprintf(urlname,"rtsp://%s%s/MOBILELIVECAST_%s_%s_%d_%d_%s.sdp",
				ServerIP, ServerPort, Devinfo->FactoryName, UlongtoIP(Devinfo->DeviceIP), Devinfo->DevPort, Devinfo->ChannelNumber, fmt);
		
		if (userinfolen > 0)
		{
			strcat(urlname,userinfo);
		}
	}
}

/*************************************************
  Function:		create_hikvision_url_name
  Description: 	����RTSP ת��url
  Input: 		
  	1.ServerIP		������IP
  	2.FactoryName 	��������
  	3.IP			IP��ַ
  	4.Port 			�豸�˿ں�
  	5.ChannelNumber	ͨ����
  	6.outputformat	��Ƶ����	
  	7.resolution	��Ƶ��ʽ
  	8.framerate		֡��
  	9.bitrate		����
  	10.user			�û���
  	11.password		����
  	12.AreaNo		С�����
  	13.DevNo		�豸��
  	12.urlname		URL����
  Output:		��
  Return:		��
  Others:
MOBILELIVECAST_FactoryName_IP_Port_ChannelNumber_OutputFormat_Resolution_Bitrat
e_Framerate_areano_deviceno.sdp
MOBILELIVECAST_FactoryName_IP_Port_ChannelNumber_OutputFormat_Resolution_Bitrat
e_Framerate_areano_deviceno.sdp?ACCOUNT@user=admin&password=12345
*************************************************/
void create_hikvision_url_name(char* ServerIP, char* ServerPort, HOMEDEVICE* Devinfo, char* AreaNo, char* DevNo, char* urlname)
{
	char tmp[50];
	char fmt[150];
	char userinfo[50];
	char devinfo[50];
	int fmtlen = 0;
	int userinfolen = 0;
	int devinfolen = 0;

	if (urlname && ServerIP)
	{
		memset(fmt, 0, sizeof(fmt));
		memset(tmp, 0, sizeof(tmp));
		memset(userinfo, 0, sizeof(userinfo));		

		sprintf(fmt, "%s", get_videofmt(DEFAULT_HIKVISION_VIDEO_FMT));
		fmtlen = strlen(fmt);

		sprintf(tmp, "_%s", get_videodec(DEFAULT_HIKVISION_VIDEO_IMGSIZE));
		strcat(fmt, tmp);
		fmtlen += strlen(tmp);

		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "_%d", DEFAULT_HIKVISION_VIDEO_BITRATE);
		strcat(fmt, tmp);
		fmtlen += strlen(tmp);
		
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "_%d", DEFAULT_HIKVISION_VIDEO_FRAMERATE);
		strcat(fmt, tmp);
		fmtlen += strlen(tmp);

		if (NULL != AreaNo && NULL != DevNo)
		{	
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s_%s", AreaNo, DevNo);
			strcat(fmt, tmp);
			fmtlen += strlen(tmp);
		}

		if (strlen(Devinfo->Password) > 0 && strlen(Devinfo->UserName) > 0)
		{	
			sprintf(userinfo, "?ACCOUNT@user=%s&password=%s", Devinfo->UserName, Devinfo->Password);
			userinfolen = strlen(userinfo);
		}

		sprintf(urlname,"rtsp://%s%s/MOBILELIVECAST_%s_%s_%d_%d_%s.sdp",
				ServerIP, ServerPort, Devinfo->FactoryName, UlongtoIP(Devinfo->DeviceIP), Devinfo->DevPort, Devinfo->ChannelNumber, fmt);
		
		if (userinfolen > 0)
		{
			strcat(urlname,userinfo);
		}
	}
}

/*************************************************
  Function:		    rtsp_monitor_connect
  Description: 	    RTSP ����
  Input: 		
  	1.Index	    
  Output:		��
  Return:		TRUE
  Others:		
*************************************************/
int32 rtsp_monitor_connect(void)
{
	char URL[300] = {0};                            // ע��˿ռ����⣬����������� 
	int i,ret = 0;
	char serverIPstr[100];
	char serverportstr[20];
	uint32 ServerIP,HostIP;
	uint32 Serverprot = 554;
	uint32 video_img = DEFAULT_VIDEO_IMGSIZE;
	V_RECT_S rect;
	
	if (NULL == g_RtspMonItoRDevList || g_RtspMonItoRDevList->Devinfo == NULL)
	{	
		return FALSE;	
	}

	dprintf("g_RtspMonItorInfo.index %d ---- %d\n",g_RtspMonItorInfo.index,g_RtspMonItoRDevList->DevNum);
	
	if (MONITOR_REQUEST == g_RtspMonItorInfo.state)
	{
		ServerIP = storage_get_netparam_bytype(RTSP_IPADDR);
		HostIP = storage_get_netparam_bytype(HOST_IPADDR);

		ServerIP = ((ServerIP == 0)? g_Rtsp_ServerInfo.ip: ServerIP);
		if (ServerIP == 0)
		{
			// add by chenbh 2015-03-31 ��ȡ����RTSP������
			int32  ReciSize;
			uint8 EchoValue;
			char RecData[100] = {0};
			uint32 SendData = 0x01;  // ��ȡRTSP ������
			uint32 CenterIP = storage_get_center_ip();
			set_nethead(G_CENTER_DEVNO, PRIRY_DEFAULT);
			ret = net_send_command(CMD_GET_SUIT_SERVER, (char *)&SendData, 4, CenterIP, NETCMD_UDP_PORT, 2, &EchoValue, RecData, &ReciSize);
			dprintf("CMD_GET_SUIT_SERVER ret : %d. EchoValue: %d\n", ret, EchoValue);
			if (TRUE == ret)
			{
				if (EchoValue == ECHO_OK)
				{
					memcpy(&g_Rtsp_ServerInfo.deviceno, (PDEVICE_NO)RecData, sizeof(DEVICE_NO));
					g_Rtsp_ServerInfo.ip = *(uint32 *)(RecData+12);
					g_Rtsp_ServerInfo.comm_port = *(uint32 *)(RecData+16);
					g_Rtsp_ServerInfo.rtsp_port = *(uint32 *)(RecData+20);					
				}
			}
			else
			{
				memset(&g_Rtsp_ServerInfo, 0, sizeof(RTSP_SERVER_INFO));
			}

			dprintf("g_Rtsp_ServerInfo.ip :%x, g_Rtsp_ServerInfo.rtsp_port : %d\n", g_Rtsp_ServerInfo.ip, g_Rtsp_ServerInfo.rtsp_port);
			dprintf("g_Rtsp_ServerInfo.comm_port :%d\n",g_Rtsp_ServerInfo.comm_port);
			ServerIP = g_Rtsp_ServerInfo.ip;
			Serverprot = g_Rtsp_ServerInfo.comm_port;
		}

		dprintf(" rtsp_monitor_connect ServerIP : %x\n", ServerIP);
		if (ServerIP == 0)
		{
			return FALSE;
		}
		memset(serverportstr,0,sizeof(serverportstr));
		memset(serverIPstr,0,sizeof(serverIPstr));
		sprintf(serverIPstr, "%s",UlongtoIP(ServerIP));

		// modi by chenbh 2016-04-06 ��������libnemesi��������� ��Ҫ��':'��Ϊ'#'
		//sprintf(serverportstr, "%c%d", ':', Serverprot);
		sprintf(serverportstr, "%c%d", '#', Serverprot);				

		dprintf("serverportstr : %s\n",serverportstr);
		#if 1
		dprintf("***********************************************\n");
		dprintf("g_index %x\n",g_RtspMonItorInfo.index);
		dprintf("g_list->Comdev[i].DeviceName %s \n",g_RtspMonItoRDevList->Devinfo[g_RtspMonItorInfo.index].DeviceName);
		dprintf("g_list->Comdev[i].DeviceIP %x \n",g_RtspMonItoRDevList->Devinfo[g_RtspMonItorInfo.index].DeviceIP);
		dprintf("g_list->Comdev[i].DevPort %x \n",g_RtspMonItoRDevList->Devinfo[g_RtspMonItorInfo.index].DevPort);
		dprintf("g_list->Comdev[i].FactoryName %s \n",g_RtspMonItoRDevList->Devinfo[g_RtspMonItorInfo.index].FactoryName);	
		dprintf("g_list->Comdev[i].UserName %s \n",g_RtspMonItoRDevList->Devinfo[g_RtspMonItorInfo.index].UserName);	
		dprintf("g_list->Comdev[i].Password %s \n",g_RtspMonItoRDevList->Devinfo[g_RtspMonItorInfo.index].Password);	
		dprintf("***********************************************\n");
		#endif

		dprintf("g_RtspMonItoRDevList->Devinfo[g_RtspMonItorInfo.index].FactoryName : %s\n",g_RtspMonItoRDevList->Devinfo[g_RtspMonItorInfo.index].FactoryName);
	    if (0 == strcmp(g_RtspMonItoRDevList->Devinfo[g_RtspMonItorInfo.index].FactoryName, "SANLI") || 
			 0 == strcmp(g_RtspMonItoRDevList->Devinfo[g_RtspMonItorInfo.index].FactoryName, "Sanli"))
		{			
			create_sanli_url_name(serverIPstr, &g_RtspMonItoRDevList->Devinfo[g_RtspMonItorInfo.index], URL);
			video_img = DEFAULT_VIDEO_IMGSIZE;
        }
		else if (0 == strcmp(g_RtspMonItoRDevList->Devinfo[g_RtspMonItorInfo.index].FactoryName, "SELFIPC") ||
				0 == strcmp(g_RtspMonItoRDevList->Devinfo[g_RtspMonItorInfo.index].FactoryName, "SelfIPC"))
		{
			create_selfipc_url_name(serverIPstr, serverportstr, &g_RtspMonItoRDevList->Devinfo[g_RtspMonItorInfo.index], NULL, NULL, URL);
			video_img = DEFAULT_SELFIPC_VIDEO_IMGSIZE;
		}		
		else if (0 == strcmp(g_RtspMonItoRDevList->Devinfo[g_RtspMonItorInfo.index].FactoryName, "HIKVISION")|| 
			 0 == strcmp(g_RtspMonItoRDevList->Devinfo[g_RtspMonItorInfo.index].FactoryName, "hikvision"))
		{
			  create_hikvision_url_name(serverIPstr, serverportstr, &g_RtspMonItoRDevList->Devinfo[g_RtspMonItorInfo.index], NULL, NULL, URL);
			  video_img = DEFAULT_HIKVISION_VIDEO_IMGSIZE;
		}
        else
        {
        	create_mobile_url_name(serverIPstr, serverportstr, &g_RtspMonItoRDevList->Devinfo[g_RtspMonItorInfo.index], NULL, NULL, URL);
			video_img = DEFAULT_VIDEO_IMGSIZE;
        }
		
        dprintf("URL = %s\n",URL);

		switch (video_img)
		{
			case QVGA:
				rect.width = 320;
				rect.height = 240;
				break;
				
			case CIF:
				rect.width = 352;
				rect.height = 288;
				break;

			case VGA:
				rect.width = 640;
				rect.height = 480;
				break;

			default:
				rect.width = 640;
				rect.height = 480;
				break;
		}
		
		// ����rtsp ��������
		ret = media_start_rtsp(rect, URL, rtsp_monitor_callback);
        if (-1 == ret)       
		{
			return FALSE;
		}	
		
        return TRUE;
	}
	else
	{
	    dprintf("monitor_rtsp_connect_get_monitor_state = %d\n",g_RtspMonItorInfo.state);
	}
	return FALSE;
}

/*************************************************
  Function:			rtsp_monitor_proc
  Description: 		�����߳�
  Input: 			
  	1.param			����
  Output:			��
  Return:			BOOLEAN
  Others:
*************************************************/
static void* rtsp_monitor_proc(void *param)
{
	// ���÷����߳�
	pthread_detach(pthread_self());
	int16 times = 0;

LabChange:	
	if (MONITOR_REQUEST == g_RtspMonItorInfo.state)
	{
		RtspGuiNotify(g_RtspMonItorInfo.state, 0);
		g_PreRtspMonitorState = g_RtspMonItorInfo.state;
		if(FALSE == rtsp_monitor_connect())
		{
			dprintf("rtsp_monitor_connect return fail!!!! \n");
			g_ErrType = MONITOR_SEARCH_ERR;
			g_RtspMonItorInfo.SendCmd = 0;
			g_RtspMonItorInfo.state = MONITOR_END;
		}
		else
		{
			dprintf("rtsp_monitor_connect return success!!!! \n");
			g_RtspMonItorInfo.Recv = 1;	
		}
		
		g_RtspMonItorInfo.HeartTime = 0;
		g_RtspMonItorInfo.TimeOut = 0;
		dprintf("g_RtspMonItorInfo.state : %d\n", g_RtspMonItorInfo.state);
		while (MONITOR_REQUEST == g_RtspMonItorInfo.state)
		{
			g_RtspMonItorInfo.TimeOut++;

			if (g_RtspMonItorInfo.TimeOut >= MONITOR_REQUEST_TIME)
			{
				g_ErrType = MONITOR_REQUEST_TIMEOUT;
				g_RtspMonItorInfo.SendCmd = 1;
				g_RtspMonItorInfo.state = MONITOR_END;
				dprintf(" g_RtspMonItorInfo.TimeOut >= MONITOR_REQUEST_TIME \n");
				break;				
			}			
			
			times = 50;
			while ((times--) > 0 && MONITOR_REQUEST == g_RtspMonItorInfo.state)
			{
				usleep(20*1000);
			}
		}
	}
	
	if (MONITOR_MONITORING == g_RtspMonItorInfo.state)
	{
		RtspGuiNotify(g_RtspMonItorInfo.state, 0);
		g_PreRtspMonitorState = g_RtspMonItorInfo.state;
		
		g_RtspMonItorInfo.HeartTime = 0;
		g_RtspMonItorInfo.TimeOut = 0;
		g_RtspMonItorInfo.TimeMax = MONITOR_TIME_MAX;
		while (MONITOR_MONITORING == g_RtspMonItorInfo.state)
		{
			g_RtspMonItorInfo.TimeOut++;

			if (g_RtspMonItorInfo.TimeOut >= g_RtspMonItorInfo.TimeMax)
			{
				dprintf("txl TimeOut out %d\n",g_RtspMonItorInfo.TimeOut);					
				if (g_RtspMonItorInfo.TimeOut > (g_RtspMonItorInfo.TimeMax+3))
				{
					dprintf("txl TimeOut oper\n");	
					g_RtspMonItorInfo.state = MONITOR_END;
					break;	
				}
				else
				{
					g_ErrType = MONITOR_OK;
					g_RtspMonItorInfo.Recv = 0;
					g_RtspMonItorInfo.SendCmd = 1;
					g_RtspMonItorInfo.state = MONITOR_END;
					break;
				}
			}
			else
			{
				if (g_RtspMonItorInfo.TimeOut % MONITOR_HEART_TIME == 0)
				{
					g_RtspMonItorInfo.HeartTime++;
					// ����������
					if (NetworkIsReady())
					{
						//rtsp_send_heart(rtsp_monitor_callback);
					}
					else
					{
						dprintf("AS_RTSP_HEART net err\n");
					}
				}
			}
			
			times = 50;
			while ((times--) > 0 && MONITOR_MONITORING == g_RtspMonItorInfo.state)
			{
				usleep(20*1000);
			}
		}
	}

	// ������һ����� �ûص���������
	if (MONITOR_REQUEST == g_RtspMonItorInfo.state)
	{
		media_stop_rtsp();
		goto LabChange;
	}

	g_RtspMonItorInfo.Recv = 0;	
	// ����ֹͣ��������
	if (g_RtspMonItorInfo.SendCmd)
	{
		g_RtspMonItorInfo.SendCmd = 0;
	}

	media_stop_rtsp();
	
	g_RtspMonItorInfo.Recv = 0;
	g_RtspMonItorInfo.state = MONITOR_END;
	RtspGuiNotify(g_RtspMonItorInfo.state, g_ErrType);
	
	dprintf("monitor proc : AS_MONITOR_END : g_ErrType : %d\n", g_ErrType);
	g_RtspMonItorInfo.state = MONITOR_END;
	g_ErrType = MONITOR_OK;
	sys_set_monitor_state(FALSE);
	dprintf("monitor proc end!\n");	
	inter_SetThread(&g_RtspMonItorInfo.mThread);
	pthread_exit(NULL);
	return NULL;             	// ���غ���Դ��ϵͳ�ͷ�
}

/*************************************************
  Function:		rtsp_monitor_sync_devlist
  Description: 	��ȡRTSP�豸�б�
  Input: 		��
  Output:		��
  Return:		PMonitorDeviceList �豸�б�
  Others:		
*************************************************/
uint32 rtsp_monitor_sync_devlist (PFGuiNotify GuiProc)
{
	memset(&g_mThread, 0, sizeof(struct ThreadInfo));
	if (0 != inter_start_thread(&g_mThread, rtsp_get_monitor_sync_devlist, NULL, 0))
	{
		RtspGuiNotify(MONITOR_GETLIST, FALSE);
		return FALSE;
	}
	return TRUE;
}

/*************************************************
  Function:		rtsp_monitor_get_devlist
  Description: 	��ȡRTSP�豸�б�
  Input: 		
  	flg			0 �������� 1 �ҾӼ���
  Output:		��
  Return:		PMonitorDeviceList �豸�б�
  Others:		
*************************************************/
PRtspDeviceList rtsp_monitor_get_devlist (uint8 flg)
{
	PMonitorDeviceList g_list;
	int max = 0;
	int i,index = 0;
	
	if (g_RtspMonItoRDevList)
	{
		if (g_RtspMonItoRDevList->Devinfo)
		{
			free(g_RtspMonItoRDevList->Devinfo);
			g_RtspMonItoRDevList->Devinfo = NULL;
		}
		free(g_RtspMonItoRDevList);
		g_RtspMonItoRDevList = NULL;
	}

	g_RtspMonItoRDevList = (PRtspDeviceList)malloc(sizeof(RtspDeviceList)+1);
	if (NULL == g_RtspMonItoRDevList)
	{
		return NULL;
	}
	
	storage_get_monitordev_used(&g_list);

	// ��������
	if (0 == flg)
	{
		max = g_list->Comnum;
		g_list->Homenum = 0;
	}
	else
	{
		max = g_list->Homenum;
		g_list->Comnum = 0;
	}

	g_RtspMonItoRDevList->Devinfo = (PHOMEDEVICE)malloc(sizeof(HOMEDEVICE)*max+1);
	if (NULL == g_RtspMonItoRDevList->Devinfo)
	{
		g_RtspMonItoRDevList->DevNum = 0;
		goto exit;
	}
	g_RtspMonItoRDevList->DevNum = 0;

	for (i = 0; i < g_list->Comnum; i++)
	{
		//if (0 == strcmp(g_list->Comdev[i].FactoryName, "SANLI") || 0 == strcmp(g_list->Comdev[i].FactoryName,"Sanli"))
		{
			sprintf(g_RtspMonItoRDevList->Devinfo[index].DeviceName, "%s", g_list->Comdev[i].DeviceName);
			sprintf(g_RtspMonItoRDevList->Devinfo[index].FactoryName, "%s", g_list->Comdev[i].FactoryName);
			g_RtspMonItoRDevList->Devinfo[index].DeviceIP = g_list->Comdev[i].DeviceIP;
			g_RtspMonItoRDevList->Devinfo[index].DevPort = g_list->Comdev[i].DevPort;
			g_RtspMonItoRDevList->Devinfo[index].ChannelNumber = g_list->Comdev[i].ChannelNumber;
			memset(g_RtspMonItoRDevList->Devinfo[index].UserName, 0, sizeof(g_RtspMonItoRDevList->Devinfo[index].UserName));
			memset(g_RtspMonItoRDevList->Devinfo[index].Password, 0, sizeof(g_RtspMonItoRDevList->Devinfo[index].Password));
			index++;
		}
	}

	for (i = 0; i < g_list->Homenum; i++)
	{
		memcpy(&g_RtspMonItoRDevList->Devinfo[index], &g_list->Homedev[i], sizeof(HOMEDEVICE));
		index++;
	}

	g_RtspMonItoRDevList->DevNum = index;
	
exit:	
	storage_free_monitordev_memory(&g_list);
	return g_RtspMonItoRDevList;
}

/*************************************************
  Function:			monitor_start
  Description:		��ʼ����
  Input: 	
  	1.ID			Ӧ��ID
  	2.DevType		�豸����
  	3.index			�豸����
  Output:			��
  Return:			�ɹ����, TRUE / FALSE
  Others:
*************************************************/
int32 rtsp_monitor_start(uint32 index, uint32 type, PFGuiNotify GuiProc)
{
	dprintf("rtsp_monitor_start %d\n",g_RtspMonItorInfo.state);

	int32 ret = sys_set_monitor_state(TRUE);
	if (ret != 0)
	{
		return FALSE;
	}	
	
	if (g_RtspMonItorInfo.state != MONITOR_END)
	{
		dprintf("monitor start : busy state: %d\n", g_RtspMonItorInfo.state);
		return FALSE;
	}
	
	if (NULL == g_RtspMonItoRDevList || NULL == g_RtspMonItoRDevList->Devinfo || g_RtspMonItoRDevList->DevNum == 0)
	{
		return FALSE;	
	}

	g_RtspMonItorInfo.Recv = 0;
	g_RtspMonItorInfo.index = index;
	g_RtspMonItorInfo.type = type;

	//g_RtspMonItorInfo.state = MONITOR_END;
	g_RtspMonItorInfo.gui_notify = GuiProc;
	//rtsp_init(rtsp_monitor_callback, rtsp_video_callback, NULL); // ���ӵ������л�״̬�ص�
	//rtsp_set_debug(0);
	
	g_RtspMonItorInfo.state = MONITOR_REQUEST;
	if (0 != inter_start_thread(&g_RtspMonItorInfo.mThread, rtsp_monitor_proc, (void*)&g_RtspMonItorInfo, index))
	{
		g_RtspMonItorInfo.state = MONITOR_END;
		return FALSE;
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
int32 rtsp_monitor_stop(void)
{
	dprintf("rtsp_monitor_stop %x\n",g_RtspMonItorInfo.state);	
	if (g_RtspMonItorInfo.state != MONITOR_END)
	{
		g_RtspMonItorInfo.SendCmd = 1;
		g_RtspMonItorInfo.Recv = 0;		
		g_RtspMonItorInfo.state = MONITOR_END;
	}
	return TRUE;
}

/*************************************************
  Function:				rtsp_monitor_list_free
  Description:			
  Input: 	
  	1.GuiProc			GUI�ص�����
  Output:				��
  Return:				
  Others:
*************************************************/
void rtsp_monitor_list_free(void)
{
	if (g_RtspMonItoRDevList)
	{
		if (g_RtspMonItoRDevList->Devinfo)
		{
			free(g_RtspMonItoRDevList->Devinfo);
			g_RtspMonItoRDevList->Devinfo = NULL;
		}
		free(g_RtspMonItoRDevList);
		g_RtspMonItoRDevList = NULL;
	}
}

/*************************************************
  Function:				rtsp_monitor_deini
  Description:			RTSP���ӳ�ʼ��
  Input: 	
  	1.GuiProc			GUI�ص�����
  Output:				��
  Return:				
  Others:
*************************************************/
int rtsp_monitor_deini(void)
{
	return rtsp_monitor_stop();
}

static HOMEDEVICE Homedev[MAX_HOME_NUM+1];
static char gsenddata[1000];
/*************************************************
  Function:			rtsp_distribute
  Description:		
  Input: 	
  Output:			��
  Return:			�ɹ����, TRUE/FALSE
  Others:	
*************************************************/
static void get_homeCamera(const PRECIVE_PACKET recPacket)
{
	uint32 j,i,size,max = 0;
	int len = sizeof(HOMEDEVICE);
	char *info = (char*)Homedev;
	
	size = get_homedev_from_storage(Homedev);
	dprintf("size %d len %d\n",size,len);
	if (0 == size)
	{
		max = 0;
		goto exit;
	}

	j = 0;
	for (i = 0; i < size; i++)
	{
		if (Homedev[i].EnableOpen)
		{
			memcpy(gsenddata+4+(j*len), info+(i*len), len);		
			j++;
			max++;
		}
	}

exit:
	memcpy(gsenddata, &max, 4);
	size = (sizeof(HOMEDEVICE)*(max) + 4);
	net_send_echo_packet(recPacket, ECHO_OK, gsenddata, size);
	dprintf("get_homeCamera 2 %d\n",size);
}

/*************************************************
  Function:			rtsp_distribute
  Description:		
  Input: 	
  Output:			��
  Return:			�ɹ����, TRUE/FALSE
  Others:	
*************************************************/
static void get_homeCamera_ext(const PRECIVE_PACKET recPacket)
{
	uint32 j,i,size,max = 0;
	int len = sizeof(HOMEDEVICE);
	int NewLen = sizeof(NEWHOMEDEVICE);
	char *info = (char*)Homedev;
	
	size = get_homedev_from_storage(Homedev);
	dprintf("size %d len %d\n",size,len);
	if (0 == size)
	{
		max = 0;
		goto exit;
	}

	j = 0;
	memset(gsenddata, 0, sizeof(gsenddata));
	for (i = 0; i < size; i++)
	{
		if (Homedev[i].EnableOpen)
		{
			// ע�� NewLen �� len������
			memcpy(gsenddata+4+(j*NewLen), info+(i*len), len);		
			j++;
			max++;
		}
	}

exit:
	memcpy(gsenddata, &max, 4);
	size = (sizeof(NEWHOMEDEVICE)*(max) + 4);
	net_send_echo_packet(recPacket, ECHO_OK, gsenddata, size);
	dprintf("get_homeCamera 2 %d\n",size);
}

#ifdef _NEW_SELF_IPC_
/*************************************************
  Function:			init_ipc_state
  Description:		
  Input: 	
  Output:			��
  Return:			
  Others:			ipc״̬��ʼ�� Ĭ������״̬
*************************************************/
void init_ipc_state(void)
{
	int i = 0;
	for (i=0; i<MAX_HOME_NUM; i++)
	{
		g_ipc_list[i].TimeOut = 0;
	}

	PMonitorDeviceList CommDevlist = NULL;
	storage_get_comm_monitordev(&CommDevlist);
	storage_free_monitordev_memory(&g_CommDevlist);
	storage_malloc_new_monitordev_memory(0, CommDevlist->Comnum, &g_CommDevlist);
	if(NULL != g_CommDevlist && NULL != g_CommDevlist->Comdev)
	{
		for (i=0; i<(CommDevlist->Comnum); i++)
		{
			memset(&g_CommDevlist->Comdev[i], 0, sizeof(NEWCOMMUNITYDEVICE));
			g_CommDevlist->Comdev[i].DeviceIP = CommDevlist->Comdev[i].DeviceIP;
			g_CommDevlist->Comdev[i].DevPort = CommDevlist->Comdev[i].DevPort;
			g_CommDevlist->Comdev[i].CanControlPTZ = CommDevlist->Comdev[i].CanControlPTZ;
			g_CommDevlist->Comdev[i].ChannelNumber = CommDevlist->Comdev[i].ChannelNumber;
			memcpy(g_CommDevlist->Comdev[i].DeviceName, CommDevlist->Comdev[i].DeviceName, sizeof(CommDevlist->Comdev[i].DeviceName));
			memcpy(g_CommDevlist->Comdev[i].FactoryName, CommDevlist->Comdev[i].FactoryName, sizeof(CommDevlist->Comdev[i].FactoryName));
			g_CommDevlist->Comdev[i].isOnLine = 1;		// Ĭ�϶������ߵ�
		}
	}
}

/*************************************************
  Function:			ipc_ontimer
  Description:		
  Input: 	
  Output:			��
  Return:			
  Others:			ipc״̬��ʱ��
*************************************************/
void ipc_ontimer(void)
{
	int i = 0;
	for (i=0; i<MAX_HOME_NUM; i++)
	{
		if (g_ipc_list[i].TimeOut > 0)
		{
			g_ipc_list[i].TimeOut--;
		}
	}
}

/*************************************************
  Function:			ipc_ontimer
  Description:		
  Input: 	
  	index			�豸������� 0-9
  Output:			��
  Return:			0 ���� 1 ����
  Others:			���IPC����״̬ �����б�����ʾ��
*************************************************/
uint8 get_ipc_state(uint8 index)
{
	if (g_ipc_list[index].TimeOut > 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*************************************************
  Function:			get_commdev_state
  Description:		
  Input: 	
  	IP				�豸IP
  Output:			��
  Return:			0 ���� 1 ����
  Others:			���IPC����״̬ �����б�����ʾ��
*************************************************/
uint8 get_commdev_state(uint32 IP)
{
	int i = 0;
	if (g_CommDevlist == NULL || g_CommDevlist->Comdev == NULL)
	{
		return 0;
	}
	
	for (i=0; i<(g_CommDevlist->Comnum); i++)
	{
		if (g_CommDevlist->Comdev[i].DeviceIP == IP)
		{
			return g_CommDevlist->Comdev[i].isOnLine;
		}
	}
	
	return 0;
}

/*************************************************
  Function:			set_ipc_online
  Description:		
  Input: 	
  	index			�豸������� 0-9
  Output:			��
  Return:			
  Others:			����IPC����״̬
*************************************************/
void set_ipc_online(uint8 index, uint32 IP)
{
	g_ipc_list[index].TimeOut = IPC_HEART_TIME;
	g_ipc_list[index].IPList.address = IP;
	g_ipc_list[index].IPList.index = index;
}

/*************************************************
  Function:			ipc_ontimer
  Description:		
  Input: 	
  	index			�豸������� 0-9
  Output:			��
  Return:			
  Others:			����������IPC�ǵõ����������
*************************************************/
void set_ipc_outline(uint8 index)
{
	g_ipc_list[index].TimeOut = 0;
}

/*************************************************
  Function:			fenji_sync_ipc_list
  Description:		
  Input: 	
  Output:			��
  Return:			
  Others:			�ֻ�������ͬ���б�
*************************************************/
int fenji_sync_ipc_list(void)
{
	// �ֻ�����Ҫ������ͬ���б�
	if (FALSE == is_main_DeviceNo())
	{
		char DeviceNo[30] = {0};		
		PFULL_DEVICE_NO dev = storage_get_devparam();
		memset(DeviceNo, 0, sizeof(DeviceNo));
		strncpy(DeviceNo, dev->DeviceNoStr, dev->DevNoLen);
		DeviceNo[dev->DevNoLen-1] = '0';
		DeviceNo[dev->DevNoLen] = 0;

		int j = 0;
		for (j=0; j<MAX_HOME_NUM; j++)
		{
			g_ipc_list[j].TimeOut = 0;
		}
		
		if (g_MainDevIP == 0)
		{
			uint32 *IPs  = NULL;				
			
			int count = net_get_ips(dev->AreaNo, DEVICE_TYPE_ROOM, DeviceNo, &IPs);	
			if (count)
			{
				dprintf("lgoic_call.c : logic_call_resident : DeviceNo = %s, count = %d , address = %x \n", DeviceNo, count, IPs[0]);
				g_MainDevIP = IPs[0];

				#ifndef _AU_PROTOCOL_
				DEVICE_NO DestNo;
				DestNo.AreaNo = dev->AreaNo;
				DestNo.DeviceType = DEVICE_TYPE_ROOM;
				DestNo.DeviceNo1 = 0;
				DestNo.DeviceNo2 = atoi(DeviceNo);
				set_nethead(DestNo, PRIRY_DEFAULT);	
				#endif

				uint8 EchoValue;
				uint32 ReciSize;
				char RecData[150] = {0};
				int ret = net_send_command(CMD_GET_HOME_ONLINE_LIST_EXT, NULL, 0, g_MainDevIP, NETCMD_UDP_PORT, 3, &EchoValue, RecData, &ReciSize);
				if (TRUE == ret)
				{
					dprintf("ReciSize : %d, EchoValue: %x\n", ReciSize, EchoValue);
					if (EchoValue == ECHO_OK && ReciSize > 4)
					{
						int i = 0, counts;
						memcpy(&counts, RecData, 4);
						dprintf("counts: %d\n", counts);

						// ��ֹ����counts�����¿���
						if (counts && counts < MAX_HOME_NUM*2)
						{
							PMonitorDeviceList Devlist = NULL;
							storage_free_monitordev_memory(&Devlist);
							storage_get_home_monitordev(&Devlist);
							SelfIPCamera IPC;
							for (i=0; i<counts; i++)
							{
								//SelfIPCamera *IPC = (SelfIPCamera *)(RecData+4+sizeof(SelfIPCamera)*i);
								memset(&IPC, 0, sizeof(SelfIPCamera));
								memcpy(&IPC, (RecData+4+sizeof(SelfIPCamera)*i), sizeof(SelfIPCamera));
								dprintf("IPC.address: %x, IPC.index: %d\n", IPC.address, IPC.index);
								if (Devlist->Homedev[IPC.index].EnableOpen &&
									(0 == strcmp(Devlist->Homedev[IPC.index].FactoryName, "SELFIPC") ||
									0 == strcmp(Devlist->Homedev[IPC.index].FactoryName, "SelfIPC")))
								{
									// Ŀǰֻ�бȽ�IP ��ַ
									dprintf("Devlist->Homedev[IPC.index].DeviceIP: %x\n", Devlist->Homedev[IPC.index].DeviceIP);
									if (IPC.address != 0)
									{
										Devlist->Homedev[IPC.index].DeviceIP = IPC.address;		
										set_ipc_online(IPC.index, IPC.address);
									}									
								}								
							}
							storage_save_monitordev(HOME_CAMERA, Devlist);
							storage_free_monitordev_memory(&Devlist);							
						}
					}				
				}
				else
				{
					g_MainDevIP = 0;
					return FALSE;
				}				
			}
			else
			{
				g_MainDevIP = 0;
				return FALSE;
			}
		}
		else
		{
			#ifndef _AU_PROTOCOL_
			DEVICE_NO DestNo;
			DestNo.AreaNo = dev->AreaNo;
			DestNo.DeviceType = DEVICE_TYPE_ROOM;
			DestNo.DeviceNo1 = 0;
			DestNo.DeviceNo2 = atoi(DeviceNo);
			set_nethead(DestNo, PRIRY_DEFAULT);	
			#endif

			uint8 EchoValue;
			uint32 ReciSize;
			char RecData[150] = {0};
			dprintf("g_MainDevIP: %x\n", g_MainDevIP);
			int ret = net_send_command(CMD_GET_HOME_ONLINE_LIST_EXT, NULL, 0, g_MainDevIP, NETCMD_UDP_PORT, 3, &EchoValue, RecData, &ReciSize);
			if (TRUE == ret)
			{
				dprintf("ReciSize : %d, EchoValue: %x\n", ReciSize, EchoValue);
				if (EchoValue == ECHO_OK && ReciSize > 4)
				{
					int i = 0, counts;
					memcpy(&counts, RecData, 4);
					dprintf("counts: %d\n", counts);
					// ��ֹ����counts�����¿���
					if (counts && counts < MAX_HOME_NUM*2)
					{
							PMonitorDeviceList Devlist = NULL;
							storage_free_monitordev_memory(&Devlist);
							storage_get_home_monitordev(&Devlist);
							SelfIPCamera IPC;
							for (i=0; i<counts; i++)
							{
								//SelfIPCamera *IPC = (SelfIPCamera *)(RecData+4+sizeof(SelfIPCamera)*i);
								memset(&IPC, 0, sizeof(SelfIPCamera));
								memcpy(&IPC, (RecData+4+sizeof(SelfIPCamera)*i), sizeof(SelfIPCamera));
								dprintf("IPC.address: %x, IPC.index: %d\n", IPC.address, IPC.index);
								if (Devlist->Homedev[IPC.index].EnableOpen &&
									(0 == strcmp(Devlist->Homedev[IPC.index].FactoryName, "SELFIPC") ||
									0 == strcmp(Devlist->Homedev[IPC.index].FactoryName, "SelfIPC")))
								{
									// Ŀǰֻ�бȽ�IP ��ַ
									dprintf("Devlist->Homedev[IPC.index].DeviceIP: %x\n", Devlist->Homedev[IPC.index].DeviceIP);
									if (IPC.address != 0)
									{
										Devlist->Homedev[IPC.index].DeviceIP = IPC.address;		
										set_ipc_online(IPC.index, IPC.address);
									}									
								}								
							}
							storage_save_monitordev(HOME_CAMERA, Devlist);
							storage_free_monitordev_memory(&Devlist);							
						}
				}				
			}
			else
			{
				g_MainDevIP = 0;
				return FALSE;
			}				
		}
	}
	else
	{
		return TRUE;
	}		
}
#endif

/*************************************************
  Function:			rtsp_distribute
  Description:		
  Input: 	
  	1.recPacket		���հ�
  Output:			��
  Return:			�ɹ����, TRUE/FALSE
  Others:	
*************************************************/
int32 rtsp_distribute(const PRECIVE_PACKET recPacket)
{
	PNET_HEAD head = (PNET_HEAD)(recPacket->data + MAIN_NET_HEAD_SIZE);
	int32 cmd = head->command | SSC_VIDEOMONITORING << 8;
	
	switch (cmd)
	{											
		case CMD_GET_HOME_CAMERALIST:								
			{
				dprintf("CMD_GET_HOME_CAMERALIST\n");
				#ifdef _IP_MODULE_
				if (recPacket->size > NET_HEAD_SIZE)
				{	
					char *data = recPacket->data + NET_HEAD_SIZE;
					char bindcode[10] = {0};				
					memcpy(bindcode, data, 6);
					if (get_ipmodule_bindcode() != atoi(bindcode))
					{
						net_send_echo_packet(recPacket,ECHO_MAC, NULL, 0);
						break;
					}
				}
				#endif			
				get_homeCamera(recPacket);
				return TRUE;
			}

		case CMD_GET_HOME_CAMERALIST_EXT:								
			{
				dprintf("CMD_GET_HOME_CAMERALIST\n");
				#ifdef _IP_MODULE_
				if (recPacket->size > NET_HEAD_SIZE)
				{	
					char *data = recPacket->data + NET_HEAD_SIZE;
					char bindcode[10] = {0};				
					memcpy(bindcode, data, 6);
					if (get_ipmodule_bindcode() != atoi(bindcode))
					{
						net_send_echo_packet(recPacket,ECHO_MAC, NULL, 0);
						break;
					}
				}
				#endif			
				get_homeCamera_ext(recPacket);
				return TRUE;
			}

		#ifdef _NEW_SELF_IPC_
		case CMD_GET_HOME_ONLINE_LIST_EXT:
			{
				if (TRUE == is_main_DeviceNo())
				{
					#ifdef _AU_PROTOCOL_
					DEVICE_NO RemoDevno = head->DeviceNo;			
					#else
					DEVICE_NO RemoDevno = head->SrcDeviceNo;
					#endif
					DEVICE_NO LocalDevno = storage_get_devno();
					dprintf("remo: %d, local: %d\n", RemoDevno.DeviceNo2, LocalDevno.DeviceNo2);
					if ((RemoDevno.DeviceNo2/10) == (LocalDevno.DeviceNo2/10))	// ȥ��һ���ֻ���
					{		
						int i = 0, index = 0;
						char data[150] = {0};
						memset(data, 0, sizeof(data));
						for (i=0; i<MAX_HOME_NUM; i++)
						{
							if (g_ipc_list[i].TimeOut > 0)
							{
								memcpy(data+4+index*(sizeof(SelfIPCamera)), &g_ipc_list[i].IPList, sizeof(SelfIPCamera));
								index++;
							}
						}
						memcpy(data, &index, 4);
						dprintf("index: %d\n", index);
						net_send_echo_packet(recPacket, ECHO_OK, data, 4+index*(sizeof(SelfIPCamera)));
					}					
				}
				return TRUE;
			}
			break;
		#endif
		
		default:
			break;
	}
	return FALSE;
}

/*************************************************
  Function:			rtsp_responsion
  Description:		
  Input: 	
  	1.recPacket		Ӧ���
  	2.SendPacket	���Ͱ�
  Output:			��
  Return:			��
  Others:
*************************************************/
void rtsp_responsion(const PRECIVE_PACKET recPacket, const PSEND_PACKET SendPacket)
{

}

