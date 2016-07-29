/*************************************************
  Copyright (C), 2006-2010, Aurine
  File name: 	logic_linkage.c
  Author:    	xiewr
  Version:   	1.0
  Date: 
  Description:  ����
				
  History:        
    1. Date:	2011-08-02
       Author:	luofl
       Modification:
       			���Ӱ�����������
    2. ...
*************************************************/
#include "logic_include.h"


/*************************************************
  Function:			linkage_get_nodisturb_state
  Description: 		��ȡϵͳ��ǰ�����״̬
  Input: 			��
  Output:			��
  Return:			TRUE / FALSE
  Others:			
*************************************************/
int32 linkage_get_nodisturb_state(void)
{	
	// ��ȡ�����״̬
	int32 isNoDisturb = storage_get_noface();
	
	return isNoDisturb;
}

/*************************************************
  Function:			linkage_get_lvd_state
  Description: 		��ȡϵͳ��ǰ��Ӱ����״̬
  Input: 			��
  Output:			��
  Return:			��Ӱ����״̬
  Others:			
*************************************************/
LVEWORD_STATE_TYPE linkage_get_lvd_state(void)
{
	LVEWORD_STATE_TYPE LvdState;
	int32 isNoDisturb = linkage_get_nodisturb_state();
	uint8 isLvdUsed = storage_get_lyly_enable();
	uint8 LvdMode = storage_get_lyly_mode();
	
	//if (isLvdUsed == FALSE || (LvdMode >= LYLY_TYPE_MAX || LvdMode < LYLY_TYPE_AUDIO))
	if (isLvdUsed == FALSE || LvdMode >= LYLY_TYPE_MAX )
	{
		LvdState = LVEWORD_TYPE_NONE;
	}
	else
	{
		// ��ȡ�����״̬�������������Ӱ����
		if (isNoDisturb == TRUE && storage_get_lyly_Linkmode(LYLY_LINK_NOFACE) == TRUE)
		{
			LvdState = LVEWORD_TYPE_AUTO;
		}
		// ��ȡ���״̬�����������Ӱ����
		else if (storage_get_defend_state() == SET_DEFEND && storage_get_lyly_Linkmode(LYLY_LINK_ALARMOUT) == TRUE)
		{
			LvdState = LVEWORD_TYPE_AUTO;
		}
		// ��ȡҹ��״̬��ҹ��������Ӱ����
		else if (storage_get_defend_state() == PART_DEFEND && storage_get_lyly_Linkmode(LYLY_LINK_ALARMNIGHT) == TRUE)
		{
			LvdState = LVEWORD_TYPE_AUTO;
		}
		else
		{
			LvdState = LVEWORD_TYPE_TIMEOUT;
		}	
	}
	return LvdState;
}

/*************************************************
  Function:			linkage_alarm_card
  Description: 		����ˢ������
  Input: 			
  	1.DevType		�豸���� 0-�ݿ� 1-��ǰ
  Output:			��
  Return:			TRUE/FALSE
  Others:			
*************************************************/
int32 linkage_alarm_card(uint8 DevType)
{
	uint8 isStairLink = 0;
	uint8 isDoorLink = 0;
	uint8 AlarmLinkParam = storage_get_link_param();
	isStairLink = AlarmLinkParam & 0x01;
	isDoorLink = AlarmLinkParam & 0x2;
	
	if (DevType == 0)
	{
		// �Ƿ������ݿ�ˢ����������
		if (isStairLink)
		{
			// ����
			alarm_unset_operator(EXECUTOR_LOCAL_HOST, 0);
		}	
	}
	else
	{
		// �Ƿ�������ǰˢ����������
		if (isDoorLink)
		{
			// ����		
			alarm_unset_operator(EXECUTOR_LOCAL_HOST, 0);
		}	
	}
	return TRUE;
}

/*************************************************
  Function:			linkage_play_door_ring
  Description: 		������ǰ��
  Input: 			��
  Output:			��
  Return:			TRUE/FALSE
  Others:			
*************************************************/
void linkage_play_door_ring(void)
{
	#if 0
	char * tmp = NULL;
	uint8 ringVolume, ret = FALSE;
	char ringFile[50] = {0};
	uint8 isRingLink = 0;
	uint8 AlarmLinkParam;

	AlarmLinkParam = storage_get_link_param();
	isRingLink = AlarmLinkParam & 0x04;
	if (isRingLink)
	{
		ringVolume = storage_get_ringvolume();
		tmp = storage_get_ring_by_devtype(DEVICE_TYPE_DOOR_NET);
		sprintf(ringFile, "%s", tmp);
		sys_start_play_audio(SYS_MEDIA_MUSIC, ringFile, FALSE, ringVolume, NULL, NULL);
		ret = TRUE;
	}
	return ret;
	#else
	char * tmp = NULL;
	uint8 ringVolume;
	char ringFile[50] = {0};

	ringVolume = storage_get_ringvolume();
	tmp = storage_get_ring_by_devtype(DEVICE_TYPE_DOOR_NET);
	sprintf(ringFile, "%s", tmp);
	sys_start_play_audio(SYS_MEDIA_MUSIC, ringFile, FALSE, ringVolume, NULL, NULL);
	
	#endif
}

/*************************************************
  Function:			linkage_8area_door
  Description: 		��8����������ǰ��
  Input: 			��
  Output:			��
  Return:			TRUE/FALSE
  Others:			
*************************************************/
int32 linkage_8area_door(void)
{
	uint8 ret = FALSE;
	uint8 isRingLink = 0;
	uint8 AlarmLinkParam;

	AlarmLinkParam = storage_get_link_param();
	isRingLink = AlarmLinkParam & 0x04;
	if (isRingLink)
	{
		ret = TRUE;
	}
	
	return ret;
}

#ifdef _JD_MODE_
/*************************************************
  Function:			linkage_alarm_scene
  Description: 		�����������龰
  Input: 			
  	1.state			����������
  Output:			��
  Return:			��
  Others:			
*************************************************/
void linkage_alarm_scene(DEFEND_STATE state)
{
	uint8 sceneLink[2];
	uint8 isbufanLink = 0, ischefanLink = 0;
	uint8 AlarmLinkParam;
	
	AlarmLinkParam = storage_get_link_param();
	storage_get_link_scene(sceneLink);
	isbufanLink = AlarmLinkParam & 0x08;
	ischefanLink = AlarmLinkParam & 0x10;
	
	if (state ==  SET_DEFEND && isbufanLink)
	{
		exec_jd_scene_mode(sceneLink[0]);
	}
	else if (state ==  DIS_DEFEND && ischefanLink)
	{
		exec_jd_scene_mode(sceneLink[1]);
	}
	dprintf("state :.......... %d \n",state);
}
#endif

