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

static ITULayer* g_NowLayer = NULL;					// 记录当前页面
static ITULayer* mainLayer = NULL;					// 主界面
static ITULayer* SetMenuLayer;						// 设置界面
static ITULayer* SetPersonalityLayer;				// 个性设置界面
static ITULayer* SetPersonalityPhotoFrameLayer;		// 电子相框设置界面
static ITULayer* SetVolumeLayer;					// 音量设置界面
static ITULayer* SetNoDisturbLayer;					// 免打扰设置界面
static ITULayer* SetNumKeyBordLayer;				// 数字键盘输入界面
static ITULayer* SetPasswordLayer;					// 开门密码界面和安防密码界面
static ITULayer* SetSystemLanguageLayer;			// 系统语言设置
static ITULayer* SetLinkLayer;						// 联动设置
static ITULayer* SetUserLayer;						// 用户设置
static ITULayer* SetAlarmNumLayer;					// 安防号码设置
static ITULayer* SetAlarmAreaListLayer;				// 防区隔离、防区局防有效
static ITULayer* SetProjectLayer;					// 工程设置界面
static ITULayer* SetDataTimeLayer;					// 设置时间和日期
static ITULayer* SetRingtoneLayer;					// 声音设置
static ITULayer* SetAlarmRemoteLayer;				// 远程控制
static ITULayer* SetJDLayer;						// 家电设置
static ITULayer* SetAlarmLayer;						// 安防设置
static ITULayer* SetNetParamLayer;					// 网络参数设置
static ITULayer* SetExtModuleLayer;					// 外部模块设置
static ITULayer* SetDevnoLayer;						// 设备编号设置
static ITULayer* SetDiantiParamLayer;				// 电梯IP设置
static ITULayer* MsgFailHintSuccessLayer;			// 失败、警告、成功消息框
static ITULayer* SetRtspLayer;						// 监视设置
static ITULayer* SetAlarmSingleAttrLayer;			// 防区属性设置

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

	return false;
}

void SetBottonReturnReset(void)
{
	g_NowLayer = NULL;
	mainLayer = NULL;
}
