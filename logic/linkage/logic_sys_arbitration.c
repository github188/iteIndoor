/*************************************************
  Copyright (C), 2006-2010, Aurine
  File name: 	logic_sys_arbitration.c
  Author:    	xiewr
  Version:   	1.0
  Date: 
  Description:  系统仲裁:包括媒体和系统提示
				
  History:        
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#include "logic_include.h"


static SysHintRecord g_SysHintRecord;
static SYS_MEDIA_TYPE g_CurMediaType;
static MediaPlayCallback g_ProgressProc;
static MediaStopedCallback g_StoppedProc;
static uint8 g_isRepeat;
static uint8 g_ViewPict_State = 0;

static uint8 g_sysHintsNum[SYS_HINT_MAX_NUM];	// 消息记录未读条数
static 	pthread_mutex_t g_MediaLock;
static uint32 g_RecordTimer = 0;				// 家人留言录制定时器
static uint32 g_RecordTimeOut = 0;				// 家人留言录制时间
static uint32 g_RecordStartTime = 0;			// 家人留言录制开始时间
static LYLY_TYPE g_LylyMode = LYLY_TYPE_VIDEO;	// 播放留影留言模式
static uint8 g_NoDeal = FALSE;

/*************************************************
  Function:			set_curplay_state
  Description: 		设置当前媒体播放状态
  Input: 		
  	1.MediaType		媒体类型
  	2.ProgressProc	进度回调函数
  	3.StoppedProc	被停止时回调函数
  Output:			无
  Return:			无
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
  Description: 		停止媒体操作
  Input: 		
  	1.MediaType		媒体类型
  Output:			无
  Return:			0-成功 非0-失败
  Others:			
*************************************************/
static int32 arbi_stop_media(SYS_MEDIA_TYPE MediaType)
{
	int times;
	switch (MediaType)
	{		
		case SYS_MEDIA_INTERCOM:
			{
				// 停止对讲
				inter_hand_down();
				// mody by chenbh 解决对讲转报警声音出不来问题
				times = 30;
				while (times > 0)
				{
					if (call_get_media_ifover() == TRUE)
					{
						break;
					}
					usleep(100*1000);
					times--;
				}
			}
			break;
			
		case SYS_MEDIA_MONITOR:
			// 停止监视
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
				// 监视转呼叫 延迟等待监视线程退出
				times = 30;
				while (times > 0)
				{
					if (monitor_get_media_ifover() == TRUE)
					{
						break;
					}
					usleep(100*1000);
					times--;
				}				
			}
			break;
			
		case SYS_MEDIA_YUJING:
		case SYS_MEDIA_OPER_HINT:
		case SYS_MEDIA_MUSIC:
		case SYS_MEDIA_ALARM:
			// 停止播放声音文件
			media_stop_sound();
			dprintf("arbi_stop_media  arbi_stop_media\n");
			break;
			
		case SYS_MEDIA_FAMILY_RECORD:
			{
				#if 0
				// add by chenbh 删除定时器
				if (0 != g_RecordTimer)
				{
					cancel_aurine_timer(&g_RecordTimer, NULL);
					g_RecordTimer = 0;
				}
				#endif
				// 停止家人留言录制
				media_stop_local_record();
			}
			break;
			
		case SYS_MEDIA_FAMILY_AUDITION:		// 播放家人留言媒体类型 SYS_MEDIA_MUSIC
			// 停止家人留言试听
			break;
			
		case SYS_MEDIA_LEAVEWORD_PLAY:
			// 停止播放留影留言
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
			// 停止播放按键音
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
  Description: 		判断是否可以进行响报警声操作
  Input: 		
  Output:			无
  Return:			0-可以 非0-不行
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
				usleep(500*1000);
			}
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
  Description: 		判断是否可以进行对讲
  Input: 		
  Output:			无
  Return:			0-可以 非0-不行
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
  Description: 		判断是否可以进行监视
  Input: 		
  Output:			无
  Return:			0-可以 非0-不行
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
  Description: 		判断是否可以播放音乐
  Input: 		
  Output:			无
  Return:			0-可以 非0-不行
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
  Description: 		判断是否可以家人留言录制
  Input: 		
  Output:			无
  Return:			0-可以 非0-不行
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
  Description: 		判断是否可以家人留言试听
  Input: 		
  Output:			无
  Return:			0-可以 非0-不行
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
  Description: 		判断是否可以播放留影留言
  Input: 		
  Output:			无
  Return:			0-可以 非0-不行
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
  Description: 		判断是否可以播放按键音
  Input: 		
  Output:			无
  Return:			0-可以 非0-不行
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
  Description: 		多媒体回调
  Input: 		
  	1.cmd 			命令码:0-err 1-process&time
  	2.time			时间-秒
  	3.percent		百分比-0至100
  Output:			无
  Return:			无
  Others:			
