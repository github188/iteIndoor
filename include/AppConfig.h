/*************************************************
  Copyright (C), 2006-2016, Aurine
  File name:  	AppConfig.h
  Author:     	luofl
  Version:    	2.0
  Date: 
  Description:  N32926方案已实现的功能（通过宏定义来开启相应的功能）
  				#define	_ELEVATOR_MODE_				// 电梯授权功能
  				#define _DOOR_PHONE_				// 模拟门前机
*************************************************/
#ifndef __APPCONFIG_H__
#define __APPCONFIG_H__

//#define AEC_DEBUG									// 消回声保存文件
#define SCREEN_CAPTURE			0					// 屏幕截图,按下开锁键拍一张保存在D盘中。

// ====定义提示音音量========================================
#define MIC_VOLUME				4					// 咪头输入音量
#define MSG_HIT_VOL				4					// 信息通知提示音大小
#define AF_HIT_VOL				4					// 安防报警提示音大小
#define LYLY_VIEW_VOL			4					// 留影留言播放声
#define MSG_VIEW_VOL			4					// 信息查看声音大小
#define JRLY_VOLUME				4					// 家人留言播放音量大小
#define RING_OUT_VOL			4					// 回铃声大小

// ====定义设备最大数目======================================
#define DOOR_DEVICE_NUM			2					// 门前机最大数
#define STAIR_DEVICE_NUM		20					// 梯口分机最大数
#define AREA_DEVICE_NUM			99					// 区口机最大数 
#define ROOM_DEVICE_NUM			8					// 室内分机最大数 
#define MANAGE_DEVICE_NUM		3					// 管理中心最大数 
#define IPC_DEVICE_NUM			99					// 社区IPC设备最大数 
#define MAX_HOME_NUM  			16					// 家居IPC设备最大数

// ====定义记录存储条数======================================
#define AF_REC_MAX 	  			20					// 安防事件最大记录数
#define MAX_RECORD_NUM  		20					// 通话记录最大条数
#define MAX_JRLY_NUM  			5					// 家人留言最大记录数
#define MAX_LYLY_NUM  			6					// 留影留言最大记录
#define MAX_MSG_NUM				10					// 信息最大接收条数
#define MAX_PHOTO_NUM  			16					// 最大抓拍记录数
#define MAX_YUYUE_NUM			20					// 家电预约最大数

// ====定义执行时间最大值====================================
#define MONITOR_TIME_MAX		90					// 监视最长时间
#define	MONITOR_TALK_TIME_MAX	90					// 通话最长时间
#define	HEART_TIMEOUT			5					// 心跳超时
#define	CALLOUT_TIMEOUT			60					// 呼叫超时
#define LEAVEWORD_TIMEOUT		60					// 超时进入留言的时间
#define	RECORDHINT_TIME			30					// 留言提示音时间
#define LEAVEWORD_TIME			30					// 留言最长时间
#define PHONE_TALK_TIMEOUT		60					// 模拟门口机通话时长
#define FENJI_TALK_TIMEOUT		300					// 户户通通话时长

#define RECORD_TIME_MAX			30					// 留言最大时间(家人留言、留影留言、留言录制音)


// ====各版本功能定义============================================
//-----------公共功能------------------
#define _JD_MODE_									// 家电模块
#define	_YUYUE_MODE_								// 启用预约功能
#define _SHOW_USED_JDDEV_ONLY_						// 家电控制和情景设置界面只显示启用的设备
#define _USE_ELEVATOR_								// 启用电梯控制器
#define _TY_STAIR_									// 支持通用梯口机
#define _AURINE_ELEC_NEW_                           // add by fanfj增加新冠林家电
#define _DIANTI_CALL_								// 主动召梯功能
#define _IP_MODULE_									// IP模块
#define _LIVEMEDIA_									// RTSP模块
#define _USE_NEW_CENTER_							// 使用新版本中心服务器
#define _SEND_SDP_PARAM_							// 是否发送媒体参数
#define _RTSP_REG_KEEP_								// RTSP心跳保持(目的获取室内摄像头监视列表)
//#define _TIMER_REBOOT_NO_LCD_           			// 定时重启时不开背光
//#define _UPLOAD_PHOTO_							// 网络门前机拍照上传功能
#ifdef _IP_MODULE_
#define _IP_MODULE_DJ_								// IP模块对讲
#define _IP_MODULE_ALARM							// 报警功能
#define _IP_MODULE_JD_								// IP模块家电
//#define _IP_MODULE_RTSP_							// RTSP注册
#endif
//#define _USE_FOR_SHOW_								// 展会临时版本

