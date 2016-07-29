/*************************************************
  Copyright (C), 2006-2016, Aurine
  File name: 	storage_comm.c
  Author:    	luofl
  Version:   	2.0
  Date: 		2014-11-03
  Description:  �洢ģ�鹫���ӿ�
  				�洢ģ��Ҫ�õ��Ĺ����ӿڶ�����ʵ��
  History:        
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#ifndef __STORAGE_COMM_H__
#define __STORAGE_COMM_H__
#include "storage_types.h"

extern uint8 g_China;
extern uint8 g_Big5;

//void set_language(LANGUAGE_E language);
//LANGUAGE_E get_language(void);

typedef enum
{
    FLAG_CALLIN = 0x00,
	FLAG_CALLOUT,
	FLAG_CALLMISS,
	FLAG_AFCZ,
	FLAG_AFBJ,
	FLAG_COUNT
}FLAG_STORAGE;

typedef enum
{
    ECHO_STORAGE_OK = 0x00,
	ECHO_STORAGE_ERR,
	ECHO_STORAGE_NOFILE,
	ECHO_STORAGE_FULL,
	ECHO_STORAGE_SAME_ADDR,
	ECHO_STORAGE_SAME_NAME,
	ECH0_STORAGE_ERR_ADDR
}ECHO_STORAGE;

#define MSG_SYS_START  			0X0000
#define MSG_APP_START 			0X8000

typedef enum
{
    MS_ALVI_UnloadProcReq = MSG_SYS_START,
    MS_VIVI_ReleaseTaskSemInd , 					// ��ͬ���ȼ�����֮��Ŀ�����Ϣ

	// testMsg
    MS_GUAL_HelloInd,
    MS_ALGU_HelloResp,
    MS_ETGU_ActiveETReq,
    MS_GUGU_PowerOff,

	// WD
    MS_ISRWD_TickInd = 50,
    MS_WDAL_PingReq,
    MS_WDAL_PingResp,

	// USB
    MS_USB_DISCONNECT = 70,
    MS_USB_CONNECT,
    MS_USB_POLL,

	// timer
    MS_ISRTM_TickInd = 100,


    // gui
    MS_SYGU_SysInitInd = 200,
    MS_SYGU_SetUpInd,
    MS_ISRGU_KeyInd , 
    MS_TMGU_SecondInd,
    MS_TMGU_KeyTimeOutInd,
    MS_GUGU_PostMsgInd,

    MS_ALGU_SysShutDownReq,

    MS_TMGU_TouchInd,
    MS_TMGU_MSInd,
    MS_TMGU_WinTimerInd,
    MS_GUGU_SwitchAppInd, 							// APPLICATION �л�.

    // 080623,haungsl,���Ӹ���Ϣ���ڷ���������,�û����Զ����Լ�����Ҫ�رյ�Ӧ��.
    MS_AUGU_ExittoDesktop,    						// ���κ������,����������

    MS_ISRGU_SdcardInd = 300, 						// msg =1: insert , msg=0:remove 
    MS_ISRGU_USBCableInd , 							// USB �ߣ�1 ���룬 0 �γ�
    MS_TMGU_AlarmInd,

    // GUI TEST MSG .
    MS_TSGU_GuiMbox = 400,
    MS_TSGU_Gui0Mbox,

    // display
    MS_DIGU_WinMoveEndInd = 500, 					// ����������ʾ������msg:��������--MOVEOUT , MOVEIN 
    MS_ISRDI_TickInd , 
    MS_ISRDI_FlushInd,
    MS_DIDI_LCDUpdateInd,
    MS_DIDI_LCDFrameInd,
    MS_GUDI_LCDClearInd,  							// for test lcd 
    MS_GUDI_LCDCSetViewAeraAlpha,
    MS_GUDI_LCDCSetPhyPosiAlpha,

	MS_INTAU_BeepStart,								// BeepSound
	MS_INTAU_Beep,
	MS_INTAU_BeepStop,

    // flushed msg
    MS_GIVEN_Msg_Flushed = 600,      				// for test lcd 

   	/* ����ΪӦ����Ϣ */
    AS_ALL_START = MSG_APP_START,

	// ��Ƶ���ſ�����Ϣ
    AS_AUAU_DecInit,
    AS_AUAU_DecStart,
    AS_GUAU_Playing,
    AS_GUAU_Stop ,
    AS_GUAU_Pause,
    AS_GUAU_Resume,
    AS_AUAU_DecError,
    
    AS_AUAU_TALK_START,	
    AS_AUAU_TALKING_IN,
    AS_AUAU_TALKING_OUT,
    AS_AUAU_TALK_STOP,
    AS_GUAU_Transmit,

	AS_AUAU_LYLY_START,
	AS_AUAU_LYLY_PROCESS,
	AS_AUAU_LYLY_STOP,
	AS_GUAU_LYLYPLAY_START,
	AS_GUAU_LYLYPLAY_STOP,

	AS_AUAU_HINT_START,
	AS_AUAU_HINT_STOP,

	// add by luofl 2013-12-25 ������ݻ�����Ƶ��˸����
	AS_AUAU_H264_SEND,

	AS_GUAU_LEAVE_START,
	AS_GUAU_LEAVE_WRITE,
	AS_GUAU_LEAVE_STOP,

	// ¼��
	AS_GUAU_TSStopRecReq = (MSG_APP_START + 200),
	AS_GUAU_RECORDWRITE,          					// ¼��д�ļ�
	AS_GUAU_SAVEDATA,
    AS_GUAU_RECORDCLOSE,
    AS_ISRAU_TSRecDMAIntInd,
    AS_GUAU_RecPrepare,

    // video
    AS_GUVI_VideoInit  = (MSG_APP_START + 300),
    AS_GUVI_VideoStart,
    AS_GUVI_VideoEnd,
    AS_GUVI_VideoFillBuf,
    AS_GUVI_VideoStop,
    AS_GUVI_CLOSE,
    AS_GUVI_DEC,
    AS_VIGU_DEC_FALSE,
    AS_GUVI_ZOOM,
    AS_GUVI_ROTATE,
    AS_GUVI_MOV,
    AS_VIGU_ZOOM_OK,
    AS_VIGU_MOVE_OK,
    AS_GUVI_IMG_RESET,
    AS_GUVI_GIF_NEXT_FRAME,
    AS_GUVI_NINE_SCREEN,
    AS_VIVI_BROWSER_DEC_OK,
    AS_VIVI_BROWSER_DEC_FALSE,
    
    // other
    AS_NET_RECV_UDPPACKET_NOTIFY  = (MSG_APP_START + 400),
	AS_NET_RECV_PACKET_DISTRIBUTE,			
	AS_NET_MULTI_SEND_NOTIFY,
	LWIP_MBOX_POST = (MSG_APP_START+1102),

	AS_CALLIN,
	AS_INTER_REQUEST,
	AS_INTER_CALLING,
	AS_INTER_RECORD_HINT,
	AS_INTER_RECORDING,
	AS_INTER_TALK,
	AS_INTER_END,

	AS_MONITOR_SEARCH,
	AS_MONITOR_REQUEST,
	AS_MONITOR_MONITORING,
	AS_MONITOR_TALK,
	AS_MONITOR_END,

	AS_RTSP_REQUEST,
	AS_RTSP_MONITORING,
	AS_RTSP_END,
	AS_RTSP_HEART,
	AS_RTSP_GETLIST,

	AS_ALARM_DEAL_MSGID,
	AS_ALARM_DAIL,
	AS_ALARM_REDAIL,
	AS_ALARM_PROC,
	AS_NEW_MSG,
	AS_ENTER_YUJING,								// ����Ԥ��(��ʱ��������)��Ϣ

    AS_UART0_REV_DATA,								// ����0���յ�����

    AS_STORAGE_LIGHT,                               // �ҵ�ƹ�洢

	AS_UPDATA_JD_DEV,								// ͬ���ҵ���Ϣ				
	
	LASTMSG											// ����������
} SYS_MSG_ID;

