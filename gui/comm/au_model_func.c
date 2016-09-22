/*************************************************
  Copyright (C), 2006-2016, Aurine
  File name:  	au_model_func.c
  Author:     	luofl
  Version:    	2.0
  Date: 		2014-09-01
  Description:  
				GUI的公共处理程序
  History:        
    1. Date:	
       Author:	
       Modification:
*************************************************/
#include "gui_include.h"

/*************************************************
  Function:			ui_show_win_arbitration
  Description: 		显示页面时仲裁
  Input: 		
  	1.hDlg			当前页面
  	2.OperType		操作类型
  Output:			无
  Return:			TRUE / FALSE
  Others:			
*************************************************/
int32 ui_show_win_arbitration(SYS_ASYN_OPER_TYPE OperType)
{
	uint8 flg = FALSE;
	SYS_MEDIA_TYPE CurState;
	CurState = sys_get_media_state();
	
	ScreenSaverRefresh();
	
	dprintf("ui_show_win_arbitration : CurState : %d\n", CurState);
	switch (OperType)
	{
		case SYS_OPER_ALARMING:
			flg = TRUE;
			break;
			
		case SYS_OPER_ALARM_SOS:
			switch (CurState)
			{
				case SYS_MEDIA_INTERCOM:
				case SYS_MEDIA_MONITOR:
				case SYS_MEDIA_FAMILY_RECORD:
					flg = FALSE;
					break;
				default:
					flg = TRUE;
					break;
			}
			break;
			
		case SYS_OPER_CALLOUT:
			switch (CurState)
			{
				// 在感应按键呼叫、界面呼叫、回拨号码时先有页面上的仲裁
				case SYS_MEDIA_ALARM:
				case SYS_MEDIA_INTERCOM:
				case SYS_MEDIA_MONITOR:
					flg = FALSE;
					break;
				default:
					flg = TRUE;
					break;
			}
			break;
	
		case SYS_OPER_CALLIN:
			// 仲裁已经在被叫进入时完成，此时只完成开屏等操作
			return TRUE;
			
		case SYS_OPER_MONITOR:
			switch (CurState)
			{
				case SYS_MEDIA_ALARM:
				case SYS_MEDIA_INTERCOM:
				case SYS_MEDIA_MONITOR:
					flg = FALSE;
					break;
				default:
					flg = TRUE;
					break;
			}
			break;
		default:
			return FALSE;
	}

	if (flg == TRUE)
	{
		if (SYS_MEDIA_ALARM == CurState && (SYS_OPER_ALARMING == OperType || SYS_OPER_ALARM_SOS == OperType))
		{
			return TRUE;
		}
		
		if ((SYS_OPER_ALARM_SOS == OperType) && (SYS_MEDIA_YUJING == CurState))	// add by caogw 2015-09-24 处于报警界面且正在预警时，再按报警快捷键时不关预警声
		{
			return TRUE;
		}
		//sys_stop_cur_media();
		return TRUE;
	}
	else
	{	
		// 显示提示
		//show_msg(CurWin, WID_MsgNotice, MSG_ERROR, SID_Inter_CaptureFailed);
		return FALSE;
	}
}

