/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_botton_return.c
Author:     	zxc
Version:
Date: 		2016-06-13
Description:
History:
1. Date:
Author:
Modification:
*************************************************/
#include "gui_include.h"

static ITULayer* g_NowLayer = NULL;						// 记录当前页面
static ITULayer* mainLayer = NULL;						// 主界面
static ITULayer* SetMenuLayer = NULL;					// 设置界面
static ITULayer* SetPersonalityLayer = NULL;			// 个性设置界面
static ITULayer* SetPersonalityPhotoFrameLayer = NULL;	// 电子相框设置界面
static ITULayer* SetVolumeLayer = NULL;					// 音量设置界面
static ITULayer* SetNoDisturbLayer = NULL;				// 免打扰设置界面
static ITULayer* SetNumKeyBordLayer = NULL;				// 数字键盘输入界面
static ITULayer* SetPasswordLayer = NULL;				// 开门密码界面和安防密码界面
static ITULayer* SetSystemLanguageLayer = NULL;			// 系统语言设置
static ITULayer* SetLinkLayer = NULL;					// 联动设置
static ITULayer* SetUserLayer = NULL;					// 用户设置
static ITULayer* SetAlarmNumLayer = NULL;				// 安防号码设置
static ITULayer* SetAlarmAreaListLayer = NULL;			// 防区隔离、防区局防有效
static ITULayer* SetProjectLayer = NULL;				// 工程设置界面
static ITULayer* SetDataTimeLayer = NULL;				// 设置时间和日期
static ITULayer* SetRingtoneLayer = NULL;				// 声音设置
static ITULayer* SetAlarmRemoteLayer = NULL;			// 远程控制
static ITULayer* SetJDLayer = NULL;						// 家电设置
static ITULayer* SetAlarmLayer = NULL;					// 安防设置
static ITULayer* SetNetParamLayer = NULL;				// 网络参数设置
static ITULayer* SetExtModuleLayer = NULL;				// 外部模块设置
static ITULayer* SetDevnoLayer = NULL;					// 设备编号设置
static ITULayer* SetDiantiParamLayer = NULL;			// 电梯IP设置
static ITULayer* MsgFailHintSuccessLayer = NULL;		// 失败、警告、成功消息框
static ITULayer* SetRtspLayer = NULL;					// 监视设置
static ITULayer* SetAlarmSingleAttrLayer = NULL;		// 防区属性设置
static ITULayer* SetNetDoorLayer = NULL;				// 门前机设置
static ITULayer* SetNetDoorNetParamLayer = NULL;		// 门前网络参数设置
static ITULayer* SetNetDoorLockLayer = NULL;			// 门前锁类型数设置
static ITULayer* SetNetDoorCardLayer = NULL;			// 门前卡管理
static ITULayer* SetNetDoorOtherLayer = NULL;			// 门前其他设置
static ITULayer* SetNetDoorSysInfoLayer = NULL;			// 门前系统信息
static ITULayer* SetSysInfoLayer = NULL;				// 系统信息
static ITULayer* SetStorageLayer = NULL;				// 存储管理
static ITULayer* SetIpProtocolHostLayer = NULL;			// IP协议转换器主机
static ITULayer* SetIpProtocolFenjiLayer = NULL;		// IP协议转换器分机

