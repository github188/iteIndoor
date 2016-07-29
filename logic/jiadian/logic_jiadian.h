/*************************************************
  Copyright (C), 2006-2010, Aurine
  File name:  	logic_jiadian.h
  Author:     	txl
  Version:    	1.0
  Date: 
  Description:  �ҵ��߼�ͷ�ļ�
  
  History:        
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#ifndef _LOGIC_JIADIAN_H_
#define _LOGIC_JIADIAN_H_
#include "logic_include.h"


// �ҵ�״̬
#define JD_STATE_OPEN			1
#define JD_STATE_CLOSE			0

#define JD_STATE_JUNE			3	

#define JD_WD_STATE_STOP        2

// �ƹ���������Сֵ
#define MAX_LIGHT_VAULE			10
#define MIN_LIGHT_VAULE			1

// �յ����������Сֵ
#ifdef _AURINE_ELEC_RILI_
#define MAX_KONGTIAO_VAULE		12
#else
#define MAX_KONGTIAO_VAULE		10
#endif
#define MIN_KONGTIAO_VAULE		1

#if 0
// �ҵ糧��
typedef enum
{
	JD_FACTORY_X10 = 0x00,							// ��Ȼ�ҵ�ģ��
	JD_FACTORY_ACBUS,								// ���ּҵ�ģ��
	JD_FACTORY_MAX
}JD_FACTORY_TYPE;

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
#endif

// ���ݷ���״̬
#define SEND_STATE_READY		0					// ���ݷ���׼��
#define SEND_STATE_WAIT			1					// ���ݷ��͵ȴ�Ӧ��
#define SEND_STATE_OK			2					// ���ݷ���Ӧ�����

// ������󳤶�
#ifdef _AURINE_ELEC_NEW_
#define	MAX_AURINE_JD_DG_NUM 	 MAX_JD_LIGHT  	    // ���ƹ�����
#define	MAX_AURINE_JD_KT_NUM 	 MAX_JD_KONGTIAO	// ���յ�����
#define	MAX_AURINE_JD_CL_NUM 	 MAX_JD_WINDOW		// ���������
#define	MAX_AURINE_JD_CZ_NUM 	 MAX_JD_POWER		// ����������
#define	MAX_AURINE_JD_MQ_NUM 	 MAX_JD_GAS		    // ���ú������
#define MAX_AURINE_JD_SCENE_NUM  MAX_JD_SCENE_NUM	// ����龰ģʽ����
#define PORT_TYPE_LIGHT			 0x01	            // �ƹ�
#define PORT_TYPE_CURTAIN		 0x02	            // ����
#define PORT_TYPE_IR			 0x03	            // ����
#define PORT_TYPE_POWER			 0x31               // ��Դ
#define PORT_TYPE_GAS			 0x32	            // ú��
#define MAX_DATA_LEN			 50

#define	JD_AURINE_SOURE_ADDR		 (0xFF)
#define	JD_AURINE_TUNE_SPEED		 (0)
#define JD_AURINE_TUNE_SPEED_FAST	 (1)
#define JD_AURINE_LIGHT_TUNE_SPEED   (10)
#define	JD_AURINE_LIGHT_LEVEL	     (32)           // �ƹ����ȼ���

#define	JD_AURINE_CONTROL		    0x0C	// ���ƹ��ּҵ�ģ��
#define	JD_LOOP_CONTROL			    0x0D	// ���ƹ��ּҵ���ѯ����
#else
#define MAX_DATA_LEN			16
#endif

// ���ڷ��ͼ��		
// modi by luofl 2011-07-19 �·�����ʱ���Ϊ500����
#define AURINE_COMMTIME_NUM	    4				        // ���������·����ʱ��	ԭ��Ϊ200*5
#define COMMTIME_NUM			4		                // ���������·����ʱ��	ԭ��Ϊ200*5

#define MAX_SEND_NUM			3                           

// ���ݷ��͵ȴ���ʱʱ��			 
#define MAX_SEND_TIME			((500/(5*AURINE_COMMTIME_NUM))+1)    //(���ͳ�ʱʱ��/TICK)

// ��ѯ����ͼ��
#define LOOP_SEND_TIME          (MAX_SEND_TIME-6)   //(��ѯ���ͼ��ʱ��/TICK)

// modi by luofl 2013-04-01 �޸����O�ҵ��·����죬����龰�޷����ơ�
#define MAX_OLD_SEND_TIME       25//(((500*2)/(5*AURINE_COMMTIME_NUM))+1)   // ����ԭ���ҵ�

typedef struct JD_SEND_PACKET_NODE
{
	struct JD_SEND_PACKET_NODE * next;
	struct JD_SEND_PACKET_NODE * prev;
	uint8 SendState;								// ���ݷ���״̬
	uint8 SendTimes;								// ���ݷ��͵ȴ�ʱ��
	uint8 SendMaxTimes;                             // �����ʱ��
	uint8 size;										// ���ݰ���С
	uint8 data[MAX_DATA_LEN];						// ��������
}JD_SEND_PACKET, *PJD_SEND_PACKET;

#if 0
typedef enum		
{
	JD_TYPE_QINGJING = 0x00,
	JD_TYPE_DENGGUANG,
	JD_TYPE_CHUANGLIAN,
	JD_TYPE_KONGTIAO,
	JD_TYPE_DIANYUAN,
	JD_TYPE_MEIQI,
}JD_YUYUE_TYPE_E;	
#endif

typedef enum		
{
    JD_CMD_LOOP = 0x00,			// ��ѯ����
	JD_CMD_COTROL    			// ��������
}JD_CMD_TYPE;

typedef	void (*JD_REPORT_CALLBACK)(uint8 * buf);

#ifdef _JD_MODE_
/*************************************************
  Function:		device_on
  Description: 	�豸��
  Input: 
	1.addr		�豸��ַ
	2.val		ֵ
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void device_on(uint16 addr, uint8 val);

/*************************************************
  Function:		device_off
  Description: 	�豸��
  Input: 
	1.addr		�豸��ַ
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void device_off(uint16 addr);

/*************************************************
  Function:		device_on_all
  Description: 	�豸ȫ��
  Input: 
	1.addr		�豸����
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void device_on_all(AU_JD_DEV_TYPE devtype);

/*************************************************
  Function:		device_off_all
  Description: 	�豸ȫ��
  Input: 
	1.addr		�豸��ַ
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void device_off_all(AU_JD_DEV_TYPE devtype);

/*************************************************
  Function:		device_value_add
  Description: 	�豸ֵ����
  Input: 
	1.addr		�豸��ַ
	2.val		ֵ
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void device_value_add(uint16 addr, char *val);

/*************************************************
  Function:		device_value_dec
  Description: 	�豸ֵ����
  Input: 
	1.addr		�豸��ַ
	2.val		ֵ
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void device_value_dec(uint16 addr, char *val);

/*************************************************
  Function:		exec_jd_scene_mode
  Description: 	ִ�мҵ��龰ģʽ
  Input: 
	1.mode		�龰ģʽ
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void exec_jd_scene_mode(AU_JD_SCENE_MODE mode);

/*************************************************
  Function:		set_device_logo
  Description: 	���õ�ǰʹ���豸�̱�
  Input: 
	1.val		ֵ
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void set_device_logo(uint8 val);

/*************************************************
  Function:		jd_aurine_scene_open
  Description:	���ƹ��ּҵ��龰ģʽ��
  Input: 		
  	1.addr		�豸�ڵ��ַ		
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_scene_open(uint8 scene_id,uint8 group_id, int index);
#endif

//#ifdef _AURINE_REG_
/*************************************************
  Function:			jd_distribute
  Description:		�ҵ���շ��Ͱ��ص�����
  Input: 	
  	1.recPacket		���հ�
  Output:			��
  Return:			�ɹ����,true/fasle
  Others:
*************************************************/
int32 jd_net_distribute(const PRECIVE_PACKET recPacket);

