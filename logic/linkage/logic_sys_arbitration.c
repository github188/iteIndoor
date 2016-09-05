/*************************************************
  Copyright (C), 2006-2010, Aurine
  File name: 	logic_sys_arbitration.c
  Author:    	xiewr
  Version:   	1.0
  Date: 
  Description:  ϵͳ�ٲ�:����ý���ϵͳ��ʾ
				
  History:        
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#include "logic_include.h"


static SYS_MEDIA_TYPE g_CurMediaType;
static MediaPlayCallback g_ProgressProc;
static MediaStopedCallback g_StoppedProc;
static uint8 g_isRepeat;
static uint8 g_ViewPict_State = 0;

static uint8 g_sysHintsNum[SYS_HINT_MAX_NUM];	// ��Ϣ��¼δ������
static uint8 g_sysHints[SYS_HINT_MAX_NUM];
SYS_LCD_STATE g_LcdState = SYS_LCD_CLOSE;
static 	pthread_mutex_t g_MediaLock;
static uint32 g_RecordTimer = 0;				// ��������¼�ƶ�ʱ��
static uint32 g_RecordTimeOut = 0;				// ��������¼��ʱ��
static uint32 g_RecordStartTime = 0;			// ��������¼�ƿ�ʼʱ��
static LYLY_TYPE g_LylyMode = LYLY_TYPE_VIDEO;	// ������Ӱ����ģʽ
static uint8 g_NoDeal = FALSE;

/*************************************************
  Function:			set_curplay_state
  Description: 		���õ�ǰý�岥��״̬
  Input: 		
  	1.MediaType		ý������
  	2.ProgressProc	���Ȼص�����
  	3.StoppedProc	��ֹͣʱ�ص�����
  Output:			��
  Return:			��
  Others:			
*************************************************/
static void set_curplay_state(SYS_MEDIA_TYPE MediaType, MediaPlayCallback ProgressProc, MediaStopedCallback StoppedProc, uint8 isRepeat)
{
	g_CurMediaType = MediaType;
	dprintf("g_CurMediaType======%d : \n",g_CurMediaType);
	g_ProgressProc = ProgressProc;
	g_StoppedProc = StoppedProc;
	g_isRepeat = isRepeat;

	if (g_CurMediaType == SYS_MEDIA_NONE || 
		g_CurMediaType == SYS_MEDIA_FAMILY_RECORD)
	{
		dprintf("set_curplay_state : spk off\n");
		hw_speak_off();
	}
	else
	{
		dprintf("set_curplay_state : spk on\n");
		hw_speak_on();
	}
}

/*************************************************
  Function:			arbi_stop_media
  Description: 		ֹͣý�����
  Input: 		
  	1.MediaType		ý������
  Output:			��
  Return:			0-�ɹ� ��0-ʧ��
  Others:			
*************************************************/
static int32 arbi_stop_media(SYS_MEDIA_TYPE MediaType)
{
	switch (MediaType)
	{		
		case SYS_MEDIA_INTERCOM:
			{
				// ֹͣ�Խ�
				inter_hand_down();
				// mody by chenbh ����Խ�ת������������������
				usleep(500*1000);
			}
			break;
			
		case SYS_MEDIA_MONITOR:
			// ֹͣ����
			if (rtsp_monitor_state())
			{
				dprintf("SYS_MEDIA_MONITOR rtsp\n");
				rtsp_monitor_stop();
				usleep(1000*1000);
			}
			else
			{
				dprintf("SYS_MEDIA_MONITOR monitor\n");
				monitor_stop();
				// ����ת���� �ӳٵȴ������߳��˳�
				usleep(1000*1000);
			}
			break;
			
		case SYS_MEDIA_YUJING:
		case SYS_MEDIA_OPER_HINT:
		case SYS_MEDIA_MUSIC:
		case SYS_MEDIA_ALARM:
			// ֹͣ���������ļ�
			media_stop_sound();
			dprintf("arbi_stop_media  arbi_stop_media\n");
			break;
			
		case SYS_MEDIA_FAMILY_RECORD:
			{
				#if 0
				// add by chenbh ɾ����ʱ��
				if (0 != g_RecordTimer)
				{
					cancel_aurine_timer(&g_RecordTimer, NULL);
					g_RecordTimer = 0;
				}
				#endif
				// ֹͣ��������¼��
				media_stop_local_record();
			}
			break;
			
		case SYS_MEDIA_FAMILY_AUDITION:		// ���ż�������ý������ SYS_MEDIA_MUSIC
			// ֹͣ������������
			break;
			
		case SYS_MEDIA_LEAVEWORD_PLAY:
			// ֹͣ������Ӱ����
			if (LYLY_TYPE_VIDEO == g_LylyMode)
			{
				media_stop_video_lyly();	
			}
			else
			{
				media_stop_sound();
			}
			break;
			
		case SYS_MEDIA_KEY:
			// ֹͣ���Ű�����
			break;	
		
		case SYS_MEDIA_NONE:
			break;
			
		default:	
			return SYS_MEDIA_ECHO_ERR;
	}
	if (g_StoppedProc)
	{
		g_StoppedProc();
	}
	return SYS_MEDIA_ECHO_OK;
}

