/*************************************************
  Copyright (C), 2006-2010, Aurine
  File name:	net_command.h
  Author:   	xiewr
  Version:   	1.0
  Date: 		
  Description:  ��������ͨѶЭ��_�����
  
  History:        
                  
    1. Date:	
       Author:	
       Modification:
    2. ...
*************************************************/
#ifndef __NET_COMMAND_H__
#define __NET_COMMAND_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned short NET_COMMAND;

/************************����Ϊ������������**********************************/
#define CMD_SET_SYS_PASS	 		(SSC_PUBLIC<<8 | 0x01)		// ���ù���Ա����
#define CMD_SET_ROOM_PARAM			(SSC_PUBLIC<<8 | 0x02)		// ���ڻ���������
#define CMD_SET_AREACODEAM			(SSC_PUBLIC<<8 | 0x04)		// ��������
#define CMD_TERMINAL_CMD			(SSC_PUBLIC<<8 | 0x05)		// �ն�����
#define CMD_DEVNO_RULE_CMD			(SSC_PUBLIC<<8 | 0x06)		// ���ñ�Ź���

#define CMD_GET_SUIT_SERVER			(SSC_PUBLIC<<8 | 0x14)		// ��ȡ���ʺϵķ�������ַ
#define CMD_GET_EVENT_SERVER		(SSC_PUBLIC<<8 | 0x15)		// ��ȡ�¼��ϱ���������ַ
#define CMD_SEND_MAXDATA			(SSC_PUBLIC<<8 | 0x20)		// �ְ�����
#define CMD_STOP_SEND_MAXDATA		(SSC_PUBLIC<<8 | 0x21)		// �жϷְ�����

#define CMD_GET_MANAGER_IP			(SSC_PUBLIC<<8 | 0x4B)		// ��ȡ����Ա����ַ
#define CMD_QUERY_DEVICE			(SSC_PUBLIC<<8 | 0x53)		// �豸��ѯ
#define CMD_REQ_SYN_TIME			(SSC_PUBLIC<<8 | 0x54)		// ����ʱ��ͬ��
#define CMD_SYN_TIME				(SSC_PUBLIC<<8 | 0x55)		// ʱ��ͬ��
#define CMD_DNS_GETIP				(SSC_PUBLIC<<8 | 0x56)		// DNS���㲥��ȡ�豸IP
#define CMD_SERVER_GETIP			(SSC_PUBLIC<<8 | 0x57)		// DNS�����ķ�������ȡ�豸IP

#define CMD_REGISTER_AURINE			(SSC_PUBLIC<<8 | 0x61)		// �����ն�ע��	0x61
#define CMD_REGISTER				(SSC_PUBLIC<<8 | 0x62)		// �ն˱���	0x62
#define CMD_RTSP_KEEP				(SSC_PUBLIC<<8 | 0x67)		// ����ý������������ն˱���

#define CMD_ELEVATOR_BECKON			(SSC_PUBLIC<<8 | 0xA0)		// �ƶ��ն����������ڻ����������ٻ�
#define CMD_ELEVATOR_BECKON_UNLOCK	(SSC_PUBLIC<<8 | 0xA1)		// �ƶ��ն������������������������ٻ�����

#define CMD_SET_MAC					(SSC_PUBLIC<<8 | 0xB0)		// ����MAC��
#define CMD_IPMODULE_HEART			(SSC_PUBLIC<<8 | 0xB1)		// IPģ����������������ͨѶ
#define CMD_GET_PARAM				(SSC_PUBLIC<<8 | 0xB2)		// ��ȡ������������ز�����Ϣ����
#define CMD_GET_NETDOOR_LIST		(SSC_PUBLIC<<8 | 0xB3)		// ��ȡ�������������������ǰ����Ϣ�б�
#define CMD_GET_MANAGER_LIST		(SSC_PUBLIC<<8 | 0xB4)		// ��ȡ������������Ĺ��������б�
#define CMD_IPMODULE_BIND_QUEST		(SSC_PUBLIC<<8 | 0xB5)		// ���ڷֻ���IPģ����������ע��
#define CMD_IPMODULE_BIND			(SSC_PUBLIC<<8 | 0xB6)		// IPģ��������������
#define CMD_GET_FENJI_INFO			(SSC_PUBLIC<<8 | 0xB8)		// ��ȡ�ֻ��б�
#define CMD_GET_EXTENSION_ADDR		(SSC_PUBLIC<<8 | 0xB9)		// �ƶ��ն˻����ڷֻ���IPģ���ȡ�ֻ���IP��ַ
/************************����Ϊ������������**********************************/