#define INVALID_MSGID   LASTMSG  					// ��Ч��ϢID

/*************************************************
Function:		sync_data
Description:	�����ݴӻ�����д��NOR Flash����
Input:		��
Output:		��
Return:		��
Others:
*************************************************/
void sync_data();

/*************************************************
  Function:		storage_get_ring_out
  Description: 	��ȡ������ȫ��·��
  Input:		
  Output:		��
  Return:		��
  Others:
*************************************************/
char* storage_get_ring_out(void);

/*************************************************
  Function:		storage_get_ring_fenji
  Description: 	��ȡ�ֻ�����ȫ��·��
  Input:		
  Output:		��
  Return:		��
  Others:
*************************************************/
char* storage_get_ring_fenji(void);

/*************************************************
  Function:		storage_get_lyly_tip_path
  Description: 	��ȡLYLY��ʾ��·��
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
char* storage_get_lyly_tip_path(void);

/*************************************************
  Function:		SaveRegInfo
  Description: 	����ע���
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void SaveRegInfo(void);

/*************************************************
  Function:		DelayMs_nops
  Description: 	
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void DelayMs_nops(uint32 count);

/*************************************************
  Function:     is_fileexist
  Description:  �ж�ָ��·���ļ��Ƿ����
  Input:       	
   	1.path		ָ���ļ���·��
  Output:       ��
  Return:
				�ļ�����:TRUE
				�ļ�������:FALSE
  Others:
*************************************************/
int32 is_fileexist(char * path);

