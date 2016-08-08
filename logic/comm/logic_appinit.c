/*************************************************
  Copyright (C), 2006-2016, Aurine
  File name:	logic_app.c
  Author:		chenbh
  Version:		2.0
  Date: 		2014-09-30
  Description:	�߼���ʼ��
  History:		  
	1. Date:
	   Author:
	   Modification:
	2. ...
*************************************************/
#include "logic_include.h"

extern int32 af_callback_gui(int32 Param1,int32 Param2);
extern void callout_state_callbak(uint32 param1, uint32 param2);
extern void callin_state_callbak(uint32 param1, uint32 param2);
extern void monitor_state_callbak(uint32 param1, uint32 param2);

extern void show_sys_event_hint(uint16 EventType);
extern int32 rtsp_distribute(const PRECIVE_PACKET recPacket);
extern void rtsp_responsion(const PRECIVE_PACKET recPacket, const PSEND_PACKET SendPacket);

#ifdef _DOOR_PHONE_
/*************************************************
  Function:		analog_callin_io
  Description: 	ģ���豸����io�ӿ�
  Input: 		��			
  Output:		��
  Return:		��
*************************************************/ 
static void analog_callin_io(uint8 event)
{
	dprintf("analog_callin_io: %d \n", event);
	if (MK_CALL_DOWN == event)
	{
		// Ĭ��1 ����
		phone_call_in(1);
	}
}
#endif

/*************************************************
  Function:		logic_init
  Description: 	�߼���ʼ��
  Input: 		��			
  Output:		��
  Return:		��
*************************************************/ 
void logic_init(void)
{
	PFULL_DEVICE_NO pDevNo = NULL;
	PNET_PARAM pNetParam = NULL;

	init_auTimer();        							// ��ʼ����ʱ��
	init_timer();
	storage_init();									// ��ʼ���洢ģ��
	init_list_address();							// ��ʼ�������б�
	NetworkInit();
	
	#ifdef _JD_MODE_
	jd_logic_init();								// ���мҵ�ģ����Բ��������Ź�,�ҵ���Ѳ�������ι��	
	#else	
	hw_start_com_dog();								// �������ڿ��Ź�
	#endif
	
	#ifdef _DOOR_PHONE_
	init_ioctrl_callback(analog_callin_io);
	hw_switch_digit();								// �л�������ͨ·
	#endif

	media_init();
	sys_init_hint_state();
	alarm_logic_init();
	alarm_init_gui_callback((ALARMGUI_CALLBACK)af_callback_gui, (SHOW_SYSEVENHIT)show_sys_event_hint);
	inter_call_ini(callout_state_callbak, callin_state_callbak);
	monitor_ini(monitor_state_callbak);

	net_set_recivedata_func(SSC_INFO, msg_distribute, msg_responsion);
	net_set_recivedata_func(SSC_ALARM, alarm_distribute, alarm_responsion);
	net_set_recivedata_func(SSC_INTERPHONE, inter_distribute, inter_responsion);
	net_set_recivedata_func(SSC_PUBLIC, public_distribute, public_responsion);
	net_set_recivedata_func(SSC_VIDEOMONITORING, rtsp_distribute, rtsp_responsion);
	#ifdef _JD_MODE_	 
	net_set_recivedata_func(SSC_EHOME, jd_net_distribute, jd_net_responsion);
	#endif
	
	pDevNo = storage_get_devparam();
	pNetParam = storage_get_netparam();
	net_start_comm(pDevNo, pNetParam);
	net_send_cmd_packet(NULL, 0, INADDR_BROADCAST, NETCMD_UDP_PORT);

	init_reboot_timer();							// ��ʱ��������
	
	#ifdef _USE_NEW_CENTER_
	logic_reg_center_ini();							// ����λ��ע�ᱣ��
	#endif
	
	#ifdef _NEW_SELF_IPC_
	init_ipc_state();								// add by chenbh 2016-03-09 ��ʼ���ҾӼ����б�����״ֵ̬
	#endif
	
	#ifdef _IP_MODULE_
	init_ipmodule();								// IPģ��� ����
	#endif

	#ifdef _RTSP_REG_KEEP_			
	init_rtsp_timer();								// ����������Ҫ����ý����������� 
	#endif

	#ifdef _YUYUE_MODE_	
	init_yuyue_timer();								// ԤԼģ�鶨ʱ��
	#endif


	// ����ʱ��ͬ��
	uint32 center = storage_get_center_ip();
	set_nethead(G_CENTER_DEVNO, PRIRY_DEFAULT);
	net_direct_send(CMD_REQ_SYN_TIME, NULL, 0, center, NETCMD_UDP_PORT);	

	// ����У�������Ƿ���ȷ
	check_sysconfig_ifcorrect(); 					
	dprintf("devno: %s \n", pDevNo->DeviceNoStr);
}