/*************************************************
  Function:			jd_responsion
  Description:		�ҵ�ģ�����Ӧ���������
  Input: 	
  	1.recPacket		Ӧ���
  	2.SendPacket	���Ͱ�
  Output:			��
  Return:			��
  Others:
*************************************************/
void jd_net_responsion(const PRECIVE_PACKET recPacket, const PSEND_PACKET SendPacket);
//#endif

#ifdef _AURINE_ELEC_NEW_

typedef struct
{
	uint8 DevType;      //�豸����
	uint8 DevID;	    //�豸����
	uint8 Active;		//����״̬
	uint8 ActiveParam;	//��������
 }AU_JD_STATE,*PAU_JD_STATE;
 
 typedef struct
{
	uint8 			Index;							// �豸����
	uint16			Address;		                // �豸��ַ(bit7-15:��;bit0-7:��)
	uint8			IsUsed;			                // �Ƿ����� 1:���ã�0:δ��
	uint8		    Ischange;                       // ״̬�Ƿ�ı��־ 0:״̬δ�ı� 1:״̬�ı�	
	AU_JD_STATE     Dev_State;                      // �豸״̬
	uint8 			IsTune;			                // �Ƿ����� ���ȵ���
}AU_JD_STATE_LIST,*PAU_JD_STATE_LIST;

