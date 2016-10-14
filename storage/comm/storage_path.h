/*************************************************
  Copyright (C), 2006-2016, Aurine
  File name:	storage_path.h
  Author:		luofl
  Version:		2.0
  Date: 		2014-10-02
  Description:	�洢·��

  History:		  
	1. Date:
	   Author:
	   Modification:
	2. ...
*************************************************/
#ifndef __STORAGE_PATH_H__
#define __STORAGE_PATH_H__

/*
	��Դ����A��   	CFG_PRIVATE_DRIVE
	ϵͳ�ļ���B�� 	CFG_PUBLIC_DRIVE
	��¼�洢�ļ��� C�� CFG_TEMP_DRIVE
*/

/**************************************************************************/
/*				ϵͳ���ô洢(����ϵͳ�����Ͱ�������)					  */	
/**************************************************************************/
#define SYSCONFIG_DIR_PATH		CFG_PUBLIC_DRIVE":/SYSCONFIG"   						// ϵͳ����·��
#define SYSCONFIG_PARAM_PATH	CFG_PUBLIC_DRIVE":/SYSCONFIG/PARAM.BIN"   				// ϵͳ���ò���

/**************************************************************************/
/*									�Խ�								  */	
/**************************************************************************/
#define CALL_DIR_PATH			CFG_TEMP_DRIVE":/RECORD"   							// �Խ���·��
#define CALL_IN_PATH			CFG_TEMP_DRIVE":/RECORD/IN.BIN"   						// �Խ������¼
#define CALL_OUT_PATH			CFG_TEMP_DRIVE":/RECORD/OUT.BIN"  						// �Խ�������¼
#define CALL_MISS_PATH			CFG_TEMP_DRIVE":/RECORD/MISS.BIN" 						// �Խ�δ�Ӽ�¼

// ����·��
#define CALL_RING1_PATH			CFG_PRIVATE_DRIVE":/res/rings/Ring/RING1.WAV"			// ����1
#define CALL_RING2_PATH			CFG_PRIVATE_DRIVE":/res/rings/Ring/RING2.WAV"			// ����2
#define CALL_RING3_PATH			CFG_PRIVATE_DRIVE":/res/rings/Ring/RING3.WAV"			// ����3
#define CALL_RING4_PATH			CFG_PRIVATE_DRIVE":/res/rings/Ring/RING4.WAV"			// ����4
#define CALL_RING5_PATH			CFG_PRIVATE_DRIVE":/res/rings/Ring/RING5.WAV"			// ����5
#define CALL_RING6_PATH			CFG_PRIVATE_DRIVE":/res/rings/Ring/RING6.WAV"			// ����6
#define CALL_RING7_PATH			CFG_PRIVATE_DRIVE":/res/rings/Ring/RING7.WAV"			// ����7
#define CALL_RING8_PATH			CFG_PRIVATE_DRIVE":/res/rings/Ring/RING8.WAV"			// ����8
#define CALL_RINGOUT_PATH		CFG_PRIVATE_DRIVE":/res/rings/Ring/RINGOUT.WAV"			// ������

/**************************************************************************/
/*									����								  */	
/**************************************************************************/
#define AF_DIR_PATH				CFG_TEMP_DRIVE":/ALARM"								// ����·��
#define AF_CZ_REC_PATH			CFG_TEMP_DRIVE":/ALARM/CZREC.BIN"						// ����������¼
#define AF_BJ_REC_PATH			CFG_TEMP_DRIVE":/ALARM/BJBRE.BIN"						// ����������¼
//#define AF_DIR_PATH				"D:/ALARM"								// ����·��
//#define AF_CZ_REC_PATH			"D:/ALARM/CZREC.BIN"						// ����������¼
//#define AF_BJ_REC_PATH			"D:/ALARM/BJBRE.BIN"						// ����������¼

