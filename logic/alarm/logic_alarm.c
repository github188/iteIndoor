/*********************************************************
  Copyright (C), 2006-2016, Aurine
  File name:	logic_alarm.c
  Author:   	�޷�»
  Version:   	2.0
  Date: 		2014-11-03
  Description:  �����߼�����

  				g_alarm_valid_flag��
				��������������Ч������:
					1.δ��·��δ����ĵ�����Ϊ����״̬�ķ�����(����ʱ������)
					2.��ʱ���������Ǵ��ڵ��������������ǳ���״̬������������Ϊ������Ч
			   	������Ч��������Ϊ��Ч������:
					1.��������ֹͣ�����±���Ϊ��Ч.(�������������±���)
					2.�����ʾ�ͳ�����
				=================================
				g_alarm_delay_flag��
				�����Ƿ�����ʱ��������������
					1.�÷���Ϊ��ʱ�ķ�����������
				����������Ϊ0������
					1.������ʱʱ�������(����÷���һֱ���ڴ���״̬ʱ��������)�����ǳ�����
  History:                   
    1. Date:			 
       Author:				
       Modification:	
    2. ...
*********************************************************/
#include "logic_include.h"

#define SOS_TIME       		3						// sos��3���ڳ���������ΪSOS����,����Ϊ�����
#define DEFINE_SIZE   		21						// �����ֽ�23λ

extern PAF_FLASH_DATA 		gpAfParam;

static struct ThreadInfo 	g_alarm_thread;
static SOUND_TYPE			g_sound_type = NO_SOUND_TYPE;
static ALARMGUI_CALLBACK	g_alarm_callback  = NULL;
static SHOW_SYSEVENHIT		g_show_sysEvenHit = NULL;

static uint8 g_alarm_valid_flag = 0;				// ������Ч��־,ÿһλ����һ������
static uint8 g_alarm_delay_flag = 0;				// ��ʱ����������־,ÿһλ����һ������
static uint8 g_alarming_flag = 0;					// ����������־,ÿһλ����һ������

static uint16 g_alarm_time = 0;						// ����ʱ���ʱ
static uint16 g_delay_time = 0;      	 			// ������ʱʱ���ʱ
static uint16 g_exit_time = 0;       				// �˳�Ԥ��ʱ���ʱ
static uint8 g_sos_time  = 0;						// ������ť��Ч����ʱ��
static uint8 g_sos_state = 0;						// sos�Ƿ񴥷�

static uint8 g_single_defend = 0;					// �����Ƿ�Ϊ���������������Է����������Ե�ʱ�򣬲���״̬���ƻ�
static uint8 g_whole_alarm_state;    				// �����Ƿ񱨾�
static uint8 g_judge_can_defend; 					// �ж��Ƿ��ܲ����������ֵ����δ�ų���ʱ����
static uint8 g_can_hear = 0;						//�Ƿ����  0����1������

static uint8 g_resume_delay_flag = 0;				// ������ʱ��������ʱ����g_alarm_delay_flag�����
static uint8 g_door_ring = 0;						// ��ǰ�崥����־
static uint8 g_7area_set = 0;						//���߷�������������־
static uint8 g_8area_unset = 0;						//�ڰ˷�������������־

// add by  wufn
static uint8 g_Alarm_No24clear = 0;                 // ��24Сʱ���������־,ÿһλ����һ������
static uint8 g_Alarm_No24touch = 0;                 // ��24Сʱ����������ÿһλ����һ������
//static uint8 g_Alarm_Unset = 0;                   // ������24Сʱ�����������������˸״̬(�����˸)
static uint8 g_Alarm_Set_Operator = 0;				// ����������־

static void deal_alarm_func(uint8 num);
static void on_alarm_timer(void * arg);
static void stop_sound_arbistrator(SOUND_TYPE sound_type);
void alarm_deal(SYS_MSG_ID id, uint8 msg);
//void alarm_set_unset(void);

/*************************************************
  Function:    	set_area_show_info
  Description: 	���ð˸������Ľ�����ʾ״̬
  Input:
  	1.can_write	Ҫд��ע���
  Output:    		
  Return:		false:ʧ�� true:�ɹ�	
  Others:           
*************************************************/
static uint8 set_area_show_info(uint8 can_write)
{
   uint8 i;
   
   for (i = 0; i < AREA_AMOUNT; i++)
   {
	   if (0 == (((gpAfParam->enable)>>i)& 0x01))
	   {
			gpAfParam->show_state[i] = UNENABLE_SHOW_STATE;	// ����δ����
	   }
	   else if (1 == ((gpAfParam->isolation>>i)&0x01))
	   {
			gpAfParam->show_state[i] = ISO_SHOW_STATE;		// ��������
	   }
	   else if (ALARM_SHOW_STATE == gpAfParam->show_state[i])
	   {
			continue;
	   }
	   else
	   {
	   		// Ϊ�˷�ֹ���������Ե�ʱ�����������������ƻ������g_single_defend�ж�
	   		if (1 == ((gpAfParam->is_24_hour>>i)&0x01) || (1 == ((g_single_defend>>i) & 0x01)))
	   		{
				gpAfParam->show_state[i] = WORK_SHOW_STATE;
			}
	   		else if (DIS_DEFEND == gpAfParam->defend_state)	// �����ڳ���״̬
	   		{
				gpAfParam->show_state[i] = NORM_SHOW_STATE;
			}
			else if (SET_DEFEND == gpAfParam->defend_state)	// �����ڲ���״̬
	   		{
				gpAfParam->show_state[i] = WORK_SHOW_STATE;
			}
			else if (PART_DEFEND == gpAfParam->defend_state)// �����ھַ�״̬
	   		{
				if (1 == ((gpAfParam->part_valid>>i) & 0x01))
				{
					gpAfParam->show_state[i] = WORK_SHOW_STATE;
				}
				else
				{
					gpAfParam->show_state[i] = NORM_SHOW_STATE;
				}
			}
	   }
   }
   
   // ���÷�����Чλ��־
   g_alarm_valid_flag = gpAfParam->enable & (~gpAfParam->isolation);
   if (DIS_DEFEND == gpAfParam->defend_state)
   {
   		g_alarm_valid_flag &= gpAfParam->is_24_hour;
		g_alarm_valid_flag |= g_single_defend;		// ����������������Ϊ��Ч
   }
   else if (PART_DEFEND == gpAfParam->defend_state)
   {
		g_alarm_valid_flag &= gpAfParam->part_valid;
   }

   // ��ʱ����������Ϊ��Ч������������������ʱ����Ҳ����ʱ 
   for (i = 0; i < AREA_AMOUNT; i++)
   {
   		if (0 == ((gpAfParam->is_24_hour>>i)&0x01))
   		{
			if (gpAfParam->delay_time[i] > 0)
			{
				g_alarm_valid_flag &= ~(1<<i);
			}
   		}
   }
   
   if (1 == can_write)
   {
   		SaveRegInfo();
   }

   return 0;
}

/*************************************************
  Function:    		alarm_get_area_define
  Description: 		�������ҳ����ʾ��Ϣ
  Input:
  Output:    		
  	1.define_info	����ҳ����ʾ����Ϣ
  Return:				
  Others:    		       
*************************************************/
void alarm_get_area_define(uint8 * define_info)
{
	if (NULL == define_info)
	{
		return;
	}
	
	memcpy(define_info, (uint8 *)(gpAfParam) + 2, DEFINE_SIZE);
}

/*************************************************
  Function:    		alarm_set_area_define
  Description: 		���ñ�������ҳ����ʾ��Ϣ
  Input:
   	1.define_info	���Ե�����
  Output:    		
  Return:			false:ʧ�� true:�ɹ�				
  Others:           
*************************************************/
uint8 alarm_set_area_define(uint8 *define_info)
{
	if (NULL == define_info)
	{
		return FALSE;
	}
	
	memcpy((uint8*)(gpAfParam) + 2, define_info,  DEFINE_SIZE);
	
	return set_area_show_info(1);
}