/*************************************************
  Function:		get_size
  Description: 	����ļ��д�С
  Input:		
  	1.Filename	�ļ�����
  Output:		��
  Return:		SIZE
  Others:
*************************************************/
int get_size (char* srcname);

/*************************************************
  Function:    		FSFileDelete
  Description: 		ɾ��ָ�����ļ�
  Input: 
  	1.path			�ļ�·����
  Output:     		��
  Return:
					�ɹ�0:ʧ�ܷ��ش����
					�������  EROFS �ļ�������ֻ���ļ�ϵͳ��
					EFAULT ����pathnameָ�볬���ɴ�ȡ�ڴ�ռ�
					ENAMETOOLONG ����pathname̫��
					ENOMEM �����ڴ治��
					ELOOP ����pathname �й��������������
					EIO I/O ��ȡ����
  others:
*************************************************/
int32 FSFileDelete(char * path);

/*************************************************
  Function:		Fwrite_common
  Description: 	д����
  Input:		
  	1.Filename	��¼����
  	2.Data		�豸����
  	3.DataSize	���ݽṹ��С
  	4.DataNum	���ݸ���
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
ECHO_STORAGE Fwrite_common(char* Filename, void* Data, int DataSize, int DataNum);

/*************************************************
  Function:		Fread_common
  Description: 	������
  Input:		
  	1.Filename	��¼����
  	2.Data		�豸����
  	3.DataSize	���ݽṹ��С
  	4.DataMaxNum ���ݸ���
  Output:		��
  Return:		���ݸ���
  Others:
*************************************************/
uint32 Fread_common (char* Filename, void* Data, int DataSize, int DataMaxNum);

/*************************************************
  Function:		storage_set_save_flag
  Description: 	���ô洢��־
  Input:		
  	1.Flag		����
  	2.state		״̬(TURE -- ��ʱ�洢 FALSE -- ����洢)
  Output:		��
  Return:		��
  Others:
*************************************************/
void storage_set_save_flag (FLAG_STORAGE Flag, uint8 state);

/*************************************************
  Function:		storage_get_save_flag
  Description: 	��ȡ�洢��־
  Input:		
  	1.Flag		����
  Output:		��
  Return:		״̬
  Others:
*************************************************/
uint8 storage_get_save_flag(FLAG_STORAGE Flag);

/*************************************************
  Function:		storage_comm_model_save
  Description: 	������ʱ�洢
  Input:		��
  Output:		��
  Return:		��
  Others:		����ģ��Ķ�ʱ�洢������
*************************************************/
void storage_comm_model_save(void);

