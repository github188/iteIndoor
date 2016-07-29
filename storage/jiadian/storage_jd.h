/*********************************************************
  Copyright (C), 2009-2012
  File name:	storage_jd.h
  Author:   	������
  Version:   	1.0
  Date: 		10.08.31
  Description:  �洢ģ��--�ҵ�
  History:                   
    1. Date:
       Author:
       Modification:
    2. ...
*********************************************************/
#ifndef __STORAGE_JD_H__
#define __STORAGE_JD_H__

#include "storage_include.h"
//#include "storage_types.h"

#define _JD_GROUP_					0

#define MAX_JD_NAME					31				// �ҵ��豸������󳤶�
#ifdef _AURINE_ELEC_NEW_
#define MAX_JD_LIGHT				64				// ����·
#else
#define MAX_JD_LIGHT				32				// ����·
#endif
#define MAX_JD_WINDOW				16				// ������·
#ifdef _IP_MODULE_//_AURINE_ELEC_RILI_
#define MAX_JD_KONGTIAO				12				// �յ���·
#else
#define MAX_JD_KONGTIAO				8				// �յ���·
#endif
#define MAX_JD_POWER				16				// ��Դ��·
#define MAX_JD_GAS					1				// ú����·
#define MAX_JD_DEVICE_NUM			(MAX_JD_LIGHT+MAX_JD_WINDOW+MAX_JD_KONGTIAO+MAX_JD_POWER+MAX_JD_GAS)
#define MAX_JD_SCENE_NUM            32

#define SCENE_CMD_CLOSE				0
#define SCENE_CMD_OPEN				1

/*******************************�ҵ��豸��Ϣ********************************/
// �ҵ糧��
typedef enum
{
	JD_FACTORY_X10 = 0x00,							// ��Ȼ�ҵ�ģ��
	JD_FACTORY_ACBUS,								// ���ּҵ�ģ��
	JD_FACTORY_MAX
}JD_FACTORY_TYPE;

typedef enum		
{
	JD_TYPE_QINGJING = 0x00,
	JD_TYPE_DENGGUANG,
	JD_TYPE_CHUANGLIAN,
	JD_TYPE_KONGTIAO,
	JD_TYPE_DIANYUAN,
	JD_TYPE_MEIQI,
}JD_YUYUE_TYPE_E;	

// �ҵ�����
#define JD						0x02				// �ҵ�ģ��485����
#define	JD_ON					0x01				// ��
#define	JD_OFF					0x02				// ��
#define	JD_ADD					0x03				// ����
#define	JD_DEC					0x04				// ����
#define	JD_INQ_STATUS			0x05				// ��Ѱ�ڵ��豸״̬
#define	JD_GET_LOGO_NUM			0x06				// ȡ��Ʒ������
#define JD_DOWNLOAD_LOGO		0x08				// �ҵ�ģ���ϵ�����ѡ�õ�Ʒ�ƺ�
#define	JD_GET_LOGO				0x0A				// ��ȡƷ������(һ�λ�ȡһ��)
#define	JD_GET_VER				0x09				// ��ȡ����汾��
#define	JD_SET_LOGO				0x0B				// �趨ѡ�õ�Ʒ��

typedef enum
{
	JD_SCENE = 0x00,
	JD_LIGHT,
	JD_WINDOW,
	JD_KONGTIAO,
	JD_POWER,
	JD_GAS,
	JD_ALL
}JIADIAN_PAGE;

// �ҵ��豸״̬
typedef struct
{
	uint8 devlight[MAX_JD_LIGHT][2];				// ����״̬�ͼ���
	uint8 devKongtiao[MAX_JD_KONGTIAO][2];			// ����״̬�ͼ���
	uint8 devwindow[MAX_JD_WINDOW];
	uint8 devpower[MAX_JD_POWER];
	uint8 devgas[MAX_JD_GAS];
}JD_STATE_INFO, * PJD_STATE_INFO;

/* �ҵ��豸���� */
typedef enum
{
	JD_DEV_LIGHT 			= 0,					// �ƹ��豸
	JD_DEV_WINDOW 			= 1,					// �����豸
	JD_DEV_KONGTIAO 		= 2,					// �յ��豸
	JD_DEV_POWER 			= 3,					// ��Դ�豸
	JD_DEV_GAS 				= 4,					// ú���豸
	JD_DEV_MAX
}AU_JD_DEV_TYPE;