// ������ʾ��·��
#define AF_YJ_PATH				CFG_PRIVATE_DRIVE":/res/rings/Alarm/YUJING.WAV"			// Ԥ������·��
#define AF_BJ_PATH				CFG_PRIVATE_DRIVE":/res/rings/Alarm/BAOJING.WAV"			// ��������·��
#define AF_YJ_WC_PATH			CFG_PRIVATE_DRIVE":/res/rings/Alarm/YJ_WC.WAV"			// Ԥ���ġ������غ��š�������
#define AF_YJQ_PATH				CFG_PRIVATE_DRIVE":/res/rings/Alarm/YJQ.WAV"			// ����ʱ�о���������ʾ����ע���о��顱
#define AF_FQCF_PATH			CFG_PRIVATE_DRIVE":/res/rings/Alarm/FQCF.WAV"			// "�����������ܲ���"��������ʾ
#define AF_JJYJC_PATH			CFG_PRIVATE_DRIVE":/res/rings/Alarm/JJYJC.WAV"			// ���г�����������ʾ����ã������ѽ��
#define AF_WANAN_PATH			CFG_PRIVATE_DRIVE":/res/rings/Alarm/WANAN.WAV"			// ���оַ�ʱ������ʾ����
#define AF_ZONE1_PATH			CFG_PRIVATE_DRIVE":/res/rings/Alarm/ZONE1.WAV"			// ��ע���о���, 1��������
#define AF_ZONE2_PATH			CFG_PRIVATE_DRIVE":/res/rings/Alarm/ZONE2.WAV"	
#define AF_ZONE3_PATH			CFG_PRIVATE_DRIVE":/res/rings/Alarm/ZONE3.WAV"	
#define AF_ZONE4_PATH			CFG_PRIVATE_DRIVE":/res/rings/Alarm/ZONE4.WAV"	
#define AF_ZONE5_PATH			CFG_PRIVATE_DRIVE":/res/rings/Alarm/ZONE5.WAV"	
#define AF_ZONE6_PATH			CFG_PRIVATE_DRIVE":/res/rings/Alarm/ZONE6.WAV"	
#define AF_ZONE7_PATH			CFG_PRIVATE_DRIVE":/res/rings/Alarm/ZONE7.WAV"	
#define AF_ZONE8_PATH			CFG_PRIVATE_DRIVE":/res/rings/Alarm/ZONE8.WAV"	
// Ӣ����ʾ��·��
#define AF_YJ_WC_PATH_E			CFG_PRIVATE_DRIVE":/res/rings/Alarm/YJ_WC_E.WAV"			// Ԥ���ġ������غ��š�������
#define AF_YJQ_PATH_E			CFG_PRIVATE_DRIVE":/res/rings/Alarm/YJQ_E.WAV"			// ����ʱ�о���������ʾ����ע���о��顱
#define AF_FQCF_PATH_E			CFG_PRIVATE_DRIVE":/res/rings/Alarm/FQCF_E.WAV"			// "�����������ܲ���"��������ʾ
#define AF_JJYJC_PATH_E			CFG_PRIVATE_DRIVE":/res/rings/Alarm/JJYJC_E.WAV"			// ���г�����������ʾ����ã������ѽ��
#define AF_WANAN_PATH_E			CFG_PRIVATE_DRIVE":/res/rings/Alarm/WANAN_E.WAV"			// ���оַ�ʱ������ʾ����
#define AF_ZONE1_PATH_E			CFG_PRIVATE_DRIVE":/res/rings/Alarm/ZONE1_E.WAV"			// ��ע���о���, 1��������
#define AF_ZONE2_PATH_E			CFG_PRIVATE_DRIVE":/res/rings/Alarm/ZONE2_E.WAV"	
#define AF_ZONE3_PATH_E			CFG_PRIVATE_DRIVE":/res/rings/Alarm/ZONE3_E.WAV"	
#define AF_ZONE4_PATH_E			CFG_PRIVATE_DRIVE":/res/rings/Alarm/ZONE4_E.WAV"	
#define AF_ZONE5_PATH_E			CFG_PRIVATE_DRIVE":/res/rings/Alarm/ZONE5_E.WAV"	
#define AF_ZONE6_PATH_E			CFG_PRIVATE_DRIVE":/res/rings/Alarm/ZONE6_E.WAV"	
#define AF_ZONE7_PATH_E			CFG_PRIVATE_DRIVE":/res/rings/Alarm/ZONE7_E.WAV"	
#define AF_ZONE8_PATH_E			CFG_PRIVATE_DRIVE":/res/rings/Alarm/ZONE8_E.WAV"	

