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
#define ESID_Set_Ring_Stair             (g_China ? (g_Big5 ? "梯口鈴聲" : "梯口铃声") : "Public Entry")
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
#define ESID_Set_Rtsp_PTZ_Used			(g_China ? (g_Big5 ? "PTZ控制已啟用" : "PTZ控制已啟用") : "PTZ control enabled")
#define ESID_Set_Rtsp_PTZ_Unused        (g_China ? (g_Big5 ? "PTZ控制未啟用" : "PTZ控制未啟用") : "PTZ control disabled")
#define ESID_Set_Data_Save_Err			(g_China ? (g_Big5 ? "數據保存失敗" : "数据保存失败") : "Data Save Error")
#define ESID_Set_Rtsp_Port_Null			(g_China ? (g_Big5 ? "端口號不能為空" : "端口号不能为空") : "Pls enter port number!")
#define ESID_Set_Rtsp_Channel_Null		(g_China ? (g_Big5 ? "視頻通道不能為空" : "视频通道不能为空") : "Pls enter channel number!")
#define ESID_Set_Ring_Tips_Fail		    (g_China ? (g_Big5 ? "請錄製留言提示音" : "请录制留言提示音") : "Please record messages prompt tone.")	
#define ESID_Msg_Connect_Outtime        (g_China ? (g_Big5 ? "連接超時..." : "连接超时...") : "Connection timeout!")
#define ESID_Msg_Save_OK				(g_China ? (g_Big5 ? "保存成功" : "保存成功") : "OK!")
#define ESID_Msg_Save_Err				(g_China ? (g_Big5 ? "保存失敗" : "保存失败") : "Failed!")
#define ESID_Msg_Param_Save				(g_China ? (g_Big5 ? "參數有更改，是否保存？" : "参数有更改，是否保存？") : "There are some changes of parameters,sure to save it?")
#define ESID_Msg_Operat_OK				(g_China ? (g_Big5 ? "操作成功" : "操作成功") : "Operation successfully")
#define ESID_Msg_Operat_Err				(g_China ? (g_Big5 ? "操作失敗" : "操作失败") : "Operation failed")
#define ESID_Msg_Format                 (g_China ? (g_Big5 ? "確定是否清空" : "确定是否清空?") : "Sure to delete all?")
#define ESID_Msg_Recover_OK             (g_China ? (g_Big5 ? "是否保存？保存系統將重啟！" : "是否保存？保存系统将重启!") : "Sure to save? Save system will restart!")
#define ESID_Msg_Return_FactorySet      (g_China ? (g_Big5 ? "恢復出廠設置后系統將重啟！" : "恢复出厂后系统将重启！") : "Reboot after factory restored!")
#define ESID_Msg_Param_Suer_Save		(g_China ? (g_Big5 ? "是否保存設置？" : "是否保存设置？") : "Sure to save settings?")
#define ESID_Msg_IP_Unline              (g_China ? (g_Big5 ? "IP協議轉換器未在線" : "IP协议转换器未在线") : "IP Converter Offline.")
#define ESID_Msg_Obtain_Jd_False        (g_China ? (g_Big5 ? "獲取家電信息失敗！" : "获取家电信息失败！") : "Obtain appliance info. failed!")
#define ESID_Msg_JD_Mode_Unused         (g_China ? (g_Big5 ? "家電控制器未啟用" : "家电控制器未启用") : "Appliance controller disabled")