/* �ҵ��豸��Ϣ�ṹ */
typedef struct
{
	AU_JD_DEV_TYPE 	JdType;							// �豸����
	uint8			Index;							// �豸������ ��0��ʼ
	uint16			Address;						// �豸��ַ(bit7-15:��;bit0-7:��)
	uint32 			TextIDPos;						// λ������
	uint32 			TextIDName;						// �豸��������
	uint32  		TextID;							// �豸����id
	char			Name[MAX_JD_NAME];				// ����
	uint8			IsUsed;							// �Ƿ����� 1:���ã�0:δ��
	uint8 			IsTune;							// �Ƿ�ɵ��ڣ����ڵƹ�Ϊ���Ƿ�ɵ��⣬���ڿյ��Ƿ�ɵ���
	uint8			IsGroup;						// �Ƿ����
	uint8 			Commond[JD_SCENE_MAX];			// �龰����	
	uint8 			param[JD_SCENE_MAX][7];			// �龰����ֵ	
//	uint8			IsSigOper;						// �Ƿ񵥿�
}JD_DEV_INFO, *PJD_DEV_INFO;

/* �ҵ��豸�б���Ϣ */
typedef struct
{
	int32 nCount;
	PJD_DEV_INFO pjd_dev_info;
}JD_DEV_LIST_INFO,*PJD_DEV_LIST_INFO;

/* �ҵ��豸�б���Ϣ */
typedef struct
{
	uint16 light_addr[MAX_JD_LIGHT];
	uint16 window_addr[MAX_JD_WINDOW];
	uint16 kongtiao_addr[MAX_JD_KONGTIAO];
	uint16 power_addr[MAX_JD_POWER];
	uint16 gas_addr[MAX_JD_GAS];
	uint16 sence_addr[MAX_JD_SCENE_NUM];
}JD_ADDR_INFO,*PJD_ADDR_INFO;

// �龰ģʽ��Ϣ
typedef struct 
{
	AU_JD_SCENE_MODE SceneIndex;
	uint8 IsUsed;
	char SceneName[MAX_JD_NAME];
	#ifdef _AURINE_ELEC_NEW_
	uint32 	  TextIDPos;		// λ������
	uint32    TextIDName;		// �豸��������
	uint8     SceneID;
	uint16    Address;			// �豸��ַ(bit7-15:��;bit0-7:��)	
	#endif
}JD_SCENE_INFO, *PJD_SCENE_INFO;

// �龰ģʽ��Ϣ����
typedef struct
{
	int32 nCount;
	PJD_SCENE_INFO pjd_scene_info;
}JD_SCENE_INFO_LIST, *PJD_SCENE_INFO_LIST;

#ifdef _AURINE_ELEC_NEW_
typedef struct
{
	uint16	Address;			// �豸��ַ(bit7-15:��;bit0-7:��)	
	uint8	IsUsed;				// �Ƿ����� 1:���ã�0:δ��
	char	Name[30];			// �豸����
}AU_JD_SCENE_PARAM,*PAU_JD_SCENE_PARAM;
#endif

typedef struct
{
	unsigned char Index;
	unsigned char Addr;
	unsigned char State;
	unsigned char IsUsed;	
}AU_JDDEV_STATE, *PAU_JDDEV_STATE;

typedef struct
{
	uint32 Count;
	PAU_JDDEV_STATE JdDev;	
}AU_JDDEV_LIST, *PAU_JDDEV_LIST;


#ifdef _JD_MODE_
/*******************************�ҵ��豸��Ϣ********************************/

/*************************************************
  Function:		storage_get_scene
  Description: 	�������龰�����мҵ��豸��Ϣ
  Input:		
  	1.mode
  Output:		��
  Return:		��
  Others:		
  	1.��ָ���ⲿ�ͷ�
*************************************************/
PJD_DEV_LIST_INFO storage_get_scene(void);

/*************************************************
  Function:		free_jd_memory
  Description: 	�ͷżҵ�洢�ڴ�
  Input:		
  	1.JDList
  Output:		��
  Return:		��
  Others:
*************************************************/
void free_jd_memory(PJD_DEV_LIST_INFO* JDList);

/*************************************************
  Function:		malloc_jd_memory
  Description: 	����ҵ�洢�ڴ�
  Input:
  	1.JDList
  	2.MaxNum	����¼��
  Output:		��
  Return:		��
  Others:
*************************************************/
void malloc_jd_memory(PJD_DEV_LIST_INFO *JDList,uint32 MaxNum);