/*************************************************
  Function:    		alarm_get_area_isolation
  Description: 		��ȡ�����������
  Input:
  Output:   
  Return:			��ʱ�������
  Others:           
*************************************************/
uint8 alarm_get_area_isolation(void)
{
	return gpAfParam->isolation;
}

/*************************************************
  Function:    		alarm_set_area_isolation
  Description: 		���ñ�����ʱ������ʾ��Ϣ
  Input:
  	1.iso_info		��ʱ������ʾ��Ϣ
  Output:    		
  Return:			false:ʧ�� true:�ɹ�
  Others:           
*************************************************/
uint8 alarm_set_area_isolation(uint8 iso_info)
{
	gpAfParam->isolation = iso_info;
	
	return set_area_show_info(1);
}

/*************************************************
  Function:    		alarm_get_part_valid
  Description: 		��ȡ�����ַ���Ч����
  Input:
  Output:   
  Return:			�ַ���Ч����	
  Others:           
*************************************************/
uint8 alarm_get_part_valid(void)
{
	return gpAfParam->part_valid;
}

/*************************************************
  Function:    		alarm_set_area_part_valid
  Description: 		���ñ����ַ���Чҳ����ʾ��Ϣ
  Input:
  	1.valid			�ַ���Чҳ����ʾ��Ϣ
  Output:    		
  Return:			false:ʧ�� true:�ɹ�
  Others:           
*************************************************/
uint8 alarm_set_area_part_valid(uint8 valid)
{
	if (gpAfParam->is_24_hour != (gpAfParam->is_24_hour & valid))
	{
		return 2;
	}
	gpAfParam->part_valid = valid;
	
	return set_area_show_info(1);
}

/*************************************************
  Function:    		start_timerEx
  Description: 		
  Input:   			
  Output:    		��
  Return:			
  Others:           
*************************************************/
static int start_timerEx(uint32 tick, void* proc, struct ThreadInfo* thread)
{
	 return inter_start_thread(thread, proc, (void *)tick, 0);
}

/*************************************************
  Function:    	on_alarm_timer
  Description: 	����ģ��Ķ�ʱ��������
  Input:   			
  Output:    	��
  Return:			
  Others:           
*************************************************/
static void deal_alarm_timer(void * arg)
{
	pthread_detach(pthread_self());
	dprintf("g_alarm_thread.running:%d\n",g_alarm_thread.running);
	while (g_alarm_thread.running)
	{
		on_alarm_timer(0);
		sleep((uint32)arg);
	}
	dprintf("end thread running %d\n",__LINE__);
	pthread_exit(NULL);
}

/*************************************************
  Function:    	start_alarm_timer
  Description: 	��ʼ����ģ��Ķ�ʱ��, 1�붨ʱ
  Input:   			
  Output:    	��
  Return:			
  Others:           
*************************************************/
static void start_alarm_timer(void)
{
	//OSSchedLock();
	if (g_alarm_thread.running == 0)
	{	
		dprintf("%d\n",__LINE__);
		start_timerEx(1, deal_alarm_timer, &g_alarm_thread);
	}
	//OSSchedUnlock();
}

/*************************************************
  Function:    		stop_alarm_timer
  Description: 		ֹͣ����ģ��Ķ�ʱ��
  Input:   			
  Output:    		��
  Return:			
  Others:           
*************************************************/
static void stop_alarm_timer(void)
{
	//OSSchedLock();
	dprintf("%d %ld\n",__LINE__,g_alarm_thread.thread);
	if (g_alarm_thread.running)
	{
		g_alarm_thread.running = 0;
		//g_alarm_thread.thread = 0;
	}
	// �ر����б�������
	stop_sound_arbistrator(STOP_ALL_SOUND_TYPE);
	dprintf("%d %ld\n",__LINE__,g_alarm_thread.thread);
	//OSSchedUnlock();
}


/*************************************************
  Function:    		change_byte_order
  Description: 		ת���ֽ�˳��
  Input:  
  	1.byte          ԭ�ֽ�
  Output:    		��
  Return:			�任����ֽ�˳��
  Others:          
*************************************************/
static uint8 change_byte_order(uint8 byte)
{
	int8 i;
	uint8 new_byte = 0;
	
	for (i = AREA_AMOUNT - 1; i >= 0; i--)
	{
		new_byte |= ((byte >> i) & 0x01) << (AREA_AMOUNT - 1 - i);
	}

	return new_byte;
}

/*************************************************
  Function:    		center_get_area_define
  Description: 		���Ĺ������ȡ�ķ�������
  Input:
  Output:  
   	1.define 		���Ĺ������ȡ������
  Return:				
  Others:         	
  					���Ĺ������ȡ�ķ���������
  					�����������ò�������������
*************************************************/
void center_get_area_define(ALARM_DEFINE_NET_DATA * alarm_define)
{
	uint8 bypass;
	
	if (NULL == alarm_define)
	{
		return;
	}
	
	memset(alarm_define, 0, sizeof(ALARM_DEFINE_NET_DATA));
	alarm_define->area_amount = AREA_AMOUNT * 2;	// ������

	bypass = gpAfParam->enable;						// �������ò���
	alarm_define->bypass[0] = change_byte_order(bypass);
	alarm_define->bypass[1] = change_byte_order(bypass);

	storage_get_area_type(alarm_define->area_type);	// ��������
	storage_get_area_type(alarm_define->area_type+AREA_AMOUNT);	// ��������
}

/*************************************************
  Function:    		get_single_area_status
  Description: 		���Ĺ��������ķ���״̬
  Input:
  Output:    		
  Return:			�����򳷷�
  Others:           
*************************************************/
static uint8 get_single_area_status(void)
{
	uint8 state = gpAfParam->is_24_hour;
	
	if (SET_DEFEND == gpAfParam->defend_state)
	{
		state = 0xFF;
	}
	else if (PART_DEFEND == gpAfParam->defend_state)
	{
		state = gpAfParam->part_valid;
	}
	return state;
}

/*************************************************
  Function:    		center_get_area_state
  Description: 		���Ĺ�����Ļ�ȡ�İ���״̬
  Input:			��
  Output:		
  	1.status 		���Ĺ������ȡ�İ���״̬�ṹ��
  Return:			��	
  Others:           
*************************************************/
void center_get_area_state(ALARM_STATUES_NET_DATA * status)
{
	if (NULL == status)
	{
		return;
	}
	
	memset(status, 0, sizeof(ALARM_STATUES_NET_DATA));

	status->defend_state = (DEFEND_STATE)gpAfParam->defend_state;
	status->area_amount = AREA_AMOUNT * 2;
	status->area_state[0] = change_byte_order(get_single_area_status());
	status->alarm_state[0] = change_byte_order(g_alarming_flag);
}

/*************************************************
  Function:    		ipad_get_area_state
  Description: 		ipad��ȡ����״̬
  Input:			��
  Output:		
  	1.status 		ipad��ȡ�İ���״̬�ṹ��
  Return:			��	
  Others:           
*************************************************/
void ipad_get_area_state(ALARM_STATUES_NET_DATA * status)
{
	uint8 i, alarm_state = 0;
	
	if (NULL == status)
	{
		return;
	}
	
	memset(status, 0, sizeof(ALARM_STATUES_NET_DATA));

	status->defend_state = (DEFEND_STATE)gpAfParam->defend_state;
	status->area_amount = AREA_AMOUNT * 2;
	status->area_state[0] = change_byte_order(get_single_area_status());
	for (i = 0; i < AREA_AMOUNT; i++)
	{
		if (ALARM_SHOW_STATE == (SHOW_STATE)gpAfParam->show_state[i])
		{
			alarm_state |= 0x01 << i;
		}
	}
	status->alarm_state[0] = change_byte_order(alarm_state);
}