/*************************************************
  Function:			arbi_play_alarm
  Description: 		�ж��Ƿ���Խ����챨��������
  Input: 		
  Output:			��
  Return:			0-���� ��0-����
  Others:			
*************************************************/
static int32 arbi_play_alarm(void)
{
	SYS_MEDIA_TYPE MediaType = sys_get_media_state();
	dprintf("MediaType = %d \n", MediaType);
	//switch (g_CurMediaType)
	switch (MediaType)
	{
		case SYS_MEDIA_ALARM:
		case SYS_MEDIA_INTERCOM:
		case SYS_MEDIA_MONITOR:
		case SYS_MEDIA_YUJING:
		case SYS_MEDIA_OPER_HINT:
		case SYS_MEDIA_MUSIC:
		case SYS_MEDIA_FAMILY_RECORD:
		case SYS_MEDIA_FAMILY_AUDITION:
		case SYS_MEDIA_LEAVEWORD_PLAY:
		case SYS_MEDIA_KEY:
			arbi_stop_media(MediaType);
			if (SYS_MEDIA_INTERCOM == MediaType ||
				SYS_MEDIA_MONITOR == MediaType)
			{
				sleep(1);
			}
			//arbi_stop_media(g_CurMediaType);
			break;
		case SYS_MEDIA_NONE:
			break;
		default:
			return SYS_MEDIA_ECHO_ERR;
	}

	return SYS_MEDIA_ECHO_OK;
}

/*************************************************
  Function:			arbi_oper_intercomm
  Description: 		�ж��Ƿ���Խ��жԽ�
  Input: 		
  Output:			��
  Return:			0-���� ��0-����
  Others:			
*************************************************/
static int32 arbi_oper_intercomm(void)
{
	SYS_MEDIA_TYPE MediaType = sys_get_media_state();
	switch (MediaType)
	{
		case SYS_MEDIA_ALARM:
		case SYS_MEDIA_INTERCOM:
			return MediaType;
		case SYS_MEDIA_MONITOR:
		case SYS_MEDIA_YUJING:
		case SYS_MEDIA_OPER_HINT:
		case SYS_MEDIA_MUSIC:
		case SYS_MEDIA_FAMILY_RECORD:
		case SYS_MEDIA_FAMILY_AUDITION:
		case SYS_MEDIA_LEAVEWORD_PLAY:
		case SYS_MEDIA_KEY:
			arbi_stop_media(MediaType);
			break;
		case SYS_MEDIA_NONE:
			break;
		default:
			return SYS_MEDIA_ECHO_ERR;
	}
	
	return SYS_MEDIA_ECHO_OK;
}

/*************************************************
  Function:			arbi_oper_monitor
  Description: 		�ж��Ƿ���Խ��м���
  Input: 		
  Output:			��
  Return:			0-���� ��0-����
  Others:			
*************************************************/
static int32 arbi_oper_monitor(void)
{
	SYS_MEDIA_TYPE MediaType = sys_get_media_state();
	switch (MediaType)
	{
		case SYS_MEDIA_ALARM:
		case SYS_MEDIA_INTERCOM:
		case SYS_MEDIA_MONITOR:	
			return MediaType;
		case SYS_MEDIA_YUJING:
		case SYS_MEDIA_OPER_HINT:
		case SYS_MEDIA_MUSIC:
		case SYS_MEDIA_FAMILY_RECORD:
		case SYS_MEDIA_FAMILY_AUDITION: 
		case SYS_MEDIA_LEAVEWORD_PLAY: 
		case SYS_MEDIA_KEY:
			arbi_stop_media(MediaType);
			break;
		case SYS_MEDIA_NONE:
			break;
		default:
			return SYS_MEDIA_ECHO_ERR;
	}	

	return SYS_MEDIA_ECHO_OK;
}

/*************************************************
  Function:			arbi_play_music
  Description: 		�ж��Ƿ���Բ�������
  Input: 		
  Output:			��
  Return:			0-���� ��0-����
  Others:			
*************************************************/
static int32 arbi_play_music(void)
{
	SYS_MEDIA_TYPE MediaType = sys_get_media_state();
	dprintf("=======MediaType :%d=============\n",MediaType);
	switch (MediaType)
	{
		case SYS_MEDIA_ALARM:
		case SYS_MEDIA_INTERCOM:
		case SYS_MEDIA_MONITOR:
		case SYS_MEDIA_FAMILY_RECORD:
		case SYS_MEDIA_FAMILY_AUDITION:
		case SYS_MEDIA_LEAVEWORD_PLAY:
			return MediaType;
		case SYS_MEDIA_YUJING:
		case SYS_MEDIA_OPER_HINT:
		case SYS_MEDIA_MUSIC:
		case SYS_MEDIA_KEY:
			arbi_stop_media(MediaType);
			break;	
		case SYS_MEDIA_NONE:
			break;
		default:
			return SYS_MEDIA_ECHO_ERR;
	}
	return SYS_MEDIA_ECHO_OK;
}

