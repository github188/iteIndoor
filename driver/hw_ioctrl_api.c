/********************************************************
  Copyright (C), 2006-2016, Aurine
  File name:	hw_ioctrl_api.c
  Author:		�޷�»
  Version:		V2.0
  Date:			2014-09-28
  Description:	4.3�����ڻ�GPIO����Ӧ�ó����װ
  
  History:        
    1. Date:
       Author:
       Modification:
    2. ...
*********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>			
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include "driver_include.h"

static int32 Fd = 0;
static uint8 InitFlag = 0;
static uint32 ReadBuf = 0;
static IO_HOOK IoEventFunc = NULL;

#ifdef _DOOR_PHONE_
/*************************************************
  Function:		hw_mic_switch_analog
  Description: 	��ͷ�л���ģ������(����������ǰ��ͨ��)
  Input: 		��	
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_mic_switch_analog(void)
{
	if (!InitFlag)
	{
		return -1;
	}
	//ioctl(Fd, MIC_SWITCH, 1);

	return 0;
}

/*************************************************
  Function:		hw_mic_switch_digit
  Description: 	��ͷ�л�����������(����������ͨ��)
  Input: 		��	
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_mic_switch_digit(void)
{
	if (!InitFlag)
	{
		return -1;
	}
	//ioctl(Fd, MIC_SWITCH, 0);

	return 0;
}

/*************************************************
  Function:		hw_spk_switch_analog
  Description: 	�����л���ģ�����(����������ǰ��ͨ��)
  Input: 		��	
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_spk_switch_analog(void)
{
	if (!InitFlag)
	{
		return -1;
	}
	//ioctl(Fd, SPK_SWITCH, 1);

	return 0;
}

/*************************************************
  Function:		hw_spk_switch_digit
  Description: 	�����л����������(����������ͨ��)
  Input: 		��	
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_spk_switch_digit(void)
{
	if (!InitFlag)
	{
		return -1;
	}
	//ioctl(Fd, SPK_SWITCH, 0);

	return 0;
}

/*************************************************
  Function:		hw_mk_lock
  Description: 	�ſڻ�����
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_mk_lock(void)
{
	if (!InitFlag)
	{
		return -1;
	}
	//ioctl(Fd, MK_LOCK, 1);

	return 0;
}

/*************************************************
  Function:		hw_mk_power_on
  Description: 	���ſڻ���Դ
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_mk_power_on(void)
{
	if (!InitFlag)
	{
		return -1;
	}
	//ioctl(Fd, MK_PWR, 0);

	return 0;
}

/*************************************************
  Function:		hw_mk_power_off
  Description: 	�ر��ſڻ���Դ
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_mk_power_off(void)
{
	if (!InitFlag)
	{
		return -1;
	}
	//ioctl(Fd, MK_PWR, 1);

	return 0;
}

/*************************************************
  Function:		hw_mk_video_on
  Description: 	���ſڻ���Ƶ
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_mk_video_on(void)
{
	if (!InitFlag)
	{
		return -1;
	}
	//ioctl(Fd, MK_VIDEO_CTRL, 0);

	return 0;
}

/*************************************************
  Function:		hw_mk_video_off
  Description: 	�ر��ſڻ���Ƶ
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_mk_video_off(void)
{
	if (!InitFlag)
	{
		return -1;
	}
	//ioctl(Fd, MK_VIDEO_CTRL, 1);

	return 0;
}

/*************************************************
  Function:		hw_mk_talk
  Description: 	�ſڻ�ͨ������
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_mk_talk(void)
{
	hw_mc34118_on();
	hw_mic_switch_analog();
	hw_spk_switch_analog();
	//hw_speak_on();
	//hw_mk_power_on();
	//hw_mk_video_on();
}

/*************************************************
  Function:		hw_mk_start
  Description:  ��ǰ����������
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_mk_start(void)
{
	hw_mc34118_off();
	hw_mk_power_on();
	hw_mk_video_on();
}

/*************************************************
  Function:		hw_mk_ring_back
  Description:  ���ſڻ�����
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_mk_ring_back(void)
{
	if (!InitFlag)
	{
		return -1;
	}
	//ioctl(Fd, MK_RING, 0);

	return 0;
}

/*************************************************
  Function:		hw_reset_tw9912
  Description: 	��λtw9912
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_reset_tw9912(void)
{
	if (!InitFlag)
	{
		return -1;
	}
	//ioctl(Fd, RST_TW9912, 1);

	return 0;
}

/*************************************************
  Function:		hw_reset_fm2018
  Description: 	��λfm2018
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_reset_fm2018(void)
{
	if (!InitFlag)
	{
		return -1;
	}
	//ioctl(Fd, RST_FM2018, 1);

	return 0;
}

/*************************************************
  Function:		hw_mc34118_on
  Description:  ��mc34118
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_mc34118_on(void)
{
	if (!InitFlag)
	{
		return -1;
	}
	//ioctl(Fd, MC34118_CTRL, 1);

	return 0;
}

/*************************************************
  Function:		hw_mc34118_off
  Description:  �ر�mc34118
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_mc34118_off(void)
{
	if (!InitFlag)
	{
		return -1;
	}
	//ioctl(Fd, MC34118_CTRL, 0);

	return 0;
}

/*************************************************
  Function:		hw_tw9912_power_on
  Description: 	��tw9912
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_tw9912_power_on(void)
{
	dprintf("hw_tw9912_power_on\n");
	if (!InitFlag)
	{
		return -1;
	}
	//ioctl(Fd, PDN_TW9912, 0);

	return 0;
}

/*************************************************
  Function:		hw_tw9912_power_down
  Description: 	�ر�tw9912
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_tw9912_power_down(void)
{
	dprintf("hw_tw9912_power_down\n");
	if (!InitFlag)
	{
		return -1;
	}
	//ioctl(Fd, PDN_TW9912, 1);

	return 0;
}
#endif

/*************************************************
  Function:		GetBit
  Description: 	
  Input: 		��	
  Output:		��
  Return:		��
  Others:		
*************************************************/
static uint32 GetBit(uint32 value, uint8 nBit)
{
	uint32 i = 1;
	uint32 BitValue = 0;
	
	BitValue = value & (i<<nBit);
	//dprintf("%s : BitValue = %d\n", __FUNCTION__, BitValue);
	return BitValue;
}