/*************************************************
  Function:		storage_set_isOpenScreen
  Description: 	���ÿ�����������
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void storage_set_isOpenScreen(uint8 state);

/*************************************************
  Function:		storage_get_isOpenScreen
  Description: 	���ÿ�����������
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
uint8 storage_get_isOpenScreen(void);

/*************************************************
  Function:		storage_get_language
  Description: 	��ȡ����
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
LANGUAGE_E storage_get_language(void);

/*************************************************
  Function:		storage_set_language
  Description: 	��������
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void storage_set_language(LANGUAGE_E language);

/*************************************************
  Function:		storage_get_extmode
  Description: 	��ȡ�ⲿģ��
  Input:		��
  Output:		��
  Return:		TRUE ���� FALSE ������
  Others:
*************************************************/
uint32 storage_get_extmode(EXT_MODE_TYPE type);

/*************************************************
  Function:		storage_set_extmode
  Description: 	�����ⲿģ��
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
uint32 storage_set_extmode(uint8 save, EXT_MODE_TYPE type, uint8 state);

/*************************************************
  Function:		storage_get_bright
  Description: 	��ȡ����
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
uint32 storage_get_bright(void);

/*************************************************
  Function:		storage_set_bright
  Description: 	���ñ���
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
uint32 storage_set_bright(uint8 save, uint8 Bright);

/*************************************************
  Function:		storage_get_desk
  Description: 	��ȡ����ID
  Input:		��
  Output:		��
  Return:		����ID
  Others:
*************************************************/
uint32 storage_get_desk(void);

/*************************************************
  Function:		storage_set_desk
  Description: 	��������ID
  Input:		��
  Output:		��
  Return:		��ȷ���
  Others:
*************************************************/
uint32 storage_set_desk(uint8 save, uint8 ID);

/*************************************************
  Function:		storage_get_mac
  Description: 	��ȡMAC
  Input:		
  	1.type		MAC����
  Output:		��
  Return:		����
  Others:
*************************************************/
uint8 * storage_get_mac(MAC_TYPE type);

/*************************************************
  Function:		storage_set_mac
  Description: 	����MAC
  Input:		
  	1.type		MAC����
  	2.data		MAC��
  Output:		��
  Return:		TRUE��FALSE
  Others:
*************************************************/
uint32 storage_set_mac(MAC_TYPE type, char * data);

/*********************************************************/
/*					 	��������						 */	
/*********************************************************/
/*************************************************
  Function:		storage_get_pass
  Description: 	��ȡ����
  Input:		
  	1.type		��������
  Output:		��
  Return:		����
  Others:
*************************************************/
uint8 * storage_get_pass(PASS_TYPE type);

/*************************************************
  Function:		storage_get_pass
  Description: 	��ȡ����
  Input:		
  	1.type		��������
  	2.data		����
  Output:		��
  Return:		����
  Others:
*************************************************/
uint32 storage_set_pass(PASS_TYPE type, char* data);

/*************************************************
  Function:		storage_get_doorserver
  Description: 	��ȡ��������������Ч
  Input:		��
  Output:		��
  Return:		
  Others:
*************************************************/
uint8 storage_get_doorserver(uint8 pass_type);

/*************************************************
  Function:		storage_set_doorserver
  Description: 	���ü�������������Ч
  Input:		
  	1.enable
  Output:		��
  Return:		
  Others:
*************************************************/
uint32 storage_set_doorserver(uint8 enable, uint8 pass_type);

/*********************************************************/
/*					 	��������						 */	
/*********************************************************/
/*************************************************
  Function:		storage_get_ringvolume
  Description: 	��ȡ������С
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
uint8 storage_get_ringvolume(void);

/*************************************************
  Function:		storage_get_talkvolume
  Description: 	��ȡͨ����С
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
uint8 storage_get_talkvolume(void);

/*************************************************
  Function:		storage_get_keykeep
  Description: 	��ȡ������
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
uint8 storage_get_keykeep(void);

/*************************************************
  Function:		storage_set_volume
  Description: 	��ȡ������С
  Input:		
  	1.ring
  	2.talk
  	3.key
  Output:		��
  Return:		��
  Others:
*************************************************/
uint32 storage_set_volume(uint8 ring, uint8 talk, uint8 key);