/*************************************************
  Function:			arbi_oper_family_record
  Description: 		�ж��Ƿ���Լ�������¼��
  Input: 		
  Output:			��
  Return:			0-���� ��0-����
  Others:			
*************************************************/
static int32 arbi_oper_family_record(void)
{
	SYS_MEDIA_TYPE MediaType = sys_get_media_state();
	switch (MediaType)
	{
		case SYS_MEDIA_ALARM:
		case SYS_MEDIA_INTERCOM:
		case SYS_MEDIA_MONITOR:
		case SYS_MEDIA_FAMILY_AUDITION:
			return MediaType;
		case SYS_MEDIA_FAMILY_RECORD:
			return SYS_MEDIA_ECHO_OK;
		case SYS_MEDIA_YUJING:
		case SYS_MEDIA_OPER_HINT:
		case SYS_MEDIA_MUSIC: 
		case SYS_MEDIA_LEAVEWORD_PLAY:
 		case SYS_MEDIA_KEY:
 			arbi_stop_media(MediaType);
 			break;
		case SYS_MEDIA_NONE:
			break;
		default:
			return SYS_MEDIA_ECHO_ERR;
 	}	
	return SYS_MEDIA_ECHO_OK;
}

/*************************************************
  Function:			arbi_oper_family_audition
  Description: 		�ж��Ƿ���Լ�����������
  Input: 		
  Output:			��
  Return:			0-���� ��0-����
  Others:			
*************************************************/
static int32 arbi_oper_family_audition(void)
{
	SYS_MEDIA_TYPE MediaType = sys_get_media_state();
	switch (MediaType)
	{
		case SYS_MEDIA_ALARM:
		case SYS_MEDIA_INTERCOM:
		case SYS_MEDIA_MONITOR:
		case SYS_MEDIA_FAMILY_RECORD:
		case SYS_MEDIA_FAMILY_AUDITION:
			return MediaType;
		case SYS_MEDIA_YUJING:
		case SYS_MEDIA_OPER_HINT:
		case SYS_MEDIA_MUSIC:
		case SYS_MEDIA_LEAVEWORD_PLAY:
 		case SYS_MEDIA_KEY:
 			arbi_stop_media(MediaType);
 			break;	
		case SYS_MEDIA_NONE:
			break;
		default:
			return SYS_MEDIA_ECHO_ERR;
	}	
	return SYS_MEDIA_ECHO_OK;
}

/*************************************************
  Function:			arbi_play_leaveword
  Description: 		�ж��Ƿ���Բ�����Ӱ����
  Input: 		
  Output:			��
  Return:			0-���� ��0-����
  Others:			
*************************************************/
static int32 arbi_play_leaveword(void)
{
	SYS_MEDIA_TYPE MediaType = sys_get_media_state();
	switch (MediaType)
	{
		case SYS_MEDIA_ALARM:
		case SYS_MEDIA_INTERCOM:
		case SYS_MEDIA_MONITOR:
		case SYS_MEDIA_FAMILY_RECORD:
		case SYS_MEDIA_FAMILY_AUDITION:
		case SYS_MEDIA_LEAVEWORD_PLAY:
			return MediaType;
			
		case SYS_MEDIA_YUJING:
		case SYS_MEDIA_OPER_HINT:
		case SYS_MEDIA_MUSIC: 
		case SYS_MEDIA_KEY:
			arbi_stop_media(MediaType);
			break;	
		case SYS_MEDIA_NONE:
			break;
		default:
			return SYS_MEDIA_ECHO_ERR;
	}	

	return SYS_MEDIA_ECHO_OK;
}

/*************************************************
  Function:			arbi_play_key
  Description: 		�ж��Ƿ���Բ��Ű�����
  Input: 		
  Output:			��
  Return:			0-���� ��0-����
  Others:			
*************************************************/
static int32 arbi_play_key(void)
{
	SYS_MEDIA_TYPE MediaType = sys_get_media_state();
	switch (MediaType)
	{
		case SYS_MEDIA_ALARM:
		case SYS_MEDIA_INTERCOM:
		case SYS_MEDIA_MONITOR:
		case SYS_MEDIA_FAMILY_RECORD:
		case SYS_MEDIA_FAMILY_AUDITION:
		case SYS_MEDIA_LEAVEWORD_PLAY:
		case SYS_MEDIA_YUJING:
		case SYS_MEDIA_OPER_HINT:
		case SYS_MEDIA_MUSIC:
		case SYS_MEDIA_KEY:
			return MediaType; 
		case SYS_MEDIA_NONE:
			break;
		default:
			return SYS_MEDIA_ECHO_ERR;			
	}	
	return SYS_MEDIA_ECHO_OK;
}

/*************************************************
  Function:			media_callback
  Description: 		��ý��ص�
  Input: 		
  	1.cmd 			������:0-err 1-process&time
  	2.time			ʱ��-��
  	3.percent		�ٷֱ�-0��100
  Output:			��
  Return:			��
  Others:			
*************************************************/
static void play_media_callback(int32 cmd, int32 time, int32 percent)
{
	if (cmd == 0)
	{
		// ����֪ͨ
		if (g_ProgressProc)
		{
			g_ProgressProc(0, 0, FALSE);
		}
		dprintf("play_media_callback : play err !");
		set_curplay_state(SYS_MEDIA_NONE, NULL, NULL, FALSE);
	}
	else
	{
		// ������ʾ
		if (g_ProgressProc)
		{
			g_ProgressProc(time, percent, TRUE);
		}
		
		if (percent == 100)
		{
			if (SYS_MEDIA_NONE != sys_get_media_state() && FALSE == g_isRepeat)
			{
				dprintf("g_NoDeal[%d] : play end !\n", g_NoDeal);
				if (g_NoDeal == FALSE)			// ��ֹg_ProgressProc ���ֵ�����ý�岿�ֽӿ� �˴���ý��״̬�ı���ɴ���
				{
					set_curplay_state(SYS_MEDIA_NONE, NULL, NULL, FALSE);
				}				
			}			
		}
		
		g_NoDeal = FALSE;
	}
}

