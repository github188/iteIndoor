/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set.h
Author:     	zxc
Version:
Date: 		2016-07-30
Description:	放置设置所有的外部函数
History:
1. Date:
Author:
Modification:
*************************************************/
#ifndef _LAYER_SET_H_
#define _LAYER_SET_H_

#ifdef __cplusplus
extern "C" {
#endif

	#include "gui_include.h"

	//设置类键盘layer枚举定义
	//键盘输入框显示明文密文
	typedef enum
	{
		EXPRESS_CHAR = 32,					// 明文				
		CIPHERTEXT = 33,					// 密文
	}NUM_CAHR_TYPE;

	//键盘显示“.”“X”
	typedef enum
	{
		CANCEL_BTN = 0,					// X		
		SPOT_BTN = 1,					// .
	}SPRITE_KEYBORD_BTN_TYPE;


	//Set_Botton_Return_Layer
	bool SetBottonReturnOnEnter(ITUWidget* widget, char* param);
	bool SetBottonReturnOnReload(ITUWidget* widget, char* param);
	bool SetBottonReturnButtonOnPress(ITUWidget* widget, char* param);

	//Set_Menu_Layer
	bool SetMenuOnEnter(ITUWidget* widget, char* param);
	bool SetMenuPwdButtonOnMouseUp(ITUWidget* widget, char* param);

	//Set_Personality_PhotoFrame_Layer
	bool SetPersonalityPhotoFrameOnEnter(ITUWidget* widget, char* param);
	bool SetCloseLcdButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetLcdSaveButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetCloseLcdTimeMsgRadioBoxOnMouseUp(ITUWidget* widget, char* param);
	bool SetSaveLcdTimeMsgRadioBoxOnMouseUp(ITUWidget* widget, char* param);
	void SetPersonalityPhotoFrameLayerOnReturn(void);

	//Set_Personality_Desk_Layer
	bool SetPersonalityDeskOnEnter(ITUWidget* widget, char* param);
	bool SetPersonalityDeskRadioBoxOnMouseUp(ITUWidget* widget, char* param);
	bool RightPreviewButtonOnMouseUp(ITUWidget* widget, char* param);
	bool RightSetDeskButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetPersonalityPreviewButtonOnMouseUp(ITUWidget* widget, char* param);

	//Set_Volume_Layer
	bool SetVolumeOnEnter(ITUWidget* widget, char* param);
	bool SetTalkVolumeButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetRingingVolumeButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetKeypadToneButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetVolumeMsgProgressBarOnChanged(ITUWidget* widget, char* param);
	bool SetVolumeMSGTureButtonOnMouseUp(ITUWidget* widget, char* param);
	void SetVolumeLayerOnReturn(void);

	//Set_No_Disturb_Layer
	bool SetNoDisturbOnEnter(ITUWidget* widget, char* param);
	bool SetEnableNoDisturButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetNoDisturTimeBtnButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetNoDisturbTimeRadioBoxOnMouseUp(ITUWidget* widget, char* param);
	void SetNoDisturLayerOnReturn(void);

	//Set_Numkeybord_Layer
	bool SetNumKeyBordOnEnter(ITUWidget* widget, char* param);
	bool SetNumKeyBord1ButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetNumKeyBordYesButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetNumKeyBordLayerOnLeave(ITUWidget* widget, char* param);
	void KeybordLayerOnShow(ITULayer* widget, PASS_TYPE pass_type, int text_maxlen, NUM_CAHR_TYPE text_flag, SPRITE_KEYBORD_BTN_TYPE sprite_flag, char * buf);
	void SetNumKeyBordLayerOnReturn(void);

	//Set_Password_Layer
	bool SetPasswordOnEnter(ITUWidget* widget, char* param);
	bool SetUserPwdButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetServanPwdButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetPasswordMsgTruePassBtnRadioBoxOnMouseUp(ITUWidget* widget, char* param);
	bool SetPasswordMsgNewPassBtnRadioBoxOnMouseUp(ITUWidget* widget, char* param);
	bool SetPasswordMsgTureButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetPasswordMsgFalseButtonOnMouseUp(ITUWidget* widget, char* param);
	void SetPasswordLayerOnReturn(void);
	void UpdataPassType(int data);

	//Set_Language
	bool SetSystemLanguageOnEnter(ITUWidget* widget, char* param);
	bool SetLanguageSimplifiedButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetLanguageTraditionalButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetLanguageEnglishButtonOnMouseUp(ITUWidget* widget, char* param);
	void SetSystemLanguageLayerOnReturn(void);

	//Set_Link
	bool SetLinkOnEnter(ITUWidget* widget, char* param);
	bool SetLinkAFButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetLinkCallButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetLinkJDButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetAFLinkDealSaveButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetLinkEnableLylyButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetCallLinkLylyModeButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetCallLinkDealSaveButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetLinkLylyModeMsgRadioBoxOnMouseUp(ITUWidget* widget, char* param);
	bool SetLinkLylyModeMsgExitButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetJDLinkDealSaveButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetJDLinkModeSelectButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetLinkJDModeMsgRadioBoxOnMouseUp(ITUWidget* widget, char* param);
	bool SetLinkJDModeMsgExitButtonOnMouseUp(ITUWidget* widget, char* param);
	void SetLinkLayerOnReturn(void);

	//Set_Alarm_Num
	bool SetAlarmNumOnEnter(ITUWidget* widget, char* param);
	bool SetAlarmNumButtonOnMouseUp(ITUWidget* widget, char* param);

	//Set_Alarm_Area_list
	bool SetAlarmAreaListOnEnter(ITUWidget* widget, char* param);
	bool AlarmAreaButtonOnMouseUp(ITUWidget* widget, char* param);
	uint32 SetAlarmAreaList(uint8 type);
	void SetAlarmAreaListLayerOnReturn(void);

	//Set_Data_Time
	bool SetDataTimeOnEnter(ITUWidget* widget, char* param);
	bool SetDataButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetDataMsgSetOKButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetTimeButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetTimeMsgOKButtonOnMouseUp(ITUWidget* widget, char* param);
	void SetDataTimeLayerOnReturn(void);

	//Set_Ring
	bool SetRingtoneOnEnter(ITUWidget* widget, char* param);
	bool SetCallRingtoneButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetCallRingtoneListButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetCallRingMsgButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetAlarmRingtoneButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetAlarmRingtoneMsgListButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetAlarmRingtoneMsgOkButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetHitRingtoneButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetRingtonePromptMsgButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetRingtonePromptMsgTureButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetRingtonePromptMsgFalseButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetLylyRingtoneButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetRingtoneOnLeave(ITUWidget* widget, char* param);
	void SetRingtoneOnReturn(void);

	//Set_User
	bool SetUserOnEnter(ITUWidget* widget, char* param);
	bool SetYujingTimeButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetYujingTimeMsgRadioBoxOnMouseUp(ITUWidget* widget, char* param);
	bool SetGeliAndJufangButtonOnMouseUp(ITUWidget* widget, char* param);
	void SetUserLayerOnReturn(void);

	//Set_Alarm_Remote
	bool SetAlarmRemoteOnEnter(ITUWidget* widget, char* param);
	bool SetAlarmRemoteHomeButtonOnMouseUp(ITUWidget* widget, char* param);

	//Set_Alarm
	bool SetAlarmOnEnter(ITUWidget* widget, char* param);
	bool SetAlarmTimeButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetAlarmTimeMsgRadioBoxOnMouseUp(ITUWidget* widget, char* param);
	bool SetAlarmParamButtonOnMouseUp(ITUWidget* widget, char* param);
	void SetAlarmLayerOnReturn(void);

	// Set_Net_Param
	bool SetNetParamOnEnter(ITUWidget* widget, char* param);
	bool SetNetParamHostButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetNetParamSeverButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetNetParamManageButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetAllNetParamButtonOnMouseUp(ITUWidget* widget, char* param);
	void SetNetParamLayerOnReturn(void);

	// Set_Ext_Module
	bool SetExtModuleOnEnter(ITUWidget* widget, char* param);
	bool SetExtModuleListButtonOnMouseUp(ITUWidget* widget, char* param);

	// Set_Devno_Rule
	bool SetDevnoOnEnter(ITUWidget* widget, char* param);
	bool SetDevnoListButtonOnMouseUp(ITUWidget* widget, char* param);
	void SetDevnoLayerOnReturn(void);

	// Set_Diati_IP
	bool SetDiantiParamOnEnter(ITUWidget* widget, char* param);
	bool SetDiantiParamIPButtonOnMouseUp(ITUWidget* widget, char* param);

	// MsgFailHintSuccess
	bool MsgFailHintSuccessOnEnter(ITUWidget* widget, char* param);
	bool MsgFailHintSuccessButtonOnMouseUp(ITUWidget* widget, char* param);
	bool MsgFailHintSuccessLayerOnLeave(ITUWidget* widget, char* param);
	void ShowMsgFailHintSuccessLayer(uint8 spriteflag, uint32 id, uint8 calendarflag);

	// Set_Project
	bool SetProjectOnEnter(ITUWidget* widget, char* param);
	bool SetPrjProjectPwdButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetPrjBackButtonOnMouseUp(ITUWidget* widget, char* param);

	// Set_Rtsp
	bool SetRtspOnEnter(ITUWidget* widget, char* param);
	bool SetCameraButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetRtspParamListButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetRtspFactoryMsgRadioBoxOnMouseUp(ITUWidget* widget, char* param);
	void SetRtspLayerOnReturn(void);

	// Set_Alarm_Single_Attr
	bool SetAlarmSingleAttrOnEnter(ITUWidget* widget, char* param);
	bool SetAlarmSingleAttrListButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetAlarmSingleAttrTimeMsgRadioBoxOnMouseUp(ITUWidget* widget, char* param);
	bool SetAlarmSingleAttrAreaMsgSpriteOnMouseUp(ITUWidget* widget, char* param);
	bool SetAlarmSingleAttrFinderMsgRadioBoxOnMouseUp(ITUWidget* widget, char* param);
	uint32 SetAlarmSingleAttr(uint8 num);
	void SetAlarmSingleAttrLayerOnReturn(void);

	// Set_Net_Door
	bool SetNetDoorOnEnter(ITUWidget* widget, char* param);
	bool SetNetDoorNumButtonOnMouseUp(ITUWidget* widget, char* param);
	void SetNetDoorLayerOnReturn();

	// Msg_Link_OutTime
	bool MsgLinkOutTimeOnTimer(ITUWidget* widget, char* param);
	bool MsgLinkOutTimeLayerOnLeave(ITUWidget* widget, char* param);
	void MsgLinkOutTimeOnShow();

	// Set_NetDoor_NetParam
	bool SetNetDoorNetParamOnEnter(ITUWidget* widget, char* param);
	bool SetNetDoorNetParamButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetNetDoorNetParamLayerOnLeave(ITUWidget* widget, char* param);
	void set_netdoorparam_Page(int pageIndex);
	void SetNetDoorNetparamLayerOnReturn();

	// Set_NetDoor_Lock
	bool SetNetDoorLockOnEnter(ITUWidget* widget, char* param);
	bool SetNetDoorLockTypeButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetNetDoorLockTimeButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetNetDoorLockTypeMsgRadioBoxOnMouseUp(ITUWidget* widget, char* param);
	void SetNetDoorLockLayerOnReturn();

	// Set_NetDoor_Card
	bool SetNetDoorCardOnEnter(ITUWidget* widget, char* param);
	bool SetNetDoorCardButtonOnMouseUp(ITUWidget* widget, char* param);

	// Set_NetDoor_Other
	bool SetNetDoorOtherOnEnter(ITUWidget* widget, char* param);
	bool SetNetDoorRemoteMoniterButtonOnMouseUp(ITUWidget* widget, char* param);
	void SetNetDoorOtherLayerOnReturn();

	// Set_NetDoor_Sysinfo
	bool SetNetDoorSysInfoLayerOnEnter(ITUWidget* widget, char* param);

	// Set_SysInfo
	bool SetSysInfoOnEnter(ITUWidget* widget, char* param);

	// Set_Storage 
	bool SetStorageOnEnter(ITUWidget* widget, char* param);
	bool SetStorageCleanDataButtonOnMouseUp(ITUWidget* widget, char* param);

	// Set_Ip_Protocol_Host
	bool SetIpProtocolHostOnEnter(ITUWidget* widget, char* param);
	bool SetIpProtocolBindNumButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetIpProtocolGetFJButtonOnMouseUp(ITUWidget* widget, char* param);

	// Set_Ip_Protocol_Fenji
	bool SetIpProtocolFenjiOnEnter(ITUWidget* widget, char* param);
	bool SetIpProtocolBindNumFenjiButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetIpProtocolIPFenjiButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetIpProtocolBingFenjiButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetIpProtocolGetFJFenjiButtonOnMouseUp(ITUWidget* widget, char* param);

	// Set_Jd
	bool SetJDOnEnter(ITUWidget* widget, char* param);
	bool SetJDCoverFlowButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetJDListCoverFlowButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetJDFactoryButtonOnMouseUp(ITUWidget* widget, char* param);
	void SetJDLayerOnReturn(void);

	//Set_JD_param
	bool SetJDParamOnEnter(ITUWidget* widget, char* param);
	bool SetJdParamEnableButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetJdParamDeviceNameButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetJdParamAddressButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetJdParamAdjustButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetJdParamIRLearnButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetJDDevNameSelectPageButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetJdDeviceNameMsgTureButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetJdDeviceNameMsgFalseButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetJdDeviNameAddrMsgButtonOnMouseUp(ITUWidget* widget, char* param);
	uint32 SetJDParam(PJD_DEV_LIST_INFO devlist, AU_JD_DEV_TYPE devtype, int index);
	void SetJDParamLayerOnReturn(void);

	//Set_JD_KongTiao_Learn_IR
	bool SetJDLearnIROnEnter(ITUWidget* widget, char* param);
	bool SetJDLearnIRTypeButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetJDLearnIRTypeMsgListButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetJDLearnIRProgramButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetJDLearnIRProgramMsgTempListButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetJDLearnIRProgramMsgModeListButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetJDLearnIRStartButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetJDLearnIRTestButtonOnMouseUp(ITUWidget* widget, char* param);
	uint32 SetJDLearnIRParam(JD_DEV_INFO devinfo);
	void SetJDLearnIRLayerOnReturn(void);

	//Set_Jd_Scence
	bool SetJdScenceOnEnter(ITUWidget* widget, char* param);
	bool SetJDSceneListCoverFlowButtonOnMouseUp(ITUWidget* widget, char* param);

	//Set_Jd_Scence_Param
	bool SetJdScenceParamOnEnter(ITUWidget* widget, char* param);
	bool SetJdScenceParamEnableButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetJdScenceParamNameButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetJdSceneNameMsgTureButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetJdSceneNameMsgFalseButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetJdSceneNameMsgButtonOnMouseUp(ITUWidget* widget, char* param);
	bool SetJdSceneAddrMsgButtonOnMouseUp(ITUWidget* widget, char* param);
	uint32 SetAurinJDSceneList(uint8 Index, PJD_SCENE_INFO_LIST sencelist);
	void SetJDSceneParamLayerOnReturn(void);


#ifdef __cplusplus
}
#endif

#endif 