/*************************************************
  Function:			stroage_sdp_param
  Description:		��ʼ����Ƶ�������
  Input: 	
  Output:			��
  Return:			�ɹ����, TRUE/FALSE
  Others:	
*************************************************/
void stroage_sdp_param_init(void);

/*************************************************
  Function:		storage_get_noface
  Description: 	�����״̬
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
uint8 storage_get_noface(void);

/*************************************************
  Function:		storage_get_ring_by_devtype
  Description: 	��ȡ����ȫ��·��
  Input:		
  	1.type		�豸����
  Output:		��
  Return:		��
  Others:
*************************************************/
char* storage_get_ring_by_devtype(DEVICE_TYPE_E type);

/*************************************************
  Function:		storage_get_msg_hit
  Description: 	��ȡ��Ϣ��ʾ��ȫ��·��
  Input:		
  	1.type		�豸����
  Output:		��
  Return:		��
  Others:
*************************************************/
char* storage_get_msg_hit(void);

/*************************************************
  Function:		storage_get_lyly_enable
  Description: 	��ȡLYLYʹ��
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
uint8 storage_get_lyly_enable(void);

/*************************************************
  Function:		storage_get_lyly_mode
  Description: 	��ȡLYLYģʽ
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
LYLY_TYPE storage_get_lyly_mode(void);

/*************************************************
  Function:		storage_get_lyly_Linkmode
  Description: 	��ȡLYLY����״̬
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
uint8 storage_get_lyly_Linkmode(LYLY_LINK_TYPE type);

/*************************************************
  Function:		storage_get_lyly_tip
  Description: 	��ȡLYLY��ʾ��״̬
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
uint8 storage_get_lyly_tip(void);

/*************************************************
  Function:		storage_set_lyly_tip
  Description: 	����LYLY��ʾ��״̬
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
uint32 storage_set_lyly_tip(uint8 tip);

/*************************************************
  Function:		storage_get_screenparam
  Description: 	�����Ļ��������
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
PEPHOTO_PARAM storage_get_screenparam(void);

/*************************************************
  Function:		storage_set_screenparam
  Description: 	������Ļ��������
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
uint8 storage_set_screenparam(PEPHOTO_PARAM param);

/*************************************************
  Function:		storage_init
  Description: 	�洢��ʼ��
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void storage_init(void);

/*************************************************
  Function:		storage_set_noface_enable
  Description: 	���������״̬
  Input:		
  	1.enable
  Output:		��
  Return:		��
  Others:
*************************************************/
uint8 storage_set_noface_enable(uint8 enable);

/*************************************************
  Function:		storage_get_closelcd_time
  Description: 	����Ļʱ��
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
uint32 storage_get_closelcd_time(void);

/*************************************************
  Function:		storage_get_lyly_param
  Description: 	��ȡLYLYʹ��
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
PLYLY_PARAM storage_get_lyly_param(void);

/*************************************************
  Function:		storage_get_lyly_param
  Description: 	����LYLYʹ��
  Input:		
  	1.param		�ṹ
  Output:		��
  Return:		��
  Others:
*************************************************/
uint32 storage_set_lyly_param(PLYLY_PARAM param);

/*************************************************
  Function:		storage_get_ring_id
  Description: 	��ȡ����ID
  Input:		
  	1.index		(�ݿ� ���� �ſ� ���� ס�� �ֻ� ��Ϣ Ԥ��)
  Output:		��
  Return:		��
  Others:
*************************************************/
uint8 storage_get_ring_id(uint8 index);

/*************************************************
  Function:		storage_get_ring_id
  Description: 	��ȡ����ID
  Input:		
  	1.index		(�ݿ� ���� �ſ� ���� ס�� �ֻ� ��Ϣ Ԥ��)
  Output:		��
  Return:		��
  Others:
*************************************************/
uint32 storage_set_ring_id(uint8 index, uint8 id);

/*************************************************
  Function:		storage_get_msg_hit_byID
  Description: 	��ȡ��Ϣ��ʾ��ȫ��·��
  Input:		
  	1.type		�豸����
  Output:		��
  Return:		��
  Others:
*************************************************/
char* storage_get_msg_hit_byID(uint8 id);