/*************************************************
  Function:		hw_alarm_led_on
  Description: 	��������
  Input: 		��	
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_alarm_led_on(void)
{
	if (!InitFlag)
	{
		return -1;
	}
	//ioctl(Fd, ALM_LED, 0);							// 1-�� 0-�� 2-��˸

	return 0;
}

/*************************************************
  Function:		hw_alarm_led_off
  Description: 	��������
  Input: 		��	
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_alarm_led_off(void)
{
	if (!InitFlag)
	{
		return -1;
	}
	//ioctl(Fd, ALM_LED, 1);

	return 0;
}

/*************************************************
  Function:		hw_alarm_led_glint
  Description: 	��������˸
  Input: 		��	
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_alarm_led_glint(void)
{
	if (!InitFlag)
	{
		return -1;
	}
	//ioctl(Fd, ALM_LED, 2);

	return 0;
}

/*************************************************
  Function:		hw_msg_led_on
  Description: 	��Ϣָʾ����
  Input: 		��	
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_message_led_on(void)
{
	if (!InitFlag)
	{
		return -1;
	}
	//ioctl(Fd, MSG_LED, 0);							// 1-�� 0-�� 2-��˸

	return 0;
}

/*************************************************
  Function:		hw_msg_led_off
  Description: 	��Ϣָʾ����
  Input: 		��	
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_message_led_off(void)
{
	if (!InitFlag)
	{
		return -1;
	}
	//ioctl(Fd, MSG_LED, 1);

	return 0;
}

/*************************************************
  Function:		hw_message_led_glint
  Description: 	��Ϣָʾ��˸
  Input: 		��	
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_message_led_glint(void)
{
	if (!InitFlag)
	{
		return -1;
	}
	//ioctl(Fd, MSG_LED, 2);
	//dprintf("%s : msg led glint\n", __FUNCTION__);
	
	return 0;	
}

/*************************************************
  Function:		hw_key_led_on
  Description:	�����̱����
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void hw_key_led_on(void)
{	
	if (!InitFlag)
	{
		return ;
	}
	
	ioctl(ITP_DEVICE_IOCTRL, ITP_IOCTL_KEY_LED_ON, NULL);
}

/*************************************************
  Function:		hw_key_led_off
  Description:	����̱����
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void hw_key_led_off(void)
{	
	if (!InitFlag)
	{
		return ;
	}
	
	ioctl(ITP_DEVICE_IOCTRL, ITP_IOCTL_KEY_LED_OFF, NULL);
}

/*************************************************
  Function:		hw_key_led_flash
  Description:	���̱������1����1��
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void hw_key_led_flash(void)
{
	if (!InitFlag)
	{
		return ;
	}
	
	ioctl(ITP_DEVICE_IOCTRL, ITP_IOCTL_KEY_LED_FLASH, NULL);
}

/*************************************************
  Function:		hw_ext_on
  Description:	���ⲿ�̵���
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void hw_ext_on(void)
{
	if (!InitFlag)
	{
		return ;
	}
	
	//ioctl(Fd, CTRL_JDQ, 1);
}

/*************************************************
  Function:		hw_ext_off
  Description:	�ر��ⲿ�̵���
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void hw_ext_off(void)
{
	if (!InitFlag)
	{
		return ;
	}
	
	//ioctl(Fd, CTRL_JDQ, 0);
}

/*************************************************
  Function:		hw_dianti_ctrl
  Description:	���ݿ���
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void hw_dianti_ctrl(void)
{
	#if (SYS_TYPE != E81_FZJB_VER)
	hw_ext_on();
	usleep(100*1000);
	hw_ext_off();
	#endif
}

/*************************************************
  Function:		hw_485_send
  Description:	485����ʹ��
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void hw_485_send(void)
{
	if (!InitFlag)
	{
		return ;
	}
	
	//ioctl(Fd, CTRL_485, 0);
}

/*************************************************
  Function:		hw_485_recv
  Description:	485����ʹ��
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void hw_485_recv(void)
{
	if (!InitFlag)
	{
		return ;
	}
	
	//ioctl(Fd, CTRL_485, 1);
}

/*************************************************
  Function:		hw_lcd_power_on
  Description: 	������Դ
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_lcd_power_on(void)
{
	if (!InitFlag)
	{
		return -1;
	}
	//ioctl(Fd, LCD_PWR, 0);
	ioctl(ITP_DEVICE_IOCTRL, ITP_IOCTL_LCD_POWER_ON, NULL);
	return 0;
}

/*************************************************
  Function:		hw_lcd_power_off
  Description:  �ر�����Դ
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_lcd_power_off(void)
{
	if (!InitFlag)
	{
		return -1;
	}
	//ioctl(Fd, LCD_PWR, 1);
	ioctl(ITP_DEVICE_IOCTRL, ITP_IOCTL_LCD_POWER_OFF, NULL);
	return 0;
}

/*************************************************
  Function:		hw_lcd_back_on
  Description: 	�����⿪
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_lcd_back_on(void)
{
	if (!InitFlag)
	{
		return -1;
	}
	//ioctl(Fd, LCD_BACK, 0);
	ioctl(ITP_DEVICE_BACKLIGHT, ITP_IOCTL_ON, NULL);
	return 0;
}

/*************************************************
  Function:		hw_lcd_back_off
  Description:  ������ر�
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_lcd_back_off(void)
{
	if (!InitFlag)
	{
		return -1;
	}
	//ioctl(Fd, LCD_BACK, 1);
	ioctl(ITP_DEVICE_BACKLIGHT, ITP_IOCTL_OFF, NULL);
	return 0;
}

/*************************************************
  Function:		hw_speak_on
  Description: 	��SPK
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_speak_on(void)
{
	dprintf("open_speaker\n");
	if (!InitFlag)
	{
		return -1;
	}
	//ioctl(Fd, SPK_CTRL, 0);
	ioctl(ITP_DEVICE_IOCTRL, ITP_IOCTL_386SPK_ON, NULL);
	return 0;
}

/*************************************************
  Function:		hw_speak_off
  Description:  �ر�SPK
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_speak_off(void)
{
	dprintf("close_speaker\n");
	if (!InitFlag)
	{
		return -1;
	}
	//ioctl(Fd, SPK_CTRL, 1);
	ioctl(ITP_DEVICE_IOCTRL, ITP_IOCTL_386SPK_OFF, NULL);
	return 0;
}

/*************************************************
  Function:		hw_key_beep
  Description:  �������
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_key_beep(void)
{
	if (!InitFlag)
	{
		return -1;
	}
	//ioctl(Fd, KEY_BEEP, 0);
	//dprintf("key beep!\n");
	ioctl(ITP_DEVICE_IOCTRL, ITP_IOCTL_BEEP_PLAY, NULL);
	return 0;
}

/*************************************************
  Function:		hw_set_volume
  Description: 	����ģ����ǰ��ͨ������(1-8��)
  Input: 		��	
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_set_volume(uint8 level)
{
	#ifdef _DOOR_PHONE_
	if (!InitFlag)
	{
		return -1;
	}
	//ioctl(Fd, TALK_VOLUME_SET, level);
	#endif
	
	return TRUE;
}

/*************************************************
  Function:		hw_switch_digit
  Description: 	����ͨ·�л�
  Input: 		��	
  Output:		��
  Return:		�ɹ���ʧ��
  Others:	
  				3157_Ctr  0
				SPK_CTL   0
				3005_Ctr  0
				MK_POW	  1
				ON_34118  0
*************************************************/
int32 hw_switch_digit(void)
{
	#ifdef _DOOR_PHONE_
	hw_mic_switch_digit();
	hw_spk_switch_digit();
	//hw_speak_on();
	hw_mk_video_off();
	hw_mk_power_off();
	hw_mc34118_off();
	#endif

	return TRUE;
}