/*************************************************
  Function:			get_houseno_desc
  Description: 		获得房号描述
  Input: 			
  	1.numtext		设备编号
  	2.temp			设备描述
  Output:			无
  Return:			无
  Others:		
*************************************************/
char * get_houseno_desc(char * numtext, char * temp)
{   
    char devno[30] = {0};
    PFULL_DEVICE_NO fulldevno;
    int32 i = 0, j = 0, nsub = 0;
    uint32 sub, subLen;
    char strsub[10];
    char des[100] = {0};
	char des_tmp[100] = { 0 };

	char num1[10], des1[11];
    char num[10] = {0};
    int32 index = 0, numIndex = 0, numCount = 0;
    char tmp[100] = {0};
    int32 numTextLen;

 	if (temp == NULL || numtext == NULL)
	{
    	return numtext;
    }
 
    fulldevno = storage_get_devparam();
    sub = fulldevno->Rule.Subsection;
    sprintf(strsub, "%d", sub);
    subLen = strlen(strsub);
    if (subLen < 2)
    {
    	strcpy(temp, numtext);
        return numtext;
    }
    
    nsub = 0;

	memcpy(des_tmp, storage_get_dev_desc(), 70);		// 分段描述符
	for (i = 0; i < subLen; i++)
	{
		// 分段描述转码才可显示
		if (CHINESE == storage_get_language())
		{
			gb2312ToUtf8(des + i * 10, 10, des_tmp + i * 10, 10);
		}
		else if (CHNBIG5 == storage_get_language())
		{

		}
		else
		{
		}
	}

    sprintf(devno, "%s", numtext);
    numTextLen = strlen(devno);

	// subLen: 多少个字段, 如规则224，subLen = 3;
	// strsub: 如"224"
	// i: 0-(subLen-1),如"224"时，i=0 - 2
    for (i = subLen-1; i >= 0; i--)
    {
    	memset(num, 0, sizeof(num));
        num[0] = strsub[subLen-1-i];
        //num[1] = 0;
        // nsub: 每个字段的整数值，如2,2,4
        nsub = atoi(num);
		// des: sub data文件的值，"224"时为30个字节
		// des1: 每个字段的描述字
        memcpy(des1, des+10*(subLen-1-i), 10);

        des1[10] = 0;
        numIndex = 0;
        for (j=index;j<nsub+numCount;j++)
        {       	
        	// num1: 每个字段的编号, 如224规则的"010203040"编号, num1 = "01"、"02"、"0304"
            num1[numIndex++] = devno[j];
            index++;
        }
        // numCount,index: 记住上个字段拷贝后的位置
        numCount = index;
        num1[numIndex] = 0;
		
        // 拷贝字段编号,描述
        sprintf(tmp, "%s%s%s", tmp, num1, des1);
        if (j+1 == numTextLen || j == numTextLen)
        {
            if (j+1 == numTextLen)
            {
                num1[0] =devno[j+1];
                num[1] = 0;
                sprintf(tmp,"%s%s",tmp,num1);
            }
            break;
        }
    }
    sprintf(temp,"%s",tmp);

    return temp;
}

