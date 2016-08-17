/*************************************************
  Copyright (C), 2006-2010, Aurine
  File name: 	logic_media.c
  Author:   	陈本惠
  Version:  	2.0
  Date: 		2016-06-30
  Description:  多媒体接口函数
				
  History:        
    1. Date:
       Author:
       Modification:
    2. ...
    返回值统一 -1 错误 0 正确
*************************************************/
#include "logic_include.h"

#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/uio.h>
#include <errno.h>
#include <sys/time.h>
#include <semaphore.h>
#include <wchar.h>
#include <sys/mman.h>
#include "logic_media.h"
#include "leaf_mediastream.h"

#define LEAVE_PIC_TYPE			".jpg"
#define LEAVE_AVI_TYPE			".avi"
#define LEAVE_WAV_TYPE			".wav"

static LeafCall *g_LeafCall = NULL;


MEDIA_LYLY_CTRL g_LylyRecordCtrl = 
{
	NULL,
	{0},
	0,
	{0, 0, 0, 0, 0},
	NULL,
	LWM_NONE,
	{0, 0, 0},
	{0, 0, 0},
	0,
	0,
	NULL
};

// modi by luofl 2014-08-28 铃声音量最大级超过标准85DB
uint8 AudioVolumeLevel[9] = {0, 7, 10, 13, 15, 17, 19, 20, 21};

// ============V5室内机(其它款室内机都共用这个参数)========================
uint8 V5_STAIR_AudioVolumeLevel[9] = {0, 10, 16, 21, 22, 23, 24, 25, 26};
uint8 V5_CENTER_AudioVolumeLevel[9] = {0, 10, 13, 16, 18, 20, 21, 22, 23};
uint8 V5_NETDOOR_AudioVolumeLevel[9] = {0, 10, 16, 18, 20, 22, 24, 25, 26};
uint8 V5_ROOM_AudioVolumeLevel[9] = {0, 10, 15, 19, 21, 22, 23, 24, 25};

static struct ThreadInfo VideoPreviewThread;		// 模拟门前机视频预览线程


/*************************************************
  Function:    		media_Ad_set_audio_PackMode
  Description: 		设置音频发送包 组包方式
  Input: 			
  	1.packmode		0: 单包发送 1: 多包发送
  Output:			无
  Return:			
  Others:
*************************************************/
void media_Ad_set_audio_PackMode(uint8 packmode)
{
	//set_audio_packmode(packmode);
}

/*************************************************
  Function:			media_stream_FileExtCmp
  Description:		查询文件类型
  Input:
  Output:			无
  Return:			成功或失败
  Others:
*************************************************/
int32 media_stream_FileExtCmp(const uint8*	pu8FileName, const char* pu8Ext)
{
	int32		i32Len1 = strlen((char*)pu8FileName);
	int32		i32Len2 = strlen(pu8Ext);
	uint8*	pu8Tmp = (uint8*)(pu8FileName + (i32Len1 - i32Len2));

	if (i32Len2 > i32Len1)
		return -1;

	return strcasecmp((char*)pu8Tmp, pu8Ext);
}

/*************************************************
  Function:			media_start_analog_video_proc
  Description:		模拟视频预览线程
  Input:
  Output:			无
  Return:			成功或失败
  Others:
*************************************************/
static void* media_start_analog_video_proc(void *param)
{
	// 设置分离线程
	pthread_detach(pthread_self());
	uint8_t * pu8PicPtr;
	uint64_t u64TS;
	uint32_t u32PicPhyAdr;

	while (VideoPreviewThread.running)
	{
		
	}

thread_end:
	pthread_exit(NULL);
	return NULL;             // 返回后资源由系统释放
}

/*************************************************
  Function:			media_start_analog_video
  Description:		启动模拟视频预览
  Input:
  Output:			无
  Return:			成功或失败
  Others:
*************************************************/
uint32 media_start_analog_video(void)
{
	return TRUE;
}

/*************************************************
  Function:			media_stop_analog_video
  Description:		停止模拟视频预览
  Input: 			无
  Output:			无
  Return:			无
  Others:
*************************************************/
void media_stop_analog_video(void)
{

	return;
}