/*************************************************
  Function:    		get_time_for_timetype
  Description: 		����ʱ�����ͻ�ȡʱ��
  Input:   		
  	1.sound_type	��������
  	2.time_type     ʱ������
  Output:    		��
  Return:			
  Others:           
*************************************************/
uint16 get_time_for_timetype(SOUND_TYPE sound_type, uint8 time_type)
{
	uint16 time = 0;
	switch(sound_type)
	{
		case IN_SOUND_TYPE:
			 switch (time_type)
			 {
				case 0: 
					time = 0;
					break;
				case 1:
					time = 30;
					break;
				case 2: 
					time = 60;
					break;
				case 3:
					time = 60*2;
					break;
				case 4: 
					time = 60*5;
					break;
			 }
			 break;
			 
		case OUT_SOUND_TYPE:
			 switch (time_type)
			 {
				case 0:
					time = 30;
					break;
				case 1: 
					time = 60;
					break;
				case 2:
					time = 60*2;
					break;
				case 3: 
					time = 60*3;
					break;
				case 4:
					time = 60*5;
					break;
			 }
			 break;
			 
		case ALARM_SOUND_TYPE:
			switch (time_type)
			 {
				case 0: 
					time = 60;
					break;					
				case 1:
					time = 3*60;
					break;					
				case 2: 
					time = 5*60;
					break;					
				case 3:
					time = 10*60;
					break;					
				case 4: 
					time = 15*60;
					break;
			 }
			 break;
			 
		default:
			break;
	}

	return time;
}

/*************************************************
  Function:    		stop_sound_arbistrator
  Description: 		���������Ĳ����߼��ж�
  Input:   		
  	1.sound_type	��������
  Output:    		��
  Return:			
  Others:           
*************************************************/
static void stop_sound_arbistrator(SOUND_TYPE sound_type)
{
	SYS_MEDIA_TYPE media_state = sys_get_media_state();
	SYS_MEDIA_TYPE stop_type = 0;
	switch (sound_type)
	{
		// ����Ԥ��
		case IN_SOUND_TYPE:
		case OUT_SOUND_TYPE:
			if (SYS_MEDIA_YUJING != media_state)
			{
				return;
			}
			break;
			
		case ALARM_SOUND_TYPE:
			if (SYS_MEDIA_ALARM != media_state)
			{
				g_sound_type = NO_SOUND_TYPE;
				return;
			}
			break;
			
		case STOP_ALL_SOUND_TYPE:
			if (SYS_MEDIA_ALARM != media_state && SYS_MEDIA_YUJING != media_state)
			{
				g_sound_type = NO_SOUND_TYPE;
				return;
			}
			break;

		default:
			break;
	}
	
	switch (g_sound_type)
	{
		// ����Ԥ��
		case IN_SOUND_TYPE:
		case OUT_SOUND_TYPE:
			stop_type = SYS_MEDIA_YUJING;
			break;
			
		case ALARM_SOUND_TYPE:
			stop_type = SYS_MEDIA_ALARM;
			break;

		default:
			break;
	}
	dprintf("stop_sound_arbistrator media_state: %d,g_sound_type: %d \n", media_state, g_sound_type);
	sys_stop_play_audio(stop_type);
	g_sound_type = NO_SOUND_TYPE;
}

/*************************************************
  Function:    		MediaStopedCallback
  Description: 		���������Ĳ����߼��ж�
  Input:   		
  Output:    		��
  Return:			
  Others:           
*************************************************/
static void alarm_time_stop_callback(void)
{
	switch (g_sound_type)
	{
		case IN_SOUND_TYPE:
			//g_delay_time = 0;
			break;
		case ALARM_SOUND_TYPE:
			//g_alarm_time = 0;
			break;
			
		default:
			break;
	}
}

/*************************************************
  Function:    		sound_arbistrator
  Description: 		���������Ĳ����߼��ж�
  Input:   		
  	1.sound_type	��������
  	2.time_type     ʱ������
	3.num			������ add by wufn 2016.4.22 ����������ʱ���������ϱ�����
  Output:    		��
  Return:			��
  Others:           		��
*************************************************/
static void sound_arbistrator(SOUND_TYPE sound_type, uint8 time_type ,uint8 num)
{
	uint16 time = get_time_for_timetype(sound_type, time_type);
	SYS_MEDIA_TYPE MediaType = sys_get_media_state();
	
	switch(sound_type)
	{
		// ����Ԥ��
		case IN_SOUND_TYPE:
			if (0 == g_delay_time)
			{
				g_delay_time = time;
			}
			else if(g_delay_time > time)			// ȡ�̵�ʱ����Ϊ��ʱʱ��
			{
				g_delay_time = time;
				return;
			}
			else if (IN_SOUND_TYPE == g_sound_type)
			{
				return;
			}

			if (DIS_DEFEND == gpAfParam->defend_state)
			{
				// ����ʱ��������������ʱ����ʱ����������Ҫ������
				if (0 == (g_alarm_delay_flag&g_single_defend))
				{
					return;
				}
			}
			// 0 == g_exit_time ��֤�ȵ��˳�Ԥ������������Ԥ���ſ�ʼ��
			dprintf("======g_sound_type :%d====\n",g_sound_type);
			if (ALARM_SOUND_TYPE != g_sound_type && 0 == g_exit_time)
			{
				if (1 == gpAfParam->used_exit_sound)
				{
					//if (SYS_MEDIA_ECHO_OK != sys_start_play_audio(SYS_MEDIA_YUJING, NULL, TRUE, AF_HIT_VOL, NULL, alarm_time_stop_callback))
					if (SYS_MEDIA_ECHO_OK != sys_start_play_audio(SYS_MEDIA_YUJING, (char *)storage_get_yj_path(), TRUE, AF_HIT_VOL, NULL, alarm_time_stop_callback))
					{
						show_sys_event_hint(AS_ENTER_YUJING);
					}
				}
				g_sound_type = IN_SOUND_TYPE;
			}
			break;

		// �˳�Ԥ��
		case OUT_SOUND_TYPE:
			if (1 == gpAfParam->used_exit_sound)
			{
				sys_start_play_audio(SYS_MEDIA_YUJING, (void *)storage_get_yj_path(), TRUE, AF_HIT_VOL, NULL, alarm_time_stop_callback); 
				sys_set_no_deal(TRUE);
			}
			g_exit_time = time;
			g_sound_type = OUT_SOUND_TYPE;
			break;

		// ����
		case ALARM_SOUND_TYPE:				//modify by wufn 2016.4.22 
			dprintf("=g_sound_type: %d==\n",g_sound_type);
			if (ALARM_SOUND_TYPE != g_sound_type)
			{
				if(0 == ((g_can_hear >> num) & 0x01))		
				{
					sys_start_play_audio(SYS_MEDIA_ALARM, storage_get_bj_path(), TRUE, AF_HIT_VOL, NULL, alarm_time_stop_callback); 
				}
  				g_alarm_time = time;
				g_sound_type = ALARM_SOUND_TYPE;
			}
			else 
			{
				if (time > g_alarm_time)
				{
					g_alarm_time = time;
				}
				dprintf("==MediaType :%d===\n",MediaType);
				if(SYS_MEDIA_ALARM != MediaType)
				{
					dprintf("==g_can_hear=: %d====(g_can_hear>>num) & 0x01):%d\n",g_can_hear,((g_can_hear>>num) & 0x01));
					if(0 == ((g_can_hear>>num) & 0x01))
					{
						sys_start_play_audio(SYS_MEDIA_ALARM, storage_get_bj_path(), TRUE, AF_HIT_VOL, NULL, alarm_time_stop_callback); 
					}
				}
				else
				{
					return;
				}
			}
			break;

		default:
			break;
	}
	
	dprintf("sound_arbistrator g_alarm_time: %d, g_sound_type; %d  media_state: %d\n", g_alarm_time, g_sound_type, sys_get_media_state());
	start_alarm_timer();
}

