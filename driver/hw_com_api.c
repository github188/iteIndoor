/*************************************************
  Copyright (C), 2006-2016, Aurine
  File name: 	hw_com_api.c
  Author:    	luofl
  Version:   	2.0
  Date: 		2014-09-30
  Description:  串口驱动程序
  				串口1---用于调试和与单片机通信共用
  History:        
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#include <stdio.h>				// printf
#include <fcntl.h>				// open
#include <string.h>				// bzero
#include <stdlib.h>				// exit
#include <sys/times.h>			// times
#include <sys/types.h>			// pid_t
#include <unistd.h>
#include <sys/ioctl.h>          	// ioctl
#include <pthread.h>
#include "ite/ith.h"
#include "ite/itp.h"
#include "driver_include.h"

#ifndef WIN32
#define UART_DEVICE				ITP_DEVICE_UART2
#define UART_BAUDRATE			CFG_UART2_BAUDRATE
#define UART_STACK_SIZE 		500000
const ITPDevice *ItpDevice = &itpDeviceUart2;
#else
#define UART_DEVICE				1
#endif

static uint8 FeetDog = 0;
static uint8 InitFeetDogFlag = 0;
static uint8 InitAlarmFlag = 0;
static uint8 InitJdFlag = 0;
static uint8 AnsFlag = 0;
static uint8 UartInit = 0;

static AlarmCallBack AlarmCallBackFunc = NULL;
static JdCallBack JdCallBackFunc = NULL;


/*************************************************
  Function:		Uart0_init
  Description: 	端口0初始化
  Input: 		无
  Output:		无
  Return:		0: 成功 其它值: 失败
  Others:
*************************************************/
int32 Uart0_init(void)
{
#ifndef WIN32
	if (!UartInit)
	{
		//itpRegisterDevice(UART_DEVICE, ItpDevice);
	    //ioctl(UART_DEVICE, ITP_IOCTL_INIT, NULL);
	    //ioctl(UART_DEVICE, ITP_IOCTL_RESET, (void*)UART_BAUDRATE);
		UartInit = 1;
	}
#endif

	return TRUE;
}

/*************************************************
  Function:		Uart0_send
  Description: 	串口0发送
  Input: 
	1.			待发送数据
	2.			数据长度
  Output:		无
  Return:		-1: 失败 成功: 实际发送长度
  Others:
*************************************************/
int32 Uart0_send(char * data, int32 datalen)
{
	int32 len = 0;
	
	len = write(UART_DEVICE, data, datalen);				// 实际写入的长度
	if(len == datalen)
	{
		usleep(5);												// 等待FIFO中的数据发送完毕
		return(len);
	}
	else
	{
		return(-1);
	}
}

/*************************************************
  Function:		Uart0_send
  Description: 	串口0发送
  Input: 
	1.			待发送数据
	2.			数据长度
  Output:		无
  Return:		-1: 失败 成功: 实际发送长度
  Others:
*************************************************/
int32 Rs485_0_send(char * data, int32 datalen)
{
	int32 len = 0;

	len = write(ITP_DEVICE_RS485_0, data, datalen);		// 实际写入的长度
	
	if (len == datalen)
	{
		usleep(5);									// 等待FIFO中的数据发送完毕
		return(len);
	}
	else
	{
		return(-1);
	}
}

/*************************************************
  Function:		alarm_recv
  Description: 	串口1接收
  Input: 
	1.datalen:	读入的数据长度
  Output:
	1.data		接收到的数据
  Return:		实际收到的字节数
  Others:
*************************************************/
int32 alarm_recv(char * data, int32 datalen)
{
	int i;	
	for (i = 0; i < datalen; i++)
	{
		read(UART_DEVICE, data+i, 1);
		usleep(50);
	}
	return datalen;
}

/*************************************************
  Function:		jd_recv
  Description: 	串口1接收家电数据
  Input: 
	1.datalen:	读入的数据长度
  Output:
	1.data		接收到的数据
  Return:		实际收到的字节数
  Others:
*************************************************/
static int32 jd_recv(char * data)
{
	int i = 0;
	uint8 len = 0;
	char getstr[256];
	
	len = read(ITP_DEVICE_RS485_0, data, 3);
	if (data[0] == 0xAA && data[1] == 0x02 && data[2] < 50)
	{
		for (i = 0; i < data[2]; i++)
		{
			read(ITP_DEVICE_RS485_0, data+3+i, 1);
			usleep(200);
		}
		len = data[2] + 3;
	}
	return len;
}

