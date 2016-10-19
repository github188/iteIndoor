/*************************************************
Copyright (C),  2006-2016, Aurine
File name:		layer_aurine_jd.c
Author:     	caogw
Version:    	
Date: 			2016-09-25
Description:
History:
1. Date:
Author:
Modification:
*************************************************/
#include "gui_include.h"

/*****************常量定义***********************/
#define BUTTON_ICON_W			152									// 按键宽
#define BUTTON_ICON_H			153									// 按键高
#define BUTTON_CUITAIN_ICON_W	304									// 窗帘宽
#define BUTTON_CUITAIN_ICON_H	305									// 窗帘高
#define COVWERFLOW_W			608									// Coverflow 宽
#define COVWERFLOW_H			305									// Coverflow 高
#define COVWERFLOW_XPOS			16									// Coverflow 起始位置
#define COVWERFLOW_YPOS			18									// Coverflow 起始位置
#define	PAGEINDEX_ICON_W		8									// 页数图标
#define	PAGEINDEX_ICON_H		8									// 页数图标
#define	PAGEINDEX_DIS			12									// 页数间距

#define JD_PAGE_NUM				8									// 每页个数
#define JD_CUITAIN_NUM			2									// 窗帘宽
#define JD_SCENE_PAGE_NUM		MAX_JD_SCENE_NUM/JD_PAGE_NUM		// 情景页数
#define JD_LIGHT_PAGE_NUM		MAX_JD_LIGHT/JD_PAGE_NUM			// 灯光页数
#define JD_AIR_PAGE_NUM			MAX_JD_KONGTIAO						// 空调页数（每页一个）
#define JD_CURTAIN_PAGE_NUM		MAX_JD_WINDOW/JD_CUITAIN_NUM		// 窗帘页数（每页两个）
#define JD_POWER_PAGE_NUM		MAX_JD_POWER/JD_PAGE_NUM			// 电源页数
#define JD_GAS_PAGE_NUM			1									// 煤气页数
#define JD_SCENE_TYPE			JD_DEV_MAX							// 情景类型
/*****************变量定义***********************/
static ITULayer* MainLayer = NULL;
static ITULayer* AurineJDLayer = NULL;
static ITURadioBox* AurineJDRightHomeRadioBox = NULL;
static ITUBackground* AurineJDRightBackground = NULL;
static ITUContainer* AurineJDExitContainer = NULL;
static ITUCoverFlow* AurineJDBottomCoverFlow = NULL;
static ITUSprite* AurineJDBottomSprite[JD_ALL] = { NULL };
static ITUSprite* AurineJDPageIndexSprite = NULL;
static ITUBackground* AurineJDBackground = NULL;
// 情景
static ITUCoverFlow* AurineJDSceneCoverFlow = NULL;
static ITUBackground* AurineJDSceneBackgroundPage[JD_SCENE_PAGE_NUM] = { NULL };
static ITUBackground* AurineJDSceneBackground = NULL;
// 灯光
static ITUCoverFlow* AurineJDLightCoverFlow = NULL;
static ITUBackground* AurineJDLightBackgroundPage[JD_LIGHT_PAGE_NUM] = { NULL };
static ITUBackground* AurineJDLightBackground = NULL;
static ITUBackground* AurineJDControlLightGrayBackground = NULL;
static ITUTrackBar* AurineJDControlLightTrackBar = NULL;
static ITUButton* AurineJDRightNullButton[3] = { NULL };
static ITUText* AurineJDControlLightDevText = NULL;
// 空调
static ITUCoverFlow* AurineJDAirCoverFlow = NULL;
static ITUBackground* AurineJDAirBackgroundPage[JD_AIR_PAGE_NUM] = { NULL };
static ITUBackground* AurineJDAirBackground = NULL;
// 窗帘
static ITUCoverFlow* AurineJDCurtainCoverFlow = NULL;
static ITUBackground* AurineJDCurtainBackgroundPage[JD_CURTAIN_PAGE_NUM] = { NULL };
static ITUBackground* AurineJDCurtainBackground = NULL;
// 电源
static ITUBackground* AurineJDPowerBackground = NULL;
// 煤气
static ITUBackground* AurineJDGasBackground = NULL;

static ITUButton* g_AurineJDSceneButton[MAX_JD_SCENE_NUM] = { NULL };
static ITUButton* g_AurineJDLightButton[MAX_JD_LIGHT] = { NULL };
static ITUSprite* g_AurineJDLightSprite[MAX_JD_LIGHT] = { NULL };
static ITUButton* g_AurineJDAirButton[MAX_JD_KONGTIAO][10] = { NULL };					// 空调按键
static ITUSprite* g_AurineJDAirShowModeSprite[MAX_JD_KONGTIAO] = { NULL };				// 显示各种状态或模式图标
static ITUText* g_AurineJDAirShowModeText[MAX_JD_KONGTIAO] = { NULL };					// 显示各种模式名称
static ITUContainer* g_AurineJDAirNumContainer[MAX_JD_KONGTIAO] = { NULL };				// 显示温度或状态
static ITUSprite* g_AurineJDAirIndexNumSprite[MAX_JD_KONGTIAO][3] = { NULL };			// 三段式显示温度或状态
static ITUButton* g_AurineJDCurtainButton[MAX_JD_WINDOW][3] = { NULL };					// 窗帘按键
static ITUSprite* g_AurineJDCurtainSateSprite[MAX_JD_WINDOW] = { NULL };				// 显示开关状态图标
static ITUButton* g_AurineJDPowerButton[MAX_JD_POWER] = { NULL };
static ITUSprite* g_AurineJDPowerSprite[MAX_JD_POWER] = { NULL };
static ITUButton* g_AurineJDGasButton[MAX_JD_GAS] = { NULL };
static ITUSprite* g_AurineJDGasSprite[MAX_JD_GAS] = { NULL };

static ITUSprite* g_AurineJDPageIndexSprite[MAX_JD_KONGTIAO] = { NULL };				// 页标
/*****************变量定义***********************/
static uint32 g_JDLastTick = 0;										// 实时更新的tick
static uint8 g_Index = 0;
static uint8 g_BtnEvent = 0;										// 设备类型按键事件
static uint8 g_scene = 0;											// 情景模式
static PJD_SCENE_INFO_LIST g_SceneList = NULL;						// 情景列表
static PJD_DEV_LIST_INFO g_DevList = NULL;							// 设备列表
static AU_JD_DEV_TYPE g_DevType = JD_DEV_LIGHT;						// 设备类型
static JD_STATE_INFO g_JDState;
static uint8 g_DevCount = 0;										// 批量查询时的设备总数
static uint8 g_LightIndex = 0;										// 可调灯的索引
static uint8 g_PageCount = 0;										// 设备的CoverFlow总页数
static uint8 g_PageIndex = 0;										// 设备的CoverFlow当前页数
static uint8 g_LightValue[MAX_JD_LIGHT] = { 0 };					// 某一盏灯的当前亮度
static uint8 g_DevIndex[MAX_JD_LIGHT] = { 0 };						// 批量查询时所有索引
static uint16 g_DevAddr[MAX_JD_LIGHT] = { 0 };						// 批量查询时的设备地址
static uint8 g_AirValue[MAX_JD_KONGTIAO] = { 0 };					// 某一台空调的当前温度
static uint8 g_AirColdHot[MAX_JD_KONGTIAO] = { 0 };					// 某一台空调的当前制冷制暖状态
static uint8 g_AirLastMode[MAX_JD_KONGTIAO] = { 0 };				// 某一台空调的当前模式

static uint8 g_CloneSceneFlag[MAX_JD_SCENE_NUM] = { NULL };
static uint8 g_CloneLightFlag[MAX_JD_LIGHT] = { NULL };
static uint8 g_CloneAirFlag[MAX_JD_KONGTIAO] = { NULL };
static uint8 g_CloneCurtainFlag[MAX_JD_WINDOW] = { NULL };
static uint8 g_ClonePowerFlag[MAX_JD_POWER] = { NULL };
// 煤气未满一页按一页算
static uint8 g_CloneGasFlag[JD_PAGE_NUM] = { NULL };

typedef enum
{
	AurineJDHomeEvent = 0x00,
	AurineJDExitEvent, 
	AurineJDSceneEvent,
	AurineJDLightEvent,
	AurineJDAirEvent,
	AurineJDCurtainEvent,
	AurineJDPowerEvent,
	AurineJDGasEvent,
	AurineJDMaxEvent,
}AurineJDButtonEvent;

// 不可调节灯图标
typedef enum
{
	AurineJDButtomNoPressIcon = 0x00,
	AurineJDButtomPressIcon,
	AurineJDButtomButtonMAXIcon,
}AurineJDButtomButtonIcon;

// 不可调节灯图标
typedef enum
{
	AurineJDCloseLightIcon = 0x00,
	AurineJDOpenLightIcon,
	AurineJDMAXUnControlLightIcon,
}AurineJDLightUnControlIcon;

// 可调节灯图标
typedef enum
{
	AurineJDReturn0LightIcon = 0x00,
	AurineJDReturn1LightIcon,
	AurineJDReturn2LightIcon,
	AurineJDReturn3LightIcon,
	AurineJDReturn4LightIcon,
	AurineJDReturn5LightIcon,
	AurineJDReturn6LightIcon,
	AurineJDReturn7LightIcon,
	AurineJDReturn8LightIcon,
	AurineJDReturn9LightIcon,
	AurineJDReturn10LightIcon,
	AurineJDMAXControlLightIcon,
}AurineJDLightControlIcon;

// 空调模式图标
typedef enum
{
	AurineJDAirShowCloseIcon = 0x00,
	AurineJDAirShowColdIcon,
	AurineJDAirShowHeatIcon,
	AurineJDAirShowRefreshIcon,
	AurineJDAirShowWarmIcon,
	AurineJDAirShowLesureIcon,
	AurineJDAirShowSleepIcon,
	AurineJDAirShowMaxModeIcon,
}AurineJDAirShowModeIcon;

// 空调状态、温度图标
typedef enum
{
	AurineJDAirIndex0Num2Icon = 0x00,
	AurineJDAirIndex0Num3Icon,
	AurineJDAirIndex0OIcon,
	AurineJDAirIndex0MaxIcon,
}AurineJDAirIndex0Icon;

// 空调状态、温度图标
typedef enum
{
	AurineJDAirIndex1Num0Icon = 0x00,
	AurineJDAirIndex1Num1Icon,
	AurineJDAirIndex1Num2Icon,
	AurineJDAirIndex1Num3Icon,
	AurineJDAirIndex1Num4Icon,
	AurineJDAirIndex1Num5Icon,
	AurineJDAirIndex1Num6Icon,
	AurineJDAirIndex1Num7Icon,
	AurineJDAirIndex1Num8Icon,
	AurineJDAirIndex1Num9Icon,
	AurineJDAirIndex1FIcon,
	AurineJDAirIndex1MaxIcon,
}AurineJDAirIndex1Icon;

// 空调状态、温度图标
typedef enum
{
	AurineJDAirIndex2SXDIcon = 0x00,
	AurineJDAirIndex2FIcon,
	AurineJDAirIndex2MaxIcon,
}AurineJDAirIndex2Icon;

// 窗帘状态图标
typedef enum
{
	AurineJDCurtainCloseIcon = 0x00,
	AurineJDCurtainOpenIcon,
	AurineJDCurtainMaxIcon,
}AurineJDCurtainStateIcon;

// 电源、煤气图标
typedef enum
{
	AurineJDDevCloseIcon = 0x00,
	AurineJDDevOpenIcon,
	AurineJDDevMAXIcon,
}AurineJDDevStateIcon;

// 页数图标
typedef enum
{
	AurineJDPageIndexUnSelectIcon = 0x00,
	AurineJDPageIndexSelectIcon,
	AurineJDPageIndexMAXIcon,
}AurineJDPageIndexStateIcon;

/*************************************************
Function:		GetDevState
Description: 	根据设备的索引值获取设备的状态
Input:
	1.devtype	设备类型
	2.index		索引
Output:			无
Return:			无
Others:			无
*************************************************/
static uint8 GetDevState(AU_JD_DEV_TYPE devtype, uint8 index)
{
	#ifdef _SHOW_USED_JDDEV_ONLY_
	// g_JDState包含未启用的所有设备状态，参数Index只是启用的设备链表的索引
	// 将Index转为设备的Index，与g_JDState一致
	index = g_DevList->pjd_dev_info[index].Index;
	#endif

	switch (devtype)
	{
		case JD_DEV_LIGHT:
			return g_JDState.devlight[index][0];

		case JD_DEV_WINDOW:
			return g_JDState.devwindow[index];

		case JD_DEV_KONGTIAO:
			return g_JDState.devKongtiao[index][0];

		case JD_DEV_POWER:
			return g_JDState.devpower[index];

		case JD_DEV_GAS:
			return g_JDState.devgas[index];

		default:
			break;
	}

	return 0;
}

/*************************************************
Function:		SetDevState
Description: 	单个设备状态
1.devtype	设备类型
2.index		索引
3.state		状态
Output:			无
Return:			单个设备状态是否需变化
Others:			无
*************************************************/
static uint8 SetDevState(AU_JD_DEV_TYPE devtype, uint8 index, uint8 state)
{
	uint8 change = FALSE;

	#ifdef _SHOW_USED_JDDEV_ONLY_
	index = g_DevList->pjd_dev_info[index].Index;
	#endif

	switch (devtype)
	{
		case JD_DEV_LIGHT:
			if (state != g_JDState.devlight[index][0])
			{
				change = TRUE;
				g_JDState.devlight[index][0] = state;
			}
			break;

		case JD_DEV_WINDOW:
			if (state != g_JDState.devwindow[index])
			{
				change = TRUE;
				g_JDState.devwindow[index] = state;
			}
			break;

		case JD_DEV_KONGTIAO:
			if (state != g_JDState.devKongtiao[index][0])
			{
				change = TRUE;
				g_JDState.devKongtiao[index][0] = state;
			}
			break;

		case JD_DEV_POWER:
			if (state != g_JDState.devpower[index])
			{
				change = TRUE;
				g_JDState.devpower[index] = state;
			}
			break;

		case JD_DEV_GAS:
			if (state != g_JDState.devgas[index])
			{
				change = TRUE;
				g_JDState.devgas[index] = state;
			}
			break;

		default:
			break;
	}
	jd_aurine_set_dev_state(devtype, index, state);

	return change;
}