typedef enum
{
	JD_AU_LIGHT_MODE_TUNE		= 1,				// �����л� ����
	JD_AU_LIGHT_MODE_STOP_TUNE	= 4,				// ֹͣ����
	JD_AU_LIGHT_MODE_ADD		= 5,				// ����1������
	JD_AU_LIGHT_MODE_DEC		= 7,				// ����1������
	JD_AU_LIGHT_MODE_OPEN		= 0,				// ���Ƶ�ĳ�����ȼ���
	JD_AU_LIGHT_MODE_CLOSE		= 0x0A,			    // ���洢�Ĺص�
	JD_AU_LIGHT_MODE_OPEN_OLD	= 0x09,			    // ����ԭ�����ȼ���
	JD_AU_LIGHT_MODE_CHANGE     = 0x0E,			    // ������Ŀ����л�
}JD_AU_LIGHT_CTRL_MODE;

typedef enum
{
	JD_AU_IR_MODE_SENDIR		=1,   			    // ����
	JD_AU_IR_MODE_LEARNIR		=2,				    // ѧϰ
	JD_AU_IR_MODE_LEARN_SCENE	=4,				    // �龰ѧϰ
}JD_AU_IR_CTRL_MODE;

typedef enum
{
	JD_AU_IR_REPORT_MODE_LEARN   =2,			    // ѧϰ����
	JD_AU_IR_REPORT_MODE_SCENE  =5,				    // ��ȡ�龰����	
}JD_AU_IR_REPORT_MODE;

typedef enum
{
	JD_AU_WINDOW_MODE_OPEN   =1,			        // ������
	JD_AU_WINDOW_MODE_CLOSE  =2,			        // �ش���
	JD_AU_WINDOW_MODE_STOP   =3,			        // ֹͣ����
}JD_AU_WINDOW_CTRL_MODE;

typedef enum
{
	JD_AU_SCENE_MODE_OPEN   =1,				        // �龰��
	JD_AU_SCENE_MODE_CLOSE   =2,			        // �龰��
	JD_AU_SCENE_MODE_CHANGE  =3,			        // �龰�л�
	JD_AU_SCENE_MODE_SAVE   =4,				        // �����龰
}JD_AU_SCENE_CTRL_MODE;