// 硬件加密码也就是厂商代码
#define _ML_HWENCRYPT_			0X00004753			// 米立使用的硬件加密码
#define _TF_HWENCRYPT_			0X54460000			// 天富使用的硬件加密码
#define _ZH_HWENCRYPT_			0X00005A48			// 中海使用的硬件加密码
#define _SD_HWENCRYPT_			0X54465344			// 索迪使用的硬件加密码
#define _HY_HWENCRYPT_			0X54464859			// 天富华鹰的硬件加密码


// 协议版本号
#define _ML_PROTOCOL_VER_		0X0101				// 米立协议版本
#define _TF_PROTOCOL_VER_		0XA0AF				// 天富协议版本
#define _ZH_PROTOCOL_VER_   	0XA101				// 中海协议版本
#define _SD_PROTOCOL_VER_		0XA001				// 索迪协议版本
#define _HY_PROTOCOL_VER_		0XA0A1				// 华鹰协议版本


// 协议加密类型,不同的加密类型有不同的KEY, 0-不加密
#define _NO_PROTOCOL_ENCRYPT_   0x00				// 协议不加密
#define _TF_PROTOCOL_ENCRYPT_ 	0xAF				// 天富协议加密类型
#define _SD_PROTOCOL_ENCRYPT_   0xA0				// 索迪协议加密类型
#define _HY_PROTOCOL_ENCRYPT_   0xA1				// 华鹰协议加密类型

// ====定义结构件============================================
#define ML8_V1					1					
#define ML8_V6					2
#define TF8_A1					3
#define AH8_E81					4
#define ML8_V7					5
#define ML8_V5					6

// ====版本定义================================================
#define E81_AH_VER				1					// 冠林E81版本
#define V5M_ML_VER				2					// 米粒V5M版本
#define V5M_TF_VER				3					// 天富V5M版本
#define V7M_ML_VER				4					// 米粒V7M版本
#define V7M_TF_VER				5					// 天富V7M版本

#define VER_TYPE				E81_AH_VER



//-----------各版本差异功能-------------
#if (VER_TYPE == E81_AH_VER)
#define _AU_PROTOCOL_									// 使用冠林协议
#define _USE_X10_JD_									// 使用X10家电
#undef _USE_NEW_CENTER_									// 不使用新的中心机

#define HARD_VER_COMM		    "AH8-E81M"
#define JIEGOU_TYPE				AH8_E81
#define _ALARM_IO_										// 报警直接用IO口
#define _CP2526_TOUCH_KEY_								// 启用感应按键
#define _TIMER_REBOOT_NO_LCD_           				// 定时重启时不开背光

#elif (VER_TYPE == V7M_ML_VER)
#define _HW_ENCRYPT_			_ML_HWENCRYPT_			// 该版本使用的硬件加密码
#define _PROTOCOL_VER_			_ML_PROTOCOL_VER_		// 该版本使用的协议版本
#define _SUB_PROTOCOL_ENCRYPT_  _NO_PROTOCOL_ENCRYPT_	// 协议加密类型,米立版本不加密

#define HARD_VER_COMM		    "SNVX98-100101"
#define JIEGOU_TYPE				ML8_V7
#define _ALARM_IO_										// 报警直接用IO口
#define _CP2526_TOUCH_KEY_								// 启用感应按键
#define _NEW_SELF_IPC_									// 使用新的本公司IPC家居监视
//#define _TIMER_REBOOT_NO_LCD_           				// 定时重启时不开背光

#endif


// ====定义版本号============================================
//-----------硬件版本------------------
#ifndef HARD_VER_COMM
#define HARD_VER_COMM		    "ITE9856-"
#endif

//----------软件版本---------------------
#define SOFT_VER_COMM			"V2.03.0001_"


#define BUILD_TIME				__TIME__ "_" __DATE__

#ifndef HARDSUBVER
#define HARDSUBVER				"-REL"					// 如果是定制单 为项目每字首字母大写
#endif

#ifndef SOFTSUBVER
#define SOFTSUBVER				BUILD_TIME
#endif

#define HARD_VER				(HARD_VER_COMM HARDSUBVER)
#define SOFT_VER				(SOFT_VER_COMM SOFTSUBVER)
#define SDK_VER					BUILD_TIME
#endif

