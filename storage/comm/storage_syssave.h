/*************************************************
  Copyright (C), 2006-2016, Aurine
  File name: 	storage_syssave.h
  Author:    	luofl
  Version:   	2.0
  Date: 		2014-10-02
  Description:  ϵͳ�����洢ͷ�ļ�

  History:        
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#include "storage_types.h"

#define SaveMagicNumber1		0x19700101
#define SaveMagicNumber2		0x20160704

// �ṹ��Ҫ8 BYTE����
typedef struct _SYSCONFIG
{
    int32 Size;
    int32 MagicNumber1;								// �����ַ��������鿴�ṹ�������Ƿ���ȷ

    AF_FLASH_DATA AfFlashData;						// ����ģ��洢�Ĳ���
    SYS_FLASH_DATA SysFlashData;					// ϵͳ����
    VIDEO_SDP_PARAM VideoSdpData;					// ��ƵSDP����
	AUDIO_SDP_PARAM AudioSdpData;					// ��ƵSDP����
    
    int32 MagicNumber2;								// �����ַ��������鿴�ṹ�������Ƿ���ȷ
    //int16 PAD;										// ���8�ֽڶ���
} SYSCONFIG, * PSYSCONFIG;

extern SYSCONFIG gpSysConfig;
extern PAF_FLASH_DATA	gpAfParam;
extern PSYS_FLASH_DATA gpSysParam;
extern VIDEO_SDP_PARAM g_venc_parm;
extern AUDIO_SDP_PARAM g_audio_parm;

/*************************************************
  Function:		check_sysconfig_ifcorrect
  Description: 	У�������Ƿ���ȷ
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void check_sysconfig_ifcorrect(void);

/*************************************************
  Function: 	SaveRegInfo
  Description:	����ע���-ϵͳ���ò���
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void SaveRegInfo(void);

/*************************************************
  Function:    	storage_recover_factory
  Description: 	�ָ���������
  Input: 		��
  Output:		��
  Return:		��	
  Others:
*************************************************/
void storage_recover_factory(void);

/*************************************************
  Function:    	storage_format_system
  Description: 	��ʽ��NAND1-2��
  Input: 		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void storage_format_system(void);

/*************************************************
  Function:		storage_init_sysconfig
  Description: 	��ʼ��ϵͳ����
  Input:		��
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
void storage_init_sysconfig(void);

