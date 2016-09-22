/*********************************************************
  Copyright (C), 2006-2010, Aurine
  File name:	logic_model_func.h
  Author:   	
  Version:   	
  Date: 		
  Description:  ��������
  
  History:                   
    1. Date:			 
       Author:				
       Modification:	
    2. ...
*********************************************************/
#ifndef __LOGIC_MODEL_FUNC_H__
#define __LOGIC_MODEL_FUNC_H__


#include "logic_include.h"

/*************************************************
  Function:		start_disturb_timer
  Description:  ��������Ŷ�ʱ��
  Input:		��
  Output:		��
  Return:		��
  Others:		
*************************************************/
void start_disturb_timer(void);

/*************************************************
  Function:		stop_disturb_timer
  Description:  ֹͣ����Ŷ�ʱ��
  Input:		��
  Output:		��
  Return:		��
  Others:		
*************************************************/
void stop_disturb_timer(void);

/*************************************************
  Function:		get_fenji_list
  Description:  ��ȡ�ֻ��б�
  Input:		��
  Output:		��
  Return:		��
  Others:		
*************************************************/
uint32 get_fenji_list(void);

/*************************************************
Function:		gb2312ToUtf8
Description:	�ַ�����ת��
Input:
1.ptDestText	Ŀ���ַ���
2.nDestLength	����
3.ptSrcText		Դ�ַ���
4.nSrcLength	����

Output:			��
Return:			��
Others:
*************************************************/
void gb2312ToUtf8(char*  ptDestText, int  nDestLength, char*  ptSrcText, int  nSrcLength);



#endif
