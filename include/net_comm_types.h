/*************************************************
  Copyright (C), 2006-2010, Aurine
  File name: 	net_comm_types.h
  Author:		xiewr
  Version:  	1.0
  Date: 
  Description:  ��������ͨѶЭ��_���Ͷ���
				˵������ϵͳö�����ͳ�����˵���⣬
				��������о�ת��Ϊunsigned char��
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


// �����־
#define DIR_SEND				0xAA 										// ����
#define DIR_ACK 				0x00 										// Ӧ��
#define DIR_DONOTHING           0xFF                						// ������
#define SOCK_ADDR_SIZE			(sizeof(struct sockaddr_in))  			 	// ��ַ����
#define NET_PACKBUF_SIZE		(1024*4)             					 	// UDP�����ֵ

#define PAYLOAD_G711A			8//rtpPayloadPCMA
#define PAYLOAD_G711U			0//rtpPayloadPCMU
#define PAYLOAD_G728			15//rtpPayloadG728
#define PAYLOAD_G729			18//rtpPayloadG729


#define PAYLOAD_H261			31//rtpPayloadH261 
#define PAYLOAD_H263			34//rtpPayloadH263 
#define PAYLOAD_H264			98//rtpPayLoadDyn98
#define PAYLOAD_MPEG4			100//rtpPayLoadDyn100

#ifdef _AU_PROTOCOL_
#define _SUB_PROTOCOL_ENCRYPT_	0x00										// ����Э�鲻����
#define _PROTOCOL_VER_ 			0x0200  									// Э��汾
#define	MAIN_NET_HEAD_SIZE		0											// ��Э�������ͷ����
#define	SUB_NET_HEAD_SIZE		(sizeof(NET_HEAD))							// ��Э�������ͷ����
#define	NET_HEAD_SIZE			(sizeof(NET_HEAD))							// �����ͷ����
#define MANAGER_NUM				50											// ����Ա����ʼ���
#define MANAGER_LEN				2											// ����Ա����ų���
#define SCREENSAVE_NUM			12											// ��������

#define NETCMD_UDP_PORT			20625 										// ͳһ������˿�,����Ϊӳ���Ķ˿�
#define	NETVIDEO_UDP_PORT		24010										// ��Ƶ�˿�
#define	NETAUDIO_UDP_PORT		24020										// ��Ƶ�˿�
#else
#define MAINPROTOCOL_VER        0x0801              						// ��Э��汾
#define SUBPROTOCALTYPE         0x08 										// ��Э������
#define SUBPROTOCALPACKTYPE     0x00										// ��Э������ʽ
#define SUBPROTOCALECHO         0x00                						// ��Э��Ӧ����

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

// ϵͳ����������
#if (ML8_RK2818 == BOARD_TYPE)
#define SYYCHIPTYPE				0x13                						// RK2818���ð����ڻ�
#elif (ML8_RK3066 == BOARD_TYPE)
#define SYYCHIPTYPE				0x12                						// RK3066���ڻ�
#elif (ML8_RK2918 == BOARD_TYPE)
#define SYYCHIPTYPE				0x11                						// RK2918���ڻ�
#else
#define SYYCHIPTYPE				0x00                						// δ����
#endif
//#define INADDR_BROADCAST 		0xffffffff

#define	MAIN_NET_HEAD_SIZE		(sizeof(MAIN_NET_HEAD))						// ��Э�������ͷ����
#define	SUB_NET_HEAD_SIZE		(sizeof(NET_HEAD))							// ��Э�������ͷ����
#define	NET_HEAD_SIZE			(sizeof(MAIN_NET_HEAD)+sizeof(NET_HEAD))	// �����ͷ����
#define MANAGER_NUM				100											// ����Ա����ʼ���
#define MANAGER_LEN				3											// ����Ա����ų���
#define SCREENSAVE_NUM			6											// ��������

#define NETCMD_UDP_PORT			14301										// ͳһ������˿�,����Ϊӳ���Ķ˿�
#define	NETVIDEO_UDP_PORT		31410
#define	NETAUDIO_UDP_PORT		31420
#endif

#define NETCMD_ELEVATOR_PORT	8008										// ������Ȩ����˿�
#define NET_DATA_MAX			(NET_PACKBUF_SIZE-NET_HEAD_SIZE) 			// �����������
/***************************����Ϊö�����Ͷ���***********************/

// ��Э���������
typedef enum
{
	SUB_ENCRYPT_NONE           = 0x00,				// ������
	SUB_ENCRYPT_YH             = 0x01				// ���ʽ����
	
}SUB_PROTOCAL_ENCRYPT;