/*************************************************
  Function:			get_dev_description
  Description: 		获得设备描述
  Input: 			
  	1.DevType		设备类型
  	2.DevStr		设备编号
 	3.StrOutLen		输出缓冲大小
  Output:			
  	1.DevStrOut		设备描述
  Return:			无
  Others:		
*************************************************/
uint32 get_dev_description(DEVICE_TYPE_E DevType, char * DevStr, char * DevStrOut, uint32 StrOutLen)
{
	int32 nlen = strlen(DevStr);
 	int32 number = atoi(DevStr);
	
	PFULL_DEVICE_NO fulldevno;
	char DevNo[100] = {0};
	uint8 StairNoLen, CellNoLen, RoomNoLen, UseCellNo;
	uint16 TextID = 0;
	//uint32 OutStrLen = 0;
		
	if (DevStrOut == NULL || DevStr == NULL)
	{
		dprintf("get_dev_description : input param is NULL\n");
    	return 0;
    }

	memset(DevStrOut, 0, sizeof(DevStrOut));
   
	fulldevno = storage_get_devparam();
	StairNoLen = fulldevno->Rule.StairNoLen;
	CellNoLen = fulldevno->Rule.CellNoLen;
	RoomNoLen = fulldevno->Rule.RoomNoLen;
	UseCellNo = fulldevno->Rule.UseCellNo;
	
	switch(DevType)
	{
		case DEVICE_TYPE_MANAGER:					// 管理员机
			TextID = SID_DevManager;
			if (nlen != 0 && number > MANAGER_NUM)
			{
				sprintf(DevNo, "%d", number-MANAGER_NUM);
			}
			break;
			
		case DEVICE_TYPE_AREA:						// 区口机
			TextID = SID_DevArea;
			sprintf(DevNo, "%d", number);
			break;
			
		case DEVICE_TYPE_STAIR:						// 梯口机
		{
			int32 LastNo = atoi(DevStr+StairNoLen);	
			TextID = SID_DevStair;
			sprintf(DevNo, "%d", LastNo+1);			
			break;
		}
		
		case DEVICE_TYPE_DOOR_NET:					// 门前机(网络)
		{
			char dev = DevStr[nlen-1];
			if (dev == '1')
			{
				TextID = SID_DevDoorNet;								
				sprintf(DevNo, "%d", 1);
			}
			else if (dev == '2')
			{
				TextID = SID_DevDoorNet;
				sprintf(DevNo, "%d", 2);
			}
			else
			{
				return 0;
			}
			break;
		}

		case DEVICE_TYPE_DOOR_PHONE:				// 门前机(模拟)
		{
			char dev = DevStr[nlen-1];
			if (dev == '1')
			{
				TextID = SID_DevDoorPhone;		
				sprintf(DevNo, "%d", 1);
			}
			else if (dev == '2')
			{
				TextID = SID_DevDoorPhone;
				sprintf(DevNo, "%d", 2);
			}
			else
			{
				return 0;
			}
			break;
		}

		case DEVICE_TYPE_IPCAMERA:
			TextID = SID_DevIPCamera;
			sprintf(DevNo, "%s", DevStr);
			break;	
		
		case DEVICE_TYPE_ROOM:						// 室内机			
			get_houseno_desc(DevStr, DevNo);
			strcpy(DevStrOut, DevNo);
			return 0;

		case DEVICE_TYPE_FENJI_NET:
			if (nlen == 1)
			{
				if (DevStr && DevStr[0] == '0')
				{
					TextID = SID_DevMaster;					
				}
				else
				{
					TextID = SID_DevFenJi;	
					sprintf(DevNo, "%s", DevStr);
				}
			}
			else
			{
				return 0;
			}
			break;
			
		default:
			return 0;
	}

	strcpy(DevStrOut, get_str(TextID));
	strcat(DevStrOut, DevNo);

	return 0;
}

/*************************************************
Function:			ToRightPwd
Description: 		转换成正确的密码
Input:
1.nPwd			密码
2.bAdd			是否转换
Output:			无
Return:			正确密码
Others:
*************************************************/
static char ToRightPwd(char nPwd, uint8 bAdd)
{
	char nResult = 0;

	if (bAdd)
	{
		if (nPwd == '9')
		{
			nResult = '0';
		}
		else
		{
			nResult = nPwd + 1;
		}
	}
	else
	{
		if (nPwd == '0')
		{
			nResult = '9';
		}
		else
		{
			nResult = nPwd - 1;
		}
	}
	return nResult;
}

/*************************************************
Function:    		toXiechiPwd
Description:		转换成挟持密码
Input:
1.oldPwd		要转换的密码
2.bAdd			挟持密码是否+1：true:+1；false: -1;
Output:
1.newPwd		转换后的密码
Return:
*************************************************/
static void toXiechiPwd(char * oldPwd, uint8 bAdd, char * newPwd)
{
	int i;
	int len = strlen(oldPwd);
	char rightPwd;
	if (len == 0)
	{
		sprintf(newPwd, "%s", "1");
		return;
	}

	rightPwd = oldPwd[len - 1];

	if (rightPwd == 0)
	{
		rightPwd = oldPwd[len - 2];
	}

	rightPwd = ToRightPwd(rightPwd, bAdd);

	for (i = 0; i < len; i++)
	{
		newPwd[i] = oldPwd[i];

		if (i == len - 2 && oldPwd[len - 1] == 0)
		{
			newPwd[i] = rightPwd;
		}
		else
		{
			if (i == len - 1 && oldPwd[len - 1] != 0)
			{
				newPwd[i] = rightPwd;
			}
		}
	}
}