/*************************************************
Function:		GetDevValue
Description: 	获取灯光、空调设备等级值
	1.devtype	设备类型
	2.index		索引
Output:			无
Return:			无
Others:			无
*************************************************/
static uint8 GetDevValue(AU_JD_DEV_TYPE devtype, uint8 index)
{
	#ifdef _SHOW_USED_JDDEV_ONLY_
	index = g_DevList->pjd_dev_info[index].Index;
	#endif

	switch (devtype)
	{
		case JD_DEV_LIGHT:
			return g_JDState.devlight[index][1];

		case JD_DEV_KONGTIAO:
			return g_JDState.devKongtiao[index][1];

		default:
			return 0;
	}
}

/*************************************************
Function:		SetDevValue
Description: 	设置空调和灯光值
	1.devtype	设备类型
	2.index		索引
	3.state		状态
Output:			无
Return:			无
Others:			无
*************************************************/
static void SetDevValue(AU_JD_DEV_TYPE devtype, uint8 index, uint8 value)
{
	#ifdef _SHOW_USED_JDDEV_ONLY_
	index = g_DevList->pjd_dev_info[index].Index;
	#endif

	switch (devtype)
	{
		case JD_DEV_LIGHT:
			g_JDState.devlight[index][1] = value;
			g_LightValue[index] = value;
			break;

		case JD_DEV_KONGTIAO:
			g_JDState.devKongtiao[index][1] = value;
			#ifdef _IP_MODULE_
			g_AirValue[index] = value;
			#endif
			break;

	default:
		break;
	}
}

/*************************************************
Function:		SetDevMode
Description: 	设置空调模式
	1.index		索引
Output:			无
Return:			无
Others:			无
*************************************************/
static void SetDevMode(uint8 index, uint8 mode)
{
	#ifdef _SHOW_USED_JDDEV_ONLY_
	index = g_DevList->pjd_dev_info[index].Index;
	#endif
	jd_aurine_set_dev_state_param(JD_DEV_KONGTIAO, index, mode + 21);
}

/*************************************************
Function:		GetDevMode
Description: 	获取空调模式
1.index			索引
Output:			无
Return:			无
Others:			无
*************************************************/
static uint8 GetDevMode(uint8 index)
{
	uint8 mode;

	#ifdef _SHOW_USED_JDDEV_ONLY_
	index = g_DevList->pjd_dev_info[index].Index;
	#endif
	mode = jd_aurine_get_dev_state_param(JD_DEV_KONGTIAO, index);

	if (mode < 22 || mode > 25)
	{
		mode = 0;
	}
	else
	{
		mode = jd_aurine_get_dev_state_param(JD_DEV_KONGTIAO, index) - 21;
	}

	return mode;
}

/*************************************************
Function:		GetLightLastValue
Description: 	获取灯光值
	1.index		索引
Output:			无
Return:			无
Others:			无
*************************************************/
static uint8 GetLightLastValue(uint8 index)
{
	#ifdef _SHOW_USED_JDDEV_ONLY_
	index = g_DevList->pjd_dev_info[index].Index;
	#endif
	return g_LightValue[index];
}

#ifdef _IP_MODULE_
/*************************************************
Function:		GetAirLastValue
Description: 	获取空调
	1.index		索引
Output:			无
Return:			无
Others:			无
*************************************************/
static uint8 GetAirLastValue(uint8 index)
{
	#ifdef _SHOW_USED_JDDEV_ONLY_
	index = g_DevList->pjd_dev_info[index].Index;
	#endif
	return g_AirValue[index];
}
#endif

/*************************************************
Function:		SetDevColdHot
Description: 	设置空调冷暖模式
	1.index		索引
Output:			无
Return:			无
Others:			无
*************************************************/
static void SetDevColdHot(uint8 index, uint8 mode)
{
	g_AirColdHot[index] = mode;
}

/*************************************************
Function:		GetDevColdHot
Description: 	获取空调冷暖模式
	1.index		索引
Output:			无
Return:			0 制冷 1 制暖
Others:			无
*************************************************/
static uint8 GetDevColdHot(uint8 index)
{
	return g_AirColdHot[index];
}

/*************************************************
Function:		SetDevLastMode
Description: 	设置空调最后模式
	1.index		索引
Output:			无
Return:			无
Others:			无
*************************************************/
static void SetDevLastMode(uint8 index, uint8 mode)
{
	g_AirLastMode[index] = mode;
}

/*************************************************
Function:		GetDevLastMode
Description: 	获取空调最后模式
	1.index		索引
Output:			无
Return:			1 制冷 2 制暖 3 清爽 
				4 暖和 5 休闲 6 睡眠
Others:			无
*************************************************/
static uint8 GetDevLastMode(uint8 index)
{
	return g_AirLastMode[index];
}

/*************************************************
Function:		GetDevText
Description: 	获取设备名称和位置
	1.index		索引
	2.text      指针
Output:			无
Return:			无
Others:			无
*************************************************/
static void GetDevText(uint8 index, char *text)
{
	if (text == NULL)
	{
		return;
	}

	#ifdef _IP_MODULE_JD_
	if (JD_FACTORY_ACBUS == storage_get_extmode(EXT_MODE_JD_FACTORY) && get_ipmodule())
	{
		strcpy(text, g_DevList->pjd_dev_info[index].Name);
	}
	else
	#endif
	{
		char tmp[50] = { 0 };
		memcpy(text, get_str(g_DevList->pjd_dev_info[index].TextIDPos), 50);
		memcpy(tmp, get_str(g_DevList->pjd_dev_info[index].TextIDName), 50);
		strcat(text, tmp);
	}
}

/*******************************************************************
Function:		ReadDevStateOnce
Description:	读取某一类型所有设备地址、索引和个数
Input: 			无
Output:			无
Return:			无
Others:
*******************************************************************/
static void ReadDevStateOnce(void)
{
	int i;

	if (g_DevList && g_DevList->nCount)
	{
		for (i = 0; i < g_DevList->nCount; i++)
		{
			g_DevAddr[i] = g_DevList->pjd_dev_info[i].Address;
			g_DevIndex[i] = g_DevList->pjd_dev_info[i].Index;
		}
		g_DevCount = g_DevList->nCount;
	}
	// 注意需添加批量查询标志
}
/*************************************************
Function:		SetSampleVisible
Description: 	调节灯状态时设置其余不可触控控件
Input:
	1.flag		TRUE: Enable FALSE: Disable
Output:			无
Return:			无
Others:			无
*************************************************/
static void SetLightControlEnable(uint8 flag)
{
	if (true == flag)
	{
		ituWidgetEnable(AurineJDLightCoverFlow);
		ituWidgetEnable(AurineJDBottomCoverFlow);
		ituWidgetEnable(AurineJDRightBackground);
		ituWidgetEnable(AurineJDExitContainer);
		ituWidgetSetVisible(AurineJDControlLightGrayBackground, false);
	}
	else
	{
		ituWidgetDisable(AurineJDLightCoverFlow);
		ituWidgetDisable(AurineJDBottomCoverFlow);
		ituWidgetDisable(AurineJDRightBackground);
		ituWidgetDisable(AurineJDExitContainer);
		ituWidgetSetVisible(AurineJDControlLightGrayBackground, true);
	}
}

/*************************************************
Function:		SetSampleVisible
Description: 	设置设备模板是否可见
Input:
	1.type		设备类型风格
	2.flag		TRUE: 可见 FALSE: 不可见
Output:			无
Return:			无
Others:			无
*************************************************/
static void SetSampleVisible(AU_JD_DEV_TYPE type, uint8 flag)
{
	switch (type)
	{
		case JD_SCENE_TYPE:
			ituWidgetSetVisible(AurineJDSceneBackground, flag);
			break;

		case JD_DEV_LIGHT:
			ituWidgetSetVisible(AurineJDLightBackground, flag);
			ituWidgetSetVisible(AurineJDPowerBackground, false);
			ituWidgetSetVisible(AurineJDGasBackground, false);
			break;

		case JD_DEV_POWER:
			ituWidgetSetVisible(AurineJDPowerBackground, flag);
			ituWidgetSetVisible(AurineJDLightBackground, false);
			ituWidgetSetVisible(AurineJDGasBackground, false);
			break;

		case JD_DEV_GAS:
			ituWidgetSetVisible(AurineJDGasBackground, flag);
			ituWidgetSetVisible(AurineJDLightBackground, false);
			ituWidgetSetVisible(AurineJDPowerBackground, false);
			break;

		case JD_DEV_KONGTIAO:
			ituWidgetSetVisible(AurineJDAirBackground, flag);
			break;

		case JD_DEV_WINDOW:
			ituWidgetSetVisible(AurineJDCurtainBackground, flag);
			break;

		default:
			break;
	}
}

/*************************************************
Function:		ShowPageType
Description: 	显示设备界面
Input:			
	1.type		设备类型风格
Output:			无
Return:			无
Others:			无
*************************************************/
static void ShowPageType(AU_JD_DEV_TYPE type)
{
	g_DevType = type;
	switch (type)
	{
		case JD_SCENE_TYPE:
			ituWidgetSetVisible(AurineJDLightCoverFlow, false);
			ituWidgetSetVisible(AurineJDAirCoverFlow, false);
			ituWidgetSetVisible(AurineJDCurtainCoverFlow, false);
			ituWidgetSetVisible(AurineJDSceneCoverFlow, true);
			break;

		case JD_DEV_LIGHT:
		case JD_DEV_POWER:
		case JD_DEV_GAS:
			ituWidgetSetVisible(AurineJDSceneCoverFlow, false);
			ituWidgetSetVisible(AurineJDAirCoverFlow, false);
			ituWidgetSetVisible(AurineJDCurtainCoverFlow, false);
			ituWidgetSetVisible(AurineJDLightCoverFlow, true);
			break;

		case JD_DEV_KONGTIAO:
			ituWidgetSetVisible(AurineJDSceneCoverFlow, false);
			ituWidgetSetVisible(AurineJDLightCoverFlow, false);
			ituWidgetSetVisible(AurineJDCurtainCoverFlow, false);
			ituWidgetSetVisible(AurineJDAirCoverFlow, true);
			break;

		case JD_DEV_WINDOW:
			ituWidgetSetVisible(AurineJDSceneCoverFlow, false);
			ituWidgetSetVisible(AurineJDLightCoverFlow, false);
			ituWidgetSetVisible(AurineJDAirCoverFlow, false);
			ituWidgetSetVisible(AurineJDCurtainCoverFlow, true);
			break;

		default:
			break;
	}
}

/*************************************************
Function:		DrawPageIndex
Description: 	画页标
Input:
	1.count		页数
Output:			无
Return:			无
Others:			无
*************************************************/
static void DrawPageIndex(uint8 count)
{
	char tmp[50];
	uint8 i, result = 0;
	uint32 xpos = 0, ypos = 0;
	ITUSprite* CloneAurineJDPageIndexSprite;
	ITUSprite* OldAurineJDPageIndexSprite;

	// 删除已经Clone的控件
	if (g_PageCount > 1)
	{
		for (i = 0; i < g_PageCount; i++)
		{
			g_AurineJDPageIndexSprite[i] = NULL;
			OldAurineJDPageIndexSprite = NULL;
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "AurineJDPageIndexSprite", i);
			OldAurineJDPageIndexSprite = (ITUContainer*)ituSceneFindWidget(&theScene, tmp);
			itcTreeRemove(OldAurineJDPageIndexSprite);						// 删除已Clone的
			ituWidgetExit(OldAurineJDPageIndexSprite);
			//dprintf("Delete Clone AurineJDPageIndexSprite!!!\n");
		}
	}

	g_PageCount = count;
	if (g_PageCount > 1)
	{
		ituWidgetSetVisible(AurineJDPageIndexSprite, true);
		xpos = COVWERFLOW_XPOS + ((COVWERFLOW_W - (PAGEINDEX_ICON_W * g_PageCount + PAGEINDEX_DIS *(g_PageCount - 1))) / 2);
		ypos = COVWERFLOW_YPOS + COVWERFLOW_H + 10;
		
		for (i = 0; i < g_PageCount; i++)
		{
			result = FALSE;
			CloneAurineJDPageIndexSprite = NULL;

			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "AurineJDPageIndexSprite", i);
			result = ituWidgetClone(AurineJDPageIndexSprite, &CloneAurineJDPageIndexSprite);
			if (result)
			{
				ituWidgetSetName(CloneAurineJDPageIndexSprite, tmp);
				ituWidgetSetX(CloneAurineJDPageIndexSprite, xpos);
				ituWidgetSetY(CloneAurineJDPageIndexSprite, ypos);
				g_AurineJDPageIndexSprite[i] = CloneAurineJDPageIndexSprite;

				ituWidgetAdd(AurineJDBackground, CloneAurineJDPageIndexSprite);
			}
			ituWidgetUpdate(AurineJDBackground, ITU_EVENT_LAYOUT, 0, 0, 0);

			if (0 == i)
			{
				ituSpriteGoto(CloneAurineJDPageIndexSprite, AurineJDPageIndexSelectIcon);
			}
			xpos += (PAGEINDEX_ICON_W + PAGEINDEX_DIS);
		}
	}
	g_PageIndex = 0;
	ituWidgetSetVisible(AurineJDPageIndexSprite, false);
}