/*************************************************
  Function:			on_alarm_timer
  Description:		��ʱ��
  Input: 			
  	1.type 			����������
  Output:			��
  Return:			��
  Others:
*************************************************/
static void* on_record_timer(uint32 ID, void * arg)
{
	if (0 != g_RecordTimer)
	{
		// ��ʱ�����洦��ҵ粿�� ��ʱ̫�ർ��ʱ�䲻׼ �����ԼӼ���ʱ��
		g_RecordTimeOut = time(NULL) - g_RecordStartTime;
		if (RECORD_TIME_MAX <= g_RecordTimeOut)
		{			
			if (g_ProgressProc)
			{
				g_ProgressProc(RECORD_TIME_MAX, 100, TRUE);
			}
			sys_stop_family_record();
		}
		else
		{
			if (g_ProgressProc)
			{
				g_ProgressProc(RECORD_TIME_MAX, ((g_RecordTimeOut*100)/RECORD_TIME_MAX), TRUE);
			}
		}
	}
}

/*************************************************
  Function:			sys_set_no_deal
  Description: 		
  Input: 			
  	1.flg			
  Output:			��
  Return:			��
  Others:			
*************************************************/
void sys_set_no_deal(uint8 flg)
{
	g_NoDeal = flg;
}

/*************************************************
  Function:			sys_stop_play_audio
  Description: 		ֹͣ������Ƶ�ļ�
  Input: 			
  	1.AudioType		���ֲ�������
  Output:			��
  Return:			��
  Others:			
*************************************************/
void sys_stop_play_audio(SYS_MEDIA_TYPE AudioType)
{	
	SYS_MEDIA_TYPE MediaType = sys_get_media_state();
	switch (AudioType)
	{
		case SYS_MEDIA_ALARM:
		case SYS_MEDIA_YUJING:
		case SYS_MEDIA_OPER_HINT:
		case SYS_MEDIA_MUSIC:
		case SYS_MEDIA_KEY:
			break;
		default:
			return;
	}
	
	if (AudioType != MediaType)
	{
		return;
	}
	
	arbi_stop_media(MediaType);
	set_curplay_state(SYS_MEDIA_NONE, NULL, NULL, FALSE);
}

/*************************************************
  Function:			sys_start_play_audio
  Description: 		��ʼ������Ƶ
  Input: 			
  	1.MediaType		ý���������
  	2.FileName		��Ҫ���ŵ�ý���ļ�·��
  	3.isRepeat		�Ƿ��ظ�����
  	4.value			���õ�����
  	5.ProgressProc	�������ص�����
  	6.StoppedProc	��ǿ����ֹ��ص�����
  Output:			��
  Return:			0-�ɹ� ��0-ʧ��
  Others:			
*************************************************/
int32 sys_start_play_audio(const SYS_MEDIA_TYPE MediaType, char *FileName, uint8 isRepeat, uint8 volume, 
							MediaPlayCallback ProgressProc, MediaStopedCallback StoppedProc)
{
	int32 ret = SYS_MEDIA_NONE; 
	
	dprintf(" 111 FileName: %s\n MediaType :%d  isRepeat :%d volume :%d\n", FileName, MediaType, isRepeat, volume);
	pthread_mutex_lock(&g_MediaLock);
	switch (MediaType)
	{
		case SYS_MEDIA_NONE:
			return SYS_MEDIA_ECHO_ERR;
			
		case SYS_MEDIA_ALARM:
			ret = arbi_play_alarm();
			break;
			
		case SYS_MEDIA_YUJING:
		case SYS_MEDIA_OPER_HINT:
		case SYS_MEDIA_MUSIC:
			ret = arbi_play_music();
			break;
			
		case SYS_MEDIA_KEY:
			ret = arbi_play_key();
			break;
			
		default:
			pthread_mutex_unlock(&g_MediaLock);
			return SYS_MEDIA_ECHO_ERR;
	}

	if (SYS_MEDIA_ECHO_OK != ret)
	{
		pthread_mutex_unlock(&g_MediaLock);
		return ret;
	} 

	if (FileName == NULL)
	{
		set_curplay_state(SYS_MEDIA_NONE, NULL, NULL, FALSE);
		pthread_mutex_unlock(&g_MediaLock);
		return SYS_MEDIA_ECHO_ERR;
	}
	// modi by luofl 2014-03-10 �ر�ý����ӳ�100ms����ֹý��û�ر���ȫ�ֿ���������
	usleep(100*1000);
	set_curplay_state(MediaType, ProgressProc, StoppedProc, isRepeat);

	// ��ʼ������Ƶ�ļ�
	media_set_ring_volume(volume);
	ret = media_play_sound(FileName, isRepeat, (void*)play_media_callback);
	if (ret == FALSE)	
	{
		set_curplay_state(SYS_MEDIA_NONE, NULL, NULL, FALSE);
		pthread_mutex_unlock(&g_MediaLock);
		return SYS_MEDIA_ECHO_ERR;
	}
	pthread_mutex_unlock(&g_MediaLock);
	return SYS_MEDIA_ECHO_OK;
}