*************************************************/
static void play_media_callback(int32 cmd, int32 time, int32 percent)
{
	if (cmd == 0)
	{
		// 出错通知
		if (g_ProgressProc)
		{
			g_ProgressProc(0, 0, FALSE);
		}
		dprintf("play_media_callback : play err !");
		set_curplay_state(SYS_MEDIA_NONE, NULL, NULL, FALSE);
	}
	else
	{
		// 进度提示
		if (g_ProgressProc)
		{
			g_ProgressProc(time, percent, TRUE);
		}
		
		if (percent == 100)
		{
			if (SYS_MEDIA_NONE != sys_get_media_state() && FALSE == g_isRepeat)
			{
				dprintf("g_NoDeal[%d] : play end !\n", g_NoDeal);
				if (g_NoDeal == FALSE)			// 防止g_ProgressProc 中又调用了媒体部分接口 此处把媒体状态改变造成错误
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
  Description:		定时器
  Input: 			
  	1.type 			声音的类型
  Output:			无
  Return:			无
  Others:
*************************************************/
static void* on_record_timer(uint32 ID, void * arg)
{
	if (0 != g_RecordTimer)
	{
		// 定时器里面处理家电部分 耗时太多导致时间不准 不用自加计算时间
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
  Output:			无
  Return:			无
  Others:			
*************************************************/
void sys_set_no_deal(uint8 flg)
{
	g_NoDeal = flg;
}

/*************************************************
  Function:			sys_continue_play_file
  Description: 		继续播放
  Input: 			
  Output:			无
  Return:			无
  Others:			
*************************************************/
int sys_continue_play_file(void)
{
	SYS_MEDIA_TYPE MediaType = sys_get_media_state();
	switch (MediaType)
	{
		case SYS_MEDIA_MUSIC:
			{
				media_continue_sound();
			}
			break;
		
		case SYS_MEDIA_LEAVEWORD_PLAY:
			{
				if (LYLY_TYPE_VIDEO == g_LylyMode)
				{
					media_continue_video_lyly();
				}
				else
				{
					media_continue_sound();
				}
			}
			break;
		
		default:
			return SYS_MEDIA_ECHO_ERR;
	}
	
	return SYS_MEDIA_ECHO_OK;
}

/*************************************************
  Function:			sys_pause_play_file
  Description: 		暂停播放媒体文件
  Input: 			
  Output:			无
  Return:			无
  Others:			
*************************************************/
int sys_pause_play_file(void)
{
	SYS_MEDIA_TYPE MediaType = sys_get_media_state();
	switch (MediaType)
	{
		case SYS_MEDIA_MUSIC:
			{
				media_pause_sound();
			}
			break;
		
		case SYS_MEDIA_LEAVEWORD_PLAY:
			{
				if (LYLY_TYPE_VIDEO == g_LylyMode)
				{
					media_pause_video_lyly();
				}
				else
				{
					media_pause_sound();
				}
			}
			break;
		
		default:
			return SYS_MEDIA_ECHO_ERR;
	}
	
	return SYS_MEDIA_ECHO_OK;
}

/*************************************************
  Function:			sys_stop_play_audio
  Description: 		停止播放音频文件
  Input: 			
  	1.AudioType		音乐播放类型
  Output:			无
  Return:			无
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
  Description: 		开始播放音频
  Input: 			
  	1.MediaType		媒体操作类型
  	2.FileName		需要播放的媒体文件路径
  	3.isRepeat		是否重复播放
  	4.value			设置的音量
  	5.ProgressProc	进度条回调函数
  	6.StoppedProc	被强制终止后回调函数
  Output:			无
  Return:			0-成功 非0-失败
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
	// modi by luofl 2014-03-10 关闭媒体后延迟100ms，防止媒体没关闭完全又开启而出错。
	usleep(100*1000);
	set_curplay_state(MediaType, ProgressProc, StoppedProc, isRepeat);

	// 开始播放音频文件
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
  Description: 		开始播放留影留言
  Input: 			
  	1.FileName		文件名
  	2.vorect		播放区域
  	3.ProcessProc	进度回调
  	4.StoppedProc	被强制终止后回调函数
  Output:			无
  Return:			0-成功 非0-失败
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

	// add by chenbh 2016-08-18 先做音量设置
	media_set_ring_volume(volume);

	dprintf("FileName: %s\n", FileName);
	// 开始播放留影留言
	if (LYLY_TYPE_VIDEO == type)
	{
		ret = media_play_video_lyly(FileName, (void*)play_media_callback);
		if (ret == FALSE)		
		{
			set_curplay_state(SYS_MEDIA_NONE, NULL, NULL, FALSE);
			return SYS_MEDIA_ECHO_ERR;
		}
	}
	else	// 播放纯声音
	{
		ret = media_play_sound(FileName, 0, (void*)play_media_callback);
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
  Description: 		停止播放留影留言
  Input: 			无
  Output:			无
  Return:			0-成功 非0-失败
  Others:			
*************************************************/
int32 sys_stop_play_leaveword(void)
{
	SYS_MEDIA_TYPE MediaType = sys_get_media_state();
	if (MediaType != SYS_MEDIA_LEAVEWORD_PLAY)
	{
		return SYS_MEDIA_ECHO_ERR;
	}
	
	// 停止播放留影留言
	if (LYLY_TYPE_VIDEO == g_LylyMode)
	{
		media_stop_video_lyly();	
	}
	else
	{
		media_stop_sound();
	}
	set_curplay_state(SYS_MEDIA_NONE, NULL, NULL, FALSE);
	g_LylyMode = LYLY_TYPE_MAX;
	return SYS_MEDIA_ECHO_OK;
}

/*************************************************
  Function:			sys_start_family_record
  Description: 		开始家人留言录制
  Input: 			
  	1.FileName		文件路径
  	2.ProcessProc	进度回调函数
  	3.StoppedProc	被强制终止后回调函数
  Output:			无
  Return:			0-成功 非0-失败
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

	// 开始家人留言录制
	//ret = media_start_local_record(FileName, RECORD_TIME_MAX, (void*)play_media_callback);
	media_set_mic_volume(MIC_VOLUME);
	ret = media_start_local_record(FileName);
	if (ret == FALSE)
	{
		set_curplay_state(SYS_MEDIA_NONE, NULL, NULL, FALSE);
		return SYS_MEDIA_ECHO_ERR;
	}
	#if 0
	else	// add by chenbh 增加定时器
	{
		// modi by chenbh UI层做定时 逻辑就不处理定时了
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
  Description: 		停止家人留言录制
  Input: 			无
  Output:			无
  Return:			0-成功 非0-失败
  Others:			
*************************************************/
int32 sys_stop_family_record(void)
{
	SYS_MEDIA_TYPE MediaType = sys_get_media_state();
	if (MediaType != SYS_MEDIA_FAMILY_RECORD)
	{
		return SYS_MEDIA_ECHO_ERR;
	}

	// modi by chenbh 20160822 定时器在UI层处理
	#if 0
	// add by chenbh 删除定时器
	if (0 != g_RecordTimer)
	{
		cancel_aurine_timer(&g_RecordTimer, NULL);
		g_RecordTimer = 0;
	}
	#endif
	
	// 停止家人留言录制
	media_stop_local_record();
	set_curplay_state(SYS_MEDIA_NONE, NULL, NULL, FALSE);
	return SYS_MEDIA_ECHO_OK;
}

/*************************************************
  Function:			sys_set_intercomm_state
  Description: 		设置当前对讲状态
  Input: 			
  	1.state			TRUE-处于对讲中 FALSE-不处于对讲中
  Output:			无
  Return:			0-成功 非0-失败
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
  Description: 		设置当前监视状态
  Input: 			
  	1.state			TRUE-处于监视中 FALSE-不处于监视中
  Output:			无
  Return:			0-成功 非0-失败
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
  Description: 		获取系统媒体状态
  Input: 		
  Output:			无
  Return:			系统媒体状态
  Others:			
*************************************************/
SYS_MEDIA_TYPE sys_get_media_state(void)
{
	return g_CurMediaType;
}

/*************************************************
  Function:			sys_stop_cur_media
  Description: 		停止当前媒体
  Input: 		
  Output:			无
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
  Function:			sys_key_beep
  Description: 		按键音
  Input: 		
  Output:			无
  Return:			无
  Others:			
*************************************************/
void sys_key_beep(void)
{
	#if 0
	// add by luofl 2014-08-28在清洁屏幕下不响按键音
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
  Function:			sys_set_hint_state
  Description: 		设置系统状态
  Input: 		
  	1.HintType		系统状态类型
  	2.flg			标志: TRUE / FALSE
  Output:			无
  Return:			无
  Others:			这个函数最好只是由本文件函数调用 外部文件不调用 后期优化
*************************************************/
void sys_set_hint_state(SYS_HINT_TYPE HintType, uint8 flg)
{
	uint8 i = 0;
	
	// 设置全局变量	
	g_SysHintRecord.syshint[HintType] = flg;
	dprintf("sys_set_hint_state : HintType:%d, flg:%d\n", HintType, flg);
#if 0	// 暂时注释掉 等驱动可用再开启
	#ifdef _CP2526_TOUCH_KEY_
	// 操作按键背光灯
	for (i = SYS_HINT_INFO; i < SYS_HINT_MISSED_CALLS+1; i++)
	{
		if (g_SysHintRecord.syshint[i] == TRUE)
		{
			// 背光灯闪烁
			dprintf("sys_set_hint_state : key led flash\n");
			hw_key_led_flash();
			return;
		}
	}

	if (g_SysHintRecord.syshint[SYS_HINT_OPERATING] || g_SysHintRecord.syshint[SYS_HINT_ALARM_STATE])
	{
		// 背光灯高亮
		dprintf("sys_set_hint_state : key led on\n");
		hw_key_led_on();
		return;
	}

	// 背光灯微亮
	dprintf("sys_set_hint_state : key led off\n");
	hw_key_led_off();
	#else
	if (TRUE == g_SysHintRecord.syshint[SYS_HINT_INFO])
	{
		hw_message_led_glint();
	}
	else
	{
		hw_message_led_off();
	}

	if (TRUE == g_SysHintRecord.syshint[SYS_HINT_ALARM_WARNING])
	{
		hw_alarm_led_glint();
	}
	else if (TRUE == g_SysHintRecord.syshint[SYS_HINT_ALARM_STATE])
	{
		hw_alarm_led_on();
	}
	else
	{
		hw_alarm_led_off();
	}
	#endif
#endif	
	return;
}

/*************************************************
  Function:			sys_get_hint_state
  Description: 		获得系统状态
  Input: 		
  	1.HintType		系统状态类型
  Output:			无
  Return:			TRUE / FALSE
  Others:			
*************************************************/
uint8 sys_get_hint_state(SYS_HINT_TYPE HintType)
{
	if (HintType >= SYS_HINT_MAX_NUM || HintType < SYS_HINT_INFO)
	{
		return FALSE;
	}

	// 免打扰即时获取状态
	if (HintType == SYS_HINT_NO_DISTURB)
	{
		g_SysHintRecord.syshint[HintType] = linkage_get_nodisturb_state();	
	}
	else if (SYS_HINT_ALARM_STATE == HintType)
	{
		g_SysHintRecord.syshint[HintType] = storage_get_alarm_state();
	}
	
	return g_SysHintRecord.syshint[HintType];
}

/*************************************************
  Function:			sys_get_hint_list
  Description: 		
  Input: 		
  Output:			无
  Return:			
  Others:			
*************************************************/
SysHintRecord *sys_get_hint_list(void)
{
	return &g_SysHintRecord;
}

/*************************************************
  Function:			sys_sync_hint_state
  Description: 		同步系统状态
  Input: 		
  Output:			无
  Return:			无
  Others:			
*************************************************/
void sys_sync_hint_state(void)
{
	uint8 i, flg, unread_num;

	{
		// 获取信息未读状态
		unread_num = 0;
		if (g_SysHintRecord.MsgUnReadList != NULL)
		{
			free(g_SysHintRecord.MsgUnReadList);
			g_SysHintRecord.MsgUnReadList = NULL;
		}
		g_SysHintRecord.MsgUnReadList = storage_read_msg_list();
		if (g_SysHintRecord.MsgUnReadList)
		{
			for (i = 0; i < g_SysHintRecord.MsgUnReadList->ncount; i++)
			{
				if (1 == g_SysHintRecord.MsgUnReadList->pinfo_data[i].is_unread)
				{
					unread_num ++;
				}
			}
			g_SysHintRecord.syshintnum[SYS_HINT_INFO] = unread_num;
			dprintf("MsgUnReadList num: %d\n", g_SysHintRecord.MsgUnReadList->ncount);
		}
		
		if (unread_num)
		{
			sys_set_hint_state(SYS_HINT_INFO, TRUE);
		}
		else
		{
			sys_set_hint_state(SYS_HINT_INFO, FALSE);
		}
	}

	{
		// 获取留影留言未读状态
		// 获取留影留言信息
		unread_num = 0;
		if (g_SysHintRecord.LylyUnReadList != NULL)
		{
			storage_free_lyly_memory(&(g_SysHintRecord.LylyUnReadList));
		}
		storage_get_lylyrecord(&(g_SysHintRecord.LylyUnReadList));
		if (g_SysHintRecord.LylyUnReadList)
		{		
			for (i = 0; i < g_SysHintRecord.LylyUnReadList->Count; i++)
			{
				if (g_SysHintRecord.LylyUnReadList->LylyInfo[i].UnRead == TRUE)
				{
					unread_num ++;
				}
			}
			g_SysHintRecord.syshintnum[SYS_HINT_LEAVEWORD] = unread_num;
			dprintf("LylyUnReadList num: %d\n", g_SysHintRecord.LylyUnReadList->Count);
		}
		if (unread_num)
		{
			sys_set_hint_state(SYS_HINT_LEAVEWORD, TRUE);
		}
		else
		{
			sys_set_hint_state(SYS_HINT_LEAVEWORD, FALSE);
		}
	}

	{
		// 获取家人留言未读状态
		unread_num = 0;
		if (g_SysHintRecord.JrlyUnReadList != NULL)
		{
			storage_free_jrly_memory(&(g_SysHintRecord.JrlyUnReadList));
		}
		storage_get_jrlyrecord(&(g_SysHintRecord.JrlyUnReadList));
		if (g_SysHintRecord.JrlyUnReadList)
		{
			for (i = 0; i < g_SysHintRecord.JrlyUnReadList->Count; i++)
			{
				if (g_SysHintRecord.JrlyUnReadList->JrlyInfo[i].UnRead == TRUE)
				{
					unread_num++;
				}
			}
			g_SysHintRecord.syshintnum[SYS_HINT_FAMILY] = unread_num;
			dprintf("JrlyUnReadList num: %d\n", g_SysHintRecord.JrlyUnReadList->Count);
		}
		if (unread_num)
		{
			sys_set_hint_state(SYS_HINT_FAMILY, TRUE);
		}
		else
		{
			sys_set_hint_state(SYS_HINT_FAMILY, FALSE);
		}
	}

	{
		// 获取报警未处理信息
		if (g_SysHintRecord.AlarmUnReadList != NULL)
		{
			free(g_SysHintRecord.AlarmUnReadList);
			g_SysHintRecord.AlarmUnReadList = NULL;
		}
		
		g_SysHintRecord.AlarmUnReadList = storage_get_afbj_unread_record_ext();
		if(g_SysHintRecord.AlarmUnReadList)
		{
			g_SysHintRecord.syshintnum[SYS_HINT_ALARM_WARNING] = g_SysHintRecord.AlarmUnReadList->nCount;
			sys_set_hint_state(SYS_HINT_ALARM_WARNING, TRUE);		
			dprintf("AlarmUnReadList num: %d\n", g_SysHintRecord.AlarmUnReadList->nCount);
		}
	}

	{
		// 获取未接来电未读状态
		unread_num = 0;
		if (g_SysHintRecord.MissUnReadList != NULL)
		{
			free(g_SysHintRecord.MissUnReadList);
			g_SysHintRecord.MissUnReadList = NULL;
		}
		g_SysHintRecord.MissUnReadList = storage_get_callrecord(MISSED);
		if (g_SysHintRecord.MissUnReadList)
		{
			for(i = 0; i < g_SysHintRecord.MissUnReadList->CallCount; i++)
			{
				if (1 == g_SysHintRecord.MissUnReadList->CallInfo[i].UnRead)
				{
					unread_num++;
					break;
				}
			}
			g_SysHintRecord.syshintnum[SYS_HINT_MISSED_CALLS] = unread_num;
			dprintf("MissUnReadList num: %d\n", g_SysHintRecord.MissUnReadList->CallCount);
		}
		if (unread_num)
		{
			sys_set_hint_state(SYS_HINT_MISSED_CALLS, TRUE);
		}
		else
		{
			sys_set_hint_state(SYS_HINT_MISSED_CALLS, FALSE);
		}
	}

	{
		// 初始化免打扰-无免打扰
		flg = storage_get_noface();
		if (flg == TRUE)
		{
			sys_set_hint_state(SYS_HINT_NO_DISTURB, TRUE);
		}
		else
		{
			sys_set_hint_state(SYS_HINT_NO_DISTURB, FALSE);
		}
		g_SysHintRecord.syshintnum[SYS_HINT_NO_DISTURB] = flg;
	}

	{
		flg = storage_get_alarm_state();	
		if (flg == DIS_DEFEND)					// 撤防
		{
			sys_set_hint_state(SYS_HINT_ALARM_STATE, FALSE);
		}	
		else									// 布防
		{
			sys_set_hint_state(SYS_HINT_ALARM_STATE, TRUE);		
		}	
		g_SysHintRecord.syshintnum[SYS_HINT_ALARM_STATE] = flg;
	}	
}

/*************************************************
  Function:			sys_sync_hint_state
  Description: 		同步系统状态
  Input: 		
  Output:			无
  Return:			无
  Others:			
*************************************************/
void sys_sync_hint_state_ext(SYS_HINT_TYPE HintType)
{
	uint8 i, flg, unread_num;

	switch (HintType)
	{
		case SYS_HINT_INFO:
			{
				// 获取信息未读状态
				unread_num = 0;
				if (g_SysHintRecord.MsgUnReadList != NULL)
				{
					free(g_SysHintRecord.MsgUnReadList);
					g_SysHintRecord.MsgUnReadList = NULL;
				}
				g_SysHintRecord.MsgUnReadList = storage_read_msg_list();
				if (g_SysHintRecord.MsgUnReadList)
				{
					for (i = 0; i < g_SysHintRecord.MsgUnReadList->ncount; i++)
					{
						if (1 == g_SysHintRecord.MsgUnReadList->pinfo_data[i].is_unread)
						{
							unread_num ++;
						}
					}
					g_SysHintRecord.syshintnum[SYS_HINT_INFO] = unread_num;
					dprintf("MsgUnReadList num: %d\n", g_SysHintRecord.MsgUnReadList->ncount);
				}
				
				if (unread_num)
				{
					sys_set_hint_state(SYS_HINT_INFO, TRUE);
				}
				else
				{
					sys_set_hint_state(SYS_HINT_INFO, FALSE);
				}
			}
			break;

		case SYS_HINT_LEAVEWORD:
			{
				// 获取留影留言未读状态
				// 获取留影留言信息
				unread_num = 0;
				if (g_SysHintRecord.LylyUnReadList != NULL)
				{
					storage_free_lyly_memory(&(g_SysHintRecord.LylyUnReadList));
				}
				storage_get_lylyrecord(&(g_SysHintRecord.LylyUnReadList));
				if (g_SysHintRecord.LylyUnReadList)
				{		
					for (i = 0; i < g_SysHintRecord.LylyUnReadList->Count; i++)
					{
						if (g_SysHintRecord.LylyUnReadList->LylyInfo[i].UnRead == TRUE)
						{
							unread_num ++;
						}
					}
					g_SysHintRecord.syshintnum[SYS_HINT_LEAVEWORD] = unread_num;
					dprintf("LylyUnReadList num: %d\n", g_SysHintRecord.LylyUnReadList->Count);
				}
				if (unread_num)
				{
					sys_set_hint_state(SYS_HINT_LEAVEWORD, TRUE);
				}
				else
				{
					sys_set_hint_state(SYS_HINT_LEAVEWORD, FALSE);
				}
			}
			break;
			
		case SYS_HINT_FAMILY:
			{
				// 获取家人留言未读状态
				unread_num = 0;
				if (g_SysHintRecord.JrlyUnReadList != NULL)
				{
					storage_free_jrly_memory(&(g_SysHintRecord.JrlyUnReadList));
				}
				storage_get_jrlyrecord(&(g_SysHintRecord.JrlyUnReadList));
				if (g_SysHintRecord.JrlyUnReadList)
				{
					for (i = 0; i < g_SysHintRecord.JrlyUnReadList->Count; i++)
					{
						if (g_SysHintRecord.JrlyUnReadList->JrlyInfo[i].UnRead == TRUE)
						{
							unread_num++;
						}
					}
					g_SysHintRecord.syshintnum[SYS_HINT_FAMILY] = unread_num;
					dprintf("JrlyUnReadList num: %d\n", g_SysHintRecord.JrlyUnReadList->Count);
				}
				if (unread_num)
				{
					sys_set_hint_state(SYS_HINT_FAMILY, TRUE);
				}
				else
				{
					sys_set_hint_state(SYS_HINT_FAMILY, FALSE);
				}
			}
			break;
	
		case SYS_HINT_ALARM_WARNING:
			{
				// 获取报警未处理信息
				if (g_SysHintRecord.AlarmUnReadList != NULL)
				{
					free(g_SysHintRecord.AlarmUnReadList);
					g_SysHintRecord.AlarmUnReadList = NULL;
				}
				
				g_SysHintRecord.AlarmUnReadList = storage_get_afbj_unread_record_ext();
				if(g_SysHintRecord.AlarmUnReadList)
				{
					g_SysHintRecord.syshintnum[SYS_HINT_ALARM_WARNING] = g_SysHintRecord.AlarmUnReadList->nCount;
					sys_set_hint_state(SYS_HINT_ALARM_WARNING, TRUE);		
					dprintf("AlarmUnReadList num: %d\n", g_SysHintRecord.AlarmUnReadList->nCount);
				}
			}
			break;
			
		case SYS_HINT_MISSED_CALLS:
			{
				// 获取未接来电未读状态
				unread_num = 0;
				if (g_SysHintRecord.MissUnReadList != NULL)
				{
					free(g_SysHintRecord.MissUnReadList);
					g_SysHintRecord.MissUnReadList = NULL;
				}
				g_SysHintRecord.MissUnReadList = storage_get_callrecord(MISSED);
				if (g_SysHintRecord.MissUnReadList)
				{
					for(i = 0; i < g_SysHintRecord.MissUnReadList->CallCount; i++)
					{
						if (1 == g_SysHintRecord.MissUnReadList->CallInfo[i].UnRead)
						{
							unread_num++;							
						}
					}
					g_SysHintRecord.syshintnum[SYS_HINT_MISSED_CALLS] = unread_num;
					dprintf("MissUnReadList num: %d, CallCount: %d\n", unread_num, g_SysHintRecord.MissUnReadList->CallCount);
				}
				if (unread_num)
				{
					sys_set_hint_state(SYS_HINT_MISSED_CALLS, TRUE);
				}
				else
				{
					sys_set_hint_state(SYS_HINT_MISSED_CALLS, FALSE);
				}
			}
			break;
			
		case SYS_HINT_NO_DISTURB:
			{
				// 初始化免打扰-无免打扰
				flg = storage_get_noface();
				if (flg == TRUE)
				{
					sys_set_hint_state(SYS_HINT_NO_DISTURB, TRUE);
				}
				else
				{
					sys_set_hint_state(SYS_HINT_NO_DISTURB, FALSE);
				}
				g_SysHintRecord.syshintnum[SYS_HINT_NO_DISTURB] = flg;
			}
			break;

		case SYS_HINT_ALARM_STATE:
			{
				flg = storage_get_alarm_state();	
				if (flg == DIS_DEFEND)					// 撤防
				{
					sys_set_hint_state(SYS_HINT_ALARM_STATE, FALSE);
				}	
				else									// 布防 局防
				{
					sys_set_hint_state(SYS_HINT_ALARM_STATE, TRUE);		
				}	
				g_SysHintRecord.syshintnum[SYS_HINT_ALARM_STATE] = flg;
			}
			break;
				
		default:
			break;
	}		
}

/*************************************************
  Function:			sys_init_hint_record
  Description: 		系统上电时初始化系统记录信息
  Input: 		
  Output:			无
  Return:			无
  Others:			
*************************************************/
static void sys_init_hint_record(void)
{
	uint8 i, unread_num;
	memset(&g_SysHintRecord, 0, sizeof(SysHintRecord));
	g_SysHintRecord.AlarmUnReadList = NULL;
	g_SysHintRecord.JrlyUnReadList = NULL;
	g_SysHintRecord.MissUnReadList = NULL;
	g_SysHintRecord.MsgUnReadList = NULL;
	g_SysHintRecord.LylyUnReadList = NULL;

	// 获取信息列表
	unread_num = 0;
	g_SysHintRecord.MsgUnReadList = storage_read_msg_list();
	if (g_SysHintRecord.MsgUnReadList)
	{
		for (i = 0; i < g_SysHintRecord.MsgUnReadList->ncount; i++)
		{
			if (1 == g_SysHintRecord.MsgUnReadList->pinfo_data[i].is_unread)
			{
				unread_num ++;
			}
		}
		g_SysHintRecord.syshintnum[SYS_HINT_INFO] = unread_num;
		dprintf("MsgUnReadList num: %d\n", g_SysHintRecord.MsgUnReadList->ncount);
	}

	// 获取留影留言信息
	unread_num = 0;	
	storage_get_lylyrecord(&(g_SysHintRecord.LylyUnReadList));
	if (g_SysHintRecord.LylyUnReadList)
	{		
		for (i = 0; i < g_SysHintRecord.LylyUnReadList->Count; i++)
		{
			if (g_SysHintRecord.LylyUnReadList->LylyInfo[i].UnRead == TRUE)
			{
				unread_num ++;
			}
		}
		g_SysHintRecord.syshintnum[SYS_HINT_LEAVEWORD] = unread_num;
		dprintf("LylyUnReadList num: %d\n", g_SysHintRecord.LylyUnReadList->Count);
	}

	// 获取家人留言信息
	unread_num = 0;	
	storage_get_jrlyrecord(&(g_SysHintRecord.JrlyUnReadList));
	if (g_SysHintRecord.JrlyUnReadList)
	{
		for (i = 0; i < g_SysHintRecord.JrlyUnReadList->Count; i++)
		{
			if (g_SysHintRecord.JrlyUnReadList->JrlyInfo[i].UnRead == TRUE)
			{
				unread_num++;
			}
		}
		g_SysHintRecord.syshintnum[SYS_HINT_FAMILY] = unread_num;
		dprintf("JrlyUnReadList num: %d\n", g_SysHintRecord.JrlyUnReadList->Count);
	}

	// 获取未接记录信息
	unread_num = 0;
	g_SysHintRecord.MissUnReadList = storage_get_callrecord(MISSED);
	if (g_SysHintRecord.MissUnReadList)
	{
		for(i = 0; i < g_SysHintRecord.MissUnReadList->CallCount; i++)
		{
			if (1 == g_SysHintRecord.MissUnReadList->CallInfo[i].UnRead)
			{
				unread_num++;
			}
		}
		g_SysHintRecord.syshintnum[SYS_HINT_MISSED_CALLS] = unread_num;
		dprintf("MissUnReadList num: %d\n", g_SysHintRecord.MissUnReadList->CallCount);
	}

	// 获取报警未处理信息
	unread_num = 0;	
	g_SysHintRecord.AlarmUnReadList = storage_get_afbj_unread_record_ext();
	if(g_SysHintRecord.AlarmUnReadList)
	{
		g_SysHintRecord.syshintnum[SYS_HINT_ALARM_WARNING] = g_SysHintRecord.AlarmUnReadList->nCount;
		dprintf("AlarmUnReadList num: %d\n", g_SysHintRecord.AlarmUnReadList->nCount);
	}

	for (i=SYS_HINT_INFO; i<SYS_HINT_MAX_NUM; i++)
	{
		dprintf("g_SysHintRecord.syshintnum[%d]:%d\n", i, g_SysHintRecord.syshintnum[i]);	
	}
}

/*************************************************
  Function:			sys_init_hint_state
  Description: 		系统上电时初始化系统状态
  Input: 		
  Output:			无
  Return:			无
  Others:			
*************************************************/
void sys_init_hint_state(void)
{
	uint8 flg, unread_num;

	pthread_mutex_init(&g_MediaLock, NULL);
	sys_init_hint_record();

	// 获取信息未读状态
	//unread_num = storage_get_msg_state();
	unread_num = g_SysHintRecord.syshintnum[SYS_HINT_INFO];
	if (unread_num)
	{
		sys_set_hint_state(SYS_HINT_INFO, TRUE);
	}
	else
	{
		sys_set_hint_state(SYS_HINT_INFO, FALSE);
	}

	// 获取留影留言未读状态
	//unread_num = storage_get_lylyrecord_flag();
	unread_num = g_SysHintRecord.syshintnum[SYS_HINT_LEAVEWORD];
	if (unread_num)
	{
		sys_set_hint_state(SYS_HINT_LEAVEWORD, TRUE);
	}
	else
	{
		sys_set_hint_state(SYS_HINT_LEAVEWORD, FALSE);
	}
	
	// 获取家人留言未读状态
	//unread_num = storage_get_jrlyrecord_flag();
	unread_num = g_SysHintRecord.syshintnum[SYS_HINT_FAMILY];;
	if (unread_num)
	{
		sys_set_hint_state(SYS_HINT_FAMILY, TRUE);
	}
	else
	{
		sys_set_hint_state(SYS_HINT_FAMILY, FALSE);
	}
		
	// 获取未接来电未读状态
	//unread_num = storage_get_callrecord_state();
	unread_num = g_SysHintRecord.syshintnum[SYS_HINT_MISSED_CALLS];;
	if (unread_num)
	{
		sys_set_hint_state(SYS_HINT_MISSED_CALLS, TRUE);
	}
	else
	{
		sys_set_hint_state(SYS_HINT_MISSED_CALLS, FALSE);
	}
		
	// 初始化免打扰-无免打扰
	storage_set_noface_enable(FALSE);
	sys_set_hint_state(SYS_HINT_NO_DISTURB, FALSE);
	g_SysHintRecord.syshintnum[SYS_HINT_NO_DISTURB] = FALSE;
	
	// 获取安防状态
	flg = storage_get_alarm_state();	
	if (flg == DIS_DEFEND)					// 撤防
	{
		sys_set_hint_state(SYS_HINT_ALARM_STATE, FALSE);
	}	
	else									// 布防 局防
	{
		sys_set_hint_state(SYS_HINT_ALARM_STATE, TRUE);		
	}	
	g_SysHintRecord.syshintnum[SYS_HINT_ALARM_STATE] = flg;
	
	// 获取报警未读条数
	//unread_num = storage_get_alarm_undeal_num();
	unread_num = g_SysHintRecord.syshintnum[SYS_HINT_ALARM_WARNING];
	if (unread_num)
	{
		sys_set_hint_state(SYS_HINT_ALARM_WARNING, TRUE);		
	}

	// 初始化系统为操作状态
	dprintf("sys_init_hint_state open lcd!\n");
	#ifdef _TIMER_REBOOT_NO_LCD_
	if (storage_get_isOpenScreen())
	{
		ScreenOn();		
	}
	else	
	{
		timer_reboot_control(FALSE);
	}
	#else
	ScreenOn();		
	#endif
}

