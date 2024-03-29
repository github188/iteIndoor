﻿#include "gui_include.h"

ITUActionFunction actionFunctions[] =
{
    //mainMenu
	"mainLayerOnEnter", mainLayerOnEnter,
	"mainLayerOnLeave", mainLayerOnLeave,
	"mainLayerTimeoutOnTimer", mainLayerTimeoutOnTimer,
	"mainDistrubStatusOnChange", mainDistrubStatusOnChange,
	"mainSOSBtnOnPress", mainSOSBtnOnPress,
	"mainCoverFlowOnChanged", mainCoverFlowOnChanged,
	"mainBtnOnClicked", mainBtnOnClicked,
	"mainMsgBoxBtnOnClicked", mainMsgBoxBtnOnClicked,

	//elevatorCall
	"elevatorCallBtnOnClicked", elevatorCallBtnOnClicked,

	//information
	"informationLayerOnEnter", informationLayerOnEnter,
	"informationLayerOnLeave", informationLayerOnLeave,
	"informationMSGListClicked", informationMSGListClicked,
	"informationBtnOnClicked", informationBtnOnClicked,
	"informationMsgBoxBtnOnClicked", informationMsgBoxBtnOnClicked,

	//photoMsg
	"photoMsgLayerOnEnter", photoMsgLayerOnEnter,
	"photoMsgLayerOnLeave", photoMsgLayerOnLeave,
	"photoMsgListOnClicked", photoMsgListOnClicked,
	"photoMsgBtnOnClicked", photoMsgBtnOnClicked,
	"photoMsgMsgBoxBtnOnClicked", photoMsgMsgBoxBtnOnClicked,
	"photoMsgVideoDrawBtnOnClicked", photoMsgVideoDrawBtnOnClicked,
	"photoMsgVolTrackBarOnChanged", photoMsgVolTrackBarOnChanged,

	//cleanScreen
	"cleanScreenLayerOnEnter", cleanScreenLayerOnEnter,
	"cleanScreenLayerOnLeave", cleanScreenLayerOnLeave,
	"cleanScreenTimeoutOnTimer", cleanScreenTimeoutOnTimer,

	// screen saver
	"ScreenSaverLayerOnEnter", ScreenSaverLayerOnEnter,
	"ScreenSaverLayerOnTimer", ScreenSaverLayerOnTimer,

	//recorder
	"recorderLayerOnEnter", recorderLayerOnEnter,
	"recorderLayerOnLeave", recorderLayerOnLeave,
	"recorderTimingOnTimer", recorderTimingOnTimer,
	"recorderMsgBoxBtnOnClicked", recorderMsgBoxBtnOnClicked,
	"recorderBtnOnClicked", recorderBtnOnClicked,
	"recorderVolTrackBarOnChanged", recorderVolTrackBarOnChanged,
	"recorderRecordRadioBoxChanged", recorderRecordRadioBoxChanged,

	//picmanager
	"picManagerLayerOnEnter", picManagerLayerOnEnter,
	"picManagerLayerOnLeave", picManagerLayerOnLeave,
	"picManagerBtnOnClicked", picManagerBtnOnClicked,
	"picManagerMsgBoxBtnOnClicked", picManagerMsgBoxBtnOnClicked,
	"picManagerMiniPicBtnClicked", picManagerMiniPicBtnClicked,
	"picManagerPictureOnChanged", picManagerPictureOnChanged,

	//tempdetection
	"tempDetectionDateMeterOnChanged", tempDetectionDateMeterOnChanged,

	// CallLayer
	"CallLayerOnEnter", CallLayerOnEnter,
	"CallLayerButtonOnMouseUp", CallLayerButtonOnMouseUp,
	"CallKeyBordButtonOnMouseUp", CallKeyBordButtonOnMouseUp,
	"CallKeyBordCalloutButtonOnMouseUp", CallKeyBordCalloutButtonOnMouseUp,
	"CallKeyBordDelButtonOnMouseUp", CallKeyBordDelButtonOnMouseUp,
	"CallLayerOnTimer", CallLayerOnTimer,
	"CallKeyBordDelButtonOnMouseLongPress", CallKeyBordDelButtonOnMouseLongPress,
	"CallRecordListButtonOnMouseUp", CallRecordListButtonOnMouseUp,
	"CallCenterListButtonOnMouseUp", CallCenterListButtonOnMouseUp,
	"CallRecordMSGButtonOnMouseUp", CallRecordMSGButtonOnMouseUp,
	"CallCallRequestState", CallCallRequestState,
	"CalloutManager", CalloutManager,
	"CallLayerOnLeave", CallLayerOnLeave,

	// BeCallLayer
	"BeCallLayerOnEnter", BeCallLayerOnEnter,
	"BeCallLayerButtonOnMouseUp", BeCallLayerButtonOnMouseUp,
	"BeCallShowButtomBackgroundOnMouseUp", BeCallShowButtomBackgroundOnMouseUp,
	"BeCallButtomSoundButtonOnMouseUp", BeCallButtomSoundButtonOnMouseUp,
	"BeCallButtomSoundOnChanged", BeCallButtomSoundOnChanged,
	"BeCallHideSoundOnMouseUp", BeCallHideSoundOnMouseUp,
	"BeCallLayerOnTimer", BeCallLayerOnTimer,
	"BeCallCallOutState", BeCallCallOutState,
	"BeCallCallInState", BeCallCallInState,
	"BeCallLayerKeyOnMouseUp", BeCallLayerKeyOnMouseUp,
	"BeCallLayerOnLeave", BeCallLayerOnLeave,
	
	// MonitorSearchLayer
	"MonitorSearchLayerOnEnter", MonitorSearchLayerOnEnter,
	"MonitorSearchLayerButtonOnMouseUp", MonitorSearchLayerButtonOnMouseUp,
	"MonitorSearchListButtonOnMouseUp", MonitorSearchListButtonOnMouseUp,
	"MonitorSearchMSGButtonOnMouseUp", MonitorSearchMSGButtonOnMouseUp,
	"MonitorSearchListState", MonitorSearchListState,
	"MonitorSearchLayerOnTimer", MonitorSearchLayerOnTimer,
	"MonitorSearchLayerOnLeave", MonitorSearchLayerOnLeave,

	// MonitorLayer
	"MonitorLayerOnEnter", MonitorLayerOnEnter,
	"MonitorLayerButtonOnMouseUp", MonitorLayerButtonOnMouseUp,
	"MonitorShowButtomBackgroundOnMouseUp", MonitorShowButtomBackgroundOnMouseUp,
	"MonitorButtomSoundButtonOnMouseUp", MonitorButtomSoundButtonOnMouseUp,
	"MonitorButtomSoundOnChanged", MonitorButtomSoundOnChanged,
	"MonitorHideSoundOnMouseUp", MonitorHideSoundOnMouseUp,
	"MonitorLayerOnTimer", MonitorLayerOnTimer,
	"MonitorState", MonitorState,
	"MonitorKeyOnMouseUp", MonitorKeyOnMouseUp,
	"MonitorChangeCameraOnMouseUp", MonitorChangeCameraOnMouseUp,
	"MonitorLayerOnLeave", MonitorLayerOnLeave,

	// RTSPSearch
	"RTSPSearchLayerStart", RTSPSearchLayerStart,
	"RTSPSearchLayerOnEnter", RTSPSearchLayerOnEnter,
	"RTSPSearchLayerButtonOnMouseUp", RTSPSearchLayerButtonOnMouseUp,
	"RTSPSearchListButtonOnMouseUp", RTSPSearchListButtonOnMouseUp,
	"RTSPSearchListState", RTSPSearchListState,
	"RTSPSearchLayerOnTimer", RTSPSearchLayerOnTimer,
	"RTSPSearchLayerOnLeave", RTSPSearchLayerOnLeave,

	// RTSPMonitorLayer
	"RTSPMonitorLayerOnEnter", RTSPMonitorLayerOnEnter,
	"RTSPMonitorLayerButtonOnMouseUp", RTSPMonitorLayerButtonOnMouseUp,
	"RTSPMonitorLayerOnTimer", RTSPMonitorLayerOnTimer,
	"RTSPMonitorState", RTSPMonitorState,
	"RTSPMonitorLayerShowBottomButtonOnMouseUp", RTSPMonitorLayerShowBottomButtonOnMouseUp,
	"RTSPMonitorLayerOnLeave", RTSPMonitorLayerOnLeave,

	// AurineJDLayer
	"AurineJDWin", AurineJDWin,
	"AurineJDLayerOnEnter", AurineJDLayerOnEnter,
	"AurineJDLayerButtonOnMouseUp", AurineJDLayerButtonOnMouseUp,
	"AurineJDLayerOnTimer", AurineJDLayerOnTimer,
	"AurineJDSceneButtonOnMouseUp", AurineJDSceneButtonOnMouseUp,
	"AurineJDLightUnControlButtonOnMouseUp", AurineJDLightUnControlButtonOnMouseUp,
	"AurineJDLightControlButtonOnMouseUp", AurineJDLightControlButtonOnMouseUp,
	"AurineJDAirButtonOnMouseUp", AurineJDAirButtonOnMouseUp,
	"AurineJDCurtainButtonOnMouseUp", AurineJDCurtainButtonOnMouseUp,
	"AurineJDPowerButtonOnMouseUp", AurineJDPowerButtonOnMouseUp,
	"AurineJDGasButtonOnMouseUp", AurineJDGasButtonOnMouseUp,
	"AurineJDLightOnChanged", AurineJDLightOnChanged,
	"AurineJDExitLightControlButtonOnMouseUp", AurineJDExitLightControlButtonOnMouseUp,
	"AurineJDCoverFlowOnChange", AurineJDCoverFlowOnChange,
	"AurineJDLayerOnLeave", AurineJDLayerOnLeave,

	//Alarm
	"AlarmOnEnter", AlarmOnEnter,
	"AlarmLayerTimeoutOnTimer", AlarmLayerTimeoutOnTimer,
	"AlarmBottomButtonOnMouseUp", AlarmBottomButtonOnMouseUp,
	"AlarmRightButtonOnMouseUp", AlarmRightButtonOnMouseUp,
	"AlarmFqSingleSetButtonOnMouseUp", AlarmFqSingleSetButtonOnMouseUp,
	"AlarmKeyBordButtonOnMouseUp", AlarmKeyBordButtonOnMouseUp,
	"AlarmKeyBordYesButtonOnMouseUp", AlarmKeyBordYesButtonOnMouseUp,
	"AlarmKeyBordDelButtonOnMouseLongPress", AlarmKeyBordDelButtonOnMouseLongPress,
	"AlarmKeyBordDianButtonOnMouseUp", AlarmKeyBordDianButtonOnMouseUp,
	"AlarmMsgHitSuccessRadioBoxOnMouseUp", AlarmMsgHitSuccessRadioBoxOnMouseUp,
	"AlarmLayerOnGoto", AlarmLayerOnGoto,

	//Set_Botton_Return_Layer
	"SetBottonReturnOnEnter", SetBottonReturnOnEnter,
	"SetBottonReturnOnReload", SetBottonReturnOnReload,
	"SetBottonReturnButtonOnPress", SetBottonReturnButtonOnPress,

	//Set_Menu_Layer
	"SetMenuOnEnter", SetMenuOnEnter,
	"SetMenuPwdButtonOnMouseUp", SetMenuPwdButtonOnMouseUp,

	//Set_Personality_PhotoFrame_Layer
	"SetPersonalityPhotoFrameOnEnter", SetPersonalityPhotoFrameOnEnter,
	"SetCloseLcdButtonOnMouseUp", SetCloseLcdButtonOnMouseUp,
	"SetLcdSaveButtonOnMouseUp", SetLcdSaveButtonOnMouseUp,
	"SetCloseLcdTimeMsgRadioBoxOnMouseUp", SetCloseLcdTimeMsgRadioBoxOnMouseUp,
	"SetSaveLcdTimeMsgRadioBoxOnMouseUp", SetSaveLcdTimeMsgRadioBoxOnMouseUp,

	//Set_Personality_Desk_Layer
	"SetPersonalityDeskOnEnter", SetPersonalityDeskOnEnter,
	"SetPersonalityDeskRadioBoxOnMouseUp", SetPersonalityDeskRadioBoxOnMouseUp,
	"RightPreviewButtonOnMouseUp", RightPreviewButtonOnMouseUp,
	"RightSetDeskButtonOnMouseUp", RightSetDeskButtonOnMouseUp,
	"SetPersonalityPreviewButtonOnMouseUp", SetPersonalityPreviewButtonOnMouseUp,

	//Set_Volume_Layer
	"SetVolumeOnEnter", SetVolumeOnEnter,
	"SetTalkVolumeButtonOnMouseUp", SetTalkVolumeButtonOnMouseUp,
	"SetRingingVolumeButtonOnMouseUp", SetRingingVolumeButtonOnMouseUp,
	"SetKeypadToneButtonOnMouseUp", SetKeypadToneButtonOnMouseUp,
	"SetVolumeMsgProgressBarOnChanged", SetVolumeMsgProgressBarOnChanged,
	"SetVolumeMSGTureButtonOnMouseUp", SetVolumeMSGTureButtonOnMouseUp,

	//Set_No_Disturb_Layer
	"SetNoDisturbOnEnter", SetNoDisturbOnEnter,
	"SetEnableNoDisturButtonOnMouseUp", SetEnableNoDisturButtonOnMouseUp,
	"SetNoDisturTimeBtnButtonOnMouseUp", SetNoDisturTimeBtnButtonOnMouseUp,
	"SetNoDisturbTimeRadioBoxOnMouseUp", SetNoDisturbTimeRadioBoxOnMouseUp,

	//Set_Numkeybord_Layer
	//"SetNumKeyBordOnEnter", SetNumKeyBordOnEnter,
	"SetNumKeyBord1ButtonOnMouseUp", SetNumKeyBord1ButtonOnMouseUp,
	"SetNumKeyBordYesButtonOnMouseUp", SetNumKeyBordYesButtonOnMouseUp,
	"SetNumKeyBordDelButtonOnMouseLongPress", SetNumKeyBordDelButtonOnMouseLongPress,
	"SetNumKeyBordLayerOnLeave", SetNumKeyBordLayerOnLeave,

	//Set_Password_Layer
	"SetPasswordOnEnter", SetPasswordOnEnter,
	"SetUserPwdButtonOnMouseUp", SetUserPwdButtonOnMouseUp,
	"SetServanPwdButtonOnMouseUp", SetServanPwdButtonOnMouseUp,
	"SetPasswordMsgTruePassBtnRadioBoxOnMouseUp", SetPasswordMsgTruePassBtnRadioBoxOnMouseUp,
	"SetPasswordMsgNewPassBtnRadioBoxOnMouseUp", SetPasswordMsgNewPassBtnRadioBoxOnMouseUp,
	"SetPasswordMsgTureButtonOnMouseUp", SetPasswordMsgTureButtonOnMouseUp,
	"SetPasswordMsgFalseButtonOnMouseUp", SetPasswordMsgFalseButtonOnMouseUp,
	//"SetPasswordMsgOKButtonOnMouseUp", SetPasswordMsgOKButtonOnMouseUp, 

	//Set_Language
	"SetSystemLanguageOnEnter", SetSystemLanguageOnEnter,
	"SetLanguageSimplifiedButtonOnMouseUp", SetLanguageSimplifiedButtonOnMouseUp,
	"SetLanguageTraditionalButtonOnMouseUp", SetLanguageTraditionalButtonOnMouseUp,
	"SetLanguageEnglishButtonOnMouseUp", SetLanguageEnglishButtonOnMouseUp,

	//Set_Link
	"SetLinkOnEnter", SetLinkOnEnter,
	"SetLinkAFButtonOnMouseUp", SetLinkAFButtonOnMouseUp,
	"SetLinkCallButtonOnMouseUp", SetLinkCallButtonOnMouseUp,
	"SetLinkJDButtonOnMouseUp", SetLinkJDButtonOnMouseUp,
	"SetAFLinkDealSaveButtonOnMouseUp", SetAFLinkDealSaveButtonOnMouseUp,
	"SetLinkEnableLylyButtonOnMouseUp", SetLinkEnableLylyButtonOnMouseUp,
	"SetCallLinkLylyModeButtonOnMouseUp", SetCallLinkLylyModeButtonOnMouseUp,
	"SetCallLinkDealSaveButtonOnMouseUp", SetCallLinkDealSaveButtonOnMouseUp,
	"SetLinkLylyModeMsgRadioBoxOnMouseUp", SetLinkLylyModeMsgRadioBoxOnMouseUp,
	"SetLinkLylyModeMsgExitButtonOnMouseUp", SetLinkLylyModeMsgExitButtonOnMouseUp,
	"SetJDLinkDealSaveButtonOnMouseUp", SetJDLinkDealSaveButtonOnMouseUp,
	"SetJDLinkModeSelectButtonOnMouseUp", SetJDLinkModeSelectButtonOnMouseUp,
	"SetLinkJDModeMsgRadioBoxOnMouseUp", SetLinkJDModeMsgRadioBoxOnMouseUp,
	"SetLinkJDModeMsgExitButtonOnMouseUp", SetLinkJDModeMsgExitButtonOnMouseUp,

	//Set_Alarm_Num
	"SetAlarmNumOnEnter", SetAlarmNumOnEnter,
	"SetAlarmNumButtonOnMouseUp", SetAlarmNumButtonOnMouseUp,

	//Set_Alarm_Area_list
	"SetAlarmAreaListOnEnter", SetAlarmAreaListOnEnter,
	"AlarmAreaButtonOnMouseUp", AlarmAreaButtonOnMouseUp,

	//Set_Data_Time
	"SetDataTimeOnEnter", SetDataTimeOnEnter,
	"SetDataButtonOnMouseUp", SetDataButtonOnMouseUp,
	"SetDataMsgSetOKButtonOnMouseUp", SetDataMsgSetOKButtonOnMouseUp,
	"SetTimeButtonOnMouseUp", SetTimeButtonOnMouseUp,
	"SetTimeMsgOKButtonOnMouseUp", SetTimeMsgOKButtonOnMouseUp,

	//Set_Ring
	"SetRingtoneOnEnter", SetRingtoneOnEnter,
	"SetCallRingtoneButtonOnMouseUp", SetCallRingtoneButtonOnMouseUp,
	"SetCallRingtoneListButtonOnMouseUp", SetCallRingtoneListButtonOnMouseUp,
	"SetCallRingMsgButtonOnMouseUp", SetCallRingMsgButtonOnMouseUp,
	"SetAlarmRingtoneButtonOnMouseUp", SetAlarmRingtoneButtonOnMouseUp,
	"SetAlarmRingtoneMsgListButtonOnMouseUp", SetAlarmRingtoneMsgListButtonOnMouseUp,
	"SetAlarmRingtoneMsgOkButtonOnMouseUp", SetAlarmRingtoneMsgOkButtonOnMouseUp,
	"SetHitRingtoneButtonOnMouseUp", SetHitRingtoneButtonOnMouseUp,
	"SetRingtonePromptMsgButtonOnMouseUp", SetRingtonePromptMsgButtonOnMouseUp,
	"SetRingtonePromptMsgTureButtonOnMouseUp", SetRingtonePromptMsgTureButtonOnMouseUp,
	"SetRingtonePromptMsgFalseButtonOnMouseUp", SetRingtonePromptMsgFalseButtonOnMouseUp,
	"SetLylyRingtoneButtonOnMouseUp", SetLylyRingtoneButtonOnMouseUp,
	"SetLylyRingRecMsgNoButtonOnMouseUp", SetLylyRingRecMsgNoButtonOnMouseUp,
	"SetLylyRingRecMsgYesButtonOnMouseUp", SetLylyRingRecMsgYesButtonOnMouseUp,
	"SetLylyRingRecMsgButtonOnMouseUp", SetLylyRingRecMsgButtonOnMouseUp,
	"SetLylyRingRecMsgRecStateButtonOnMouseUp", SetLylyRingRecMsgRecStateButtonOnMouseUp,
	"SetRingLylyRecTipTimeoutOnTimer", SetRingLylyRecTipTimeoutOnTimer,
	"SetRingtoneOnLeave", SetRingtoneOnLeave,

	//Set_User
	"SetUserOnEnter", SetUserOnEnter,
	"SetYujingTimeButtonOnMouseUp", SetYujingTimeButtonOnMouseUp,
	"SetYujingTimeMsgRadioBoxOnMouseUp", SetYujingTimeMsgRadioBoxOnMouseUp,
	"SetGeliAndJufangButtonOnMouseUp", SetGeliAndJufangButtonOnMouseUp,
	"SetUserJDModeButtonOnMouseUp", SetUserJDModeButtonOnMouseUp,
	"SetUserALarmModeButtonOnMouseUp", SetUserALarmModeButtonOnMouseUp,

	//Set_Alarm_Remote
	"SetAlarmRemoteOnEnter", SetAlarmRemoteOnEnter,
	"SetAlarmRemoteHomeButtonOnMouseUp", SetAlarmRemoteHomeButtonOnMouseUp,

	//Set_Alarm
	"SetAlarmOnEnter", SetAlarmOnEnter,
	"SetAlarmTimeButtonOnMouseUp", SetAlarmTimeButtonOnMouseUp,
	"SetAlarmTimeMsgRadioBoxOnMouseUp", SetAlarmTimeMsgRadioBoxOnMouseUp,
	"SetAlarmParamButtonOnMouseUp", SetAlarmParamButtonOnMouseUp,

	//Set_Net_Param
	"SetNetParamOnEnter", SetNetParamOnEnter,
	"SetNetParamHostButtonOnMouseUp", SetNetParamHostButtonOnMouseUp,
	"SetNetParamSeverButtonOnMouseUp", SetNetParamSeverButtonOnMouseUp,
	"SetNetParamManageButtonOnMouseUp", SetNetParamManageButtonOnMouseUp,
	"SetAllNetParamButtonOnMouseUp", SetAllNetParamButtonOnMouseUp,

	//Set_Ext_Module
	"SetExtModuleOnEnter", SetExtModuleOnEnter,
	"SetExtModuleListButtonOnMouseUp", SetExtModuleListButtonOnMouseUp,

	// Set_Devno_Rule
	"SetDevnoOnEnter", SetDevnoOnEnter,
	"SetDevnoListButtonOnMouseUp", SetDevnoListButtonOnMouseUp,

	// Set_Dianti_IP
	"SetDiantiParamOnEnter", SetDiantiParamOnEnter,
	"SetDiantiParamIPButtonOnMouseUp", SetDiantiParamIPButtonOnMouseUp,

	// MsgFailHintSuccess
	//"MsgFailHintSuccessOnEnter", MsgFailHintSuccessOnEnter,
	"MsgFailHintSuccessButtonOnMouseUp", MsgFailHintSuccessButtonOnMouseUp,
	"MsgFailHintSuccessLayerOnLeave", MsgFailHintSuccessLayerOnLeave,

	// Set_Project
	"SetProjectOnEnter", SetProjectOnEnter,
	"SetPrjProjectPwdButtonOnMouseUp", SetPrjProjectPwdButtonOnMouseUp,
	"SetPrjBackButtonOnMouseUp", SetPrjBackButtonOnMouseUp,
	"SetPrjJDButtonOnMouseUp", SetPrjJDButtonOnMouseUp,
	"SetPrjAlarmButtonOnMouseUp", SetPrjAlarmButtonOnMouseUp,

	// Set_Rtsp
	"SetRtspOnEnter", SetRtspOnEnter,
	"SetCameraButtonOnMouseUp", SetCameraButtonOnMouseUp,
	"SetRtspParamListButtonOnMouseUp", SetRtspParamListButtonOnMouseUp,
	"SetRtspFactoryMsgRadioBoxOnMouseUp", SetRtspFactoryMsgRadioBoxOnMouseUp,

	// Set_Alarm_Single_Attr
	"SetAlarmSingleAttrOnEnter", SetAlarmSingleAttrOnEnter,
	"SetAlarmSingleAttrListButtonOnMouseUp", SetAlarmSingleAttrListButtonOnMouseUp,
	"SetAlarmSingleAttrTimeMsgRadioBoxOnMouseUp", SetAlarmSingleAttrTimeMsgRadioBoxOnMouseUp,
	"SetAlarmSingleAttrAreaMsgSpriteOnMouseUp", SetAlarmSingleAttrAreaMsgSpriteOnMouseUp,
	"SetAlarmSingleAttrFinderMsgRadioBoxOnMouseUp", SetAlarmSingleAttrFinderMsgRadioBoxOnMouseUp,

	// Set_Net_Door
	"SetNetDoorOnEnter", SetNetDoorOnEnter,
	"SetNetDoorNumButtonOnMouseUp", SetNetDoorNumButtonOnMouseUp,
	//"SetNetDoorLayerOnTimer", SetNetDoorLayerOnTimer,

	// Msg_Link_OutTime
	"MsgLinkOutTimeOnTimer", MsgLinkOutTimeOnTimer,
	"MsgLinkOutTimeLayerOnLeave", MsgLinkOutTimeLayerOnLeave,

	// Set_NetDoor_NetParam
	"SetNetDoorNetParamOnEnter", SetNetDoorNetParamOnEnter,
	"SetNetDoorNetParamButtonOnMouseUp", SetNetDoorNetParamButtonOnMouseUp,
	"SetNetDoorNetParamLayerOnLeave", SetNetDoorNetParamLayerOnLeave,

	// Set_NetDoor_Lock
	"SetNetDoorLockOnEnter", SetNetDoorLockOnEnter,
	"SetNetDoorLockTypeButtonOnMouseUp", SetNetDoorLockTypeButtonOnMouseUp,
	"SetNetDoorLockTimeButtonOnMouseUp", SetNetDoorLockTimeButtonOnMouseUp,
	"SetNetDoorLockTypeMsgRadioBoxOnMouseUp", SetNetDoorLockTypeMsgRadioBoxOnMouseUp,

	// Set_NetDoor_Card
	"SetNetDoorCardOnEnter", SetNetDoorCardOnEnter,
	"SetNetDoorCardButtonOnMouseUp", SetNetDoorCardButtonOnMouseUp,

	// Set_NetDoor_Other
	"SetNetDoorOtherOnEnter", SetNetDoorOtherOnEnter,
	"SetNetDoorRemoteMoniterButtonOnMouseUp", SetNetDoorRemoteMoniterButtonOnMouseUp,

	// Set_NetDoor_Sysinfo
	"SetNetDoorSysInfoLayerOnEnter", SetNetDoorSysInfoLayerOnEnter,

	// Set_SysInfo
	"SetSysInfoOnEnter", SetSysInfoOnEnter,

	// Set_Storage 
	"SetStorageOnEnter", SetStorageOnEnter,
	"SetStorageCleanDataButtonOnMouseUp", SetStorageCleanDataButtonOnMouseUp,

	// Set_Ip_Protocol_Host
	"SetIpProtocolHostOnEnter", SetIpProtocolHostOnEnter,
	"SetIpProtocolBindNumButtonOnMouseUp", SetIpProtocolBindNumButtonOnMouseUp,
	"SetIpProtocolGetFJButtonOnMouseUp", SetIpProtocolGetFJButtonOnMouseUp,

	// Set_Ip_Protocol_Fenji
	"SetIpProtocolFenjiOnEnter", SetIpProtocolFenjiOnEnter,
	"SetIpProtocolBindNumFenjiButtonOnMouseUp", SetIpProtocolBindNumFenjiButtonOnMouseUp,
	"SetIpProtocolIPFenjiButtonOnMouseUp", SetIpProtocolIPFenjiButtonOnMouseUp,
	"SetIpProtocolBingFenjiButtonOnMouseUp", SetIpProtocolBingFenjiButtonOnMouseUp,
	"SetIpProtocolGetFJFenjiButtonOnMouseUp", SetIpProtocolGetFJFenjiButtonOnMouseUp,

	// Set_Jd
	"SetJDOnEnter", SetJDOnEnter,
	"SetJDCoverFlowButtonOnMouseUp", SetJDCoverFlowButtonOnMouseUp,
	"SetJDListCoverFlowButtonOnMouseUp", SetJDListCoverFlowButtonOnMouseUp,
	"SetJDFactoryButtonOnMouseUp", SetJDFactoryButtonOnMouseUp,

	//	Set_Jd_Param
	"SetJDParamOnEnter", SetJDParamOnEnter,
	"SetJdParamEnableButtonOnMouseUp", SetJdParamEnableButtonOnMouseUp,
	"SetJdParamDeviceNameButtonOnMouseUp", SetJdParamDeviceNameButtonOnMouseUp,
	"SetJdParamAddressButtonOnMouseUp", SetJdParamAddressButtonOnMouseUp,
	"SetJdParamAdjustButtonOnMouseUp", SetJdParamAdjustButtonOnMouseUp,
	"SetJdParamIRLearnButtonOnMouseUp", SetJdParamIRLearnButtonOnMouseUp,
	"SetJDDevNameSelectPageButtonOnMouseUp", SetJDDevNameSelectPageButtonOnMouseUp,
	"SetJdDeviceNameMsgTureButtonOnMouseUp", SetJdDeviceNameMsgTureButtonOnMouseUp,
	"SetJdDeviceNameMsgFalseButtonOnMouseUp", SetJdDeviceNameMsgFalseButtonOnMouseUp,
	"SetJdDeviNameAddrMsgButtonOnMouseUp", SetJdDeviNameAddrMsgButtonOnMouseUp,

	//Set_JD_KongTiao_Learn_IR
	"SetJDLearnIROnEnter", SetJDLearnIROnEnter,
	"SetJDLearnIRTypeButtonOnMouseUp", SetJDLearnIRTypeButtonOnMouseUp,
	"SetJDLearnIRTypeMsgListButtonOnMouseUp", SetJDLearnIRTypeMsgListButtonOnMouseUp,
	"SetJDLearnIRProgramButtonOnMouseUp", SetJDLearnIRProgramButtonOnMouseUp,
	"SetJDLearnIRProgramMsgTempListButtonOnMouseUp", SetJDLearnIRProgramMsgTempListButtonOnMouseUp,
	"SetJDLearnIRProgramMsgModeListButtonOnMouseUp", SetJDLearnIRProgramMsgModeListButtonOnMouseUp,
	"SetJDLearnIRStartButtonOnMouseUp", SetJDLearnIRStartButtonOnMouseUp,
	"SetJDLearnIRTestButtonOnMouseUp", SetJDLearnIRTestButtonOnMouseUp,

	//Set_Jd_Scence
	"SetJdScenceOnEnter", SetJdScenceOnEnter,
	"SetJDSceneListCoverFlowButtonOnMouseUp", SetJDSceneListCoverFlowButtonOnMouseUp,

	//Set_Jd_Scence_Param
	"SetJdScenceParamOnEnter", SetJdScenceParamOnEnter,
	"SetJdScenceParamEnableButtonOnMouseUp", SetJdScenceParamEnableButtonOnMouseUp,
	"SetJdScenceParamNameButtonOnMouseUp", SetJdScenceParamNameButtonOnMouseUp,
	"SetJdSceneNameMsgTureButtonOnMouseUp", SetJdSceneNameMsgTureButtonOnMouseUp,
	"SetJdSceneNameMsgFalseButtonOnMouseUp", SetJdSceneNameMsgFalseButtonOnMouseUp,
	"SetJdSceneNameMsgButtonOnMouseUp", SetJdSceneNameMsgButtonOnMouseUp,
	"SetJdSceneAddrMsgButtonOnMouseUp", SetJdSceneAddrMsgButtonOnMouseUp,
	"SetJdScenceParamAddressButtonOnMouseUp", SetJdScenceParamAddressButtonOnMouseUp,

	// Msg_Wait_Hit_Layer
	"MsgWaitHitLayerOnTimer", MsgWaitHitLayerOnTimer,

	// JiaDian_Yuyue_Layer
	"SetJdYuYueOnEnter", SetJdYuYueOnEnter,
	"SetJdYuYueAddButtonOnMouseUp", SetJdYuYueAddButtonOnMouseUp,
	"SetJdYuYueDelButtonOnMouseUp", SetJdYuYueDelButtonOnMouseUp,
	"SetDelJdYuYueListDataListButtonOnMouseUp", SetDelJdYuYueListDataListButtonOnMouseUp,
	"SetDelJdYuYueDelButtonOnMouseUp", SetDelJdYuYueDelButtonOnMouseUp,
	"SetJdYuYueListDataButtonOnMouseUp", SetJdYuYueListDataButtonOnMouseUp,

	// JiaDian_Yuyue_Param_Layer
	"SetJdYuYueParamOnEnter", SetJdYuYueParamOnEnter,
	"SetJdYuYueParamYuyueTypeButtonOnMouseUp", SetJdYuYueParamYuyueTypeButtonOnMouseUp,
	"SetJdYuyueTypeRadioBoxOnMouseUp", SetJdYuyueTypeRadioBoxOnMouseUp,
	"SetJdYuYueParamDeviceButtonOnMouseUp", SetJdYuYueParamDeviceButtonOnMouseUp,
	"SetJdYuyueDeviceListButtonOnMouseUp", SetJdYuyueDeviceListButtonOnMouseUp,
	"SetJdYuYueParamOperaButtonOnMouseUp", SetJdYuYueParamOperaButtonOnMouseUp,
	"SetJdYuYueParamBrightnessButtonOnMouseUp", SetJdYuYueParamBrightnessButtonOnMouseUp,
	"SetJdYuYueParamBrightnessProgressBarOnChanged",SetJdYuYueParamBrightnessProgressBarOnChanged,
	"SetJdYuYueParamBrightnessTureButtonOnMouseUp", SetJdYuYueParamBrightnessTureButtonOnMouseUp,
	"SetJdYuYueParamTimeTypeButtonOnMouseUp", SetJdYuYueParamTimeTypeButtonOnMouseUp,
	"SetJdYuYueParamTimeTypeListRadioBoxOnMouseUp", SetJdYuYueParamTimeTypeListRadioBoxOnMouseUp,
	"SetJdYuYueParamTimeDateButtonOnMouseUp", SetJdYuYueParamTimeDateButtonOnMouseUp,
	"SetJdYuYueParamTimeDateOKButtonOnMouseUp", SetJdYuYueParamTimeDateOKButtonOnMouseUp,
	"SetJdYuYueParamTimeTimeButtonOnMouseUp", SetJdYuYueParamTimeTimeButtonOnMouseUp,
	"SetJdYuYueParamTimeTimeOKButtonOnMouseUp", SetJdYuYueParamTimeTimeOKButtonOnMouseUp,
	"SetJdYuYueParamTimeWeekButtonOnMouseUp", SetJdYuYueParamTimeWeekButtonOnMouseUp,
	"SetJdYuYueParamTimeWeekListButtonOnMouseUp", SetJdYuYueParamTimeWeekListButtonOnMouseUp,

    NULL, NULL
};