/*************************************************
  Function:		storage_get_yj_path_byID
  Description: 	��ȡԤ����ȫ��·��
  Input:		
  	1.type		�豸����
  Output:		��
  Return:		��
  Others:
*************************************************/
char* storage_get_yj_path_byID(uint8 id);

/*************************************************
  Function:		storage_get_ring_by_id
  Description: 	��ȡ����ȫ��·��
  Input:		
  	1.id		
  Output:		��
  Return:		��
  Others:
*************************************************/
char* storage_get_ring_by_id(uint8 id);

/*********************************************************/
/*						��������						 */	
/*********************************************************/

/*************************************************
  Function:		storage_get_netparam
  Description: 	��ȡIP��Ϣ�ṹ
  Input:		
  Output:		��
  Return:		�������
  Others:
*************************************************/
PNET_PARAM storage_get_netparam(void);

/*************************************************
  Function:		storage_get_netparam_bytype
  Description: 	��ȡIP��Ϣ
  Input:		
  	1.type		Ҫ��ȡ��IP����
  Output:		��
  Return:		��
  Others:
*************************************************/
uint32 storage_get_netparam_bytype(IP_TYPE type);

/*************************************************
  Function:		storage_set_netparam
  Description: 	����IP��Ϣ
  Input:			
  	1.save		�Ƿ�Ҫ����
  	1.type		Ҫ���õ�IP����
  	2.IP		IP��ַ
  Output:		��
  Return:		��
  Others:
*************************************************/
uint32 storage_set_netparam(uint8 save, IP_TYPE type, uint32 IP);

/*************************************************
  Function:    		storage_get_local_ip
  Description:		���IP��ַ
  Input: 			��
  Output:			��
  Return:			�ɹ����, TRUE/FALSE
  Others:
*************************************************/
uint32 storage_get_local_ip(void);

/*************************************************
  Function:    		storage_get_center_ip
  Description:		������ķ�����IP
  Input: 			��
  Output:			��
  Return:			���ķ�����IP��ַ
  Others:
*************************************************/
uint32 storage_get_center_ip(void);

/*************************************************
  Function:    		storage_get_manager_ip
  Description:		��ù���Ա��IP
  Input: 			
  	1.NO			����Ա����� 1-3
  Output:			��
  Return:			��Ա��IP
  Others:
*************************************************/
uint32 storage_get_manager_ip(uint8 NO);

/*************************************************
  Function:    		storage_set_subdev_ip
  Description:		���÷ֻ��豸IP
  Input: 			
  Output:			��
  Return:			��Ա��IP
  Others:
*************************************************/
uint32 storage_clear_subdev_ip(void);

/*************************************************
  Function:    		storage_set_subdev_ip
  Description:		���÷ֻ��豸IP
  Input: 			
  Output:			��
  Return:			��Ա��IP
  Others:
*************************************************/
uint32 storage_set_subdev_ip(uint8 index, uint32 ip);

/*************************************************
  Function:    		storage_get_subdev_ip
  Description:		��ȡ�ֻ��豸IP
  Input: 			
  Output:			��
  Return:			
  Others:			�ֻ��豸���˸�
*************************************************/
uint32 storage_get_subdev_ip(uint8 index);

/*************************************************
  Function:    		storage_get_aurine_ip
  Description:		��ù��ַ�����IP
  Input: 			��
  Output:			��
  Return:			����IP
  Others:
*************************************************/
uint32 storage_get_aurine_ip(void);
#ifdef _USE_NEW_CENTER_

/*************************************************
  Function:			storage_set_predevno
  Description:		����ע��ɹ��豸���
  Input: 	
  Output:			��
  Return:			�ɹ����, TRUE/FALSE
  Others:	
*************************************************/
DEVICE_NO storage_get_predevno(void);

/*************************************************
  Function:			storage_set_predevno
  Description:		����ע��ɹ��豸���
  Input: 	
  Output:			��
  Return:			�ɹ����, TRUE/FALSE
  Others:	
*************************************************/
int storage_set_predevno(PDEVICE_NO DeviceNo);

#endif

#endif