/*************************************************
  Function:		storage_get_jddev
  Description: 	����豸��Ϣ
  Input:		
  	1.dev
  	2.UsedOnly	ֻ��ȡ���õ��豸
  Output:		��
  Return:		��
  Others:		
  	1.��ָ���ⲿ�ͷ�
*************************************************/
PJD_DEV_LIST_INFO storage_get_jddev (AU_JD_DEV_TYPE dev, uint8 UsedOnly);

/*************************************************
  Function:		storage_add_jddev
  Description: 	���ͨ����¼
  Input:		
  	1.Calltype	��¼����
  	2.Type		�豸����
  	3.devno		�豸����
  	3.Time		ʱ��
  	4.LylyFlag	ͼƬ״̬
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
ECHO_STORAGE storage_add_jddev (PJD_DEV_INFO info);

/*************************************************
  Function:		storage_del_devinfo
  Description: 	ɾ��
  Input:		
  	1.Calltype	��¼����
  	2.Index		����
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
ECHO_STORAGE storage_del_devinfo (AU_JD_DEV_TYPE dev, uint8 Index);

/*************************************************
  Function:		storage_clear_devinfo
  Description: 	���
  Input:		
  	1.Calltype	��¼����
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
ECHO_STORAGE storage_clear_devinfo(AU_JD_DEV_TYPE dev);

/*************************************************
  Function:		storage_clear_all_callrecord
  Description:  ���
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void storage_clear_all_devinfo(void);

/*************************************************
  Function:		get_dev_max
  Description: 	��üҵ��ܸ���
  Input:		
  	1.JDList
  Output:		��
  Return:		��
  Others:
*************************************************/
uint16 get_dev_max(AU_JD_DEV_TYPE dev);

/*************************************************
  Function:		free_scene_memory
  Description: 	�ͷ��龰�洢�ڴ�
  Input:		
  	1.JDList
  Output:		��
  Return:		��
  Others:
*************************************************/
void free_scene_memory(PJD_SCENE_INFO_LIST *pSceneList);

#ifdef _AURINE_ELEC_NEW_
/*************************************************
  Function:		storage_get_aurine_light_value
  Description:	��õ�����״̬
  Input: 		
  	2.addr		�豸�ڵ��ַ,=0xFF:��㲥��ѯ�������Ե��ѯ
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void storage_get_aurine_light_value(uint8 *value_list);

/*************************************************
  Function:		jd_aurine_read_lightvalue
  Description:	���ļ��ж�ȡ�ƹ��豸��һ������
  Input: 		
  	2.addr		�豸�ڵ��ַ,=0xFF:��㲥��ѯ�������Ե��ѯ
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_read_lightvalue(void);

/*************************************************
  Function:		storage_save_aurine_light_value
  Description:	����ƹ��豸��һ������
  Input: 		
  	2.addr		�豸�ڵ��ַ,=0xFF:��㲥��ѯ�������Ե��ѯ
  Output:		��
  Return:		��
  Others:		��
*************************************************/
ECHO_STORAGE storage_save_aurine_light_value(uint8 *value_list);

/*************************************************
  Function:		storage_set_scene_info
  Description: 	�龰��Ϣ����
  Input:		
  	1.pSceneList	
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
ECHO_STORAGE storage_set_aurien_scene_info(PJD_SCENE_INFO pSceneList);

/*************************************************
  Function:		storage_get_scene_info
  Description: 	����龰ģʽ��Ϣ����
  Input:		
  Output:		��
  Return:		��
  Others:		
  	1.��ָ���ⲿ�ͷ�
*************************************************/
PJD_SCENE_INFO_LIST storage_get_aurine_scene_info(uint8 UsedOnly);

/*************************************************
  Function:		storage_add_jddev
  Description: 	����豸����
  Input:		
  	1.info	
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
ECHO_STORAGE storage_add_aurine_scenemode(PJD_SCENE_INFO info);

/*************************************************
  Function:		storage_get_aurine_kongtiao_value
  Description: 	���
  Input:		
  	1.Calltype	��¼����
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
void storage_get_aurine_kongtiao_value(uint8 *value_list);

/*************************************************
  Function:		storage_save_aurine_kongtiao_value
  Description: 	���
  Input:		
  	1.Calltype	��¼����
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
ECHO_STORAGE storage_save_aurine_kongtiao_value(uint8 * value_list);
#endif
#endif

/*************************************************
  Function:		storage_jd_init
  Description:  ��ʼ��
  Input:		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void storage_jd_init(void);
#endif

