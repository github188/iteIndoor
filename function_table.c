#include "ite/itu.h"
#include "gui_include.h"


// CallLayer
extern bool CallLayerOnEnter(ITUWidget* widget, char* param);
extern bool CallLayerButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool CallKeyBordButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool CallKeyBordCalloutButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool CallKeyBordDelButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool CallLayerOnTimer(ITUWidget* widget, char* param);
extern bool CallKeyBordDelButtonOnMouseLongPress(ITUWidget* widget, char* param);
extern bool CallRecordListButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool CallCeterListButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool CallRecordMSGButtonOnMouseUp(ITUWidget* widget, char* param);

// MonitorSearch
extern bool MonitorSearchLayerOnEnter(ITUWidget* widget, char* param);
extern bool MonitorSearchLayerButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool MonitorSearchListButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool MonitorSearchMSGButtonOnMouseUp(ITUWidget* widget, char* param);

//Set_Botton_Return_Layer
extern bool SetBottonReturnOnEnter(ITUWidget* widget, char* param);
extern bool SetBottonReturnOnReload(ITUWidget* widget, char* param);
extern bool SetBottonReturnButtonOnPress(ITUWidget* widget, char* param);

//Set_Menu_Layer
extern bool SetMenuOnEnter(ITUWidget* widget, char* param);
extern bool SetMenuPwdButtonOnMouseUp(ITUWidget* widget, char* param);

//Set_Personality_PhotoFrame_Layer
extern bool SetPersonalityPhotoFrameOnEnter(ITUWidget* widget, char* param);
extern bool SetCloseLcdButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetLcdSaveButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetCloseLcdTimeMsgRadioBoxOnMouseUp(ITUWidget* widget, char* param);
extern bool SetSaveLcdTimeMsgRadioBoxOnMouseUp(ITUWidget* widget, char* param);
extern void SetPersonalityPhotoFrameLayerOnReturn(void);

//Set_Volume_Layer
extern bool SetVolumeOnEnter(ITUWidget* widget, char* param);
extern bool SetTalkVolumeButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetRingingVolumeButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetKeypadToneButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetVolumeMsgProgressBarOnChanged(ITUWidget* widget, char* param);
extern bool SetVolumeMSGTureButtonOnMouseUp(ITUWidget* widget, char* param);
extern void SetVolumeLayerOnReturn(void);

//Set_No_Disturb_Layer
extern bool SetNoDisturbOnEnter(ITUWidget* widget, char* param);
extern bool SetEnableNoDisturButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetNoDisturTimeBtnButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetNoDisturbTimeRadioBoxOnMouseUp(ITUWidget* widget, char* param);
extern void SetNoDisturLayerOnReturn(void);

//Set_Numkeybord_Layer
extern bool SetNumKeyBordOnEnter(ITUWidget* widget, char* param);
extern bool SetNumKeyBord1ButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetNumKeyBordYesButtonOnMouseUp(ITUWidget* widget, char* param);
extern void KeybordLayerOnShow(ITULayer* widget, PASS_TYPE pass_type, int text_maxlen, uint32 text_flag, int sprite_flag, char * buf);
extern void SetNumKeyBordLayerOnReturn(void);

//Set_Password_Layer
extern bool SetPasswordOnEnter(ITUWidget* widget, char* param);
extern bool SetUserPwdButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetServanPwdButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetPasswordMsgTruePassBtnRadioBoxOnMouseUp(ITUWidget* widget, char* param);
extern bool SetPasswordMsgNewPassBtnRadioBoxOnMouseUp(ITUWidget* widget, char* param);
extern bool SetPasswordMsgTureButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetPasswordMsgFalseButtonOnMouseUp(ITUWidget* widget, char* param);
//extern bool SetPasswordMsgOKButtonOnMouseUp(ITUWidget* widget, char* param);
extern void SetPasswordLayerOnReturn(void);
extern void UpdataPassType(int data);

//Set_Language
extern bool SetSystemLanguageOnEnter(ITUWidget* widget, char* param);
extern bool SetLanguageSimplifiedButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetLanguageTraditionalButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetLanguageEnglishButtonOnMouseUp(ITUWidget* widget, char* param);