/*************************************************
  Function:    		get_min_delay_time
  Description: 		����д�����ʱ��������Сֵ
  Input:   			��
  Output:    		��
  Return:			��			
  Others:
*************************************************/
static uint8 get_min_delay_time(void)
{
	uint8 i;
	uint8 tmp = 254;
	
	for (i = 0; i < AREA_AMOUNT; i++)
	{
		if (0 != ((g_alarm_delay_flag>>i) & 0x01))
		{
			if (gpAfParam->delay_time[i] < tmp)
			{
				tmp = gpAfParam->delay_time[i];
			}
		}
	}
	return tmp;
}

/*************************************************
  Function:    		on_alarm_timer
  Description: 		����ģ��Ķ�ʱ��������
  Input:   			
  Output:    		��
  Return:			
  Others:           
*************************************************/
static void on_alarm_timer(void * arg)
{
	// ��һ������������
	if (1 == g_sos_state && (g_sos_time > 0)) 
	{
		dprintf("g_sos_time: %d\n",g_sos_time);
		g_sos_time--;
		if (0 == g_sos_time)
		{
			g_sos_state = 0;
			g_alarming_flag |= 0x01;
			alarm_deal(AS_ALARM_PROC, 0);
		}
	}

	// ֹͣ����������
	if (ALARM_SOUND_TYPE == g_sound_type)
	{
		dprintf("g_alarm_time: %d\n",g_alarm_time);
		if (g_alarm_time > 0)
		{
			g_alarm_time--;
		}
		else
		{
			stop_sound_arbistrator(ALARM_SOUND_TYPE);
		}
	}
	
	// �˳�Ԥ������, 8����������һ��ʱ��
	if (g_exit_time > 0)
	{
		g_exit_time--;
		dprintf("g_exit_time %d\n",g_exit_time);

		if ((0 != g_alarm_time) || 0 == g_exit_time)			//����󴥷���ʱ�б����Ļ������ϱ�
		{
			if (0 != g_exit_time)
			{
				g_exit_time = 0;
			}
			if (OUT_SOUND_TYPE == g_sound_type)
			{
				stop_sound_arbistrator(OUT_SOUND_TYPE);
			}
			// �˳�Ԥ��ʱ�����,��ʱ�������ʱ��������,�����Ԥ����ʾ
			if (g_alarm_delay_flag > 0)
			{
				sound_arbistrator(IN_SOUND_TYPE, get_min_delay_time(), 0);
			}
		}
	}
	// ����Ԥ������
	else if (0 == g_delay_time)						// ����Ԥ��ʱ�䵽
	{
	}
	else
	{
		if (DIS_DEFEND == gpAfParam->defend_state)
		{
			// ����ʱ����δ����������ʱ������ʱ
			//if (0 == (g_alarm_delay_flag&g_single_defend))
			// �޸�7-27������������ʱ����������,g_alarm_delay_flag��Ϊ0
			if (0 == (g_single_defend))
			{
				return;
			}
		}
		dprintf("g_delay_time:%d\n",g_delay_time);
		g_delay_time--;
		if ((0 == g_delay_time) || (0 != g_alarm_time))	
				// ����Ԥ��ʱ�䵽
		{			
			if (0 != g_delay_time)
			{
				g_delay_time = 0;
			}
			// Ԥ��ʱ�䵽��ֹͣԤ����
			if (IN_SOUND_TYPE == g_sound_type)
			{
				//sys_stop_play_audio();
				stop_sound_arbistrator(IN_SOUND_TYPE);
			}

			// ��Ԥ�����������ڴ���״̬,��Ԥ��תΪ��ʽ����
			if (0 != g_alarm_delay_flag)
			{
				uint8 i;
				for (i = 0; i < AREA_AMOUNT; i++)
				{
					if ((1 == ((g_alarm_delay_flag>>i) & 0x01))&& (0 == ((g_alarming_flag>>i) & 0x01)))
					{
						// ������״̬���������������ķ�����ת
						if (DIS_DEFEND == gpAfParam->defend_state)
						{
							if (0 == (0x01&(g_single_defend>>i)))
							{
								continue;
							}
						}
						if (1 == ((g_resume_delay_flag>>i)&0x01))
						{
							g_alarm_delay_flag &= ~(1>>i);
							g_resume_delay_flag &= ~(1>>i);
						}
						g_alarming_flag |= 1<<i;
						alarm_deal(AS_ALARM_PROC, i);
					}
				}
			}
		}
	}

	if (0 == g_sos_time && 0 == g_exit_time && 0 == g_delay_time && 0 == g_alarm_time)
	{
		dprintf("%d\n",__LINE__);
		stop_alarm_timer();
		return;
	}
}

/*************************************************
  Function:			judge_can_be_defend
  Description: 		��ȡ�Ƿ��ܲ���
  Input: 
  Output:			��
  Return:			��
  Others:			��
*************************************************/
uint8 judge_can_be_defend(void)
{
	uint8 i;
	uint8 tmp = g_judge_can_defend;
	
	// Ϊ���Ƿ��ܲ������ж�
    for (i = 0; i < AREA_AMOUNT; i++)
    {
 		if (gpAfParam->delay_time[i] > 0)
 		{
 			tmp &= ~(1<<i);
 		}
    }
	return tmp;
}

/*************************************************
  Function:			check_defend_oper
  Description:		����״̬����
  Input: 
  	1.state			����״̬
  	2.exec			ִ����
  Output:			��
  Return:			��
  Others:			��
*************************************************/
AF_RETURN_TYPE check_defend_oper(DEFEND_STATE state, EXECUTOR_E exec)
{
	if (EXECUTOR_LOCAL_HOST != exec)
	{
		// �ж��Ƿ���Զ�̿���Ȩ��
		if (0 == ((gpAfParam->remote_set >> state) & 0x01))
		{
			return AF_NO_REMOTE_CTRL;
		}

		// �жϱ���ģ���Ƿ�����
		if (FALSE == storage_get_extmode(EXT_MODE_ALARM))
		{
			return AF_SETTING_ERR;
		}
	}
	
	if (DIS_DEFEND == state)
	{
		return AF_SUCCESS;
	}
	
	if (DIS_DEFEND != gpAfParam->defend_state)
	{
		if (gpAfParam->defend_state == state)
		{
			return AF_STATE_SAME;
		}
		else
		{
			return AF_CANNOT_OVERLEAP;
		}
	}
	else
	{
		// �����󣬿��ܱ�����ʹ�ܻ���·������Ҫ�����ж���
		if (g_alarm_time > 0)
		{
			sys_start_play_audio(SYS_MEDIA_YUJING, storage_get_fqcf_path(), FALSE, AF_HIT_VOL, NULL, NULL); 
			return 	AF_BAOJINGNOBUFANG;
		}
		else if(0 != judge_can_be_defend())
		{
			sys_start_play_audio(SYS_MEDIA_YUJING, storage_get_fqcf_path(), FALSE, AF_HIT_VOL, NULL, NULL); 
			return  AF_FQCHUFA;
		}
	}

	return AF_SUCCESS;
}

