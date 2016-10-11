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
#define LYLY_VIEW_VOL			8					// 留影留言播放声
#define MSG_VIEW_VOL			4					// 信息查看声音大小
#define JRLY_VOLUME				8					// 家人留言播放音量大小
#define RING_OUT_VOL			4					// 回铃声大小

// ====定义记录存储条数======================================
#define AF_REC_MAX 	  			20					// 安防事件最大记录数
#define MAX_RECORD_NUM  		20					// 通话记录最大条数
#define MAX_JRLY_NUM  			5					// 家人留言最大记录数
#define MAX_LYLY_NUM  			5					// 留影留言最大记录
#define MAX_MSG_NUM				10					// 信息最大接收条数
#define MAX_PHOTO_NUM  			10					// 最大抓拍记录数
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

// ====定义结构件============================================
#define ML8_V1					1
#define ML8_V6					2
#define TF8_A1					3
#define AH8_E81					4
#define ML8_V7					5
#define ML8_V5					6

// ====定义硬件类型=========================================
#define ML8_N32926_V1S			1					// ML8-V1S室内机
#define ML8_N32926_V6S			2					// ML8-V6S室内机
#define TF8_N32926_A1S			3					// TF8-A1S室内机
#define AH8_N32926_E81M			4					// AH8-E81/E91/F91M室内机
#define ML8_N32926_V7M			5					// ML8-V7M室内机

// ====版本定义================================================
#define V1_JHB_VER				1					// ML8-V1S简化版,采用机械按键和4.3寸液晶屏
#define V6_JHB_VER				2					// ML8-V6S简化版,采用机械按键和7寸液晶屏
#define A1_JHB_VER				3					// TF8-A1S简化版,采用机械按键和4.3寸液晶屏
#define E81_BZB_VER				4					// AH8-E81/E91/F91M标准版室内机,采用感应按键,电容屏和7寸液晶屏
#define V7_BZB_VER				5					// ML8-V7M标准版室内机,采用感应按键,电容屏和7寸液晶屏
#define V5_BZB_VER				6					// ML8-V5M标准版室内机,采用感应按键,电容屏和7寸液晶屏
#define V6_BZB_VER				7					// ML8-V6M标准版室内机,采用感应按键,电容屏和7寸液晶屏
#define V8_BZB_VER				8					// ML8-V8M标准版室内机,采用感应按键,电容屏和7寸液晶屏

#define SYS_TYPE				V7_BZB_VER

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

//-----------各版本差异功能-------------
#if (SYS_TYPE == E81_BZB_VER)
#if 1
#define _AU_PROTOCOL_									// 使用冠林协议
#define _USE_X10_JD_									// 使用X10家电
#undef _USE_NEW_CENTER_									// 不使用新的中心机
#else
#define _HW_ENCRYPT_			_ML_HWENCRYPT_			// 该版本使用的硬件加密码
#define _PROTOCOL_VER_			_ML_PROTOCOL_VER_		// 该版本使用的协议版本
#define _SUB_PROTOCOL_ENCRYPT_  _NO_PROTOCOL_ENCRYPT_	// 协议加密类型,米立版本不加密
#endif

#define	SOFTSUBVER				"_AH8-E81M"
#define HARDSUBVER				"E81M-V2.0"
#define BOARD_TYPE				AH8_N32926_E81M
#define JIEGOU_TYPE				ML8_V1
#define _TFT_7_											// 7寸屏
#define _ALARM_IO_										// 报警直接用IO口
#define _CP2526_TOUCH_KEY_								// 启用感应按键
#define _CP_7INCH_TOUCH_								// 启用7寸电容屏
#define _TIMER_REBOOT_NO_LCD_           				// 定时重启时不开背光

#elif (SYS_TYPE == V7_BZB_VER)
#if 1
#define _HW_ENCRYPT_			_ML_HWENCRYPT_			// 该版本使用的硬件加密码
#define _PROTOCOL_VER_			_ML_PROTOCOL_VER_		// 该版本使用的协议版本
#define _SUB_PROTOCOL_ENCRYPT_  _NO_PROTOCOL_ENCRYPT_	// 协议加密类型,米立版本不加密
#else
#define _HW_ENCRYPT_			_TF_HWENCRYPT_			// 该版本使用的硬件加密码
#define _PROTOCOL_VER_			_TF_PROTOCOL_VER_		// 该版本使用的协议版本
#define _SUB_PROTOCOL_ENCRYPT_  _TF_PROTOCOL_ENCRYPT_	// 协议加密类型
#endif
#ifdef _USE_NEW_CENTER_
#define HARD_VER_COMM		    "SNV792-100101"
#else
#define	SOFTSUBVER				"_ML8-V7M"
#define HARDSUBVER				"ML8-V2.0"
#endif
#define BOARD_TYPE				ML8_N32926_V7M
#define JIEGOU_TYPE				ML8_V7
#define _TFT_7_											// 7寸屏
#define _ALARM_IO_										// 报警直接用IO口
#define _CP2526_TOUCH_KEY_								// 启用感应按键
#define _CP_7INCH_TOUCH_								// 启用7寸电容屏
#define _NO_LOGO_										// 不显示logo
#define _NEW_SELF_IPC_									// 使用新的本公司IPC家居监视

#endif


// ====定义版本号============================================
//-----------硬件版本------------------
#ifndef HARD_VER_COMM
#define HARD_VER_COMM		    "ITE-"
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

