/*************************************************
  Copyright (C), 2006-2010, Aurine
  File name: 	logic_media.c
  Author:   	�±���
  Version:  	2.0
  Date: 		2016-06-30
  Description:  ��ý��ӿں���
				
  History:        
    1. Date:
       Author:
       Modification:
    2. ...
    ����ֵͳһ -1 ���� 0 ��ȷ
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

// modi by luofl 2014-08-28 ����������󼶳�����׼85DB
uint8 AudioVolumeLevel[9] = {0, 7, 10, 13, 15, 17, 19, 20, 21};

// ============V5���ڻ�(���������ڻ��������������)========================
uint8 V5_STAIR_AudioVolumeLevel[9] = {0, 10, 16, 21, 22, 23, 24, 25, 26};
uint8 V5_CENTER_AudioVolumeLevel[9] = {0, 10, 13, 16, 18, 20, 21, 22, 23};
uint8 V5_NETDOOR_AudioVolumeLevel[9] = {0, 10, 16, 18, 20, 22, 24, 25, 26};
uint8 V5_ROOM_AudioVolumeLevel[9] = {0, 10, 15, 19, 21, 22, 23, 24, 25};

static struct ThreadInfo VideoPreviewThread;		// ģ����ǰ����ƵԤ���߳�


/*************************************************
  Function:    		media_Ad_set_audio_PackMode
  Description: 		������Ƶ���Ͱ� �����ʽ
  Input: 			
  	1.packmode		0: �������� 1: �������
  Output:			��
  Return:			
  Others:
*************************************************/
void media_Ad_set_audio_PackMode(uint8 packmode)
{
	//set_audio_packmode(packmode);
}

/*************************************************
  Function:			media_stream_FileExtCmp
  Description:		��ѯ�ļ�����
  Input:
  Output:			��
  Return:			�ɹ���ʧ��
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
  Description:		ģ����ƵԤ���߳�
  Input:
  Output:			��
  Return:			�ɹ���ʧ��
  Others:
*************************************************/
static void* media_start_analog_video_proc(void *param)
{
	// ���÷����߳�
	pthread_detach(pthread_self());
	uint8_t * pu8PicPtr;
	uint64_t u64TS;
	uint32_t u32PicPhyAdr;

	while (VideoPreviewThread.running)
	{
		
	}

thread_end:
	pthread_exit(NULL);
	return NULL;             // ���غ���Դ��ϵͳ�ͷ�
}

/*************************************************
  Function:			media_start_analog_video
  Description:		����ģ����ƵԤ��
  Input:
  Output:			��
  Return:			�ɹ���ʧ��
  Others:
*************************************************/
uint32 media_start_analog_video(void)
{
	return TRUE;
}

/*************************************************
  Function:			media_stop_analog_video
  Description:		ֹͣģ����ƵԤ��
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
void media_stop_analog_video(void)
{

	return;
}

/*************************************************
  Function:			media_set_analog_video_win
  Description:		����ģ����ƵԤ������
  Input:
  	1.flag			1-ȫ����ʾ, ����Ϊ������ʾ
  Output:			��
  Return:			��
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
  Description:		����ģ���豸ͨ������
  Input: 
  	vol				�����ȼ�(0���� - 8���)
  Output:			��
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
  Description:		��ȡwav�ļ�����ʱ��
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
  Description:		����������Ƶ����
  Input: 	
  	1.addres		�Զ˵�ַ
  	2.mode			��Ƶģʽ ���͡�����
  Output:			��
  Return:			�ɹ���ʧ��
  Others:
*************************************************/
uint32 media_start_net_video(uint32 address, uint8 mode)
{
	return TRUE;
}

/*************************************************
  Function:			media_stop_net_video
  Description:		ֹͣ������Ƶ����
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
void media_stop_net_video(uint8 mode)
{	
	return;
}

/*************************************************
  Function:			media_start_net_audio
  Description:		����������Ƶ����
  Input: 			
  	1.tp			��Ƶ��ʽ
  	2.proc			�ص�
  Output:			��
  Return:			TRUE/FALSE
  Others:
*************************************************/
uint32 media_start_net_audio(int address)
{
	return TRUE;
}

