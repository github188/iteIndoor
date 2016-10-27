/*************************************************
  Copyright (C), 2006-2016, Aurine
  File name:  	AppConfig.h
  Author:     	luofl
  Version:    	2.0
  Date: 
  Description:  N32926������ʵ�ֵĹ��ܣ�ͨ���궨����������Ӧ�Ĺ��ܣ�
  				#define	_ELEVATOR_MODE_				// ������Ȩ����
  				#define _DOOR_PHONE_				// ģ����ǰ��
*************************************************/
#ifndef __APPCONFIG_H__
#define __APPCONFIG_H__

//#define AEC_DEBUG									// �����������ļ�
#define SCREEN_CAPTURE			0					// ��Ļ��ͼ,���¿�������һ�ű�����D���С�

// ====������ʾ������========================================
#define MIC_VOLUME				4					// ��ͷ��������
#define MSG_HIT_VOL				4					// ��Ϣ֪ͨ��ʾ����С
#define AF_HIT_VOL				4					// ����������ʾ����С
#define LYLY_VIEW_VOL			4					// ��Ӱ���Բ�����
#define MSG_VIEW_VOL			4					// ��Ϣ�鿴������С
#define JRLY_VOLUME				4					// �������Բ���������С
#define RING_OUT_VOL			4					// ��������С

// ====�����豸�����Ŀ======================================
#define DOOR_DEVICE_NUM			2					// ��ǰ�������
#define STAIR_DEVICE_NUM		20					// �ݿڷֻ������
#define AREA_DEVICE_NUM			99					// ���ڻ������ 
#define ROOM_DEVICE_NUM			8					// ���ڷֻ������ 
#define MANAGE_DEVICE_NUM		3					// ������������� 
#define IPC_DEVICE_NUM			99					// ����IPC�豸����� 
#define MAX_HOME_NUM  			16					// �Ҿ�IPC�豸�����

// ====�����¼�洢����======================================
#define AF_REC_MAX 	  			20					// �����¼�����¼��
#define MAX_RECORD_NUM  		20					// ͨ����¼�������
#define MAX_JRLY_NUM  			5					// ������������¼��
#define MAX_LYLY_NUM  			6					// ��Ӱ��������¼
#define MAX_MSG_NUM				10					// ��Ϣ����������
#define MAX_PHOTO_NUM  			16					// ���ץ�ļ�¼��
#define MAX_YUYUE_NUM			20					// �ҵ�ԤԼ�����

// ====����ִ��ʱ�����ֵ====================================
#define MONITOR_TIME_MAX		90					// �����ʱ��
#define	MONITOR_TALK_TIME_MAX	90					// ͨ���ʱ��
#define	HEART_TIMEOUT			5					// ������ʱ
#define	CALLOUT_TIMEOUT			60					// ���г�ʱ
#define LEAVEWORD_TIMEOUT		60					// ��ʱ�������Ե�ʱ��
#define	RECORDHINT_TIME			30					// ������ʾ��ʱ��
#define LEAVEWORD_TIME			30					// �����ʱ��
#define PHONE_TALK_TIMEOUT		60					// ģ���ſڻ�ͨ��ʱ��
#define FENJI_TALK_TIMEOUT		300					// ����ͨͨ��ʱ��

#define RECORD_TIME_MAX			30					// �������ʱ��(�������ԡ���Ӱ���ԡ�����¼����)


// ====���汾���ܶ���============================================
//-----------��������------------------
#define _JD_MODE_									// �ҵ�ģ��
#define	_YUYUE_MODE_								// ����ԤԼ����
#define _SHOW_USED_JDDEV_ONLY_						// �ҵ���ƺ��龰���ý���ֻ��ʾ���õ��豸
#define _USE_ELEVATOR_								// ���õ��ݿ�����
#define _TY_STAIR_									// ֧��ͨ���ݿڻ�
#define _AURINE_ELEC_NEW_                           // add by fanfj�����¹��ּҵ�
#define _DIANTI_CALL_								// �������ݹ���
#define _IP_MODULE_									// IPģ��
#define _LIVEMEDIA_									// RTSPģ��
#define _USE_NEW_CENTER_							// ʹ���°汾���ķ�����
#define _SEND_SDP_PARAM_							// �Ƿ���ý�����
#define _RTSP_REG_KEEP_								// RTSP��������(Ŀ�Ļ�ȡ��������ͷ�����б�)
//#define _TIMER_REBOOT_NO_LCD_           			// ��ʱ����ʱ��������
//#define _UPLOAD_PHOTO_							// ������ǰ�������ϴ�����
#ifdef _IP_MODULE_
#define _IP_MODULE_DJ_								// IPģ��Խ�
#define _IP_MODULE_ALARM							// ��������
#define _IP_MODULE_JD_								// IPģ��ҵ�
//#define _IP_MODULE_RTSP_							// RTSPע��
#endif
//#define _USE_FOR_SHOW_								// չ����ʱ�汾