/*************************************************
  Function:		send_to_uart0
  Description: 	发送一帧数据到串口1
  Input: 
	1.			待发送数据
	2.			数据长度
  Output:		无
  Return:		实际发送长度
  Others:
*************************************************/
static int8 send_to_uart0(char * data, uint8 len)
{
	int8 ret;
	uint8 i, sum = 0;

	// 计算校验和
	#ifdef _USE_MA802_MCU_
	for (i = 2; i < len-1; i++)
	{
		sum += *(data+i);
	}
	* (data + len-1) = sum;
	#else
	for (i = 1; i < len-1; i++)
	{
		sum += *(data+i);
	}
	* (data + len-1) = 0 - sum;
	#endif
	ret = Uart0_send(data, len);
	
	return ret;
}

/*************************************************
  Function:    	send_feetdog_cmd
  Description:	发送喂狗命令
  Input: 		无
  Output:		无
  Return:		TRUE/FALSE
  Others:
*************************************************/
static int32 send_feetdog_cmd(void)
{
	#ifdef _USE_MA802_MCU_
	// 下发命令格式:
	// 长度：后面所有字节总数(不包括本身)
	// 校验字：控制字到数据域（包含控制字）的Uint8无进位累加和
	// 帧起始 + 数据长度 + 控制字 + 主命令 + 次命令 + 数据域 + 校验和
	// buf[0]	buf[1]	   buf[2]	 buf[3]	 buf[4]	  buf[5]   buf[6]
	int8 ret;
	uint8 len = 6;
	char buf[10] = {0xAA, 0x04, 0x80, 0x05, 0x02, 0x87};
	#else
	// 下发命令格式:
	// 长度：从byte2开始到最后一个字节
	// 校验字：buf[6] = 0 - buf[1] - buf[2]……- buf[5]
	// 同步码 + 地址 + 数据长度 + 流水号(2字节) + 命令 + 校验和
	// buf[0]	buf[1] buf[2]	 buf[3]	 buf[4]	  buf[5] buf[6]
	int8 ret;
	uint8 i, len = 7;
	char buf[10] = {0xAA, 0x8F, 0x04, 0x00, 0x00, 0x0FF, 0x00};
	#endif
	
	if (!UartInit)
	{
		return FALSE;
	}

	if (AnsFlag)				// 有等应答时不发喂狗命令
	{
		AnsFlag = 0;
		return FALSE;
	}
	
	ret = send_to_uart0(buf, len);
	#if 0
	uint8 i;
	for (i = 0; i < len; i++)
	{
		//printf("buf[%d]: %02x\n", i, buf[i]);
		printf("%02x-", buf[i]);
	}
	printf("\n");
	#endif
	if (ret == len)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	
	return FALSE;
}

/*************************************************
  Function:		send_disable_dog
  Description: 	发看门狗禁能命令
  Input: 		无
  Output:		无
  Return:		无
  Others:		
*************************************************/
static int32 send_disable_dog(void)
{
	int8 ret = FALSE;
	#ifdef _USE_MA802_MCU_
	uint8 len = 7;
	char buf[10] = {0xAA, 0x05, 0xC0, 0x05, 0x01, 0x00, 0xC7};
	#else
	uint8 len = 7;
	char buf[10] = {0xAA, 0x8F, 0x04, 0x00, 0x00, 0xE2, 0x00};
	#endif
	
	if (!UartInit)
	{
		return FALSE;
	}
	
	ret = send_to_uart0(buf, len);
	#if 0
	uint8 i;
	for (i = 0; i < len; i++)
	{
		//printf("buf[%d]: %02x\n", i, buf[i]);
		printf("%02x-", buf[i]);
	}
	printf("\n");
	#endif

	if (ret == len)
	{
		ret = TRUE;
	}
	else
	{
		ret = FALSE;
	}
	
	return ret;
}

/*************************************************
  Function:    	hw_stop_feet_dog
  Description:	停止喂狗,让单片机复位主芯片
  Input: 		无
  Output:		无
  Return:		无
  Others:
*************************************************/
void hw_stop_feet_dog(void)
{	
	#ifdef _JD_MODE_
	uint8 buf[7] = {0xAA, 0x8F, 0x04, 0x00, 0x00, 0xE2, 0x00};
	jd_stop_feet_dog(buf, sizeof(buf));
	jd_stop_feet_dog(buf, sizeof(buf));
	jd_stop_feet_dog(buf, sizeof(buf));
	#else
	FeetDog = 1;
	#endif
}