/************************����Ϊ������������**********************************/
#define CMD_QUERY_AREA_DEFINE		(SSC_ALARM<<8 | 0x10)		// ��ѯ��������
#define CMD_RP_AREA_DEFINE			(SSC_ALARM<<8 | 0x11)		// �ϱ���������
#define CMD_QUERY_ALARM_STATUS		(SSC_ALARM<<8 | 0x12)		// ����״̬��ѯ
#define CMD_RP_ALARM_STATE			(SSC_ALARM<<8 | 0x14)		// ����״̬�ϱ�

#define CMD_ALARM_SET				(SSC_ALARM<<8 | 0x20)		// ��������
#define CMD_ALARM_UNSET				(SSC_ALARM<<8 | 0x21)		// ��������
#define CMD_ALARM_PARTSET			(SSC_ALARM<<8 | 0x22)		// �ַ�����

#define CMD_RP_SET_EVENT			(SSC_ALARM<<8 | 0x30)		// ���������¼�
#define CMD_RP_UNSET_EVENT			(SSC_ALARM<<8 | 0x31)		// ���������¼�
#define CMD_RP_PARTSET_EVENT		(SSC_ALARM<<8 | 0x32)		// �ַ������¼�
#define CMD_RP_BYPASS_EVENT			(SSC_ALARM<<8 | 0x33)		// ��·�����¼�
// add by luofl 2011-4-27 begin
#define CMD_SYNC_SET_EVENT			(SSC_ALARM<<8 | 0x3D)		// ����ͬ���¼�
#define CMD_SYNC_UNSET_EVENT		(SSC_ALARM<<8 | 0x3E)		// ����ͬ���¼�
#define CMD_SYNC_PARTSET_EVENT		(SSC_ALARM<<8 | 0x3F)		// �ַ�ͬ���¼�
// add by luofl 2011-4-27 end

#define CMD_RP_FORCE_UNSET_ALARM	(SSC_ALARM<<8 | 0x41)		// Ю�ֳ�������
#define CMD_RP_SOS_ALARM			(SSC_ALARM<<8 | 0x42)		// ������ȱ���
#define CMD_RP_AREA_BREAK_ALARM		(SSC_ALARM<<8 | 0x43)		// ������������
#define CMD_RP_AREA_FAULT_ALARM		(SSC_ALARM<<8 | 0x44)		// �������ϱ���

//add by fanfj
#ifdef _IP_MODULE_ALARM
#define CMD_CLEAR_ALARM				(SSC_ALARM<<8 | 0x23)		// �����ʾ
#define CMD_SOS_REMOTECTRL			(SSC_ALARM<<8 | 0x25)		// Զ��SOS����
#define CMD_TRANSMIT_ALARMRECORD	(SSC_ALARM<<8 | 0x45)		// ת��������¼
#define CMD_QUERY_ALARMRECORD		(SSC_ALARM<<8 | 0x46)		// ��ѯ������¼
#endif
/************************����Ϊ������������**********************************/

/************************����Ϊ�Խ���������**********************************/
#define CMD_QUERY_ACCESS_PASS		(SSC_INTERPHONE<<8 | 0x10)	// ��ѯ��������
#define CMD_CLEAR_ACCESS_PASS		(SSC_INTERPHONE<<8 | 0x11)	// ��λ��������
#define CMD_RP_CARD_ACCESS_EVENT	(SSC_INTERPHONE<<8 | 0x31)	// �ϱ�ˢ�������¼�

#ifdef _AU_PROTOCOL_
#define CMD_UNLOCK					(SSC_INTERPHONE<<8 | 0x20)	// ����
#define CMD_MONITOR					(SSC_INTERPHONE<<8 | 0x21)	// ����
#define CMD_MONITOR_TO_TALK			(SSC_INTERPHONE<<8 | 0x22)	// ���ӽ���ͨ��
#define CMD_STOP_MONITOR			(SSC_INTERPHONE<<8 | 0x23)	// �жϼ���
#define CMD_MONITOR_HEART			(SSC_INTERPHONE<<8 | 0x24)	// �������֣�������
#define CMD_RP_SNAPSHOT				(SSC_INTERPHONE<<8 | 0x25)	// ��(��)�������ϱ�

#define CMD_CALL_CALLING			(SSC_INTERPHONE<<8 | 0x50)	// ��������	0x50
#define CMD_CALL_ANSWER				(SSC_INTERPHONE<<8 | 0x51)	// ȷ������	0x51
#define CMD_CALL_HANDDOWN			(SSC_INTERPHONE<<8 | 0x52)	// �Ҷ�����	0x52
#define CMD_CALL_HEART				(SSC_INTERPHONE<<8 | 0x53)	// ͨ������	0x53
#else
#define CMD_UNLOCK					(SSC_INTERPHONE<<8 | 0xA0)	// ����
#define CMD_MONITOR					(SSC_INTERPHONE<<8 | 0xA1)	// ����
#define CMD_MONITOR_TO_TALK			(SSC_INTERPHONE<<8 | 0xA2)	// ���ӽ���ͨ��
#define CMD_STOP_MONITOR			(SSC_INTERPHONE<<8 | 0xA3)	// �жϼ���
#define CMD_MONITOR_HEART			(SSC_INTERPHONE<<8 | 0xA4)	// �������֣�������
#define CMD_RP_SNAPSHOT				(SSC_INTERPHONE<<8 | 0xA5)	// ��(��)�������ϱ�

#define CMD_CALL_CALLING			(SSC_INTERPHONE<<8 | 0xB0)	// ��������
#define CMD_CALL_ANSWER				(SSC_INTERPHONE<<8 | 0xB1)	// ȷ������
#define CMD_CALL_HANDDOWN			(SSC_INTERPHONE<<8 | 0xB2)	// �Ҷ�����
#define CMD_CALL_HEART				(SSC_INTERPHONE<<8 | 0xB3)	// ͨ������
#endif
/************************����Ϊ�Խ���������**********************************/

/************************����Ϊ��Ϣ��������**********************************/
#define CMD_SEND_INFO				(SSC_INFO<<8 | 0x10)		// ������Ϣ
#define CMD_SEND_SHORT_MSG			(SSC_INFO<<8 | 0x11)		// ���ŷ���(�û������ڻ���)
#define CMD_SEND_MSG_VALIDATE		(SSC_INFO<<8 | 0x12)		// ���Ͷ��Ź�����֤��
#define CMD_SHORT_MSG_TRANSMIT		(SSC_INFO<<8 | 0x13)		// ����ת��
/************************����Ϊ��Ϣ��������**********************************/

/************************����Ϊ������ǰ������(�������������ع���)************/
#define CMD_GATEWAY_SET_NET_PARAM	(SSC_IPDOORPHONE<<8 | 0x10)		// ����������ǰ�������
#define CMD_GATEWAY_SET_DEV_INFO	(SSC_IPDOORPHONE<<8 | 0x11)		// ����������ǰ���豸����Լ�����
#define CMD_GATEWAY_SET_DATE_TIME	(SSC_IPDOORPHONE<<8 | 0x14)		// �������ڡ�ʱ�䡢ʱ��
#define CMD_GATEWAY_SET_OTHERS		(SSC_IPDOORPHONE<<8 | 0x16)		// ��������

#define CMD_GATEWAY_GET_NET_PARAM	(SSC_IPDOORPHONE<<8 | 0x20)		// ��ѯ������ǰ�������
#define CMD_GATEWAY_GET_DEV_INFO	(SSC_IPDOORPHONE<<8 | 0x21)		// ��ѯ������ǰ���豸���
#define CMD_GATEWAY_GET_DATE_TIME	(SSC_IPDOORPHONE<<8 | 0x24)		// ��ѯ���ڡ�ʱ�䡢ʱ��
#define CMD_GATEWAY_GET_OTHERS		(SSC_IPDOORPHONE<<8 | 0x26)		// ��ѯ��������

#define CMD_GATEWAY_GET_MAC			(SSC_IPDOORPHONE<<8 | 0x30)		// ��ѯ������ǰ��MAC
#define CMD_GATEWAY_GET_VER			(SSC_IPDOORPHONE<<8 | 0x31)		// ��ѯ������ǰ���汾��Ϣ

#define CMD_NETDOOR_MNG_CARD		(SSC_IPDOORPHONE<<8 | 0x40)		// ������ǰ��������
#define CMD_NETDOOR_GET_CARD_INFO	(SSC_IPDOORPHONE<<8 | 0x41)		// ��ÿ���Ϣ
#define CMD_NETDOOR_SET_LOCK		(SSC_IPDOORPHONE<<8 | 0x42)		// ��������Ϣ
#define CMD_NETDOOR_GET_LOCK		(SSC_IPDOORPHONE<<8 | 0x43)		// �������Ϣ
/************************����Ϊ������ǰ������(�������������ع���)************/

/************************����Ϊ������������**********************************/
#define CMD_ELEVATOR_CALL_AUTHORIZE	(SSC_OTHER<<8 | 0x16)		// ������Ȩ ���������豸1B+���к��봮30B+���к��봮30B
#define CMD_ELEVATOR_OPEN_TK_LOCK	(SSC_OTHER<<8 | 0x17)		// ֪ͨ���ڻ����ݿڻ���
#define CMD_ELEVATOR_OTHER_CTRL		(SSC_OTHER<<8 | 0x19)		// ���������ݿ���
/************************����Ϊ������������**********************************/

/************************����ΪԶ�̼ҵ�����**********************************/
#define CMD_JD_DEV_OPEN				(SSC_EHOME<<8 | 0x10)		// �ҵ��豸��
#define CMD_JD_DEV_CLOSE			(SSC_EHOME<<8 | 0x11)		// �ҵ��豸��
#define CMD_JD_SCENE_MODE			(SSC_EHOME<<8 | 0x12)		// �ҵ��龰ģʽ
#define CMD_JD_GET_JD_INFO			(SSC_EHOME<<8 | 0xA0)		// ��ȡ�ҵ���Ϣ
#define CMD_JD_GET_DEVINFO			(SSC_EHOME<<8 | 0xA1)		// ��IPģ���ȡ�ҵ��豸��Ϣ�б�
#ifdef _AURINE_ELEC_NEW_
#define CMD_JD_STATE_QUERY 			(SSC_EHOME<<8 | 0x0F)		// �ҵ��ѯ����
#endif
/************************����ΪԶ�̼ҵ�����**********************************/

/************************����Ϊ��Ƶ���Ӳ�������******************************/
#define CMD_RTSP_PTZ_CONTROL		(SSC_VIDEOMONITORING<<8 | 0x01)		// ��̨���� add by fanfj
#define CMD_GET_AREA_CAMERALIST		(SSC_VIDEOMONITORING<<8 | 0x03)		// ��ȡС����������豸����Ϣ�б� add by fanfj
#define CMD_GET_HOME_CAMERALIST		(SSC_VIDEOMONITORING<<8 | 0xD0)		// ��ȡ��������ͷ��Ϣ�б� add by fanfj

#define CMD_GET_AREA_CAMERALIST_EXT	(SSC_VIDEOMONITORING<<8 | 0x06)		// ��ȡС����������豸����Ϣ�б� add by chenbh 2016-03-04
#define CMD_GET_HOME_ONLINE_LIST_EXT (SSC_VIDEOMONITORING<<8 | 0x07)	// ��ȡ���ڻ�IPC�����б�
#define CMD_GET_HOME_CAMERALIST_EXT	(SSC_VIDEOMONITORING<<8 | 0xD1)		// ��ȡ��������ͷ��Ϣ�б� add by chenbh 2016-03-04
/************************����Ϊ��Ƶ���Ӳ�������******************************/


#ifdef __cplusplus
}
#endif

#endif

