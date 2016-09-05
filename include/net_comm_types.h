/*************************************************
  Copyright (C), 2006-2010, Aurine
  File name: 	net_comm_types.h
  Author:		xiewr
  Version:  	1.0
  Date: 
  Description:  冠林网络通讯协议_类型定义
				说明：本系统枚举类型除特殊说明外，
				在网络包中均转换为unsigned char。
  History:        
    1. Date:	
       Author:	
       Modification:
    2. ...
*************************************************/
#ifndef _NET_COMM_TYPES_H_
#define _NET_COMM_TYPES_H_

#include "au_types.h"

#ifdef  __cplusplus
extern "C"
{
#endif


// 方向标志
#define DIR_SEND				0xAA 										// 主动
#define DIR_ACK 				0x00 										// 应答
#define DIR_DONOTHING           0xFF                						// 不处理
#define SOCK_ADDR_SIZE			(sizeof(struct sockaddr_in))  			 	// 地址长度
#define NET_PACKBUF_SIZE		(1024*4)             					 	// UDP包最大值

#define PAYLOAD_G711A			8//rtpPayloadPCMA
#define PAYLOAD_G711U			0//rtpPayloadPCMU
#define PAYLOAD_G728			15//rtpPayloadG728
#define PAYLOAD_G729			18//rtpPayloadG729


#define PAYLOAD_H261			31//rtpPayloadH261 
#define PAYLOAD_H263			34//rtpPayloadH263 
#define PAYLOAD_H264			98//rtpPayLoadDyn98
#define PAYLOAD_MPEG4			100//rtpPayLoadDyn100

#ifdef _AU_PROTOCOL_
#define _SUB_PROTOCOL_ENCRYPT_	0x00										// 冠林协议不加密
#define _PROTOCOL_VER_ 			0x0200  									// 协议版本
#define	MAIN_NET_HEAD_SIZE		0											// 主协议网络包头长度
#define	SUB_NET_HEAD_SIZE		(sizeof(NET_HEAD))							// 子协议网络包头长度
#define	NET_HEAD_SIZE			(sizeof(NET_HEAD))							// 网络包头长度
#define MANAGER_NUM				50											// 管理员机起始编号
#define MANAGER_LEN				2											// 管理员机编号长度
#define SCREENSAVE_NUM			12											// 屏保数量

#define NETCMD_UDP_PORT			20625 										// 统一的命令端口,外区为映射后的端口
#define	NETVIDEO_UDP_PORT		24010										// 视频端口
#define	NETAUDIO_UDP_PORT		24020										// 音频端口
#else
#define MAINPROTOCOL_VER        0x0801              						// 主协议版本
#define SUBPROTOCALTYPE         0x08 										// 子协议类型
#define SUBPROTOCALPACKTYPE     0x00										// 子协议打包方式
#define SUBPROTOCALECHO         0x00                						// 子协议应答码

#if (_SUB_PROTOCOL_ENCRYPT_ == _ML_PROTOCOL_ENCRYPT_)
#define ENCRYPT_KEY		"ML80_20140315@yfzx.ml"
#elif (_SUB_PROTOCOL_ENCRYPT_ == _SD_PROTOCOL_ENCRYPT_)
#define ENCRYPT_KEY		"TFSUODI_20141201@yfzx.cn"
#elif (_SUB_PROTOCOL_ENCRYPT_ == _TF_PROTOCOL_ENCRYPT_)
#define ENCRYPT_KEY		"TF_20150201@tianfu.cn"
#elif (_SUB_PROTOCOL_ENCRYPT_ == _HY_PROTOCOL_ENCRYPT_)
#define ENCRYPT_KEY		"TFHUAYING_20160310@tianfu.cn"
#endif

#if (_SUB_PROTOCOL_ENCRYPT_ == _NO_PROTOCOL_ENCRYPT_)
#define ENCRYPT_KEY_LEN		0
#else
#define ENCRYPT_KEY_LEN		strlen(ENCRYPT_KEY)
#endif

// 系统方案定义码
#if (ML8_RK2818 == BOARD_TYPE)
#define SYYCHIPTYPE				0x13                						// RK2818经济版室内机
#elif (ML8_RK3066 == BOARD_TYPE)
#define SYYCHIPTYPE				0x12                						// RK3066室内机
#elif (ML8_RK2918 == BOARD_TYPE)
#define SYYCHIPTYPE				0x11                						// RK2918室内机
#else
#define SYYCHIPTYPE				0x00                						// 未定义
#endif
//#define INADDR_BROADCAST 		0xffffffff

#define	MAIN_NET_HEAD_SIZE		(sizeof(MAIN_NET_HEAD))						// 主协议网络包头长度
#define	SUB_NET_HEAD_SIZE		(sizeof(NET_HEAD))							// 子协议网络包头长度
#define	NET_HEAD_SIZE			(sizeof(MAIN_NET_HEAD)+sizeof(NET_HEAD))	// 网络包头长度
#define MANAGER_NUM				100											// 管理员机起始编号
#define MANAGER_LEN				3											// 管理员机编号长度
#define SCREENSAVE_NUM			6											// 屏保数量

#define NETCMD_UDP_PORT			14301										// 统一的命令端口,外区为映射后的端口
#define	NETVIDEO_UDP_PORT		31410
#define	NETAUDIO_UDP_PORT		31420
#endif

#define NETCMD_ELEVATOR_PORT	8008										// 电梯授权命令端口
#define NET_DATA_MAX			(NET_PACKBUF_SIZE-NET_HEAD_SIZE) 			// 附加数据最大
/***************************以下为枚举类型定义***********************/

// 子协议加密类型
typedef enum
{
	SUB_ENCRYPT_NONE           = 0x00,				// 不加密
	SUB_ENCRYPT_YH             = 0x01				// 异或方式加密
	
}SUB_PROTOCAL_ENCRYPT;

// 主协议包类型
typedef enum
{
	MAIN_PACKTYPE_NORM			= 0x0000,			// 普通包
	MAIN_PACKTYPE_TC			= 0xFFFF			// 透传包

}MAIN_PROTOCAL_PACKTYPE;

#ifdef _AU_PROTOCOL_
// 子系统代号
typedef enum
{
	SSC_PUBLIC					= 0x00,				// 公共部分
	SSC_INTERPHONE				= 0x10,				// 对讲，含区间通话
	SSC_IPPHONE					= 0x11,				// IP电话
	SSC_PSTNPHONE				= 0x12,				// 普通电话
	SSC_ALARM					= 0x20,				// 报警
	SSC_INFO					= 0x30,				// 信息
	SSC_EHOME					= 0x40,				// 家电
	SSC_MULTIMEDIA				= 0x50,				// 多媒体
	SSC_ELIVE					= 0x60,				// E生活
	SSC_OTHER					= 0x70,				// 便利功能
	SSC_VIDEOMONITORING 		= 0x75,		 	   	// 视频监视
	SSC_IPDOORPHONE				= 0x80,				// 家庭网关 add by luofl 2007-9-29
}SUB_SYS_CODE_E;
#else
//  子系统代号
typedef enum
{
	SSC_PUBLIC					= 0xFF,				// 公共部分
	SSC_INTERPHONE				= 0xF0,				// 对讲，含区间通话
	SSC_ALARM					= 0xF5,				// 报警
	SSC_INFO					= 0xE5,				// 信息
	SSC_EHOME					= 0xE0,				// 家电
	SSC_MULTIMEDIA				= 0xD0,				// 多媒体
	SSC_ELIVE					= 0xD1,				// E生活
	SSC_OTHER					= 0xD2,				// 便利功能
	SSC_VIDEOMONITORING 		= 0xC0,		 	   	// 视频监视
	SSC_IPDOORPHONE				= 0xF1,				// 网络门前机
	SSC_SOFTWARE 				= 0XB0				// 冠林网络间协议
}SUB_SYS_CODE_E;
#endif

// 网络应答码
typedef enum
{
	ECHO_OK						= 0x00,				// 正确应答
	ECHO_ERROR					= 0x01,				// 一般错误
	ECHO_BUSY					= 0x02,				// 忙应答
	ECHO_NO_RECORD				= 0x03,				// 请求的信息不存在
	ECHO_UNALLOWDD				= 0x04,				// 不允许远程控制
	ECHO_NO_APPLY				= 0x05,				// 未申请该服务（或无此服务）
	ECHO_CARD_FULL				= 0x06,				// 卡区已满
	ECHO_HAVED					= 0x07,				// 记录已经存在
	ECHO_OFFLINE				= 0x08,				// 请求方离线
	ECHO_MAC					= 0x09,				// MAC码错误
	ECHO_REPEAT					= 0x0A,				// 设置重复
	ECHO_ALARM_ERROR			= 0x0B,				// 安防控制失败, 主要用于布防时，报警防区处于故障、触发等状态
	ECHO_NOT_SUPPORT			= 0x0C              // 不支持该媒体参数
}ECHO_VALUE_E;

// 控制执行者（报警远程控制、家电远程控制）
typedef enum
{
	EXECUTOR_LOCAL_HOST			= 0x01,				// 本机控制
	EXECUTOR_REMOTE_DEVICE		= 0x02,				// 遥控器

	EXECUTOR_LOCAL_FJ			= 0x05,				// 本地网络分机(包含无线终端、家庭PC)
	EXECUTOR_LOCAL_MNFJ			= 0x06,				// 本地模拟分机

	EXECUTOR_MANAGER			= 0x10,				// 管理员机
	EXECUTOR_STAIR				= 0x11,				// 梯口机
	EXECUTOR_AREA				= 0x12, 			// 区口机
	EXECUTOR_IE_USER			= 0x13,				// 远程IE平台
	EXECUTOR_SHORT_MSG			= 0x20,				// 住户短信
	EXECUTOR_PHONE_USER			= 0x21,				// 住户电话
	EXECUTOR_SERVER				= 0x22				// 中心服务器
}EXECUTOR_E;

// 事件发送方
typedef enum
{
	EST_AURINE					= 0x01,				// 发送到冠林平台
	EST_SERVER					= 0x02,				// 发送到中心服务器
	EST_MANAGER					= 0x03				// 发送到管理员机
}EVENT_SENDTO_E;

// 发送包状态
typedef enum
{
	 SEND_STATE_SEND_READY		= 0,				// 等待发送
	 SEND_STATE_SENDING, 							// 正在发送
	 SEND_STATE_WAIT_ECHO,							// 等待应答
	 SEND_STATE_RECIVED_ECHO,						// 收到应答包
	 SEND_STATE_ECHO_TIMEOUT						// 应答超时	
}SEND_STATE_E;

// 网络包优先级
typedef enum
{
	PRIRY_HIGHEST = 0x00,							//最高优先级，一般用于报警
	PRIRY_REALTIME = 0x01,							//实时命令，如开锁等
	PRIRY_DEFAULT = 0xFF							//默认参数；没有定义优先级的，默认填该参数
}PRIRY_TYPE_E;

// 设备类型
typedef enum
{
	DEVICE_TYPE_NONE					= 0x00,	
	DEVICE_TYPE_AURINE_SERVER_COMM 		= 0x10,		// 冠林服务器
	DEVICE_TYPE_AURINE_SERVER_WEB		= 0x11,		// 冠林WEB服务器
	DEVICE_TYPE_AURINE_SERVER_SERVICE	= 0x12,		// 冠林服务子系统
	DEVICE_TYPE_AURINE_SERVER_STREAMINGSERVER = 0x13,  // 流媒体服务器
	DEVICE_TYPE_AURINE_SERVER_SMSMMSSERVER    = 0x14,  // SMS/MMS 服务器
	DEVICE_TYPE_CENTER					= 0x20,		// 中心服务器
	DEVICE_TYPE_MANAGER,							// 管理员机
	DEVICE_TYPE_UNIT_MANAGER,                       // 楼道管理员机
	DEVICE_TYPE_AREA					= 0x30,		// 区口机
	DEVICE_TYPE_STAIR,								// 梯口机
	DEVICE_TYPE_ROOM,								// 室内机
	DEVICE_TYPE_FENJI_NET,							// 网络分机
	DEVICE_TYPE_AREA_ROXY,							// 小区代理，目前计划DNS Server、DHCP Server运行在同一台设备上
	DEVICE_TYPE_MEDIA_AD,							// 媒体发布，广告播放
	DEVICE_TYPE_PHONE					= 0x40,		// 普通电话
	DEVICE_TYPE_DOOR_PHONE,							// 门前机(电话)
	DEVICE_TYPE_DOOR_NET,							// 门前机(网络)
	DEVICE_TYPE_GATEWAY					= 0x50,		// 家庭网关
	DEVICE_TYPE_IPCAMERA,							// IP Camera
	DEVICE_TYPE_IPC						= 0x70,		// 社区IPC设备
	DEVICE_TYPE_HOME_IPC				= 0x71		// 家居IPC设备
}DEVICE_TYPE_E;

// 视频编码格式
typedef enum	
{
	VIDEO_ENCMODE_NONE 	= 0,
	VIDEO_ENCMODE_H264 	= 1
}VIDEO_ENCMODE_E;

// 视频分辨率
typedef enum
{
	VIDEO_SIZE_NONE		= 0,
	VIDEO_SIZE_QQVGA	= 1,
	VIDEO_SIZE_QCIF		= 2,
	VIDEO_SIZE_QVGA		= 3,
	VIDEO_SIZE_CIF		= 4,
	VIDEO_SIZE_VGA		= 5,
	VIDEO_SIZE_4CIF		= 6, //704*576
	VIDEO_SIZE_D1		= 7,
	VIDEO_SIZE_SVGA		= 8, //800*600
	VIDEO_SIZE_720P		= 9
}VIDEO_SIZE_E;

// 音频格式
typedef enum
{
	AUDIO_ENCMODE_NONE	= 0,
	AUDIO_ENCMODE_PCMA	= 1,
	AUDIO_ENCMODE_PCMU	= 2
}AUDIO_ENCMODE_E;

typedef enum
{
	ALARM_MODE_UNSET 	=0x00,					//撤防状态
	ALARM_MODE_SET 		=0x01,					//布防状态
	ALARM_MODE_PARTSET	=0x02,					//局防状态
	ALARM_MODE_NONE		=0x03,					//未知
}ALARM_MODE_E;

typedef enum
{
	CALL_STATE_NONE,		  					// 无
	CALL_STATE_CALLING,							// 呼叫中
	CALL_STATE_RECORDHINT,						// 留言模式提示音
	CALL_STATE_RECORDING,						// 留言模式录制
	CALL_STATE_TALK,							// 通话状态
	CALL_STATE_END,								// 通话结束
	CALL_STATE_MOVETEMP							// 临时转向中
}CALL_STATE_E;

#define CALL_TIMER				(CALL_STATE_END+10) 	// 对讲计时
#define CALL_STATE_REQUEST		(CALL_STATE_END+11) 	// 对讲请求
#define CALL_NEW_CALLING		(CALL_STATE_END+12) 	// 对讲新呼入
#define CALL_FORMSHOW_CALLING	(CALL_STATE_END+13) 	// 对讲呼入显示界面提醒
#define CALL_SNAP_CALLBACK		(CALL_STATE_END+14) 	// 抓拍图片回调


// 家电类型
typedef enum
{
	EHOME_TYPE_LIGHT			=0x00,			//灯
	EHOME_TYPE_AIRCONDITION		=0x01,			//空调
	EHOME_TYPE_CURTAIN			=0x02,			//窗帘
	EHOME_TYPE_POWER			=0x03,			//电源
	EHOME_TYPE_GAS				=0x04, 			//煤气
	EHOME_TYPE_OTHER			=0x05			//其它设备
}EHOME_TYPE_E;

// 家电情景模式
typedef enum
{
	JD_SCENE_RECEPTION 		= 0x00,				// 会客模式
	JD_SCENE_MEALS,								// 就餐模式
	JD_SCENE_NIGHT,								// 夜间模式
	JD_SCENE_POWERSAVING,						// 节电模式
	JD_SCENE_GENERAL,							// 普通模式
	JD_SCENE_MAX,
	JD_SCENE_NONE,
}AU_JD_SCENE_MODE;								// 家电情景模式

/***************************以上为枚举类型定义***********************/



/********************以下为网络命令相关的结构类型定义************************/

// 开始定义数据包, 使用字节对齐方式
#pragma pack(push,1)  

#ifdef _AU_PROTOCOL_
typedef struct
{
	uint32		DeviceNo1;							// 设备号高位，不大于999999999
	uint32		DeviceNo2;							// 设备号低位，不大于999999999
}DEVICE_NO, *PDEVICE_NO;
#else
// 设备编号（房号）
typedef struct
{
	uint8 		DeviceType;                         // 设备类型 
	uint8 		Resver;								// 预留位
	uint16 		AreaNo;								// 小区编号
	uint32		DeviceNo1;							// 设备号高位，不大于999999999
	uint32		DeviceNo2;							// 设备号低位，不大于999999999
}DEVICE_NO, *PDEVICE_NO;
#endif

// 设备编号规则, 或放在存储头文件中定义
typedef struct
{
	uint8		StairNoLen;							// 梯号长度，默认4
	uint8		RoomNoLen;							// 房号长度，默认4
	uint8		CellNoLen;							// 单元号长度，默认2
	uint8		UseCellNo;							// 启动单元标志，0－false 1－true， 默认1
	uint32		Subsection;							// 分段参数，0为不分段，默认224
}DEVICENO_RULE;

// 网络主包头 ML80
typedef struct
{
	uint8 		subProtocolType;                   	// 子协议类型
	uint8 		DirectFlag;							// 包方向标志
	uint16 		mainProtocolType;					// 主协议版本
	uint8 		hwEncrypt[16];						// 硬件设备加密密码
	uint32 		mainSeq;							// 数据包序列号
	uint8 		sysChipType;						// 系统芯片方案定义码
	uint8 		subProtocolPackType;				// 子协议打包类型
	uint8 		subProtocolEncrypt;					// 子协议加密类型
	uint8 		EchoValue;							// 应答码
	uint16	 	mainPacketType;						// 主协议包类型
	uint16 		subPacketLen;                       // 子协议包长度
} MAIN_NET_HEAD, *PMAIN_NET_HEAD;

#ifdef _AU_PROTOCOL_
// 网络包头
typedef struct
{
	uint16		PackageID;							// 包标识
	uint16		ProtocolVer;						// 协议版本
	uint32		AreaCode;							// 小区编号
	DEVICE_NO	DeviceNo;							// 设备编号
	uint8		DeviceType;							// 设备类型
	uint8		DirectFlag;							// 方向标志（主动：0xAA；应答：0x00）
	uint8		SubSysCode;							// 子系统代号
	uint8		command;							// 命令值
	uint8		EchoValue;							// 应答码
	uint8		Encrypt;							// 加密类型
	uint16		DataLen;							// 数据长度
}NET_HEAD, *PNET_HEAD;
#else
typedef struct
{ 
	uint32		PackageID;                          // 包标识
	uint16 		ProtocolVer;						// 协议版本
	uint16 		Priry;								// 包优先级
	DEVICE_NO 	SrcDeviceNo;						// 源设备编号
	DEVICE_NO 	DestDeviceNo;						// 目标设备编号
	//uint8 		Resver;								// 保留位
	uint8 		DeviceType;
	uint8 		DirectFlag;							// 方向标志
	uint8 		SubSysCode;							// 子系统代号
	uint8		command;							// 命令
	uint8		EchoValue;							// 应答码
	uint8 		Encrypt;							// 加密类型
	uint16 		DataLen;							//数据长度
}NET_HEAD, *PNET_HEAD;
#endif

// 时间结构
typedef struct
{
	uint16		year;								// 年
	uint8		month;								// 月
	uint8		day;								// 日
	uint8		week;								// 星期
	uint8		hour;								// 小时
	uint8		min;								// 分
	uint8		sec;								// 秒
}DATE_TIME, *PDATE_TIME;

typedef struct
{
	uint8   encMode;								// 视频编码格式 0为不受限制
	uint8   frameRate;								// 视频帧率     0为不受限制
	uint8   imgSize;								// 视频分辨率   0为不受限制
	uint8   resver0;								// 保留位
	uint32  bitRate;								// 视频码率 单位(kbps) 0不受限制
	uint32  resver1;								// 保留位
	uint32  resver2;								// 保留位
} VIDEO_SDP_PARAM, * PVIDEO_SDP_PARAM;

typedef struct
{
	uint8 encMode;									// 音频格式 0为不受限制
	uint8 resver0;
	uint16 resver1;
	uint32 resver2;
}AUDIO_SDP_PARAM, * PAUDIO_SDP_PARAM;

// 短信接收格式
typedef struct
{
	DATE_TIME	SendTime;							// 发送时间
	uint8		level;								// 级别 0-一般, 1-紧急, 2－特急
	char		SendNo[20];							// 发送号码
	char		Content[200];						// 内容, 一般短信规定英文160个字符，汉字70个
}REC_SHORT_MSG;

// 短信发送格式
typedef struct
{
	uint8		level;								// 级别 0-一般, 1-紧急, 2－特急
	char		ReceiveNo[20];						// 接收号码
	char		Content[200];						// 内容, 一般短信规定英文160个字符，汉字70个
}SEND_SHORT_MSG;

// 接收信息头
typedef struct
{
	uint32		ID;									// 信息ID号 
	uint8		BulletinType;						// 类型 0-文字  1－图片 2－音乐 3－文字.音乐 4－图片.音乐
	char		title[40];							// 标题
	uint32		CharLen;							// 文字部分长度
	uint32		ImageLen;							// 图片部分长度
	uint32		VoiceLen;							// 声音部分长度
}REC_BULLETIN_HEAD;

#pragma pack(pop)  									// 结束定义数据包, 恢复原对齐方式
/********************以上为网络命令相关的结构类型定义************************/

/***************************以下为本地使用网络包定义**************************/
// 网络发送包
typedef struct SEND_PACKET_NODE
{
	struct SEND_PACKET_NODE * next;
	struct SEND_PACKET_NODE * prev;
	char SendState;									// 发送状态, 见TSendState
	char SendTimes; 								// 当前发送次数
	char SendTimesMax; 								// 最多发送次数
	int TimeOut;									// 发送超时时间, 秒
	int WaitCount; 									// 等待时间计数
	uint32 address;									// 对方IP地址
	uint16 port;									// 对方端口号
	int size;										// 数据包大小
	char * data;									// 发送数据
}SEND_PACKET, *PSEND_PACKET;

// 网络接收包
typedef struct RECIVE_PACKET_NODE
{
	struct RECIVE_PACKET_NODE * next;
	uint32 address;									// 对方IP地址
	uint16 port;									// 对方端口号
	int size;										// 数据包大小
	char * data;									// 发送数据
}RECIVE_PACKET, *PRECIVE_PACKET;
/***************************以上为本地使用网络包定义**************************/

#ifdef __cplusplus
}
#endif
#endif 

