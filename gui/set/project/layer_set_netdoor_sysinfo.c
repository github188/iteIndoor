/*************************************************
Copyright (C), 2006-2016, Aurine
File name:  	layer_set_netdoor_sysinfo.c
Author:     	zxc
Version:    	
Date: 		2016-07-28
Description: 网络门前机系统信息
History:
1. Date:
Author:
Modification:
*************************************************/
#include "../layer_set.h"

static ITUText* SetNetDoorSysInfo2Text[14] = { NULL };
static ITUCoverFlow* SetNetDoorSysInfoCoverFlow = NULL;

static FULL_DEVICE_NO g_devno;
static FWM_VERSION_T  g_kerver;
static NET_PARAM g_param;
static char g_buildtime[100];
static char g_hwver[100];
static char g_prgver[100];

/*************************************************
Function:		get_version_str
Description: 	获取版本信息
Input:
1.			端口信息
Output:		无
Return:		端口名称
Others:
*************************************************/
static void get_version_str(uint32 ver, char *sver)
{
	if (sver == NULL)
		return;
	sprintf(sver, "V%d.%d.%d.%d", ver >> 24, (ver >> 16) & 0xff, (ver >> 8) & 0xff, ver & 0xff);
}

/*************************************************
Function:		show_updata
Description: 	
Input:
1.hDlg
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
static void show_updata()
{
	int i;
	char value[100];
	int8 NetDevIndex = -1;
	char devno[30] = { 0 };
	int32 DevLen = 0;
	uint8 sMac[30] = { 0 };


	for (i = 0; i < 14; i++)
	{
		memset(value, 0, sizeof(value));

		switch (i)
		{
		case 0:
			sprintf(value, "%s", g_devno.DeviceNoStr);
			break;

		case 1:
		{
			sprintf(devno, "%s", g_devno.DeviceNoStr);
			DevLen = strlen(devno);

			if (DevLen != 0)
			{
				if (devno[DevLen - 1] == '0')
				{
					get_dev_description(DEVICE_TYPE_GATEWAY, devno, value, 30);
				}
				else
				{
					if (devno[DevLen - 1] == '8')
					{
						devno[DevLen - 1] = '1';
					}
					else if (devno[DevLen - 1] == '9')
					{
						devno[DevLen - 1] = '2';
					}
					get_dev_description(DEVICE_TYPE_DOOR_NET, devno, value, 30);
				}
			}
			break;
		}

		case 2:
			sprintf(value, "%s", UlongtoIP(g_param.IP));
			break;

		case 3:
		{
				  char devno[30] = { 0 };
				  int32 DevLen = 0;
				  sprintf(devno, "%s", g_devno.DeviceNoStr);
				  DevLen = strlen(devno);

				  if (DevLen == 0)
				  {
					  NetDevIndex = -1;
				  }
				  else
				  {
					  if (devno[DevLen - 1] == '0')
					  {
						  NetDevIndex = 0;
					  }
					  else if (devno[DevLen - 1] == '8')
					  {
						  NetDevIndex = 1;
					  }
					  else if (devno[DevLen - 1] == '9')
					  {
						  NetDevIndex = 2;
					  }
					  else
					  {
						  NetDevIndex = -1;
					  }
				  }

				  if (NetDevIndex != -1)
				  {
					  memcpy(sMac, storage_get_mac(NetDevIndex), 6);
				  }
				  sprintf(value, "%02X-%02X-%02X-%02X-%02X-%02X", sMac[0], sMac[1], sMac[2], sMac[3], sMac[4], sMac[5]);
				  break;
		}

		case 4:
			sprintf(value, "%s", UlongtoIP(g_param.DefaultGateway));
			break;

		case 5:
			sprintf(value, "%s", UlongtoIP(g_param.ManagerIP[0]));
			break;

		case 6:
			sprintf(value, "%s", UlongtoIP(g_param.CenterIP));
			break;

		case 7:
			sprintf(value, "%s", g_hwver);
			break;

		case 8:
			sprintf(value, "%s-%s", g_prgver, g_buildtime);
			break;

		case 9:
			get_version_str(g_kerver.u32ChipVer, value);
			break;

		case 10:
			get_version_str(g_kerver.u32MspVer, value);
			break;

		case 11:
			get_version_str(g_kerver.u32RootBoxVer, value);
			break;

		case 12:
			get_version_str(g_kerver.u32LinuxVer, value);
			break;

		case 13:
			get_version_str(g_kerver.u32ProductVer, value);
			sprintf(value, "%s  %s ", value, g_buildtime);
			break;
		}

		ituTextSetString(SetNetDoorSysInfo2Text[i], value);
	}
}

/*************************************************
Function:			OnNetDoorDeal
Description: 		处理网络门前机
Input:
1.win:   		当前处理的窗口
2.wParam:
Output:
Return:
Others:
*************************************************/
static void OnNetDoorDeal(uint32 wParam)
{
	uint32 ret = 0;
	char buildtime[100];

	switch (wParam)
	{
	case CMD_GATEWAY_GET_DEV_INFO:
		ret = logic_get_netdevice_netparam(&g_param);
		ret |= logic_get_netdevice_devnoinfo(&g_devno, NULL);
		ret = logic_get_netdoor_version(&g_kerver, buildtime, g_prgver);
		if (NETDOOR_ECHO_SUCESS == ret)
		{
			memset(g_buildtime, 0, sizeof(g_buildtime));
			memset(g_hwver, 0, sizeof(g_hwver));
			memcpy(g_buildtime, buildtime, 40);
			memcpy(g_hwver, buildtime + 40, 40);
			//show_updata();
		}
		else
		{
			MsgLinkOutTimeOnShow();			//显示连接超时
		}
		break;
	}
}

/*************************************************
Function:		SetNetDoorOnEnter
Description: 	设置网络门前机界面初始化函数
Input:		无
Output:		无
Return:		TRUE 是 FALSE 否
Others:
*************************************************/
bool SetNetDoorSysInfoLayerOnEnter(ITUWidget* widget, char* param)
{
	if (!SetNetDoorSysInfoCoverFlow)
	{
		uint8 i;
		char tmp[50];

		SetNetDoorSysInfoCoverFlow = ituSceneFindWidget(&theScene, "SetNetDoorSysInfoCoverFlow");
		assert(SetNetDoorSysInfoCoverFlow);

		for (i = 0; i < 14; i++)
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s%d", "SetNetDoorSysInfo2Text", i);
			SetNetDoorSysInfo2Text[i] = ituSceneFindWidget(&theScene, tmp);
			assert(SetNetDoorSysInfo2Text[i]);
		}
	}

	memset(&g_param, 0, sizeof(NET_PARAM));
	memset(&g_devno, 0, sizeof(FULL_DEVICE_NO));
	memset(&g_kerver, 0, sizeof(FWM_VERSION_T));
	memset(g_buildtime, 0, sizeof(g_buildtime));
	memset(g_prgver, 0, sizeof(g_prgver));

	ituCoverFlowGoto(SetNetDoorSysInfoCoverFlow, 0);
	OnNetDoorDeal(CMD_GATEWAY_GET_DEV_INFO);
	show_updata();

	return true;
}
