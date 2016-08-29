#include "gui_include.h"

ITUActionFunction actionFunctions[] =
{
    //mainMenu
	"mainLayerOnEnter", mainLayerOnEnter,
	"mainLayerTimeoutOnTimer", mainLayerTimeoutOnTimer,
	"mainDistrubStatusOnChange", mainDistrubStatusOnChange,
	"mainSOSBtnOnPress", mainSOSBtnOnPress,
	"mainCoverFlowOnChanged", mainCoverFlowOnChanged,

	//elevatorCall
	"elevatorCallBtnOnClicked", elevatorCallBtnOnClicked,

	//information
	"informationLayerOnEnter", informationLayerOnEnter,
	"informationMSGListClicked", informationMSGListClicked,
	"informationBtnOnClicked", informationBtnOnClicked,
	"informationMsgBoxBtnOnClicked", informationMsgBoxBtnOnClicked,

	//photoMsg
	"photoMsgLayerOnEnter", photoMsgLayerOnEnter,
	"photoMsgListOnClicked", photoMsgListOnClicked,
	"photoMsgBtnOnClicked", photoMsgBtnOnClicked,
	"photoMsgMsgBoxBtnOnClicked", photoMsgMsgBoxBtnOnClicked,
	"photoMsgVideoDrawBtnOnClicked", photoMsgVideoDrawBtnOnClicked,
	"photoMsgVolTrackBarOnChanged", photoMsgVolTrackBarOnChanged,

	//cleanScreen
	"cleanScreenOnEnter", cleanScreenOnEnter,
	"cleanScreenTimeoutOnTimer", cleanScreenTimeoutOnTimer,

	//recorder
	"recorderLayerOnEnter", recorderLayerOnEnter,
	"recorderTimingOnTimer", recorderTimingOnTimer,
	"recorderMsgBoxBtnOnClicked", recorderMsgBoxBtnOnClicked,
	"recorderBtnOnClicked", recorderBtnOnClicked,
	"recorderVolTrackBarOnChanged", recorderVolTrackBarOnChanged,
	"recorderRecordRadioBoxChanged", recorderRecordRadioBoxChanged,

	//picmanager
	"picManagerLayerOnEnter", picManagerLayerOnEnter,
	"picManagerBtnOnClicked", picManagerBtnOnClicked,
	"picManagerMsgBoxBtnOnClicked", picManagerMsgBoxBtnOnClicked,
	"picManagerMiniPicBtnClicked", picManagerMiniPicBtnClicked,
	"picManagerPictureOnChanged", picManagerPictureOnChanged,

	// CallLayer
	"CallLayerOnEnter", CallLayerOnEnter,
	"CallLayerButtonOnMouseUp", CallLayerButtonOnMouseUp,
	"CallKeyBordButtonOnMouseUp", CallKeyBordButtonOnMouseUp,
	"CallKeyBordCalloutButtonOnMouseUp", CallKeyBordCalloutButtonOnMouseUp,
	"CallKeyBordDelButtonOnMouseUp", CallKeyBordDelButtonOnMouseUp,
	"CallLayerOnTimer", CallLayerOnTimer,
	"CallKeyBordDelButtonOnMouseLongPress", CallKeyBordDelButtonOnMouseLongPress,
	"CallRecordListButtonOnMouseUp", CallRecordListButtonOnMouseUp,
	"CallCeterListButtonOnMouseUp", CallCeterListButtonOnMouseUp,
	"CallRecordMSGButtonOnMouseUp", CallRecordMSGButtonOnMouseUp,
	"CallCallRequestState", CallCallRequestState,
	
	// BeCall
	"BeCallLayerOnEnter", BeCallLayerOnEnter,
	"BeCallLayerButtonOnMouseUp", BeCallLayerButtonOnMouseUp,
	"BeCallShowButtomBackgroundOnMouseUp", BeCallShowButtomBackgroundOnMouseUp,
	"BeCallButtomSoundButtonOnMouseUp", BeCallButtomSoundButtonOnMouseUp,
	"BeCallButtomSoundOnChanged", BeCallButtomSoundOnChanged,
	"BeCallLayerOnTimer", BeCallLayerOnTimer,
	"BeCallCallOutState", BeCallCallOutState,
	"BeCallCallInState", BeCallCallInState,

	// MonitorSearch
	"MonitorSearchLayerOnEnter", MonitorSearchLayerOnEnter,
	"MonitorSearchLayerButtonOnMouseUp", MonitorSearchLayerButtonOnMouseUp,
	"MonitorSearchListButtonOnMouseUp", MonitorSearchListButtonOnMouseUp,
	"MonitorSearchMSGButtonOnMouseUp", MonitorSearchMSGButtonOnMouseUp,
	"MonitorSearchListState", MonitorSearchListState,

	// Monitor
	"MonitorLayerOnEnter", MonitorLayerOnEnter,
	"MonitorLayerButtonOnMouseUp", MonitorLayerButtonOnMouseUp,
	"MonitorShowButtomBackgroundOnMouseUp", MonitorShowButtomBackgroundOnMouseUp,
	"MonitorButtomSoundButtonOnMouseUp", MonitorButtomSoundButtonOnMouseUp,
	"MonitorButtomSoundOnChanged", MonitorButtomSoundOnChanged,
	"MonitorLayerOnTimer", MonitorLayerOnTimer,
	"MonitorState", MonitorState,

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
	"SetNumKeyBordOnEnter", SetNumKeyBordOnEnter,
	"SetNumKeyBord1ButtonOnMouseUp", SetNumKeyBord1ButtonOnMouseUp,
	"SetNumKeyBordYesButtonOnMouseUp", SetNumKeyBordYesButtonOnMouseUp,
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
	"SetRingtoneOnLeave", SetRingtoneOnLeave,

	//Set_User
	"SetUserOnEnter", SetUserOnEnter,
	"SetYujingTimeButtonOnMouseUp", SetYujingTimeButtonOnMouseUp,
	"SetYujingTimeMsgRadioBoxOnMouseUp", SetYujingTimeMsgRadioBoxOnMouseUp,
	"SetGeliAndJufangButtonOnMouseUp", SetGeliAndJufangButtonOnMouseUp,

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

	//	Set_Dianti_IP
	"SetDiantiParamOnEnter", SetDiantiParamOnEnter,
	"SetDiantiParamIPButtonOnMouseUp", SetDiantiParamIPButtonOnMouseUp,

	// MsgFailHintSuccess
	"MsgFailHintSuccessOnEnter", MsgFailHintSuccessOnEnter,
	"MsgFailHintSuccessButtonOnMouseUp", MsgFailHintSuccessButtonOnMouseUp,
	"MsgFailHintSuccessLayerOnLeave", MsgFailHintSuccessLayerOnLeave,

	// Set_Project
	"SetProjectOnEnter", SetProjectOnEnter,
	"SetPrjProjectPwdButtonOnMouseUp", SetPrjProjectPwdButtonOnMouseUp,
	"SetPrjBackButtonOnMouseUp", SetPrjBackButtonOnMouseUp,

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


    NULL, NULL
};