/*************************************************
  Function:		send_alarm_answer
  Description: 	发送报警应答命令
  Input: 		无
  Output:		无
  Return:		无
  Others:		
*************************************************/
static int32 send_alarm_answer(void)
{
	int8 ret = FALSE;
	uint8 len = 7;
	char buf[10] = {0xAA, 0x05, 0x00, 0x04, 0x01, 0x00, 0x05};

	if (!UartInit)
	{
		return FALSE;
	}

	AnsFlag = 1;
	ret = send_to_uart0(buf, len);
	#if 0
	uint8 i;
	for (i = 0; i < len; i++)
	{
		dprintf("buf[%d]: %02x\n", i, buf[i]);
	}
	#endif

	if (ret == len)
	{
		ret = TRUE;
	}
	else
	{
		ret = FALSE;
	}
	
	return ret;
}

/*************************************************
  Function:		send_485_pack
  Description: 	发送一帧数据到485
  Input: 
	1.			待发送数据
	2.			数据长度
  Output:		无
  Return:		实际发送长度
  Others:
*************************************************/
int8 send_485_pack(char * data, uint8 len)
{	
	int8 ret;
	uint8 i, sum = 0;

	hw_485_send();
	usleep(1000);
	
	// 计算校验和
	for (i = 1; i < len-1; i++)
	{
		sum += *(data+i);
	}
	* (data + len-1) = 0 - sum;
	 //ret = Uart0_send(data, len);
	 ret = Rs485_0_send(data, len);	// modi by linp 2016-08-17 置485处于接收状态

	// 发送控制延时时间放到Rs485write里面做 modi by linp 2016-08-17
	#if 0
	if ((JD_FACTORY_ACBUS == storage_get_extmode(EXT_MODE_JD_FACTORY)))
	{
		usleep(20*1000);
	}
	else
	{
		usleep(30*1000);
	}
	#endif 
	
	hw_485_recv();
	
	return ret;
}

/*************************************************
  Function:		feetdog_thread
  Description: 	喂狗线程
  Input: 		无
  Output:		无
  Return:		无
  Others:		
*************************************************/
static void * feetdog_thread(void* data)
{
	while (1)
	{		
		if (!FeetDog)
		{
			send_feetdog_cmd();
		}
		else
		{
			send_disable_dog();						// 停止喂狗,让单片机立即重启
		}
		sleep(2);
	}
}

/*************************************************
  Function:		alarm_receive_thread
  Description: 	串口1接收处理线程
  Input: 		无
  Output:		无
  Return:		无
  Others:		
*************************************************/
static void * alarm_receive_thread(void* data)
{
	uint8 len = 7;
	char buf[10] = {""};

	dprintf("Ready receive data form com1!!!\n");
	for (;;)
	{
		memset(buf, 0, sizeof(buf));
		alarm_recv(buf, len);
		
		#if 0
		uint8 i;
		dprintf("len = %d\n", len);
		for (i = 0; i < len; i++)
		{
			printf("  %02x", buf[i]);
		}
		dprintf("\n===================\n");
		#endif
		//send_to_uart0(buf, len);					// 用于调试,将收到的数据发送回串口
		
		// 以下根据协议更改
		if (buf[0] == 0xAA)
		{
			#if 1
			if ((buf[3] == 0x04) && (buf[4] == 0x01))
			{
				send_alarm_answer();
				if (AlarmCallBackFunc)				// 防区状态上报
				{
					AlarmCallBackFunc(buf[5]);
				}
			}
			#endif
		}
	}
}

/*************************************************
  Function:		jd_receive_thread
  Description: 	串口1接收处理线程(接收家电模块发出的数据)
  Input: 		无
  Output:		无
  Return:		无
  Others:		
*************************************************/
static void * jd_receive_thread(void* data)
{
	uint8 len = 0;
	static char buf[51] = {""};

	printf("Ready receive data form com1!!!\n");
	
	for (;;)
	{
		memset(buf, 0, sizeof(buf));
		len = jd_recv(buf);
		//send_485_pack(buf, len);						// 用于调试,将收到的数据发送回串口

		#if 0
		uint8 i;
		printf("len = %d\n", len);
		for (i = 0; i < len; i++)
		{
			printf("  %02x", buf[i]);
		}
		printf("\n===================\n");
		#endif
		
		if (JdCallBackFunc)
		{
			//JdCallBackFunc(buf);
		}
        usleep(1000);
	}
}