typedef enum
{
	JD_AU_ADDR_ID				=2,					// IDѰַ��ʽ
	JD_AU_GROUP_ID				=3,					// ��Ѱַ��ʽ
	JD_AU_ADDR_BROADCAST		=9,					// �㲥Ѱַ��ʽ
	JD_AU_PORT_BROADCAST		=0x0a,				// �˿ڹ㲥
}JD_AU_ADDR_MODE;

typedef enum
{
	JD_AU_POWER_MODE_OPEN    = 1,				    // ��Դ��
	JD_AU_POWER_MODE_CLOSE	 = 2,				    // ��Դ��
}JD_AU_POWER_CTRL_MODE;

typedef enum
{
	JD_AU_GAS_MODE_OPEN    = 1,				        // ú����
	JD_AU_GAS_MODE_CLOSE   = 2,				        // ú����
}JD_AU_GAS_CTRL_MODE;

#define		JD_AU_CMD_DW_QUERY_STATE		(0x34)				//��ȡ�豸״̬(�ƹⴰ��)
#define     JD_AU_CMD_DEV_BATCH_QUERY_STATE	(0x3a)				//������ѯ�豸״̬
#define		JD_AU_CMD_UP_STATE_REPORT		(0x35)				//�豸״̬�ϱ�(�ƹⴰ��)
#define		JD_AU_CMD_DW_CTRL_SCENE			(0x40)				//�����龰
#define 	JD_AU_CMD_DW_CTRL_POWER			(0x41)				//���Ƶ�Դ
#define		JD_AU_CMD_DW_CTRL_LIGHT			(0x42)				//���Ƶƹ�
#define 	JD_AU_CMD_DW_CTRL_GAS			(0x43)				//����ú��
#define		JD_AU_CMD_DW_CTRL_WINDOW		(0x44)				//���ƴ���
#define		JD_AU_CMD_DW_CTRL_IR			(0x46)				//���ƺ����豸
#define 	JD_AU_CMD_DW_CTRL_IR_REPORT		(0x47)				//�����豸Ӧ��
#define     JD_AU_CMD_DW_CTRL_ALL           (0x48)				//����ָ�����͵������豸



typedef enum
{
	JD_AURINE_UPDATE_IDLE			=0,
	//���͸������������״̬�ȴ���
	//״̬��Ӧ�󣬻ָ����������ٳ�ʱ�ȴ�5s
	JD_AURINE_UPDATE_WAITING,		
	//�����мҵ������������ʱ�����ô�״̬��־
	// ������ʱ2s��ָ�����
	JD_AURINE_UPDATE_PAUSE	,
	JD_AURINE_UPDATE_RUNNING,
	JD_AURINE_UPDATE_STOP,
	JD_AURINE_UPDATE_SINGLE_DEV,
	
}JD_AURINE_DEV_UPDATE_STATE;