//Set_Link
extern bool SetLinkOnEnter(ITUWidget* widget, char* param);
extern bool SetLinkAFButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetLinkCallButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetLinkJDButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetAFLinkDealSaveButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetLinkEnableLylyButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetCallLinkLylyModeButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetCallLinkDealSaveButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetLinkLylyModeMsgRadioBoxOnMouseUp(ITUWidget* widget, char* param);
extern bool SetLinkLylyModeMsgExitButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetJDLinkDealSaveButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetJDLinkModeSelectButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetLinkJDModeMsgRadioBoxOnMouseUp(ITUWidget* widget, char* param);
extern bool SetLinkJDModeMsgExitButtonOnMouseUp(ITUWidget* widget, char* param);
extern void SetLinkLayerOnReturn(void);

//Set_Alarm_Num
extern bool SetAlarmNumOnEnter(ITUWidget* widget, char* param);
extern bool SetAlarmNumButtonOnMouseUp(ITUWidget* widget, char* param);

//Set_Alarm_Area_list
extern bool SetAlarmAreaListOnEnter(ITUWidget* widget, char* param);
extern bool AlarmAreaButtonOnMouseUp(ITUWidget* widget, char* param);
extern uint32 SetAlarmAreaList(uint8 type);
extern void SetAlarmAreaListLayerOnReturn(void);

//Set_Data_Time
extern bool SetDataTimeOnEnter(ITUWidget* widget, char* param);
extern bool SetDataButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetDataMsgSetOKButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetTimeButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetTimeMsgOKButtonOnMouseUp(ITUWidget* widget, char* param);
extern void SetDataTimeLayerOnReturn(void);

//Set_Ring
extern bool SetRingtoneOnEnter(ITUWidget* widget, char* param);
extern bool SetCallRingtoneButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetAlarmRingtoneButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetHitRingtoneButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetLylyRingtoneButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetRingtoneOnLeave(ITUWidget* widget, char* param);
extern bool SetRingtonePromptMsgButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetRingtonePromptMsgTureButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetRingtonePromptMsgFalseButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetCallRingMsgButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetCallRingMsgYesButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetCallRingMsgNoButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetCallRingtoneListButtonOnMouseUp(ITUWidget* widget, char* param);
extern void SetRingtoneOnReturn(void);

//Set_User
extern bool SetUserOnEnter(ITUWidget* widget, char* param);
extern bool SetYujingTimeButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetYujingTimeMsgRadioBoxOnMouseUp(ITUWidget* widget, char* param);
extern bool SetGeliAndJufangButtonOnMouseUp(ITUWidget* widget, char* param);
extern void SetUserLayerOnReturn(void);

//Set_Alarm_Remote
extern bool SetAlarmRemoteOnEnter(ITUWidget* widget, char* param);
extern bool SetAlarmRemoteHomeButtonOnMouseUp(ITUWidget* widget, char* param);

//Set_Alarm
extern bool SetAlarmOnEnter(ITUWidget* widget, char* param);
extern bool SetAlarmTimeButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetAlarmTimeMsgRadioBoxOnMouseUp(ITUWidget* widget, char* param);
extern bool SetAlarmParamButtonOnMouseUp(ITUWidget* widget, char* param);
extern void SetAlarmLayerOnReturn(void);

// Set_Net_Param
extern bool SetNetParamOnEnter(ITUWidget* widget, char* param);
extern bool SetNetParamHostButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetNetParamSeverButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetNetParamManageButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetAllNetParamButtonOnMouseUp(ITUWidget* widget, char* param);
extern void SetNetParamLayerOnReturn(void);

// Set_Ext_Module
extern bool SetExtModuleOnEnter(ITUWidget* widget, char* param);
extern bool SetExtModuleListButtonOnMouseUp(ITUWidget* widget, char* param);

// Set_Devno_Rule
extern bool SetDevnoOnEnter(ITUWidget* widget, char* param);
extern bool SetDevnoListButtonOnMouseUp(ITUWidget* widget, char* param);
extern void SetDevnoLayerOnReturn(void);

// Set_Diati_IP
extern bool SetDiantiParamOnEnter(ITUWidget* widget, char* param);
extern bool SetDiantiParamIPButtonOnMouseUp(ITUWidget* widget, char* param);