/*************************************************
  Function:			on_alarm_sound
  Description:		���������ص�
  Input: 		
  Output:			��
  Return:			��
  Others:			��
*************************************************/
static int32 on_alarm_sound(int32 param1, int32 param2 ,int32 state)
{
	dprintf("param1 = %d,param2 = %d state = %d\n",param1,param2,state);
	if (FALSE == state)
	{
		
	}
	else if (100 == param2)
	{
		g_exit_time = 0;
		
		if (ALARM_SOUND_TYPE == g_sound_type)
		{
 			return 0;
		}
		// ���������ϳ������п��ܻ��������������Ҫ��SET_DEFEND == gpAfParam->defend_state�ж�
		if (SET_DEFEND == gpAfParam->defend_state)
		{
			dprintf("%d\n",__LINE__);
			sound_arbistrator(OUT_SOUND_TYPE, gpAfParam->exit_alarm_time, 0);
		}
	}

	return 0;
}

/*************************************************
  Function:    		alarm_gui_callback
  Description: 		ִ�а����ص�
  Input:
  	1.Param1		����1		
  	2.Param2		����2
  Output: 			��   		
  Return:			��
  Others:           
*************************************************/
void alarm_gui_callback(int32 Param1, int32 Param2)
{
	if (g_alarm_callback)
	{
		g_alarm_callback(Param1, Param2);
	}
}

/*************************************************
  Function:			alarm_clear_alerts_operator
  Description:		�����ʾ����
  Input: 		
  Output:			��
  Return:			
  Others:			��
*************************************************/
void alarm_clear_alerts_operator(void)
{
	uint8 i;
	uint8 state;

	state = gpAfParam->is_24_hour ;
	for (i = 0; i < AREA_AMOUNT; i++)
	{

		if(gpAfParam->show_state[i] == ALARM_SHOW_STATE)
		{
			// �����24Сʱ�������������󳷷�̽ͷδ�ָ������ʾ��״̬��˸ add by wufn 2015-8-15 
			if (0 == ((state>>i)&0x01))
			{
				if(1 == g_Alarm_Set_Operator)
				{
					gpAfParam->show_state[i] = NORM_SHOW_STATE;
					continue;
				}

				if(1 == ((g_Alarm_No24clear>>i) & 0x01))
				{
					continue;
				}
				else
				{
					g_Alarm_No24clear |= (1<<i);
					continue;
				}
				g_Alarm_No24touch&= ~(1<<i);
			}
			else
			{
				gpAfParam->show_state[i] = NORM_SHOW_STATE;
			}
		}
		
		else 
		{
			gpAfParam->show_state[i] = NORM_SHOW_STATE;				
		}
	}
	
	g_Alarm_Set_Operator = 0;
	
	// ����Ĵ���ֻ�ǰ����з�����Ϊ�Ǳ���״̬,�������жϻ�������ĺ���
	set_area_show_info(1);

	if (DIS_DEFEND == gpAfParam->defend_state)
	{
		//g_alarm_time = 0;
		if (0 == g_alarming_flag)
		{
			sys_set_hint_state(SYS_HINT_ALARM_WARNING, FALSE);	// �رձ�����
		}
		g_whole_alarm_state = 0;
		g_alarming_flag = 0;									// ������24Сʱ�����������´���				
	}

}

/*************************************************
  Function:			alarm_set_operator
  Description:		��������
  Input: 
   	1.exec			ִ����
  Output:			��
  Return:			��
  Others:			��
*************************************************/
AF_RETURN_TYPE alarm_set_operator(EXECUTOR_E exec)
{
	AF_RETURN_TYPE retype = check_defend_oper(SET_DEFEND, exec);
 	if (AF_SUCCESS != retype)
 	{
		return retype;
	}

	gpAfParam->defend_state = SET_DEFEND;
	g_Alarm_Set_Operator = 1;						
	alarm_clear_alerts_operator();
	g_alarm_delay_flag = 0;							// ֻ���ڲ����ϱ�����״̬����������������
	g_resume_delay_flag = 0;
	if (EXECUTOR_LOCAL_HOST == exec)				// ��������,��������
	{
		g_exit_time = get_time_for_timetype(OUT_SOUND_TYPE, gpAfParam->exit_alarm_time);  
		sys_start_play_audio(SYS_MEDIA_YUJING, storage_get_yj_wc_path(), FALSE, AF_HIT_VOL, on_alarm_sound, NULL); 
	}
	else
	{
		alarm_gui_callback(BAOJING_FRESH, 0);		// ˢ�½���
	}

	set_event_report(exec);
	storage_add_afcz_record(SET_DEFEND, exec);		// д���������¼�
	sys_set_hint_state(SYS_HINT_ALARM_STATE, TRUE);	

	#ifdef _JD_MODE_
	linkage_alarm_scene(SET_DEFEND);				// add by luofl 2011-08-03 ��������
	#endif
	hw_get_zone_status();		
	return AF_SUCCESS;
}

/*************************************************
  Function:			alarm_single_set_operator
  Description:		��������������
  Input: 		
  	1.num			������
  Output:			��
  Return:			��
  Others:			��
*************************************************/
AF_RETURN_TYPE alarm_single_set_operator(uint8 num)
{
	uint8 state, tmp, i;
	
	if (DIS_DEFEND != gpAfParam->defend_state)
	{
		return AF_SETTING_ERR;
	}
	
	state = gpAfParam->is_24_hour | gpAfParam->isolation | (~gpAfParam->enable);
	// ����,δ����,24Сʱ����������
	if (1 == ((state>>num)&0x01))
	{
		return AF_NO_REMOTE_CTRL;
	}
	
	tmp = judge_can_be_defend();
	if (1 == ((tmp>>num)&0x01))
	{
		return AF_FQCHUFA;
	}
	g_single_defend |= 1<<num;
	set_area_show_info(1);

	// ��ʱ��������Ҫ��ʼ��ʱ
	i = num;
	if (gpAfParam->delay_time[i] > 0 && (1 == ((g_alarm_delay_flag>>i) & 0x01)))
	{
		g_alarm_delay_flag &= ~(1<<i);				// �÷���Ԥ����־
	}
	for (i = 0; i < AREA_AMOUNT; i++)
	{
		if (NORM_SHOW_STATE == gpAfParam->show_state[i])
		{
			break;
		}
	}
	if (AREA_AMOUNT == i)
	{
		alarm_set_operator(EXECUTOR_LOCAL_HOST);
		alarm_gui_callback(BAOJING_FRESH, 0);		// ˢ�½���
	}
	
	return AF_SUCCESS;
}

/*************************************************
  Function:			alarm_unset_operator
  Description:		��������
  Input: 
  	1.exec			ִ����
  	2.force_type    1:Ю�� 0:����
  Output:			��
  Return:			��
  Others:			��
*************************************************/
AF_RETURN_TYPE alarm_unset_operator(EXECUTOR_E exec, uint8 force_type)
{
	AF_RETURN_TYPE retype;
	
	if (1 == force_type)
	{
		force_alarm_report();
	}

	retype = check_defend_oper(DIS_DEFEND, exec);
 	if (AF_SUCCESS != retype)
 	{
		return retype;
	}

	//alarm_set_unset();
	stop_alarm_timer();
	g_alarm_delay_flag = 0;
	g_resume_delay_flag = 0;
	g_exit_time = 0;
	g_delay_time = 0;
	g_single_defend = 0;							// ԭ�����������ָ�ԭ״̬
	// ��ʱ����ʱ��ֹͣ����������ֹͣ���������ݵĻָ�
	
	g_sound_type = NO_SOUND_TYPE;

 	if (EXECUTOR_LOCAL_HOST == exec)				// ��������,��������
	{
		if (1 == g_whole_alarm_state)				// "ע���о���"
		{
			sys_start_play_audio(SYS_MEDIA_YUJING, storage_get_yjq_path(), FALSE, AF_HIT_VOL, NULL, NULL); 
			g_whole_alarm_state = 0;
		}
		else										// "�����ѽ��"
		{
			if (gpAfParam->defend_state != DIS_DEFEND)
			{
				sys_start_play_audio(SYS_MEDIA_YUJING, storage_get_jjyjc_path(), FALSE, AF_HIT_VOL, NULL, NULL); 
			}
		}
	}
		
	g_whole_alarm_state = 0;					// Զ�̳���Ҳ��Ϊ 0 
	gpAfParam->defend_state = DIS_DEFEND;
	set_area_show_info(1);
	alarm_gui_callback(BAOJING_FRESH,0);			// ˢ�½���

	g_alarm_time = 0;

	unset_event_report(exec);						// ���������¼��ϱ�
	storage_add_afcz_record(DIS_DEFEND, exec);		// д���������¼�
	if (0 == g_alarming_flag)
	{
		sys_set_hint_state(SYS_HINT_ALARM_STATE, FALSE);// �رձ�����
	}
    g_alarming_flag = 0;
	
	//ld_action_deal(LD_ACTION_AF_HOME,0,NULL);		// ��������	
	#ifdef _JD_MODE_
	linkage_alarm_scene(DIS_DEFEND);				// add by luofl 2011-08-03 ��������
	#endif
	hw_get_zone_status();							//add by wufn ������ȡ״̬2015-9-07
	return AF_SUCCESS;
}