/*************************************************
  Function:		hw_reset_dev
  Description: 	��λ�ⲿ�豸(������������)
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_reset_dev(void)
{
#if 0
	if (!InitFlag)
	{
		return -1;
	}
	ioctl(Fd, RST_PHY, 0);
#endif	
	return 0;
}

/*************************************************
  Function:		hw_get_zone_status
  Description: 	��ȡ����״̬
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_get_zone_status(void)
{
	if (!InitFlag)
	{
		return -1;
	}

	ioctl(ITP_DEVICE_IOCTRL, ITP_IOCTL_GET_ZONE_STATUS, NULL);
	//sleep(1);
	//ioctl(Fd, GET_ZONE_STATUS, 0);
	
	return 0;
}

/*************************************************
  Function:		*check_ioctrl
  Description:	���IO�������
  Input: 
	1.			����ָ��
  Output:		��
  Return:		��
  Others:		�ص�
*************************************************/
static void * check_ioctrl(void* data)
{
	int32_t ret;
	
	while (1)
	{
		ret = read(ITP_DEVICE_IOCTRL, &ReadBuf, sizeof(ReadBuf));
		if (ret == sizeof(ReadBuf))
		{
			if (IoEventFunc)
			{
				
				#ifdef _DOOR_PHONE_					// ģ����ǰ�����д���,һֱ��ס�����Ǳ�ֻ��һ��
				if (GetBit(ReadBuf, 0))
				{
					IoEventFunc(MK_CALL_DOWN);
					dprintf("Call Down!\n");
					dprintf("ReadBuf: %d\n", ReadBuf);
				}
				#elif defined _ALARM_IO_			// 8�������
				IoEventFunc(ReadBuf&0xFF);
				#elif defined _NET_DOOR_			// ������ǰ�������¼����
				IoEventFunc(ReadBuf&0x0F);
				#endif
				
			}
		}
        usleep(1000);
		// sleep(1);
	}
}

/*************************************************
  Function:		init_ioctrl_callback
  Description:	��ʼ��ioctrl�ص�����
  Input: 
	1.func		ioctrl�ص�����ָ��
  Output:
  Return:		�ɹ�=0������ֵʧ��
  Others:		�ص�
*************************************************/
int32 init_ioctrl_callback(IO_HOOK func)
{

	pthread_t th;
	pthread_attr_t new_attr;

	if (!InitFlag)
	{
		// modi by linp 2016-08-17
		//Fd = open("/dev/w55fa92_ioctrl", O_RDONLY);
		//if (Fd == -1)
		//{
		//	perror("IOCTRL");
		//	return -1;
		//}
		InitFlag = 1;
	}
	if (func != NULL)
	{
		printf("pthread_create check_ioctrl!!!\n");
		IoEventFunc = func;
		pthread_attr_init (&new_attr);
		pthread_attr_setdetachstate (&new_attr, PTHREAD_CREATE_DETACHED);
		//pthread_attr_setstacksize (&new_attr, 48*1024);
		pthread_create(&th, &new_attr, check_ioctrl, NULL);
	}
	return 0;
}


