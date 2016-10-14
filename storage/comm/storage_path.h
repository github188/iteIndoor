/*************************************************
  Copyright (C), 2006-2016, Aurine
  File name:	storage_path.h
  Author:		luofl
  Version:		2.0
  Date: 		2014-10-02
  Description:	存储路径

  History:		  
	1. Date:
	   Author:
	   Modification:
	2. ...
*************************************************/
#ifndef __STORAGE_PATH_H__
#define __STORAGE_PATH_H__

/*
	资源放在A盘   	CFG_PRIVATE_DRIVE
	系统文件放B盘 	CFG_PUBLIC_DRIVE
	记录存储文件放 C盘 CFG_TEMP_DRIVE
*/

/**************************************************************************/
/*				系统配置存储(包含系统参数和安防参数)					  */	
/**************************************************************************/
#define SYSCONFIG_DIR_PATH		CFG_PUBLIC_DRIVE":/SYSCONFIG"   						// 系统配置路径
#define SYSCONFIG_PARAM_PATH	CFG_PUBLIC_DRIVE":/SYSCONFIG/PARAM.BIN"   				// 系统配置参数

/**************************************************************************/
/*									对讲								  */	
/**************************************************************************/
#define CALL_DIR_PATH			CFG_TEMP_DRIVE":/RECORD"   							// 对讲的路径
#define CALL_IN_PATH			CFG_TEMP_DRIVE":/RECORD/IN.BIN"   						// 对讲呼入记录
#define CALL_OUT_PATH			CFG_TEMP_DRIVE":/RECORD/OUT.BIN"  						// 对讲呼出记录
#define CALL_MISS_PATH			CFG_TEMP_DRIVE":/RECORD/MISS.BIN" 						// 对讲未接记录

// 铃声路径
#define CALL_RING1_PATH			CFG_PRIVATE_DRIVE":/res/rings/Ring/RING1.WAV"			// 铃声1
#define CALL_RING2_PATH			CFG_PRIVATE_DRIVE":/res/rings/Ring/RING2.WAV"			// 铃声2
#define CALL_RING3_PATH			CFG_PRIVATE_DRIVE":/res/rings/Ring/RING3.WAV"			// 铃声3
#define CALL_RING4_PATH			CFG_PRIVATE_DRIVE":/res/rings/Ring/RING4.WAV"			// 铃声4
#define CALL_RING5_PATH			CFG_PRIVATE_DRIVE":/res/rings/Ring/RING5.WAV"			// 铃声5
#define CALL_RING6_PATH			CFG_PRIVATE_DRIVE":/res/rings/Ring/RING6.WAV"			// 铃声6
#define CALL_RING7_PATH			CFG_PRIVATE_DRIVE":/res/rings/Ring/RING7.WAV"			// 铃声7
#define CALL_RING8_PATH			CFG_PRIVATE_DRIVE":/res/rings/Ring/RING8.WAV"			// 铃声8
#define CALL_RINGOUT_PATH		CFG_PRIVATE_DRIVE":/res/rings/Ring/RINGOUT.WAV"			// 回铃声

/**************************************************************************/
/*									报警								  */	
/**************************************************************************/
#define AF_DIR_PATH				CFG_TEMP_DRIVE":/ALARM"								// 安防路径
#define AF_CZ_REC_PATH			CFG_TEMP_DRIVE":/ALARM/CZREC.BIN"						// 安防操作记录
#define AF_BJ_REC_PATH			CFG_TEMP_DRIVE":/ALARM/BJBRE.BIN"						// 安防报警记录
//#define AF_DIR_PATH				"D:/ALARM"								// 安防路径
//#define AF_CZ_REC_PATH			"D:/ALARM/CZREC.BIN"						// 安防操作记录
//#define AF_BJ_REC_PATH			"D:/ALARM/BJBRE.BIN"						// 安防报警记录