/*************************************************
  Function:			sys_start_play_leaveword
  Description: 		��ʼ������Ӱ����
  Input: 			
  	1.FileName		�ļ���
  	2.vorect		��������
  	3.ProcessProc	���Ȼص�
  	4.StoppedProc	��ǿ����ֹ��ص�����
  Output:			��
  Return:			0-�ɹ� ��0-ʧ��
  Others:			
*************************************************/
int32 sys_start_play_leaveword(char *FileName, LYLY_TYPE type, uint8 volume, MediaPlayCallback ProcessProc, MediaStopedCallback StoppedProc)
{
	int32 ret = arbi_play_leaveword();
	if (SYS_MEDIA_ECHO_OK != ret)
	{
		return ret;
	}
	set_curplay_state(SYS_MEDIA_LEAVEWORD_PLAY, ProcessProc, StoppedProc, FALSE);

	// add by chenbh 2016-08-18 ������������
	media_set_ring_volume(volume);
	
	// ��ʼ������Ӱ����
	if (LYLY_TYPE_VIDEO == type)
	{
		ret = media_play_video_lyly(FileName, (void*)play_media_callback);
		if (ret == FALSE)		
		{
			set_curplay_state(SYS_MEDIA_NONE, NULL, NULL, FALSE);
			return SYS_MEDIA_ECHO_ERR;
		}
	}
	else	// ���Ŵ�����
	{
		ret = media_play_sound_lyly(FileName, 0, (void*)play_media_callback);
		if (ret == FALSE)
		{
			set_curplay_state(SYS_MEDIA_NONE, NULL, NULL, FALSE);
			return SYS_MEDIA_ECHO_ERR;
		}
	}

	g_LylyMode = type;
	return SYS_MEDIA_ECHO_OK;
}

/*************************************************
  Function:			sys_stop_play_leaveword
  Description: 		ֹͣ������Ӱ����
  Input: 			��
  Output:			��
  Return:			0-�ɹ� ��0-ʧ��
  Others:			
*************************************************/
int32 sys_stop_play_leaveword(void)
{
	SYS_MEDIA_TYPE MediaType = sys_get_media_state();
	if (MediaType != SYS_MEDIA_LEAVEWORD_PLAY)
	{
		return SYS_MEDIA_ECHO_ERR;
	}
	
	// ֹͣ������Ӱ����
	if (LYLY_TYPE_VIDEO == g_LylyMode)
	{
		media_stop_video_lyly();	
	}
	else
	{
		media_stop_sound_lyly();
	}
	set_curplay_state(SYS_MEDIA_NONE, NULL, NULL, FALSE);
	g_LylyMode = LYLY_TYPE_MAX;
	return SYS_MEDIA_ECHO_OK;
}

/*************************************************
  Function:			sys_start_family_record
  Description: 		��ʼ��������¼��
  Input: 			
  	1.FileName		�ļ�·��
  	2.ProcessProc	���Ȼص�����
  	3.StoppedProc	��ǿ����ֹ��ص�����
  Output:			��
  Return:			0-�ɹ� ��0-ʧ��
  Others:			
*************************************************/
int32 sys_start_family_record(char *FileName, MediaPlayCallback ProcessProc, MediaStopedCallback StoppedProc)
{
	int32 ret = arbi_oper_family_record();
	if (SYS_MEDIA_ECHO_OK != ret)
	{
		return ret;
	}
	set_curplay_state(SYS_MEDIA_FAMILY_RECORD, ProcessProc, StoppedProc, FALSE);

	// ��ʼ��������¼��
	//ret = media_start_local_record(FileName, RECORD_TIME_MAX, (void*)play_media_callback);
	ret = media_start_local_record(FileName);
	if (ret == FALSE)
	{
		set_curplay_state(SYS_MEDIA_NONE, NULL, NULL, FALSE);
		return SYS_MEDIA_ECHO_ERR;
	}
	#if 0
	else	// add by chenbh ���Ӷ�ʱ��
	{
		// modi by chenbh UI������ʱ �߼��Ͳ�����ʱ��
		#if 1	
		if (0 != g_RecordTimer)
		{
			cancel_aurine_timer(g_RecordTimer, NULL);
		}
		g_RecordStartTime = time(NULL);
		g_RecordTimer = 0;
		g_RecordTimer = add_aurine_realtimer(1000, on_record_timer, NULL);
		#endif				
	}
	#endif
	
	return SYS_MEDIA_ECHO_OK;
}

/*************************************************
  Function:			sys_stop_family_record
  Description: 		ֹͣ��������¼��
  Input: 			��
  Output:			��
  Return:			0-�ɹ� ��0-ʧ��
  Others:			
*************************************************/
int32 sys_stop_family_record(void)
{
	SYS_MEDIA_TYPE MediaType = sys_get_media_state();
	if (MediaType != SYS_MEDIA_FAMILY_RECORD)
	{
		return SYS_MEDIA_ECHO_ERR;
	}

	// modi by chenbh 20160822 ��ʱ����UI�㴦��
	#if 0
	// add by chenbh ɾ����ʱ��
	if (0 != g_RecordTimer)
	{
		cancel_aurine_timer(&g_RecordTimer, NULL);
		g_RecordTimer = 0;
	}
	#endif
	
	// ֹͣ��������¼��
	media_stop_local_record();
	set_curplay_state(SYS_MEDIA_NONE, NULL, NULL, FALSE);
	return SYS_MEDIA_ECHO_OK;
}