/*************************************************
Function:		SetBottonReturnOnEnter
Description: 	设置界面底部返回键初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetBottonReturnOnEnter(ITUWidget* widget, char* param)
{
	if (!mainLayer)
	{
		mainLayer = ituSceneFindWidget(&theScene, "mainLayer");
		assert(mainLayer);

		SetMenuLayer = ituSceneFindWidget(&theScene, "SetMenuLayer");
		assert(SetMenuLayer);

		SetPersonalityLayer = ituSceneFindWidget(&theScene, "SetPersonalityLayer");
		assert(SetPersonalityLayer);

		SetPersonalityPhotoFrameLayer = ituSceneFindWidget(&theScene, "SetPersonalityPhotoFrameLayer");
		assert(SetPersonalityPhotoFrameLayer);

		SetVolumeLayer = ituSceneFindWidget(&theScene, "SetVolumeLayer");
		assert(SetVolumeLayer);

		SetNoDisturbLayer = ituSceneFindWidget(&theScene, "SetNoDisturbLayer");
		assert(SetNoDisturbLayer);

		SetNumKeyBordLayer = ituSceneFindWidget(&theScene, "SetNumKeyBordLayer");
		assert(SetNumKeyBordLayer);

		SetPasswordLayer = ituSceneFindWidget(&theScene, "SetPasswordLayer");
		assert(SetPasswordLayer); 

		SetSystemLanguageLayer = ituSceneFindWidget(&theScene, "SetSystemLanguageLayer");
		assert(SetSystemLanguageLayer); 

		SetLinkLayer = ituSceneFindWidget(&theScene, "SetLinkLayer");
		assert(SetLinkLayer);

		SetUserLayer = ituSceneFindWidget(&theScene, "SetUserLayer");
		assert(SetUserLayer); 

		SetAlarmNumLayer = ituSceneFindWidget(&theScene, "SetAlarmNumLayer");
		assert(SetAlarmNumLayer); 

		SetAlarmAreaListLayer = ituSceneFindWidget(&theScene, "SetAlarmAreaListLayer");
		assert(SetAlarmAreaListLayer); 

		SetProjectLayer = ituSceneFindWidget(&theScene, "SetProjectLayer");
		assert(SetProjectLayer); 

		SetDataTimeLayer = ituSceneFindWidget(&theScene, "SetDataTimeLayer");
		assert(SetDataTimeLayer);

		SetRingtoneLayer = ituSceneFindWidget(&theScene, "SetRingtoneLayer");
		assert(SetRingtoneLayer);
		
		SetAlarmRemoteLayer = ituSceneFindWidget(&theScene, "SetAlarmRemoteLayer");
		assert(SetAlarmRemoteLayer); 

		SetJDLayer = ituSceneFindWidget(&theScene, "SetJDLayer");
		assert(SetJDLayer); 

		SetAlarmLayer = ituSceneFindWidget(&theScene, "SetAlarmLayer");
		assert(SetAlarmLayer); 

		SetNetParamLayer = ituSceneFindWidget(&theScene, "SetNetParamLayer");
		assert(SetNetParamLayer); 

		SetExtModuleLayer = ituSceneFindWidget(&theScene, "SetExtModuleLayer");
		assert(SetExtModuleLayer); 

		SetDevnoLayer = ituSceneFindWidget(&theScene, "SetDevnoLayer");
		assert(SetDevnoLayer); 

		SetDiantiParamLayer = ituSceneFindWidget(&theScene, "SetDiantiParamLayer");
		assert(SetDiantiParamLayer); 

		MsgFailHintSuccessLayer = ituSceneFindWidget(&theScene, "MsgFailHintSuccessLayer");
		assert(MsgFailHintSuccessLayer); 

		SetRtspLayer = ituSceneFindWidget(&theScene, "SetRtspLayer");
		assert(SetRtspLayer);

		SetAlarmSingleAttrLayer = ituSceneFindWidget(&theScene, "SetAlarmSingleAttrLayer");
		assert(SetAlarmSingleAttrLayer);

		SetNetDoorLayer = ituSceneFindWidget(&theScene, "SetNetDoorLayer");
		assert(SetNetDoorLayer);

		SetNetDoorNetParamLayer = ituSceneFindWidget(&theScene, "SetNetDoorNetParamLayer");
		assert(SetNetDoorNetParamLayer);

		SetNetDoorLockLayer = ituSceneFindWidget(&theScene, "SetNetDoorLockLayer");
		assert(SetNetDoorLockLayer);

		SetNetDoorCardLayer = ituSceneFindWidget(&theScene, "SetNetDoorCardLayer");
		assert(SetNetDoorCardLayer);

		SetNetDoorOtherLayer = ituSceneFindWidget(&theScene, "SetNetDoorOtherLayer");
		assert(SetNetDoorOtherLayer);

		SetNetDoorSysInfoLayer = ituSceneFindWidget(&theScene, "SetNetDoorSysInfoLayer");
		assert(SetNetDoorSysInfoLayer);

		SetSysInfoLayer = ituSceneFindWidget(&theScene, "SetSysInfoLayer");
		assert(SetSysInfoLayer);

		SetStorageLayer = ituSceneFindWidget(&theScene, "SetStorageLayer");
		assert(SetStorageLayer);

		SetIpProtocolHostLayer = ituSceneFindWidget(&theScene, "SetIpProtocolHostLayer");
		assert(SetIpProtocolHostLayer);

		SetIpProtocolFenjiLayer = ituSceneFindWidget(&theScene, "SetIpProtocolFenjiLayer");
		assert(SetIpProtocolFenjiLayer);
	}

	return true;
}

/*************************************************
Function:		SetBottonReturnOnReload
Description: 	设置界面底部返回键重载函数,更新当前页面的ITUWidget
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetBottonReturnOnReload(ITUWidget* widget, char* param)
{
	g_NowLayer = (ITULayer*)widget;
	
	return true;
}

/*************************************************
Function:		SetBottonReturnButtonOnPress
Description: 	设置界面底部返回键按下处理函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetBottonReturnButtonOnPress(ITUWidget* widget, char* param)
{
	if (g_NowLayer == SetMenuLayer)
	{
		if (!ituWidgetIsVisible(mainLayer))
		{
			ituLayerGoto(mainLayer);
		}	
		return true;
	}

	if (g_NowLayer == SetPersonalityLayer)
	{
		if (!ituWidgetIsVisible(SetMenuLayer))
			ituLayerGoto(SetMenuLayer);
		return true;
	}

	if ((g_NowLayer == SetSysInfoLayer) || (g_NowLayer == SetStorageLayer) || (g_NowLayer == SetIpProtocolHostLayer) || (g_NowLayer == SetIpProtocolFenjiLayer))
	{
		if (!ituWidgetIsVisible(SetMenuLayer))
			ituLayerGoto(SetMenuLayer);
		return true;
	}

	if (g_NowLayer == SetPersonalityPhotoFrameLayer)
	{	
		SetPersonalityPhotoFrameLayerOnReturn();
		return true;
	}

	if (g_NowLayer == SetVolumeLayer)
	{
		SetVolumeLayerOnReturn();
		return true;
	}

	if (g_NowLayer == SetNoDisturbLayer)
	{
		SetNoDisturLayerOnReturn();
		return true;
	}

	if (g_NowLayer == SetNumKeyBordLayer)
	{
		SetNumKeyBordLayerOnReturn();
		return true;
	}

	if (g_NowLayer == SetPasswordLayer)
	{
		SetPasswordLayerOnReturn();
		return true;
	}

	if (g_NowLayer == SetSystemLanguageLayer)
	{
		if (!ituWidgetIsVisible(SetMenuLayer))
		{
			ituLayerGoto(SetMenuLayer);
		}	
		return true;
	}

	if (g_NowLayer == SetLinkLayer)
	{
		SetLinkLayerOnReturn();
		return true;
	}

	if (g_NowLayer == SetUserLayer)
	{
		SetUserLayerOnReturn();
		return true;
	}

	if (g_NowLayer == SetAlarmNumLayer)
	{
		if (!ituWidgetIsVisible(SetUserLayer))
		{
			ituLayerGoto(SetUserLayer);
		}
		return true;
	}

	if (g_NowLayer == SetAlarmAreaListLayer)
	{
		SetAlarmAreaListLayerOnReturn();
		return true;
	}

	if (g_NowLayer == SetProjectLayer) 
	{
		if (!ituWidgetIsVisible(SetMenuLayer))
		{
			ituLayerGoto(SetMenuLayer);
		}
		return true;
	}
	
	if (g_NowLayer == SetDataTimeLayer)
	{
		SetDataTimeLayerOnReturn();
		return true;
	}

	if (g_NowLayer == SetRingtoneLayer)
	{
		SetRingtoneOnReturn();
		return true;
	}

	if (g_NowLayer == SetAlarmRemoteLayer)
	{
		if (!ituWidgetIsVisible(SetUserLayer))
		{
			ituLayerGoto(SetUserLayer);
		}
		return true;
	}

	if (g_NowLayer == SetJDLayer)
	{
		if (!ituWidgetIsVisible(SetProjectLayer))
		{
			ituLayerGoto(SetProjectLayer);
		}
		return true;
	}

	if (g_NowLayer == SetAlarmLayer)
	{
		SetAlarmLayerOnReturn();
		return true;
	}

	if (g_NowLayer == SetNetParamLayer)
	{
		SetNetParamLayerOnReturn();
		return true;
	}

	if (g_NowLayer == SetExtModuleLayer)
	{
		if (!ituWidgetIsVisible(SetProjectLayer))
		{
			ituLayerGoto(SetProjectLayer);
		}
		return true;
	}

	if (g_NowLayer == SetDevnoLayer)
	{
		SetDevnoLayerOnReturn();
		return true;
	}

	if (g_NowLayer == SetDiantiParamLayer)
	{
		if (!ituWidgetIsVisible(SetProjectLayer))
		{
			ituLayerGoto(SetProjectLayer);
		}
		return true;
	}

	if (g_NowLayer == MsgFailHintSuccessLayer)
	{
		MsgFailHintSuccessButtonOnMouseUp(widget, param);
		return true;
	}

	if (g_NowLayer == SetRtspLayer)
	{
		SetRtspLayerOnReturn();
		return true;
	}
	
	if (g_NowLayer == SetAlarmSingleAttrLayer)
	{
		SetAlarmSingleAttrLayerOnReturn();
		return true;
	}

	if (g_NowLayer == SetNetDoorLayer)
	{
		SetNetDoorLayerOnReturn();
		return true;
	}

	if (g_NowLayer == SetNetDoorNetParamLayer)
	{
		SetNetDoorNetparamLayerOnReturn();
		return true;
	}

	if (g_NowLayer == SetNetDoorLockLayer)
	{
		SetNetDoorLockLayerOnReturn();
		return true;
	}

	if (g_NowLayer == SetNetDoorCardLayer)
	{
		if (!ituWidgetIsVisible(SetNetDoorLayer))
		{
			ituLayerGoto(SetNetDoorLayer);
		}
		return true;
	}

	if (g_NowLayer == SetNetDoorOtherLayer)
	{
		SetNetDoorOtherLayerOnReturn();
		return true;
	}

	if (g_NowLayer == SetNetDoorSysInfoLayer)
	{
		if (!ituWidgetIsVisible(SetNetDoorLayer))
		{
			ituLayerGoto(SetNetDoorLayer);
		}
		return true;
	}

	return false;
}