// 中文提示音路径
#define AF_YJ_PATH				CFG_PRIVATE_DRIVE":/res/rings/Alarm/YUJING.WAV"			// 预警声音路径
#define AF_BJ_PATH				CFG_PRIVATE_DRIVE":/res/rings/Alarm/BAOJING.WAV"			// 报警声音路径
#define AF_YJ_WC_PATH			CFG_PRIVATE_DRIVE":/res/rings/Alarm/YJ_WC.WAV"			// 预警的“外出请关好门”的声音
#define AF_YJQ_PATH				CFG_PRIVATE_DRIVE":/res/rings/Alarm/YJQ.WAV"			// 撤防时有警情语音提示“请注意有警情”
#define AF_FQCF_PATH			CFG_PRIVATE_DRIVE":/res/rings/Alarm/FQCF.WAV"			// "防区触发不能布防"的语音提示
#define AF_JJYJC_PATH			CFG_PRIVATE_DRIVE":/res/rings/Alarm/JJYJC.WAV"			// 进行撤防的语音提示：你好！警戒已解除
#define AF_WANAN_PATH			CFG_PRIVATE_DRIVE":/res/rings/Alarm/WANAN.WAV"			// 进行局防时语音提示：晚安
#define AF_ZONE1_PATH			CFG_PRIVATE_DRIVE":/res/rings/Alarm/ZONE1.WAV"			// 请注意有警情, 1防区触发
#define AF_ZONE2_PATH			CFG_PRIVATE_DRIVE":/res/rings/Alarm/ZONE2.WAV"	
#define AF_ZONE3_PATH			CFG_PRIVATE_DRIVE":/res/rings/Alarm/ZONE3.WAV"	
#define AF_ZONE4_PATH			CFG_PRIVATE_DRIVE":/res/rings/Alarm/ZONE4.WAV"	
#define AF_ZONE5_PATH			CFG_PRIVATE_DRIVE":/res/rings/Alarm/ZONE5.WAV"	
#define AF_ZONE6_PATH			CFG_PRIVATE_DRIVE":/res/rings/Alarm/ZONE6.WAV"	
#define AF_ZONE7_PATH			CFG_PRIVATE_DRIVE":/res/rings/Alarm/ZONE7.WAV"	
#define AF_ZONE8_PATH			CFG_PRIVATE_DRIVE":/res/rings/Alarm/ZONE8.WAV"	
// 英文提示音路径
#define AF_YJ_WC_PATH_E			CFG_PRIVATE_DRIVE":/res/rings/Alarm/YJ_WC_E.WAV"			// 预警的“外出请关好门”的声音
#define AF_YJQ_PATH_E			CFG_PRIVATE_DRIVE":/res/rings/Alarm/YJQ_E.WAV"			// 撤防时有警情语音提示“请注意有警情”
#define AF_FQCF_PATH_E			CFG_PRIVATE_DRIVE":/res/rings/Alarm/FQCF_E.WAV"			// "防区触发不能布防"的语音提示
#define AF_JJYJC_PATH_E			CFG_PRIVATE_DRIVE":/res/rings/Alarm/JJYJC_E.WAV"			// 进行撤防的语音提示：你好！警戒已解除
#define AF_WANAN_PATH_E			CFG_PRIVATE_DRIVE":/res/rings/Alarm/WANAN_E.WAV"			// 进行局防时语音提示：晚安
#define AF_ZONE1_PATH_E			CFG_PRIVATE_DRIVE":/res/rings/Alarm/ZONE1_E.WAV"			// 请注意有警情, 1防区触发
#define AF_ZONE2_PATH_E			CFG_PRIVATE_DRIVE":/res/rings/Alarm/ZONE2_E.WAV"	
#define AF_ZONE3_PATH_E			CFG_PRIVATE_DRIVE":/res/rings/Alarm/ZONE3_E.WAV"	
#define AF_ZONE4_PATH_E			CFG_PRIVATE_DRIVE":/res/rings/Alarm/ZONE4_E.WAV"	
#define AF_ZONE5_PATH_E			CFG_PRIVATE_DRIVE":/res/rings/Alarm/ZONE5_E.WAV"	
#define AF_ZONE6_PATH_E			CFG_PRIVATE_DRIVE":/res/rings/Alarm/ZONE6_E.WAV"	
#define AF_ZONE7_PATH_E			CFG_PRIVATE_DRIVE":/res/rings/Alarm/ZONE7_E.WAV"	
#define AF_ZONE8_PATH_E			CFG_PRIVATE_DRIVE":/res/rings/Alarm/ZONE8_E.WAV"	