/*************************************************
  Function:			sys_set_intercomm_state
  Description: 		���õ�ǰ�Խ�״̬
  Input: 			
  	1.state			TRUE-���ڶԽ��� FALSE-�����ڶԽ���
  Output:			��
  Return:			0-�ɹ� ��0-ʧ��
  Others:			
*************************************************/
int32 sys_set_intercomm_state(uint8 state)
{
	int32 ret;
	pthread_mutex_lock(&g_MediaLock);
	SYS_MEDIA_TYPE MediaType = sys_get_media_state();
	if (state)
	{
		ret = arbi_oper_intercomm();
		if (ret != SYS_MEDIA_ECHO_OK)
		{
			pthread_mutex_unlock(&g_MediaLock);
			return ret;
		}
		else
		{
			set_curplay_state(SYS_MEDIA_INTERCOM, NULL, NULL, FALSE);
			pthread_mutex_unlock(&g_MediaLock);
			return SYS_MEDIA_ECHO_OK;
		}
	}
	else
	{
		if (MediaType != SYS_MEDIA_INTERCOM)
		{
			pthread_mutex_unlock(&g_MediaLock);
			return SYS_MEDIA_ECHO_ERR;
		}
		else
		{
			set_curplay_state(SYS_MEDIA_NONE, NULL, NULL, FALSE);
			pthread_mutex_unlock(&g_MediaLock);
			return SYS_MEDIA_ECHO_OK;
		}
	}
}

/*************************************************
  Function:			sys_set_monitor_state
  Description: 		���õ�ǰ����״̬
  Input: 			
  	1.state			TRUE-���ڼ����� FALSE-�����ڼ�����
  Output:			��
  Return:			0-�ɹ� ��0-ʧ��
  Others:			
*************************************************/
int32 sys_set_monitor_state(uint8 state)
{
	int32 ret;
	pthread_mutex_lock(&g_MediaLock);
	SYS_MEDIA_TYPE MediaType = sys_get_media_state();
	dprintf("sys_set_monitor_state  MediaType :%d  state: %d\n", MediaType, state);
	if (state)
	{
		ret = arbi_oper_monitor();
		if (ret != SYS_MEDIA_ECHO_OK)
		{
			pthread_mutex_unlock(&g_MediaLock);
			return ret;
		}
		else
		{
			set_curplay_state(SYS_MEDIA_MONITOR, NULL, NULL, FALSE);
			pthread_mutex_unlock(&g_MediaLock);
			return SYS_MEDIA_ECHO_OK;
		}
	}
	else
	{
		if (MediaType != SYS_MEDIA_MONITOR)
		{
			pthread_mutex_unlock(&g_MediaLock);
			return SYS_MEDIA_ECHO_ERR;
		}
		else
		{
			set_curplay_state(SYS_MEDIA_NONE, NULL, NULL, FALSE);
			pthread_mutex_unlock(&g_MediaLock);
			return SYS_MEDIA_ECHO_OK;
		}
	}
}

/*************************************************
  Function:			sys_get_media_state
  Description: 		��ȡϵͳý��״̬
  Input: 		
  Output:			��
  Return:			ϵͳý��״̬
  Others:			
*************************************************/
SYS_MEDIA_TYPE sys_get_media_state(void)
{
	return g_CurMediaType;
}

/*************************************************
  Function:			sys_stop_cur_media
  Description: 		ֹͣ��ǰý��
  Input: 		
  Output:			��
  Return:			
  Others:			
*************************************************/
void sys_stop_cur_media(void)
{
	SYS_MEDIA_TYPE CurState = sys_get_media_state();
	arbi_stop_media(CurState);
	set_curplay_state(SYS_MEDIA_NONE, NULL, NULL, FALSE);
}

/*************************************************
  Function:			sys_set_hint_state
  Description: 		����ϵͳ״̬
  Input: 		
  	1.HintType		ϵͳ״̬����
  	2.flg			��־: TRUE / FALSE
  Output:			��
  Return:			��
  Others:			
*************************************************/
void sys_set_hint_state(SYS_HINT_TYPE HintType, uint8 flg)
{
	uint8 i = 0;
	
	// ����ȫ�ֱ���
	g_sysHints[HintType] = flg;
	dprintf("sys_set_hint_state : HintType:%d, flg:%d\n", HintType, flg);

	#ifdef _CP2526_TOUCH_KEY_
	// �������������
	for (i = SYS_HINT_INFO; i < SYS_HINT_MISSED_CALLS+1; i++)
	{
		if (g_sysHints[i] == TRUE)
		{
			// �������˸
			dprintf("sys_set_hint_state : key led flash\n");
			hw_key_led_flash();
			return;
		}
	}

	if (g_sysHints[SYS_HINT_OPERATING] || g_sysHints[SYS_HINT_ALARM_STATE])
	{
		// ����Ƹ���
		dprintf("sys_set_hint_state : key led on\n");
		hw_key_led_on();
		return;
	}

	// �����΢��
	dprintf("sys_set_hint_state : key led off\n");
	hw_key_led_off();
	#else
	if (TRUE == g_sysHints[SYS_HINT_INFO])
	{
		hw_message_led_glint();
	}
	else
	{
		hw_message_led_off();
	}

	if (TRUE == g_sysHints[SYS_HINT_ALARM_WARNING])
	{
		hw_alarm_led_glint();
	}
	else if (TRUE == g_sysHints[SYS_HINT_ALARM_STATE])
	{
		hw_alarm_led_on();
	}
	else
	{
		hw_alarm_led_off();
	}
	#endif
	
	return;
}