/*************************************************
Function:		pass_oper
Description:	键盘确认键按下处理函数
Input:
Output:		无
Return:		无
Others:
*************************************************/
MSG_EVENT msg_pass_oper_deal(PASS_TYPE type, uint8 xiechi, char* inputPass)
{
	MSG_EVENT event = MSG_EVENT_NO;

	char otherPass[MAX_PASSWORD_LEN+1];
	char otherPassXc[MAX_PASSWORD_LEN + 1];
	char adminPass[MAX_PASSWORD_LEN + 1];
	char adminPassXc[MAX_PASSWORD_LEN + 1];
	char serverPass[MAX_PASSWORD_LEN + 1];
	char serverPassXc[MAX_PASSWORD_LEN + 1];

	memset(otherPass, 0, sizeof(otherPass));
	memset(adminPass, 0, sizeof(adminPass));
	sprintf(adminPass, "%s", storage_get_pass(PASS_TYPE_ADMIN));	
	sprintf(otherPass, "%s", storage_get_pass(type));
	
	memset(adminPassXc, 0, sizeof(adminPassXc));
	memset(otherPassXc, 0, sizeof(otherPassXc));
	toXiechiPwd(adminPass, TRUE, adminPassXc);
	toXiechiPwd(otherPass, TRUE, otherPassXc);

	if (0xff == xiechi)
	{
		if (1 == storage_get_doorserver(0))
		{
			memset(serverPass, 0, sizeof(serverPass));
			memset(serverPassXc, 0, sizeof(serverPassXc));
			sprintf(serverPass, "%s", storage_get_pass(PASS_TYPE_SERVER));
			toXiechiPwd(serverPass, TRUE, serverPassXc);
			if (0 == strcmp(inputPass, serverPassXc))
			{
				event = MSG_EVENT_XIECHI;
			}
			else if (0 == strcmp(inputPass, serverPass))
			{
				event = MSG_EVENT_YES;
			}
			else
			{
				event = MSG_EVENT_NO;
			}
		}
	}

	if ((0 == strcmp(inputPass, otherPassXc) || 0 == strcmp(inputPass, adminPassXc)) && xiechi)				// 挟持
	{
		event = MSG_EVENT_XIECHI;
	}

	if (MSG_EVENT_NO == event)
	{
		if (0 == strcmp(inputPass, otherPass) || 0 == strcmp(inputPass, adminPass))
		{
			event = MSG_EVENT_YES;
		}
		else
		{
			event = MSG_EVENT_NO;
		}
	}

	return event;
}

#if 0
/*************************************************
Function:		change_ip_to_char
Description:  切换IP地址4个字节
Input:
1.IP		IP地址
2.ipaddr	地址
Output:		无
Return:		无
Others:
*************************************************/
void change_ip_to_char(uint32 IP, uint8* ipaddr)
{
	uint32 g_IP;

	if (ipaddr)
	{
		g_IP = IP;
		ipaddr[0] = ip_addr1(g_IP);
		g_IP = IP;
		ipaddr[1] = ip_addr2(g_IP);
		g_IP = IP;
		ipaddr[2] = ip_addr3(g_IP);
		g_IP = IP;
		ipaddr[3] = ip_addr4(g_IP);
	}
}

/*************************************************
Function:		change_char_to_ip
Description:  切换4个字节为IP地址
Input:
1.ipaddr	ip地址
Output:		无
Return:		无
Others:
*************************************************/
uint32 change_char_to_ip(uint8* ipaddr)
{
	uint32 IP = 0;

	if (ipaddr)
	{
		IP = (((uint32)(ipaddr[0] & 0xff) << 24) | ((uint32)(ipaddr[1] & 0xff) << 16) | \
			((uint32)(ipaddr[2] & 0xff) << 8) | (uint32)(ipaddr[3] & 0xff));
	}

	return IP;
}

/*************************************************
Function:		change_ip_to_str
Description:  切换IP地址字符串
Input:
1.IP		IP地址
2.ipaddr	地址
Output:		无
Return:		无
Others:
*************************************************/
void change_ip_to_str(uint32 IP, char *ip)
{
	uint32 g_IP;
	uint8 ipaddr[4] = { 0, 0, 0, 0 };

	g_IP = IP;
	ipaddr[0] = ip_addr1(g_IP);
	g_IP = IP;
	ipaddr[1] = ip_addr2(g_IP);
	g_IP = IP;
	ipaddr[2] = ip_addr3(g_IP);
	g_IP = IP;
	ipaddr[3] = ip_addr4(g_IP);

	sprintf(ip, "%d.%d.%d.%d", ipaddr[0], ipaddr[1], ipaddr[2], ipaddr[3]);
}