// 预警提示音
#define AF_YJ1_PATH				CFG_PRIVATE_DRIVE":/res/rings/Alarm/YUJING1.WAV"			// 预警提示音1
#define AF_YJ2_PATH				CFG_PRIVATE_DRIVE":/res/rings/Alarm/YUJING2.WAV"			// 预警提示音2
#define AF_YJ3_PATH				CFG_PRIVATE_DRIVE":/res/rings/Alarm/YUJING3.WAV"			// 预警提示音3
#define AF_YJ4_PATH				CFG_PRIVATE_DRIVE":/res/rings/Alarm/YUJING4.WAV"			// 预警提示音4

/**************************************************************************/
/*									信息								  */	
/**************************************************************************/
#define MSG_DIR_PATH			CFG_TEMP_DRIVE":/INFO"								// 信息文件夹		
#define MSG_MANAGE_PATH			CFG_TEMP_DRIVE":/INFO/MANAGE.BIN"						// 信息管理文件
#define MSG_VOICE_PATH			CFG_TEMP_DRIVE":/INFO/VOICE"							// 信息声音文件

// 信息提示音
#define MSG_HIT1_PATH			CFG_PRIVATE_DRIVE":/res/rings/Hit/MSG1.WAV"				// 信息提示音1
#define MSG_HIT2_PATH			CFG_PRIVATE_DRIVE":/res/rings/Hit/MSG2.WAV"				// 信息提示音2
#define MSG_HIT3_PATH			CFG_PRIVATE_DRIVE":/res/rings/Hit/MSG3.WAV"				// 信息提示音3
#define MSG_HIT4_PATH			CFG_PRIVATE_DRIVE":/res/rings/Hit/MSG4.WAV"				// 信息提示音4

/**************************************************************************/
/*									抓拍								  */	
/**************************************************************************/
#define SNAP_DIR_PATH			CFG_TEMP_DRIVE":/SNAP"								// 抓拍文件夹		
#define SNAP_MANAGE_PATH		CFG_TEMP_DRIVE":/SNAP/MANAGE.BIN"						// 抓拍管理文件
//#define SNAP_DIR_PATH			"D:/SNAP"								// 抓拍文件夹		
//#define SNAP_MANAGE_PATH		"D:/SNAP/MANAGE.BIN"						// 抓拍管理文件

/**************************************************************************/
/*								  留影留言								  */	
/**************************************************************************/
#define LYLY_DIR_PATH			CFG_TEMP_DRIVE":/LYLY"								// 留影留言文件夹		
#define LYLY_MANAGE_PATH		CFG_TEMP_DRIVE":/LYLY/MANAGE.BIN"						// 留影留言管理文件
//#define LYLY_DIR_PATH			"D:/LYLY"								// 留影留言文件夹		
//#define LYLY_MANAGE_PATH		"D:/LYLY/MANAGE.BIN"						// 留影留言管理文件

// 留言提示音
#define LYLY_HIT_CN_PATH		CFG_PRIVATE_DRIVE":/res/rings/Hit/LYLYCN.WAV"			// 默认中文留言提示音
#define LYLY_HIT_EN_PATH		CFG_PRIVATE_DRIVE":/res/rings/Hit/LYLYEN.WAV"			// 默认英文留言提示音
#define LYLY_HIT_RE_PATH		CFG_PUBLIC_DRIVE":/LYLY"								// 录制留言提示音
#define LYLY_HIT_RE_FILE_PATH	CFG_PUBLIC_DRIVE":/LYLY/LYLY.WAV"						// 录制留言提示音

