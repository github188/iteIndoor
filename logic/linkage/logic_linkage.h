/*************************************************
  Copyright (C), 2006-2010, Aurine
  File name:  	logic_linkage.h
  Author:     	xiewr
  Version:    	1.0
  Date: 
  Description:  ����ͷ�ļ�
  
  History:        
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#ifndef _SYS_LINKAGE_H_
#define _SYS_LINKAGE_H_

#include "logic_include.h"


typedef enum
{
	LVEWORD_TYPE_NONE = 0,							// ����Ӱ���Թ���
	LVEWORD_TYPE_TIMEOUT,							// ���к��г�ʱ��Ӱ���Թ���
	LVEWORD_TYPE_AUTO								// �Զ��������Թ���
}LVEWORD_STATE_TYPE;

/*************************************************
  Function:			linkage_get_nodisturb_state
  Description: 		��ȡϵͳ��ǰ�����״̬
  Input: 			��
  Output:			��
  Return:			TRUE / FALSE
  Others:			
*************************************************/
int32 linkage_get_nodisturb_state(void);

/*************************************************
  Function:			linkage_get_lvd_state
  Description: 		��ȡϵͳ��ǰ��Ӱ����״̬
  Input: 			��
  Output:			��
  Return:			��Ӱ����״̬
  Others:			
*************************************************/
LVEWORD_STATE_TYPE linkage_get_lvd_state(void);

/*************************************************
  Function:			linkage_alarm_card
  Description: 		����ˢ������
  Input: 			
  	1.DevType		�豸���� 0-�ݿ� 1-��ǰ
  Output:			��
  Return:			TRUE/FALSE
  Others:			
*************************************************/
int32 linkage_alarm_card(uint8 DevType);

/*************************************************
  Function:			linkage_play_door_ring
  Description: 		������ǰ��
  Input: 			��
  Output:			��
  Return:			TRUE/FALSE
  Others:			
*************************************************/
void linkage_play_door_ring(void);

/*************************************************
  Function:			linkage_8area_door
  Description: 		��8����������ǰ��
  Input: 			��
  Output:			��
  Return:			TRUE/FALSE
  Others:			
*************************************************/
int32 linkage_8area_door(void);

/*************************************************
  Function:			linkage_alarm_scene
  Description: 		�����������龰
  Input: 			
  	1.state			����������
  Output:			��
  Return:			��
  Others:			
*************************************************/
void linkage_alarm_scene(DEFEND_STATE state);
#endif