/*************************************************
  Function:			alarm_partset_operator
  Description:		�ַ�����
  Input: 	
  	1.exec			ִ����
  Output:			��
  Return:			��
  Others:			��
*************************************************/
AF_RETURN_TYPE alarm_partset_operator(EXECUTOR_E exec)
{
	AF_RETURN_TYPE retype = check_defend_oper(PART_DEFEND, exec);
 	if (AF_SUCCESS != retype)
 	{
		return retype;
	}
	gpAfParam->defend_state = PART_DEFEND;
	g_Alarm_Set_Operator = 1;						// ����û��������ʱ������ҹ��ʱͼ�괦�� add by wufn
	alarm_clear_alerts_operator();
	g_alarm_delay_flag = 0;							// ֻ���ڲ����ϱ�����״̬����������������
	g_resume_delay_flag = 0;

	if (EXECUTOR_LOCAL_HOST == exec)				// ��������,��������
	{
		sys_start_play_audio(SYS_MEDIA_YUJING, storage_get_wanan_path(), FALSE, AF_HIT_VOL, NULL, NULL); 
	}
	else
	{
		alarm_gui_callback(BAOJING_FRESH, 0);		// ˢ�½���
	}

	partset_event_report(exec);						// �ַ������¼��ϱ�
	storage_add_afcz_record(PART_DEFEND, exec);		// д���������¼�
	sys_set_hint_state(SYS_HINT_ALARM_STATE, TRUE);	// �򿪱�����
	hw_get_zone_status();		
	return AF_SUCCESS;
}

// ���Ӳ������������ַ�����ͬ�����ֻ��Ĳ������� add by luofl 2011-4-27
#ifdef _ALARM_OPER_SYNC_
/*************************************************
  Function:			alarm_check_devno
  Description:		����ǲ���ͬһ�����ŵķֻ�
  Input: 			��
  Output:			��
  Return:			TRUE, FALSE
  Others:			��
*************************************************/
uint8 alarm_check_devno(const PRECIVE_PACKET RecvPacket)
{
	EXECUTOR_E exec;
	PNET_HEAD head;
	PFULL_DEVICE_NO devparam;
	uint32 remoteDevno1, remoteDevno2, localDevno1, localDevno2;

	head = (PNET_HEAD)(RecvPacket->data + MAIN_NET_HEAD_SIZE);
	remoteDevno1 = head->SrcDeviceNo.DeviceNo1;
	remoteDevno2 = head->SrcDeviceNo.DeviceNo2  / 10;		// ȥ���ֻ���
 	devparam = storage_get_devparam();
 	localDevno1 = devparam->DeviceNo.DeviceNo1;
 	localDevno2 = devparam->DeviceNo.DeviceNo2  / 10; 	// ȥ���ֻ���
 	
 	exec = (EXECUTOR_E)(* (RecvPacket->data + NET_HEAD_SIZE));

 	if ((exec == EXECUTOR_REMOTE_DEVICE) && (localDevno1 == remoteDevno1))
	{
		if (localDevno2 == remoteDevno2)
		{
			return TRUE;
		}
 	}
 	return FALSE;
}

/*************************************************
  Function:			alarm_set_response
  Description:		�Բ��������¼������Ӧ��
  Input: 			��
  Output:			��
  Return:			��
  Others:			��
*************************************************/
void alarm_set_response(void)
{
	gpAfParam->defend_state = SET_DEFEND;
	alarm_clear_alerts_operator();
	g_alarm_delay_flag = 0;					// ֻ���ڲ����ϱ�����״̬����������������
	g_resume_delay_flag = 0;

	alarm_gui_callback(BAOJING_FRESH, 0);	// ˢ�½���
	#ifdef _ALARM_OPER_SYNC_
	sync_comm_event_send(CMD_RP_SET_EVENT, EXECUTOR_LOCAL_HOST);
	#endif
	storage_add_afcz_record(SET_DEFEND, EXECUTOR_REMOTE_DEVICE);	// д���������¼�
	sys_set_hint_state(SYS_HINT_ALARM_STATE, TRUE);	
}

/*************************************************
  Function:			alarm_unset_response
  Description:		�Գ��������¼������Ӧ��
  Input: 			��
  Output:			��
  Return:			��
  Others:			��
*************************************************/
void alarm_unset_response(void)
{
	stop_alarm_timer();
	g_alarm_delay_flag = 0;
	g_resume_delay_flag = 0;
	g_exit_time = 0;
	g_delay_time = 0;
	g_single_defend = 0;							// ԭ�����������ָ�ԭ״̬
	// ��ʱ����ʱ��ֹͣ����������ֹͣ���������ݵĻָ�
	gpAfParam->defend_state = DIS_DEFEND;
	set_area_show_info(1);

	g_sound_type = NO_SOUND_TYPE;
	alarm_gui_callback(BAOJING_FRESH, 0);			// ˢ�½���
	g_alarm_time = 0;

	storage_add_afcz_record(DIS_DEFEND, EXECUTOR_REMOTE_DEVICE);	// д���������¼�
	if (0 == g_alarming_flag)
	{
		sys_set_hint_state(SYS_HINT_ALARM_STATE, FALSE);// �رձ�����
	}

	#ifdef _ALARM_OPER_SYNC_
	sync_comm_event_send(CMD_RP_UNSET_EVENT, EXECUTOR_LOCAL_HOST);  //ͬ�����������ϱ�
	#endif

	g_alarming_flag = 0;
}

/*************************************************
  Function:			alarm_partset_response
  Description:		�Ծַ������¼������Ӧ��
  Input: 			��
  Output:			��
  Return:			��
  Others:			��
*************************************************/
void alarm_partset_response(void)
{
	gpAfParam->defend_state = PART_DEFEND;
	alarm_clear_alerts_operator();
	g_alarm_delay_flag = 0;							// ֻ���ڲ����ϱ�����״̬����������������
	g_resume_delay_flag = 0;

	alarm_gui_callback(BAOJING_FRESH, 0);			                  // ˢ�½���
	storage_add_afcz_record(PART_DEFEND, EXECUTOR_REMOTE_DEVICE);	  // д���������¼�
	sys_set_hint_state(SYS_HINT_ALARM_STATE, TRUE);	                  // �򿪱�����
	#ifdef _ALARM_OPER_SYNC_
	sync_comm_event_send(CMD_RP_PARTSET_EVENT, EXECUTOR_LOCAL_HOST);  // �ַ��¼�ͬ���ϱ�
	#endif
}
#endif