// ��Э�������
typedef enum
{
	MAIN_PACKTYPE_NORM			= 0x0000,			// ��ͨ��
	MAIN_PACKTYPE_TC			= 0xFFFF			// ͸����

}MAIN_PROTOCAL_PACKTYPE;

#ifdef _AU_PROTOCOL_
// ��ϵͳ����
typedef enum
{
	SSC_PUBLIC					= 0x00,				// ��������
	SSC_INTERPHONE				= 0x10,				// �Խ���������ͨ��
	SSC_IPPHONE					= 0x11,				// IP�绰
	SSC_PSTNPHONE				= 0x12,				// ��ͨ�绰
	SSC_ALARM					= 0x20,				// ����
	SSC_INFO					= 0x30,				// ��Ϣ
	SSC_EHOME					= 0x40,				// �ҵ�
	SSC_MULTIMEDIA				= 0x50,				// ��ý��
	SSC_ELIVE					= 0x60,				// E����
	SSC_OTHER					= 0x70,				// ��������
	SSC_VIDEOMONITORING 		= 0x75,		 	   	// ��Ƶ����
	SSC_IPDOORPHONE				= 0x80,				// ��ͥ���� add by luofl 2007-9-29
}SUB_SYS_CODE_E;
#else
//  ��ϵͳ����
typedef enum
{
	SSC_PUBLIC					= 0xFF,				// ��������
	SSC_INTERPHONE				= 0xF0,				// �Խ���������ͨ��
	SSC_ALARM					= 0xF5,				// ����
	SSC_INFO					= 0xE5,				// ��Ϣ
	SSC_EHOME					= 0xE0,				// �ҵ�
	SSC_MULTIMEDIA				= 0xD0,				// ��ý��
	SSC_ELIVE					= 0xD1,				// E����
	SSC_OTHER					= 0xD2,				// ��������
	SSC_VIDEOMONITORING 		= 0xC0,		 	   	// ��Ƶ����
	SSC_IPDOORPHONE				= 0xF1,				// ������ǰ��
	SSC_SOFTWARE 				= 0XB0				// ���������Э��
}SUB_SYS_CODE_E;
#endif

// ����Ӧ����
typedef enum
{
	ECHO_OK						= 0x00,				// ��ȷӦ��
	ECHO_ERROR					= 0x01,				// һ�����
	ECHO_BUSY					= 0x02,				// æӦ��
	ECHO_NO_RECORD				= 0x03,				// �������Ϣ������
	ECHO_UNALLOWDD				= 0x04,				// ������Զ�̿���
	ECHO_NO_APPLY				= 0x05,				// δ����÷��񣨻��޴˷���
	ECHO_CARD_FULL				= 0x06,				// ��������
	ECHO_HAVED					= 0x07,				// ��¼�Ѿ�����
	ECHO_OFFLINE				= 0x08,				// ��������
	ECHO_MAC					= 0x09,				// MAC�����
	ECHO_REPEAT					= 0x0A,				// �����ظ�
	ECHO_ALARM_ERROR			= 0x0B,				// ��������ʧ��, ��Ҫ���ڲ���ʱ�������������ڹ��ϡ�������״̬
	ECHO_NOT_SUPPORT			= 0x0C              // ��֧�ָ�ý�����
}ECHO_VALUE_E;

// ����ִ���ߣ�����Զ�̿��ơ��ҵ�Զ�̿��ƣ�
typedef enum
{
	EXECUTOR_LOCAL_HOST			= 0x01,				// ��������
	EXECUTOR_REMOTE_DEVICE		= 0x02,				// ң����

	EXECUTOR_LOCAL_FJ			= 0x05,				// ��������ֻ�(���������նˡ���ͥPC)
	EXECUTOR_LOCAL_MNFJ			= 0x06,				// ����ģ��ֻ�

	EXECUTOR_MANAGER			= 0x10,				// ����Ա��
	EXECUTOR_STAIR				= 0x11,				// �ݿڻ�
	EXECUTOR_AREA				= 0x12, 			// ���ڻ�
	EXECUTOR_IE_USER			= 0x13,				// Զ��IEƽ̨
	EXECUTOR_SHORT_MSG			= 0x20,				// ס������
	EXECUTOR_PHONE_USER			= 0x21,				// ס���绰
	EXECUTOR_SERVER				= 0x22				// ���ķ�����
}EXECUTOR_E;