/*************************************************
  Function:			media_set_analog_video_win
  Description:		设置模拟视频预览窗口
  Input:
  	1.flag			1-全屏显示, 其它为开窗显示
  Output:			无
  Return:			无
  Others:
*************************************************/
static int32 media_set_analog_video_win(uint8 flag)
{
	#ifdef _DOOR_PHONE_
	
	#else
	return TRUE;
	#endif
}

/*************************************************
  Function:			media_set_analog_output_volume
  Description:		设置模拟设备通话音量
  Input: 
  	vol				音量等级(0静音 - 8最大)
  Output:			无
  Return:			TRUE/FALSE
  Others:
*************************************************/
uint32 media_set_analog_talk_volume(uint32 vol)
{
	int32 ret = hw_set_volume(vol);
	if (ret == -1)
	{
		return FALSE;
	}
	return TRUE;
}

/*************************************************
  Function:			media_get_wav_time
  Description:		获取wav文件播放时长
  Input: 	
  Output:			
  Return:			
  Others:
*************************************************/
int media_get_wav_time(char *filename)
{
	int ret = 0;
	if (filename)
	{
		//ret = leaf_get_wav_file_play_time(filename);
	}
	return ret;
}

/*************************************************
  Function:			media_start_net_video
  Description:		启动网络视频播放
  Input: 	
  	1.addres		对端地址
  	2.mode			视频模式 发送、接收
  Output:			无
  Return:			成功或失败
  Others:
*************************************************/
uint32 media_start_net_video(uint32 address, uint8 mode)
{
	return TRUE;
}

/*************************************************
  Function:			media_stop_net_video
  Description:		停止网络视频播放
  Input: 			无
  Output:			无
  Return:			无
  Others:
*************************************************/
void media_stop_net_video(uint8 mode)
{	
	return;
}

/*************************************************
  Function:			media_start_net_audio
  Description:		启动网络音频播放
  Input: 			
  	1.tp			音频格式
  	2.proc			回调
  Output:			无
  Return:			TRUE/FALSE
  Others:
*************************************************/
uint32 media_start_net_audio(int address)
{
	return TRUE;
}

/*************************************************
  Function:			media_stop_net_audio
  Description:		停止网络音频播放
  Input: 			无
  Output:			无
  Return:			无
  Others:
*************************************************/
void media_stop_net_audio(void)
{
	dprintf("net_stop_audio\n");
	return;
}

/*************************************************
  Function:			meida_start_net_hint
  Description:		启动留言提示音发送
  Input: 			
  	1.tp			发送的格式
  	2.filename		提示音文件
  Output:			无
  Return:			TRUE/FALSE
  Others:
*************************************************/
uint32 meida_start_net_hint(PAYLOAD_TYPE_E tp, char *filename, void * proc)
{
	return TRUE;
}

/*************************************************
  Function:			meida_stop_net_hint
  Description:		停止提示音发送
  Input: 			无
  Output:			无
  Return:			无
  Others:
*************************************************/
uint32 meida_stop_net_hint(void)
{
	return TRUE;
}

/*************************************************
  Function:			media_fill_LylyRecordCtrl
  Description:		
  Input: 			
  	1.mode			录制的模式
  	2.atp			音频格式
  	3.vtp			视频格式
  	4.filename		保存的文件名
  Output:			无
  Return:			TRUE/FALSE
  Others:
*************************************************/
static void media_fill_LylyRecordCtrl(LEAVE_WORD_MODE_E mode, PAYLOAD_TYPE_E atp, PAYLOAD_TYPE_E vtp, char * filename)
{
	memset(g_LylyRecordCtrl.FileName, 0, sizeof(g_LylyRecordCtrl.FileName));
	strcpy(g_LylyRecordCtrl.FileName, filename);
	g_LylyRecordCtrl.mode = mode;
}

/*************************************************
  Function:			media_clean_LylyRecordCtrl
  Description:		
  Input: 			
  Output:			无
  Return:			
  Others:
*************************************************/
static void media_clean_LylyRecordCtrl(void)
{
	g_LylyRecordCtrl.mode = LWM_NONE;
	memset(g_LylyRecordCtrl.FileName, 0, sizeof(g_LylyRecordCtrl.FileName));
}

