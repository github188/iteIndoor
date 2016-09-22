/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	storage_include.c
Author:     	luofl
Version:    	2.0
Date: 		2014-09-15
Description:
字符处理程序
History:
1. Date:
Author:
Modification:
*************************************************/
#include "storage_include.h"

/*************************************************
Function:		get_str
Description: 	根据字符ID获取字符
Input:
id			图片ID
Output:
filename	图片的路径
Return:		无
Others:
*************************************************/
const char * get_str(STRID_E id)
{
	switch (id)
	{

// 设置文字
	case SID_NULL:
		return ESID_NULL;

	case SID_Set_PhotoFrame_Lcd_Close:
		return ESID_Set_PhotoFrame_Lcd_Close;

	case SID_Set_PhotoFrame_Lcd_Save:
		return ESID_Set_PhotoFrame_Lcd_Save;

	case SID_Set_KeyTone_ON:
		return ESID_Set_KeyTone_ON;

	case SID_Set_KeyTone_OFF:
		return ESID_Set_KeyTone_OFF;

	case SID_Set_Talk_Volume:
		return ESID_Set_Talk_Volume;

	case SID_Set_Ring_Volume:
		return ESID_Set_Ring_Volume;

	case SID_Time_Sec:
		return ESID_Time_Sec;

	case SID_Time_Hour:
		return ESID_Time_Hour;

	case SID_Time_Min:
		return ESID_Time_Min; 

	case SID_Time_Never:
		return ESID_Time_Never;

	case SID_Set_Pwd_Err:
		return ESID_Set_Pwd_Err;

	case SID_Set_EnterUserPwd:
		return ESID_Set_EnterUserPwd;

	case SID_Set_EnterProjectPwd:
		return ESID_Set_EnterProjectPwd;

	case SID_Set_DoorPwd:
		return ESID_Set_DoorPwd;

	case SID_Set_SecurityPwd:
		return ESID_Set_SecurityPwd;

	case SID_Set_Pass_DoorUser:
		return ESID_Set_Pass_DoorUser;

	case SID_Set_Pass_DoorServer:
		return ESID_Set_Pass_DoorServer;

	case SID_Set_Pass_AlarmUser:
		return ESID_Set_Pass_AlarmUser;

	case SID_Set_Pass_AlarmServer:
		return ESID_Set_Pass_AlarmServer;

	case SID_Set_Pass_Project:
		return ESID_Set_Pass_Project;

	case SID_Set_Pass_Change_DoorUser:
		return ESID_Set_Pass_Change_DoorUser;

	case SID_Set_Pass_Change_AlarmUser:
		return ESID_Set_Pass_Change_AlarmUser;

	case SID_Set_Pass_New:
		return ESID_Set_Pass_New;

	case SID_Set_Pass_Query:
		return ESID_Set_Pass_Query; 

	case SID_Set_Link_LinkSet:
		return ESID_Set_Link_LinkSet;

	case SID_Set_Link_AfLink:
		return ESID_Set_Link_AfLink;

	case SID_Set_Link_CallLink:
		return ESID_Set_Link_CallLink;

	case SID_Set_Link_JDLink:
		return ESID_Set_Link_JDLink;

	case SID_Set_Line_Lyly_Mode_Audio:
		return ESID_Set_Line_Lyly_Mode_Audio;

	case SID_Set_Line_Lyly_Mode_Both:
		return ESID_Set_Line_Lyly_Mode_Both;

	case SID_Set_Line_Lyly_Mode_Video:
		return ESID_Set_Line_Lyly_Mode_Video;

	case SID_Set_Line_Jd_Mode_Huike:
		return ESID_Set_Line_Jd_Mode_Huike;

	case SID_Set_Line_Jd_Mode_Jiucan:
		return ESID_Set_Line_Jd_Mode_Jiucan;

	case SID_Set_Line_Jd_Mode_Yejian:
		return ESID_Set_Line_Jd_Mode_Yejian;

	case SID_Set_Line_Jd_Mode_Jiedian:
		return ESID_Set_Line_Jd_Mode_Jiedian;

	case SID_Set_Line_Jd_Mode_Putong:
		return ESID_Set_Line_Jd_Mode_Putong;

	case SID_Set_Ring_Lyly_default_tip:
		return ESID_Set_Ring_Lyly_default_tip;

	case SID_Set_Ring_Lyly_record_tip:
		return ESID_Set_Ring_Lyly_record_tip;

	case SID_Set_Ring_Ring:
		return ESID_Set_Ring_Ring;

	case SID_Set_Ring_Hit:
		return ESID_Set_Ring_Hit;

	case SID_Set_Ring_Info:
		return ESID_Set_Ring_Info;

	case SID_Set_Ring_Alarm:
		return ESID_Set_Ring_Alarm;

	case SID_Set_Ring_RingSet:
		return ESID_Set_Ring_RingSet;

	case SID_Set_Ring_CallRing:
		return ESID_Set_Ring_CallRing;

	case SID_Set_Ring_Stair:
		return ESID_Set_Ring_Stair;

	case SID_Set_Ring_Area:
		return ESID_Set_Ring_Area;

	case SID_Set_Ring_Door:
		return ESID_Set_Ring_Door;

	case SID_Set_Ring_Center:
		return ESID_Set_Ring_Center;

	case SID_Set_Ring_Room:
		return ESID_Set_Ring_Room;

	case SID_Set_Ring_Fenji:
		return ESID_Set_Ring_Fenji;

	case SID_Set_User_AfGeLi:
		return ESID_Set_User_AfGeLi;

	case SID_Set_User_AfJuFang:
		return ESID_Set_User_AfJuFang;

	case SID_Set_Prj_AfProperty:
		return ESID_Set_Prj_AfProperty;

	case SID_Set_User_AfGeLi_Open:
		return ESID_Set_User_AfGeLi_Open;

	case SID_Set_User_AfGeLi_Close:
		return ESID_Set_User_AfGeLi_Close;

	case SID_Set_User_AfJuFang_Close:
		return ESID_Set_User_AfJuFang_Close;

	case SID_Set_User_AfJuFang_Open:
		return ESID_Set_User_AfJuFang_Open;

	case SID_Set_Used:
		return ESID_Set_Used;

	case SID_Set_Null:
		return ESID_Set_Null;

	case SID_Set_UnUsed:
		return ESID_Set_UnUsed;

	case SID_Set_Prj_ExtJD_Used:
		return ESID_Set_Prj_ExtJD_Used;

	case SID_Set_Prj_ExtJD_Unused:
		return ESID_Set_Prj_ExtJD_Unused;

	case SID_Set_Prj_ExtDoor1_Used:
		return ESID_Set_Prj_ExtDoor1_Used;

	case SID_Set_Prj_ExtDoor1_Unused:
		return ESID_Set_Prj_ExtDoor1_Unused;

	case SID_Set_Prj_ExtDoor2_Used:
		return ESID_Set_Prj_ExtDoor2_Used;

	case SID_Set_Prj_ExtDoor2_Unused:
		return ESID_Set_Prj_ExtDoor2_Unused;

	case SID_Set_Prj_ExtStair_Used:
		return ESID_Set_Prj_ExtStair_Used;

	case SID_Set_Prj_ExtStair_Unused:
		return ESID_Set_Prj_ExtStair_Unused;

	case SID_Set_Prj_Rule_Riser_Used:
		return ESID_Set_Prj_Rule_Riser_Used;

	case SID_Set_Prj_Rule_Riser_Unused:
		return ESID_Set_Prj_Rule_Riser_Unused;

	case SID_Set_Prj_Rule_Device_Wrong:
		return ESID_Set_Prj_Rule_Device_Wrong;

	case SID_Set_Prj_Rule_Riser_Null:
		return ESID_Set_Prj_Rule_Riser_Null;

	case SID_Set_Prj_Rule_Room_Null:
		return ESID_Set_Prj_Rule_Room_Null;

	case SID_Set_Prj_Rule_Stair_Null:
		return ESID_Set_Prj_Rule_Stair_Null;

	case SID_Set_Prj_Rule_Riser_Err:
		return ESID_Set_Prj_Rule_Riser_Err;

	case SID_Set_Prj_Rule_Room_Err:
		return ESID_Set_Prj_Rule_Room_Err;

	case SID_Set_Prj_Rule_Stair_Err:
		return ESID_Set_Prj_Rule_Stair_Err;

	case SID_Set_Prj_Rule_RuleAll_Err:
		return ESID_Set_Prj_Rule_RuleAll_Err;

	case SID_Set_Prj_Rule_Len_Err:
		return ESID_Set_Prj_Rule_Len_Err;

	case SID_Set_Prj_Rule_RoomNo_Err:
		return ESID_Set_Prj_Rule_RoomNo_Err;

	case SID_Set_Prj_IP_Address_Err:
		return ESID_Set_Prj_IP_Address_Err;

	case SID_Set_Pwd_Two_Err:
		return ESID_Set_Pwd_Two_Err;

	case SID_Set_Rtsp_Indoor_Camera:
		return ESID_Set_Rtsp_Indoor_Camera;

	case SID_Set_Rtsp_Camera_Used:
		return ESID_Set_Rtsp_Camera_Used;

	case SID_Set_Rtsp_Camera_Unused:
		return ESID_Set_Rtsp_Camera_Unused;

	case SID_Set_Rtsp_PTZ_Used:
		return ESID_Set_Rtsp_PTZ_Used;

	case SID_Set_Rtsp_PTZ_Unused:
		return ESID_Set_Rtsp_PTZ_Unused;

	case SID_Set_Data_Save_Err:
		return ESID_Set_Data_Save_Err;

	case SID_Set_Rtsp_Port_Null:
		return ESID_Set_Rtsp_Port_Null;

	case SID_Set_Rtsp_Channel_Null:
		return ESID_Set_Rtsp_Channel_Null;

	case SID_Msg_Connect_Outtime:
		return ESID_Msg_Connect_Outtime;

	case SID_Msg_Save_OK:
		return ESID_Msg_Save_OK;

	case SID_Msg_Save_Err:
		return ESID_Msg_Save_Err;

	case SID_Msg_Param_Save:
		return ESID_Msg_Param_Save;

	case SID_Msg_Operat_OK:
		return ESID_Msg_Operat_OK;

	case SID_Msg_Operat_Err:
		return ESID_Msg_Operat_Err;

	case SID_Net_Door_Card_Delet_All:
		return ESID_Net_Door_Card_Delet_All;

	case SID_Msg_Format:
		return ESID_Msg_Format;

	case SID_Msg_Recover_OK:
		return ESID_Msg_Recover_OK;

	case SID_Msg_Return_FactorySet:
		return ESID_Msg_Return_FactorySet;

	case SID_Msg_Param_Suer_Save:
		return ESID_Msg_Param_Suer_Save;

	case SID_Msg_IP_Unline:
		return ESID_Msg_IP_Unline;

	case SID_Msg_Obtain_Jd_False:
		return ESID_Msg_Obtain_Jd_False;

	case SID_Set_Ring_Tips_Fail:
		return ESID_Set_Ring_Tips_Fail;

// IP协议转换器
	case SID_Set_Unbounded:
		return ESID_Set_Unbounded;

	case SID_Set_Bounded:
		return ESID_Set_Bounded;

	case SID_Set_Generate_Binding_Code:
		return ESID_Set_Generate_Binding_Code;

	case SID_Set_Binding_Code:
		return ESID_Set_Binding_Code;

	case SID_Set_ExtenNo:
		return ESID_Set_ExtenNo;

	case SID_Set_Online:
		return ESID_Set_Online;

	case SID_Set_Offline:
		return ESID_Set_Offline;




// 网络门前机文字
	case SID_Net_Door:
		return ESID_Net_Door;

	case SID_Net_Door_First:
		return ESID_Net_Door_First;

	case SID_Net_Door_Second:
		return ESID_Net_Door_Second;

	case SID_Net_Door_NetParam:
		return ESID_Net_Door_NetParam;

	case SID_Net_Door_Get_MAC:
		return ESID_Net_Door_Get_MAC;

	case SID_Net_Door_Save_Reboot_Door:
		return ESID_Net_Door_Save_Reboot_Door;

// 报警文字
	case SID_Bj_Report_Type_Chufa:
		return ESID_Bj_Report_Type_Chufa;

	case SID_Bj_Fangqu:
		return ESID_Bj_Fangqu;

	case SID_Bj_SOS:
		return ESID_Bj_SOS;

	case SID_Bj_Fire:
		return ESID_Bj_Fire;

	case SID_Bj_Gas:
		return ESID_Bj_Gas;

	case SID_Bj_Door:
		return ESID_Bj_Door;

	case SID_Bj_Room:
		return ESID_Bj_Room;

	case SID_Bj_Big_Window:
		return ESID_Bj_Big_Window;

	case SID_Bj_Window:
		return ESID_Bj_Window;

	case SID_Bj_Balcony:
		return ESID_Bj_Balcony;

	case SID_Bj_Remote_Home_Used:
		return ESID_Bj_Remote_Home_Used;

	case SID_Bj_Remote_Home_Unused:
		return ESID_Bj_Remote_Home_Unused;

	case SID_Bj_Remote_Out_Used:
		return ESID_Bj_Remote_Out_Used;

	case SID_Bj_Remote_Out_Unused:
		return ESID_Bj_Remote_Out_Unused;

	case SID_Bj_Remote_Night_Used:
		return ESID_Bj_Remote_Night_Used;

	case SID_Bj_Remote_Night_Unused:
		return ESID_Bj_Remote_Night_Unused;

	case SID_Bj_Set_24h_Unused:
		return ESID_Bj_Set_24h_Unused;

	case SID_Bj_Set_24h_Used:
		return ESID_Bj_Set_24h_Used;

	case SID_Bj_Set_UnSee:
		return ESID_Bj_Set_UnSee;

	case SID_Bj_Set_See:
		return ESID_Bj_Set_See;

	case SID_Bj_Set_UnHear:
		return ESID_Bj_Set_UnHear;

	case SID_Bj_Set_Hear:
		return ESID_Bj_Set_Hear;

	case SID_Bj_Tantou_Close:
		return ESID_Bj_Tantou_Close;

	case SID_Bj_Tantou_Open:
		return ESID_Bj_Tantou_Open;

	case SID_Bj_0Seconds:
		return ESID_Bj_0Seconds;

	case SID_Bj_30Seconds:
		return ESID_Bj_30Seconds;

	case SID_Bj_1Minute:
		return ESID_Bj_1Minute;

	case SID_Bj_2Minutes:
		return ESID_Bj_2Minutes;

	case SID_Bj_3Minutes:
		return ESID_Bj_3Minutes;

	case SID_Bj_4Minutes:
		return ESID_Bj_4Minutes;

	case SID_Bj_5Minutes:
		return ESID_Bj_5Minutes;

	case SID_Bj_10Minutes:
		return ESID_Bj_10Minutes;

	case SID_Bj_15Minutes:
		return ESID_Bj_15Minutes;

	case SID_Bj_Fangqu1:
		return ESID_Bj_Fangqu1;

	case SID_Bj_Fangqu2:
		return ESID_Bj_Fangqu2;

	case SID_Bj_Fangqu3:
		return ESID_Bj_Fangqu3;

	case SID_Bj_Fangqu4:
		return ESID_Bj_Fangqu4;

	case SID_Bj_Fangqu5:
		return ESID_Bj_Fangqu5;

	case SID_Bj_Fangqu6:
		return ESID_Bj_Fangqu6;

	case SID_Bj_Fangqu7:
		return ESID_Bj_Fangqu7;

	case SID_Bj_Fangqu8:
		return ESID_Bj_Fangqu8;

	case SID_Bj_Bengjikongzhi:
		return ESID_Bj_Bengjikongzhi;

	case SID_Bj_Fengjikongzhi:
		return ESID_Bj_Fengjikongzhi;

	case SID_Bj_Bf_Home:
		return ESID_Bj_Bf_Home;

	case SID_Bj_Bf_Out:
		return ESID_Bj_Bf_Out;

	case SID_Bj_Bf_Night:
		return ESID_Bj_Bf_Night;

	case SID_Bj_Already_Bufang:
		return ESID_Bj_Already_Bufang;

	case SID_Bj_Alarm_Cannot_Bf:
		return ESID_Bj_Alarm_Cannot_Bf;

	case SID_Bj_Touch_Cannot_Bf:
		return ESID_Bj_Touch_Cannot_Bf;

	case SID_Bj_Alarm_Cannot_Qc:
		return ESID_Bj_Alarm_Cannot_Qc;

	case SID_Set_Enter_Alarm_Pwd:
		return ESID_Set_Enter_Alarm_Pwd;

	case SID_Bj_Alarm_Clear_data:
		return ESID_Bj_Alarm_Clear_data;


// 对讲文字
	case SID_MainMonitor_Door:
		return ESID_MainMonitor_Door;

	case SID_MainMonitor_Stair:
		return ESID_MainMonitor_Stair;

	case SID_MainMonitor_Area:
		return ESID_MainMonitor_Area;

	case SID_MainMonitor_Camera:
		return ESID_MainMonitor_Camera;

	case SID_MainMonitor_Home:
		return ESID_MainMonitor_Room;

	case SID_DevManager:
		return ESID_DevManager;

	case SID_DevArea:
		return ESID_DevArea;

	case SID_DevStair:
		return ESID_DevStair;

	case SID_DevRoom:
		return ESID_DevRoom;

	case SID_DevPhone:
		return ESID_DevPhone;

	case SID_DevDoorPhone:
		return ESID_DevDoorPhone;

	case SID_DevDoorNet:
		return ESID_DevDoorNet;

	case SID_DevGateWay:
		return ESID_DevGateWay;

	case SID_DevIPCamera:
		return ESID_DevIPCamera;

	case SID_DevFenJi:
		return ESID_DevFenJi;

	case SID_DevMaster:
		return ESID_DevMaster;
	case SID_Inter_EnterUserNo:
		return ESID_Inter_EnterUserNo;

	case SID_Inter_EnterFJNo:
		return ESID_Inter_EnterFJNo;

	case SID_Inter_WrongNo:
		return ESID_Inter_WrongNo;

	case SID_Inter_NoNotFound:
		return ESID_Inter_NoNotFound;

	case SID_Inter_Connecting:
		return ESID_Inter_Connecting;

	case SID_Inter_BeCalling:
		return ESID_Inter_BeCalling;

	case SID_Inter_Calling:
		return ESID_Inter_Calling;

	case SID_Inter_Talking:
		return ESID_Inter_Talking;

	case SID_Inter_RecordHinting:
		return ESID_Inter_RecordHinting;

	case SID_Inter_Recording:
		return ESID_Inter_Recording;

	case SID_Inter_UnlockSuccess:
		return ESID_Inter_UnlockSuccess;

	case SID_Inter_UnlockFailed:
		return ESID_Inter_UnlockFailed;

	case SID_Inter_CaptureSuccess:
		return ESID_Inter_CaptureSuccess;

	case SID_Inter_CaptureFailed:
		return ESID_Inter_CaptureFailed;

	case SID_Inter_ManagerNoSet:
		return ESID_Inter_ManagerNoSet;

	case SID_Inter_Search:
		return ESID_Inter_Search;

	case SID_Inter_Monitor:
		return ESID_Inter_Monitor;

	case SID_Inter_busy:
		return ESID_Inter_busy;

	case SID_Inter_ConnFailed:
		return ESID_Inter_ConnFailed;

	case SID_Inter_SearchFailed:
		return ESID_Inter_SearchFailed;

// 家電文字
	case SID_Jd_Set:
		return ESID_Jd_Set;

	case SID_Jd_Lamp_Set:
		return ESID_Jd_Lamp_Set;

	case SID_Jd_KongTiao_Set:
		return ESID_Jd_KongTiao_Set;

	case SID_Jd_ChuangLian_Set:
		return ESID_Jd_ChuangLian_Set;

	case SID_Jd_Dianyuan_Set:
		return ESID_Jd_Dianyuan_Set;

	case SID_Jd_Meiqi_Set:
		return ESID_Jd_Meiqi_Set;
	case SID_Jd_Lamp:
		return ESID_Jd_Lamp;

	case SID_Jd_KongTiao:
		return ESID_Jd_KongTiao;

	case SID_Jd_Dianyuan:
		return ESID_Jd_Dianyuan;

	case SID_Jd_Meiqi:
		return ESID_Jd_Meiqi;

	case SID_Jd_None:
		return ESID_Jd_None;

	case SID_Jd_Dev_Set_UnAdjust:
		return ESID_Jd_Dev_Set_UnAdjust;

	case SID_Jd_Dev_Set_Adjust:
		return ESID_Jd_Dev_Set_Adjust;

	case SID_Jd_Factory:
		return ESID_Jd_Factory;

	case SID_Jd_Set_Dev_Noname:
		return ESID_Jd_Set_Dev_Noname;

	case SID_Jd_Set_Scene_Noname:
		return ESID_Jd_Set_Scene_Noname;

	case SID_Jd_Set_Name_Err:
		return ESID_Jd_Set_Name_Err;

	case SID_Jd_Set_Adr_Err:
		return ESID_Jd_Set_Adr_Err;

	case SID_Jd_Set_Adr_Max_Err:
		return ESID_Jd_Set_Adr_Max_Err;

	case SID_Jd_Set_Dev_Name1:
		return ESID_Jd_Set_Dev_Name1;

	case SID_Jd_Set_Dev_Name2:
		return ESID_Jd_Set_Dev_Name2;

	case SID_Jd_LouShang:
		return ESID_Jd_LouShang;

	case SID_Jd_LouXia:
		return ESID_Jd_LouXia;

	case SID_Jd_WuNei:
		return ESID_Jd_WuNei;

	case SID_Jd_WuWai:
		return ESID_Jd_WuWai;

	case SID_Jd_TianTai:
		return ESID_Jd_TianTai;

	case SID_Jd_GeLou:
		return ESID_Jd_GeLou;

	case SID_Jd_HuaYuan:
		return ESID_Jd_HuaYuan;

	case SID_Jd_DiJiao:
		return ESID_Jd_DiJiao;

	case SID_Jd_TianJin:
		return ESID_Jd_TianJin;

	case SID_Jd_WuYan:
		return ESID_Jd_WuYan;

	case SID_Jd_QianTing:
		return ESID_Jd_QianTing;

	case SID_Jd_HouYuan:
		return ESID_Jd_HouYuan;

	case SID_Jd_DianTiJian:
		return ESID_Jd_DianTiJian;

	case SID_Jd_TingCheFang:
		return ESID_Jd_TingCheFang;

	case SID_Jd_DaMen:
		return ESID_Jd_DaMen;

	case SID_Jd_QianMen:
		return ESID_Jd_QianMen;

	case SID_Jd_ZhongMen:
		return ESID_Jd_ZhongMen;

	case SID_Jd_PangMen:
		return ESID_Jd_PangMen;

	case SID_Jd_HouMen:
		return ESID_Jd_HouMen;

	case SID_Jd_CeMen:
		return ESID_Jd_CeMen;

	case SID_Jd_BianMen:
		return ESID_Jd_BianMen;

	case SID_Jd_HenMen:
		return ESID_Jd_HenMen;

	case SID_Jd_YanTai:
		return ESID_Jd_YanTai;

	case SID_Jd_Zhoulan:
		return ESID_Jd_Zhoulan;

	case SID_Jd_GuoDao:
		return ESID_Jd_GuoDao;

	case SID_Jd_LouTi:
		return ESID_Jd_LouTi;

	case SID_Jd_TiKou:
		return ESID_Jd_TiKou;

	case SID_Jd_TiJian:
		return ESID_Jd_TiJian;

	case SID_Jd_TiXia:
		return ESID_Jd_TiXia;

	case SID_Jd_KeTing:
		return ESID_Jd_KeTing;

	case SID_Jd_DaTing:
		return ESID_Jd_DaTing;

	case SID_Jd_ZhuTing:
		return ESID_Jd_ZhuTing;

	case SID_Jd_CanTing:
		return ESID_Jd_CanTing;

	case SID_Jd_FanTing:
		return ESID_Jd_FanTing;

	case SID_Jd_MenTing:
		return ESID_Jd_MenTing;

	case SID_Jd_ZhuWoShi:
		return ESID_Jd_ZhuWoShi;

	case SID_Jd_ZhuRenFang:
		return ESID_Jd_ZhuRenFang;

	case SID_Jd_WoShi:
		return ESID_Jd_WoShi;

	case SID_Jd_ShuiFang:
		return ESID_Jd_ShuiFang;

	case SID_Jd_FangJianA:
		return ESID_Jd_FangJianA;

	case SID_Jd_FangJianB:
		return ESID_Jd_FangJianB;

	case SID_Jd_FangJianC:
		return ESID_Jd_FangJianC;

	case SID_Jd_FangJianD:
		return ESID_Jd_FangJianD;

	case SID_Jd_FangJianE:
		return ESID_Jd_FangJianE;

	case SID_Jd_ChuFang:
		return ESID_Jd_ChuFang;

	case SID_Jd_ShuFang:
		return ESID_Jd_ShuFang;

	case SID_Jd_KeFang:
		return ESID_Jd_KeFang;

	case SID_Jd_ZhuYuShi:
		return ESID_Jd_ZhuYuShi;

	case SID_Jd_YuShi:
		return ESID_Jd_YuShi;

	case SID_Jd_ErTongFang:
		return ESID_Jd_ErTongFang;

	case SID_Jd_GongRenFang:
		return ESID_Jd_GongRenFang;

	case SID_Jd_DuoYongTuJian:
		return ESID_Jd_DuoYongTuJian;

	case SID_Jd_QiJuShi:
		return ESID_Jd_QiJuShi;

	case SID_Jd_YouLeShi:
		return ESID_Jd_YouLeShi;

	case SID_Jd_ShuXiShi:
		return ESID_Jd_ShuXiShi;

	case SID_Jd_WeiShengJian:
		return ESID_Jd_WeiShengJian;

	case SID_Jd_XieMaoJian:
		return ESID_Jd_XieMaoJian;

	case SID_Jd_GongZuoJian:
		return ESID_Jd_GongZuoJian;

	case SID_Jd_DiXiaShi:
		return ESID_Jd_DiXiaShi;

	case SID_Jd_Set_Scene:
		return ESID_Jd_Set_Scene;

	case SID_Jd_Mode_Huike:
		return ESID_Jd_Mode_Huike;

	case SID_Jd_Mode_Jiucan:
		return ESID_Jd_Mode_Jiucan;

	case SID_Jd_Mode_Yejian:
		return ESID_Jd_Mode_Yejian;

	case SID_Jd_Mode_Jiedian:
		return ESID_Jd_Mode_Jiedian;

	case SID_Jd_Mode_Putong:
		return ESID_Jd_Mode_Putong;

	case SID_Jd_Mode_Self:
		return ESID_Jd_Mode_Self;

	case SID_Jd_Mode_Set_Name1:
		return ESID_Jd_Mode_Set_Name1;

	case SID_Jd_Mode_Set_Name2:
		return ESID_Jd_Mode_Set_Name2;

	case SID_Jd_Mode_Out:
		return ESID_Jd_Mode_Out;

	case SID_Jd_Mode_Cook:
		return ESID_Jd_Mode_Cook;

	case SID_Jd_Mode_first:
		return ESID_Jd_Mode_first;

	case SID_Jd_Mode_Afater:
		return ESID_Jd_Mode_Afater;

	case SID_Jd_Mode_Night:
		return ESID_Jd_Mode_Night;

	case SID_Jd_Mode_Bed:
		return ESID_Jd_Mode_Bed;

	case SID_Jd_Mode_QiYe:
		return ESID_Jd_Mode_QiYe;

	case SID_Jd_Mode_Up:
		return ESID_Jd_Mode_Up;

	case SID_Jd_Mode_Muyu:
		return ESID_Jd_Mode_Muyu;

	case SID_Jd_Mode_Tea:
		return ESID_Jd_Mode_Tea;

	case SID_Jd_Mode_Drink:
		return ESID_Jd_Mode_Drink;

	case SID_Jd_Mode_Read:
		return ESID_Jd_Mode_Read;

	case SID_Jd_Mode_Work:
		return ESID_Jd_Mode_Work;

	case SID_Jd_Mode_Juhui:
		return ESID_Jd_Mode_Juhui;

	case SID_Jd_Mode_Play:
		return ESID_Jd_Mode_Play;

	case SID_Jd_Mode_Film:
		return ESID_Jd_Mode_Film;

	case SID_Jd_Mode_Sing:
		return ESID_Jd_Mode_Sing;

	case SID_Jd_Mode_Storang:
		return ESID_Jd_Mode_Storang;

	case SID_Jd_Mode_Wenxi:
		return ESID_Jd_Mode_Wenxi;

	case SID_Jd_Mode_Langman:
		return ESID_Jd_Mode_Langman;

	case SID_Jd_Mode_Jiqing:
		return ESID_Jd_Mode_Jiqing;

	case SID_Jd_Mode_Dianya:
		return ESID_Jd_Mode_Dianya;

	case SID_Jd_Mode_Chlid:
		return ESID_Jd_Mode_Chlid;

	case SID_Jd_Mode_Chenwen:
		return ESID_Jd_Mode_Chenwen;

	case SID_Jd_Mode_Think:
		return ESID_Jd_Mode_Think;

	case SID_Jd_Mode_Cool:
		return ESID_Jd_Mode_Cool;

	case SID_Jd_Mode_Warm:
		return ESID_Jd_Mode_Warm;

	case SID_Jd_Mode_Mode:
		return ESID_Jd_Mode_Mode;

	case SID_Jd_Mode_Sence:
		return ESID_Jd_Mode_Sence;

	case SID_Jd_Mode_Qingdiao:
		return ESID_Jd_Mode_Qingdiao;

	case SID_Jd_Mode_Fenwei:
		return ESID_Jd_Mode_Fenwei;

	case SID_Jd_Mode_Time:
		return ESID_Jd_Mode_Time;

	case SID_Jd_Mode_Changjing:
		return ESID_Jd_Mode_Changjing;

	case SID_Jd_Deng:
		return ESID_Jd_Deng;

	case SID_Jd_TiaoGuangDeng:
		return ESID_Jd_TiaoGuangDeng;

	case SID_Jd_BaiChiDeng:
		return ESID_Jd_BaiChiDeng;

	case SID_Jd_GuangGuan:
		return ESID_Jd_GuangGuan;

	case SID_Jd_DiaoDeng:
		return ESID_Jd_DiaoDeng;

	case SID_Jd_HuaDeng:
		return ESID_Jd_HuaDeng;

	case SID_Jd_BiDeng:
		return ESID_Jd_BiDeng;

	case SID_Jd_SheDeng:
		return ESID_Jd_SheDeng;

	case SID_Jd_TaiDeng:
		return ESID_Jd_TaiDeng;

	case SID_Jd_LouDiDeng:
		return ESID_Jd_LouDiDeng;

	case SID_Jd_ZhaoMingDeng:
		return ESID_Jd_ZhaoMingDeng;

	case SID_Jd_ShiYingDeng:
		return ESID_Jd_ShiYingDeng;

	case SID_Jd_YeDeng:
		return ESID_Jd_YeDeng;

	case SID_Jd_ZhuanshiDEng:
		return ESID_Jd_ZhuanshiDEng;

	case SID_Jd_DianZhiSuo:
		return ESID_Jd_DianZhiSuo;

	case SID_Jd_XiaoduWanGui:
		return ESID_Jd_XiaoduWanGui;

	case SID_Jd_KongQiJinHua:
		return ESID_Jd_KongQiJinHua;

	case SID_Jd_YinShuiJi:
		return ESID_Jd_YinShuiJi;

	case SID_Jd_KongTiaoJi:
		return ESID_Jd_KongTiaoJi;

	case SID_Jd_LenQiJi:
		return ESID_Jd_LenQiJi;

	case SID_Jd_CouYouJi:
		return ESID_Jd_CouYouJi;

	case SID_Jd_NuanFenJi:
		return ESID_Jd_NuanFenJi;

	case SID_Jd_DianReTan:
		return ESID_Jd_DianReTan;

	case SID_Jd_ReShuiLu:
		return ESID_Jd_ReShuiLu;

	case SID_Jd_DianFanBao:
		return ESID_Jd_DianFanBao;

	case SID_Jd_DianZhiWaGuo:
		return ESID_Jd_DianZhiWaGuo;

	case SID_Jd_WeiBoLu:
		return ESID_Jd_WeiBoLu;

	case SID_Jd_XiYiJi:
		return ESID_Jd_XiYiJi;

	case SID_Jd_DianShiJi:
		return ESID_Jd_DianShiJi;

	case SID_Jd_LuXiangJi:
		return ESID_Jd_LuXiangJi;

	case SID_Jd_YingXiang:
		return ESID_Jd_YingXiang;

	case SID_Jd_DianNao:
		return ESID_Jd_DianNao;

	case SID_Jd_FengShan:
		return ESID_Jd_FengShan;

	case SID_Jd_CD:
		return ESID_Jd_CD;

	case SID_Jd_LD:
		return ESID_Jd_LD;

	case SID_Jd_VCD:
		return ESID_Jd_VCD;

	case SID_Jd_DVD:
		return ESID_Jd_DVD;

	case SID_Jd_ChaZuo1:
		return ESID_Jd_ChaZuo1;

	case SID_Jd_ChaZuo2:
		return ESID_Jd_ChaZuo2;

	case SID_Jd_ChaZuo3:
		return ESID_Jd_ChaZuo3;

	case SID_Jd_ChaZuo4:
		return ESID_Jd_ChaZuo4;

	case SID_Jd_ChaZuo5:
		return ESID_Jd_ChaZuo5;

	case SID_Jd_XiaoDuSheBe:
		return ESID_Jd_XiaoDuSheBe;

	case SID_Jd_SettingMusic:
		return ESID_Jd_SettingMusic;

	case SID_Jd_ChuangLian:
		return ESID_Jd_ChuangLian;

	case SID_Jd_JuanLian:
		return ESID_Jd_JuanLian;

	case SID_Jd_DianDongBuLian:
		return ESID_Jd_DianDongBuLian;

	case SID_Jd_DianDongShaLian:
		return ESID_Jd_DianDongShaLian;

	case SID_Jd_Deng1:
		return ESID_Jd_Deng1;

	case SID_Jd_Deng2:
		return ESID_Jd_Deng2;

	case SID_Jd_Deng3:
		return ESID_Jd_Deng3;

	case SID_Jd_Deng4:
		return ESID_Jd_Deng4;

	case SID_Jd_Deng5:
		return ESID_Jd_Deng5;

	case SID_Jd_Deng6:
		return ESID_Jd_Deng6;

	case SID_Jd_Deng7:
		return ESID_Jd_Deng7;

	case SID_Jd_Deng8:
		return ESID_Jd_Deng8;

	case SID_Jd_Deng9:
		return ESID_Jd_Deng9;

	case SID_Jd_Deng10:
		return ESID_Jd_Deng10;

	case SID_Jd_TiaoGuangDeng1:
		return ESID_Jd_TiaoGuangDeng1;

	case SID_Jd_TiaoGuangDeng2:
		return ESID_Jd_TiaoGuangDeng2;

	case SID_Jd_TiaoGuangDeng3:
		return ESID_Jd_TiaoGuangDeng3;

	case SID_Jd_TiaoGuangDeng4:
		return ESID_Jd_TiaoGuangDeng4;

	case SID_Jd_TiaoGuangDeng5:
		return ESID_Jd_TiaoGuangDeng5;

	case SID_Jd_TiaoGuangDeng6:
		return ESID_Jd_TiaoGuangDeng6;

	case SID_Jd_TiaoGuangDeng7:
		return ESID_Jd_TiaoGuangDeng7;

	case SID_Jd_TiaoGuangDeng8:
		return ESID_Jd_TiaoGuangDeng8;

	case SID_Jd_TiaoGuangDeng9:
		return ESID_Jd_TiaoGuangDeng9;

	case SID_Jd_TiaoGuangDeng10:
		return ESID_Jd_TiaoGuangDeng10;

	case SID_Jd_BaiChiDeng1:
		return ESID_Jd_BaiChiDeng1;

	case SID_Jd_BaiChiDeng2:
		return ESID_Jd_BaiChiDeng2;

	case SID_Jd_BaiChiDeng3:
		return ESID_Jd_BaiChiDeng3;

	case SID_Jd_BaiChiDeng4:
		return ESID_Jd_BaiChiDeng4;

	case SID_Jd_BaiChiDeng5:
		return ESID_Jd_BaiChiDeng5;

	case SID_Jd_BaiChiDeng6:
		return ESID_Jd_BaiChiDeng6;

	case SID_Jd_BaiChiDeng7:
		return ESID_Jd_BaiChiDeng7;

	case SID_Jd_BaiChiDeng8:
		return ESID_Jd_BaiChiDeng8;

	case SID_Jd_BaiChiDeng9:
		return ESID_Jd_BaiChiDeng9;

	case SID_Jd_BaiChiDeng10:
		return ESID_Jd_BaiChiDeng10;

	case SID_Jd_GuangGuan1:
		return ESID_Jd_GuangGuan1;

	case SID_Jd_GuangGuan2:
		return ESID_Jd_GuangGuan2;

	case SID_Jd_GuangGuan3:
		return ESID_Jd_GuangGuan3;

	case SID_Jd_GuangGuan4:
		return ESID_Jd_GuangGuan4;

	case SID_Jd_GuangGuan5:
		return ESID_Jd_GuangGuan5;

	case SID_Jd_GuangGuan6:
		return ESID_Jd_GuangGuan6;

	case SID_Jd_GuangGuan7:
		return ESID_Jd_GuangGuan7;

	case SID_Jd_GuangGuan8:
		return ESID_Jd_GuangGuan8;

	case SID_Jd_GuangGuan9:
		return ESID_Jd_GuangGuan9;

	case SID_Jd_GuangGuan10:
		return ESID_Jd_GuangGuan10;

	case SID_Jd_DiaoDeng1:
		return ESID_Jd_DiaoDeng1;

	case SID_Jd_DiaoDeng2:
		return ESID_Jd_DiaoDeng2;

	case SID_Jd_DiaoDeng3:
		return ESID_Jd_DiaoDeng3;

	case SID_Jd_DiaoDeng4:
		return ESID_Jd_DiaoDeng4;

	case SID_Jd_DiaoDeng5:
		return ESID_Jd_DiaoDeng5;

	case SID_Jd_DiaoDeng6:
		return ESID_Jd_DiaoDeng6;

	case SID_Jd_DiaoDeng7:
		return ESID_Jd_DiaoDeng7;

	case SID_Jd_DiaoDeng8:
		return ESID_Jd_DiaoDeng8;

	case SID_Jd_DiaoDeng9:
		return ESID_Jd_DiaoDeng9;

	case SID_Jd_DiaoDeng10:
		return ESID_Jd_DiaoDeng10;

	case SID_Jd_HuaDeng1:
		return ESID_Jd_HuaDeng1;

	case SID_Jd_HuaDeng2:
		return ESID_Jd_HuaDeng2;

	case SID_Jd_HuaDeng3:
		return ESID_Jd_HuaDeng3;

	case SID_Jd_HuaDeng4:
		return ESID_Jd_HuaDeng4;

	case SID_Jd_HuaDeng5:
		return ESID_Jd_HuaDeng5;

	case SID_Jd_HuaDeng6:
		return ESID_Jd_HuaDeng6;

	case SID_Jd_HuaDeng7:
		return ESID_Jd_HuaDeng7;

	case SID_Jd_HuaDeng8:
		return ESID_Jd_HuaDeng8;

	case SID_Jd_HuaDeng9:
		return ESID_Jd_HuaDeng9;

	case SID_Jd_HuaDeng10:
		return ESID_Jd_HuaDeng10;

	case SID_Jd_BiDeng1:
		return ESID_Jd_BiDeng1;

	case SID_Jd_BiDeng2:
		return ESID_Jd_BiDeng2;

	case SID_Jd_BiDeng3:
		return ESID_Jd_BiDeng3;

	case SID_Jd_BiDeng4:
		return ESID_Jd_BiDeng4;

	case SID_Jd_BiDeng5:
		return ESID_Jd_BiDeng5;

	case SID_Jd_BiDeng6:
		return ESID_Jd_BiDeng6;

	case SID_Jd_BiDeng7:
		return ESID_Jd_BiDeng7;

	case SID_Jd_BiDeng8:
		return ESID_Jd_BiDeng8;

	case SID_Jd_BiDeng9:
		return ESID_Jd_BiDeng9;

	case SID_Jd_BiDeng10:
		return ESID_Jd_BiDeng10;

	case SID_Jd_SheDeng1:
		return ESID_Jd_SheDeng1;

	case SID_Jd_SheDeng2:
		return ESID_Jd_SheDeng2;

	case SID_Jd_SheDeng3:
		return ESID_Jd_SheDeng3;

	case SID_Jd_SheDeng4:
		return ESID_Jd_SheDeng4;

	case SID_Jd_SheDeng5:
		return ESID_Jd_SheDeng5;

	case SID_Jd_SheDeng6:
		return ESID_Jd_SheDeng6;

	case SID_Jd_SheDeng7:
		return ESID_Jd_SheDeng7;

	case SID_Jd_SheDeng8:
		return ESID_Jd_SheDeng8;

	case SID_Jd_SheDeng9:
		return ESID_Jd_SheDeng9;

	case SID_Jd_SheDeng10:
		return ESID_Jd_SheDeng10;

	case SID_Jd_TaiDeng1:
		return ESID_Jd_TaiDeng1;

	case SID_Jd_TaiDeng2:
		return ESID_Jd_TaiDeng2;

	case SID_Jd_TaiDeng3:
		return ESID_Jd_TaiDeng3;

	case SID_Jd_TaiDeng4:
		return ESID_Jd_TaiDeng4;

	case SID_Jd_TaiDeng5:
		return ESID_Jd_TaiDeng5;

	case SID_Jd_TaiDeng6:
		return ESID_Jd_TaiDeng6;

	case SID_Jd_TaiDeng7:
		return ESID_Jd_TaiDeng7;

	case SID_Jd_TaiDeng8:
		return ESID_Jd_TaiDeng8;

	case SID_Jd_TaiDeng9:
		return ESID_Jd_TaiDeng9;

	case SID_Jd_TaiDeng10:
		return ESID_Jd_TaiDeng10;

	case SID_Jd_LouDiDeng1:
		return ESID_Jd_LouDiDeng1;

	case SID_Jd_LouDiDeng2:
		return ESID_Jd_LouDiDeng2;

	case SID_Jd_LouDiDeng3:
		return ESID_Jd_LouDiDeng3;

	case SID_Jd_LouDiDeng4:
		return ESID_Jd_LouDiDeng4;

	case SID_Jd_LouDiDeng5:
		return ESID_Jd_LouDiDeng5;

	case SID_Jd_LouDiDeng6:
		return ESID_Jd_LouDiDeng6;

	case SID_Jd_LouDiDeng7:
		return ESID_Jd_LouDiDeng7;

	case SID_Jd_LouDiDeng8:
		return ESID_Jd_LouDiDeng8;

	case SID_Jd_LouDiDeng9:
		return ESID_Jd_LouDiDeng9;

	case SID_Jd_LouDiDeng10:
		return ESID_Jd_LouDiDeng10;

	case SID_Jd_ZhaoMingDeng1:
		return ESID_Jd_ZhaoMingDeng1;

	case SID_Jd_ZhaoMingDeng2:
		return ESID_Jd_ZhaoMingDeng2;

	case SID_Jd_ZhaoMingDeng3:
		return ESID_Jd_ZhaoMingDeng3;

	case SID_Jd_ZhaoMingDeng4:
		return ESID_Jd_ZhaoMingDeng4;

	case SID_Jd_ZhaoMingDeng5:
		return ESID_Jd_ZhaoMingDeng5;

	case SID_Jd_ZhaoMingDeng6:
		return ESID_Jd_ZhaoMingDeng6;

	case SID_Jd_ZhaoMingDeng7:
		return ESID_Jd_ZhaoMingDeng7;

	case SID_Jd_ZhaoMingDeng8:
		return ESID_Jd_ZhaoMingDeng8;

	case SID_Jd_ZhaoMingDeng9:
		return ESID_Jd_ZhaoMingDeng9;

	case SID_Jd_ZhaoMingDeng10:
		return ESID_Jd_ZhaoMingDeng10;

	case SID_Jd_ShiYingDeng1:
		return ESID_Jd_ShiYingDeng1;

	case SID_Jd_ShiYingDeng2:
		return ESID_Jd_ShiYingDeng2;

	case SID_Jd_ShiYingDeng3:
		return ESID_Jd_ShiYingDeng3;

	case SID_Jd_ShiYingDeng4:
		return ESID_Jd_ShiYingDeng4;

	case SID_Jd_ShiYingDeng5:
		return ESID_Jd_ShiYingDeng5;

	case SID_Jd_ShiYingDeng6:
		return ESID_Jd_ShiYingDeng6;

	case SID_Jd_ShiYingDeng7:
		return ESID_Jd_ShiYingDeng7;

	case SID_Jd_ShiYingDeng8:
		return ESID_Jd_ShiYingDeng8;

	case SID_Jd_ShiYingDeng9:
		return ESID_Jd_ShiYingDeng9;

	case SID_Jd_ShiYingDeng10:
		return ESID_Jd_ShiYingDeng10;

	case SID_Jd_YeDeng1:
		return ESID_Jd_YeDeng1;

	case SID_Jd_YeDeng2:
		return ESID_Jd_YeDeng2;

	case SID_Jd_YeDeng3:
		return ESID_Jd_YeDeng3;

	case SID_Jd_YeDeng4:
		return ESID_Jd_YeDeng4;

	case SID_Jd_YeDeng5:
		return ESID_Jd_YeDeng5;

	case SID_Jd_YeDeng6:
		return ESID_Jd_YeDeng6;

	case SID_Jd_YeDeng7:
		return ESID_Jd_YeDeng7;

	case SID_Jd_YeDeng8:
		return ESID_Jd_YeDeng8;

	case SID_Jd_YeDeng9:
		return ESID_Jd_YeDeng9;

	case SID_Jd_YeDeng10:
		return ESID_Jd_YeDeng10;

	case SID_Jd_ZhuanshiDEng1:
		return ESID_Jd_ZhuanshiDEng1;

	case SID_Jd_ZhuanshiDEng2:
		return ESID_Jd_ZhuanshiDEng2;

	case SID_Jd_ZhuanshiDEng3:
		return ESID_Jd_ZhuanshiDEng3;

	case SID_Jd_ZhuanshiDEng4:
		return ESID_Jd_ZhuanshiDEng4;

	case SID_Jd_ZhuanshiDEng5:
		return ESID_Jd_ZhuanshiDEng5;

	case SID_Jd_ZhuanshiDEng6:
		return ESID_Jd_ZhuanshiDEng6;

	case SID_Jd_ZhuanshiDEng7:
		return ESID_Jd_ZhuanshiDEng7;

	case SID_Jd_ZhuanshiDEng8:
		return ESID_Jd_ZhuanshiDEng8;

	case SID_Jd_ZhuanshiDEng9:
		return ESID_Jd_ZhuanshiDEng9;

	case SID_Jd_ZhuanshiDEng10:
		return ESID_Jd_ZhuanshiDEng10;

	case SID_Jd_Button_Cold:
		return ESID_Jd_Button_Cold;

	case SID_Jd_Button_Warm:
		return ESID_Jd_Button_Warm;

	case SID_Jd_Button_Mode:
		return ESID_Jd_Button_Mode;

	case SID_Jd_KongTiao_Refre:
		return ESID_Jd_KongTiao_Refre;

	case SID_Jd_KongTiao_Warm:
		return ESID_Jd_KongTiao_Warm;

	case SID_Jd_KongTiao_Leisure:
		return ESID_Jd_KongTiao_Leisure;

	case SID_Jd_KongTiao_Sleep:
		return ESID_Jd_KongTiao_Sleep;

	case SID_Jd_KongTiao_Close:
		return ESID_Jd_KongTiao_Close;


// 公共文字
	case SID_Bj_Query_Del_Rec_One:
		return ESID_Bj_Query_Del_Rec_One;

	case SID_Bj_Query_Del_Rec_All:
		return ESID_Bj_Query_Del_Rec_All;

	case SID_Unregistered:
		return ESID_Unregistered;

	case SID_Registered:
		return ESID_Registered;


	default:
		return ESID_NULL;
	}
}


