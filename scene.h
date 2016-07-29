/** @file
 * ITE DoorBell Indoor Scene Definition.
 *
 * @author Jim Tan
 * @version 1.0
 * @date 2013
 * @copyright ITE Tech. Inc. All Rights Reserved.
 */
/** @defgroup doorbell_indoor ITE DoorBell Indoor Modules
 *  @{
 */
#ifndef SCENE_H
#define SCENE_H
#include "SysGlobal.h"
#include "ite/itu.h"

#ifdef __cplusplus
extern "C" {
#endif


	extern ITUScene theScene;

	// ������״̬�ص�����
	typedef enum
	{
		UPDATE_DEVNO_E = 0X00,					// �����豸��		
		UPDATE_NET_STATE_E = 0X01,					// ��������״̬
		UPDATE_IPMODEL_STATE_E = 0X02,					// ����IPģ��״̬
		UPDATE_ALARM_E = 0X03,					// ���±�������
		UPDATE_CALL_E = 0X04,					// ����δ�Ӽ�¼����
		UPDATE_MESSAGE_E = 0X05,					// ������Ϣ��Ϣ����
		UPDATE_LYLY_E = 0X06,					// ������Ӱ��������
		UPDATE_UNDISTURB_E = 0X07,					// ���������״̬
		UPDATE_JRLY_E = 0X08,					// ���¼�����������

	}UPDATE_STATE_E;

	/**
	 * Custom event definition. from Custom0->Custom30
	 */
	typedef enum
	{
		EVENT_CUSTOM0_STATE_UPDATE = ITU_EVENT_CUSTOM, 	// ״̬����֪ͨ �ο�UPDATE_STATE_E 
		EVENT_CUSTOM1_SCREENSAVER, 								// ��Ļ����
		EVENT_CUSTOM2_RESER, 									// Ԥ��
		EVENT_CUSTOM3_RESER, 									// Ԥ��
		EVENT_CUSTOM4_RESER, 									// Ԥ��
		EVENT_CUSTOM5_RESER, 									// Ԥ��
		EVENT_CUSTOM6_RESER, 									// Ԥ��
		EVENT_CUSTOM7_RESER, 									// Ԥ��
		EVENT_CUSTOM8_RESER, 									// Ԥ��
		EVENT_CUSTOM9_RESER, 									// Ԥ��
		EVENT_CUSTOM10_RESER, 									// Ԥ��
		EVENT_CUSTOM11_CALLOUT, 								// ���лص���Ϣ						
		EVENT_CUSTOM12_CALLIN, 									// ���лص���Ϣ
		EVENT_CUSTOM13_MONITOR, 								// ���ӻص���Ϣ	
		EVENT_CUSTOM14_RTSP_MONITOR, 							// RTSP���ӻص���Ϣ	

	} CustomEvent;

	/**
	 * Quit value definition.
	 */
	typedef enum
	{
		QUIT_NONE,                  					///< Do not quit
		QUIT_DEFAULT,               					///< Quit for nothing
		QUIT_RESET_FACTORY,         					///< Quit to reset to factory setting
		QUIT_RESET_TEMP_DRIVE,     	 					///< Quit to reset temp drive
		QUIT_UPGRADE_FIRMWARE,      					///< Quit to upgrade firmware
		QUIT_UPGRADE_RESOURCE,      					///< Quit to upgrade resource
		QUIT_UPGRADE_ADDRESSBOOK,   					///< Quit to upgrade address book
		QUIT_UPGRADE_CARDLIST,      					///< Quit to upgrade card list
		QUIT_UPGRADE_WEB,           					///< Quit to wait web upgrading
		QUIT_RESET_NETWORK          					///< Quit to reset network
	} QuitValue;

	typedef enum
	{
		SPOTTED_NONE,
		SPOTTED_INIT,
		SPOTTED_REJECT,
		SPOTTED_ALLOW,
	} SPOTTED_STATUS;


	/*************************************************
	  Function:			af_callback_gui
	  Description:
	  Input:
	  Output:			��
	  Return:
	  Others:
	  *************************************************/
	int32 af_callback_gui(int32 Param1, int32 Param2);

	/*************************************************
	  Function:			callout_state_callbak
	  Description:
	  Input:
	  Output:			��
	  Return:
	  Others:
	  *************************************************/
	void callout_state_callbak(uint32 param1, uint32 param2);

	/*************************************************
	  Function:			callin_state_callbak
	  Description:
	  Input:
	  Output:			��
	  Return:
	  Others:
	  *************************************************/
	void callin_state_callbak(uint32 param1, uint32 param2);

	/*************************************************
	  Function:			monitor_state_callbak
	  Description:
	  Input:
	  Output:			��
	  Return:
	  Others:
	  *************************************************/
	void monitor_state_callbak(uint32 param1, uint32 param2);

	/*************************************************
	  Function:			rtsp_monitor_state_callbak
	  Description: 		����״̬�ص�
	  Input:
	  1.param1
	  2.param2
	  Output:			��
	  Return:
	  Others:
	  *************************************************/
	void rtsp_monitor_state_callbak(uint32 param1, uint32 param2);

	/*************************************************
	  Function:			show_sys_event_hint
	  Description:
	  Input:
	  Output:			��
	  Return:
	  Others:
	  *************************************************/
	void show_sys_event_hint(uint16 EventType);

	// scene
	/**
	 * Initializes scene module.
	 */
	void SceneInit(void);

	/**
	 * Exits scene module.
	 */
	void SceneExit(void);

	/**
	 * Loads ITU file.
	 */
	void SceneLoad(void);

	/**
	 * Runs the main loop to receive events, update and draw scene.
	 *
	 * @return The QuitValue.
	 */
	int SceneRun(void);


	/**
	 * Quits the scene.
	 *
	 * @param value The reason to quit the scene.
	 */
	void SceneSetQuitValue(QuitValue value);

	/**
	 * Gets the current quit value.
	 *
	 * @return The current quit value.
	 */
	QuitValue SceneGetQuitValue(void);


#ifdef __cplusplus
}
#endif

#endif /* SCENE_H */
/** @} */ // end of doorbell_indoor