/*************************************************
  Function:			meida_start_net_leave_rec
  Description:		启动从网络上录制
  Input: 			
  	1.mode			录制的模式
  	2.atp			音频格式
  	3.vtp			视频格式
  	4.filename		保存的文件名
  Output:			无
  Return:			TRUE/FALSE
  Others:
*************************************************/
uint32 meida_start_net_leave_rec(LEAVE_WORD_MODE_E mode,
						PAYLOAD_TYPE_E atp, PAYLOAD_TYPE_E vtp, char * filename)
{	
	return TRUE;	
}

/*************************************************
  Function:			stop_leave_word_net
  Description:		停止录制
  Input: 			
  	1.issave		是否保存(1保存, 0不保存)
  Output:			无
  Return:			TRUE/FALSE
  Others:
*************************************************/
uint32 media_stop_net_leave_rec(uint8 issave)
{
	dprintf("  1111111111111111 \n");
	char filename[100] = {0};
	/*
	stop_lyly_audio_recv();
	
	lyly_record_stop();
	
	ms_media_unlink(mMediaStream.AudioRtpRecv, mMediaStream.AviRecord);
	if (g_LylyRecordCtrl.mode == LWM_AUDIO_VIDEO)
	{
		ms_media_unlink(mMediaStream.VideoRtpRecv, mMediaStream.AviRecord);
	}
*/
	if (issave == FALSE)
	{
		if (g_LylyRecordCtrl.mode == LWM_AUDIO_PIC)
		{
			char picname[100] = {0};
			sprintf(picname, "%s%s", g_LylyRecordCtrl.FileName, LEAVE_PIC_TYPE);
			FSFileDelete(picname);
		}

		if (g_LylyRecordCtrl.mode == LWM_AUDIO_VIDEO)
		{			
			sprintf(filename, "%s%s", g_LylyRecordCtrl.FileName, LEAVE_AVI_TYPE);
		}
		else
		{
			sprintf(filename, "%s%s", g_LylyRecordCtrl.FileName, LEAVE_WAV_TYPE);
		}
		FSFileDelete(filename);	
	}
	media_clean_LylyRecordCtrl();
	
	return TRUE;
}

/*************************************************
  Function:    		media_add_audio_sendaddr
  Description: 		增加音频发送地址
  Input: 			
  	1.IP			IP地址
  	2.AudioPort		音频端口
  Output:			无
  Return:			成功与否true/false
  Others:
*************************************************/
int32 media_add_audio_sendaddr(uint32 IP, uint16 AudioPort)
{
	return TRUE;
}

/*************************************************
  Function:			media_start_local_record
  Description:		启动本地录音
  Input: 			
  	1.filename		文件名
  	2.maxtime		录制的最长时间
  	3.proc			回调
  Output:			无
  Return:			TRUE/FALSE
  Others:
*************************************************/
uint32 media_start_local_record(char *filename)
{
	return TRUE;
}

/*************************************************
  Function:			media_stop_local_record
  Description:		停止本地录音
  Input: 			
  	1.issave		是否保存
  Output:			无
  Return:			TRUE/FALSE
  Others:
*************************************************/
void media_stop_local_record(void)
{
}

/*************************************************
  Function:			media_del_audio_send_addr
  Description:		删除音频的发送地址和端口
  Input: 
  Output:			无
  Return:			无
  Others:
*************************************************/
void media_del_audio_send_addr(uint32 IP, uint16 AudioPort)
{	
	return;
}

/*************************************************
  Function:			media_set_talk_volume
  Description:		设置数字对讲音量
  Input: 
  	vol				音量等级(0静音 - 8最大)
  Output:			无
  Return:			TRUE/FALSE
  Others:
*************************************************/
uint32 media_set_talk_volume(DEVICE_TYPE_E devtype, uint32 vol)
{
	int level = (vol%9)*12;
	leaf_set_play_level(g_LeafCall, level);
	return TRUE;
}