/*************************************************
Function:		DrawScene
Description: 	画情景界面
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void DrawScene(uint8 num)
{
	char tmp[100];
	uint8 rest = 0;
	uint32 xpos, ypos;
	uint8 i, j, result, count, xindex, yindex, pagenum;
	ITUText* CloneChildAurineJDSceneText;
	ITUButton* CloneChildAurineJDSceneButton;
	ITUContainer* CloneChildAurineJDSceneContainer[2];
	ITUBackground* CloneAurineJDSceneBackground;
	ITUBackground* OldAurineJDSceneBackground;

	// 删除已经Clone的控件
	for (i = 0; i < MAX_JD_SCENE_NUM; i++)
	{
		if (1 == g_CloneSceneFlag[i])
		{
			g_CloneSceneFlag[i] = 0;
			OldAurineJDSceneBackground = NULL;
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "AurineJDSceneBackground", i);
			OldAurineJDSceneBackground = (ITUContainer*)ituSceneFindWidget(&theScene, tmp);
			itcTreeRemove(OldAurineJDSceneBackground);						// 删除已有的
			ituWidgetExit(OldAurineJDSceneBackground);
			//dprintf("Delete Clone AurineJDSceneBackground!!!\n");
		}
		// 情景按键指针初始化
		g_AurineJDSceneButton[i] = NULL;
	}

	// 情景页面初始化
	for (i = 0; i < JD_SCENE_PAGE_NUM; i++)
	{
		ituWidgetSetVisible(AurineJDSceneBackgroundPage[i], true);
	}

	// 情景总页数
	if (num <= JD_PAGE_NUM)
	{
		pagenum = 1;
	}
	else
	{
		pagenum = num / JD_PAGE_NUM;
		rest = num % JD_PAGE_NUM;
		if (rest > 0)
		{
			pagenum++;
		}
	}
	// 画页标
	DrawPageIndex(pagenum);

	// 隐藏多余情景页
	for (i = pagenum; i < JD_SCENE_PAGE_NUM; i++)
	{
		ituWidgetSetVisible(AurineJDSceneBackgroundPage[i], false);
	}
	ituCoverFlowGoto(AurineJDSceneCoverFlow, 0);
	SetSampleVisible(JD_SCENE_TYPE, true);

	xindex = 1;
	yindex = 1;
	// clone总情景个数
	count = pagenum*JD_PAGE_NUM - JD_SCENE_MAX;
	for (i = 0; i < count; i++)
	{
		result = FALSE;
		CloneChildAurineJDSceneText = NULL;
		CloneChildAurineJDSceneButton = NULL;
		CloneChildAurineJDSceneContainer[0] = NULL;
		CloneChildAurineJDSceneContainer[1] = NULL;
		CloneAurineJDSceneBackground = NULL;
		
		// 计算当前clone位置
		xpos = BUTTON_ICON_W * xindex;
		ypos = BUTTON_ICON_H * yindex;
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "AurineJDSceneBackground", i);
		result = ituWidgetClone(AurineJDSceneBackground, &CloneAurineJDSceneBackground);
		if (result)
		{
			ituWidgetSetName(CloneAurineJDSceneBackground, tmp);
			ituWidgetSetX(CloneAurineJDSceneBackground, xpos);
			ituWidgetSetY(CloneAurineJDSceneBackground, ypos);

			for (j = 0; j < 2; j++)
			{
				memset(tmp, 0, sizeof(tmp));
				CloneChildAurineJDSceneContainer[j] = itcTreeGetChildAt(CloneAurineJDSceneBackground, j);
				sprintf(tmp, "%s%d%d", "AurineJDSceneContainer", i, j);
				ituWidgetSetName(CloneChildAurineJDSceneContainer[j], tmp);
			}

			// clone Button
			memset(tmp, 0, sizeof(tmp));
			CloneChildAurineJDSceneButton = itcTreeGetChildAt(CloneChildAurineJDSceneContainer[1], 0);
			sprintf(tmp, "%s%d%d", "AurineJDSceneButton", i, 1);
			ituWidgetSetName(CloneChildAurineJDSceneButton, tmp);
			g_AurineJDSceneButton[i] = CloneChildAurineJDSceneButton;

			// clone Text
			memset(tmp, 0, sizeof(tmp));
			CloneChildAurineJDSceneText = itcTreeGetChildAt(CloneChildAurineJDSceneContainer[1], 1);
			sprintf(tmp, "%s%d%d", "AurineJDSceneText", i, 1);
			ituWidgetSetName(CloneChildAurineJDSceneText, tmp);
			ituTextSetString(CloneChildAurineJDSceneText, "");

			if ((i + JD_SCENE_MAX + 1) <= JD_PAGE_NUM)
			{
				pagenum = 1;
			}
			else
			{
				pagenum = (i + JD_SCENE_MAX + 1) / JD_PAGE_NUM;
				rest = (i + JD_SCENE_MAX + 1) % JD_PAGE_NUM;
				if (rest > 0)
				{
					pagenum++;
				}
			}
			g_CloneSceneFlag[i] = 1;
			ituWidgetAdd(AurineJDSceneBackgroundPage[pagenum - 1], CloneAurineJDSceneBackground);
		}
		ituCoverFlowUpdate((ITUWidget*)AurineJDSceneCoverFlow, ITU_EVENT_LAYOUT, 0, 0, 0);
		// 判断情景是否已启用
		if (g_SceneList && g_SceneList->pjd_scene_info[i + JD_SCENE_MAX].IsUsed)
		{
			ituWidgetSetVisible(CloneChildAurineJDSceneContainer[0], false);
			ituWidgetSetVisible(CloneChildAurineJDSceneContainer[1], true);
			// 设备名称和位置
			memset(tmp, 0, sizeof(tmp));
			#ifdef _IP_MODULE_JD_
			if (JD_FACTORY_ACBUS == storage_get_extmode(EXT_MODE_JD_FACTORY) && get_ipmodule())
			{
				strcpy(tmp, g_SceneList->pjd_scene_info[i + JD_SCENE_MAX].SceneName);
			}
			else
			#endif
			{
				char tmp2[50] = {0};
				memcpy(tmp, get_str(g_SceneList->pjd_scene_info[i + JD_SCENE_MAX].TextIDPos), 50);
				memcpy(tmp2, get_str(g_SceneList->pjd_scene_info[i + JD_SCENE_MAX].TextIDName), 50);
				strcat(tmp, tmp2);
			}
			ituTextSetString(CloneChildAurineJDSceneText, tmp);
		}
		else
		{
			ituWidgetSetVisible(CloneChildAurineJDSceneContainer[0], true);
			ituWidgetSetVisible(CloneChildAurineJDSceneContainer[1], false);
		}

		if (xindex > 2)
		{
			xindex = 0;
			yindex = !yindex;
		}
		else
		{
			xindex++;
		}
	}
	SetSampleVisible(JD_SCENE_TYPE, false);
}

/*************************************************
Function:		DrawLight
Description: 	画灯光界面
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void DrawLight(uint8 num)
{
	char tmp[100];
	uint8 rest = 0;
	uint32 xpos, ypos;
	uint8 i, j, result, count, xindex, yindex, pagenum;
	ITUText* CloneChildAurineJDLightText[2];
	ITUButton* CloneChildAurineJDLightButton[2];
	ITUSprite* CloneChildAurineJDLightSprite[2];
	ITUContainer* CloneChildAurineJDLightContainer[3];
	ITUBackground* CloneAurineJDLightBackground;
	ITUBackground* OldAurineJDLightBackground;

	// 删除已经Clone的控件
	for (i = 0; i < MAX_JD_LIGHT; i++)
	{
		if (1 == g_CloneLightFlag[i])
		{
			g_CloneLightFlag[i] = 0;
			OldAurineJDLightBackground = NULL;
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "AurineJDLightBackground", i);
			OldAurineJDLightBackground = (ITUContainer*)ituSceneFindWidget(&theScene, tmp);
			itcTreeRemove(OldAurineJDLightBackground);						// 删除已有的
			ituWidgetExit(OldAurineJDLightBackground);
			//dprintf("Delete Clone AurineJDLightBackground!!!\n");
		}
		// 灯光按键指针初始化
		g_AurineJDLightButton[i] = NULL;
		g_AurineJDLightSprite[i] = NULL;
	}

	// 灯光页面初始化
	for (i = 0; i < JD_LIGHT_PAGE_NUM; i++)
	{
		ituWidgetSetVisible(AurineJDLightBackgroundPage[i], true);
	}

	// 灯光总页数
	if (num <= JD_PAGE_NUM)
	{
		pagenum = 1;
	}
	else
	{
		pagenum = num / JD_PAGE_NUM;
		rest = num % JD_PAGE_NUM;
		if (rest > 0)
		{
			pagenum++;
		}
	}
	// 画页标
	DrawPageIndex(pagenum);

	// 隐藏多余灯光页
	for (i = pagenum; i < JD_LIGHT_PAGE_NUM; i++)
	{
		ituWidgetSetVisible(AurineJDLightBackgroundPage[i], false);
	}
	ituCoverFlowGoto(AurineJDLightCoverFlow, 0);
	SetSampleVisible(JD_DEV_LIGHT, true);

	xindex = 0;
	yindex = 0;
	// clone总灯光个数
	count = pagenum * JD_PAGE_NUM;
	for (i = 0; i < count; i++)
	{
		result = FALSE;
		for (j = 0; j < 3; j++)
		{
			CloneChildAurineJDLightContainer[j] = NULL;
			if (j < 2)
			{
				CloneChildAurineJDLightText[j] = NULL;
				CloneChildAurineJDLightButton[j] = NULL;
				CloneChildAurineJDLightSprite[j] = NULL;
			}
		}
		CloneAurineJDLightBackground = NULL;

		// 计算当前clone位置
		xpos = BUTTON_ICON_W * xindex;
		ypos = BUTTON_ICON_H * yindex;
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "AurineJDLightBackground", i);
		result = ituWidgetClone(AurineJDLightBackground, &CloneAurineJDLightBackground);
		if (result)
		{
			ituWidgetSetName(CloneAurineJDLightBackground, tmp);
			ituWidgetSetX(CloneAurineJDLightBackground, xpos);
			ituWidgetSetY(CloneAurineJDLightBackground, ypos);

			for (j = 0; j < 3; j++)
			{
				memset(tmp, 0, sizeof(tmp));
				CloneChildAurineJDLightContainer[j] = itcTreeGetChildAt(CloneAurineJDLightBackground, j);
				sprintf(tmp, "%s%d%d", "AurineJDLightContainer", i, j);
				ituWidgetSetName(CloneChildAurineJDLightContainer[j], tmp);
			}

			for (j = 0; j < 2; j++)
			{
				// clone Button
				memset(tmp, 0, sizeof(tmp));
				CloneChildAurineJDLightButton[j] = itcTreeGetChildAt(CloneChildAurineJDLightContainer[j + 1], 0);
				sprintf(tmp, "%s%d%d", "AurineJDLightButton", i, j + 1);
				ituWidgetSetName(CloneChildAurineJDLightButton[j], tmp);

				// clone Text
				memset(tmp, 0, sizeof(tmp));
				CloneChildAurineJDLightText[j] = itcTreeGetChildAt(CloneChildAurineJDLightContainer[j + 1], 1);
				sprintf(tmp, "%s%d%d", "AurineJDLightText", i, j + 1);
				ituWidgetSetName(CloneChildAurineJDLightText[j], tmp);
				ituTextSetString(CloneChildAurineJDLightText[j], "");

				// clone Sprite
				memset(tmp, 0, sizeof(tmp));
				CloneChildAurineJDLightSprite[j] = itcTreeGetChildAt(CloneChildAurineJDLightContainer[j + 1], 2);
				sprintf(tmp, "%s%d%d", "AurineJDLightSprite", i, j + 1);
				ituWidgetSetName(CloneChildAurineJDLightSprite[j], tmp);
			}

			if ((i + 1) <= JD_PAGE_NUM)
			{
				pagenum = 1;
			}
			else
			{
				pagenum = (i + 1) / JD_PAGE_NUM;
				rest = (i + 1) % JD_PAGE_NUM;
				if (rest > 0)
				{
					pagenum++;
				}
			}
			g_CloneLightFlag[i] = 1;
			ituWidgetAdd(AurineJDLightBackgroundPage[pagenum - 1], CloneAurineJDLightBackground);
		}
		ituCoverFlowUpdate((ITUWidget*)AurineJDLightCoverFlow, ITU_EVENT_LAYOUT, 0, 0, 0);

		if (g_DevList && g_DevList->pjd_dev_info[i].IsUsed)
		{
			ituWidgetSetVisible(CloneChildAurineJDLightContainer[0], false);
			if (g_DevList->pjd_dev_info[i].IsTune)
			{
				ituWidgetSetVisible(CloneChildAurineJDLightContainer[1], false);
				ituWidgetSetVisible(CloneChildAurineJDLightContainer[2], true);
				// 显示调节灯状态
				if (JD_STATE_OPEN == GetDevState(JD_DEV_LIGHT, i))
				{
					uint8 value = GetDevValue(JD_DEV_LIGHT, i);
					ituSpriteGoto(CloneChildAurineJDLightSprite[1], AurineJDReturn1LightIcon + value);
				}
				else
				{
					ituSpriteGoto(CloneChildAurineJDLightSprite[1], AurineJDReturn0LightIcon);
				}
				g_AurineJDLightButton[i] = CloneChildAurineJDLightButton[1];
				g_AurineJDLightSprite[i] = CloneChildAurineJDLightSprite[1];
			}
			else
			{
				ituWidgetSetVisible(CloneChildAurineJDLightContainer[1], true);
				ituWidgetSetVisible(CloneChildAurineJDLightContainer[2], false);
				// 显示灯状态
				if (JD_STATE_OPEN == GetDevState(JD_DEV_LIGHT, i))
				{
					ituSpriteGoto(CloneChildAurineJDLightSprite[0], AurineJDOpenLightIcon);
				}
				else
				{
					ituSpriteGoto(CloneChildAurineJDLightSprite[0], AurineJDCloseLightIcon);
				}
				g_AurineJDLightButton[i] = CloneChildAurineJDLightButton[0];
				g_AurineJDLightSprite[i] = CloneChildAurineJDLightSprite[0];
			}

			// 设备名称和位置
			memset(tmp, 0, sizeof(tmp));
			GetDevText(i, tmp);
			for (j = 0; j < 2; j++)
			{
				ituTextSetString(CloneChildAurineJDLightText[j], tmp);
			}
		}
		else
		{
			ituWidgetSetVisible(CloneChildAurineJDLightContainer[0], true);
			ituWidgetSetVisible(CloneChildAurineJDLightContainer[1], false);
			ituWidgetSetVisible(CloneChildAurineJDLightContainer[2], false);
		}
			
		if (xindex > 2)
		{
			xindex = 0;
			yindex = !yindex;
		}
		else
		{
			xindex++;
		}
	}
	SetSampleVisible(JD_DEV_LIGHT, false);
}

/*************************************************
Function:		DrawAir
Description: 	画空调界面
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void DrawAir(uint8 num)
{
	char tmp[100];
	uint8 i, j, count, result;
	ITUText* CloneChildAurineJDAirDevNameText;
	ITUSprite* CloneChildAurineJDAirIndexNumSprite[3];
	ITUContainer* CloneChildAurineJDAirNumContainer;
	ITUText* CloneChildAurineJDAirShowModeText;
	ITUSprite* CloneChildAurineJDAirShowModeSprite;
	ITUContainer* CloneChildAurineJDAirtContainer[2];
	ITUBackground* CloneAurineJDAirBackground;
	ITUBackground* OldAurineJDAirBackground;

	// 删除已经Clone的控件
	for (i = 0; i < MAX_JD_KONGTIAO; i++)
	{
		if (1 == g_CloneAirFlag[i])
		{
			g_CloneAirFlag[i] = 0;
			OldAurineJDAirBackground = NULL;
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "AurineJDAirBackground", i);
			OldAurineJDAirBackground = (ITUContainer*)ituSceneFindWidget(&theScene, tmp);
			itcTreeRemove(OldAurineJDAirBackground);						// 删除已有的
			ituWidgetExit(OldAurineJDAirBackground);
			//dprintf("Delete Clone AurineJDAirBackground!!!\n");
		}
		// 空调指针初始化
		g_AurineJDAirShowModeSprite[i] = NULL;
		g_AurineJDAirShowModeText[i] = NULL;
		g_AurineJDAirNumContainer[i] = NULL;
		for (j = 0; j < 10; j++)
		{
			if (j < 3)
			{
				g_AurineJDAirIndexNumSprite[i][j] = NULL;
			}
			g_AurineJDAirButton[i][j] = NULL;
		}
	}

	if (0 == num)
	{
		count = 1;
	}
	else
	{
		count = num;
	}
	// 画页标
	DrawPageIndex(count);

	// 空调页面初始化
	for (i = 0; i < JD_AIR_PAGE_NUM; i++)
	{
		ituWidgetSetVisible(AurineJDAirBackgroundPage[i], true);
	}

	// 隐藏多余空调页
	for (i = count; i < JD_AIR_PAGE_NUM; i++)
	{
		ituWidgetSetVisible(AurineJDAirBackgroundPage[i], false);
	}
	ituCoverFlowGoto(AurineJDAirCoverFlow, 0);
	SetSampleVisible(JD_DEV_KONGTIAO, true);

	// clone总空调个数
	for (i = 0; i < count; i++)
	{
		result = FALSE;
		CloneChildAurineJDAirDevNameText = NULL;
		CloneChildAurineJDAirNumContainer = NULL;
		CloneChildAurineJDAirShowModeText = NULL;
		CloneChildAurineJDAirShowModeSprite = NULL;
		CloneAurineJDAirBackground = NULL;
		for (j = 0; j < 3; j++)
		{
			CloneChildAurineJDAirIndexNumSprite[j] = NULL;
			if (j < 2)
			{
				CloneChildAurineJDAirtContainer[j] = NULL;
			}
		}

		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "AurineJDAirBackground", i);
		result = ituWidgetClone(AurineJDAirBackground, &CloneAurineJDAirBackground);
		if (result)
		{
			ituWidgetSetName(CloneAurineJDAirBackground, tmp);
			ituWidgetSetX(CloneAurineJDAirBackground, 0);
			ituWidgetSetY(CloneAurineJDAirBackground, 0);

			// clone 空调Container
			for (j = 0; j < 2; j++)
			{
				memset(tmp, 0, sizeof(tmp));
				CloneChildAurineJDAirtContainer[j] = itcTreeGetChildAt(CloneAurineJDAirBackground, j);
				sprintf(tmp, "%s%d%d", "AurineJDAirtContainer", i, j);
				ituWidgetSetName(CloneChildAurineJDAirtContainer[j], tmp);
			}
			ituWidgetDisable(CloneChildAurineJDAirtContainer[0]);

			// clone 显示状态、模式Sprite
			memset(tmp, 0, sizeof(tmp));
			CloneChildAurineJDAirShowModeSprite = itcTreeGetChildAt(CloneChildAurineJDAirtContainer[1], 1);
			sprintf(tmp, "%s%d%d", "AurineJDAirShowModeSprite", i, 1);
			ituWidgetSetName(CloneChildAurineJDAirShowModeSprite, tmp);

			// clone 显示模式Text
			memset(tmp, 0, sizeof(tmp));
			CloneChildAurineJDAirShowModeText = itcTreeGetChildAt(CloneChildAurineJDAirtContainer[1], 2);
			sprintf(tmp, "%s%d%d", "AurineJDAirShowModeText", i, 1);
			ituWidgetSetName(CloneChildAurineJDAirShowModeText, tmp);
			ituTextSetString(CloneChildAurineJDAirShowModeText, "");

			// clone 显示设备名称Text
			memset(tmp, 0, sizeof(tmp));
			CloneChildAurineJDAirDevNameText = itcTreeGetChildAt(CloneChildAurineJDAirtContainer[1], 4);
			sprintf(tmp, "%s%d%d", "AurineJDAirDevNameText", i, 1);
			ituWidgetSetName(CloneChildAurineJDAirDevNameText, tmp);
			ituTextSetString(CloneChildAurineJDAirDevNameText, "");

			// clone 显示状态、温度Container
			memset(tmp, 0, sizeof(tmp));
			CloneChildAurineJDAirNumContainer = itcTreeGetChildAt(CloneChildAurineJDAirtContainer[1], 3);
			sprintf(tmp, "%s%d%d", "AurineJDAirNumContainer", i, 1);
			ituWidgetSetName(CloneChildAurineJDAirNumContainer, tmp);

			// 记录按键指针
			for (j = 0; j < 10; j++)
			{
				memset(tmp, 0, sizeof(tmp));
				g_AurineJDAirButton[i][j] = itcTreeGetChildAt(CloneChildAurineJDAirtContainer[1], 5 + j);
				sprintf(tmp, "%s%d%d", "AurineJDAirButton", i, j);
				ituWidgetSetName(g_AurineJDAirButton[i][j], tmp);
			}

			// clone 显示状态、温度Sprite
			for (j = 0; j < 3; j++)
			{
				memset(tmp, 0, sizeof(tmp));
				CloneChildAurineJDAirIndexNumSprite[j] = itcTreeGetChildAt(CloneChildAurineJDAirNumContainer, j);
				sprintf(tmp, "%s%d%d", "AurineJDAirIndexNumSprite", i, j);
				ituWidgetSetName(CloneChildAurineJDAirIndexNumSprite[j], tmp);
			}
			g_CloneAirFlag[i] = 1;
			ituWidgetAdd(AurineJDAirBackgroundPage[i], CloneAurineJDAirBackground);
		}
		ituCoverFlowUpdate((ITUWidget*)AurineJDAirCoverFlow, ITU_EVENT_LAYOUT, 0, 0, 0);
		// 判断空调是否已启用
		if (g_DevList && g_DevList->pjd_dev_info[i].IsUsed)
		{
			ituWidgetSetVisible(CloneChildAurineJDAirtContainer[0], false);
			ituWidgetSetVisible(CloneChildAurineJDAirtContainer[1], true);
			if (JD_STATE_CLOSE == GetDevState(JD_DEV_KONGTIAO, i))
			{
				// 显示关状态
				ituSpriteGoto(CloneChildAurineJDAirShowModeSprite, AurineJDAirShowCloseIcon);
				ituWidgetSetVisible(CloneChildAurineJDAirShowModeText, false);
				ituWidgetSetVisible(CloneChildAurineJDAirNumContainer, true);
						
				ituSpriteGoto(CloneChildAurineJDAirIndexNumSprite[0], AurineJDAirIndex0OIcon);
				ituSpriteGoto(CloneChildAurineJDAirIndexNumSprite[1], AurineJDAirIndex1FIcon);
				ituSpriteGoto(CloneChildAurineJDAirIndexNumSprite[2], AurineJDAirIndex2FIcon);
			}
			else
			{
				uint8 mode = GetDevLastMode(i);
				uint8 value = GetDevValue(JD_DEV_KONGTIAO, i);
				uint8 lastvalue = GetAirLastValue(i);
				uint8 ColdHot = GetDevColdHot(i);
				if (0 == mode)
				{
					// 最初制冷、制暖模式
					ituSpriteGoto(CloneChildAurineJDAirShowModeSprite, AurineJDAirShowColdIcon + ColdHot);
					if (value == 0 && lastvalue == 0)
					{
						value = 5;
					}
					else
					{
						value = lastvalue;
					}
					// 显示温度
					ituWidgetSetVisible(CloneChildAurineJDAirShowModeText, false);
					ituWidgetSetVisible(CloneChildAurineJDAirNumContainer, true);

					if (value == 10)
					{
						ituSpriteGoto(CloneChildAurineJDAirIndexNumSprite[0], AurineJDAirIndex0Num3Icon);
						ituSpriteGoto(CloneChildAurineJDAirIndexNumSprite[1], AurineJDAirIndex1Num0Icon);
					}
					else
					{
						ituSpriteGoto(CloneChildAurineJDAirIndexNumSprite[0], AurineJDAirIndex0Num2Icon);
						ituSpriteGoto(CloneChildAurineJDAirIndexNumSprite[1], AurineJDAirIndex1Num0Icon + value);
					}
					ituSpriteGoto(CloneChildAurineJDAirIndexNumSprite[2], AurineJDAirIndex2SXDIcon);
				}
				else
				{
					// 最后显示模式
					ituSpriteGoto(CloneChildAurineJDAirShowModeSprite, AurineJDAirShowColdIcon + mode - 1);
					if (mode > 0 && mode <= 2)
					{
						if (value == 0 && lastvalue == 0)
						{
							value = 5;
						}
						else
						{
							value = lastvalue;
						}
						// 显示温度
						ituWidgetSetVisible(CloneChildAurineJDAirShowModeText, false);
						ituWidgetSetVisible(CloneChildAurineJDAirNumContainer, true);

						if (value == 10)
						{
							ituSpriteGoto(CloneChildAurineJDAirIndexNumSprite[0], AurineJDAirIndex0Num3Icon);
							ituSpriteGoto(CloneChildAurineJDAirIndexNumSprite[1], AurineJDAirIndex1Num0Icon);
						}
						else
						{
							ituSpriteGoto(CloneChildAurineJDAirIndexNumSprite[0], AurineJDAirIndex0Num2Icon);
							ituSpriteGoto(CloneChildAurineJDAirIndexNumSprite[1], AurineJDAirIndex1Num0Icon + value);
						}
						ituSpriteGoto(CloneChildAurineJDAirIndexNumSprite[2], AurineJDAirIndex2SXDIcon);
					}
					else if (mode > 2 && mode < 7)
					{
						// 显示模式
						uint32 textid[4] = {SID_Jd_KongTiao_Refre, SID_Jd_KongTiao_Warm, SID_Jd_KongTiao_Leisure, SID_Jd_KongTiao_Sleep};

						ituWidgetSetVisible(CloneChildAurineJDAirNumContainer, false);
						ituWidgetSetVisible(CloneChildAurineJDAirShowModeText, true);
						ituTextSetString(CloneChildAurineJDAirShowModeText, get_str(textid[mode-3]));
					}
				}
			}
			// 设备名称和位置
			memset(tmp, 0, sizeof(tmp));
			GetDevText(i, tmp);
			ituTextSetString(CloneChildAurineJDAirDevNameText, tmp);

			g_AurineJDAirShowModeSprite[i] = CloneChildAurineJDAirShowModeSprite;
			g_AurineJDAirShowModeText[i] = CloneChildAurineJDAirShowModeText;
			g_AurineJDAirNumContainer[i] = CloneChildAurineJDAirNumContainer;
			for (j = 0; j < 3; j++)
			{
				g_AurineJDAirIndexNumSprite[i][j] = CloneChildAurineJDAirIndexNumSprite[j];
			}			
		}
		else
		{
			ituWidgetSetVisible(CloneChildAurineJDAirtContainer[0], true);
			ituWidgetSetVisible(CloneChildAurineJDAirtContainer[1], false);
		}
	}
	SetSampleVisible(JD_DEV_KONGTIAO, false);
}

/*************************************************
Function:		DrawCurtain
Description: 	画窗帘界面
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void DrawCurtain(uint8 num)
{
	char tmp[100];
	uint8 rest = 0;
	uint8 i, j, result, count, index, pagenum;
	ITUText* CloneChildAurineJDCurtainDevNameText;
	ITUSprite* CloneChildAurineJDCurtainSateSprite;
	ITUContainer* CloneChildAurineJDCurtainContainer[2];
	ITUBackground* CloneAurineJDCurtainBackground;
	ITUBackground* OldAurineJDCurtainBackground;

	// 删除已经Clone的控件
	for (i = 0; i < MAX_JD_WINDOW; i++)
	{
		if (1 == g_CloneCurtainFlag[i])
		{
			g_CloneCurtainFlag[i] = 0;
			OldAurineJDCurtainBackground = NULL;
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "AurineJDCurtainBackground", i);
			OldAurineJDCurtainBackground = (ITUContainer*)ituSceneFindWidget(&theScene, tmp);
			itcTreeRemove(OldAurineJDCurtainBackground);						// 删除已有的
			ituWidgetExit(OldAurineJDCurtainBackground);
			//dprintf("Delete Clone AurineJDCurtainBackground!!!\n");
		}
		// 窗帘指针初始化
		g_AurineJDCurtainSateSprite[i] = NULL;
		for (j = 0; j < 3; j++)
		{
			g_AurineJDCurtainButton[i][j] = NULL;
		}
	}

	// 窗帘页面初始化
	for (i = 0; i < JD_CURTAIN_PAGE_NUM; i++)
	{
		ituWidgetSetVisible(AurineJDCurtainBackgroundPage[i], true);
	}

	// 窗帘总页数
	if (num <= JD_CUITAIN_NUM)
	{
		pagenum = 1;
	}
	else
	{
		pagenum = num / JD_CUITAIN_NUM;
		rest = num % JD_CUITAIN_NUM;
		if (rest > 0)
		{
			pagenum++;
		}
	}
	// 画页标
	DrawPageIndex(pagenum);

	// 隐藏多余窗帘页
	for (i = pagenum; i < JD_CURTAIN_PAGE_NUM; i++)
	{
		ituWidgetSetVisible(AurineJDCurtainBackgroundPage[i], false);
	}
	ituCoverFlowGoto(AurineJDCurtainCoverFlow, 0);
	SetSampleVisible(JD_DEV_WINDOW, true);

	index = 0;
	// clone总窗帘个数
	count = pagenum * JD_CUITAIN_NUM;
	for (i = 0; i < count; i++)
	{
		result = FALSE;

		for (j = 0; j < 2; j++)
		{
			CloneChildAurineJDCurtainContainer[j] = NULL;
		}
		CloneChildAurineJDCurtainDevNameText = NULL;
		CloneChildAurineJDCurtainSateSprite = NULL;
		CloneAurineJDCurtainBackground = NULL;

		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "AurineJDCurtainBackground", i);
		result = ituWidgetClone(AurineJDCurtainBackground, &CloneAurineJDCurtainBackground);
		if (result)
		{
			ituWidgetSetName(CloneAurineJDCurtainBackground, tmp);
			ituWidgetSetX(CloneAurineJDCurtainBackground, BUTTON_CUITAIN_ICON_W * index);
			ituWidgetSetY(CloneAurineJDCurtainBackground, 0);

			for (j = 0; j < 2; j++)
			{
				memset(tmp, 0, sizeof(tmp));
				CloneChildAurineJDCurtainContainer[j] = itcTreeGetChildAt(CloneAurineJDCurtainBackground, j);
				sprintf(tmp, "%s%d%d", "AurineJDCurtainContainer", i, j);
				ituWidgetSetName(CloneChildAurineJDCurtainContainer[j], tmp);
			}
			ituWidgetDisable(CloneChildAurineJDCurtainContainer[0]);

			// clone Sprite
			memset(tmp, 0, sizeof(tmp));
			CloneChildAurineJDCurtainSateSprite = itcTreeGetChildAt(CloneChildAurineJDCurtainContainer[1], 0);
			sprintf(tmp, "%s%d%d", "AurineJDCurtainSateSprite", i, 1);
			ituWidgetSetName(CloneChildAurineJDCurtainSateSprite, tmp);

			// 记录按键指针
			for (j = 0; j < 3; j++)
			{
				memset(tmp, 0, sizeof(tmp));
				g_AurineJDCurtainButton[i][j] = itcTreeGetChildAt(CloneChildAurineJDCurtainContainer[1], j + 1);
				sprintf(tmp, "%s%d%d", "AurineJDCurtainButton", i, j);
				ituWidgetSetName(g_AurineJDCurtainButton[i][j], tmp);
			}

			// clone Text
			memset(tmp, 0, sizeof(tmp));
			CloneChildAurineJDCurtainDevNameText = itcTreeGetChildAt(CloneChildAurineJDCurtainContainer[1], 4);
			sprintf(tmp, "%s%d%d", "AurineJDCurtainDevNameText", i, 1);
			ituWidgetSetName(CloneChildAurineJDCurtainDevNameText, tmp);
			ituTextSetString(CloneChildAurineJDCurtainDevNameText, "");
			
			if ((i + 1) <= JD_CUITAIN_NUM)
			{
				pagenum = 1;
			}
			else
			{
				pagenum = (i + 1) / JD_CUITAIN_NUM;
				rest = (i + 1) % JD_CUITAIN_NUM;
				if (rest > 0)
				{
					pagenum++;
				}
			}
			g_CloneCurtainFlag[i] = 1;
			ituWidgetAdd(AurineJDCurtainBackgroundPage[pagenum - 1], CloneAurineJDCurtainBackground);
		}
		ituCoverFlowUpdate((ITUWidget*)AurineJDCurtainCoverFlow, ITU_EVENT_LAYOUT, 0, 0, 0);

		if (g_DevList && g_DevList->pjd_dev_info[i].IsUsed)
		{
			ituWidgetSetVisible(CloneChildAurineJDCurtainContainer[0], false);
			ituWidgetSetVisible(CloneChildAurineJDCurtainContainer[1], true);

			// 显示灯状态
			if (JD_STATE_OPEN == GetDevState(JD_DEV_WINDOW, i))
			{
				ituSpriteGoto(CloneChildAurineJDCurtainSateSprite, AurineJDCurtainOpenIcon);
			}
			else
			{
				ituSpriteGoto(CloneChildAurineJDCurtainSateSprite, AurineJDCurtainCloseIcon);
			}
			g_AurineJDCurtainSateSprite[i] = CloneChildAurineJDCurtainSateSprite;

			// 设备名称和位置
			memset(tmp, 0, sizeof(tmp));
			GetDevText(i, tmp);
			ituTextSetString(CloneChildAurineJDCurtainDevNameText, tmp);
		}
		else
		{
			ituWidgetSetVisible(CloneChildAurineJDCurtainContainer[0], true);
			ituWidgetSetVisible(CloneChildAurineJDCurtainContainer[1], false);
		}
		index = !index;
	}
	SetSampleVisible(JD_DEV_WINDOW, false);
}

/*************************************************
Function:		DrawPower
Description: 	电源界面
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void DrawPower(uint8 num)
{
	char tmp[100];
	uint8 rest = 0;
	uint32 xpos, ypos;
	uint8 i, j, result, count, xindex, yindex, pagenum;
	ITUText* CloneChildAurineJDPowerText;
	ITUSprite* CloneChildAurineJDPowerSprite;
	ITUButton* CloneChildAurineJDPowerButton;
	ITUContainer* CloneChildAurineJDPowerContainer[2];
	ITUBackground* CloneAurineJDPowerBackground;
	ITUBackground* OldAurineJDPowerBackground;

	// 删除已经Clone的控件
	for (i = 0; i < MAX_JD_POWER; i++)
	{
		if (1 == g_ClonePowerFlag[i])
		{
			g_ClonePowerFlag[i] = 0;
			OldAurineJDPowerBackground = NULL;
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "AurineJDPowerBackground", i);
			OldAurineJDPowerBackground = (ITUContainer*)ituSceneFindWidget(&theScene, tmp);
			itcTreeRemove(OldAurineJDPowerBackground);						// 删除已有的
			ituWidgetExit(OldAurineJDPowerBackground);
			//dprintf("Delete Clone AurineJDPowerBackground!!!\n");
		}
		// 插座指针初始化
		g_AurineJDPowerButton[i] = NULL;
		g_AurineJDPowerSprite[i] = NULL;
	}

	// 灯光页面初始化
	for (i = 0; i < JD_POWER_PAGE_NUM; i++)
	{
		ituWidgetSetVisible(AurineJDLightBackgroundPage[i], true);
	}

	// 插座总页数
	if (num <= JD_PAGE_NUM)
	{
		pagenum = 1;
	}
	else
	{
		pagenum = num / JD_PAGE_NUM;
		rest = num % JD_PAGE_NUM;
		if (rest > 0)
		{
			pagenum++;
		}
	}
	// 画页标
	DrawPageIndex(pagenum);

	// 隐藏多余设备页
	for (i = pagenum; i < JD_LIGHT_PAGE_NUM; i++)
	{
		ituWidgetSetVisible(AurineJDLightBackgroundPage[i], false);
	}	
	ituCoverFlowGoto(AurineJDLightCoverFlow, 0);
	SetSampleVisible(JD_DEV_POWER, true);

	xindex = 0;
	yindex = 0;
	// clone插座个数
	count = pagenum * JD_PAGE_NUM;
	for (i = 0; i < count; i++)
	{
		result = FALSE;
		for (j = 0; j < 2; j++)
		{
			CloneChildAurineJDPowerContainer[j] = NULL;
		}
		CloneChildAurineJDPowerText = NULL;
		CloneChildAurineJDPowerSprite = NULL;
		CloneChildAurineJDPowerButton = NULL;
		CloneAurineJDPowerBackground = NULL;

		// 计算当前clone位置
		xpos = BUTTON_ICON_W * xindex;
		ypos = BUTTON_ICON_H * yindex;

		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "AurineJDPowerBackground", i);
		result = ituWidgetClone(AurineJDPowerBackground, &CloneAurineJDPowerBackground);
		if (result)
		{
			ituWidgetSetName(CloneAurineJDPowerBackground, tmp);
			ituWidgetSetX(CloneAurineJDPowerBackground, xpos);
			ituWidgetSetY(CloneAurineJDPowerBackground, ypos);

			for (j = 0; j < 2; j++)
			{
				memset(tmp, 0, sizeof(tmp));
				CloneChildAurineJDPowerContainer[j] = itcTreeGetChildAt(CloneAurineJDPowerBackground, j);
				sprintf(tmp, "%s%d%d", "AurineJDPowerContainer", i, j);
				ituWidgetSetName(CloneChildAurineJDPowerContainer[j], tmp);
			}

			// Clone Button
			memset(tmp, 0, sizeof(tmp));
			CloneChildAurineJDPowerButton = itcTreeGetChildAt(CloneChildAurineJDPowerContainer[1], 0);
			sprintf(tmp, "%s%d%d", "AurineJDPowerButton", i, 1);
			ituWidgetSetName(CloneChildAurineJDPowerButton, tmp);

			// clone Text
			memset(tmp, 0, sizeof(tmp));
			CloneChildAurineJDPowerText = itcTreeGetChildAt(CloneChildAurineJDPowerContainer[1], 1);
			sprintf(tmp, "%s%d%d", "AurineJDPowerText", i, 1);
			ituWidgetSetName(CloneChildAurineJDPowerText, tmp);
			ituTextSetString(CloneChildAurineJDPowerText, "");

			// clone Sprite
			memset(tmp, 0, sizeof(tmp));
			CloneChildAurineJDPowerSprite = itcTreeGetChildAt(CloneChildAurineJDPowerContainer[1], 2);
			sprintf(tmp, "%s%d%d", "AurineJDPowerSprite", i, 1);
			ituWidgetSetName(CloneChildAurineJDPowerSprite, tmp);

			if ((i + 1) <= JD_PAGE_NUM)
			{
				pagenum = 1;
			}
			else
			{
				pagenum = (i + 1) / JD_PAGE_NUM;
				rest = (i + 1) % JD_PAGE_NUM;
				if (rest > 0)
				{
					pagenum++;
				}
			}
			g_ClonePowerFlag[i] = 1;
			ituWidgetAdd(AurineJDLightBackgroundPage[pagenum - 1], CloneAurineJDPowerBackground);
		}
		ituCoverFlowUpdate((ITUWidget*)AurineJDLightCoverFlow, ITU_EVENT_LAYOUT, 0, 0, 0);
		
		if (g_DevList && g_DevList->pjd_dev_info[i].IsUsed)
		{
			ituWidgetSetVisible(CloneChildAurineJDPowerContainer[0], false);
			ituWidgetSetVisible(CloneChildAurineJDPowerContainer[1], true);
			// 显示灯状态
			if (JD_STATE_OPEN == GetDevState(JD_DEV_POWER, i))
			{
				ituSpriteGoto(CloneChildAurineJDPowerSprite, AurineJDDevOpenIcon);
			}
			else
			{
				ituSpriteGoto(CloneChildAurineJDPowerSprite, AurineJDDevCloseIcon);
			}

			g_AurineJDPowerButton[i] = CloneChildAurineJDPowerButton;
			g_AurineJDPowerSprite[i] = CloneChildAurineJDPowerSprite;
	
			// 设备名称和位置
			memset(tmp, 0, sizeof(tmp));
			GetDevText(i, tmp);
			ituTextSetString(CloneChildAurineJDPowerText, tmp);
		}
		else
		{
			ituWidgetSetVisible(CloneChildAurineJDPowerContainer[0], true);
			ituWidgetSetVisible(CloneChildAurineJDPowerContainer[1], false);
		}

		if (xindex > 2)
		{
			xindex = 0;
			yindex = !yindex;
		}
		else
		{
			xindex++;
		}
	}
	SetSampleVisible(JD_DEV_POWER, false);
}

/*************************************************
Function:		DrawGas
Description: 	煤气界面
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void DrawGas(uint8 num)
{
	char tmp[100];
	uint8 rest = 0;
	uint32 xpos, ypos;
	uint8 maxpage = 0;
	uint8 i, j, result, count, xindex, yindex, pagenum;
	ITUText* CloneChildAurineJDGasText;
	ITUSprite* CloneChildAurineJDGasSprite;
	ITUButton* CloneChildAurineJDGasButton;
	ITUContainer* CloneChildAurineJDGasContainer[2];
	ITUBackground* CloneAurineJDGasBackground;
	ITUBackground* OldAurineJDGasBackground;

	// 删除已经Clone的控件
	for (i = 0; i < JD_PAGE_NUM; i++)
	{
		if (1 == g_CloneGasFlag[i])
		{
			g_CloneGasFlag[i] = 0;
			OldAurineJDGasBackground = NULL;
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "AurineJDGasBackground", i);
			OldAurineJDGasBackground = (ITUContainer*)ituSceneFindWidget(&theScene, tmp);
			itcTreeRemove(OldAurineJDGasBackground);						// 删除已有的
			ituWidgetExit(OldAurineJDGasBackground);
			//dprintf("Delete Clone AurineJDGasBackground!!!\n");
		}
	}
	
	// 煤气指针初始化
	for (i = 0; i < MAX_JD_GAS; i++)
	{
		g_AurineJDGasButton[i] = NULL;
		g_AurineJDGasSprite[i] = NULL;
	}

	// 灯光页面初始化
	for (i = 0; i < JD_GAS_PAGE_NUM; i++)
	{
		ituWidgetSetVisible(AurineJDLightBackgroundPage[i], true);
	}

	// 设备总页数
	if (num <= JD_PAGE_NUM)
	{
		pagenum = 1;
	}
	else
	{
		pagenum = num / JD_PAGE_NUM;
		rest = num % JD_PAGE_NUM;
		if (rest > 0)
		{
			pagenum++;
		}
	}
	// 画页标
	DrawPageIndex(pagenum);

	// 隐藏多余设备页
	for (i = pagenum; i < JD_LIGHT_PAGE_NUM; i++)
	{
		ituWidgetSetVisible(AurineJDLightBackgroundPage[i], false);
	}
	ituCoverFlowGoto(AurineJDLightCoverFlow, 0);
	SetSampleVisible(JD_DEV_GAS, true);

	xindex = 0;
	yindex = 0;
	// clone总设备个数
	count = pagenum * JD_PAGE_NUM;
	for (i = 0; i < count; i++)
	{
		result = FALSE;
		for (j = 0; j < 2; j++)
		{
			CloneChildAurineJDGasContainer[j] = NULL;
		}
		CloneChildAurineJDGasText = NULL;
		CloneChildAurineJDGasSprite = NULL;
		CloneChildAurineJDGasButton = NULL;
		CloneAurineJDGasBackground = NULL;

		// 计算当前clone位置
		xpos = BUTTON_ICON_W * xindex;
		ypos = BUTTON_ICON_H * yindex;

		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s%d", "AurineJDGasBackground", i);
		result = ituWidgetClone(AurineJDGasBackground, &CloneAurineJDGasBackground);
		if (result)
		{
			ituWidgetSetName(CloneAurineJDGasBackground, tmp);
			ituWidgetSetX(CloneAurineJDGasBackground, xpos);
			ituWidgetSetY(CloneAurineJDGasBackground, ypos);

			for (j = 0; j < 2; j++)
			{
				memset(tmp, 0, sizeof(tmp));
				CloneChildAurineJDGasContainer[j] = itcTreeGetChildAt(CloneAurineJDGasBackground, j);
				sprintf(tmp, "%s%d%d", "AurineJDGasContainer", i, j);
				ituWidgetSetName(CloneChildAurineJDGasContainer[j], tmp);
			}

			// Clone Button
			memset(tmp, 0, sizeof(tmp));
			CloneChildAurineJDGasButton = itcTreeGetChildAt(CloneChildAurineJDGasContainer[1], 0);
			sprintf(tmp, "%s%d%d", "AurineJDGasButton", i, 1);
			ituWidgetSetName(CloneChildAurineJDGasButton, tmp);

			// clone Text
			memset(tmp, 0, sizeof(tmp));
			CloneChildAurineJDGasText = itcTreeGetChildAt(CloneChildAurineJDGasContainer[1], 1);
			sprintf(tmp, "%s%d%d", "AurineJDGasText", i, 1);
			ituWidgetSetName(CloneChildAurineJDGasText, tmp);
			ituTextSetString(CloneChildAurineJDGasText, "");

			// clone Sprite
			memset(tmp, 0, sizeof(tmp));
			CloneChildAurineJDGasSprite = itcTreeGetChildAt(CloneChildAurineJDGasContainer[1], 2);
			sprintf(tmp, "%s%d%d", "AurineJDGasSprite", i, 1);
			ituWidgetSetName(CloneChildAurineJDGasSprite, tmp);

			if ((i + 1) <= JD_PAGE_NUM)
			{
				pagenum = 1;
			}
			else
			{
				pagenum = (i + 1) / JD_PAGE_NUM;
				rest = (i + 1) % JD_PAGE_NUM;
				if (rest > 0)
				{
					pagenum++;
				}
			}
			g_CloneGasFlag[i] = 1;
			ituWidgetAdd(AurineJDLightBackgroundPage[pagenum - 1], CloneAurineJDGasBackground);
		}
		ituCoverFlowUpdate((ITUWidget*)AurineJDLightCoverFlow, ITU_EVENT_LAYOUT, 0, 0, 0);

		ituWidgetSetVisible(CloneChildAurineJDGasContainer[0], true);
		ituWidgetSetVisible(CloneChildAurineJDGasContainer[1], false);
		if (i < MAX_JD_GAS)
		{
			if (g_DevList && g_DevList->pjd_dev_info[i].IsUsed)
			{
				ituWidgetSetVisible(CloneChildAurineJDGasContainer[0], false);
				ituWidgetSetVisible(CloneChildAurineJDGasContainer[1], true);
				// 显示灯状态
				if (JD_STATE_OPEN == GetDevState(JD_DEV_GAS, i))
				{
					ituSpriteGoto(CloneChildAurineJDGasSprite, AurineJDDevOpenIcon);
				}
				else
				{
					ituSpriteGoto(CloneChildAurineJDGasSprite, AurineJDDevCloseIcon);
				}

				g_AurineJDGasButton[i] = CloneChildAurineJDGasButton;
				g_AurineJDGasSprite[i] = CloneChildAurineJDGasSprite;

				// 设备名称和位置
				memset(tmp, 0, sizeof(tmp));
				GetDevText(i, tmp);
				ituTextSetString(CloneChildAurineJDGasText, tmp);
			}
		}

		if (xindex > 2)
		{
			xindex = 0;
			yindex = !yindex;
		}
		else
		{
			xindex++;
		}
	}
	SetSampleVisible(JD_DEV_GAS, false);
}

/*************************************************
Function:		ShowSceneType
Description: 	显示情景界面
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void ShowSceneType(void)
{
	free_scene_memory(&g_SceneList);
	#ifdef _SHOW_USED_JDDEV_ONLY_
	g_SceneList = storage_get_aurine_scene_info(1);
	#else
	g_SceneList = storage_get_aurine_scene_info(0);
	#endif
	if (g_SceneList && g_SceneList->nCount > 0)
	{
		DrawScene(g_SceneList->nCount);
	}
}

/*************************************************
Function:		ShowDevType
Description: 	显示界面
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void ShowDevType(AU_JD_DEV_TYPE dev)
{
	uint8 i;

	free_jd_memory(&g_DevList);
	#ifdef _SHOW_USED_JDDEV_ONLY_
	g_DevList = storage_get_jddev(dev, TRUE);
	#else
	g_DevList = storage_get_jddev(dev, FALSE);
	#endif

	storage_get_aurine_light_value(g_LightValue);
	#ifdef _IP_MODULE_
	storage_get_aurine_kongtiao_value(g_AirValue);
	#endif

	for (i = 0; i < MAX_JD_LIGHT; i++)
	{
		g_JDState.devlight[i][0] = jd_aurine_get_dev_state(JD_DEV_LIGHT, i);
		if (g_JDState.devlight[i][0] == JD_STATE_CLOSE)
		{
			g_JDState.devlight[i][1] = 0;
		}
		else
		{
			g_JDState.devlight[i][1] = g_LightValue[i];
		}
	}

	for (i = 0; i < MAX_JD_KONGTIAO; i++)
	{
		g_JDState.devKongtiao[i][0] = jd_aurine_get_dev_state(JD_DEV_KONGTIAO, i);
		#ifdef _IP_MODULE_
		g_JDState.devKongtiao[i][1] = g_AirValue[i];
		#endif
	}

	for (i = 0; i < MAX_JD_POWER; i++)
	{
		g_JDState.devpower[i] = jd_aurine_get_dev_state(JD_DEV_POWER, i);
	}

	for (i = 0; i < MAX_JD_WINDOW; i++)
	{
		g_JDState.devwindow[i] = jd_aurine_get_dev_state(JD_DEV_WINDOW, i);
	}

	for (i = 0; i < MAX_JD_GAS; i++)
	{
		g_JDState.devgas[i] = jd_aurine_get_dev_state(JD_DEV_GAS, i);
	}

	jd_set_state_list(dev, g_DevList);
	ReadDevStateOnce();
	if (g_DevList)
	{
		switch (dev)
		{
			case JD_DEV_LIGHT:
				DrawLight(g_DevList->nCount);
				break;

			case JD_DEV_KONGTIAO:
				DrawAir(g_DevList->nCount);
				break;

			case JD_DEV_WINDOW:
				DrawCurtain(g_DevList->nCount);
				break;

			case JD_DEV_POWER:
				DrawPower(g_DevList->nCount);
				break;

			case JD_DEV_GAS:
				DrawGas(g_DevList->nCount);
				break;

			default:
				break;
		}
	}
}

/*************************************************
Function:		AurineJDLayerOnLeave
Description:	销毁处理函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool AurineJDLayerOnLeave(ITUWidget* widget, char* param)
{
	ituSpriteGoto(AurineJDBottomSprite[g_BtnEvent - 2], AurineJDButtomNoPressIcon);

	return true;
}

/*************************************************
Function:		AurineJDLayerOnTimer
Description: 	定时器
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool AurineJDLayerOnTimer(ITUWidget* widget, char* param)
{
	uint32 tick = SDL_GetTicks();
	uint32 diff = tick - g_JDLastTick;
	if (diff >= 1000)
	{
	}

	return true;
}

/*************************************************
Function:		AurineJDCoverFlowOnChange
Description: 	设备翻页时，画当前焦点页标
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool AurineJDCoverFlowOnChange(ITUWidget* widget, char* param)
{
	uint8 index = 0;
	dprintf("g_PageCount..............:%d\n", g_PageCount);

	if (g_PageCount <= 1)
	{
		dprintf("g_PageCount num is 1!!!\n");
		return false;
	}

	switch (g_DevType)
	{
		case JD_SCENE_TYPE:
			index = AurineJDSceneCoverFlow->focusIndex;
			ituSpriteGoto(g_AurineJDPageIndexSprite[g_PageIndex], AurineJDPageIndexUnSelectIcon);
			ituSpriteGoto(g_AurineJDPageIndexSprite[index], AurineJDPageIndexSelectIcon);
			g_PageIndex = index;
			break;

		case JD_DEV_LIGHT:
		case JD_DEV_POWER:
		case JD_DEV_GAS:
			index = AurineJDLightCoverFlow->focusIndex;
			ituSpriteGoto(g_AurineJDPageIndexSprite[g_PageIndex], AurineJDPageIndexUnSelectIcon);
			ituSpriteGoto(g_AurineJDPageIndexSprite[index], AurineJDPageIndexSelectIcon);
			g_PageIndex = index;
			break;

		case JD_DEV_KONGTIAO:
			index = AurineJDAirCoverFlow->focusIndex;
			dprintf("index............:%d\n", index);
			ituSpriteGoto(g_AurineJDPageIndexSprite[g_PageIndex], AurineJDPageIndexUnSelectIcon);
			ituSpriteGoto(g_AurineJDPageIndexSprite[index], AurineJDPageIndexSelectIcon);
			g_PageIndex = index;
			break;

		case JD_DEV_WINDOW:
			index = AurineJDCurtainCoverFlow->focusIndex;
			ituSpriteGoto(g_AurineJDPageIndexSprite[g_PageIndex], AurineJDPageIndexUnSelectIcon);
			ituSpriteGoto(g_AurineJDPageIndexSprite[index], AurineJDPageIndexSelectIcon);
			g_PageIndex = index;
			break;

		default:
			break;
	}

	return true;
}

/*************************************************
Function:		AurineJDState
Description: 	监视回调执行函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool AurineJDState(ITUWidget* widget, char* param)
{
	return true;
}

/*************************************************
Function:		AurineJDSceneButtonOnMouseUp
Description: 	情景模式按下执行函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool AurineJDSceneButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 i;
	uint8 btn_event = atoi(param);

	// 判断某个克隆按键按下
	if (btn_event >= JD_SCENE_MAX)
	{
		if (g_SceneList && g_SceneList->nCount >= JD_SCENE_MAX)
		{
			for (i = 0; i < g_SceneList->nCount - JD_SCENE_MAX; i++)
			{
				if (g_AurineJDSceneButton[i] == (ITUButton*)widget)
				{
					btn_event = i + JD_SCENE_MAX;
					break;
				}
			}
		}
	}

	if (g_scene != btn_event)
	{
		g_scene = btn_event;
		if (JD_FACTORY_ACBUS == storage_get_extmode(EXT_MODE_JD_FACTORY))
		{
			jd_aurine_scene_open(g_SceneList->pjd_scene_info[btn_event].Address, g_SceneList->pjd_scene_info[btn_event].Address, btn_event + 1);
		}
	}

	return true;
}

/*************************************************
Function:		AurineJDLightOnChanged
Description: 	灯光按下执行函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool AurineJDLightOnChanged(ITUWidget* widget, char* param)
{
	uint8 value = atoi(param);

	SetDevValue(JD_DEV_LIGHT, g_LightIndex, value);
	ituSpriteGoto(g_AurineJDLightSprite[g_LightIndex], AurineJDReturn0LightIcon + value);

	if (value)
	{
		SetDevState(JD_DEV_LIGHT, g_LightIndex, JD_STATE_OPEN);
		jd_aurine_light_open(g_DevList->pjd_dev_info[g_LightIndex].Index, g_DevList->pjd_dev_info[g_LightIndex].Address, value * 10);
	}
	else
	{
		SetDevState(JD_DEV_LIGHT, g_LightIndex, JD_STATE_CLOSE);
		jd_aurine_light_close(g_DevList->pjd_dev_info[g_LightIndex].Index, g_DevList->pjd_dev_info[g_LightIndex].Address);
	}
	jd_aurine_set_dev_state_param(JD_DEV_LIGHT, g_LightIndex, value * 10);

	return true;
}

/*************************************************
Function:		AurineJDExitLightControlButtonOnMouseUp
Description: 	退出灯光控制执行函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool AurineJDExitLightControlButtonOnMouseUp(ITUWidget* widget, char* param)
{
	SetLightControlEnable(true);

	return true;
}

/*************************************************
Function:		AurineJDLightUnControlButtonOnMouseUp
Description: 	不可调灯光设备按下执行函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool AurineJDLightUnControlButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 change = FALSE;
	uint8 i, value = 0; 
	uint8 btn_event = atoi(param);

	if (g_DevList && g_DevList->nCount)
	{
		for (i = 0; i < g_DevList->nCount; i++)
		{
			if (g_AurineJDLightButton[i] == (ITUButton*)widget)
			{
				// 控制灯的开关状态
				btn_event = i;
				value = GetDevState(JD_DEV_LIGHT, i);
				if (JD_STATE_CLOSE == value)
				{
					change = SetDevState(JD_DEV_LIGHT, i, JD_STATE_OPEN);
					if (TRUE == change)
					{ 
						ituSpriteGoto(g_AurineJDLightSprite[i], AurineJDOpenLightIcon);
					}
					jd_aurine_light_open(g_DevList->pjd_dev_info[i].Index, g_DevList->pjd_dev_info[i].Address, 100);
					jd_aurine_set_dev_state_param(JD_DEV_LIGHT, i, 100);
				}
				else
				{
					change = SetDevState(JD_DEV_LIGHT, i, JD_STATE_CLOSE);
					if (TRUE == change)
					{
						ituSpriteGoto(g_AurineJDLightSprite[i], AurineJDCloseLightIcon);
					}
					jd_aurine_light_close(g_DevList->pjd_dev_info[i].Index, g_DevList->pjd_dev_info[i].Address);
					jd_aurine_set_dev_state_param(JD_DEV_LIGHT, i, 0);
				}
				break;
			}
		}
	}

	return true;
}

/*************************************************
Function:		AurineJDLightControlButtonOnMouseUp
Description: 	可调灯光设备按下执行函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool AurineJDLightControlButtonOnMouseUp(ITUWidget* widget, char* param)
{
	char tmp[100] = { 0 };
	char tmp2[50] = { 0 };
	uint8 i, value = 0, lastvalue = 0;
	uint8 btn_event = atoi(param);

	if (g_DevList && g_DevList->nCount)
	{
		for (i = 0; i < g_DevList->nCount; i++)
		{
			if (g_AurineJDLightButton[i] == (ITUButton*)widget)
			{
				g_LightIndex = i;
				if (JD_STATE_OPEN == GetDevState(JD_DEV_LIGHT, i))
				{
					ituSpriteGoto(g_AurineJDLightSprite[i], AurineJDReturn0LightIcon);
					SetDevState(JD_DEV_LIGHT, i, JD_STATE_CLOSE);
					jd_aurine_light_close(g_DevList->pjd_dev_info[i].Index, g_DevList->pjd_dev_info[i].Address);
				}
				else
				{
					value = GetDevValue(JD_DEV_LIGHT, i);
					lastvalue = GetLightLastValue(i);
					if (value == 0 && lastvalue == 0)
					{
						value = 5;
					}
					else
					{
						value = lastvalue;
					}

					SetLightControlEnable(false);
					ituTrackBarSetValue(AurineJDControlLightTrackBar, value);

					// 设备名称和位置
					GetDevText(i, tmp);
					ituTextSetString(AurineJDControlLightDevText, tmp);
				}
				break;
			}
		}
	}

	return true;
}

/*************************************************
Function:		AurineJDControlAir
Description: 	空调设备具体执行函数
Input:			
	1.index		索引
	2.btn		按键值
Output:			无
Return:			无
Others:			无
*************************************************/
static void AurineJDControlAir(uint8 index, uint8 btn)
{
	uint8 mode = GetDevLastMode(index);
	uint8 value = GetDevValue(JD_DEV_KONGTIAO, index);
	uint8 lastvalue = GetAirLastValue(index);
	uint8 ColdHot = GetDevColdHot(index);

	switch (btn)
	{
		case 0:
			// 显示关状态
			SetDevState(JD_DEV_KONGTIAO, index, JD_STATE_CLOSE);
			ituSpriteGoto(g_AurineJDAirShowModeSprite[index], AurineJDAirShowCloseIcon);
			ituWidgetSetVisible(g_AurineJDAirShowModeText[index], false);
			ituWidgetSetVisible(g_AurineJDAirNumContainer[index], true);
			ituSpriteGoto(g_AurineJDAirIndexNumSprite[index][0], AurineJDAirIndex0OIcon);
			ituSpriteGoto(g_AurineJDAirIndexNumSprite[index][1], AurineJDAirIndex1FIcon);
			ituSpriteGoto(g_AurineJDAirIndexNumSprite[index][2], AurineJDAirIndex2FIcon);

			jd_aurine_send_ir_to_dev(g_DevList->pjd_dev_info[index].Index, g_DevList->pjd_dev_info[index].Address, 21);
			jd_aurine_set_dev_state_param(JD_DEV_KONGTIAO, index, 21);
			break;

		case 1:
			{
				SetDevState(JD_DEV_KONGTIAO, index, JD_STATE_OPEN);
				if (0 == mode)
				{
					// 最初制冷、制暖模式
					ituSpriteGoto(g_AurineJDAirShowModeSprite[index], AurineJDAirShowColdIcon + ColdHot);
					if (value == 0 && lastvalue == 0)
					{
						value = 5;
					}
					else
					{
						value = lastvalue;
					}
					SetDevValue(JD_DEV_KONGTIAO, index, value);
					// 显示温度
					ituWidgetSetVisible(g_AurineJDAirShowModeText[index], false);
					ituWidgetSetVisible(g_AurineJDAirNumContainer[index], true);

					if (value == 10)
					{
						ituSpriteGoto(g_AurineJDAirIndexNumSprite[index][0], AurineJDAirIndex0Num3Icon);
						ituSpriteGoto(g_AurineJDAirIndexNumSprite[index][1], AurineJDAirIndex1Num0Icon);
					}
					else
					{
						ituSpriteGoto(g_AurineJDAirIndexNumSprite[index][0], AurineJDAirIndex0Num2Icon);
						ituSpriteGoto(g_AurineJDAirIndexNumSprite[index][1], AurineJDAirIndex1Num0Icon + value);
					}
					ituSpriteGoto(g_AurineJDAirIndexNumSprite[index][2], AurineJDAirIndex2SXDIcon);

					if (ColdHot)
					{
						value += 10;
					}
					jd_aurine_send_ir_to_dev(g_DevList->pjd_dev_info[index].Index, g_DevList->pjd_dev_info[index].Address, value);
					jd_aurine_set_dev_state_param(JD_DEV_KONGTIAO, index, value);
				}
				else
				{
					// 最后显示模式
					ituSpriteGoto(g_AurineJDAirShowModeSprite[index], AurineJDAirShowColdIcon + mode - 1);
					if (mode > 0 && mode <= 2)
					{
						if (value == 0 && lastvalue == 0)
						{
							value = 5;
						}
						else
						{
							value = lastvalue;
						}
						SetDevValue(JD_DEV_KONGTIAO, index, value);
						// 显示温度
						ituWidgetSetVisible(g_AurineJDAirShowModeText[index], false);
						ituWidgetSetVisible(g_AurineJDAirNumContainer[index], true);
						if (value == 10)
						{
							ituSpriteGoto(g_AurineJDAirIndexNumSprite[index][0], AurineJDAirIndex0Num3Icon);
							ituSpriteGoto(g_AurineJDAirIndexNumSprite[index][1], AurineJDAirIndex1Num0Icon);
						}
						else
						{
							ituSpriteGoto(g_AurineJDAirIndexNumSprite[index][0], AurineJDAirIndex0Num2Icon);
							ituSpriteGoto(g_AurineJDAirIndexNumSprite[index][1], AurineJDAirIndex1Num0Icon + value);
						}
						ituSpriteGoto(g_AurineJDAirIndexNumSprite[index][2], AurineJDAirIndex2SXDIcon);

						if (2 == mode)
						{
							value += 10;
						}
						jd_aurine_send_ir_to_dev(g_DevList->pjd_dev_info[index].Index, g_DevList->pjd_dev_info[index].Address, value);
						jd_aurine_set_dev_state_param(JD_DEV_KONGTIAO, index, value);

					}
					else if (mode > 2 && mode < 7)
					{
						// 显示模式
						uint32 textid[4] = {SID_Jd_KongTiao_Refre, SID_Jd_KongTiao_Warm, SID_Jd_KongTiao_Leisure, SID_Jd_KongTiao_Sleep};

						ituWidgetSetVisible(g_AurineJDAirNumContainer[index], false);
						ituWidgetSetVisible(g_AurineJDAirShowModeText[index], true);
						ituTextSetString(g_AurineJDAirShowModeText[index], get_str(textid[mode - 3]));
						jd_aurine_send_ir_to_dev(g_DevList->pjd_dev_info[index].Index, g_DevList->pjd_dev_info[index].Address, mode - 2 + 21);
					}
				}
			}
			break;

		case 2:
			{
				SetDevState(JD_DEV_KONGTIAO, index, JD_STATE_OPEN);
				SetDevColdHot(index, 0);
				SetDevLastMode(index, 1);

				// 制冷模式
				ituSpriteGoto(g_AurineJDAirShowModeSprite[index], AurineJDAirShowColdIcon);
				if (value == 0 && lastvalue == 0)
				{
					value = 5;
				}
				else
				{
					value = lastvalue;
				}
				SetDevValue(JD_DEV_KONGTIAO, index, value);
				// 显示温度
				ituWidgetSetVisible(g_AurineJDAirShowModeText[index], false);
				ituWidgetSetVisible(g_AurineJDAirNumContainer[index], true);

				if (value == 10)
				{
					ituSpriteGoto(g_AurineJDAirIndexNumSprite[index][0], AurineJDAirIndex0Num3Icon);
					ituSpriteGoto(g_AurineJDAirIndexNumSprite[index][1], AurineJDAirIndex1Num0Icon);
				}
				else
				{
					ituSpriteGoto(g_AurineJDAirIndexNumSprite[index][0], AurineJDAirIndex0Num2Icon);
					ituSpriteGoto(g_AurineJDAirIndexNumSprite[index][1], AurineJDAirIndex1Num0Icon + value);
				}
				ituSpriteGoto(g_AurineJDAirIndexNumSprite[index][2], AurineJDAirIndex2SXDIcon);
				jd_aurine_send_ir_to_dev(g_DevList->pjd_dev_info[index].Index, g_DevList->pjd_dev_info[index].Address, value);
				jd_aurine_set_dev_state_param(JD_DEV_KONGTIAO, index, value);
			}
			break;

		case 3:
			{
				SetDevState(JD_DEV_KONGTIAO, index, JD_STATE_OPEN);
				SetDevColdHot(index, 1);
				SetDevLastMode(index, 2);
				// 制冷模式
				ituSpriteGoto(g_AurineJDAirShowModeSprite[index], AurineJDAirShowHeatIcon);
				if (value == 0 && lastvalue == 0)
				{
					value = 5;
				}
				else
				{
					value = lastvalue;
				}
				SetDevValue(JD_DEV_KONGTIAO, index, value);
				// 显示温度
				ituWidgetSetVisible(g_AurineJDAirShowModeText[index], false);
				ituWidgetSetVisible(g_AurineJDAirNumContainer[index], true);

				if (value == 10)
				{
					ituSpriteGoto(g_AurineJDAirIndexNumSprite[index][0], AurineJDAirIndex0Num3Icon);
					ituSpriteGoto(g_AurineJDAirIndexNumSprite[index][1], AurineJDAirIndex1Num0Icon);
				}
				else
				{
					ituSpriteGoto(g_AurineJDAirIndexNumSprite[index][0], AurineJDAirIndex0Num2Icon);
					ituSpriteGoto(g_AurineJDAirIndexNumSprite[index][1], AurineJDAirIndex1Num0Icon + value);
				}
				ituSpriteGoto(g_AurineJDAirIndexNumSprite[index][2], AurineJDAirIndex2SXDIcon);

				value += 10;
				jd_aurine_send_ir_to_dev(g_DevList->pjd_dev_info[index].Index, g_DevList->pjd_dev_info[index].Address, value);
				jd_aurine_set_dev_state_param(JD_DEV_KONGTIAO, index, value);
			}
			break;

		case 4:
			{
				if (value > MIN_KONGTIAO_VAULE)
				{
					value--;
				}
				else
				{
					value = MIN_KONGTIAO_VAULE;
				}
				SetDevState(JD_DEV_KONGTIAO, index, JD_STATE_OPEN);
				SetDevValue(JD_DEV_KONGTIAO, index, value);
				SetDevLastMode(index, ColdHot+1);
				// 制冷、制暖模式
				ituSpriteGoto(g_AurineJDAirShowModeSprite[index], AurineJDAirShowColdIcon + ColdHot);
				// 显示温度
				ituWidgetSetVisible(g_AurineJDAirShowModeText[index], false);
				ituWidgetSetVisible(g_AurineJDAirNumContainer[index], true);

				if (value == 10)
				{
					ituSpriteGoto(g_AurineJDAirIndexNumSprite[index][0], AurineJDAirIndex0Num3Icon);
					ituSpriteGoto(g_AurineJDAirIndexNumSprite[index][1], AurineJDAirIndex1Num0Icon);
				}
				else
				{
					ituSpriteGoto(g_AurineJDAirIndexNumSprite[index][0], AurineJDAirIndex0Num2Icon);
					ituSpriteGoto(g_AurineJDAirIndexNumSprite[index][1], AurineJDAirIndex1Num0Icon + value);
				}
				ituSpriteGoto(g_AurineJDAirIndexNumSprite[index][2], AurineJDAirIndex2SXDIcon);

				if (ColdHot)
				{
					value += 10;
				}
				jd_aurine_send_ir_to_dev(g_DevList->pjd_dev_info[index].Index, g_DevList->pjd_dev_info[index].Address, value);
				jd_aurine_set_dev_state_param(JD_DEV_KONGTIAO, index, value);
			}
			break;

		case 5:
			{
				if (value < MAX_KONGTIAO_VAULE)
				{
					value++;
				}
				else
				{
					value = MAX_KONGTIAO_VAULE;
				}
				SetDevState(JD_DEV_KONGTIAO, index, JD_STATE_OPEN);
				SetDevValue(JD_DEV_KONGTIAO, index, value);
				SetDevLastMode(index, ColdHot + 1);
				// 制冷、制暖模式
				ituSpriteGoto(g_AurineJDAirShowModeSprite[index], AurineJDAirShowColdIcon + ColdHot);
				// 显示温度
				ituWidgetSetVisible(g_AurineJDAirShowModeText[index], false);
				ituWidgetSetVisible(g_AurineJDAirNumContainer[index], true);

				if (value == 10)
				{
					ituSpriteGoto(g_AurineJDAirIndexNumSprite[index][0], AurineJDAirIndex0Num3Icon);
					ituSpriteGoto(g_AurineJDAirIndexNumSprite[index][1], AurineJDAirIndex1Num0Icon);
				}
				else
				{
					ituSpriteGoto(g_AurineJDAirIndexNumSprite[index][0], AurineJDAirIndex0Num2Icon);
					ituSpriteGoto(g_AurineJDAirIndexNumSprite[index][1], AurineJDAirIndex1Num0Icon + value);
				}
				ituSpriteGoto(g_AurineJDAirIndexNumSprite[index][2], AurineJDAirIndex2SXDIcon);

				if (ColdHot)
				{
					value += 10;
				}
				jd_aurine_send_ir_to_dev(g_DevList->pjd_dev_info[index].Index, g_DevList->pjd_dev_info[index].Address, value);
				jd_aurine_set_dev_state_param(JD_DEV_KONGTIAO, index, value);
			}
			break;

		case 6:
		case 7:
		case 8:
		case 9:
			{	
				SetDevState(JD_DEV_KONGTIAO, index, JD_STATE_OPEN);
				SetDevLastMode(index, btn - 3);
				SetDevMode(index, btn - 5);
				// 显示模式
				uint32 textid[4] = {SID_Jd_KongTiao_Refre, SID_Jd_KongTiao_Warm, SID_Jd_KongTiao_Leisure, SID_Jd_KongTiao_Sleep};

				ituSpriteGoto(g_AurineJDAirShowModeSprite[index], AurineJDAirShowRefreshIcon + btn - 6);
				ituWidgetSetVisible(g_AurineJDAirNumContainer[index], false);
				ituWidgetSetVisible(g_AurineJDAirShowModeText[index], true);
				ituTextSetString(g_AurineJDAirShowModeText[index], get_str(textid[btn - 6]));
				jd_aurine_send_ir_to_dev(g_DevList->pjd_dev_info[index].Index, g_DevList->pjd_dev_info[index].Address, btn - 5 + 21);
			}
			break;
	}
}