/*************************************************
  Function:			sys_get_hint_state
  Description: 		���ϵͳ״̬
  Input: 		
  	1.HintType		ϵͳ״̬����
  Output:			��
  Return:			TRUE / FALSE
  Others:			
*************************************************/
uint8 sys_get_hint_state(SYS_HINT_TYPE HintType)
{
	if (HintType >= SYS_HINT_MAX_NUM || HintType < SYS_HINT_INFO)
	{
		return FALSE;
	}

	// ����ż�ʱ��ȡ״̬
	if (HintType == SYS_HINT_NO_DISTURB)
	{
		g_sysHints[HintType] = linkage_get_nodisturb_state();	
	}
	
	return g_sysHints[HintType];
}

/*************************************************
  Function:			sys_get_hint_num
  Description: 		���ÿ��״̬����Ϣ����
  Input: 		
  	1.HintType		ϵͳ״̬����
  Output:			��
  Return:			
  Others:			
*************************************************/
uint8 sys_get_hint_num(SYS_HINT_TYPE HintType)
{
	if (HintType >= SYS_HINT_MAX_NUM || HintType < SYS_HINT_INFO)
	{
		return FALSE;
	}

	// ����ż�ʱ��ȡ״̬
	if (SYS_HINT_NO_DISTURB == HintType)
	{
		g_sysHintsNum[HintType] = linkage_get_nodisturb_state();	
	}
	else if (SYS_HINT_ALARM_STATE == HintType)
	{
		g_sysHintsNum[HintType] = storage_get_alarm_state();
	}
	else if (SYS_HINT_ALARM_WARNING == HintType)
	{
		g_sysHintsNum[HintType] = storage_get_alarm_undeal_num();
	}
	
	return g_sysHintsNum[HintType];
}

/*************************************************
  Function:			sys_sync_hint_state
  Description: 		ͬ��ϵͳ״̬
  Input: 		
  Output:			��
  Return:			��
  Others:			
*************************************************/
void sys_sync_hint_state(void)
{
	uint8 flg, unread_num;
	
	// ��ȡ��Ϣδ��״̬
	unread_num = storage_get_msg_state();
	if (unread_num)
	{
		sys_set_hint_state(SYS_HINT_INFO, TRUE);
	}
	else
	{
		sys_set_hint_state(SYS_HINT_INFO, FALSE);
	}
	g_sysHintsNum[SYS_HINT_INFO] = unread_num;
	
	// ��ȡ��Ӱ����δ��״̬
	unread_num = storage_get_lylyrecord_flag();
	if (unread_num)
	{
		sys_set_hint_state(SYS_HINT_LEAVEWORD, TRUE);
	}
	else
	{
		sys_set_hint_state(SYS_HINT_LEAVEWORD, FALSE);
	}
	g_sysHintsNum[SYS_HINT_LEAVEWORD] = unread_num;
	
	// ��ȡ��������δ��״̬
	unread_num = storage_get_jrlyrecord_flag();
	if (unread_num)
	{
		sys_set_hint_state(SYS_HINT_FAMILY, TRUE);
	}
	else
	{
		sys_set_hint_state(SYS_HINT_FAMILY, FALSE);
	}
	g_sysHintsNum[SYS_HINT_FAMILY] = unread_num;
	
	// ��ȡδ������δ��״̬
	unread_num = storage_get_callrecord_state();
	if (unread_num)
	{
		sys_set_hint_state(SYS_HINT_MISSED_CALLS, TRUE);
	}
	else
	{
		sys_set_hint_state(SYS_HINT_MISSED_CALLS, FALSE);
	}
	g_sysHintsNum[SYS_HINT_MISSED_CALLS] = unread_num;

	// ��ʼ�������-�������
	flg = storage_get_noface();
	if (flg == TRUE)
	{
		sys_set_hint_state(SYS_HINT_NO_DISTURB, TRUE);
	}
	else
	{
		sys_set_hint_state(SYS_HINT_NO_DISTURB, FALSE);
	}
	g_sysHintsNum[SYS_HINT_NO_DISTURB] = flg;
}

/*************************************************
  Function:			sys_get_lcd_state
  Description: 		��Ļ״̬
  Input: 		
  Output:			��
  Return:			��
  Others:			
*************************************************/
SYS_LCD_STATE sys_get_lcd_state(void)
{
	return g_LcdState;
}