/*************************************************
  Function:			media_set_ring_volume
  Description:		设置铃声音量
  Input: 
  	vol				音量等级(0静音 - 8最大)
  Output:			无
  Return:			TRUE/FALSE
  Others:
*************************************************/
uint32 media_set_ring_volume(uint32 vol)
{
	int level = (vol%9)*12;
   	leaf_set_ring_level(g_LeafCall, level);	
	return TRUE;
}

/*************************************************
  Function:			media_set_mic_volume
  Description:		设置音频codec输入增益
  Input: 
  	vol				音量等级
  Output:			无
  Return:			TRUE/FALSE
  Others:
*************************************************/
uint32 media_set_mic_volume(uint8 vol)
{
	return TRUE;
}

/*************************************************
  Function:			media_fill_video_data
  Description:		将网络视频数据填到buf
  Input: 
  	1.data			数据指针
  	2.len			数据长度
  	3.time			时间戳
  Output:			无
  Return:			TRUE/FALSE
  Others:
*************************************************/
uint32 media_fill_video_data(uint8 *data, uint32 len, uint32 time)
{
#if 0
	uint32 ret;
	
	if (LeaveFlag & 0x02)							// 留影留言
	{
		leave_fill_data(data, len, time, 1);		// 填充视频数据
	}
	
	ret = talk_video_put_data(data, len, time);
	if (ret != TRUE)
	{
		dprintf("media_fill_video_data : err\n");
		return ret;
	}

	/*
	if (g_H264File)
	{
		g_DataLen += len;
		if (g_DataLen > 1024*512*5)
		{
			FS_fwrite(LeaveStaticBuf, 1, g_DataLen-len, g_H264File);
			g_DataLen = len;
			memcpy(LeaveStaticBuf, data, len);
		}
		else
		{
			memcpy(LeaveStaticBuf+g_DataLen-len, data, len);
		}
	}
	*/
#endif
	return TRUE;
}

/*************************************************
  Function:			media_full_dispaly_video
  Description:		全屏显示视频
  Input: 			
  	1.devtype		设备类型
  	2.flg			1 全屏 0缩屛
  Output:			无
  Return:			TRUE/FALSE
  Others:
*************************************************/
uint32 media_full_dispaly_video(DEVICE_TYPE_E devtype, uint8 flg)
{
	int32 ret = FALSE;
	if (devtype == DEVICE_TYPE_DOOR_PHONE) // 模拟门前机
	{
		ret = media_set_analog_video_win(flg);
	}
	else
	{
		//set_full_screen(flg);
		ret = TRUE;
	}
	return ret;
}

/*************************************************
  Function:			media_rtsp_set_full_screen
  Description:		全屏显示视频
  Input: 			
   	2.flg			1 全屏 0缩屛
  Output:			无
  Return:			TRUE/FALSE
  Others:
*************************************************/
int media_rtsp_set_full_screen(uint8 flg)
{
	return TRUE;
}

/*************************************************
  Function:			media_beep
  Description:		播放按键音
  Input: 			无
  Output:			无
  Return:			
  Others:
*************************************************/
void media_beep(void)
{
	
}

/*************************************************
  Function:			media_snapshot
  Description:		抓拍
  Input: 			
  	1.filename		图像保存的文件名
  					如果>1时文件名自动加上编号
	2.dstW			目标图片的宽
	3.dstH			目标图片的高
  Output:			无
  Return:			TRUE/FALSE
  Others:
*************************************************/
uint32 media_snapshot(char *filename, uint32 dstW, uint32 dstH, DEVICE_TYPE_E DevType)
{
#if 0
	// 模拟状态下 视频未开启，抓拍直接返回错误
	if (DEVICE_TYPE_DOOR_PHONE == DevType && g_start_video == 0)
	{
		return FALSE;
	}
	set_jpg_enc_param(filename, dstW, dstH, DevType);
	int32 ret =  open_jpeg_enc();
	if (ret == 0)
	{
		close_jpeg_enc();
		return TRUE;
	}
	return FALSE;
#endif
}