// �¼����ͷ�
typedef enum
{
	EST_AURINE					= 0x01,				// ���͵�����ƽ̨
	EST_SERVER					= 0x02,				// ���͵����ķ�����
	EST_MANAGER					= 0x03				// ���͵�����Ա��
}EVENT_SENDTO_E;

// ���Ͱ�״̬
typedef enum
{
	 SEND_STATE_SEND_READY		= 0,				// �ȴ�����
	 SEND_STATE_SENDING, 							// ���ڷ���
	 SEND_STATE_WAIT_ECHO,							// �ȴ�Ӧ��
	 SEND_STATE_RECIVED_ECHO,						// �յ�Ӧ���
	 SEND_STATE_ECHO_TIMEOUT						// Ӧ��ʱ	
}SEND_STATE_E;

// ��������ȼ�
typedef enum
{
	PRIRY_HIGHEST = 0x00,							//������ȼ���һ�����ڱ���
	PRIRY_REALTIME = 0x01,							//ʵʱ����翪����
	PRIRY_DEFAULT = 0xFF							//Ĭ�ϲ�����û�ж������ȼ��ģ�Ĭ����ò���
}PRIRY_TYPE_E;

// �豸����
typedef enum
{
	DEVICE_TYPE_NONE					= 0x00,	
	DEVICE_TYPE_AURINE_SERVER_COMM 		= 0x10,		// ���ַ�����
	DEVICE_TYPE_AURINE_SERVER_WEB		= 0x11,		// ����WEB������
	DEVICE_TYPE_AURINE_SERVER_SERVICE	= 0x12,		// ���ַ�����ϵͳ
	DEVICE_TYPE_AURINE_SERVER_STREAMINGSERVER = 0x13,  // ��ý�������
	DEVICE_TYPE_AURINE_SERVER_SMSMMSSERVER    = 0x14,  // SMS/MMS ������
	DEVICE_TYPE_CENTER					= 0x20,		// ���ķ�����
	DEVICE_TYPE_MANAGER,							// ����Ա��
	DEVICE_TYPE_UNIT_MANAGER,                       // ¥������Ա��
	DEVICE_TYPE_AREA					= 0x30,		// ���ڻ�
	DEVICE_TYPE_STAIR,								// �ݿڻ�
	DEVICE_TYPE_ROOM,								// ���ڻ�
	DEVICE_TYPE_FENJI_NET,							// ����ֻ�
	DEVICE_TYPE_AREA_ROXY,							// С������Ŀǰ�ƻ�DNS Server��DHCP Server������ͬһ̨�豸��
	DEVICE_TYPE_MEDIA_AD,							// ý�巢������沥��
	DEVICE_TYPE_PHONE					= 0x40,		// ��ͨ�绰
	DEVICE_TYPE_DOOR_PHONE,							// ��ǰ��(�绰)
	DEVICE_TYPE_DOOR_NET,							// ��ǰ��(����)
	DEVICE_TYPE_GATEWAY					= 0x50,		// ��ͥ����
	DEVICE_TYPE_IPCAMERA,							// IP Camera
	DEVICE_TYPE_IPC						= 0x70,		// ����IPC�豸
	DEVICE_TYPE_HOME_IPC				= 0x71		// �Ҿ�IPC�豸
}DEVICE_TYPE_E;

// ��Ƶ�����ʽ
typedef enum	
{
	VIDEO_ENCMODE_NONE 	= 0,
	VIDEO_ENCMODE_H264 	= 1
}VIDEO_ENCMODE_E;

// ��Ƶ�ֱ���
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

// ��Ƶ��ʽ
typedef enum
{
	AUDIO_ENCMODE_NONE	= 0,
	AUDIO_ENCMODE_PCMA	= 1,
	AUDIO_ENCMODE_PCMU	= 2
}AUDIO_ENCMODE_E;

typedef enum
{
	ALARM_MODE_UNSET 	=0x00,					//����״̬
	ALARM_MODE_SET 		=0x01,					//����״̬
	ALARM_MODE_PARTSET	=0x02,					//�ַ�״̬
	ALARM_MODE_NONE		=0x03,					//δ֪
}ALARM_MODE_E;

typedef enum
{
	CALL_STATE_NONE,		  					// ��
	CALL_STATE_CALLING,							// ������
	CALL_STATE_RECORDHINT,						// ����ģʽ��ʾ��
	CALL_STATE_RECORDING,						// ����ģʽ¼��
	CALL_STATE_TALK,							// ͨ��״̬
	CALL_STATE_END,								// ͨ������
	CALL_STATE_MOVETEMP							// ��ʱת����
}CALL_STATE_E;