// IP协议转换器
#define ESID_Set_Unbounded				 (g_China ? (g_Big5 ? "未綁定" : "未绑定") : "Unbounded")
#define ESID_Set_Bounded  				 (g_China ? (g_Big5 ? "已綁定" : "已绑定") : "Binding")
#define ESID_Set_Generate_Binding_Code   (g_China ? (g_Big5 ? "生成綁定碼" : "生成绑定码") : "Binding Code")
#define ESID_Set_Binding_Code        	 (g_China ? (g_Big5 ? "綁定碼" : "绑定码") : "Binding Code")
#define ESID_Set_ExtenNo 				 (g_China ? (g_Big5 ? "分機" : "分机") : "EXT")
#define ESID_Set_Online				     (g_China ? (g_Big5 ? "在線" : "在线") : "OnLine")
#define ESID_Set_Offline 				 (g_China ? (g_Big5 ? "離線" : "离线") : "UnLine")
#define ESID_Set_Bind_OK				 (g_China ? (g_Big5 ? "綁定成功" : "绑定成功") : "OK")
#define ESID_Set_Bind_Fail				 (g_China ? (g_Big5 ? "綁定失敗" : "绑定失败") : "Failed")

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
#define ESID_Bj_Bengjikongzhi           (g_China ? (g_Big5 ? "本機控制" : "本机控制") : "Local Control") 
#define ESID_Bj_Fengjikongzhi           (g_China ? (g_Big5 ? "分機控制" : "分机控制") : "Sub-phone Control") 
#define ESID_Bj_Bf_Home                 (g_China ? (g_Big5 ? "在家" : "在家") : "Home")
#define ESID_Bj_Bf_Out                  (g_China ? (g_Big5 ? "外出" : "外出") : "Away")
#define ESID_Bj_Bf_Night                (g_China ? (g_Big5 ? "夜間" : "夜间") : "Night")
#define ESID_Bj_Already_Bufang          (g_China ? (g_Big5 ? "點擊單獨的防區可進行單獨布防" : "点击单独的防区可进行单独布防") : "Tap a zone to arm it")
#define ESID_Bj_Alarm_Cannot_Bf         (g_China ? (g_Big5 ? "正在報警不能不防" : "正在报警不能布防") : "Alarming now. Cannot arm!")
#define ESID_Bj_Touch_Cannot_Bf         (g_China ? (g_Big5 ? "觸發不能不防" : "触发不能布防") : "triggerred.Cannot arm!")
#define ESID_Bj_Alarm_Cannot_Qc         (g_China ? (g_Big5 ? "正在報警，無法操作" : "正在报警，无法操作") : "Zone triggerred.Cannot operate!")
#define ESID_Set_Enter_Alarm_Pwd        (g_China ? (g_Big5 ? "請輸入安防密碼" : "请输入安防密码") : "Enter Security Password")
#define ESID_Bj_Alarm_Clear_data        (g_China ? (g_Big5 ? "是否清空數據" : "是否清空数据") : "Sure to empty data?")
#define ESID_Bj_Mode_Unused             (g_China ? (g_Big5 ? "报警模块未啟用" : "报警模块未启用") : "Alarm module inactivated!")
#define ESID_Bj_Set_Err                 (g_China ? (g_Big5 ? "主機不在撤防狀態，不能設置!" : "主机不在撤防状态，不能设置!") : "Please disarm the system first!")


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
#define ESID_Jd_Set						 (g_China ? (g_Big5 ? "家電設置" : "家电设置") : "Home Appliances")
#define ESID_Jd_Lamp_Set                 (g_China ? (g_Big5 ? "燈光設置" : "灯光设置") : "Light")
#define ESID_Jd_KongTiao_Set             (g_China ? (g_Big5 ? "空調設置" : "空调设置") : "A/C")
#define ESID_Jd_ChuangLian_Set           (g_China ? (g_Big5 ? "窗簾設置" : "窗帘设置") : "Curtain")
#define ESID_Jd_Dianyuan_Set             (g_China ? (g_Big5 ? "電源設置" : "电源设置") : "Socket")
#define ESID_Jd_Meiqi_Set                (g_China ? (g_Big5 ? "煤氣設置" : "煤气设置") : "Gas")
#define ESID_Jd_Lamp                     (g_China ? (g_Big5 ? "燈光" : "灯光") : "Light ")
#define ESID_Jd_KongTiao                 (g_China ? (g_Big5 ? "空調" : "空调") : "A/C ")
#define ESID_Jd_Dianyuan                 (g_China ? (g_Big5 ? "電源" : "电源") : "Socket ")
#define ESID_Jd_Meiqi                    (g_China ? (g_Big5 ? "煤氣" : "煤气") : "Gas ")
#define ESID_Jd_None                     (g_China ? (g_Big5 ? "礚" : "无") : "None")
#define ESID_Jd_Dev_Set_UnAdjust         (g_China ? (g_Big5 ? "不可調" : "不可调") : "Unadjustable")
#define ESID_Jd_Dev_Set_Adjust           (g_China ? (g_Big5 ? "可調" : "可调") : "Adjustable")
#define ESID_Jd_Factory                  (g_China ? (g_Big5 ? "家電廠商" : "家电厂商") : "Makers")
#define ESID_Jd_Set_Dev_Noname           (g_China ? (g_Big5 ? "請設置名稱！" : "请设置名称！") : "Please enter the device name!")
#define ESID_Jd_Set_Scene_Noname         (g_China ? (g_Big5 ? "請設置名稱！" : "请设置名称！") : "Please enter the scene name!")
#define ESID_Jd_Set_Name_Err             (g_China ? (g_Big5 ? "名稱重複，保存失敗！" : "名称重复,保存失败!") : "Description Repeated!")
#define ESID_Jd_Set_Adr_Err              (g_China ? (g_Big5 ? "地址重複，不能設置！" : "地址重复,不能设置！") : "Address Repeated!")
#define ESID_Jd_Set_Adr_Max_Err          (g_China ? (g_Big5 ? "你輸入的地址超出範圍！" : "你输入的地址超出范围！") : "Address is out of range!")
#define ESID_Jd_Set_Dev_Name1            (g_China ? (g_Big5 ? "設備位置" : "设备位置") : "Address")
#define ESID_Jd_Set_Dev_Name2            (g_China ? (g_Big5 ? "設備名稱" : "设备类型") : "Type")
#define ESID_Jd_LouShang                 (g_China ? (g_Big5 ? "樓上" : "楼上") : "Upstairs")
#define ESID_Jd_LouXia                   (g_China ? (g_Big5 ? "樓下" : "楼下") : "Downstairs")
#define ESID_Jd_WuNei                    (g_China ? (g_Big5 ? "屋內" : "屋内") : "Indoor")
#define ESID_Jd_WuWai                    (g_China ? (g_Big5 ? "屋外" : "屋外") : "Outdoor")
#define ESID_Jd_TianTai                  (g_China ? (g_Big5 ? "天台" : "天台") : "Roof")
#define ESID_Jd_GeLou                    (g_China ? (g_Big5 ? "閣樓" : "阁楼") : "Attic")
#define ESID_Jd_HuaYuan                  (g_China ? (g_Big5 ? "花園" : "花园") : "Garden")
#define ESID_Jd_DiJiao                   (g_China ? (g_Big5 ? "地窖" : "地窖") : "Cellar")
#define ESID_Jd_TianJin                  (g_China ? (g_Big5 ? "天井" : "天井") : "Patio")
#define ESID_Jd_WuYan                    (g_China ? (g_Big5 ? "屋簷" : "屋檐") : "Roof")
#define ESID_Jd_QianTing                 (g_China ? (g_Big5 ? "前廳" : "前庭") : "Vestibule")
#define ESID_Jd_HouYuan                  (g_China ? (g_Big5 ? "後院" : "后院") : "Backyard")
#define ESID_Jd_DianTiJian               (g_China ? (g_Big5 ? "電梯間" : "电梯间") : "Elevator")
#define ESID_Jd_TingCheFang              (g_China ? (g_Big5 ? "停車房" : "停车房") : "Parking Room")
#define ESID_Jd_DaMen                    (g_China ? (g_Big5 ? "大門" : "大门") : "Gate")
#define ESID_Jd_QianMen                  (g_China ? (g_Big5 ? "前門" : "前门") : "Front Door")
#define ESID_Jd_ZhongMen                 (g_China ? (g_Big5 ? "中門" : "中门") : "MidDoor")
#define ESID_Jd_PangMen                  (g_China ? (g_Big5 ? "傍門" : "傍门") : "Side Door")
#define ESID_Jd_HouMen                   (g_China ? (g_Big5 ? "後門" : "后门") : "Backdoor")
#define ESID_Jd_CeMen                    (g_China ? (g_Big5 ? "側門" : "侧门") : "Side gate")
#define ESID_Jd_BianMen                  (g_China ? (g_Big5 ? "邊門" : "边门") : "Wicket")
#define ESID_Jd_HenMen                   (g_China ? (g_Big5 ? "橫門" : "横门") : "Fence Door")
#define ESID_Jd_YanTai                   (g_China ? (g_Big5 ? "陽台" : "阳台") : "Balcony")
#define ESID_Jd_Zhoulan                  (g_China ? (g_Big5 ? "走廊" : "走廊") : "Corridor")
#define ESID_Jd_GuoDao                   (g_China ? (g_Big5 ? "過道" : "过道") : "Aisle")
#define ESID_Jd_LouTi                    (g_China ? (g_Big5 ? "樓梯" : "楼梯") : "Stairs")
#define ESID_Jd_TiKou                    (g_China ? (g_Big5 ? "梯口" : "梯口") : "Stairway")
#define ESID_Jd_TiJian                   (g_China ? (g_Big5 ? "梯間" : "梯间") : "Staircase")
#define ESID_Jd_TiXia                    (g_China ? (g_Big5 ? "梯下" : "梯下") : "Down the Ladder")
#define ESID_Jd_KeTing                   (g_China ? (g_Big5 ? "客廳" : "客厅") : "Drawing Room")
#define ESID_Jd_DaTing                   (g_China ? (g_Big5 ? "大廳" : "大厅") : "Hall")
#define ESID_Jd_ZhuTing                  (g_China ? (g_Big5 ? "主廳" : "主厅") : "Main Hall")
#define ESID_Jd_CanTing                  (g_China ? (g_Big5 ? "餐廳" : "餐厅") : "Restaurants")
#define ESID_Jd_FanTing                  (g_China ? (g_Big5 ? "飯廳" : "饭厅") : "Dining Hall")
#define ESID_Jd_MenTing                  (g_China ? (g_Big5 ? "門廳" : "门厅") : "Foyer")
#define ESID_Jd_ZhuWoShi                 (g_China ? (g_Big5 ? "主臥室" : "主卧室") : "Master Bedroom")
#define ESID_Jd_ZhuRenFang               (g_China ? (g_Big5 ? "主人房" : "主人房") : "Host Room")
#define ESID_Jd_WoShi                    (g_China ? (g_Big5 ? "臥室" : "卧室") : "Bedroom")
#define ESID_Jd_ShuiFang                 (g_China ? (g_Big5 ? "睡房" : "睡房") : "Bedroom")
#define ESID_Jd_FangJianA                (g_China ? (g_Big5 ? "房間A" : "房间A") : "RoomA")
#define ESID_Jd_FangJianB                (g_China ? (g_Big5 ? "房間B" : "房间B") : "RoomB")
#define ESID_Jd_FangJianC                (g_China ? (g_Big5 ? "房間C" : "房间C") : "RoomC")
#define ESID_Jd_FangJianD                (g_China ? (g_Big5 ? "房間D" : "房间D") : "RoomD")
#define ESID_Jd_FangJianE                (g_China ? (g_Big5 ? "房間E" : "房间E") : "RoomE")
#define ESID_Jd_ChuFang                  (g_China ? (g_Big5 ? "廚房" : "厨房") : "Kitchen")
#define ESID_Jd_ShuFang                  (g_China ? (g_Big5 ? "書房" : "书房") : "Study")
#define ESID_Jd_KeFang                   (g_China ? (g_Big5 ? "客廳" : "客房") : "Rooms")
#define ESID_Jd_ZhuYuShi                 (g_China ? (g_Big5 ? "主浴室" : "主浴室") : "Master Bathroom")
#define ESID_Jd_YuShi                    (g_China ? (g_Big5 ? "浴室" : "浴室") : "Bathroom")
#define ESID_Jd_ErTongFang               (g_China ? (g_Big5 ? "兒童房" : "儿童房") : "Children Room")
#define ESID_Jd_GongRenFang              (g_China ? (g_Big5 ? "工人房" : "工人房") : "Servant Room")
#define ESID_Jd_DuoYongTuJian            (g_China ? (g_Big5 ? "多用途間" : "多用途间") : "Multi-purpose rooms")
#define ESID_Jd_QiJuShi                  (g_China ? (g_Big5 ? "起居室" : "起居室") : "Living Room")
#define ESID_Jd_YouLeShi                 (g_China ? (g_Big5 ? "遊樂室" : "游乐室") : "Playroom")
#define ESID_Jd_ShuXiShi                 (g_China ? (g_Big5 ? "梳洗室" : "梳洗室") : "Toilet Room")
#define ESID_Jd_WeiShengJian             (g_China ? (g_Big5 ? "衛生間" : "卫生间") : "Toilet")
#define ESID_Jd_XieMaoJian               (g_China ? (g_Big5 ? "鞋帽間" : "鞋帽间") : "Home Locker Room")
#define ESID_Jd_GongZuoJian              (g_China ? (g_Big5 ? "工作間" : "工作间") : "Workplace")
#define ESID_Jd_DiXiaShi                 (g_China ? (g_Big5 ? "地下室" : "地下室") : "Basement")
#define ESID_Jd_Set_Scene                (g_China ? (g_Big5 ? "情景設置" : "情景设置") : "Scene Mode")
#define ESID_Jd_Mode_Huike               (g_China ? (g_Big5 ? "會客模式" : "会客模式") : "Meeting")
#define ESID_Jd_Mode_Jiucan              (g_China ? (g_Big5 ? "就餐模式" : "就餐模式") : "Dining")
#define ESID_Jd_Mode_Yejian              (g_China ? (g_Big5 ? "夜間模式" : "夜间模式") : "Night")
#define ESID_Jd_Mode_Jiedian             (g_China ? (g_Big5 ? "節點模式" : "节电模式") : "Saving")
#define ESID_Jd_Mode_Putong              (g_China ? (g_Big5 ? "普通模式" : "普通模式") : "Regular")
#define ESID_Jd_Mode_Self                (g_China ? (g_Big5 ? "情景模式" : "情景模式") : "Scene Mode")
#define ESID_Jd_Mode_Set_Name1           (g_China ? (g_Big5 ? "情景名字" : "情景名字") : "Scene Name")
#define ESID_Jd_Mode_Set_Name2           (g_China ? (g_Big5 ? "情景類型" : "情景类型") : "Scene Type")
#define ESID_Jd_Mode_Out                 (g_China ? (g_Big5 ? "外出" : "外出") : "Away")
#define ESID_Jd_Mode_Cook                (g_China ? (g_Big5 ? "烹飪" : "烹饪") : "Cooking")
#define ESID_Jd_Mode_first               (g_China ? (g_Big5 ? "早茶" : "早茶") : "Breakfast")
#define ESID_Jd_Mode_Afater              (g_China ? (g_Big5 ? "午餐" : "午餐") : "Lunch")
#define ESID_Jd_Mode_Night               (g_China ? (g_Big5 ? "晚宴" : "晚宴") : "Supper")
#define ESID_Jd_Mode_Bed                 (g_China ? (g_Big5 ? "就寢" : "就寝") : "Bed")
#define ESID_Jd_Mode_QiYe                (g_China ? (g_Big5 ? "起夜" : "起夜") : "Mid-night")
#define ESID_Jd_Mode_Up                  (g_China ? (g_Big5 ? "起床" : "起床") : "Wake-up")
#define ESID_Jd_Mode_Muyu                (g_China ? (g_Big5 ? "沐浴" : "沐浴") : "Bath")
#define ESID_Jd_Mode_Tea                 (g_China ? (g_Big5 ? "品茶" : "品茶") : "Tea")
#define ESID_Jd_Mode_Drink               (g_China ? (g_Big5 ? "飲酒" : "饮酒") : "Bibulous")
#define ESID_Jd_Mode_Read                (g_China ? (g_Big5 ? "閱讀" : "阅读") : "Reading")
#define ESID_Jd_Mode_Work                (g_China ? (g_Big5 ? "工作" : "工作") : "Working")
#define ESID_Jd_Mode_Juhui               (g_China ? (g_Big5 ? "聚會" : "聚会") : "Party")
#define ESID_Jd_Mode_Play                (g_China ? (g_Big5 ? "棋牌" : "棋牌") : "Chess&Cards")
#define ESID_Jd_Mode_Film                (g_China ? (g_Big5 ? "影院" : "影院") : "Theatre")
#define ESID_Jd_Mode_Sing                (g_China ? (g_Big5 ? "歡唱" : "欢唱") : "Carolling")
#define ESID_Jd_Mode_Storang             (g_China ? (g_Big5 ? "健身" : "健身") : "Exercising")
#define ESID_Jd_Mode_Wenxi               (g_China ? (g_Big5 ? "溫馨" : "温馨") : "Cozy")
#define ESID_Jd_Mode_Langman             (g_China ? (g_Big5 ? "浪漫" : "浪漫") : "Romatic")
#define ESID_Jd_Mode_Jiqing              (g_China ? (g_Big5 ? "激情" : "激情") : "Passionate")
#define ESID_Jd_Mode_Dianya              (g_China ? (g_Big5 ? "典雅" : "典雅") : "Elegant")
#define ESID_Jd_Mode_Chlid               (g_China ? (g_Big5 ? "童趣" : "童趣") : "Childish")
#define ESID_Jd_Mode_Chenwen             (g_China ? (g_Big5 ? "沉穩" : "沉稳") : "Sedate")
#define ESID_Jd_Mode_Think               (g_China ? (g_Big5 ? "靜思" : "静思") : "Calm")
#define ESID_Jd_Mode_Cool                (g_China ? (g_Big5 ? "清涼" : "清凉") : "Cool")
#define ESID_Jd_Mode_Warm                (g_China ? (g_Big5 ? "溫暖" : "温暖") : "Warm")
#define ESID_Jd_Mode_Mode                (g_China ? (g_Big5 ? "模式" : "模式") : "Mode")
#define ESID_Jd_Mode_Sence               (g_China ? (g_Big5 ? "情景" : "情景") : "Scene")
#define ESID_Jd_Mode_Qingdiao            (g_China ? (g_Big5 ? "情調" : "情调") : "Sentiment")
#define ESID_Jd_Mode_Fenwei              (g_China ? (g_Big5 ? "氛圍" : "氛围") : "Atmosphere")
#define ESID_Jd_Mode_Time                (g_China ? (g_Big5 ? "時光" : "时光") : "Time")
#define ESID_Jd_Mode_Changjing           (g_China ? (g_Big5 ? "場景" : "场景") : "Occasion")
#define ESID_Jd_Deng                     (g_China ? (g_Big5 ? "燈" : "灯") : "Light")
#define ESID_Jd_TiaoGuangDeng            (g_China ? (g_Big5 ? "調光燈" : "调光灯") : "Dimming Lights")
#define ESID_Jd_BaiChiDeng               (g_China ? (g_Big5 ? "白熾燈" : "白炽灯") : "Incandescent Lamp")
#define ESID_Jd_GuangGuan                (g_China ? (g_Big5 ? "光管" : "光管") : "Lightpipe")
#define ESID_Jd_DiaoDeng                 (g_China ? (g_Big5 ? "吊燈" : "吊灯") : "Chandelier")
#define ESID_Jd_HuaDeng                  (g_China ? (g_Big5 ? "花燈" : "花灯") : "Lantern")
#define ESID_Jd_BiDeng                   (g_China ? (g_Big5 ? "壁燈" : "壁灯") : "Wall Lamp")
#define ESID_Jd_SheDeng                  (g_China ? (g_Big5 ? "射燈" : "射灯") : "Spotlight")
#define ESID_Jd_TaiDeng                  (g_China ? (g_Big5 ? "檯燈" : "台灯") : "Table Lamp")
#define ESID_Jd_LouDiDeng                (g_China ? (g_Big5 ? "落地燈" : "落地灯") : "Floor Lamp")
#define ESID_Jd_ZhaoMingDeng             (g_China ? (g_Big5 ? "照明燈" : "照明灯") : "Lighting Lamp")
#define ESID_Jd_ShiYingDeng              (g_China ? (g_Big5 ? "石英燈" : "石英灯") : "Quartz Lamp")
#define ESID_Jd_YeDeng                   (g_China ? (g_Big5 ? "夜燈" : "夜灯") : "Nightlights")
#define ESID_Jd_ZhuanshiDEng             (g_China ? (g_Big5 ? "裝飾燈" : "装饰灯") : "Deco Lamp")
#define ESID_Jd_DianZhiSuo               (g_China ? (g_Big5 ? "電子鎖" : "电子锁") : "E-Lock")
#define ESID_Jd_XiaoduWanGui             (g_China ? (g_Big5 ? "消毒碗櫃" : "消毒碗柜") : "Sterilizer")
#define ESID_Jd_KongQiJinHua             (g_China ? (g_Big5 ? "空氣淨化" : "空气净化") : "Air Purifier")
#define ESID_Jd_YinShuiJi                (g_China ? (g_Big5 ? "飲水機" : "饮水机") : "Water Dispenser")
#define ESID_Jd_KongTiaoJi               (g_China ? (g_Big5 ? "空調機" : "空调机") : "A/C 1")
#define ESID_Jd_LenQiJi                  (g_China ? (g_Big5 ? "冷氣機" : "冷气机") : "A/C 2")
#define ESID_Jd_CouYouJi                 (g_China ? (g_Big5 ? "抽油機" : "抽油机") : "A/C 3")
#define ESID_Jd_NuanFenJi                (g_China ? (g_Big5 ? "暖風機" : "暖风机") : "A/C 4")
#define ESID_Jd_DianReTan                (g_China ? (g_Big5 ? "電熱毯" : "电热毯") : "Electric Blanket")
#define ESID_Jd_ReShuiLu                 (g_China ? (g_Big5 ? "熱水爐" : "热水炉") : "Water Heater")
#define ESID_Jd_DianFanBao               (g_China ? (g_Big5 ? "電飯煲" : "电饭煲") : "Rice Cooker")
#define ESID_Jd_DianZhiWaGuo             (g_China ? (g_Big5 ? "電子瓦鍋" : "电子瓦锅") : "Electric Stewpot")
#define ESID_Jd_WeiBoLu                  (g_China ? (g_Big5 ? "微波爐" : "微波炉") : "Microwave Oven")
#define ESID_Jd_XiYiJi                   (g_China ? (g_Big5 ? "洗衣機" : "洗衣机") : "Socket 6")
#define ESID_Jd_DianShiJi                (g_China ? (g_Big5 ? "電視機" : "电视机") : "Socket 1")
#define ESID_Jd_LuXiangJi                (g_China ? (g_Big5 ? "錄像機" : "录像机") : "VCR")
#define ESID_Jd_YingXiang                (g_China ? (g_Big5 ? "音響" : "音响") : "Stereo")
#define ESID_Jd_DianNao                  (g_China ? (g_Big5 ? "電腦" : "电脑") : "PC")
#define ESID_Jd_FengShan                 (g_China ? (g_Big5 ? "風扇" : "风扇") : "Fan")
#define ESID_Jd_CD                       (g_China ? (g_Big5 ? "CD" : "CD") : "CD")
#define ESID_Jd_LD                       (g_China ? (g_Big5 ? "LD" : "LD") : "LD")
#define ESID_Jd_VCD                      (g_China ? (g_Big5 ? "VCD" : "VCD") : "VCD")
#define ESID_Jd_DVD                      (g_China ? (g_Big5 ? "DVD" : "DVD") : "DVD")
#define ESID_Jd_ChaZuo1                  (g_China ? (g_Big5 ? "插座1" : "插座1") : "Socket 1")
#define ESID_Jd_ChaZuo2                  (g_China ? (g_Big5 ? "插座2" : "插座2") : "Socket 2")
#define ESID_Jd_ChaZuo3                  (g_China ? (g_Big5 ? "插座3" : "插座3") : "Socket 3")
#define ESID_Jd_ChaZuo4                  (g_China ? (g_Big5 ? "插座4" : "插座4") : "Socket 4")
#define ESID_Jd_ChaZuo5                  (g_China ? (g_Big5 ? "插座5" : "插座5") : "Socket 5")
#define ESID_Jd_XiaoDuSheBe              (g_China ? (g_Big5 ? "消毒設備" : "消毒设备") : "Disinfectant Apparatus")
#define ESID_Jd_SettingMusic             (g_China ? (g_Big5 ? "背景音樂" : "背景音乐") : "Background Music")
#define ESID_Jd_ChuangLian               (g_China ? (g_Big5 ? "窗簾" : "窗帘") : "Curtain")
#define ESID_Jd_JuanLian                 (g_China ? (g_Big5 ? "捲簾" : "卷帘") : "Shutter")
#define ESID_Jd_DianDongBuLian           (g_China ? (g_Big5 ? "電動布簾" : "电动布帘") : "Electric Curtain")
#define ESID_Jd_DianDongShaLian          (g_China ? (g_Big5 ? "電動紗簾" : "电动纱帘") : "Electric Gauze")
#define ESID_Jd_Deng1                    (g_China ? (g_Big5 ? "燈1" : "灯1") : "Light1")
#define ESID_Jd_Deng2                    (g_China ? (g_Big5 ? "燈2" : "灯2") : "Light2")
#define ESID_Jd_Deng3                    (g_China ? (g_Big5 ? "燈3" : "灯3") : "Light3")
#define ESID_Jd_Deng4                    (g_China ? (g_Big5 ? "燈4" : "灯4") : "Light4")
#define ESID_Jd_Deng5                    (g_China ? (g_Big5 ? "燈5" : "灯5") : "Light5")
#define ESID_Jd_Deng6                    (g_China ? (g_Big5 ? "燈6" : "灯6") : "Light6")
#define ESID_Jd_Deng7                    (g_China ? (g_Big5 ? "燈7" : "灯7") : "Light7")
#define ESID_Jd_Deng8                    (g_China ? (g_Big5 ? "燈8" : "灯8") : "Light8")
#define ESID_Jd_Deng9                    (g_China ? (g_Big5 ? "燈9" : "灯9") : "Light9")
#define ESID_Jd_Deng10                   (g_China ? (g_Big5 ? "燈10" : "灯10") : "Light10")
#define ESID_Jd_TiaoGuangDeng1           (g_China ? (g_Big5 ? "調光燈1" : "调光灯1") : "Dimming Lights1")
#define ESID_Jd_TiaoGuangDeng2           (g_China ? (g_Big5 ? "調光燈2" : "调光灯2") : "Dimming Lights2")
#define ESID_Jd_TiaoGuangDeng3           (g_China ? (g_Big5 ? "調光燈3" : "调光灯3") : "Dimming Lights3")
#define ESID_Jd_TiaoGuangDeng4           (g_China ? (g_Big5 ? "調光燈4" : "调光灯4") : "Dimming Lights4")
#define ESID_Jd_TiaoGuangDeng5           (g_China ? (g_Big5 ? "調光燈5" : "调光灯5") : "Dimming Lights5")
#define ESID_Jd_TiaoGuangDeng6           (g_China ? (g_Big5 ? "調光燈6" : "调光灯6") : "Dimming Lights6")
#define ESID_Jd_TiaoGuangDeng7           (g_China ? (g_Big5 ? "調光燈7" : "调光灯7") : "Dimming Lights7")
#define ESID_Jd_TiaoGuangDeng8           (g_China ? (g_Big5 ? "調光燈8" : "调光灯8") : "Dimming Lights8")
#define ESID_Jd_TiaoGuangDeng9           (g_China ? (g_Big5 ? "調光燈9" : "调光灯9") : "Dimming Lights9")
#define ESID_Jd_TiaoGuangDeng10          (g_China ? (g_Big5 ? "調光燈10" : "调光灯10") : "Dimming Lights10")
#define ESID_Jd_BaiChiDeng1              (g_China ? (g_Big5 ? "白熾燈1" : "白炽灯1") : "Incandescent Lamp1")
#define ESID_Jd_BaiChiDeng2              (g_China ? (g_Big5 ? "白熾燈2" : "白炽灯2") : "Incandescent Lamp2")
#define ESID_Jd_BaiChiDeng3              (g_China ? (g_Big5 ? "白熾燈3" : "白炽灯3") : "Incandescent Lamp3")
#define ESID_Jd_BaiChiDeng4              (g_China ? (g_Big5 ? "白熾燈4" : "白炽灯4") : "Incandescent Lamp4")
#define ESID_Jd_BaiChiDeng5              (g_China ? (g_Big5 ? "白熾燈5" : "白炽灯5") : "Incandescent Lamp5")
#define ESID_Jd_BaiChiDeng6              (g_China ? (g_Big5 ? "白熾燈6" : "白炽灯6") : "Incandescent Lamp6")
#define ESID_Jd_BaiChiDeng7              (g_China ? (g_Big5 ? "白熾燈7" : "白炽灯7") : "Incandescent Lamp7")
#define ESID_Jd_BaiChiDeng8              (g_China ? (g_Big5 ? "白熾燈8" : "白炽灯8") : "Incandescent Lamp8")
#define ESID_Jd_BaiChiDeng9              (g_China ? (g_Big5 ? "白熾燈9" : "白炽灯9") : "Incandescent Lamp9")
#define ESID_Jd_BaiChiDeng10             (g_China ? (g_Big5 ? "白熾燈10" : "白炽灯10") : "Incandescent Lamp10")
#define ESID_Jd_GuangGuan1               (g_China ? (g_Big5 ? "燈管1" : "光管1") : "Lightpipe1")
#define ESID_Jd_GuangGuan2               (g_China ? (g_Big5 ? "燈管2" : "光管2") : "Lightpipe2")
#define ESID_Jd_GuangGuan3               (g_China ? (g_Big5 ? "燈管3" : "光管3") : "Lightpipe3")
#define ESID_Jd_GuangGuan4               (g_China ? (g_Big5 ? "燈管4" : "光管4") : "Lightpipe4")
#define ESID_Jd_GuangGuan5               (g_China ? (g_Big5 ? "燈管5" : "光管5") : "Lightpipe5")
#define ESID_Jd_GuangGuan6               (g_China ? (g_Big5 ? "燈管6" : "光管6") : "Lightpipe6")
#define ESID_Jd_GuangGuan7               (g_China ? (g_Big5 ? "燈管7" : "光管7") : "Lightpipe7")
#define ESID_Jd_GuangGuan8               (g_China ? (g_Big5 ? "燈管8" : "光管8") : "Lightpipe8")
#define ESID_Jd_GuangGuan9               (g_China ? (g_Big5 ? "燈管9" : "光管9") : "Lightpipe9")
#define ESID_Jd_GuangGuan10              (g_China ? (g_Big5 ? "燈管10" : "光管10") : "Lightpipe10")
#define ESID_Jd_DiaoDeng1                (g_China ? (g_Big5 ? "吊燈1" : "吊灯1") : "Chandelier1")
#define ESID_Jd_DiaoDeng2                (g_China ? (g_Big5 ? "吊燈2" : "吊灯2") : "Chandelier2")
#define ESID_Jd_DiaoDeng3                (g_China ? (g_Big5 ? "吊燈3" : "吊灯3") : "Chandelier3")
#define ESID_Jd_DiaoDeng4                (g_China ? (g_Big5 ? "吊燈4" : "吊灯4") : "Chandelier4")
#define ESID_Jd_DiaoDeng5                (g_China ? (g_Big5 ? "吊燈5" : "吊灯5") : "Chandelier5")
#define ESID_Jd_DiaoDeng6                (g_China ? (g_Big5 ? "吊燈6" : "吊灯6") : "Chandelier6")
#define ESID_Jd_DiaoDeng7                (g_China ? (g_Big5 ? "吊燈7" : "吊灯7") : "Chandelier7")
#define ESID_Jd_DiaoDeng8                (g_China ? (g_Big5 ? "吊燈8" : "吊灯8") : "Chandelier8")
#define ESID_Jd_DiaoDeng9                (g_China ? (g_Big5 ? "吊燈9" : "吊灯9") : "Chandelier9")
#define ESID_Jd_DiaoDeng10               (g_China ? (g_Big5 ? "吊燈10" : "吊灯10") : "Chandelier10")
#define ESID_Jd_HuaDeng1                 (g_China ? (g_Big5 ? "花燈1" : "花灯1") : "Lantern1")
#define ESID_Jd_HuaDeng2                 (g_China ? (g_Big5 ? "花燈2" : "花灯2") : "Lantern2")
#define ESID_Jd_HuaDeng3                 (g_China ? (g_Big5 ? "花燈3" : "花灯3") : "Lantern3")
#define	ESID_Jd_HuaDeng4                 (g_China ? (g_Big5 ? "花燈4" : "花灯4") : "Lantern4")
#define ESID_Jd_HuaDeng5                 (g_China ? (g_Big5 ? "花燈5" : "花灯5") : "Lantern5")
#define ESID_Jd_HuaDeng6                 (g_China ? (g_Big5 ? "花燈6" : "花灯6") : "Lantern6")
#define ESID_Jd_HuaDeng7                 (g_China ? (g_Big5 ? "花燈7" : "花灯7") : "Lantern7")
#define ESID_Jd_HuaDeng8                 (g_China ? (g_Big5 ? "花燈8" : "花灯8") : "Lantern8")
#define ESID_Jd_HuaDeng9                 (g_China ? (g_Big5 ? "花燈9" : "花灯9") : "Lantern9")
#define ESID_Jd_HuaDeng10                (g_China ? (g_Big5 ? "花燈10" : "花灯10") : "Lantern10")
#define ESID_Jd_BiDeng1                  (g_China ? (g_Big5 ? "壁燈1" : "壁灯1") : "Wall Lamp1")
#define ESID_Jd_BiDeng2                  (g_China ? (g_Big5 ? "壁燈2" : "壁灯2") : "Wall Lamp2")
#define ESID_Jd_BiDeng3                  (g_China ? (g_Big5 ? "壁燈3" : "壁灯3") : "Wall Lamp3")
#define ESID_Jd_BiDeng4                  (g_China ? (g_Big5 ? "壁燈4" : "壁灯4") : "Wall Lamp4")
#define ESID_Jd_BiDeng5                  (g_China ? (g_Big5 ? "壁燈5" : "壁灯5") : "Wall Lamp5")
#define ESID_Jd_BiDeng6                  (g_China ? (g_Big5 ? "壁燈6" : "壁灯6") : "Wall Lamp6")
#define ESID_Jd_BiDeng7                  (g_China ? (g_Big5 ? "壁燈7" : "壁灯7") : "Wall Lamp7")
#define ESID_Jd_BiDeng8                  (g_China ? (g_Big5 ? "壁燈8" : "壁灯8") : "Wall Lamp8")
#define ESID_Jd_BiDeng9                  (g_China ? (g_Big5 ? "壁燈9" : "壁灯9") : "Wall Lamp9")
#define ESID_Jd_BiDeng10                 (g_China ? (g_Big5 ? "壁燈10" : "壁灯10") : "Wall Lamp10")
#define ESID_Jd_SheDeng1                 (g_China ? (g_Big5 ? "射燈1" : "射灯1") : "Spotlight1")
#define ESID_Jd_SheDeng2                 (g_China ? (g_Big5 ? "射燈2" : "射灯2") : "Spotlight2")
#define ESID_Jd_SheDeng3                 (g_China ? (g_Big5 ? "射燈3" : "射灯3") : "Spotlight3")
#define ESID_Jd_SheDeng4                 (g_China ? (g_Big5 ? "射燈4" : "射灯4") : "Spotlight4")
#define ESID_Jd_SheDeng5                 (g_China ? (g_Big5 ? "射燈5" : "射灯5") : "Spotlight5")
#define ESID_Jd_SheDeng6                 (g_China ? (g_Big5 ? "射燈6" : "射灯6") : "Spotlight6")
#define ESID_Jd_SheDeng7                 (g_China ? (g_Big5 ? "射燈7" : "射灯7") : "Spotlight7")
#define ESID_Jd_SheDeng8                 (g_China ? (g_Big5 ? "射燈8" : "射灯8") : "Spotlight8")
#define ESID_Jd_SheDeng9                 (g_China ? (g_Big5 ? "射燈9" : "射灯9") : "Spotlight9")
#define ESID_Jd_SheDeng10                (g_China ? (g_Big5 ? "射燈10" : "射灯10") : "Spotlight10")
#define ESID_Jd_TaiDeng1                 (g_China ? (g_Big5 ? "檯燈1" : "台灯1") : "Table Lamp1")
#define ESID_Jd_TaiDeng2                 (g_China ? (g_Big5 ? "檯燈2" : "台灯2") : "Table Lamp2")
#define ESID_Jd_TaiDeng3                 (g_China ? (g_Big5 ? "檯燈3" : "台灯3") : "Table Lamp3")
#define ESID_Jd_TaiDeng4                 (g_China ? (g_Big5 ? "檯燈4" : "台灯4") : "Table Lamp4")
#define ESID_Jd_TaiDeng5                 (g_China ? (g_Big5 ? "檯燈5" : "台灯5") : "Table Lamp5")
#define ESID_Jd_TaiDeng6                 (g_China ? (g_Big5 ? "檯燈6" : "台灯6") : "Table Lamp6")
#define ESID_Jd_TaiDeng7                 (g_China ? (g_Big5 ? "檯燈7" : "台灯7") : "Table Lamp7")
#define ESID_Jd_TaiDeng8                 (g_China ? (g_Big5 ? "檯燈8" : "台灯8") : "Table Lamp8")
#define ESID_Jd_TaiDeng9                 (g_China ? (g_Big5 ? "檯燈9" : "台灯9") : "Table Lamp9")
#define ESID_Jd_TaiDeng10                (g_China ? (g_Big5 ? "檯燈10" : "台灯10") : "Table Lamp10")
#define ESID_Jd_LouDiDeng1               (g_China ? (g_Big5 ? "落地燈1" : "落地灯1") : "Floor Lamp1")
#define ESID_Jd_LouDiDeng2               (g_China ? (g_Big5 ? "落地燈2" : "落地灯2") : "Floor Lamp2")
#define ESID_Jd_LouDiDeng3               (g_China ? (g_Big5 ? "落地燈3" : "落地灯3") : "Floor Lamp3")
#define ESID_Jd_LouDiDeng4               (g_China ? (g_Big5 ? "落地燈4" : "落地灯4") : "Floor Lamp4")
#define ESID_Jd_LouDiDeng5               (g_China ? (g_Big5 ? "落地燈5" : "落地灯5") : "Floor Lamp5")
#define ESID_Jd_LouDiDeng6               (g_China ? (g_Big5 ? "落地燈6" : "落地灯6") : "Floor Lamp6")
#define ESID_Jd_LouDiDeng7               (g_China ? (g_Big5 ? "落地燈7" : "落地灯7") : "Floor Lamp7")
#define ESID_Jd_LouDiDeng8               (g_China ? (g_Big5 ? "落地燈8" : "落地灯8") : "Floor Lamp8")
#define ESID_Jd_LouDiDeng9               (g_China ? (g_Big5 ? "落地燈9" : "落地灯9") : "Floor Lamp9")
#define ESID_Jd_LouDiDeng10              (g_China ? (g_Big5 ? "落地燈10" : "落地灯10") : "Floor Lamp10")
#define ESID_Jd_ZhaoMingDeng1            (g_China ? (g_Big5 ? "照明燈1" : "照明灯1") : "Lighting Lamp1")
#define ESID_Jd_ZhaoMingDeng2            (g_China ? (g_Big5 ? "照明燈2" : "照明灯2") : "Lighting Lamp2")
#define ESID_Jd_ZhaoMingDeng3            (g_China ? (g_Big5 ? "照明燈3" : "照明灯3") : "Lighting Lamp3")
#define ESID_Jd_ZhaoMingDeng4            (g_China ? (g_Big5 ? "照明燈4" : "照明灯4") : "Lighting Lamp4")
#define ESID_Jd_ZhaoMingDeng5            (g_China ? (g_Big5 ? "照明燈5" : "照明灯5") : "Lighting Lamp5")
#define ESID_Jd_ZhaoMingDeng6            (g_China ? (g_Big5 ? "照明燈6" : "照明灯6") : "Lighting Lamp6")
#define ESID_Jd_ZhaoMingDeng7            (g_China ? (g_Big5 ? "照明燈7" : "照明灯7") : "Lighting Lamp7")
#define ESID_Jd_ZhaoMingDeng8            (g_China ? (g_Big5 ? "照明燈8" : "照明灯8") : "Lighting Lamp8")
#define ESID_Jd_ZhaoMingDeng9            (g_China ? (g_Big5 ? "照明燈9" : "照明灯9") : "Lighting Lamp9")
#define ESID_Jd_ZhaoMingDeng10           (g_China ? (g_Big5 ? "照明燈10" : "照明灯10") : "Lighting Lamp10")
#define ESID_Jd_ShiYingDeng1             (g_China ? (g_Big5 ? "石英燈1" : "石英灯1") : "Quartz Lamp1")
#define ESID_Jd_ShiYingDeng2             (g_China ? (g_Big5 ? "石英燈2" : "石英灯2") : "Quartz Lamp2")
#define ESID_Jd_ShiYingDeng3             (g_China ? (g_Big5 ? "石英燈3" : "石英灯3") : "Quartz Lamp3")
#define ESID_Jd_ShiYingDeng4             (g_China ? (g_Big5 ? "石英燈4" : "石英灯4") : "Quartz Lamp4")
#define ESID_Jd_ShiYingDeng5             (g_China ? (g_Big5 ? "石英燈5" : "石英灯5") : "Quartz Lamp5")
#define ESID_Jd_ShiYingDeng6             (g_China ? (g_Big5 ? "石英燈6" : "石英灯6") : "Quartz Lamp6")
#define ESID_Jd_ShiYingDeng7             (g_China ? (g_Big5 ? "石英燈7" : "石英灯7") : "Quartz Lamp7")
#define ESID_Jd_ShiYingDeng8             (g_China ? (g_Big5 ? "石英燈8" : "石英灯8") : "Quartz Lamp8")
#define ESID_Jd_ShiYingDeng9             (g_China ? (g_Big5 ? "石英燈9" : "石英灯9") : "Quartz Lamp9")
#define ESID_Jd_ShiYingDeng10            (g_China ? (g_Big5 ? "石英燈10" : "石英灯10") : "Quartz Lamp10")
#define ESID_Jd_YeDeng1                  (g_China ? (g_Big5 ? "夜燈1" : "夜灯1") : "Nightlights1")
#define ESID_Jd_YeDeng2                  (g_China ? (g_Big5 ? "夜燈2" : "夜灯2") : "Nightlights2")
#define ESID_Jd_YeDeng3                  (g_China ? (g_Big5 ? "夜燈3" : "夜灯3") : "Nightlights3")
#define ESID_Jd_YeDeng4                  (g_China ? (g_Big5 ? "夜燈4" : "夜灯4") : "Nightlights4")
#define ESID_Jd_YeDeng5                  (g_China ? (g_Big5 ? "夜燈5" : "夜灯5") : "Nightlights5")
#define ESID_Jd_YeDeng6                  (g_China ? (g_Big5 ? "夜燈6" : "夜灯6") : "Nightlights6")
#define ESID_Jd_YeDeng7                  (g_China ? (g_Big5 ? "夜燈7" : "夜灯7") : "Nightlights7")
#define ESID_Jd_YeDeng8                  (g_China ? (g_Big5 ? "夜燈8" : "夜灯8") : "Nightlights8")
#define ESID_Jd_YeDeng9                  (g_China ? (g_Big5 ? "夜燈9" : "夜灯9") : "Nightlights9")
#define ESID_Jd_YeDeng10                 (g_China ? (g_Big5 ? "夜燈10" : "夜灯10") : "Nightlights10")
#define ESID_Jd_ZhuanshiDEng1            (g_China ? (g_Big5 ? "裝飾燈1" : "装饰灯1") : "Deco Lamp1")
#define ESID_Jd_ZhuanshiDEng2            (g_China ? (g_Big5 ? "裝飾燈2" : "装饰灯2") : "Deco Lamp2")
#define ESID_Jd_ZhuanshiDEng3            (g_China ? (g_Big5 ? "裝飾燈3" : "装饰灯3") : "Deco Lamp3")
#define ESID_Jd_ZhuanshiDEng4            (g_China ? (g_Big5 ? "裝飾燈4" : "装饰灯4") : "Deco Lamp4")
#define ESID_Jd_ZhuanshiDEng5            (g_China ? (g_Big5 ? "裝飾燈5" : "装饰灯5") : "Deco Lamp5")
#define ESID_Jd_ZhuanshiDEng6            (g_China ? (g_Big5 ? "裝飾燈6" : "装饰灯6") : "Deco Lamp6")
#define ESID_Jd_ZhuanshiDEng7            (g_China ? (g_Big5 ? "裝飾燈7" : "装饰灯7") : "Deco Lamp7")
#define ESID_Jd_ZhuanshiDEng8            (g_China ? (g_Big5 ? "裝飾燈8" : "装饰灯8") : "Deco Lamp8")
#define ESID_Jd_ZhuanshiDEng9            (g_China ? (g_Big5 ? "裝飾燈9" : "装饰灯9") : "Deco Lamp9")
#define ESID_Jd_ZhuanshiDEng10           (g_China ? (g_Big5 ? "裝飾燈10" : "装饰灯10") : "Deco Lamp10")
#define ESID_Jd_Button_Cold              (g_China ? (g_Big5 ? "製冷" : "制冷") : "Cool")
#define ESID_Jd_Button_Warm              (g_China ? (g_Big5 ? "製暖" : "制暖") : "Heat")
#define ESID_Jd_Button_Mode              (g_China ? (g_Big5 ? "模式" : "模式") : "Mode")
#define ESID_Jd_KongTiao_Refre           (g_China ? (g_Big5 ? "清爽" : "清爽") : "Fresh")
#define ESID_Jd_KongTiao_Warm            (g_China ? (g_Big5 ? "溫暖" : "温暖") : "Warm")
#define ESID_Jd_KongTiao_Leisure         (g_China ? (g_Big5 ? "休閒" : "休闲") : "Casual")
#define ESID_Jd_KongTiao_Sleep           (g_China ? (g_Big5 ? "睡眠" : "睡眠") : "Sleep")
#define ESID_Jd_KongTiao_Close           (g_China ? (g_Big5 ? "關閉" : "关闭") : "OFF")



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
	SID_Set_Rtsp_PTZ_Used,
	SID_Set_Rtsp_PTZ_Unused,
	SID_Set_Data_Save_Err,
	SID_Set_Rtsp_Port_Null,
	SID_Set_Rtsp_Channel_Null,
	SID_Set_Ring_Tips_Fail,
	SID_Msg_Connect_Outtime,
	SID_Msg_Save_OK,
	SID_Msg_Save_Err,
	SID_Msg_Param_Save,
	SID_Msg_Operat_OK,
	SID_Msg_Operat_Err,
	SID_Net_Door_Card_Delet_All,
	SID_Msg_Format,
	SID_Msg_Recover_OK,
	SID_Msg_Return_FactorySet,
	SID_Msg_Param_Suer_Save,
	SID_Msg_IP_Unline,
	SID_Msg_Obtain_Jd_False,
	SID_Msg_JD_Mode_Unused,

	// IP协议转换器
	SID_Set_Unbounded,
	SID_Set_Bounded,
	SID_Set_Generate_Binding_Code,
	SID_Set_Binding_Code,
	SID_Set_ExtenNo,
	SID_Set_Online,
	SID_Set_Offline, 
	SID_Set_Bind_OK,
	SID_Set_Bind_Fail,	


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
	SID_Bj_Bengjikongzhi,
	SID_Bj_Fengjikongzhi,
	SID_Bj_Bf_Home,
	SID_Bj_Bf_Out,
	SID_Bj_Bf_Night,
	SID_Bj_Already_Bufang,
	SID_Bj_Alarm_Cannot_Bf,
	SID_Bj_Touch_Cannot_Bf,
	SID_Bj_Alarm_Cannot_Qc,
	SID_Set_Enter_Alarm_Pwd,
	SID_Bj_Alarm_Clear_data,
	SID_Bj_Mode_Unused,
	SID_Bj_Set_Err, 


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
	SID_Jd_Set,
	SID_Jd_Lamp_Set,
	SID_Jd_KongTiao_Set,
	SID_Jd_ChuangLian_Set,
	SID_Jd_Dianyuan_Set,
	SID_Jd_Meiqi_Set,
	SID_Jd_Lamp,
	SID_Jd_KongTiao,
	SID_Jd_Dianyuan,
	SID_Jd_Meiqi,  
	SID_Jd_None,
	SID_Jd_Dev_Set_UnAdjust,
	SID_Jd_Dev_Set_Adjust,
	SID_Jd_Factory,
	SID_Jd_Set_Dev_Noname,
	SID_Jd_Set_Scene_Noname,
	SID_Jd_Set_Name_Err,
	SID_Jd_Set_Adr_Err,
	SID_Jd_Set_Adr_Max_Err,
	SID_Jd_Set_Dev_Name1,
	SID_Jd_Set_Dev_Name2,
	SID_Jd_LouShang,
	SID_Jd_LouXia,
	SID_Jd_WuNei,
	SID_Jd_WuWai,
	SID_Jd_TianTai,
	SID_Jd_GeLou,
	SID_Jd_HuaYuan,
	SID_Jd_DiJiao,
	SID_Jd_TianJin,
	SID_Jd_WuYan,
	SID_Jd_QianTing,
	SID_Jd_HouYuan,
	SID_Jd_DianTiJian,
	SID_Jd_TingCheFang,
	SID_Jd_DaMen,
	SID_Jd_QianMen,
	SID_Jd_ZhongMen,
	SID_Jd_PangMen,
	SID_Jd_HouMen,
	SID_Jd_CeMen,
	SID_Jd_BianMen,
	SID_Jd_HenMen,
	SID_Jd_YanTai,
	SID_Jd_Zhoulan,
	SID_Jd_GuoDao,
	SID_Jd_LouTi,
	SID_Jd_TiKou,
	SID_Jd_TiJian,
	SID_Jd_TiXia,
	SID_Jd_KeTing,
	SID_Jd_DaTing,
	SID_Jd_ZhuTing,
	SID_Jd_CanTing,
	SID_Jd_FanTing,
	SID_Jd_MenTing,
	SID_Jd_ZhuWoShi,
	SID_Jd_ZhuRenFang,
	SID_Jd_WoShi,
	SID_Jd_ShuiFang,
	SID_Jd_FangJianA,
	SID_Jd_FangJianB,
	SID_Jd_FangJianC,
	SID_Jd_FangJianD,
	SID_Jd_FangJianE,
	SID_Jd_ChuFang,
	SID_Jd_ShuFang,
	SID_Jd_KeFang,
	SID_Jd_ZhuYuShi,
	SID_Jd_YuShi,
	SID_Jd_ErTongFang,
	SID_Jd_GongRenFang,
	SID_Jd_DuoYongTuJian,
	SID_Jd_QiJuShi,
	SID_Jd_YouLeShi,
	SID_Jd_ShuXiShi,
	SID_Jd_WeiShengJian,
	SID_Jd_XieMaoJian,
	SID_Jd_GongZuoJian,
	SID_Jd_DiXiaShi,
	SID_Jd_Set_Scene,
	SID_Jd_Mode_Huike,
	SID_Jd_Mode_Jiucan,
	SID_Jd_Mode_Yejian,
	SID_Jd_Mode_Jiedian,
	SID_Jd_Mode_Putong,
	SID_Jd_Mode_Self,
	SID_Jd_Mode_Set_Name1,
	SID_Jd_Mode_Set_Name2,
	SID_Jd_Mode_Out,
	SID_Jd_Mode_Cook,
	SID_Jd_Mode_first,
	SID_Jd_Mode_Afater,
	SID_Jd_Mode_Night,
	SID_Jd_Mode_Bed,
	SID_Jd_Mode_QiYe,
	SID_Jd_Mode_Up,
	SID_Jd_Mode_Muyu,
	SID_Jd_Mode_Tea,
	SID_Jd_Mode_Drink,
	SID_Jd_Mode_Read,
	SID_Jd_Mode_Work,
	SID_Jd_Mode_Juhui,
	SID_Jd_Mode_Play,
	SID_Jd_Mode_Film,
	SID_Jd_Mode_Sing,
	SID_Jd_Mode_Storang,
	SID_Jd_Mode_Wenxi,
	SID_Jd_Mode_Langman,
	SID_Jd_Mode_Jiqing,
	SID_Jd_Mode_Dianya,
	SID_Jd_Mode_Chlid,
	SID_Jd_Mode_Chenwen,
	SID_Jd_Mode_Think,
	SID_Jd_Mode_Cool,
	SID_Jd_Mode_Warm,
	SID_Jd_Mode_Mode,
	SID_Jd_Mode_Sence,
	SID_Jd_Mode_Qingdiao,
	SID_Jd_Mode_Fenwei,
	SID_Jd_Mode_Time,
	SID_Jd_Mode_Changjing,
	SID_Jd_Deng,
	SID_Jd_TiaoGuangDeng,
	SID_Jd_BaiChiDeng,
	SID_Jd_GuangGuan,
	SID_Jd_DiaoDeng,
	SID_Jd_HuaDeng,
	SID_Jd_BiDeng,
	SID_Jd_SheDeng,
	SID_Jd_TaiDeng,
	SID_Jd_LouDiDeng,
	SID_Jd_ZhaoMingDeng,
	SID_Jd_ShiYingDeng,
	SID_Jd_YeDeng,
	SID_Jd_ZhuanshiDEng,
	SID_Jd_DianZhiSuo,
	SID_Jd_XiaoduWanGui,
	SID_Jd_KongQiJinHua,
	SID_Jd_YinShuiJi,
	SID_Jd_KongTiaoJi,
	SID_Jd_LenQiJi,
	SID_Jd_CouYouJi,
	SID_Jd_NuanFenJi,
	SID_Jd_DianReTan,
	SID_Jd_ReShuiLu,
	SID_Jd_DianFanBao,
	SID_Jd_DianZhiWaGuo,
	SID_Jd_WeiBoLu,
	SID_Jd_XiYiJi,
	SID_Jd_DianShiJi,
	SID_Jd_LuXiangJi,
	SID_Jd_YingXiang,
	SID_Jd_DianNao,
	SID_Jd_FengShan,
	SID_Jd_CD,
	SID_Jd_LD,
	SID_Jd_VCD,
	SID_Jd_DVD,
	SID_Jd_ChaZuo1,
	SID_Jd_ChaZuo2,
	SID_Jd_ChaZuo3,
	SID_Jd_ChaZuo4,
	SID_Jd_ChaZuo5,
	SID_Jd_XiaoDuSheBe,
	SID_Jd_SettingMusic,
	SID_Jd_ChuangLian,
	SID_Jd_JuanLian,
	SID_Jd_DianDongBuLian,
	SID_Jd_DianDongShaLian,
	SID_Jd_Deng1,
	SID_Jd_Deng2,
	SID_Jd_Deng3,
	SID_Jd_Deng4,
	SID_Jd_Deng5,
	SID_Jd_Deng6,
	SID_Jd_Deng7,
	SID_Jd_Deng8,
	SID_Jd_Deng9,
	SID_Jd_Deng10,
	SID_Jd_TiaoGuangDeng1,
	SID_Jd_TiaoGuangDeng2,
	SID_Jd_TiaoGuangDeng3,
	SID_Jd_TiaoGuangDeng4,
	SID_Jd_TiaoGuangDeng5,
	SID_Jd_TiaoGuangDeng6,
	SID_Jd_TiaoGuangDeng7,
	SID_Jd_TiaoGuangDeng8,
	SID_Jd_TiaoGuangDeng9,
	SID_Jd_TiaoGuangDeng10,
	SID_Jd_BaiChiDeng1,
	SID_Jd_BaiChiDeng2,
	SID_Jd_BaiChiDeng3,
	SID_Jd_BaiChiDeng4,
	SID_Jd_BaiChiDeng5,
	SID_Jd_BaiChiDeng6,
	SID_Jd_BaiChiDeng7,
	SID_Jd_BaiChiDeng8,
	SID_Jd_BaiChiDeng9,
	SID_Jd_BaiChiDeng10,
	SID_Jd_GuangGuan1,
	SID_Jd_GuangGuan2,
	SID_Jd_GuangGuan3,
	SID_Jd_GuangGuan4,
	SID_Jd_GuangGuan5,
	SID_Jd_GuangGuan6,
	SID_Jd_GuangGuan7,
	SID_Jd_GuangGuan8,
	SID_Jd_GuangGuan9,
	SID_Jd_GuangGuan10,
	SID_Jd_DiaoDeng1,
	SID_Jd_DiaoDeng2,
	SID_Jd_DiaoDeng3,
	SID_Jd_DiaoDeng4,
	SID_Jd_DiaoDeng5,
	SID_Jd_DiaoDeng6,
	SID_Jd_DiaoDeng7,
	SID_Jd_DiaoDeng8,
	SID_Jd_DiaoDeng9,
	SID_Jd_DiaoDeng10,
	SID_Jd_HuaDeng1,
	SID_Jd_HuaDeng2,
	SID_Jd_HuaDeng3,
	SID_Jd_HuaDeng4,
	SID_Jd_HuaDeng5,
	SID_Jd_HuaDeng6,
	SID_Jd_HuaDeng7,
	SID_Jd_HuaDeng8,
	SID_Jd_HuaDeng9,
	SID_Jd_HuaDeng10,
	SID_Jd_BiDeng1,
	SID_Jd_BiDeng2,
	SID_Jd_BiDeng3,
	SID_Jd_BiDeng4,
	SID_Jd_BiDeng5,
	SID_Jd_BiDeng6,
	SID_Jd_BiDeng7,
	SID_Jd_BiDeng8,
	SID_Jd_BiDeng9,
	SID_Jd_BiDeng10,
	SID_Jd_SheDeng1,
	SID_Jd_SheDeng2,
	SID_Jd_SheDeng3,
	SID_Jd_SheDeng4,
	SID_Jd_SheDeng5,
	SID_Jd_SheDeng6,
	SID_Jd_SheDeng7,
	SID_Jd_SheDeng8,
	SID_Jd_SheDeng9,
	SID_Jd_SheDeng10,
	SID_Jd_TaiDeng1,
	SID_Jd_TaiDeng2,
	SID_Jd_TaiDeng3,
	SID_Jd_TaiDeng4,
	SID_Jd_TaiDeng5,
	SID_Jd_TaiDeng6,
	SID_Jd_TaiDeng7,
	SID_Jd_TaiDeng8,
	SID_Jd_TaiDeng9,
	SID_Jd_TaiDeng10,
	SID_Jd_LouDiDeng1,
	SID_Jd_LouDiDeng2,
	SID_Jd_LouDiDeng3,
	SID_Jd_LouDiDeng4,
	SID_Jd_LouDiDeng5,
	SID_Jd_LouDiDeng6,
	SID_Jd_LouDiDeng7,
	SID_Jd_LouDiDeng8,
	SID_Jd_LouDiDeng9,
	SID_Jd_LouDiDeng10,
	SID_Jd_ZhaoMingDeng1,
	SID_Jd_ZhaoMingDeng2,
	SID_Jd_ZhaoMingDeng3,
	SID_Jd_ZhaoMingDeng4,
	SID_Jd_ZhaoMingDeng5,
	SID_Jd_ZhaoMingDeng6,
	SID_Jd_ZhaoMingDeng7,
	SID_Jd_ZhaoMingDeng8,
	SID_Jd_ZhaoMingDeng9,
	SID_Jd_ZhaoMingDeng10,
	SID_Jd_ShiYingDeng1,
	SID_Jd_ShiYingDeng2,
	SID_Jd_ShiYingDeng3,
	SID_Jd_ShiYingDeng4,
	SID_Jd_ShiYingDeng5,
	SID_Jd_ShiYingDeng6,
	SID_Jd_ShiYingDeng7,
	SID_Jd_ShiYingDeng8,
	SID_Jd_ShiYingDeng9,
	SID_Jd_ShiYingDeng10,
	SID_Jd_YeDeng1,
	SID_Jd_YeDeng2,
	SID_Jd_YeDeng3,
	SID_Jd_YeDeng4,
	SID_Jd_YeDeng5,
	SID_Jd_YeDeng6,
	SID_Jd_YeDeng7,
	SID_Jd_YeDeng8,
	SID_Jd_YeDeng9,
	SID_Jd_YeDeng10,
	SID_Jd_ZhuanshiDEng1,
	SID_Jd_ZhuanshiDEng2,
	SID_Jd_ZhuanshiDEng3,
	SID_Jd_ZhuanshiDEng4,
	SID_Jd_ZhuanshiDEng5,
	SID_Jd_ZhuanshiDEng6,
	SID_Jd_ZhuanshiDEng7,
	SID_Jd_ZhuanshiDEng8,
	SID_Jd_ZhuanshiDEng9,
	SID_Jd_ZhuanshiDEng10,
	SID_Jd_Button_Cold,
	SID_Jd_Button_Warm,
	SID_Jd_Button_Mode,
	SID_Jd_KongTiao_Refre,
	SID_Jd_KongTiao_Warm,
	SID_Jd_KongTiao_Leisure,
	SID_Jd_KongTiao_Sleep,
	SID_Jd_KongTiao_Close,


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