/*************************************************
  Function:		jd_aurine_light_ctrlmode
  Description:	���ƹ��ּҵ�ƹ⿪
  Input: 		
  	2.addr		�豸�ڵ��ַ
  	3.param	���ȼ���:0-100%��0xff: ����ԭ�������ȼ���
  			
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_light_open(uint8 Index, uint16 addr, uint8 param);

/*************************************************
  Function:		jd_aurine_light_ctrlmode
  Description:	���ƹ��ּҵ�ƹ⿪
  Input: 		
  	2.addr		�豸�ڵ��ַ
  	3.param	���ȼ���:0-100%��0xff: ����ԭ�������ȼ���
  			
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_light_open_by_speed(uint8 Index, uint16 addr,uint8 param);

/*************************************************
  Function:		jd_auine_light_close
  Description:	���ƹ��ּҵ�ƹ��
  Input: 		
  	1.addr		�豸�ڵ��ַ		
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_light_close(uint8 Index, uint16 addr);

/*************************************************
  Function:		jd_auine_window_open
  Description:	���ƹ��ּҵ�ƹ��
  Input: 		
  	1.addr		�豸�ڵ��ַ		
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_window_open(uint8 Index, uint16 addr );

/*************************************************
  Function:		jd_auine_light_close
  Description:	���ƹ��ּҵ�ƹ��
  Input: 		
  	1.addr		�豸�ڵ��ַ		
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_window_close(uint8 Index,uint16 addr );

/*************************************************
  Function:		jd_auine_light_close
  Description:	���ƹ��ּҵ�ƹ��
  Input: 		
  	1.addr		�豸�ڵ��ַ		
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_window_stop(uint8 Index,uint16 addr );

/*************************************************
  Function:		jd_aurine_power_open
  Description:	���ƹ��ּҵ�ƹ��
  Input: 		
  	1.addr		�豸�ڵ��ַ		
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_power_open(uint8 index, uint16 addr , uint8 param);

/*************************************************
  Function:		jd_aurine_power_close
  Description:	���ƹ��ּҵ�ƹ��
  Input: 		
  	1.addr		�豸�ڵ��ַ		
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_power_close(uint8 index,uint16 addr );

/*************************************************
  Function:		jd_auine_light_close
  Description:	���ƹ��ּҵ�ƹ��
  Input: 		
  	1.addr		�豸�ڵ��ַ		
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_gas_open(uint8 index,uint16 addr , uint8 param);

/*************************************************
  Function:		jd_aurine_gas_close
  Description:	���ƹ��ּҵ�ƹ��
  Input: 		
  	1.addr		�豸�ڵ��ַ		
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_gas_close(uint8 index,uint16 addr );

/*************************************************
  Function:		jd_aurine_get_dev_state
  Description:	��ȡ���ּҵ�ƹ�״̬
  Input: 		
  	1.inedx		�豸����		
  Output:		��
  Return:		��
  Others:		��
*************************************************/
uint8 jd_aurine_get_dev_state(AU_JD_DEV_TYPE devtype,uint8 index);

/*************************************************
  Function:		jd_aurine_set_light_state
  Description:	������ּҵ�ƹ�״̬
  Input: 		
  	1.inedx		�豸����
  	2.param		����
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_set_dev_state(AU_JD_DEV_TYPE devtype,uint8 index,uint8 param);

/*************************************************
  Function:		jd_aurine_get_light_state
  Description:	��ȡ���ּҵ�ƹ�״̬
  Input: 		
  	1.inedx		�豸����		
  Output:		��
  Return:		��
  Others:		��
*************************************************/
uint8 jd_aurine_get_dev_state_param(AU_JD_DEV_TYPE devtype,uint8 index);

/*************************************************
  Function:		jd_aurine_set_light_state
  Description:	������ּҵ�ƹ�״̬
  Input: 		
  	1.inedx		�豸����
  	2.param		����
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_set_dev_state_param(AU_JD_DEV_TYPE devtype,uint8 index,uint8 param);

/*************************************************
  Function:		jd_aurine_get_dev_address
  Description:	������ּҵ�ƹ�״̬
  Input: 		
  	1.inedx		�豸����
  	2.param		����
  Output:		��
  Return:		��
  Others:		��
*************************************************/
uint16 jd_aurine_get_dev_address(AU_JD_DEV_TYPE devtype,uint8 index);

