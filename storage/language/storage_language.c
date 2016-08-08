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
	case SID_Jd_Mode_Out:
		return ESID_Jd_Mode_Out;

	case SID_Jd_Mode_Mode:
		return ESID_Jd_Mode_Mode;

	case SID_Jd_LouShang:
		return ESID_Jd_LouShang;

	case SID_Jd_Deng:
		return ESID_Jd_Deng;

	case SID_Jd_ChuangLian:
		return ESID_Jd_ChuangLian;

	case SID_Jd_KongTiaoJi:
		return ESID_Jd_KongTiaoJi;

	case SID_Jd_ChaZuo1:
		return ESID_Jd_ChaZuo1;

	case SID_Jd_Meiqi:
		return ESID_Jd_Meiqi;


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


