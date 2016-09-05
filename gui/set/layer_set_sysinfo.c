/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_sysinfo.c
Author:     	zxc
Version:
Date: 		2016-07-28
Description: 系统信息
History:
1. Date:
Author:
Modification:
*************************************************/
#include "layer_set.h"

typedef enum
{
	SYS_INFO_DEV = 0x00,
	SYS_INFO_ROOM,
	SYS_INFO_HOSTIP,
	SYS_INFO_MASK,
	SYS_INFO_GATEWAY,
	SYS_INFO_MAC,
	SYS_INFO_CENTERIP,
	SYS_INFO_MANAGE1,
	SYS_INFO_MANAGE2,
	SYS_INFO_MANAGE3,
#ifdef _USE_ELEVATOR_
	SYS_INFO_DIANTI,
#endif
#ifdef _IP_MODULE_
	SYS_INFO_IP_MODILE,
#endif
	SYS_INFO_HW,
	SYS_INFO_SF,
#ifndef _USE_NEW_CENTER_  
	SYS_INFO_SDK,
#endif
	SYS_INFO_MAX,
}SYS_INFO_TYPE;

static ITUText* SetSysInfo2Text[14] = { NULL };
static ITUCoverFlow* SetSysInfoCoverFlow = NULL;
//static ITUCalendar* SetSysInfoContainer[14] = { NULL };

#if 0
/*************************************************
Function:		init_Container_num
Description: 	隐藏多余的Container
Input:
1.hDlg
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void init_Container_num()
{
	uint8 i;
	char tmp[50] = { 0 };

	if (14 > SYS_INFO_MAX)
	{
		for (i = 0; i < 14; i++)
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "SetSysInfoContainer", i);
			SetSysInfoContainer[i] = ituSceneFindWidget(&theScene, tmp);
			assert(SetSysInfoContainer[i]);
		}

		for (i = SYS_INFO_MAX; i < 14; i++)
		{
			ituWidgetSetVisible(SetSysInfoContainer[i], false);
		}	
	}
}
#endif

/*************************************************
Function:		init_data
Description: 	初始化数据
Input:
1.hDlg
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void init_data()
{
	uint8 i;
	uint16 areano;
	char *tmp;
	char tmp1[6];
	char tmp2[20];

	for (i = 0; i < SYS_INFO_MAX; i++)
	{
		tmp = malloc(100);
		if (!tmp)
		{
			return FALSE;
		}
		memset(tmp, 0, sizeof(tmp));
		switch (i)
		{
		case SYS_INFO_DEV:
			areano = storage_get_areano();
			sprintf(tmp, "%d-%s", areano, storage_get_devno_str());
			break;

		case SYS_INFO_ROOM:
			get_dev_description(DEVICE_TYPE_ROOM, storage_get_devno_str(), tmp, sizeof(tmp));
			break;

		case SYS_INFO_HOSTIP:
			sprintf(tmp, "%s", UlongtoIP(storage_get_netparam_bytype(HOST_IPADDR)));
			break;

		case SYS_INFO_MASK:
			sprintf(tmp, "%s", UlongtoIP(storage_get_netparam_bytype(HOST_NETMASK)));
			break;

		case SYS_INFO_GATEWAY:
			sprintf(tmp, "%s", UlongtoIP(storage_get_netparam_bytype(HOST_GATEWAY)));
			break;

		case SYS_INFO_MAC:
			memcpy(tmp1, storage_get_mac(HOUSE_MAC), sizeof(tmp1));
			sprintf(tmp, "%02x-%02x-%02x-%02x-%02x-%02x", tmp1[0], tmp1[1], tmp1[2], tmp1[3], tmp1[4], tmp1[5]);
			break;

		case SYS_INFO_CENTERIP:
			sprintf(tmp, "%s", UlongtoIP(storage_get_netparam_bytype(CENTER_IPADDR)));
#ifdef _USE_NEW_CENTER_
			uint32 ret = logic_reg_center_state();
			memset(tmp2, 0, sizeof(tmp2));
			if (ret == 2)
			{
				sprintf(tmp2, "  ( %s )", get_str(SID_Registered));
				strcat(tmp, tmp2);
			}
			else
			{
				sprintf(tmp2, "  ( %s )", get_str(SID_Unregistered));
				strcat(tmp, tmp2);
			}
#endif
			break;

		case SYS_INFO_MANAGE1:
			sprintf(tmp, "%s", UlongtoIP(storage_get_netparam_bytype(MANAGER1_IPADDR)));
			break;

		case SYS_INFO_MANAGE2:
			sprintf(tmp, "%s", UlongtoIP(storage_get_netparam_bytype(MANAGER2_IPADDR)));
			break;

		case SYS_INFO_MANAGE3:
			sprintf(tmp, "%s", UlongtoIP(storage_get_netparam_bytype(MANAGER3_IPADDR)));
			break;

#ifdef _USE_ELEVATOR_
		case SYS_INFO_DIANTI:
			sprintf(tmp, "%s", UlongtoIP(storage_get_netparam_bytype(DIANTI_IPADDR)));
			break;
#endif

		case SYS_INFO_HW:
			sprintf(tmp, "%s", HARD_VER);
			break;

		case SYS_INFO_SF:
			sprintf(tmp, "%s", SOFT_VER);
			break;

#ifndef _USE_NEW_CENTER_
		case SYS_INFO_SDK:
			sprintf(tmp, "%s", SDK_VER);
			break;
#endif

#ifdef _IP_MODULE_
		case SYS_INFO_IP_MODILE:
			sprintf(tmp, "%s", UlongtoIP(get_ipmodule_addr()));
			break;
#endif
		}
		ituTextSetString(SetSysInfo2Text[i], tmp);
	}
	//init_Container_num();

	free(tmp);
	tmp = NULL;
}


/*************************************************
Function:		SetSystemLanguageOnEnter
Description: 	进入系统信息界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetSysInfoOnEnter(ITUWidget* widget, char* param)
{
	if (!SetSysInfoCoverFlow)
	{
		uint8 i;
		char tmp[50];

		SetSysInfoCoverFlow = ituSceneFindWidget(&theScene, "SetSysInfoCoverFlow");
		assert(SetSysInfoCoverFlow);

		for (i = 0; i < 14; i++)
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "SetSysInfo2Text", i);
			SetSysInfo2Text[i] = ituSceneFindWidget(&theScene, tmp);
			assert(SetSysInfo2Text[i]);
		}
	}

	ituCoverFlowGoto(SetSysInfoCoverFlow, 0);

	init_data();

	return true;
}