#define CALL_TIMER				(CALL_STATE_END+10) 	// �Խ���ʱ
#define CALL_STATE_REQUEST		(CALL_STATE_END+11) 	// �Խ�����
#define CALL_NEW_CALLING		(CALL_STATE_END+12) 	// �Խ��º���
#define CALL_FORMSHOW_CALLING	(CALL_STATE_END+13) 	// �Խ�������ʾ��������
#define CALL_SNAP_CALLBACK		(CALL_STATE_END+14) 	// ץ��ͼƬ�ص�


// �ҵ�����
typedef enum
{
	EHOME_TYPE_LIGHT			=0x00,			//��
	EHOME_TYPE_AIRCONDITION		=0x01,			//�յ�
	EHOME_TYPE_CURTAIN			=0x02,			//����
	EHOME_TYPE_POWER			=0x03,			//��Դ
	EHOME_TYPE_GAS				=0x04, 			//ú��
	EHOME_TYPE_OTHER			=0x05			//�����豸
}EHOME_TYPE_E;

// �ҵ��龰ģʽ
typedef enum
{
	JD_SCENE_RECEPTION 		= 0x00,				// ���ģʽ
	JD_SCENE_MEALS,								// �Ͳ�ģʽ
	JD_SCENE_NIGHT,								// ҹ��ģʽ
	JD_SCENE_POWERSAVING,						// �ڵ�ģʽ
	JD_SCENE_GENERAL,							// ��ͨģʽ
	JD_SCENE_MAX,
	JD_SCENE_NONE,
}AU_JD_SCENE_MODE;								// �ҵ��龰ģʽ

/***************************����Ϊö�����Ͷ���***********************/



/********************����Ϊ����������صĽṹ���Ͷ���************************/

// ��ʼ�������ݰ�, ʹ���ֽڶ��뷽ʽ
#pragma pack(push,1)  

#ifdef _AU_PROTOCOL_
typedef struct
{
	uint32		DeviceNo1;							// �豸�Ÿ�λ��������999999999
	uint32		DeviceNo2;							// �豸�ŵ�λ��������999999999
}DEVICE_NO, *PDEVICE_NO;
#else
// �豸��ţ����ţ�
typedef struct
{
	uint8 		DeviceType;                         // �豸���� 
	uint8 		Resver;								// Ԥ��λ
	uint16 		AreaNo;								// С�����
	uint32		DeviceNo1;							// �豸�Ÿ�λ��������999999999
	uint32		DeviceNo2;							// �豸�ŵ�λ��������999999999
}DEVICE_NO, *PDEVICE_NO;
#endif

// �豸��Ź���, ����ڴ洢ͷ�ļ��ж���
typedef struct
{
	uint8		StairNoLen;							// �ݺų��ȣ�Ĭ��4
	uint8		RoomNoLen;							// ���ų��ȣ�Ĭ��4
	uint8		CellNoLen;							// ��Ԫ�ų��ȣ�Ĭ��2
	uint8		UseCellNo;							// ������Ԫ��־��0��false 1��true�� Ĭ��1
	uint32		Subsection;							// �ֶβ�����0Ϊ���ֶΣ�Ĭ��224
}DEVICENO_RULE;

// ��������ͷ ML80
typedef struct
{
	uint8 		subProtocolType;                   	// ��Э������
	uint8 		DirectFlag;							// �������־
	uint16 		mainProtocolType;					// ��Э��汾
	uint8 		hwEncrypt[16];						// Ӳ���豸��������
	uint32 		mainSeq;							// ���ݰ����к�
	uint8 		sysChipType;						// ϵͳоƬ����������
	uint8 		subProtocolPackType;				// ��Э��������
	uint8 		subProtocolEncrypt;					// ��Э���������
	uint8 		EchoValue;							// Ӧ����
	uint16	 	mainPacketType;						// ��Э�������
	uint16 		subPacketLen;                       // ��Э�������
} MAIN_NET_HEAD, *PMAIN_NET_HEAD;