/*************************************************
  Function:			port_status_callback
  Description: 		����״̬�ص�,��Ӳ�������лص�
  Input: 
	1.rcvbuf		����״̬,һ��λ����һ������
  Output:			��
  Return:			��
  Others:			
*************************************************/
void port_status_callback(uint8 PortLevel)
{
	uint8 i, area_num = AREA_AMOUNT;
	uint8 touch;									// ��¼��������
	uint8 is_alarm = 0;
	uint8 un_use = 0xFF;							//���ڴ�������δ���õ�״̬����
	g_Alarm_No24clear = 0;							
	g_Alarm_No24touch = 0;                          

	// 2011-10-12 ����Ϊ��8����������ʹ��ֻ֧�ֳ���,����������.һֱ��סֻ��һ��
	if (linkage_8area_door() == TRUE)
	{
		area_num = AREA_AMOUNT - 1;
		if ((PortLevel & 0x80) == 0)
		{
			if (!g_door_ring)
			{
				g_door_ring = 1;					// ��ǰ�尴��ʱ������
				linkage_play_door_ring();
			}
		}
		else
		{
			g_door_ring = 0;						// ��ǰ��̧��
		}
	}
	// yanjl 2014-6-24 ���ӵڰ˷�����������
	#ifdef _LINKAGE_78AREA_
	else if (linkage_8area_unset() == TRUE )
	{
		area_num = AREA_AMOUNT - 1;
		if ((PortLevel & 0x80) == 0)
		{
			if (!g_8area_unset)
			{
				g_8area_unset = 1;
				alarm_unset_operator(EXECUTOR_LOCAL_HOST,0);
			}	
		}
		else
		{
			g_8area_unset = 0;
		}
	}
	#endif
	else
	{
		area_num = AREA_AMOUNT;
	}
	// yanjl 2014-6-24 ���ӵ��߷�����������
	#ifdef _LINKAGE_78AREA_
	if (linkage_7area_set() == TRUE )
	{
		area_num = AREA_AMOUNT - 2;
		if ((PortLevel & 0x40) == 0 )
		{
			if (!g_7area_set)   
			{
				g_7area_set = 1;
				alarm_set_operator(EXECUTOR_LOCAL_HOST);
				alarm_gui_callback(BAOJING_FRESH,0);// ˢ�½���
			}				
		}
		else
		{
			g_7area_set = 0;
		}
	}
	#endif
	
	if (FALSE == storage_get_extmode(EXT_MODE_ALARM))
	{
		return;
	}
	
	touch = PortLevel ^ gpAfParam->finder_state;	// ��̽ͷ������ͬ״̬������
	dprintf("PortLevel:%x touch:%x\n", PortLevel, touch);
	// ����δ���ú���ʱ�������
	touch = touch & (gpAfParam->enable) & (~gpAfParam->isolation);
	// δ���ú͸���ķ����ı�״̬ʱ������
	un_use = un_use & (gpAfParam->enable) & (~gpAfParam->isolation);					// add by wufn 2015.9.15
	g_judge_can_defend = touch;

	// ��һ�����Ĵ���
	if (1 == (touch & 0x01))
	{
		// modi by luofl 2013-09-03���2-8������������1�����������ϱ�����
		if ((0 == g_sos_state) && (0 == (g_alarming_flag&0x01)))
		{
			g_sos_time = SOS_TIME;
			g_sos_state = 1;
			start_alarm_timer();
		}
	}
	else
	{
		g_alarming_flag &= 0xFE;
		g_sos_state = 0;
	}

	// �����˷�������
	//for (i = 1; i < AREA_AMOUNT; i++)
	for (i = 1; i < area_num; i++)
	{
		if (1 == ((touch>>i) & 0x01))
		{
			// �Ѿ�����������Ԥ��,���ظ�����
			if (1 == ((g_alarming_flag>>i) & 0x01) || (1 == ((g_alarm_delay_flag>>i) & 0x01)))
			{
				touch &= ~(1<<i);
				if (1 == ((g_alarm_delay_flag>>i) & 0x01))
				{
					// ��ʱ�����������ų����ٴδ������
					g_resume_delay_flag &= ~(1<<i);
				}
				continue;
			}
			// ����Ԥ������
			if (gpAfParam->delay_time[i] > 0 && (0 == ((g_alarm_valid_flag>>i) & 0x01)))
			{
				// �ַ���Ч�ķ�����Ԥ��������
				if ((PART_DEFEND == gpAfParam->defend_state) && (0 ==((gpAfParam->part_valid>>i)&0x01)))
				{
					touch &= ~(1<<i);
					continue;
				}
				//  �����ķ���������
				else if ((DIS_DEFEND == gpAfParam->defend_state) && (0 ==((g_single_defend>>i)&0x01)))
				{
					// ���ӳ���״̬���� ˢ����˸  add by wufn
					gpAfParam->show_state[i] = ALARM_SHOW_STATE;
					//if(0 == ((g_Alarm_Unset>>i) & 0x01))					// ��ֹ������ȡ״̬ʱ�����˸״̬���ı�
					{
						g_Alarm_No24touch |= (1<<i);
					}
					touch &= ~(1<<i);
					continue;
				}
				g_alarm_delay_flag |= 1<<i;			// �÷���Ԥ����־
			}
			// ���뱨������
			else
			{
				if (1 == ((g_alarm_valid_flag>>i) & 0x01))
				{
					g_alarming_flag |= (1<<i);		// �÷���������־
					g_Alarm_No24clear &= ~(1<<i);     // �������ʾ��־��Ϊ0
				}
				else
				{	
					// ���ӳ���״̬���� ˢ����˸  add by wufn
					gpAfParam->show_state[i] = ALARM_SHOW_STATE;
					//if(0 == ((g_Alarm_Unset>>i) & 0x01))					// ��ֹ������ȡ״̬ʱ�����˸״̬���ı�
					{
						g_Alarm_No24touch |= (1<<i);
					}
					continue;
				}
			}
			is_alarm = 1;							// ���µķ�������
		}
		else
		{
			//������״̬�ָ�  add by wufn
			if (DIS_DEFEND == gpAfParam->defend_state)
			{
				if (1 == ((un_use>>i) & 0x01))
				{
					if(0 == ((gpAfParam->is_24_hour>>i)&0x01))
					{
						if (0 == ((g_single_defend>>i) & 0x01))
						{
							gpAfParam->show_state[i] = NORM_SHOW_STATE;
						}
					}
				}
			}

			g_alarming_flag &= ~(1<<i);				// �������������־

			// ������ʱ��������, ��Ԥ�����̽ͷ״̬�ָ������Ԥ����־
			if ((1 == ((g_alarm_delay_flag>>i) & 0x01)) && gpAfParam->delay_time[i] > 0)
			{
				// ֻ���˳�Ԥ��ʱ���ڻָ����Ų�������ʱ����������ֻ��ͨ����������ȡ����ʱ����
				if (g_exit_time > 0)
				{
					if (0 == g_alarm_delay_flag)
					{
						g_delay_time = 0;					
					}
					g_alarm_delay_flag &= ~(1<<i);
				}
				else 
				{
					g_resume_delay_flag |= (1<<i);
					dprintf("g_resume_delay_flag:%x\n",g_resume_delay_flag);
				}
				// 10-7-21,�޸�ΪԤ����ֻ�ڳ���ʱ���ſ�ֹͣ
				#if 0
				if (0 == g_alarm_delay_flag)
				{
					g_delay_time = 0;					
				}
				#endif
			}
		}
	}

	if (1 == is_alarm)
	{
		alarm_deal(AS_ALARM_DEAL_MSGID, touch);
	}
	//g_Alarm_Unset = 0; 
	alarm_gui_callback(BAOJING_FRESH, 0);
}