// MsgFailHintSuccess
extern bool MsgFailHintSuccessOnEnter(ITUWidget* widget, char* param);
extern bool MsgFailHintSuccessButtonOnMouseUp(ITUWidget* widget, char* param);
extern void ShowMsgFailHintSuccessLayer(uint8 spriteflag, uint32 id);

// Set_Project
extern bool SetProjectOnEnter(ITUWidget* widget, char* param);
extern bool SetPrjProjectPwdButtonOnMouseUp(ITUWidget* widget, char* param);

// Set_Rtsp
extern bool SetRtspOnEnter(ITUWidget* widget, char* param);
extern bool SetCameraButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetRtspParamListButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetRtspFactoryMsgRadioBoxOnMouseUp(ITUWidget* widget, char* param);
extern void SetRtspLayerOnReturn(void);

// Set_Alarm_Single_Attr
extern bool SetAlarmSingleAttrOnEnter(ITUWidget* widget, char* param);
extern bool SetAlarmSingleAttrListButtonOnMouseUp(ITUWidget* widget, char* param);
extern bool SetAlarmSingleAttrTimeMsgRadioBoxOnMouseUp(ITUWidget* widget, char* param);
extern uint32 SetAlarmSingleAttr(uint8 num);
extern void SetAlarmSingleAttrLayerOnReturn(void);


ITUActionFunction actionFunctions[] =
{
    //mainMenu
	"mainMenuLayerOnEnter", mainMenuLayerOnEnter,
	"mainDistrubStatusOnChange", mainDistrubStatusOnChange,

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
	
	// MonitorSearch
	"MonitorSearchLayerOnEnter", MonitorSearchLayerOnEnter,
	"MonitorSearchLayerButtonOnMouseUp", MonitorSearchLayerButtonOnMouseUp,
	"MonitorSearchListButtonOnMouseUp", MonitorSearchListButtonOnMouseUp,
	"MonitorSearchMSGButtonOnMouseUp", MonitorSearchMSGButtonOnMouseUp,

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
	"SetAlarmRingtoneButtonOnMouseUp", SetAlarmRingtoneButtonOnMouseUp,
	"SetHitRingtoneButtonOnMouseUp", SetHitRingtoneButtonOnMouseUp,
	"SetLylyRingtoneButtonOnMouseUp", SetLylyRingtoneButtonOnMouseUp,
	"SetRingtoneOnLeave", SetRingtoneOnLeave,
	"SetRingtonePromptMsgButtonOnMouseUp", SetRingtonePromptMsgButtonOnMouseUp,
	"SetRingtonePromptMsgTureButtonOnMouseUp", SetRingtonePromptMsgTureButtonOnMouseUp,
	"SetRingtonePromptMsgFalseButtonOnMouseUp", SetRingtonePromptMsgFalseButtonOnMouseUp,
	"SetCallRingMsgButtonOnMouseUp", SetCallRingMsgButtonOnMouseUp,
	"SetCallRingMsgYesButtonOnMouseUp", SetCallRingMsgYesButtonOnMouseUp,
	"SetCallRingMsgNoButtonOnMouseUp", SetCallRingMsgNoButtonOnMouseUp,
	"SetCallRingtoneListButtonOnMouseUp", SetCallRingtoneListButtonOnMouseUp,

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

	// Set_Project
	"SetProjectOnEnter", SetProjectOnEnter,
	"SetPrjProjectPwdButtonOnMouseUp", SetPrjProjectPwdButtonOnMouseUp,

	// Set_Rtsp
	"SetRtspOnEnter", SetRtspOnEnter,
	"SetCameraButtonOnMouseUp", SetCameraButtonOnMouseUp,
	"SetRtspParamListButtonOnMouseUp", SetRtspParamListButtonOnMouseUp,
	"SetRtspFactoryMsgRadioBoxOnMouseUp", SetRtspFactoryMsgRadioBoxOnMouseUp,

	// 
	"SetAlarmSingleAttrOnEnter", SetAlarmSingleAttrOnEnter,
	"SetAlarmSingleAttrListButtonOnMouseUp", SetAlarmSingleAttrListButtonOnMouseUp,
	"SetAlarmSingleAttrTimeMsgRadioBoxOnMouseUp", SetAlarmSingleAttrTimeMsgRadioBoxOnMouseUp,

    NULL, NULL
};