#ifdef _AU_PROTOCOL_
// �����ͷ
typedef struct
{
	uint16		PackageID;							// ����ʶ
	uint16		ProtocolVer;						// Э��汾
	uint32		AreaCode;							// С�����
	DEVICE_NO	DeviceNo;							// �豸���
	uint8		DeviceType;							// �豸����
	uint8		DirectFlag;							// �����־��������0xAA��Ӧ��0x00��
	uint8		SubSysCode;							// ��ϵͳ����
	uint8		command;							// ����ֵ
	uint8		EchoValue;							// Ӧ����
	uint8		Encrypt;							// ��������
	uint16		DataLen;							// ���ݳ���
}NET_HEAD, *PNET_HEAD;
#else
typedef struct
{ 
	uint32		PackageID;                          // ����ʶ
	uint16 		ProtocolVer;						// Э��汾
	uint16 		Priry;								// �����ȼ�
	DEVICE_NO 	SrcDeviceNo;						// Դ�豸���
	DEVICE_NO 	DestDeviceNo;						// Ŀ���豸���
	//uint8 		Resver;								// ����λ
	uint8 		DeviceType;
	uint8 		DirectFlag;							// �����־
	uint8 		SubSysCode;							// ��ϵͳ����
	uint8		command;							// ����
	uint8		EchoValue;							// Ӧ����
	uint8 		Encrypt;							// ��������
	uint16 		DataLen;							//���ݳ���
}NET_HEAD, *PNET_HEAD;
#endif

// ʱ��ṹ
typedef struct
{
	uint16		year;								// ��
	uint8		month;								// ��
	uint8		day;								// ��
	uint8		week;								// ����
	uint8		hour;								// Сʱ
	uint8		min;								// ��
	uint8		sec;								// ��
}DATE_TIME, *PDATE_TIME;

typedef struct
{
	uint8   encMode;								// ��Ƶ�����ʽ 0Ϊ��������
	uint8   frameRate;								// ��Ƶ֡��     0Ϊ��������
	uint8   imgSize;								// ��Ƶ�ֱ���   0Ϊ��������
	uint8   resver0;								// ����λ
	uint32  bitRate;								// ��Ƶ���� ��λ(kbps) 0��������
	uint32  resver1;								// ����λ
	uint32  resver2;								// ����λ
} VIDEO_SDP_PARAM, * PVIDEO_SDP_PARAM;

typedef struct
{
	uint8 encMode;									// ��Ƶ��ʽ 0Ϊ��������
	uint8 resver0;
	uint16 resver1;
	uint32 resver2;
}AUDIO_SDP_PARAM, * PAUDIO_SDP_PARAM;

// ���Ž��ո�ʽ
typedef struct
{
	DATE_TIME	SendTime;							// ����ʱ��
	uint8		level;								// ���� 0-һ��, 1-����, 2���ؼ�
	char		SendNo[20];							// ���ͺ���
	char		Content[200];						// ����, һ����Ź涨Ӣ��160���ַ�������70��
}REC_SHORT_MSG;

// ���ŷ��͸�ʽ
typedef struct
{
	uint8		level;								// ���� 0-һ��, 1-����, 2���ؼ�
	char		ReceiveNo[20];						// ���պ���
	char		Content[200];						// ����, һ����Ź涨Ӣ��160���ַ�������70��
}SEND_SHORT_MSG;

// ������Ϣͷ
typedef struct
{
	uint32		ID;									// ��ϢID�� 
	uint8		BulletinType;						// ���� 0-����  1��ͼƬ 2������ 3������.���� 4��ͼƬ.����
	char		title[40];							// ����
	uint32		CharLen;							// ���ֲ��ֳ���
	uint32		ImageLen;							// ͼƬ���ֳ���
	uint32		VoiceLen;							// �������ֳ���
}REC_BULLETIN_HEAD;

#pragma pack(pop)  									// �����������ݰ�, �ָ�ԭ���뷽ʽ
/********************����Ϊ����������صĽṹ���Ͷ���************************/

/***************************����Ϊ����ʹ�����������**************************/
// ���緢�Ͱ�
typedef struct SEND_PACKET_NODE
{
	struct SEND_PACKET_NODE * next;
	struct SEND_PACKET_NODE * prev;
	char SendState;									// ����״̬, ��TSendState
	char SendTimes; 								// ��ǰ���ʹ���
	char SendTimesMax; 								// ��෢�ʹ���
	int TimeOut;									// ���ͳ�ʱʱ��, ��
	int WaitCount; 									// �ȴ�ʱ�����
	uint32 address;									// �Է�IP��ַ
	uint16 port;									// �Է��˿ں�
	int size;										// ���ݰ���С
	char * data;									// ��������
}SEND_PACKET, *PSEND_PACKET;

// ������հ�
typedef struct RECIVE_PACKET_NODE
{
	struct RECIVE_PACKET_NODE * next;
	uint32 address;									// �Է�IP��ַ
	uint16 port;									// �Է��˿ں�
	int size;										// ���ݰ���С
	char * data;									// ��������
}RECIVE_PACKET, *PRECIVE_PACKET;
/***************************����Ϊ����ʹ�����������**************************/

#ifdef __cplusplus
}
#endif
#endif 