/*************************************************
  Function:			media_stop_net_audio
  Description:		ֹͣ������Ƶ����
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
void media_stop_net_audio(void)
{
	dprintf("net_stop_audio\n");
	return;
}

/*************************************************
  Function:			meida_start_net_hint
  Description:		����������ʾ������
  Input: 			
  	1.tp			���͵ĸ�ʽ
  	2.filename		��ʾ���ļ�
  Output:			��
  Return:			TRUE/FALSE
  Others:
*************************************************/
uint32 meida_start_net_hint(PAYLOAD_TYPE_E tp, char *filename, void * proc)
{
	return TRUE;
}

/*************************************************
  Function:			meida_stop_net_hint
  Description:		ֹͣ��ʾ������
  Input: 			��
  Output:			��
  Return:			��
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
  	1.mode			¼�Ƶ�ģʽ
  	2.atp			��Ƶ��ʽ
  	3.vtp			��Ƶ��ʽ
  	4.filename		������ļ���
  Output:			��
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
  Output:			��
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
  Description:		������������¼��
  Input: 			
  	1.mode			¼�Ƶ�ģʽ
  	2.atp			��Ƶ��ʽ
  	3.vtp			��Ƶ��ʽ
  	4.filename		������ļ���
  Output:			��
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
  Description:		ֹͣ¼��
  Input: 			
  	1.issave		�Ƿ񱣴�(1����, 0������)
  Output:			��
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
  Description: 		������Ƶ���͵�ַ
  Input: 			
  	1.IP			IP��ַ
  	2.AudioPort		��Ƶ�˿�
  Output:			��
  Return:			�ɹ����true/false
  Others:
*************************************************/
int32 media_add_audio_sendaddr(uint32 IP, uint16 AudioPort)
{
	return TRUE;
}

/*************************************************
  Function:			media_start_local_record
  Description:		��������¼��
  Input: 			
  	1.filename		�ļ���
  	2.maxtime		¼�Ƶ��ʱ��
  	3.proc			�ص�
  Output:			��
  Return:			TRUE/FALSE
  Others:
*************************************************/
uint32 media_start_local_record(char *filename)
{
	return TRUE;
}

/*************************************************
  Function:			media_stop_local_record
  Description:		ֹͣ����¼��
  Input: 			
  	1.issave		�Ƿ񱣴�
  Output:			��
  Return:			TRUE/FALSE
  Others:
*************************************************/
void media_stop_local_record(void)
{
}

/*************************************************
  Function:			media_del_audio_send_addr
  Description:		ɾ����Ƶ�ķ��͵�ַ�Ͷ˿�
  Input: 
  Output:			��
  Return:			��
  Others:
*************************************************/
void media_del_audio_send_addr(uint32 IP, uint16 AudioPort)
{	
	return;
}

/*************************************************
  Function:			media_set_talk_volume
  Description:		�������ֶԽ�����
  Input: 
  	vol				�����ȼ�(0���� - 8���)
  Output:			��
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
  Description:		������������
  Input: 
  	vol				�����ȼ�(0���� - 8���)
  Output:			��
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
  Description:		������Ƶcodec��������
  Input: 
  	vol				�����ȼ�
  Output:			��
  Return:			TRUE/FALSE
  Others:
*************************************************/
uint32 media_set_mic_volume(uint8 vol)
{
	return TRUE;
}