/*************************************************
  Function:		jd_aurine_send_ir_to_dev
  Description:	��ȡ�ҵ�״̬��Ϣ
  Input: 		
  	1.inedx		�豸����
  	2.param		����
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_send_ir_to_dev(uint8 index, uint8 addr,uint16 param);

/*************************************************
  Function:		jd_aurine_learn_ir
  Description:	��ȡ�ҵ�״̬��Ϣ
  Input: 		
  	1.inedx		�豸����
  	2.param		����
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_learn_ir(uint8 index,uint8 addr,uint16 param);

/*************************************************
  Function:		jd_aurine_learn_ir_scene
  Description:	��ȡ�ҵ�״̬��Ϣ
  Input: 		
  	1.inedx		�豸����
  	2.param		����
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_learn_ir_scene(uint8 addr,uint16 param);

/*************************************************
  Function:		jd_aurine_yuyue_oper
  Description:	���ּҵ�ԤԼ����
  Input: 		
  	1.inedx		�豸����
  	2.param		����
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_yuyue_oper(uint16 cmd,uint8 devtype, uint8 index, uint16 address,uint8 param);

/*************************************************
  Function:		jd_aurine_clean_address_list
  Description:	���ּҵ�ԤԼ����
  Input: 		
  	1.inedx		�豸����
  	2.param		����
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_clean_address_list(void);

/*************************************************
  Function:		jd_aurine_dev_update
  Description:	��ѯ���ּҵ��豸״̬
  Input: 		
  	2.addr		�豸�ڵ��ַ,=0xFF:��㲥��ѯ�������Ե��ѯ
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_dev_update(AU_JD_DEV_TYPE JDType,uint32 Check,uint16 index, uint16 addr);

/*************************************************
  Function:		jd_aurine_dev_Batchupdate
  Description:	������ѯ���ּҵ��豸״̬
  Input: 		
  	2.addr		�豸�ڵ��ַ,=0xFF:��㲥��ѯ�������Ե��ѯ
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_dev_Batchupdate(AU_JD_DEV_TYPE JDType,uint32 Check,uint8 num,uint16 *ID_list, uint8 *index);
/*************************************************
  Function:		jd_aurine_dev_all_open
  Description:	���ּҵ��豸ȫ��
  Input: 		
  	1.inedx		�豸����		
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_dev_all_open(uint8 devtype);

/*************************************************
  Function:		jd_aurine_dev_all_close
  Description:	���ּҵ��豸ȫ��
  Input: 		
  	1.inedx		�豸����		
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_aurine_dev_all_close(uint8 devtype);

/*************************************************
  Function:		jd_aurine_init
  Description: 	
  Input: 		
  Return:		��  
*************************************************/
void jd_set_state_list(AU_JD_DEV_TYPE devtype, PJD_DEV_LIST_INFO statelist);

typedef void (*PAuJiaDianStateChanged)(uint8 devtype, uint8 devindex , uint8 param2);

/*************************************************
  Function:    	jd_state_report_init
  Description: 	�ҵ����ݴ���ص��ҵ����	
  Input:
  	1.			�ص�
  Output: 		��   		
  Return:		��
  Others:           
*************************************************/
void jd_state_report_init(PAuJiaDianStateChanged proc);

/*************************************************
  Function:		jd_load_devinfo
  Description: 	�����豸��Ϣ
  Input: 		
  Return:		��  
*************************************************/
void jd_load_devinfo(void);

#endif

/*************************************************
  Function:		init_jiadian_callback
  Description:	��ʼ���ҵ�ģ��ַ��ص�����
  Input: 
	1.func		���ݴ�����ָ��
	2.timeout	��ʱ������ָ��
  Output:		��
  Return:		��
  Others:
*************************************************/
int32 init_jiadian_callback(JD_REPORT_CALLBACK func);

/*************************************************
  Function:		jiadian_responsion
  Description: 	�ҵ�ģ��Ӧ�������
  Input: 
	1.mode		�龰ģʽ
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jiadian_responsion(uint8 *rcvbuf);

/*************************************************
  Function:		jd_stop_feet_dog
  Description:  �ҵ�ģ����ֹͣ���Ź�
  Input:
  	1.buf		����
  	2.len		����
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_stop_feet_dog(uint8 * buf, uint8 len);

/*************************************************
  Function:		jd_logic_init
  Description:  �ҵ��ʼ��
  Input: 
  Output:		��
  Return:		��
  Others:		��
*************************************************/
void jd_logic_init(void);
#endif

