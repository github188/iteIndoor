/********************************************************
  Copyright (C), 2006-2016, Aurine
  File name:	storage_yuyue.h
  Author:		luofl
  Version:		V2.0
  Date:			2011-4-9
  Description:	�ҵ�ԤԼ�洢ͷ�ļ�

  History:                 
    1. Date:
       Author:
       Modification: 
    2. ...
*********************************************************/
#ifndef __STORAGE_YUYUE_H__
#define __STORAGE_YUYUE_H__

#define FILE_LEN				100 				// �����ļ��ı��泤��
#define EVENT_COUNT				20					// �¼�ԤԼ����
#define CLOCK_COUNT				5					// ��������
#define JD_COUNT				20					// �ҵ�ԤԼ����
#define SM_COUNT				20					// �龰ԤԼ����
#define BJ_COUNT				20					// ����ԤԼ����

#define MAX_YUYUE_NUM			20

// ԤԼ����
typedef enum
{
	EV_BE_EVENT = 0,								// �¼�ԤԼ
	EV_BE_AF,										// ����ԤԼ
	EV_BE_JD,										// �ҵ�ԤԼ
	EV_BE_SM										// �龰ģʽԤԼ
}AU_BE_TYPE;

// �¼�ԤԼ����
typedef enum
{
	BE_BWL = 0,										// ����¼
	BE_SR,											// ����
	BE_NZ,											// ����
	BE_HY,											// ����
	BE_JR,											// ����
	BE_SJ,											// �¼�
	BE_JC											// �Ͳ�
}BESPOKE_EVENT;

// ԤԼʱ������
typedef enum
{
	BE_DAY = 0,										// ÿ��
	BE_WEEK,										// ÿ��
	BE_MONTH,										// ÿ��
	BE_YEAR,										// ÿ��
	BE_MON_FRI,										// ��һ~��
	BE_MON_SAT,										// ��һ~��
	BE_ONE_DAY										// ĳ��
}BESPOKE_TYPE;

// ԤԼʱ���ṹ
typedef struct
{
	BESPOKE_TYPE	BeType;
	int8			tm_sec;         				// seconds
  	int8   			tm_min;         				// minutes
  	int8   			tm_hour;        				// hours
  	int16			tm_mday;        				// day of the month
  	int16  			tm_mon;         				// month
  	int16  			tm_year;        				// year
  	int16  			tm_wday;        				// day of the week
  	int16  			tm_yday;        				// day in the year
}BE_DATETIME, * PBE_DATETIME;

// ԤԼͨ�ýṹ��
typedef struct
{
	/*������ֵ���ڱ�ʶ: ����, ����, �ַ�
	�龰ģʽ��ֵ���ڱ�ʶ: 5��ģʽ*/
	uint8			Index;							// �¼�, �ҵ�, ����, �龰ģʽ������ֵ
	BE_DATETIME		BeTime;							// ��ʼʱ��
	//BE_DATETIME		EndTime;						// ����ʱ��
	AU_BE_TYPE 		BeType;							// ԤԼ����
	uint8			Used;							// �Ƿ�����	
	uint16			Action;							// ִ�ж���: ��/��						
	uint16 			Address;						// �豸��ַ
	uint8 			Type;							// �豸����
	uint8 			TipUsed;						// �¼��Ƿ�������ʾ��
	//uint32			Id;								// ��ʱ��ID
	uint8			IsRun;							// �Ƿ��ѿ�ʼִ��
	uint8			Degree;							// ���ڵĳ̶�
	uint8           IsTune;                         // �豸�Ƿ�ɵ�
	char			Name[50];						// �豸����
}BE_COMM, * PBE_COMM;

// ԤԼ���ݽṹ��������Ϣ
typedef struct
{
	int32		nCount;
	PBE_COMM 	be_comm;
}BE_COMM_LIST, * PBE_COMM_LIST;

#if 0
// ��ʾ����������Ϣ
typedef struct __TIPLIST
{
	INT32	nCount;
	int8  	filename[10][FILE_LEN];
}TIPLIST;

// ԤԼ�ṹ������
typedef struct __SAVE_BECOMM_LIST{
	BE_COMM		Be_Comm;
	int32 		time_id; 							// �ö�ʱ��ID����ʶ
	struct 		__SAVE_BECOMM_LIST *next;
}SAVE_BECOMM_LIST, * PSAVE_BECOMM_LIST;

// �¼���ԤԼ�������ݽṹ
typedef struct
{
	BE_DATETIME		BeEventTime;
	uint8 			RingID;							// ��ʾ��ID  0:��������ʾ��
	uint8 			Type;							// �¼�����
}BE_EVENT_SJ, * PBE_EVENT_SJ;

typedef void (* BE_SJ_CALLBACK)(WINDOW* win, PBE_EVENT_SJ pEvent);
#endif
/*************************************************
  Function:		storage_get_yuyue
  Description: 	���ԤԼ�¼�
  Input:		
  	1.Calltype
  Output:		��
  Return:		��
  Others:		
  	1.��ָ���ⲿ�ͷ�
*************************************************/
PBE_COMM_LIST storage_get_yuyue (void);

/*************************************************
  Function:		storage_add_yuyue
  Description: 	���һ��ԤԼ�¼�
  Input:		
  	1.
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
ECHO_STORAGE storage_add_yuyue (PBE_COMM yuyue);

/*************************************************
  Function:		storage_del_yuyue
  Description: 	ɾ��ԤԼ
  Input:		
  	1.Index		����
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
ECHO_STORAGE storage_del_yuyue (uint8 Index);

/*************************************************
  Function:		storage_clear_yuyue
  Description: 	���ԤԼ�¼�
  Input:		��		
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
ECHO_STORAGE storage_clear_yuyue (void);

/*************************************************
  Function:		storage_yuyue_init
  Description: 	��ʼ��ԤԼ����
  Input:		��
  Output:		��
  Return:		
  Others:		
*************************************************/
void storage_yuyue_init(void);
#endif