/*************************************************
  Function:			media_start_show_pict
  Description:		JPG图片显示
  Input: 			
  	1.filename		图像保存的文件名
  					如果>1时文件名自动加上编号
	2.pos_x			目标图片显示X 坐标
	3.pos_y			目标图片显示Y 坐标
	4.with			目标图片显示宽度
	5.heigh			目标图片显示长度
  Output:			无
  Return:			TRUE/FALSE
  Others:
*************************************************/
int32 media_start_show_pict(char *filename, uint16 pos_x, uint16 pos_y, uint16 with, uint16 heigh)
{
	int32 ret = FALSE;
	
	//set_jpg_dec_param(filename, pos_x, pos_y, with, heigh);
	//ret = open_jpeg_dec();
	if (ret == 0)
	{
		sys_set_view_picture_state(TRUE);
		return TRUE;
	}
	
	return FALSE;
}

/*************************************************
  Function:			media_stop_show_pict
  Description:		关闭图片显示
  Input: 			
  Output:			无
  Return:			TRUE/FALSE
  Others:
*************************************************/
void media_stop_show_pict(void)
{
	sys_set_view_picture_state(FALSE);	
}

/*************************************************
  Function:			media_enable_audio_aec
  Description:		使能消回声接口
  Input: 			
  Output:			无
  Return:			TRUE/FALSE
  Others:
*************************************************/
int media_enable_audio_aec(void)
{
	return TRUE;
}

/*************************************************
  Function:			media_disable_audio_aec
  Description:		使能消回声接口
  Input: 			
  Output:			无
  Return:			TRUE/FALSE
  Others:
*************************************************/
int media_disable_audio_aec(void)
{
	return TRUE;
}

/*************************************************
  Function:			media_play_sound
  Description:		播放音频文件
  Input: 			
  	1.type			播放类型
  	2.filename		文件名
  	3.isrepeat		是否重复播放	1重复 0不重复
  	4.proc			回调
  Output:			无
  Return:			TRUE/FALSE
  Others:
*************************************************/
uint32 media_play_sound(char *filename, uint8 IsRepeat, void * proc)
{
	if(!g_LeafCall->audiostream)
	{
    	leaf_start_sound_play(g_LeafCall, filename, IsRepeat, proc);
		return TRUE;
    }
	return FALSE;	
}

/*************************************************
  Function:			media_stop_sound
  Description:		停止播放
  Input: 			无
  Output:			无
  Return:			
  Others:
*************************************************/
void media_stop_sound (void)
{
	leaf_stop_sound_play(g_LeafCall);
}

/*************************************************
  Function:			media_start_rtsp
  Description:		开启rtsp
  Input: 			无
  Output:			无
  Return:			
  Others:
*************************************************/
int media_start_rtsp(V_RECT_S rect, char *Url, void *callback_func)
{}

/*************************************************
  Function:			media_stop_rtsp
  Description:		关闭rtsp
  Input: 			无
  Output:			无
  Return:			
  Others:
*************************************************/
int media_stop_rtsp(void)
{
	return TRUE;
}	

/*************************************************
  Function:			media_play_lyly
  Description:		播放留影留言文件
  Input: 			
  	1.filename		文件名
  	2.proc			回调
  Output:			无
  Return:			TRUE/FALSE
  Others:
*************************************************/
uint32 media_play_lyly (char *filename, void * proc)
{
	leaf_start_video_memo_playback(g_LeafCall, filename);
	return TRUE;
}

/*************************************************
  Function:			media_stop_lyly
  Description:		停止留影留言播放
  Input: 			无
  Output:			无
  Return:			
  Others:
*************************************************/
void media_stop_lyly (void)
{
	leaf_stop_video_memo_playback(g_LeafCall);
}

/*************************************************
  Function:		media_clear_fb
  Description: 	清空fb
  Input: 		无
  Output:		无
  Return:		无
  Others:
*************************************************/
int32 media_clear_fb(void)
{
	return TRUE;
}

/*************************************************
  Function:		init_media
  Description: 	媒体初始化
  Input: 		无
  Output:		无
  Return:		无
  Others:
*************************************************/
void media_init(void)
{
	// 开启
	#if MEDIA_DEBUG_PRINT
	int ret = putenv("MEDIASTREAMER_DEBUG=1");
	dprintf("ret: %d\n", ret);
	#endif

	if (g_LeafCall == NULL)
	{
		g_LeafCall = leaf_init();
	}
}