// Ӳ��������Ҳ���ǳ��̴���
#define _ML_HWENCRYPT_			0X00004753			// ����ʹ�õ�Ӳ��������
#define _TF_HWENCRYPT_			0X54460000			// �츻ʹ�õ�Ӳ��������
#define _ZH_HWENCRYPT_			0X00005A48			// �к�ʹ�õ�Ӳ��������
#define _SD_HWENCRYPT_			0X54465344			// ����ʹ�õ�Ӳ��������
#define _HY_HWENCRYPT_			0X54464859			// �츻��ӥ��Ӳ��������


// Э��汾��
#define _ML_PROTOCOL_VER_		0X0101				// ����Э��汾
#define _TF_PROTOCOL_VER_		0XA0AF				// �츻Э��汾
#define _ZH_PROTOCOL_VER_   	0XA101				// �к�Э��汾
#define _SD_PROTOCOL_VER_		0XA001				// ����Э��汾
#define _HY_PROTOCOL_VER_		0XA0A1				// ��ӥЭ��汾


// Э���������,��ͬ�ļ��������в�ͬ��KEY, 0-������
#define _NO_PROTOCOL_ENCRYPT_   0x00				// Э�鲻����
#define _TF_PROTOCOL_ENCRYPT_ 	0xAF				// �츻Э���������
#define _SD_PROTOCOL_ENCRYPT_   0xA0				// ����Э���������
#define _HY_PROTOCOL_ENCRYPT_   0xA1				// ��ӥЭ���������

// ====����ṹ��============================================
#define ML8_V1					1					
#define ML8_V6					2
#define TF8_A1					3
#define AH8_E81					4
#define ML8_V7					5
#define ML8_V5					6

// ====�汾����================================================
#define E81_AH_VER				1					// ����E81�汾
#define V5M_ML_VER				2					// ����V5M�汾
#define V5M_TF_VER				3					// �츻V5M�汾
#define V7M_ML_VER				4					// ����V7M�汾
#define V7M_TF_VER				5					// �츻V7M�汾

#define VER_TYPE				E81_AH_VER



//-----------���汾���칦��-------------
#if (VER_TYPE == E81_AH_VER)
#define _AU_PROTOCOL_									// ʹ�ù���Э��
#define _USE_X10_JD_									// ʹ��X10�ҵ�
#undef _USE_NEW_CENTER_									// ��ʹ���µ����Ļ�

#define HARD_VER_COMM		    "AH8-E81M"
#define JIEGOU_TYPE				AH8_E81
#define _ALARM_IO_										// ����ֱ����IO��
#define _CP2526_TOUCH_KEY_								// ���ø�Ӧ����
#define _TIMER_REBOOT_NO_LCD_           				// ��ʱ����ʱ��������

#elif (VER_TYPE == V7M_ML_VER)
#define _HW_ENCRYPT_			_ML_HWENCRYPT_			// �ð汾ʹ�õ�Ӳ��������
#define _PROTOCOL_VER_			_ML_PROTOCOL_VER_		// �ð汾ʹ�õ�Э��汾
#define _SUB_PROTOCOL_ENCRYPT_  _NO_PROTOCOL_ENCRYPT_	// Э���������,�����汾������

#define HARD_VER_COMM		    "SNVX98-100101"
#define JIEGOU_TYPE				ML8_V7
#define _ALARM_IO_										// ����ֱ����IO��
#define _CP2526_TOUCH_KEY_								// ���ø�Ӧ����
#define _NEW_SELF_IPC_									// ʹ���µı���˾IPC�ҾӼ���
//#define _TIMER_REBOOT_NO_LCD_           				// ��ʱ����ʱ��������

#endif


// ====����汾��============================================
//-----------Ӳ���汾------------------
#ifndef HARD_VER_COMM
#define HARD_VER_COMM		    "ITE9856-"
#endif

//----------����汾---------------------
#define SOFT_VER_COMM			"V2.03.0001_"


#define BUILD_TIME				__TIME__ "_" __DATE__

#ifndef HARDSUBVER
#define HARDSUBVER				"-REL"					// ����Ƕ��Ƶ� Ϊ��Ŀÿ������ĸ��д
#endif

#ifndef SOFTSUBVER
#define SOFTSUBVER				BUILD_TIME
#endif

#define HARD_VER				(HARD_VER_COMM HARDSUBVER)
#define SOFT_VER				(SOFT_VER_COMM SOFTSUBVER)
#define SDK_VER					BUILD_TIME
#endif