/**************************************************************************/
/*								  家人留言								  */	
/**************************************************************************/
#define JRLY_DIR_PATH			CFG_TEMP_DRIVE":/JRLY"								// 家人留言文件夹
#define JRLY_MANAGE_PATH		CFG_TEMP_DRIVE":/JRLY/MANAGE.BIN"						// 家人留言管理文件

/**************************************************************************/
/*								  家电									  */	
/**************************************************************************/
#define JD_DIR_PATH				CFG_TEMP_DRIVE":/JD"								
#define JD_LIGHT_VALUE_PATH     CFG_TEMP_DRIVE":/JD/ALIGHT.BIN"
#define JD_KONGTIAO_VALUE_PATH  CFG_TEMP_DRIVE":/JD/AKONGTIAO.BIN"
#define JD_DEVICE_LIGHT_PATH	CFG_TEMP_DRIVE":/JD/LIGHT.BIN"							// 家电灯光管理文件
#define JD_DEVICE_WINDOW_PATH	CFG_TEMP_DRIVE":/JD/WINDOW.BIN"						// 家电窗帘管理文件
#define JD_DEVICE_KONGTIAO_PATH	CFG_TEMP_DRIVE":/JD/KONGTIAO.BIN"						// 家电空调管理文件
#define JD_DEVICE_POWER_PATH	CFG_TEMP_DRIVE":/JD/POWER.BIN"							// 家电电源管理文件
#define JD_DEVICE_GAS_PATH		CFG_TEMP_DRIVE":/JD/GAS.BIN"							// 家电煤气管理文件
#define JD_SCENE_PATH			CFG_TEMP_DRIVE":/JD/SCENE"							// 家电情景管理文件
#define JD_AURINE_SCENE_PATH    CFG_TEMP_DRIVE":/JD/ASCENE.BIN"

/**************************************************************************/
/*								 预约									  */	
/**************************************************************************/
#define YUYUE_DIR_PATH			CFG_TEMP_DRIVE":/YUYUE"								// 预约文件
#define YUYUE_MANAGE_PATH		CFG_TEMP_DRIVE":/YUYUE/YUYUE.BIN"						// 预约列表
//#define EVENT_TIP_PATHID		CFG_TEMP_DRIVE":/APPDATA/TIP/"						// 提示音保存路径

/**************************************************************************/
/*								 视频监视								  */	
/**************************************************************************/
#define MONITOR_DIR_PATH		CFG_TEMP_DRIVE":/MONITOR"				
#define HOME_CAMERA_PATH		CFG_TEMP_DRIVE":/MONITOR/HOME_CAMERA.BIN"	
#define COMMUNITY_CAMERA_PATH	CFG_TEMP_DRIVE":/MONITOR/COMMUNITY_CAMERA.BIN"

#define MONITOR_DOOR_PATH		CFG_TEMP_DRIVE":/MONITOR/MONITOR_DOOR.BIN"	
#define MONITOR_STAIR_PATH		CFG_TEMP_DRIVE":/MONITOR/MONITOR_STAIR.BIN"	
#define MONITOR_AREA_PATH		CFG_TEMP_DRIVE":/MONITOR/MONITOR_AREA.BIN"	

/**************************************************************************/
/*								 屏幕保护								  */	
/**************************************************************************/
#define SCREEN_SAVER_DIR_PATH	CFG_PRIVATE_DRIVE":/res/ScreenSaver/"					// 屏幕保护资源		

/**************************************************************************/
/*								 桌面背景								  */	
/**************************************************************************/
#define WALL_PAPER_DIR_PATH		CFG_PRIVATE_DRIVE":/res/wallpaper/"					// 桌面背景图片资源		

/**************************************************************************/
/*								 留影留言								  */
/**************************************************************************/
#define PHOTO_MSG_DIR_PATH		CFG_PRIVATE_DRIVE":/res/photomsg/"					// 留影留言图片资源		




#ifdef _USE_NEW_CENTER_
/**************************************************************************/
/*								保存注册成功设备编号					  */	
/**************************************************************************/
#define PRE_DEVNO_FILENAME		CFG_TEMP_DRIVE":/pre_devno.dat"
#endif

#endif 

