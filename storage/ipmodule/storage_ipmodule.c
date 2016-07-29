/*************************************************
  Copyright (C), 2006-2010, Aurine
  File name: 	storage_ipmodule.c
  Author:   	
  Version:  	
  Date: 		2012-8-24
  Description:  ��IPģ��֮��Ĵ洢�ӿ��ļ�
				
  History:        
*************************************************/
#include "storage_ipmodule.h"

#ifdef _IP_MODULE_
static IPAD_EXTENSION_LIST g_ipadList;

/*************************************************
  Function:    		set_ipmodule_bindstate
  Description:		������IPģ�������״̬
  Input: 			
  	1.state			�����״̬
  Output:			��
  Return:			��
  Others:
*************************************************/
void set_ipmodule_bindstate(uint32 state)
{
	gpSysParam->IpModule.State= state;
	dprintf("gpSysParam->IpModule.State = %d\n",gpSysParam->IpModule.State);
	SaveRegInfo();
}

/*************************************************
  Function:    		get_ipmodule_bindcode
  Description:		�����IPģ�������״̬
  Input: 			��
  Output:			��
  Return:			�����״̬
  Others:
*************************************************/
uint32 get_ipmodule_bindstate(void)
{
	return gpSysParam->IpModule.State;
}

/*************************************************
  Function:    		set_ipmodule_bindcode
  Description:		������IPģ���������֤��
  Input: 			
  	1.code			�������֤��
  Output:			��
  Return:			��
  Others:
*************************************************/
void set_ipmodule_bindcode(uint32 code)
{
	gpSysParam->IpModule.BindCode = code;
	SaveRegInfo();
}

/*************************************************
  Function:    		get_ipmodule_bindcode
  Description:		�����IPģ���������֤��
  Input: 			��
  Output:			��
  Return:			�������֤��
  Others:
*************************************************/
uint32 get_ipmodule_bindcode(void)
{
	return gpSysParam->IpModule.BindCode;
}

/*************************************************
  Function:    		set_ipmodule_addr
  Description:		������IPģ�������IP
  Input: 			
  	1.addr			IP��ַ
  Output:			��
  Return:			��
  Others:
*************************************************/
void set_ipmodule_addr(uint32 addr)
{
	gpSysParam->IpModule.IpAddr = addr;
	SaveRegInfo();
}

/*************************************************
  Function:    		get_ipmodule_bindcode
  Description:		�����IPģ�������IP
  Input: 			��
  Output:			��
  Return:			�����IP
  Others:
*************************************************/
uint32 get_ipmodule_addr(void)
{
	return gpSysParam->IpModule.IpAddr;
}

/*************************************************
  Function:    		set_ipmodule_info
  Description:		������IPģ��
  Input: 			
  	1.code			�������֤��
  Output:			��
  Return:			��
  Others:
*************************************************/
void set_ipmodule_info(PIPMODULE_INFO IpModule)
{
	memcpy(&gpSysParam->IpModule, IpModule, sizeof(IPMODULE_INFO));
	dprintf("set_ipmodule_info = %d\n",gpSysParam->IpModule.State);
	SaveRegInfo();
}

/*************************************************
  Function:    		set_ipad_extension
  Description:		����ipad�ֻ���Ϣ
  Input: 			
  	1.ipadList		ipad�ֻ���Ϣ�б�
  Output:			��
  Return:			��
  Others:
*************************************************/
int32 set_ipad_extension(IPAD_EXTENSION_LIST ipadList)
{
	memcpy(&g_ipadList, &ipadList, sizeof(IPAD_EXTENSION_LIST));
	return 1;
}

/*************************************************
  Function:    		get_ipad_extension
  Description:		��ȡipad�ֻ���Ϣ
  Input: 			��
  Output:			
  	1.ipadList		ipad�ֻ���Ϣ�б�
  Return:			true/false
  Others:
*************************************************/
uint32 get_ipad_extension(IPAD_EXTENSION_LIST *pIpadList)
{
	if ( NULL == pIpadList )
	{
		return FALSE;
	}

	memcpy(pIpadList, &g_ipadList, sizeof(IPAD_EXTENSION_LIST));
	return TRUE;
}

/*************************************************
  Function:    		get_ipad_extension_addr
  Description:		��ȡipad�ֻ���ϢIP
  Input: 			��
  Output:			
  	1.devno			
  Return:			IP
  Others:
*************************************************/
uint32 get_ipad_extension_addr(uint32 devno)
{
	uint8 i;
	uint32 address = 0;
	
	for (i = 0 ; i < g_ipadList.count; i++)
	{
		if (g_ipadList.ipadData[i].devno == devno)
		{
			if (g_ipadList.ipadData[i].state)
			{
				address = g_ipadList.ipadData[i].ipAddr;
			}
			else
			{
				address = get_ipmodule_addr();
			}
			break;
		}
	}
	
	return address;
}
#endif