/*************************************************
Function:		AurineJDAirButtonOnMouseUp
Description: 	空调设备按下执行函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool AurineJDAirButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 i, j, index = 0;
	uint8 btn_event = atoi(param);

	if (g_DevList && g_DevList->nCount)
	{
		for (i = 0; i < g_DevList->nCount; i++)
		{
			for (j = 0; j < 10; j++)
			{
				if (g_AurineJDAirButton[i][j] == (ITUButton*)widget)
				{
					index = i;
					AurineJDControlAir(index, btn_event);
					break;
				}
			}
		}
	}

	return true;
}

/*************************************************
Function:		AurineJDCurtainButtonOnMouseUp
Description: 	窗帘设备按下执行函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool AurineJDCurtainButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 i, j;
	uint8 btn_event = atoi(param);

	if (g_DevList && g_DevList->nCount)
	{
		for (i = 0; i < g_DevList->nCount; i++)
		{
			for (j = 0; j < 3; j++)
			{
				if (g_AurineJDCurtainButton[i][j] == (ITUButton*)widget)
				{
					switch (btn_event)
					{
						case 0:
							SetDevState(JD_DEV_WINDOW, i, JD_STATE_CLOSE);
							ituSpriteGoto(g_AurineJDCurtainSateSprite[i], AurineJDCurtainCloseIcon);
							jd_aurine_window_close(g_DevList->pjd_dev_info[i].Index, g_DevList->pjd_dev_info[i].Address);
							jd_aurine_set_dev_state_param(JD_DEV_WINDOW, i, JD_STATE_CLOSE);
							break;

						case 1:
							jd_aurine_window_stop(g_DevList->pjd_dev_info[i].Index, g_DevList->pjd_dev_info[i].Address);
							break;

						case 2:
							SetDevState(JD_DEV_WINDOW, i, JD_STATE_OPEN);
							ituSpriteGoto(g_AurineJDCurtainSateSprite[i], AurineJDCurtainOpenIcon);
							jd_aurine_window_open(g_DevList->pjd_dev_info[i].Index, g_DevList->pjd_dev_info[i].Address);
							jd_aurine_set_dev_state_param(JD_DEV_WINDOW, i, JD_STATE_OPEN);
							break;
					}
					break;
				}
			}
		}
	}

	return true;
}

/*************************************************
Function:		AurineJDPowerButtonOnMouseUp
Description: 	电源设备按下执行函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool AurineJDPowerButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 i, change, state = 0;
	uint8 btn_event = atoi(param);

	if (g_DevList && g_DevList->nCount)
	{
		for (i = 0; i < g_DevList->nCount; i++)
		{
			if (g_AurineJDPowerButton[i] == (ITUButton*)widget)
			{
				// 控制灯的开关状态
				state = GetDevState(JD_DEV_POWER, i);
				if (JD_STATE_CLOSE == state)
				{
					change = SetDevState(JD_DEV_POWER, i, JD_STATE_OPEN);
					if (TRUE == change)
					{
						ituSpriteGoto(g_AurineJDPowerSprite[i], AurineJDDevOpenIcon);
					}
					jd_aurine_power_open(g_DevList->pjd_dev_info[i].Index, g_DevList->pjd_dev_info[i].Address, 100);
					jd_aurine_set_dev_state_param(JD_DEV_POWER, i, 0xff);
				}
				else
				{
					change = SetDevState(JD_DEV_POWER, i, JD_STATE_CLOSE);
					if (TRUE == change)
					{
						ituSpriteGoto(g_AurineJDPowerSprite[i], AurineJDDevCloseIcon);
					}
					jd_aurine_power_close(g_DevList->pjd_dev_info[i].Index, g_DevList->pjd_dev_info[i].Address);
					jd_aurine_set_dev_state_param(JD_DEV_POWER, i, 0);
				}
				break;
			}
		}
	}

	return true;
}

/*************************************************
Function:		AurineJDGasButtonOnMouseUp
Description: 	煤气设备按下执行函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool AurineJDGasButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 i, change, state = 0;
	uint8 btn_event = atoi(param);

	if (g_DevList && g_DevList->nCount)
	{
		for (i = 0; i < g_DevList->nCount; i++)
		{
			if (g_AurineJDGasButton[i] == (ITUButton*)widget)
			{
				// 控制灯的开关状态
				state = GetDevState(JD_DEV_GAS, i);
				if (JD_STATE_CLOSE == state)
				{
					change = SetDevState(JD_DEV_GAS, i, JD_STATE_OPEN);
					if (TRUE == change)
					{
						ituSpriteGoto(g_AurineJDGasSprite[i], AurineJDDevOpenIcon);
					}
					jd_aurine_gas_open(g_DevList->pjd_dev_info[i].Index, g_DevList->pjd_dev_info[i].Address, 100);
					jd_aurine_set_dev_state_param(JD_DEV_GAS, i, 0xff);
				}
				else
				{
					change = SetDevState(JD_DEV_GAS, i, JD_STATE_CLOSE);
					if (TRUE == change)
					{
						ituSpriteGoto(g_AurineJDGasSprite[i], AurineJDDevCloseIcon);
					}
					jd_aurine_gas_close(g_DevList->pjd_dev_info[i].Index, g_DevList->pjd_dev_info[i].Address);
					jd_aurine_set_dev_state_param(JD_DEV_GAS, i, 0);
				}
				break;
			}
		}
	}

	return true;
}

/*************************************************
Function:		AurineJDLayerButtonOnMouseUp
Description: 	右边按键弹起函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool AurineJDLayerButtonOnMouseUp(ITUWidget* widget, char* param)
{
	uint8 btn_event = atoi(param);

	dprintf("btn_event: %d\n", btn_event);
	if (btn_event >= AurineJDSceneEvent && btn_event <= AurineJDGasEvent)
	{
		if (g_BtnEvent)
		{
			if (g_BtnEvent != btn_event)
			{
				ituSpriteGoto(AurineJDBottomSprite[g_BtnEvent-2], AurineJDButtomNoPressIcon);
			}
		}
		g_BtnEvent = btn_event;
		ituSpriteGoto(AurineJDBottomSprite[g_BtnEvent-2], AurineJDButtomPressIcon);
	}
	switch (btn_event)
	{
		case AurineJDHomeEvent:
			 break;

		case AurineJDExitEvent:
			ituLayerGoto(MainLayer);
			break;

		case AurineJDSceneEvent:
			if (JD_SCENE_TYPE != g_DevType)
			{
				ShowPageType(JD_SCENE_TYPE);
				ShowSceneType();
			}
			break;

		case AurineJDLightEvent:
			if (JD_DEV_LIGHT != g_DevType)
			{
				ShowPageType(JD_DEV_LIGHT);
				ShowDevType(JD_DEV_LIGHT);
			}
			break;

		case AurineJDAirEvent:
			if (JD_DEV_KONGTIAO != g_DevType)
			{
				ShowPageType(JD_DEV_KONGTIAO);
				ShowDevType(JD_DEV_KONGTIAO);
			}
			break;

		case AurineJDCurtainEvent:
			if (JD_DEV_WINDOW != g_DevType)
			{
				ShowPageType(JD_DEV_WINDOW);
				ShowDevType(JD_DEV_WINDOW);
			}
			break;

		case AurineJDPowerEvent:
			if (JD_DEV_POWER != g_DevType)
			{
				ShowPageType(JD_DEV_POWER);
				ShowDevType(JD_DEV_POWER);
			}
			break;

		case AurineJDGasEvent:
			if (JD_DEV_GAS != g_DevType)
			{
				ShowPageType(JD_DEV_GAS);
				ShowDevType(JD_DEV_GAS);
			}
			break;

		default:
			break;
	}
	return true;
}

/*************************************************
Function:		AurineJDLayerOnEnter
Description:	家电初始化处理函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
bool AurineJDLayerOnEnter(ITUWidget* widget, char* param)
{
	uint8 i;

	g_BtnEvent = AurineJDSceneEvent;
	SetLightControlEnable(true);
	ituRadioBoxSetChecked(AurineJDRightHomeRadioBox, true);
	ituCoverFlowGoto(AurineJDBottomCoverFlow, 0);
	ituSpriteGoto(AurineJDBottomSprite[g_BtnEvent - 2], AurineJDButtomPressIcon);

	for (i = 0; i < 3; i++)
	{
		ituWidgetDisable(AurineJDRightNullButton[i]);
	}
	ShowPageType(JD_SCENE_TYPE);
	ShowSceneType();

	return true;
}

/*************************************************
Function:		InitAurineJDLayer
Description: 	家电页面初始化
Input:			无
Output:			无
Return:			无
Others:			无
*************************************************/
static void InitAurineJDLayer(void)
{
	if (!AurineJDLayer)
	{
		uint8 i;
		char callname[50];

		MainLayer = ituSceneFindWidget(&theScene, "mainLayer");
		assert(MainLayer);

		AurineJDLayer = ituSceneFindWidget(&theScene, "AurineJDLayer");
		assert(AurineJDLayer);

		AurineJDRightHomeRadioBox = ituSceneFindWidget(&theScene, "AurineJDRightHomeRadioBox");
		assert(AurineJDRightHomeRadioBox);

		AurineJDRightBackground = ituSceneFindWidget(&theScene, "AurineJDRightBackground");
		assert(AurineJDRightBackground);

		AurineJDExitContainer = ituSceneFindWidget(&theScene, "AurineJDExitContainer");
		assert(AurineJDExitContainer);

		AurineJDBottomCoverFlow = ituSceneFindWidget(&theScene, "AurineJDBottomCoverFlow");
		assert(AurineJDBottomCoverFlow);

		AurineJDSceneBackground = ituSceneFindWidget(&theScene, "AurineJDSceneBackground");
		assert(AurineJDSceneBackground);

		AurineJDSceneCoverFlow = ituSceneFindWidget(&theScene, "AurineJDSceneCoverFlow");
		assert(AurineJDSceneCoverFlow);

		AurineJDLightCoverFlow = ituSceneFindWidget(&theScene, "AurineJDLightCoverFlow");
		assert(AurineJDLightCoverFlow);

		AurineJDLightBackground = ituSceneFindWidget(&theScene, "AurineJDLightBackground");
		assert(AurineJDLightBackground);

		AurineJDControlLightGrayBackground = ituSceneFindWidget(&theScene, "AurineJDControlLightGrayBackground");
		assert(AurineJDControlLightGrayBackground);

		AurineJDControlLightTrackBar = ituSceneFindWidget(&theScene, "AurineJDControlLightTrackBar");
		assert(AurineJDControlLightTrackBar);

		AurineJDControlLightDevText = ituSceneFindWidget(&theScene, "AurineJDControlLightDevText");
		assert(AurineJDControlLightDevText);

		AurineJDAirCoverFlow = ituSceneFindWidget(&theScene, "AurineJDAirCoverFlow");
		assert(AurineJDAirCoverFlow);

		AurineJDAirBackground = ituSceneFindWidget(&theScene, "AurineJDAirBackground");
		assert(AurineJDAirBackground);

		AurineJDCurtainCoverFlow = ituSceneFindWidget(&theScene, "AurineJDCurtainCoverFlow");
		assert(AurineJDCurtainCoverFlow);

		AurineJDCurtainBackground = ituSceneFindWidget(&theScene, "AurineJDCurtainBackground");
		assert(AurineJDCurtainBackground);

		AurineJDPowerBackground = ituSceneFindWidget(&theScene, "AurineJDPowerBackground");
		assert(AurineJDPowerBackground);

		AurineJDGasBackground = ituSceneFindWidget(&theScene, "AurineJDGasBackground");
		assert(AurineJDGasBackground);

		AurineJDPageIndexSprite = ituSceneFindWidget(&theScene, "AurineJDPageIndexSprite");
		assert(AurineJDPageIndexSprite);

		AurineJDBackground = ituSceneFindWidget(&theScene, "AurineJDBackground");
		assert(AurineJDBackground);

		for (i = 0; i < 3; i++)
		{
			memset(callname, 0, sizeof(callname));
			sprintf(callname, "%s%d", "AurineJDRightNullButton", i);
			AurineJDRightNullButton[i] = ituSceneFindWidget(&theScene, callname);
			assert(AurineJDRightNullButton[i]);
		}

		for (i = 0; i < JD_ALL; i++)
		{
			memset(callname, 0, sizeof(callname));
			sprintf(callname, "%s%d", "AurineJDBottomSprite", i);
			AurineJDBottomSprite[i] = ituSceneFindWidget(&theScene, callname);
			assert(AurineJDBottomSprite[i]);
		}

		for (i = 0; i < JD_SCENE_PAGE_NUM; i++)
		{
			memset(callname, 0, sizeof(callname));
			sprintf(callname, "%s%d", "AurineJDSceneBackgroundPage", i);
			AurineJDSceneBackgroundPage[i] = ituSceneFindWidget(&theScene, callname);
			assert(AurineJDSceneBackgroundPage[i]);
		}

		for (i = 0; i < JD_LIGHT_PAGE_NUM; i++)
		{
			memset(callname, 0, sizeof(callname));
			sprintf(callname, "%s%d", "AurineJDLightBackgroundPage", i);
			AurineJDLightBackgroundPage[i] = ituSceneFindWidget(&theScene, callname);
			assert(AurineJDLightBackgroundPage[i]);
		}

		for (i = 0; i < JD_AIR_PAGE_NUM; i++)
		{
			memset(callname, 0, sizeof(callname));
			sprintf(callname, "%s%d", "AurineJDAirBackgroundPage", i);
			AurineJDAirBackgroundPage[i] = ituSceneFindWidget(&theScene, callname);
			assert(AurineJDAirBackgroundPage[i]);
		}

		for (i = 0; i < JD_CURTAIN_PAGE_NUM; i++)
		{
			memset(callname, 0, sizeof(callname));
			sprintf(callname, "%s%d", "AurineJDCurtainBackgroundPage", i);
			AurineJDCurtainBackgroundPage[i] = ituSceneFindWidget(&theScene, callname);
			assert(AurineJDCurtainBackgroundPage[i]);
		}
	}
}

/*************************************************
Function:		AurineJDWin
Description: 	家电界面
Input:
1.DevType		设备类型
2.index			索引
utput:			无
Return:			无
Others:			无
*************************************************/
void AurineJDWin(void)
{
	g_LightIndex = 0;
	g_scene = MAX_JD_SCENE_NUM + 1;
	//jd_state_report_init((PAuJiaDianStateChanged)auJdStateChanged);
	InitAurineJDLayer();
	ituLayerGoto(AurineJDLayer);
}

/*************************************************
Function:		AurineJDReset
Description: 	家电界面初始化函数
Input:			无
Output:			无
Return:			TRUE 是 FALSE 否
Others:			无
*************************************************/
void AurineJDReset(void)
{
	AurineJDLayer = NULL;
}