// Ԥ����ʾ��
#define AF_YJ1_PATH				CFG_PRIVATE_DRIVE":/res/rings/Alarm/YUJING1.WAV"			// Ԥ����ʾ��1
#define AF_YJ2_PATH				CFG_PRIVATE_DRIVE":/res/rings/Alarm/YUJING2.WAV"			// Ԥ����ʾ��2
#define AF_YJ3_PATH				CFG_PRIVATE_DRIVE":/res/rings/Alarm/YUJING3.WAV"			// Ԥ����ʾ��3
#define AF_YJ4_PATH				CFG_PRIVATE_DRIVE":/res/rings/Alarm/YUJING4.WAV"			// Ԥ����ʾ��4

/**************************************************************************/
/*									��Ϣ								  */	
/**************************************************************************/
#define MSG_DIR_PATH			CFG_TEMP_DRIVE":/INFO"								// ��Ϣ�ļ���		
#define MSG_MANAGE_PATH			CFG_TEMP_DRIVE":/INFO/MANAGE.BIN"						// ��Ϣ�����ļ�
#define MSG_VOICE_PATH			CFG_TEMP_DRIVE":/INFO/VOICE"							// ��Ϣ�����ļ�

// ��Ϣ��ʾ��
#define MSG_HIT1_PATH			CFG_PRIVATE_DRIVE":/res/rings/Hit/MSG1.WAV"				// ��Ϣ��ʾ��1
#define MSG_HIT2_PATH			CFG_PRIVATE_DRIVE":/res/rings/Hit/MSG2.WAV"				// ��Ϣ��ʾ��2
#define MSG_HIT3_PATH			CFG_PRIVATE_DRIVE":/res/rings/Hit/MSG3.WAV"				// ��Ϣ��ʾ��3
#define MSG_HIT4_PATH			CFG_PRIVATE_DRIVE":/res/rings/Hit/MSG4.WAV"				// ��Ϣ��ʾ��4

/**************************************************************************/
/*									ץ��								  */	
/**************************************************************************/
#define SNAP_DIR_PATH			CFG_TEMP_DRIVE":/SNAP"								// ץ���ļ���		
#define SNAP_MANAGE_PATH		CFG_TEMP_DRIVE":/SNAP/MANAGE.BIN"						// ץ�Ĺ����ļ�
//#define SNAP_DIR_PATH			"D:/SNAP"								// ץ���ļ���		
//#define SNAP_MANAGE_PATH		"D:/SNAP/MANAGE.BIN"						// ץ�Ĺ����ļ�

/**************************************************************************/
/*								  ��Ӱ����								  */	
/**************************************************************************/
#define LYLY_DIR_PATH			CFG_TEMP_DRIVE":/LYLY"								// ��Ӱ�����ļ���		
#define LYLY_MANAGE_PATH		CFG_TEMP_DRIVE":/LYLY/MANAGE.BIN"						// ��Ӱ���Թ����ļ�
//#define LYLY_DIR_PATH			"D:/LYLY"								// ��Ӱ�����ļ���		
//#define LYLY_MANAGE_PATH		"D:/LYLY/MANAGE.BIN"						// ��Ӱ���Թ����ļ�

// ������ʾ��
#define LYLY_HIT_CN_PATH		CFG_PRIVATE_DRIVE":/res/rings/Hit/LYLYCN.WAV"			// Ĭ������������ʾ��
#define LYLY_HIT_EN_PATH		CFG_PRIVATE_DRIVE":/res/rings/Hit/LYLYEN.WAV"			// Ĭ��Ӣ��������ʾ��
#define LYLY_HIT_RE_PATH		CFG_PUBLIC_DRIVE":/LYLY"								// ¼��������ʾ��
#define LYLY_HIT_RE_FILE_PATH	CFG_PUBLIC_DRIVE":/LYLY/LYLY.WAV"						// ¼��������ʾ��

/**************************************************************************/
/*								  ��������								  */	
/**************************************************************************/
#define JRLY_DIR_PATH			CFG_TEMP_DRIVE":/JRLY"								// ���������ļ���
#define JRLY_MANAGE_PATH		CFG_TEMP_DRIVE":/JRLY/MANAGE.BIN"						// �������Թ����ļ�

/**************************************************************************/
/*								  �ҵ�									  */	
/**************************************************************************/
#define JD_DIR_PATH				CFG_TEMP_DRIVE":/JD"								
#define JD_LIGHT_VALUE_PATH     CFG_TEMP_DRIVE":/JD/ALIGHT.BIN"
#define JD_KONGTIAO_VALUE_PATH  CFG_TEMP_DRIVE":/JD/AKONGTIAO.BIN"
#define JD_DEVICE_LIGHT_PATH	CFG_TEMP_DRIVE":/JD/LIGHT.BIN"							// �ҵ�ƹ�����ļ�
#define JD_DEVICE_WINDOW_PATH	CFG_TEMP_DRIVE":/JD/WINDOW.BIN"						// �ҵ細�������ļ�
#define JD_DEVICE_KONGTIAO_PATH	CFG_TEMP_DRIVE":/JD/KONGTIAO.BIN"						// �ҵ�յ������ļ�
#define JD_DEVICE_POWER_PATH	CFG_TEMP_DRIVE":/JD/POWER.BIN"							// �ҵ��Դ�����ļ�
#define JD_DEVICE_GAS_PATH		CFG_TEMP_DRIVE":/JD/GAS.BIN"							// �ҵ�ú�������ļ�
#define JD_SCENE_PATH			CFG_TEMP_DRIVE":/JD/SCENE"							// �ҵ��龰�����ļ�
#define JD_AURINE_SCENE_PATH    CFG_TEMP_DRIVE":/JD/ASCENE.BIN"

/**************************************************************************/
/*								 ԤԼ									  */	
/**************************************************************************/
#define YUYUE_DIR_PATH			CFG_TEMP_DRIVE":/YUYUE"								// ԤԼ�ļ�
#define YUYUE_MANAGE_PATH		CFG_TEMP_DRIVE":/YUYUE/YUYUE.BIN"						// ԤԼ�б�
//#define EVENT_TIP_PATHID		CFG_TEMP_DRIVE":/APPDATA/TIP/"						// ��ʾ������·��

/**************************************************************************/
/*								 ��Ƶ����								  */	
/**************************************************************************/
#define MONITOR_DIR_PATH		CFG_TEMP_DRIVE":/MONITOR"				
#define HOME_CAMERA_PATH		CFG_TEMP_DRIVE":/MONITOR/HOME_CAMERA.BIN"	
#define COMMUNITY_CAMERA_PATH	CFG_TEMP_DRIVE":/MONITOR/COMMUNITY_CAMERA.BIN"

#define MONITOR_DOOR_PATH		CFG_TEMP_DRIVE":/MONITOR/MONITOR_DOOR.BIN"	
#define MONITOR_STAIR_PATH		CFG_TEMP_DRIVE":/MONITOR/MONITOR_STAIR.BIN"	
#define MONITOR_AREA_PATH		CFG_TEMP_DRIVE":/MONITOR/MONITOR_AREA.BIN"	

/**************************************************************************/
/*								 ��Ļ����								  */	
/**************************************************************************/
#define SCREEN_SAVER_DIR_PATH	CFG_PRIVATE_DRIVE":/res/ScreenSaver/"					// ��Ļ������Դ		

/**************************************************************************/
/*								 ���汳��								  */	
/**************************************************************************/
#define WALL_PAPER_DIR_PATH		CFG_PRIVATE_DRIVE":/res/wallpaper/"					// ���汳��ͼƬ��Դ		

/**************************************************************************/
/*								 ��Ӱ����								  */
/**************************************************************************/
#define PHOTO_MSG_DIR_PATH		CFG_PRIVATE_DRIVE":/res/photomsg/"					// ��Ӱ����ͼƬ��Դ		




#ifdef _USE_NEW_CENTER_
/**************************************************************************/
/*								����ע��ɹ��豸���					  */	
/**************************************************************************/
#define PRE_DEVNO_FILENAME		CFG_TEMP_DRIVE":/pre_devno.dat"
#endif

#endif 