/*************************************************
  Function:			media_fill_video_data
  Description:		��������Ƶ�����buf
  Input: 
  	1.data			����ָ��
  	2.len			���ݳ���
  	3.time			ʱ���
  Output:			��
  Return:			TRUE/FALSE
  Others:
*************************************************/
uint32 media_fill_video_data(uint8 *data, uint32 len, uint32 time)
{
#if 0
	uint32 ret;
	
	if (LeaveFlag & 0x02)							// ��Ӱ����
	{
		leave_fill_data(data, len, time, 1);		// �����Ƶ����
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
  Description:		ȫ����ʾ��Ƶ
  Input: 			
  	1.devtype		�豸����
  	2.flg			1 ȫ�� 0����
  Output:			��
  Return:			TRUE/FALSE
  Others:
*************************************************/
uint32 media_full_dispaly_video(DEVICE_TYPE_E devtype, uint8 flg)
{
	int32 ret = FALSE;
	if (devtype == DEVICE_TYPE_DOOR_PHONE) // ģ����ǰ��
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
  Description:		ȫ����ʾ��Ƶ
  Input: 			
   	2.flg			1 ȫ�� 0����
  Output:			��
  Return:			TRUE/FALSE
  Others:
*************************************************/
int media_rtsp_set_full_screen(uint8 flg)
{
	return TRUE;
}

/*************************************************
  Function:			media_beep
  Description:		���Ű�����
  Input: 			��
  Output:			��
  Return:			
  Others:
*************************************************/
void media_beep(void)
{
	
}

/*************************************************
  Function:			media_snapshot
  Description:		ץ��
  Input: 			
  	1.filename		ͼ�񱣴���ļ���
  					���>1ʱ�ļ����Զ����ϱ��
	2.dstW			Ŀ��ͼƬ�Ŀ�
	3.dstH			Ŀ��ͼƬ�ĸ�
  Output:			��
  Return:			TRUE/FALSE
  Others:
*************************************************/
uint32 media_snapshot(char *filename, uint32 dstW, uint32 dstH, DEVICE_TYPE_E DevType)
{
#if 0
	// ģ��״̬�� ��Ƶδ������ץ��ֱ�ӷ��ش���
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
  Description:		JPGͼƬ��ʾ
  Input: 			
  	1.filename		ͼ�񱣴���ļ���
  					���>1ʱ�ļ����Զ����ϱ��
	2.pos_x			Ŀ��ͼƬ��ʾX ����
	3.pos_y			Ŀ��ͼƬ��ʾY ����
	4.with			Ŀ��ͼƬ��ʾ���
	5.heigh			Ŀ��ͼƬ��ʾ����
  Output:			��
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
  Description:		�ر�ͼƬ��ʾ
  Input: 			
  Output:			��
  Return:			TRUE/FALSE
  Others:
*************************************************/
void media_stop_show_pict(void)
{
	sys_set_view_picture_state(FALSE);	
}

/*************************************************
  Function:			media_enable_audio_aec
  Description:		ʹ���������ӿ�
  Input: 			
  Output:			��
  Return:			TRUE/FALSE
  Others:
*************************************************/
int media_enable_audio_aec(void)
{
	return TRUE;
}

/*************************************************
  Function:			media_disable_audio_aec
  Description:		ʹ���������ӿ�
  Input: 			
  Output:			��
  Return:			TRUE/FALSE
  Others:
*************************************************/
int media_disable_audio_aec(void)
{
	return TRUE;
}

/*************************************************
  Function:			media_play_sound
  Description:		������Ƶ�ļ�
  Input: 			
  	1.type			��������
  	2.filename		�ļ���
  	3.isrepeat		�Ƿ��ظ�����	1�ظ� 0���ظ�
  	4.proc			�ص�
  Output:			��
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
  Description:		ֹͣ����
  Input: 			��
  Output:			��
  Return:			
  Others:
*************************************************/
void media_stop_sound (void)
{
	leaf_stop_sound_play(g_LeafCall);
}

/*************************************************
  Function:			media_start_rtsp
  Description:		����rtsp
  Input: 			��
  Output:			��
  Return:			
  Others:
*************************************************/
int media_start_rtsp(V_RECT_S rect, char *Url, void *callback_func)
{}

/*************************************************
  Function:			media_stop_rtsp
  Description:		�ر�rtsp
  Input: 			��
  Output:			��
  Return:			
  Others:
*************************************************/
int media_stop_rtsp(void)
{
	return TRUE;
}	

/*************************************************
  Function:			media_play_lyly
  Description:		������Ӱ�����ļ�
  Input: 			
  	1.filename		�ļ���
  	2.proc			�ص�
  Output:			��
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
  Description:		ֹͣ��Ӱ���Բ���
  Input: 			��
  Output:			��
  Return:			
  Others:
*************************************************/
void media_stop_lyly (void)
{
	leaf_stop_video_memo_playback(g_LeafCall);
}

/*************************************************
  Function:		media_clear_fb
  Description: 	���fb
  Input: 		��
  Output:		��
  Return:		��
  Others:
*************************************************/
int32 media_clear_fb(void)
{
	return TRUE;
}

/*************************************************
  Function:		init_media
  Description: 	ý���ʼ��
  Input: 		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void media_init(void)
{
	// ����
	#if MEDIA_DEBUG_PRINT
	int ret = putenv("MEDIASTREAMER_DEBUG=1");
	dprintf("ret: %d\n", ret);
	#endif

	if (g_LeafCall == NULL)
	{
		g_LeafCall = leaf_init();
	}
}

