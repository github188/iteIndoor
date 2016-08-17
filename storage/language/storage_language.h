/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	storage_include.h
Author:     	luofl
Version:    	2.0
Date: 		2014-08-30
Description:
GUI语言处理程序头文件
History:
1. Date:
Author:
Modification:
*************************************************/
#ifndef __STORAGE_LANGUAGE_H__
#define __STORAGE_LANGUAGE_H__

// need to install VS90SP1-KB980263-x86.exe for vs2008
#pragma execution_character_set("utf-8")

#ifdef __cplusplus
extern "C" {
#endif

	//将自己用到的字符串移到所属分类
	//如果在这里面没有相应的字符串，请自行在所属分类末尾按格式添加!

	// 设置面文字
#define	ESID_NULL						(g_China ? (g_Big5 ? "NULL" : "NULL") : "NULL")
#define ESID_Set_PhotoFrame_Lcd_Close	(g_China ? (g_Big5 ? "關屏時間" : "关屏时间") : "Turn off Display")
#define ESID_Set_PhotoFrame_Lcd_Save	(g_China ? (g_Big5 ? "進入屏保時間" : "进入屏保时间") : "Starts In")
#define ESID_Set_KeyTone_ON				(g_China ? (g_Big5 ? "已啟用按鍵音" : "已启用按键音") : "Activate")
#define ESID_Set_KeyTone_OFF			(g_China ? (g_Big5 ? "未啟用按鍵音" : "未启用按键音") : "Deactivate")
#define ESID_Set_Talk_Volume			(g_China ? (g_Big5 ? "通話音量" : "通话音量") : "Talk Volume")
#define ESID_Set_Ring_Volume			(g_China ? (g_Big5 ? "振鈴音量" : "振铃音量") : "Ringing")
#define ESID_Time_Sec                   (g_China ? (g_Big5 ? "秒" : "秒") : "Sec")
#define ESID_Time_Hour                  (g_China ? (g_Big5 ? "小時" : "小时") : "Hour")
#define ESID_Time_Min                   (g_China ? (g_Big5 ? "分鐘" : "分钟") : "Min")
#define ESID_Time_Never                 (g_China ? (g_Big5 ? "從不" : "从不") : "Never")
#define ESID_Set_Pwd_Err				(g_China ? (g_Big5 ? "密碼錯誤，請重新輸入!" : "密码错误,请重新输入!") : "Wrong Password!")
#define ESID_Set_EnterUserPwd			(g_China ? (g_Big5 ? "請輸入用戶密碼" : "请输入用户密码") : "Enter User Password")
#define ESID_Set_EnterProjectPwd		(g_China ? (g_Big5 ? "請輸入工程密碼" : "请输入工程密码") : "Enter Installer Password")
#define ESID_Set_DoorPwd				(g_China ? (g_Big5 ? "開門密碼" : "开门密码") : "Unlock Password")
#define ESID_Set_SecurityPwd			(g_China ? (g_Big5 ? "安防密碼" : "安防密码") : "Security Password")
#define ESID_Set_Pass_DoorUser          (g_China ? (g_Big5 ? "住戶開門密碼" : "住户开门密码") : "Household Password")
#define ESID_Set_Pass_DoorServer        (g_China ? (g_Big5 ? "家政開門密碼" : "家政开门密码") : "Servant Password")
#define ESID_Set_Pass_AlarmUser         (g_China ? (g_Big5 ? "住戶安防密碼" : "住户安防密码") : "Household Password")
#define ESID_Set_Pass_AlarmServer       (g_China ? (g_Big5 ? "家政安防密碼" : "家政安防密码") : "Servant Password")
#define ESID_Set_Pass_Project			(g_China ? (g_Big5 ? "工程密碼" : "工程密码") : "Installer Password")
#define ESID_Set_Pass_Change_DoorUser   (g_China ? (g_Big5 ? "修改住戶開門密碼" : "修改住户开门密码") : "Change resident door-opening password")
#define ESID_Set_Pass_Change_AlarmUser  (g_China ? (g_Big5 ? "修改住戶安防密碼" : "修改住户安防密码") : "Change resident security password")
#define ESID_Set_Pass_New               (g_China ? (g_Big5 ? "新密碼" : "新密码") : "New:")
#define ESID_Set_Pass_Query             (g_China ? (g_Big5 ? "確認新密碼" : "确认新密码") : "Re-enter:")
#define ESID_Set_Link_LinkSet           (g_China ? (g_Big5 ? "聯動設置" : "联动设置") : "Interlocking")
#define ESID_Set_Link_AfLink            (g_China ? (g_Big5 ? "安防聯動" : "安防联动") : "Security Interlocking")
#define ESID_Set_Link_CallLink          (g_China ? (g_Big5 ? "對講聯動" : "对讲联动") : "Intercom Interlocking")
#define ESID_Set_Link_JDLink            (g_China ? (g_Big5 ? "家電聯動" : "家电联动") : "Household Interlocking")
#define ESID_Set_Line_Lyly_Mode_Audio   (g_China ? (g_Big5 ? "純留言模式" : "纯留言模式") : "Voice MSG")
#define ESID_Set_Line_Lyly_Mode_Both    (g_China ? (g_Big5 ? "留言照片模式" : "留言照片模式") : "Voice MSG & Image")
#define ESID_Set_Line_Lyly_Mode_Video   (g_China ? (g_Big5 ? "留影留言模式" : "留影留言模式") : "Video MSG")
#define ESID_Set_Line_Jd_Mode_Huike     (g_China ? (g_Big5 ? "會客模式" : "会客模式") : "Meeting")
#define ESID_Set_Line_Jd_Mode_Jiucan    (g_China ? (g_Big5 ? "就餐模式" : "就餐模式") : "Dining")
#define ESID_Set_Line_Jd_Mode_Yejian    (g_China ? (g_Big5 ? "夜間模式" : "夜间模式") : "Night")
#define ESID_Set_Line_Jd_Mode_Jiedian   (g_China ? (g_Big5 ? "節電模式" : "节电模式") : "Saving")
#define ESID_Set_Line_Jd_Mode_Putong    (g_China ? (g_Big5 ? "普通模式" : "普通模式") : "Regular")
#define ESID_Set_Ring_Lyly_default_tip  (g_China ? (g_Big5 ? "默認提示音" : "默认提示音") : "Default")
#define ESID_Set_Ring_Lyly_record_tip   (g_China ? (g_Big5 ? "錄製提示音" : "录制提示音") : "Customized")
#define ESID_Set_Ring_Ring              (g_China ? (g_Big5 ? "鈴聲" : "铃声") : "Ringtone")
#define ESID_Set_Ring_Hit               (g_China ? (g_Big5 ? "提示音" : "提示音") : "Tip Sound")
#define ESID_Set_Ring_Info              (g_China ? (g_Big5 ? "信息提示音" : "信息提示音") : "New Message")
#define ESID_Set_Ring_Alarm             (g_China ? (g_Big5 ? "預警提示音" : "预警提示音") : "Exit Delay Alert")
#define ESID_Set_Ring_RingSet           (g_China ? (g_Big5 ? "聲音設置" : "声音设置") : "Ringtone")
#define ESID_Set_Ring_CallRing          (g_China ? (g_Big5 ? "來電鈴聲" : "来电铃声") : "Ring Tone")
#define ESID_Set_Ring_Stair             (g_China ? (g_Big5 ? "梯口鈴聲" : "梯口铃声") : "Sec.Entry")
#define ESID_Set_Ring_Area              (g_China ? (g_Big5 ? "渠口鈴聲" : "区口铃声") : "Prim.Entry")
#define ESID_Set_Ring_Door              (g_China ? (g_Big5 ? "門前鈴聲" : "门前铃声") : "Doorway")
#define ESID_Set_Ring_Center            (g_China ? (g_Big5 ? "管理員機鈴聲" : "管理员机铃声") : "Admin")
#define ESID_Set_Ring_Room              (g_China ? (g_Big5 ? "住戶鈴聲" : "住户铃声") : "Resident")
#define ESID_Set_Ring_Fenji             (g_China ? (g_Big5 ? "分機鈴聲" : "分机铃声") : "Extension")
#define ESID_Set_User_AfGeLi            (g_China ? (g_Big5 ? "隔離設置" : "隔离设置") : "Bypass")
#define ESID_Set_User_AfJuFang          (g_China ? (g_Big5 ? "局防有效設置" : "局防有效设置") : "Night Mode")
#define ESID_Set_Prj_AfProperty         (g_China ? (g_Big5 ? "防區屬性" : "防区属性") : "Zone Property")
#define ESID_Set_User_AfGeLi_Open       (g_China ? (g_Big5 ? "防區隔離" : "防区隔离") : "Zone bypass")
#define ESID_Set_User_AfGeLi_Close      (g_China ? (g_Big5 ? "防區不隔離" : "防区不隔离") : "Zone non-bypass")
#define ESID_Set_User_AfJuFang_Close    (g_China ? (g_Big5 ? "局防無效" : "局防无效") : "Invalid local arm ")
#define ESID_Set_User_AfJuFang_Open     (g_China ? (g_Big5 ? "局防有效" : "局防有效") : "Valid local arm")
#define ESID_Set_UnUsed                 (g_China ? (g_Big5 ? "未啟用" : "未启用") : "Deactivate")
#define ESID_Set_Used                   (g_China ? (g_Big5 ? "啟用" : "启用") : "Activate")
#define ESID_Set_Null                   (g_China ? (g_Big5 ? "無" : "无") : "Null")
#define ESID_Set_Prj_ExtJD_Used         (g_China ? (g_Big5 ? "家電控制器已啟用" : "家电控制器已启用") : "Appliance Controller Enabled")
#define ESID_Set_Prj_ExtJD_Unused       (g_China ? (g_Big5 ? "家電控制器未啟用" : "家电控制器未启用") : "Appliance Controller Disabled")
#define ESID_Set_Prj_ExtDoor1_Unused    (g_China ? (g_Big5 ? "門前機一未啟用" : "门前机一未启用") : "Outdoor Phone1 Disabled")
#define ESID_Set_Prj_ExtDoor1_Used      (g_China ? (g_Big5 ? "門前機一已啟用" : "门前机一已启用") : "Outdoor Phone1 Enabled")
#define ESID_Set_Prj_ExtDoor2_Unused    (g_China ? (g_Big5 ? "門前機二未啟用" : "门前机二未启用") : "Outdoor Phone2 Disabled")
#define ESID_Set_Prj_ExtDoor2_Used      (g_China ? (g_Big5 ? "門前機二已啟用" : "门前机二已启用") : "Outdoor Phone2 Enabled")
#define ESID_Set_Prj_ExtStair_Used      (g_China ? (g_Big5 ? "通用梯口機已啟用" : "通用梯口机已启用") : "General Secondary Entry Station Enabled")
#define ESID_Set_Prj_ExtStair_Unused    (g_China ? (g_Big5 ? "通用梯口機未啟用" : "通用梯口机未启用") : "General Secondary Entry Station Disabled")
#define ESID_Set_Prj_Rule_Riser_Used    (g_China ? (g_Big5 ? "單元號長度有效" : "单元号长度有效") : "Valid Riser NO.")
#define ESID_Set_Prj_Rule_Riser_Unused  (g_China ? (g_Big5 ? "單元號長度無效" : "单元号长度无效") : "Invalid Riser NO.")
#define ESID_Set_Prj_Rule_Device_Wrong  (g_China ? (g_Big5 ? "設備編號格式錯誤" : "设备编号格式错误") : "Wrong Device NO.")
#define ESID_Set_Prj_Rule_Riser_Null    (g_China ? (g_Big5 ? "單元號長度不能為空" : "单元号长度不能为空") : "Pls Enter Riser NO. Length ")
#define ESID_Set_Prj_Rule_Room_Null     (g_China ? (g_Big5 ? "房號長度不能為空" : "房号长度不能为空") : "Pls Enter Room No. Length ")
#define ESID_Set_Prj_Rule_Stair_Null    (g_China ? (g_Big5 ? "梯號長度不能為空" : "梯号长度不能为空") : "Pls Enter Stair NO. Length")
#define ESID_Set_Prj_Rule_Riser_Err     (g_China ? (g_Big5 ? "單元號長度錯誤" : "单元号长度错误") : "Wrong Riser NO. Length")
#define ESID_Set_Prj_Rule_Room_Err	    (g_China ? (g_Big5 ? "房號長度錯誤" : "房号长度错误") : "Wrong Room No. Length")
#define ESID_Set_Prj_Rule_Stair_Err     (g_China ? (g_Big5 ? "梯號長度錯誤" : "梯号长度错误") : "Wrong Stair NO. Length")
#define ESID_Set_Prj_Rule_RuleAll_Err   (g_China ? (g_Big5 ? "總長度不能大於18" : "总长度不能大于18") : "Length should be less than 18")
#define ESID_Set_Prj_Rule_Len_Err       (g_China ? (g_Big5 ? "設備編號長度非法" : "设备编号长度非法") : "Illegal Length")
#define ESID_Set_Prj_Rule_RoomNo_Err    (g_China ? (g_Big5 ? "房號段全為0" : "房号段全为0") : "Room No. can not be all zero")
#define ESID_Set_Prj_IP_Address_Err		(g_China ? (g_Big5 ? "IP地址格式錯誤" : "IP地址格式错误") : "Wrong IP address format")
#define ESID_Set_Pwd_Two_Err			(g_China ? (g_Big5 ? "兩次密碼不相同" : "两次密码不相同") : "Inconsistent passwords!")
#define ESID_Set_Rtsp_Indoor_Camera     (g_China ? (g_Big5 ? "室內攝像頭" : "室内摄像头") : "Indoor Camera")
#define ESID_Set_Rtsp_Camera_Used       (g_China ? (g_Big5 ? "室內攝像頭已啟用" : "室内摄像头已启用") : "Indoor Camera Enabled")
#define ESID_Set_Rtsp_Camera_Unused     (g_China ? (g_Big5 ? "室內攝像頭未啟用" : "室内摄像头未启用") : "Indoor Camera Disabled")
#define ESID_Set_Data_Save_Err			(g_China ? (g_Big5 ? "數據保存失敗" : "数据保存失败") : "Data Save Error")
#define ESID_Set_Rtsp_Port_Null			(g_China ? (g_Big5 ? "端口號不能為空" : "端口号不能为空") : "Pls enter port number!")
#define ESID_Set_Rtsp_Channel_Null		(g_China ? (g_Big5 ? "視頻通道不能為空" : "视频通道不能为空") : "Pls enter channel number!")
#define ESID_Msg_Connect_Outtime        (g_China ? (g_Big5 ? "連接超時..." : "连接超时...") : "Connection timeout!")
#define ESID_Msg_Save_OK				(g_China ? (g_Big5 ? "保存成功" : "保存成功") : "OK!")
#define ESID_Msg_Save_Err				(g_China ? (g_Big5 ? "保存失敗" : "保存失败") : "Failed!")
#define ESID_Msg_Param_Save				(g_China ? (g_Big5 ? "參數有更改，是否保存？" : "参数有更改，是否保存？") : "There are some changes of parameters,sure to save it?")
#define ESID_Msg_Operat_OK				(g_China ? (g_Big5 ? "操作成功" : "操作成功") : "Operation successfully")
#define ESID_Msg_Operat_Err				(g_China ? (g_Big5 ? "操作失敗" : "操作失败") : "Operation failed")
#define ESID_Msg_Format                 (g_China ? (g_Big5 ? "確定是否清空" : "确定是否清空?") : "Sure to delete all?")

// IP协议转换器
#define ESID_Set_Unbounded				 (g_China ? (g_Big5 ? "ゼ竕﹚" : "未绑定") : "Unbounded")
#define ESID_Set_Bounded  				 (g_China ? (g_Big5 ? "竕﹚" : "已绑定") : "Binding")
#define ESID_Set_Generate_Binding_Code   (g_China ? (g_Big5 ? "生成綁定碼" : "生成绑定码") : "Binding Code")
#define ESID_Set_Binding_Code        	 (g_China ? (g_Big5 ? "綁定碼" : "绑定码") : "Binding Code")
#define ESID_Set_ExtenNo 				 (g_China ? (g_Big5 ? "分機" : "分机") : "EXT")
#define ESID_Set_Online				     (g_China ? (g_Big5 ? "在線" : "在线") : "OnLine")
#define ESID_Set_Offline 				 (g_China ? (g_Big5 ? "瞒絬" : "離線") : "UnLine")



// 网络门前机文字
#define ESID_Net_Door             		 (g_China ? (g_Big5 ? "網絡門前機" : "网络门前机") : "IP Camera")
#define ESID_Net_Door_First              (g_China ? (g_Big5 ? "網絡門前機一" : "网络门前机一") : "IP Camera 1")
#define ESID_Net_Door_Second             (g_China ? (g_Big5 ? "網絡門前機二" : "网络门前机二") : "IP Camera 2")
#define ESID_Net_Door_NetParam           (g_China ? (g_Big5 ? "網絡參數" : "网络参数") : "Network Parameters")
#define ESID_Net_Door_Get_MAC            (g_China ? (g_Big5 ? "是否獲取MAC？" : "是否获取MAC？") : "Sure to obtain MAC?")
#define ESID_Net_Door_Save_Reboot_Door   (g_China ? (g_Big5 ? "是否保存？保存后門前機將重啟" : "是否保存？保存后门前机将重启") : "Sure to save? Outdoor Camera will be rebooted after saving.")
#define ESID_Net_Door_Card_Delet_All     (g_China ? (g_Big5 ? "是否刪除網絡門前機的所有卡？" : "是否删除网络门前机的所有卡？") : "Sure to save? Outdoor Camera will be rebooted after saving.")


// 报警文字
#define ESID_Bj_Report_Type_Chufa       (g_China ? (g_Big5 ? "觸發報警" : "触发报警") : "Triggerred Alarm")
#define ESID_Bj_Fangqu					(g_China ? (g_Big5 ? "ň跋" : "防区") : "Zone")
#define ESID_Bj_SOS                     (g_China ? (g_Big5 ? "緊急求救" : "紧急求救") : "SOS")
#define ESID_Bj_Fire                    (g_China ? (g_Big5 ? "火災探測" : "火灾探测") : "Fire")
#define ESID_Bj_Gas                     (g_China ? (g_Big5 ? "瓦斯探測" : "瓦斯探测") : "Gas Leak")
#define ESID_Bj_Door                    (g_China ? (g_Big5 ? "大門入侵" : "大门入侵") : "Door")
#define ESID_Bj_Room                    (g_China ? (g_Big5 ? "客廳防護" : "客厅防护") : "Room")
#define ESID_Bj_Big_Window              (g_China ? (g_Big5 ? "窗戶1入侵" : "窗户1入侵") : "Window1")
#define ESID_Bj_Window                  (g_China ? (g_Big5 ? "窗戶2入侵" : "窗户2入侵") : "Window2")
#define ESID_Bj_Balcony                 (g_China ? (g_Big5 ? "陽台入侵" : "阳台入侵") : "Balcony")
#define ESID_Bj_Remote_Home_Used		(g_China ? (g_Big5 ? "啟用遠程撤防" : "启用远程撤防") : "Activate Remote Disarm(HOME)")
#define ESID_Bj_Remote_Home_Unused		(g_China ? (g_Big5 ? "未啟用遠程撤防" : "未启用远程撤防") : "Deactivate Remote Disarm(HOME)")
#define ESID_Bj_Remote_Out_Used         (g_China ? (g_Big5 ? "啟用遠程布防" : "启用远程布防") : "Activate Remote Arm(AWAY)")
#define ESID_Bj_Remote_Out_Unused       (g_China ? (g_Big5 ? "未啟用遠程布防" : "未启用远程布防") : "Deactivate Remote Arm(AWAY)")
#define ESID_Bj_Remote_Night_Used       (g_China ? (g_Big5 ? "啟用遠程局防" : "启用远程局防") : "Activate Remote Arm(NIGHT)")
#define ESID_Bj_Remote_Night_Unused     (g_China ? (g_Big5 ? "未啟用遠程局防" : "未启用远程局防") : "Deactivate Remote Arm(NIGHT)")
#define ESID_Bj_Set_24h_Unused			(g_China ? (g_Big5 ? "非24小時防區" : "非24小时防区") : "Non-24-hour zone")
#define ESID_Bj_Set_24h_Used			(g_China ? (g_Big5 ? "24小時防區" : "24小时防区") : "24-hour zone")
#define ESID_Bj_Set_UnSee				(g_China ? (g_Big5 ? "不可見" : "不可见") : "Invisible")
#define ESID_Bj_Set_See					(g_China ? (g_Big5 ? "可見" : "可见") : "Visible")
#define ESID_Bj_Set_UnHear				(g_China ? (g_Big5 ? "不可聽" : "不可听") : "Inaudible")
#define ESID_Bj_Set_Hear				(g_China ? (g_Big5 ? "可聽" : "可听") : "Audible")
#define ESID_Bj_Set_UnHear				(g_China ? (g_Big5 ? "不可聽" : "不可听") : "Inaudible")
#define ESID_Bj_Set_Hear				(g_China ? (g_Big5 ? "可聽" : "可听") : "Audible")
#define ESID_Bj_Tantou_Close            (g_China ? (g_Big5 ? "常閉" : "常闭") : "N/C")
#define ESID_Bj_Tantou_Open             (g_China ? (g_Big5 ? "常開" : "常开") : "N/O")
#define ESID_Bj_0Seconds                (g_China ? (g_Big5 ? "0秒" : "0秒") : "0 Sec")
#define ESID_Bj_30Seconds			    (g_China ? (g_Big5 ? "30秒" : "30秒") : "30Sec")
#define ESID_Bj_1Minute				    (g_China ? (g_Big5 ? "1分" : "1分") : "1Min")
#define ESID_Bj_2Minutes				(g_China ? (g_Big5 ? "2分" : "2分") : "2Min")
#define ESID_Bj_3Minutes				(g_China ? (g_Big5 ? "3分" : "3分") : "3Min")
#define ESID_Bj_4Minutes				(g_China ? (g_Big5 ? "4分" : "4分") : "4Min")
#define ESID_Bj_5Minutes				(g_China ? (g_Big5 ? "5分" : "5分") : "5Min")
#define ESID_Bj_10Minutes			    (g_China ? (g_Big5 ? "10分" : "10分") : "10Min")
#define ESID_Bj_15Minutes			    (g_China ? (g_Big5 ? "15分" : "15分") : "15Min")   
#define ESID_Bj_Fangqu1					(g_China ? (g_Big5 ? "防區1" : "防区1") : "Zone1")
#define ESID_Bj_Fangqu2					(g_China ? (g_Big5 ? "防區2" : "防区2") : "Zone2")
#define ESID_Bj_Fangqu3					(g_China ? (g_Big5 ? "防區3" : "防区3") : "Zone3")
#define ESID_Bj_Fangqu4					(g_China ? (g_Big5 ? "防區4" : "防区4") : "Zone4")
#define ESID_Bj_Fangqu5					(g_China ? (g_Big5 ? "防區5" : "防区5") : "Zone5")
#define ESID_Bj_Fangqu6					(g_China ? (g_Big5 ? "防區6" : "防区6") : "Zone6")
#define ESID_Bj_Fangqu7					(g_China ? (g_Big5 ? "防區7" : "防区7") : "Zone7")
#define ESID_Bj_Fangqu8					(g_China ? (g_Big5 ? "防區8" : "防区8") : "Zone8")



// 对讲文字
#define ESID_MainMonitor_Door			 (g_China ? (g_Big5 ? "監視門前" : "监视门前") : "Doorway")
#define ESID_MainMonitor_Stair			 (g_China ? (g_Big5 ? "監視梯口" : "监视梯口") : "Sec.Entry")
#define ESID_MainMonitor_Area			 (g_China ? (g_Big5 ? "監視區口" : "监视区口") : "Prim.Entry")
#define ESID_MainMonitor_Camera			 (g_China ? (g_Big5 ? "監視社區" : "监视社区") : "Public")
#define ESID_MainMonitor_Room			 (g_China ? (g_Big5 ? "家居監視" : "家居监视") : "Home")
#define ESID_DevManager                  (g_China ? (g_Big5 ? "管理員機" : "管理员机") : "Admin")
#define ESID_DevArea                     (g_China ? (g_Big5 ? "區口機" : "区口机") : "Primary Entry Terminal")
#define ESID_DevStair                    (g_China ? (g_Big5 ? "梯口機" : "梯口机") : "Secondary Entry Terminal")
#define ESID_DevRoom                     (g_China ? (g_Big5 ? "室內機" : "室内机") : "Indoor Phone")
#define ESID_DevPhone                    (g_China ? (g_Big5 ? "普通電話" : "普通电话") : "Phone")
#define ESID_DevDoorPhone                (g_China ? (g_Big5 ? "門前機" : "门前机") : "Outdoor Camera")
#define ESID_DevDoorNet                  (g_China ? (g_Big5 ? "網絡門前機" : "网络门前机") : "IP Outdoor Camera")
#define ESID_DevGateWay                  (g_China ? (g_Big5 ? "家庭子網路匝道" : "家庭网关") : "Gateway")
#define ESID_DevIPCamera                 (g_China ? (g_Big5 ? "IP Camera" : "IP Camera") : "IP Camera")
#define ESID_DevFenJi                    (g_China ? (g_Big5 ? "分機" : "分机") : "Extension")
#define ESID_DevMaster                   (g_China ? (g_Big5 ? "主機" : "主机") : "Main Phone")
#define ESID_Inter_EnterUserNo			 (g_China ? (g_Big5 ? "請輸入房號" : "请输入房号") : "Enter apartmrnt number")
#define ESID_Inter_EnterFJNo			 (g_China ? (g_Big5 ? "請輸入1位分機號" : "请输入位1分机号") : "Pls enter a number")
#define ESID_Inter_WrongNo               (g_China ? (g_Big5 ? "輸入號碼錯誤!" : "输入号码错误") : "Wrong number input")
#define ESID_Inter_NoNotFound            (g_China ? (g_Big5 ? "查無此號!" : "查无此号") : "Number not found")
#define ESID_Inter_Connecting            (g_China ? (g_Big5 ? "連接中..." : "连接中...") : "Connecting...")
#define ESID_Inter_BeCalling             (g_China ? (g_Big5 ? "來電中..." : "来电中...") : "Calling from...")
#define ESID_Inter_Calling               (g_China ? (g_Big5 ? "呼叫中..." : "呼叫中...") : "Calling...")
#define ESID_Inter_Talking               (g_China ? (g_Big5 ? "通話中..." : "通话中...") : "Talking...")
#define ESID_Inter_RecordHinting         (g_China ? (g_Big5 ? "留言提示中..." : "留言提示中...") : "Ready to record…")
#define ESID_Inter_Recording             (g_China ? (g_Big5 ? "留言中..." : "留言中...") : "Recording...")
#define ESID_Inter_UnlockSuccess         (g_China ? (g_Big5 ? "開鎖成功" : "开 锁 成 功") : "Door Unlocked!")
#define ESID_Inter_UnlockFailed          (g_China ? (g_Big5 ? "開鎖失敗" : "开 锁 失 败") : "Failed!")
#define ESID_Inter_CaptureSuccess        (g_China ? (g_Big5 ? "拍照成功" : "抓 拍 成 功") : "OK!")
#define ESID_Inter_CaptureFailed         (g_China ? (g_Big5 ? "抓拍失敗" : "抓 拍 失 败") : "Failed!")
#define ESID_Inter_ManagerNoSet          (g_China ? (g_Big5 ? "此中心未設置" : "此中心未设置") : "The admin unit IP not set yet!")
#define ESID_Inter_Search                (g_China ? (g_Big5 ? "加載中,請稍後..." : "加载中,请稍候...") : "Uploading and please wait a moment")
#define ESID_Inter_Monitor               (g_China ? (g_Big5 ? "監視中..." : "监视中...") : "Monitoring...")
#define ESID_Inter_busy                  (g_China ? (g_Big5 ? "對方忙" : "对方忙") : "The line is busy.")
#define ESID_Inter_ConnFailed            (g_China ? (g_Big5 ? "連接失敗" : "连接失败") : "Connection failed")
#define ESID_Inter_SearchFailed          (g_China ? (g_Big5 ? "未搜索到設備" : "未搜索到设备") : "Failed")

// 家电文字
#define ESID_Jd_Mode_Out				 (g_China ? (g_Big5 ? "外出" : "外出") : "Away")
#define ESID_Jd_Mode_Mode                (g_China ? (g_Big5 ? "模式" : "模式") : "Mode")
#define ESID_Jd_LouShang                 (g_China ? (g_Big5 ? "樓上" : "楼上") : "Upstairs")
#define ESID_Jd_Deng                     (g_China ? (g_Big5 ? "燈" : "灯") : "Light")
#define ESID_Jd_ChuangLian               (g_China ? (g_Big5 ? "窗簾" : "窗帘") : "Curtain")
#define ESID_Jd_KongTiaoJi               (g_China ? (g_Big5 ? "空調機" : "空调机") : "A/C 1")
#define ESID_Jd_ChaZuo1                  (g_China ? (g_Big5 ? "插座1" : "插座1") : "Socket 1")
#define ESID_Jd_Meiqi                    (g_China ? (g_Big5 ? "煤氣" : "煤气") : "Gas")


// 公共文字
#define ESID_Bj_Query_Del_Rec_One        (g_China ? (g_Big5 ? "是否刪除該數據?" : "是否删除该数据?") : "Sure to delete it?")  
#define ESID_Bj_Query_Del_Rec_All        (g_China ? (g_Big5 ? "是否清空數據?" : "是否清空数据?") : "Sure to delete all?") 
#define ESID_Unregistered           	 (g_China ? (g_Big5 ? "未註冊" : "未注册") : "Unregistered")
#define ESID_Registered           		 (g_China ? (g_Big5 ? "已註冊" : "已注册") : "Registered")

typedef enum
{
	SID_NULL = 1000,
	// 设置类字符ID
	SID_Set_PhotoFrame_Lcd_Close,
	SID_Set_PhotoFrame_Lcd_Save,
	SID_Set_KeyTone_ON,
	SID_Set_KeyTone_OFF,
	SID_Set_Talk_Volume,
	SID_Set_Ring_Volume,
	SID_Time_Sec,
	SID_Time_Hour,
	SID_Time_Min,
	SID_Time_Never,
	SID_Set_Pwd_Err,
	SID_Set_EnterUserPwd,
	SID_Set_EnterProjectPwd,
	SID_Set_DoorPwd,
	SID_Set_SecurityPwd,
	SID_Set_Pass_DoorUser,
	SID_Set_Pass_DoorServer,
	SID_Set_Pass_AlarmUser,
	SID_Set_Pass_AlarmServer,
	SID_Set_Pass_Project,
	SID_Set_Pass_Change_DoorUser,
	SID_Set_Pass_Change_AlarmUser,
	SID_Set_Pass_New,
	SID_Set_Pass_Query,
	SID_Set_Link_LinkSet,
	SID_Set_Link_AfLink,
	SID_Set_Link_CallLink,
	SID_Set_Link_JDLink,
	SID_Set_Line_Lyly_Mode_Audio,
	SID_Set_Line_Lyly_Mode_Both,
	SID_Set_Line_Lyly_Mode_Video,
	SID_Set_Line_Jd_Mode_Huike,
	SID_Set_Line_Jd_Mode_Jiucan,
	SID_Set_Line_Jd_Mode_Yejian,
	SID_Set_Line_Jd_Mode_Jiedian,
	SID_Set_Line_Jd_Mode_Putong,
	SID_Set_Ring_Lyly_default_tip,
	SID_Set_Ring_Lyly_record_tip,
	SID_Set_Ring_Ring,
	SID_Set_Ring_Hit,
	SID_Set_Ring_Info,
	SID_Set_Ring_Alarm,
	SID_Set_Ring_RingSet,
	SID_Set_Ring_CallRing,
	SID_Set_Ring_Stair,
	SID_Set_Ring_Area,
	SID_Set_Ring_Door,
	SID_Set_Ring_Center,
	SID_Set_Ring_Room,
	SID_Set_Ring_Fenji,
	SID_Set_User_AfGeLi,
	SID_Set_User_AfJuFang,
	SID_Set_Prj_AfProperty,
	SID_Set_User_AfGeLi_Open,
	SID_Set_User_AfGeLi_Close,
	SID_Set_User_AfJuFang_Close,
	SID_Set_User_AfJuFang_Open,
	SID_Set_UnUsed,
	SID_Set_Used,
	SID_Set_Null,
	SID_Set_Prj_ExtJD_Used,
	SID_Set_Prj_ExtJD_Unused,
	SID_Set_Prj_ExtDoor1_Unused,
	SID_Set_Prj_ExtDoor1_Used,
	SID_Set_Prj_ExtDoor2_Unused,
	SID_Set_Prj_ExtDoor2_Used,
	SID_Set_Prj_ExtStair_Used,
	SID_Set_Prj_ExtStair_Unused,
	SID_Set_Prj_Rule_Riser_Used,
	SID_Set_Prj_Rule_Riser_Unused,
	SID_Set_Prj_Rule_Device_Wrong,
	SID_Set_Prj_Rule_Riser_Null,
	SID_Set_Prj_Rule_Room_Null,
	SID_Set_Prj_Rule_Stair_Null,
	SID_Set_Prj_Rule_Riser_Err,
	SID_Set_Prj_Rule_Room_Err,
	SID_Set_Prj_Rule_Stair_Err,
	SID_Set_Prj_Rule_RuleAll_Err,
	SID_Set_Prj_Rule_Len_Err,
	SID_Set_Prj_Rule_RoomNo_Err,
	SID_Set_Prj_IP_Address_Err,
	SID_Set_Pwd_Two_Err,
	SID_Set_Rtsp_Indoor_Camera,
	SID_Set_Rtsp_Camera_Used,
	SID_Set_Rtsp_Camera_Unused,
	SID_Set_Data_Save_Err,
	SID_Set_Rtsp_Port_Null,
	SID_Set_Rtsp_Channel_Null,
	SID_Msg_Connect_Outtime,
	SID_Msg_Save_OK,
	SID_Msg_Save_Err,
	SID_Msg_Param_Save,
	SID_Msg_Operat_OK,
	SID_Msg_Operat_Err,
	SID_Net_Door_Card_Delet_All,
	SID_Msg_Format,

	// IP协议转换器
	SID_Set_Unbounded,
	SID_Set_Bounded,
	SID_Set_Generate_Binding_Code,
	SID_Set_Binding_Code,
	SID_Set_ExtenNo,
	SID_Set_Online,
	SID_Set_Offline, 	


	// 网络门前机文字
	SID_Net_Door, 
	SID_Net_Door_First,
	SID_Net_Door_Second,
	SID_Net_Door_NetParam,
	SID_Net_Door_Get_MAC,
	SID_Net_Door_Save_Reboot_Door,


	// 报警文字
	SID_Bj_Report_Type_Chufa,
	SID_Bj_Fangqu,
	SID_Bj_SOS,
	SID_Bj_Fire,
	SID_Bj_Gas,
	SID_Bj_Door,
	SID_Bj_Room,
	SID_Bj_Big_Window,
	SID_Bj_Window,
	SID_Bj_Balcony,
	SID_Bj_Remote_Home_Used,
	SID_Bj_Remote_Home_Unused,
	SID_Bj_Remote_Out_Used,
	SID_Bj_Remote_Out_Unused,
	SID_Bj_Remote_Night_Used,
	SID_Bj_Remote_Night_Unused,
	SID_Bj_Set_24h_Unused,
	SID_Bj_Set_24h_Used,
	SID_Bj_Set_UnSee,
	SID_Bj_Set_See,
	SID_Bj_Set_UnHear,
	SID_Bj_Set_Hear,
	SID_Bj_Tantou_Close,
	SID_Bj_Tantou_Open, 
	SID_Bj_0Seconds,
	SID_Bj_30Seconds,
	SID_Bj_1Minute,
	SID_Bj_2Minutes,
	SID_Bj_3Minutes,
	SID_Bj_4Minutes,
	SID_Bj_5Minutes,
	SID_Bj_10Minutes,
	SID_Bj_15Minutes,
	SID_Bj_Fangqu1,
	SID_Bj_Fangqu2,
	SID_Bj_Fangqu3,
	SID_Bj_Fangqu4,
	SID_Bj_Fangqu5,
	SID_Bj_Fangqu6,
	SID_Bj_Fangqu7,
	SID_Bj_Fangqu8,

	// 对讲文字
	SID_MainMonitor_Door,
	SID_MainMonitor_Stair,
	SID_MainMonitor_Area,
	SID_MainMonitor_Camera,
	SID_MainMonitor_Home,
	SID_DevManager,
	SID_DevArea,
	SID_DevStair,
	SID_DevRoom,
	SID_DevPhone,
	SID_DevDoorPhone,
	SID_DevDoorNet,
	SID_DevGateWay,
	SID_DevIPCamera,
	SID_DevFenJi,
	SID_DevMaster,
	SID_Inter_EnterUserNo,
	SID_Inter_EnterFJNo,
	SID_Inter_WrongNo,
	SID_Inter_NoNotFound,
	SID_Inter_Connecting,
	SID_Inter_BeCalling,
	SID_Inter_Calling,
	SID_Inter_Talking,
	SID_Inter_RecordHinting,
	SID_Inter_Recording,
	SID_Inter_UnlockSuccess,
	SID_Inter_UnlockFailed,
	SID_Inter_CaptureSuccess,
	SID_Inter_CaptureFailed,
	SID_Inter_ManagerNoSet,
	SID_Inter_Search,
	SID_Inter_Monitor,
	SID_Inter_busy,
	SID_Inter_ConnFailed,
	SID_Inter_SearchFailed,

	// 家电文字
	SID_Jd_Mode_Out,
	SID_Jd_Mode_Mode,
	SID_Jd_LouShang,
	SID_Jd_Deng,
	SID_Jd_ChuangLian,
	SID_Jd_KongTiaoJi,
	SID_Jd_ChaZuo1,
	SID_Jd_Meiqi,

	
	// 公共文字
	SID_Bj_Query_Del_Rec_One,
	SID_Bj_Query_Del_Rec_All,
	SID_Unregistered,
	SID_Registered,

	SID_MAX
}STRID_E;

typedef enum _FontType
{
	Font12X12 = 12,
	Font16X16 = 16,
	Font20X20 = 20,
	Font24X24 = 24
}FontType;

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
const char * get_str(STRID_E id);

#ifdef __cplusplus
}
#endif
#endif