/*************************************************
  Function:			deal_alarm_func
  Description: 		�������¼�
  Input: 
	1.num			������
  Output:			��
  Return:			��
  Others:			��
*************************************************/
static void deal_alarm_func(uint8 num)
{
	storage_add_afbj_record(num+1, 1);				// ��¼�����¼�
	if (0 == num)
	{
		sos_alarm_report();							// �ϱ���������¼�
		// add by luofl 2014-03-10��һ����д��һ����ҲҪ��˸
		if (gpAfParam->b_write)
		{
			return;
		}
	}
	else
	{
		break_alarm_report(num+1);					// �ϱ����������¼�
	}
	
	g_whole_alarm_state = 1;
	gpAfParam->show_state[num] = ALARM_SHOW_STATE;
	SaveRegInfo();

	sys_set_hint_state(SYS_HINT_ALARM_WARNING, 1);	// ��������

	if (1 == ((gpAfParam->can_hear>>num) & 0x01))	// ����
	{
		if((1 == g_can_hear>>num)& 0x01)
		{
			g_can_hear &= ~(1<<num);
		}
		sound_arbistrator(ALARM_SOUND_TYPE, gpAfParam->alarm_time, num);
	}
	else
	{
		if((0 == g_can_hear>>num)& 0x01)
		{
			g_can_hear |= (1<<num);//������
		}
		sound_arbistrator(ALARM_SOUND_TYPE, gpAfParam->alarm_time, num);
	}
	
	if (1 == ((gpAfParam->can_see>>num) & 0x01))	// �ɼ�
	{
		alarm_gui_callback(BAOJING_SHOW, 0);
	}
	else
	{
		alarm_gui_callback(BAOJING_FRESH, 0);
	}
}

/*************************************************
  Function:			alarm_logic_init
  Description:		�����߼���ʼ��
  Input: 		
  Output:			��
  Return:			��
  Others:			��
*************************************************/
void alarm_logic_init(void)
{
	#ifdef _DOOR_PHONE_
	init_alarm_callback(port_status_callback);		// ��ʼ��Ӳ������״̬�ص�
	#else
	init_ioctrl_callback(port_status_callback);
	#endif
	set_area_show_info(0);
}

/*************************************************
  Function:    		alarm_init_gui_callback
  Description: 		����GUI �ص�����
  Input:
  	1.func			�ص�����
  Output:    		��
  Return:			��
  Others:           
*************************************************/
void alarm_init_gui_callback(ALARMGUI_CALLBACK func, SHOW_SYSEVENHIT func1)
{	
	g_alarm_callback = func;	
	g_show_sysEvenHit = func1;
}

/*************************************************
  Function:    		logic_get_area_isolation
  Description: 		��ȡ�����������
  Input:
  Output:   
  Return:			��ʱ�������
  Others:           
*************************************************/
uint8 logic_get_area_isolation(void)
{
	return gpAfParam->isolation;
}

/*************************************************
  Function:    		logic_set_area_isolation
  Description: 		���÷�����ʱ�������
  Input:
  	1.iso_info		1- ��ʱ����
  Output:    		
  Return:			false:ʧ�� true:�ɹ�
  Others:           ÿһλ����һ������
*************************************************/
uint8 logic_set_area_isolation(uint8 iso_info)
{
	gpAfParam->isolation = iso_info;
	// 10-7-21���޸�Ϊ���ϱ�
	bypass_event_report(8, change_byte_order(iso_info));
	return set_area_show_info(1);
}

/*************************************************
  Function:    		logic_get_part_valid
  Description: 		��ȡ�����ַ���Ч����
  Input:
  Output:   
  Return:			�ַ���Ч����	
  Others:           
*************************************************/
uint8 logic_get_part_valid(void)
{
	return gpAfParam->part_valid;
}

/*************************************************
  Function:    		logic_set_part_valid
  Description: 		���÷����ַ���Ч����
  Input:
  	1.valid			�ַ���Ч����
  Output:    		
  Return:			false:ʧ�� true:�ɹ�
  Others:           
*************************************************/
uint8 logic_set_part_valid(uint8 valid)
{
	if (gpAfParam->is_24_hour != (gpAfParam->is_24_hour & valid))
	{
		return 2;
	}
	
	gpAfParam->part_valid = valid;
 	return set_area_show_info(1);
}

/*************************************************
  Function:    		logic_get_alarm_param
  Description: 		��ð�������
  Input:
  Output:    		
  	1.param_info	��������
  Return:				
  Others:    		       
*************************************************/
void logic_get_alarm_param(uint8 * param_info)
{
	if (NULL == param_info)
	{
		return;
	}

	memcpy(param_info, (uint8 *)(gpAfParam), sizeof(AF_FLASH_DATA));
}

/*************************************************
  Function:    		logic_set_alarm_param
  Description: 		���ð�������
  Input:
   	1.param_info	��������
  Output:    		
  Return:			false:ʧ�� true:�ɹ�				
  Others:           
*************************************************/
uint8 logic_set_alarm_param(uint8 *param_info)
{
	if (NULL == param_info)
	{
		return FALSE;
	}
	
	memcpy((uint8*)(gpAfParam), param_info, sizeof(AF_FLASH_DATA));
	alarm_define_report();
	return set_area_show_info(1);
}

/*************************************************
  Function:			alarm_deal
  Description: 		����������
  Output:			��
  Return:			��
  Others:			��
*************************************************/
void alarm_deal(SYS_MSG_ID id, uint8 msg)
{
	uint8 touch, i;
	touch = msg;
	
	switch(id)
	{
		case AS_ALARM_DEAL_MSGID:
			for (i = 1; i < AREA_AMOUNT; i++)
			{
				if (1 == ((touch>>i)&0x01))
				{
					// ����Ԥ������
					if (1 == ((g_alarm_delay_flag>>i) & 0x01))
					{
						sound_arbistrator(IN_SOUND_TYPE, gpAfParam->delay_time[i], 0);
					}
					// ���뱨������
					else
					{
						if (1 == ((g_alarming_flag>>i) & 0x01))
						{
							alarm_deal(AS_ALARM_PROC, i);
						}
					}
				}
			}
			break;

		case AS_ALARM_PROC:
			deal_alarm_func(touch);
			break;
			
		default:
			break;
	}
}

/*************************************************
  Function:    		alarm_get_24clear_param  
  Description: 		��÷�24Сʱ����������־
  Input:
  Output:    		
  	1.param	��24Сʱ����������־����
  Return:				
  Others:    add by wufn		       
*************************************************/
uint8 alarm_get_24clear_param(void)
{
	return g_Alarm_No24clear;
}

/*************************************************
  Function:    		alarm_get_24clear_param  
  Description: 		��÷�24Сʱ��������
  Input:
  Output:    		
  	1.param	��24Сʱ����������־����
  Return:				
  Others:    add by wufn		       
*************************************************/
uint8 alarm_get_no24touch_param(void)
{
	return g_Alarm_No24touch;
}

/*************************************************
  Function:    	logic_get_g_alarming_flag  
  Description: 		��ȡ����������־
  Input:
  Output:    		
  	1.param	����������־
  Return:				
  Others:    add by wufn		       
*************************************************/
uint8 logic_get_g_whole_alarm_state_param(void)
{
	return 	g_whole_alarm_state;
}

#if 0
/*************************************************
  Function:			alarm_set_unset
  Description:		���ó���������˸������־
  Input: 
  Output:			��
  Return:			��
  Others:			add by wufn	
*************************************************/
void alarm_set_unset(void)
{
	uint8 i;
	uint8 state;
	
	state = gpAfParam->is_24_hour ;
	for (i = 0; i < AREA_AMOUNT; i++)
	{
		if(gpAfParam->show_state[i] == ALARM_SHOW_STATE)
		{
			if (0 == ((state>>i)&0x01))		
			{
				if((1 == ((g_single_defend>>i) & 0x01)))
				{
					g_Alarm_Unset |= (1<<i);
				}
				else
				{
					g_Alarm_Unset &= ~(1<<i);
				}
			}
		}
	}
}
#endif

