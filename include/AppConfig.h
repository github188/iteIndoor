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
#define LYLY_VIEW_VOL			8					// ��Ӱ���Բ�����
#define MSG_VIEW_VOL			4					// ��Ϣ�鿴������С
#define JRLY_VOLUME				8					// �������Բ���������С
#define RING_OUT_VOL			4					// ��������С

// ====�����¼�洢����======================================
#define AF_REC_MAX 	  			20					// �����¼�����¼��
#define MAX_RECORD_NUM  		20					// ͨ����¼�������
#define MAX_JRLY_NUM  			5					// ������������¼��
#define MAX_LYLY_NUM  			5					// ��Ӱ��������¼
#define MAX_MSG_NUM				10					// ��Ϣ����������
#define MAX_PHOTO_NUM  			10					// ���ץ�ļ�¼��
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

// ====����ṹ��============================================
#define ML8_V1					1
#define ML8_V6					2
#define TF8_A1					3
#define AH8_E81					4
#define ML8_V7					5
#define ML8_V5					6

// ====����Ӳ������=========================================
#define ML8_N32926_V1S			1					// ML8-V1S���ڻ�
#define ML8_N32926_V6S			2					// ML8-V6S���ڻ�
#define TF8_N32926_A1S			3					// TF8-A1S���ڻ�
#define AH8_N32926_E81M			4					// AH8-E81/E91/F91M���ڻ�
#define ML8_N32926_V7M			5					// ML8-V7M���ڻ�

// ====�汾����================================================
#define V1_JHB_VER				1					// ML8-V1S�򻯰�,���û�е������4.3��Һ����
#define V6_JHB_VER				2					// ML8-V6S�򻯰�,���û�е������7��Һ����
#define A1_JHB_VER				3					// TF8-A1S�򻯰�,���û�е������4.3��Һ����
#define E81_BZB_VER				4					// AH8-E81/E91/F91M��׼�����ڻ�,���ø�Ӧ����,��������7��Һ����
#define V7_BZB_VER				5					// ML8-V7M��׼�����ڻ�,���ø�Ӧ����,��������7��Һ����
#define V5_BZB_VER				6					// ML8-V5M��׼�����ڻ�,���ø�Ӧ����,��������7��Һ����
#define V6_BZB_VER				7					// ML8-V6M��׼�����ڻ�,���ø�Ӧ����,��������7��Һ����
#define V8_BZB_VER				8					// ML8-V8M��׼�����ڻ�,���ø�Ӧ����,��������7��Һ����

#define SYS_TYPE				V7_BZB_VER

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

//-----------���汾���칦��-------------
#if (SYS_TYPE == E81_BZB_VER)
#if 1
#define _AU_PROTOCOL_									// ʹ�ù���Э��
#define _USE_X10_JD_									// ʹ��X10�ҵ�
#undef _USE_NEW_CENTER_									// ��ʹ���µ����Ļ�
#else
#define _HW_ENCRYPT_			_ML_HWENCRYPT_			// �ð汾ʹ�õ�Ӳ��������
#define _PROTOCOL_VER_			_ML_PROTOCOL_VER_		// �ð汾ʹ�õ�Э��汾
#define _SUB_PROTOCOL_ENCRYPT_  _NO_PROTOCOL_ENCRYPT_	// Э���������,�����汾������
#endif

#define	SOFTSUBVER				"_AH8-E81M"
#define HARDSUBVER				"E81M-V2.0"
#define BOARD_TYPE				AH8_N32926_E81M
#define JIEGOU_TYPE				ML8_V1
#define _TFT_7_											// 7����
#define _ALARM_IO_										// ����ֱ����IO��
#define _CP2526_TOUCH_KEY_								// ���ø�Ӧ����
#define _CP_7INCH_TOUCH_								// ����7�������
#define _TIMER_REBOOT_NO_LCD_           				// ��ʱ����ʱ��������

#elif (SYS_TYPE == V7_BZB_VER)
#if 1
#define _HW_ENCRYPT_			_ML_HWENCRYPT_			// �ð汾ʹ�õ�Ӳ��������
#define _PROTOCOL_VER_			_ML_PROTOCOL_VER_		// �ð汾ʹ�õ�Э��汾
#define _SUB_PROTOCOL_ENCRYPT_  _NO_PROTOCOL_ENCRYPT_	// Э���������,�����汾������
#else
#define _HW_ENCRYPT_			_TF_HWENCRYPT_			// �ð汾ʹ�õ�Ӳ��������
#define _PROTOCOL_VER_			_TF_PROTOCOL_VER_		// �ð汾ʹ�õ�Э��汾
#define _SUB_PROTOCOL_ENCRYPT_  _TF_PROTOCOL_ENCRYPT_	// Э���������
#endif
#ifdef _USE_NEW_CENTER_
#define HARD_VER_COMM		    "SNV792-100101"
#else
#define	SOFTSUBVER				"_ML8-V7M"
#define HARDSUBVER				"ML8-V2.0"
#endif
#define BOARD_TYPE				ML8_N32926_V7M
#define JIEGOU_TYPE				ML8_V7
#define _TFT_7_											// 7����
#define _ALARM_IO_										// ����ֱ����IO��
#define _CP2526_TOUCH_KEY_								// ���ø�Ӧ����
#define _CP_7INCH_TOUCH_								// ����7�������
#define _NO_LOGO_										// ����ʾlogo
#define _NEW_SELF_IPC_									// ʹ���µı���˾IPC�ҾӼ���

#endif


// ====����汾��============================================
//-----------Ӳ���汾------------------
#ifndef HARD_VER_COMM
#define HARD_VER_COMM		    "ITE-"
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

