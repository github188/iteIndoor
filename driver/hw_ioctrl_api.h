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
#ifndef __IOCTRL_API_H__
#define __IOCTRL_API_H__	
#include "driver_include.h"

#define ALM_LED				2140
#define MSG_LED				2141
#define MIC_SWITCH			2142
#define SPK_SWITCH			2143
#define MK_LOCK				2144
#define MK_PWR				2145
#define MK_VIDEO_CTRL		2146
#define RST_TW9912			2147
#define LCD_PWR				2148
#define LCD_BACK			2149
#define RST_FM2018			2150
#define SPK_CTRL			2151
#define MC34118_CTRL		2152
#define KEY_BEEP			2153
#define MK_RING				2154
#define PDN_TW9912			2155
#define TALK_VOLUME_SET		2156
#define KEY_LED				2157
#define CTRL_JDQ			2158
#define CTRL_485			2159
#define OPEN_LOCK			2160
#define CARD_PWR			2161
#define RST_PHY				2162
#define GET_ZONE_STATUS		2163

#define	MK_CALL_DOWN		0x01

#if 0
typedef enum
{
	LED_ALARM = 0x00,			// �����������
	LED_CENTER,					// �������ļ������
	LED_MONITOR,				// ���Ӽ������
	LED_LOCK,					// �����������
	LED_TALK,					// ͨ���������
	LED_ALL						// ���м������
}LED_TYPE;

typedef enum
{
	LED_ON,						// ��
	LED_OFF,					// ��
	LED_FLASH					// ��
}LED_STATE;
#endif

typedef	void (*IO_HOOK)(uint8 event);

