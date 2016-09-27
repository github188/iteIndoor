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

typedef enum
{
	UPDATE_DEVNO_E = 0X00,	
	UPDATE_NET_STATE_E = 0X01,	
	UPDATE_IPMODEL_STATE_E = 0X02,
	UPDATE_ALARM_E = 0X03,			
	UPDATE_CALL_E = 0X04,			
	UPDATE_MESSAGE_E = 0X05,		
	UPDATE_LYLY_E = 0X06,			
	UPDATE_UNDISTURB_E = 0X07,		
	UPDATE_JRLY_E = 0X08,			

}UPDATE_STATE_E;

/**
 * Custom event definition. from Custom0->Custom30
 */
typedef enum
{
	EVENT_CUSTOM0_STATE_UPDATE = ITU_EVENT_CUSTOM, 
	EVENT_CUSTOM1_SCREENSAVER, 						
	EVENT_CUSTOM2_RESER, 							
	EVENT_CUSTOM3_RESER, 								
	EVENT_CUSTOM4_RESER, 								
	EVENT_CUSTOM5_RESER, 								
	EVENT_CUSTOM6_RESER, 								
	EVENT_CUSTOM7_RESER, 								
	EVENT_CUSTOM8_RESER, 								
	EVENT_CUSTOM9_RESER, 								
	EVENT_CUSTOM10_RESER, 								
	EVENT_CUSTOM11_CALLREQUEST, 						
	EVENT_CUSTOM12_CALLOUT, 							
	EVENT_CUSTOM13_CALLIN, 								
	EVENT_CUSTOM14_MONITOR_LIST, 						
	EVENT_CUSTOM15_MONITOR, 							
	EVENT_CUSTOM16_RTSP_LIST, 							
	EVENT_CUSTOM17_RTSP_MONITOR, 						
	EVENT_CUSTOM18_KEY_SCENE,								// 快捷按键命令
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

typedef struct
{
	uint8 InterState;
	uint8 DataLen;
	char Buf[40];
}INTER_CALLBACK, *PINTER_CALLBACK;

/*************************************************
  Function:			af_callback_gui
  Description:
  Input:
  Output:			ï¿½ï¿½
  Return:
  Others:
  *************************************************/
int32 af_callback_gui(int32 Param1, int32 Param2);

/*************************************************
Function:		callrequest_state_callbak
Description:	呼叫请求回调函数
Input:
1. param1		状态
2. param2		私有数据
Output:			无
Return:			无
Others:			无
*************************************************/
void callrequest_state_callbak(uint32 param1, uint32 param2);

/*************************************************
  Function:			callout_state_callbak
  Description:
  Input:
  Output:			ï¿½ï¿½
  Return:
  Others:
  *************************************************/
void callout_state_callbak(uint32 param1, uint32 param2);

/*************************************************
  Function:			callin_state_callbak
  Description:
  Input:
  Output:			ï¿½ï¿½
  Return:
  Others:
  *************************************************/
void callin_state_callbak(uint32 param1, uint32 param2);

/*************************************************
Function:		monitor_list_state_callbak
Description:	获取监视列表回调函数
Input:
1. param1		状态
2. param2		私有数据
Output:			无
Return:			无
Others:			无
*************************************************/
void monitor_list_state_callbak(uint32 param1, uint32 param2);

/*************************************************
  Function:			monitor_state_callbak
  Description:
  Input:
  Output:			ï¿½ï¿½
  Return:
  Others:
  *************************************************/
void monitor_state_callbak(uint32 param1, uint32 param2);

/*************************************************
Function:		rtsp_list_state_callbak
Description: 	搜索列表回调函数
Input:
1. param1		状态
2. param2		私有数据
Output:			无
Return:			无
Others:			无
*************************************************/
void rtsp_list_state_callbak(uint32 param1, uint32 param2);

/*************************************************
  Function:			rtsp_state_callbak
  Description: 		ï¿½ï¿½ï¿½ï¿½×´Ì¬ï¿½Øµï¿½
  Input:
  1.param1
  2.param2
  Output:			ï¿½ï¿½
  Return:
  Others:
  *************************************************/
void rtsp_state_callbak(uint32 param1, uint32 param2);

/*************************************************
  Function:			show_sys_event_hint
  Description:
  Input:
  Output:			ï¿½ï¿½
  Return:
  Others:
  *************************************************/
void show_sys_event_hint(uint16 EventType);

/*************************************************
  Function:		ScenePredraw
  Description: 	ÖØ»æ
  Input:		
  Output:	
  Return:	
  Others:
*************************************************/
void ScenePredraw(int arg);

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