/*************************************************
  Function:			sys_open_lcd
  Description: 		��������
  Input: 		
  Output:			��
  Return:			��
  Others:			
*************************************************/
uint8 sys_open_lcd(void)
{
	#if 0
	if (SYS_LCD_CLOSE == g_LcdState)
	{
		//PMU_EnterModule(PMU_NORMAL);				// add by luofl 2011-08-16 ���ʹ�������
		hw_lcd_pwr_on();
		g_LcdState = SYS_LCD_OPEN;
		sys_set_hint_state(SYS_HINT_OPERATING, TRUE);
		//hw_spk_on();
		DelayMs_nops(150);
		BL_SetBright(storage_get_bright());
		return TRUE;
	}
	return FALSE;
	#else
	if (SYS_LCD_CLOSE == g_LcdState)
	{
		hw_lcd_power_on();							// ������Դ
		sys_set_hint_state(SYS_HINT_OPERATING, TRUE);
		g_LcdState = SYS_LCD_OPEN;
		usleep(200*1000);
		hw_lcd_back_on();							// ��������
		//hw_speak_on();							// ������
		return TRUE;
	}
	
	return FALSE;
	#endif
}

/*************************************************
  Function:			sys_close_lcd
  Description: 		��������
  Input: 		
  Output:			��
  Return:			��
  Others:			
*************************************************/
void sys_close_lcd(void)
{
	if (SYS_LCD_OPEN == g_LcdState)
	{
		hw_lcd_back_off();
		g_LcdState = SYS_LCD_CLOSE;
		sys_set_hint_state(SYS_HINT_OPERATING, FALSE);
		hw_speak_off();								// ������
		hw_lcd_power_off();
	}
}

/*************************************************
  Function:			sys_key_beep
  Description: 		������
  Input: 		
  Output:			��
  Return:			��
  Others:			
*************************************************/
void sys_key_beep(void)
{
	#if 0
	// add by luofl 2014-08-28�������Ļ�²��찴����
	if (GUID_EXE_SCREEN == ui_get_active_guid())
	{
		return;
	}
	#endif

	if (g_CurMediaType != SYS_MEDIA_NONE)
	{
		return;
	}
	
	if (TRUE == storage_get_keykeep())
	{
		hw_key_beep();
	}
}

/*************************************************
  Function:			sys_init_hint_state
  Description: 		ϵͳ�ϵ�ʱ��ʼ��ϵͳ״̬
  Input: 		
  Output:			��
  Return:			��
  Others:			
*************************************************/
void sys_init_hint_state(void)
{
	uint8 flg, unread_num;

	memset(g_sysHintsNum, 0, sizeof(g_sysHintsNum));
	pthread_mutex_init(&g_MediaLock, NULL);
	
	// ��ȡ��Ϣδ��״̬
	unread_num = storage_get_msg_state();
	if (unread_num)
	{
		sys_set_hint_state(SYS_HINT_INFO, TRUE);
	}
	else
	{
		sys_set_hint_state(SYS_HINT_INFO, FALSE);
	}
	g_sysHintsNum[SYS_HINT_INFO] = unread_num;

	// ��ȡ��Ӱ����δ��״̬
	unread_num = storage_get_lylyrecord_flag();
	if (unread_num)
	{
		sys_set_hint_state(SYS_HINT_LEAVEWORD, TRUE);
	}
	else
	{
		sys_set_hint_state(SYS_HINT_LEAVEWORD, FALSE);
	}
	g_sysHintsNum[SYS_HINT_LEAVEWORD] = unread_num;

	// ��ȡ��������δ��״̬
	unread_num = storage_get_jrlyrecord_flag();
	if (unread_num)
	{
		sys_set_hint_state(SYS_HINT_FAMILY, TRUE);
	}
	else
	{
		sys_set_hint_state(SYS_HINT_FAMILY, FALSE);
	}
	g_sysHintsNum[SYS_HINT_FAMILY] = unread_num;
	
	// ��ȡδ������δ��״̬
	unread_num = storage_get_callrecord_state();
	if (unread_num)
	{
		sys_set_hint_state(SYS_HINT_MISSED_CALLS, TRUE);
	}
	else
	{
		sys_set_hint_state(SYS_HINT_MISSED_CALLS, FALSE);
	}
	g_sysHintsNum[SYS_HINT_MISSED_CALLS] = unread_num;
	
	// ��ʼ�������-�������
	storage_set_noface_enable(FALSE);
	sys_set_hint_state(SYS_HINT_NO_DISTURB, FALSE);
	g_sysHintsNum[SYS_HINT_NO_DISTURB] = FALSE;
	
	// ��ȡ����״̬
	flg = storage_get_alarm_state();	
	if (flg == FALSE)						// ����
	{
		sys_set_hint_state(SYS_HINT_ALARM_STATE, FALSE);
	}	
	else									// ����
	{
		sys_set_hint_state(SYS_HINT_ALARM_STATE, TRUE);		
	}	
	g_sysHintsNum[SYS_HINT_ALARM_STATE] = flg;
	
	// ��ȡ����δ������
	unread_num = storage_get_alarm_undeal_num();
	if (unread_num)
	{
		sys_set_hint_state(SYS_HINT_ALARM_WARNING, TRUE);		
	}
	g_sysHintsNum[SYS_HINT_ALARM_WARNING] = unread_num;

	// ��ʼ��ϵͳΪ����״̬
	dprintf("sys_init_hint_state open lcd!\n");
	#ifdef _TIMER_REBOOT_NO_LCD_
	if (storage_get_isOpenScreen())
	{
		sys_open_lcd();		
	}
	else	
	{
		g_LcdState = SYS_LCD_OPEN;
		timer_reboot_control(FALSE);
	}
	#else
	sys_open_lcd();		
	#endif
}