/*************************************************
  Function:		hw_mic_switch_analog
  Description: 	��ͷ�л���ģ������(����������ǰ��ͨ��)
  Input: 		��	
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_mic_switch_analog(void);

/*************************************************
  Function:		hw_mic_switch_digit
  Description: 	��ͷ�л�����������(����������ͨ��)
  Input: 		��	
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_mic_switch_digit(void);

/*************************************************
  Function:		hw_spk_switch_analog
  Description: 	�����л���ģ�����(����������ǰ��ͨ��)
  Input: 		��	
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_spk_switch_analog(void);

/*************************************************
  Function:		hw_spk_switch_digit
  Description: 	�����л����������(����������ͨ��)
  Input: 		��	
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_spk_switch_digit(void);

/*************************************************
  Function:		hw_mk_lock
  Description: 	�ſڻ�����
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_mk_lock(void);

/*************************************************
  Function:		hw_mk_power_on
  Description: 	���ſڻ���Դ
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_mk_power_on(void);

/*************************************************
  Function:		hw_mk_power_off
  Description: 	�ر��ſڻ���Դ
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_mk_power_off(void);

/*************************************************
  Function:		hw_mk_video_on
  Description: 	���ſڻ���Ƶ
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_mk_video_on(void);

/*************************************************
  Function:		hw_mk_video_off
  Description: 	�ر��ſڻ���Ƶ
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_mk_video_off(void);

/*************************************************
  Function:		hw_mk_talk
  Description: 	�ſڻ�ͨ������
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_mk_talk(void);

/*************************************************
  Function:		hw_mk_start
  Description:  ��ǰ����������
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_mk_start(void);

/*************************************************
  Function:		hw_mk_ring_back
  Description:  ���ſڻ�����
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_mk_ring_back(void);

/*************************************************
  Function:		hw_reset_tw9912
  Description: 	��λtw9912
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_reset_tw9912(void);

/*************************************************
  Function:		hw_reset_fm2018
  Description: 	��λfm2018
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_reset_fm2018(void);

/*************************************************
  Function:		hw_mc34118_on
  Description:  ��mc34118
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_mc34118_on(void);

/*************************************************
  Function:		hw_mc34118_off
  Description:  �ر�mc34118
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_mc34118_off(void);

/*************************************************
  Function:		hw_tw9912_power_on
  Description: 	��tw9912
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_tw9912_power_on(void);

/*************************************************
  Function:		hw_tw9912_power_down
  Description: 	�ر�tw9912
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_tw9912_power_down(void);

/*************************************************
  Function:		hw_alarm_led_on
  Description: 	��������
  Input: 		��	
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_alarm_led_on(void);

/*************************************************
  Function:		hw_alarm_led_off
  Description: 	��������
  Input: 		��	
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_alarm_led_off(void);

/*************************************************
  Function:		hw_alarm_led_glint
  Description: 	��������˸
  Input: 		��	
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_alarm_led_glint(void);

/*************************************************
  Function:		hw_msg_led_on
  Description: 	��Ϣָʾ����
  Input: 		��	
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_message_led_on(void);

/*************************************************
  Function:		hw_msg_led_off
  Description: 	��Ϣָʾ����
  Input: 		��	
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_message_led_off(void);

/*************************************************
  Function:		hw_message_led_glint
  Description: 	��Ϣָʾ��˸
  Input: 		��	
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_message_led_glint(void);

/*************************************************
  Function:		hw_key_led_on
  Description:	�����̱����
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void hw_key_led_on(void);

/*************************************************
  Function:		hw_key_led_off
  Description:	����̱����
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void hw_key_led_off(void);

/*************************************************
  Function:		hw_key_led_flash
  Description:	���̱������1����1��
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void hw_key_led_flash(void);

/*************************************************
  Function:		hw_ext_on
  Description:	���ⲿ�̵���
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void hw_ext_on(void);

/*************************************************
  Function:		hw_ext_off
  Description:	�ر��ⲿ�̵���
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void hw_ext_off(void);

/*************************************************
  Function:		hw_dianti_ctrl
  Description:	���ݿ���
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void hw_dianti_ctrl(void);

/*************************************************
  Function:		hw_485_send
  Description:	485����ʹ��
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void hw_485_send(void);

/*************************************************
  Function:		hw_485_recv
  Description:	485����ʹ��
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void hw_485_recv(void);

/*************************************************
  Function:		hw_lcd_power_on
  Description: 	������Դ
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_lcd_power_on(void);

/*************************************************
  Function:		hw_lcd_power_off
  Description:  �ر�����Դ
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_lcd_power_off(void);

/*************************************************
  Function:		hw_lcd_back_on
  Description: 	�����⿪
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_lcd_back_on(void);

/*************************************************
  Function:		hw_lcd_back_off
  Description:  ������ر�
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_lcd_back_off(void);

/*************************************************
  Function:		hw_speak_on
  Description: 	��SPK
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_speak_on(void);

/*************************************************
  Function:		hw_speak_off
  Description:  �ر�SPK
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_speak_off(void);

/*************************************************
  Function:		hw_key_beep
  Description:  �������
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_key_beep(void);

/*************************************************
  Function:		hw_set_volume
  Description: 	����ģ����ǰ��ͨ������(1-8��)
  Input: 		��	
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_set_volume(uint8 level);

/*************************************************
  Function:		hw_switch_digit
  Description: 	����ͨ·�л�
  Input: 		��	
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_switch_digit(void);

/*************************************************
  Function:		hw_reset_dev
  Description: 	��λ�ⲿ�豸(������������)
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_reset_dev(void);

/*************************************************
  Function:		hw_get_zone_status
  Description: 	��ȡ����״̬
  Input: 		��
  Output:		��
  Return:		�ɹ���ʧ��
  Others:		
*************************************************/
int32 hw_get_zone_status(void);

/*************************************************
  Function:		init_ioctrl_callback
  Description:	��ʼ��ioctrl�ص�����
  Input: 
	1.func		ioctrl�ص�����ָ��
  Output:
  Return:		�ɹ�=0������ֵʧ��
  Others:		�ص�
*************************************************/
int32 init_ioctrl_callback(IO_HOOK func);
#endif

