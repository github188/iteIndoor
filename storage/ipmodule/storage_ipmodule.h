/*************************************************
  Copyright (C), 2006-2010, Aurine
  File name: 	storage_ipmodule.h
  Author:		
  Version:		
  Date:	  		2012-8-27
  Description: 	��IPģ��֮��Ĵ洢�ӿ�ͷ�ļ�	 
  
  History:				  
*************************************************/
#ifndef __STORAGE_IPMODULE_H__
#define	__STORAGE_IPMODULE_H__

#include "storage_include.h"
//#include "storage_types.h"

#define IPAD_EXTENSION_MAX			6 // ipad�ֻ��������

typedef struct
{
	uint32 devno;				// �ֻ���
	uint32 state;				// �ֻ�״̬: 0-���� 1-����
	uint32 ipAddr;				// �ֻ�IP��ַ
}IPAD_DATA, *PIPAD_DATA;		// ipad�ֻ���Ϣ


typedef struct
{
	uint32 count;							// �ֻ�����
	IPAD_DATA ipadData[IPAD_EXTENSION_MAX];	// �ֻ���Ϣ
}IPAD_EXTENSION_LIST, *PIPAD_EXTENSION_LIST;// ipad�ֻ���Ϣ�б�



/*************************************************
  Function:    		get_ipmodule_bindcode
  Description:		�����IPģ���������֤��
  Input: 			��
  Output:			��
  Return:			�������֤��
  Others:
*************************************************/
uint32 get_ipmodule_bindcode(void);

/*************************************************
  Function:    		get_ipmodule_bindcode
  Description:		�����IPģ�������IP
  Input: 			��
  Output:			��
  Return:			�����IP
  Others:
*************************************************/
uint32 get_ipmodule_addr(void);

/*************************************************
  Function:    		get_ipmodule_bindstate
  Description:		�����IPģ�������״̬
  Input: 			��
  Output:			��
  Return:			�����״̬
  Others:
*************************************************/
uint32 get_ipmodule_bindstate(void);

/*************************************************
  Function:    		set_ipmodule_bindstate
  Description:		������IPģ�������״̬
  Input: 			
  	1.state			�����״̬
  Output:			��
  Return:			��
  Others:
*************************************************/
void set_ipmodule_bindstate(uint32 state);

/*************************************************
  Function:    		set_ipmodule_info
  Description:		������IPģ��
  Input: 			
  	1.code			�������֤��
  Output:			��
  Return:			��
  Others:
*************************************************/
void set_ipmodule_info(PIPMODULE_INFO IpModule);

/*************************************************
  Function:    		set_ipad_extension
  Description:		����ipad�ֻ���Ϣ
  Input: 			
  	1.ipadList		ipad�ֻ���Ϣ�б�
  Output:			��
  Return:			��
  Others:
*************************************************/
int32 set_ipad_extension(IPAD_EXTENSION_LIST ipadList);

/*************************************************
  Function:    		get_ipad_extension_addr
  Description:		��ȡipad�ֻ���ϢIP
  Input: 			��
  Output:			
  	1.devno			
  Return:			IP
  Others:
*************************************************/
uint32 get_ipad_extension_addr(uint32 devno);

#endif /*__STORAGE_IPMODULE_H__*/