/*************************************************
Function:		check_ip_to_true
Description:  检查IP是否合法
Input:
1.IP		IP地址
2.ipaddr	地址
Output:		无
Return:		无
Others:
*************************************************/
uint8 check_ip_to_true(char *ip)
{
	uint32 ipaddr[4] = { 0, 0, 0, 0 };
	char* p[4] = { NULL };
	uint8 tmp[10] = { 0 };
	uint8 i = 0;
	uint8 len = 0;
	char ip_str[16] = { 0 };
	uint32 size = 0;

	strcpy(ip_str, ip);
	len = strlen(ip_str);

	for (i = 0; i < 4; i++)
	{
		p[i] = strchr(ip_str + size, '.');
		if (NULL != p[i])
		{
			if (((p[i] - (ip_str + size)) > 3) || (0 == (p[i] - (ip_str + size))) || (3 == i))
			{
				return FALSE;
			}
			memset(tmp, 0, sizeof(tmp));
			memcpy(tmp, ip_str + size, (p[i] - (ip_str + size)));
			ipaddr[i] = atoi(tmp);
			if (ipaddr[i] > 255)
			{
				return FALSE;
			}

			size = p[i] - ip_str + 1;
			if (len == size)
			{
				return FALSE;
			}
		}
		else
		{
			if (i != 3)
			{
				return FALSE;
			}
		}
	}

	memset(tmp, 0, sizeof(tmp));
	memcpy(tmp, ip_str + size, (len - size));
	ipaddr[3] = atoi(tmp);
	if (ipaddr[3] > 255)
	{
		return FALSE;
	}

	return TRUE;
}
#endif

/*************************************************
Function:		LogicShowWin
Description:	逻辑回调显示窗口
Input:
1.type			显示类型
Output:			无
Return:			无
Others:
*************************************************/
void LogicShowWin(SHOW_WIN_TYPE type, char* param)
{
	switch (type)
	{
		case SHOW_MSG_WIN:
			break;

		case SHOW_SAFETY_WIN:
			break;

		case SHOW_BECALL_WIN:
			{
				PINTER_INFO_S pcallbak_data = (PINTER_INFO_S)param;
				BeCallWin(pcallbak_data);
			}
			break;

		default:
			break;
	}
}

/*************************************************
Function:		TouchManagerKey
Description:	管理员机快捷键
Input:
1.type			显示类型
Output:			无
Return:			无
Others:
*************************************************/
void TouchManagerKey(void)
{
	uint8 i = 0, tmp = 0;
	INTER_INFO_S CallInfo = { 0 };

	int32 ret = ui_show_win_arbitration(SYS_OPER_CALLOUT);
	if (ret == TRUE)
	{
		CallInfo.InterType = INTER_CALLOUT_E;
		CallInfo.DevType = DEVICE_TYPE_MANAGER;

		for (i = 0; i < 3; i++)
		{
			if (storage_get_manager_ip(i + 1))
			{
				tmp = i + MANAGER_NUM + 1;
				continue;
			}
		}

		if (0 == tmp)
		{
			tmp = 0xFF;
		}
		sprintf(CallInfo.DevStr, "%d", tmp);
		BeCallWin(&CallInfo);
	}
}

/*************************************************
Function:		TouchMonitorKey
Description:	监视梯口快捷键
Input:
1.type			显示类型
Output:			无
Return:			无
Others:
*************************************************/
void TouchMonitorKey(void)
{
	PMONITORINFO info = NULL;

	dprintf("monitor!\n");
	info = storage_get_monitorinfo(DEVICE_TYPE_STAIR, 0);
	if (NULL == info)
	{
		dprintf("monitor stair list is null\n");
		return;
	}
	else
	{
		MonitorWin(DEVICE_TYPE_STAIR, 0);		// 点击直接开始监视
	}
}