/*************************************************
  Function:		test_receive_thread
  Description: 	串口测试程序
  Input: 		无
  Output:		无
  Return:		无
  Others:		
*************************************************/
static void* test_receive_thread(void* data)
{
	int i;
    char getstr[256];
    char sendtr[256];
	int len = 0;
	
	while(1)
    {
       memset(getstr, 0, 256);
	   memset(sendtr, 0, 256);
	   	len = read(ITP_DEVICE_RS485_0,getstr,256);

	   if(len)
	   {
	  		for (i=0;i<len;i++)
		   	printf("uart read getstr[%d]: %02x\n",i, getstr[i]);

			if (getstr[1] == 0x02)
			{
				hw_lcd_power_on();
				hw_lcd_back_on();
			}
			else
			{
				hw_lcd_power_off();
				hw_lcd_back_off();
			}
           		// write(UART_DEVICE, sendtr, 256);
		   	len = 0;
	   	}
	   sleep(1);
	}
}

/*************************************************
  Function:     hw_set_lcd_pwm0
  Description:  设置单片机接管屏背光控制
  Input:       	
  	1.flag: 	TRUE: 关屏 FALSE: 开屏				
  Output:       无
  Return:       无
  Others:
*************************************************/
void hw_set_lcd_pwm0(uint8 flag)
{   
    uint8 buf[7] = {0xAA, 0x8F, 0x04, 0x00, 0x00, 0xF1, 0x00};
	
    if(flag == TRUE)
    {
        buf[5] = 0xF1;//设置单片机接管屏背光控制
    }
    else
    {
        buf[5] = 0xF2;
    }
	
    jd_stop_feet_dog(buf, sizeof(buf));
    jd_stop_feet_dog(buf, sizeof(buf));
    jd_stop_feet_dog(buf, sizeof(buf));
}

/*************************************************
  Function:		hw_start_com_dog
  Description: 	启动串口喂狗
  Input: 		无
  Output:		无
  Return:		无
  Others:		
*************************************************/
int hw_start_com_dog(void)
{
#ifndef WIN32
	pthread_t task;
	pthread_attr_t attr;
	
	if (!InitFeetDogFlag)
	{					
		if (FALSE == Uart0_init())
		{
			return;
		}

		InitFeetDogFlag = 1;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		//pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
		//pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
		//pthread_attr_setstacksize(&attr, 48*1024);
		pthread_create(&task, &attr, feetdog_thread, NULL);

		// 不用此命令,只要有发心跳,单片机就使能看门狗
		//send_enable_dog();
	}
#endif
	return 0;
}

/*************************************************
  Function:		init_alarm_callback
  Description:	初始化报警回调函数(防区状态由单片机检测)
  Input: 
	1.func		报警回调函数指针
  Output:
  Return:		成功 = 0，其它值失败
  Others:		报警8个防区端口状态回调,
  	防区端口状态有变化时,单片机会上报
*************************************************/
int32 init_alarm_callback(AlarmCallBack func)
{
#ifndef WIN32
	pthread_t task;
	pthread_attr_t attr;
	
	if (!InitAlarmFlag)
	{			
		if (FALSE == Uart0_init())
		{
			return;
		}
		
		InitAlarmFlag = 1;			
		AlarmCallBackFunc = func;
		pthread_attr_init (&attr);
		pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);
		//pthread_attr_setstacksize (&attr, 48*1024);
		pthread_create(&task, &attr, alarm_receive_thread, NULL);
	}
#endif

	return 0;
}

/*************************************************
  Function:		init_jd_callbackt
  Description:	初始化家电回调函数
  Input: 
	1.func		家电回调函数指针
  Output:
  Return:		成功 = 0，其它值失败
  Others:		
*************************************************/
int32 init_jd_callbackt(JdCallBack func)
{
#ifndef WIN32
	pthread_t task;
	pthread_attr_t attr;
	
	if (!InitJdFlag)
	{		
		if (FALSE == Uart0_init())
		{
			return;
		}
		
		InitJdFlag = 1;
		JdCallBackFunc = func;			

		// 底下注释掉 会导致按复位键和断电重启起不来现象
		#if 1
		pthread_attr_init(&attr);
		attr.stacksize = UART_STACK_SIZE;
		pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);
		pthread_create(&task, &attr, jd_receive_thread, NULL);	
		// pthread_create(&task, &attr, test_receive_thread, NULL);	
		#endif
	}
#endif
	return 0;
}

