/*************************************************
  Copyright (C), 2006-2016, Aurine
  File name: 	hw_com_api.c
  Author:    	luofl
  Version:   	2.0
  Date: 		2014-09-30
  Description:  ������������
  				����1---���ڵ��Ժ��뵥Ƭ��ͨ�Ź���
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
  Description: 	�˿�0��ʼ��
  Input: 		��
  Output:		��
  Return:		0: �ɹ� ����ֵ: ʧ��
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
  Description: 	����0����
  Input: 
	1.			����������
	2.			���ݳ���
  Output:		��
  Return:		-1: ʧ�� �ɹ�: ʵ�ʷ��ͳ���
  Others:
*************************************************/
int32 Uart0_send(char * data, int32 datalen)
{
	int32 len = 0;
	
	len = write(UART_DEVICE, data, datalen);				// ʵ��д��ĳ���
	if(len == datalen)
	{
		usleep(5);												// �ȴ�FIFO�е����ݷ������
		return(len);
	}
	else
	{
		return(-1);
	}
}

/*************************************************
  Function:		Uart0_send
  Description: 	����0����
  Input: 
	1.			����������
	2.			���ݳ���
  Output:		��
  Return:		-1: ʧ�� �ɹ�: ʵ�ʷ��ͳ���
  Others:
*************************************************/
int32 Rs485_0_send(char * data, int32 datalen)
{
	int32 len = 0;

	len = write(ITP_DEVICE_RS485_0, data, datalen);		// ʵ��д��ĳ���
	
	if (len == datalen)
	{
		usleep(5);									// �ȴ�FIFO�е����ݷ������
		return(len);
	}
	else
	{
		return(-1);
	}
}

/*************************************************
  Function:		alarm_recv
  Description: 	����1����
  Input: 
	1.datalen:	��������ݳ���
  Output:
	1.data		���յ�������
  Return:		ʵ���յ����ֽ���
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
  Description: 	����1���ռҵ�����
  Input: 
	1.datalen:	��������ݳ���
  Output:
	1.data		���յ�������
  Return:		ʵ���յ����ֽ���
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
  Description: 	����һ֡���ݵ�����1
  Input: 
	1.			����������
	2.			���ݳ���
  Output:		��
  Return:		ʵ�ʷ��ͳ���
  Others:
*************************************************/
static int8 send_to_uart0(char * data, uint8 len)
{
	int8 ret;
	uint8 i, sum = 0;

	// ����У���
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
  Description:	����ι������
  Input: 		��
  Output:		��
  Return:		TRUE/FALSE
  Others:
*************************************************/
static int32 send_feetdog_cmd(void)
{
	#ifdef _USE_MA802_MCU_
	// �·������ʽ:
	// ���ȣ����������ֽ�����(����������)
	// У���֣������ֵ������򣨰��������֣���Uint8�޽�λ�ۼӺ�
	// ֡��ʼ + ���ݳ��� + ������ + ������ + ������ + ������ + У���
	// buf[0]	buf[1]	   buf[2]	 buf[3]	 buf[4]	  buf[5]   buf[6]
	int8 ret;
	uint8 len = 6;
	char buf[10] = {0xAA, 0x04, 0x80, 0x05, 0x02, 0x87};
	#else
	// �·������ʽ:
	// ���ȣ���byte2��ʼ�����һ���ֽ�
	// У���֣�buf[6] = 0 - buf[1] - buf[2]����- buf[5]
	// ͬ���� + ��ַ + ���ݳ��� + ��ˮ��(2�ֽ�) + ���� + У���
	// buf[0]	buf[1] buf[2]	 buf[3]	 buf[4]	  buf[5] buf[6]
	int8 ret;
	uint8 i, len = 7;
	char buf[10] = {0xAA, 0x8F, 0x04, 0x00, 0x00, 0x0FF, 0x00};
	#endif
	
	if (!UartInit)
	{
		return FALSE;
	}

	if (AnsFlag)				// �е�Ӧ��ʱ����ι������
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
  Description: 	�����Ź���������
  Input: 		��
  Output:		��
  Return:		��
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
  Description:	ֹͣι��,�õ�Ƭ����λ��оƬ
  Input: 		��
  Output:		��
  Return:		��
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
  Description: 	���ͱ���Ӧ������
  Input: 		��
  Output:		��
  Return:		��
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
  Description: 	����һ֡���ݵ�485
  Input: 
	1.			����������
	2.			���ݳ���
  Output:		��
  Return:		ʵ�ʷ��ͳ���
  Others:
*************************************************/
int8 send_485_pack(char * data, uint8 len)
{	
	int8 ret;
	uint8 i, sum = 0;

	hw_485_send();
	usleep(1000);
	
	// ����У���
	for (i = 1; i < len-1; i++)
	{
		sum += *(data+i);
	}
	* (data + len-1) = 0 - sum;
	 //ret = Uart0_send(data, len);
	 ret = Rs485_0_send(data, len);	// modi by linp 2016-08-17 ��485���ڽ���״̬

	// ���Ϳ�����ʱʱ��ŵ�Rs485write������ modi by linp 2016-08-17
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
  Description: 	ι���߳�
  Input: 		��
  Output:		��
  Return:		��
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
			send_disable_dog();						// ֹͣι��,�õ�Ƭ����������
		}
		sleep(2);
	}
}

/*************************************************
  Function:		alarm_receive_thread
  Description: 	����1���մ����߳�
  Input: 		��
  Output:		��
  Return:		��
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
		//send_to_uart0(buf, len);					// ���ڵ���,���յ������ݷ��ͻش���
		
		// ���¸���Э�����
		if (buf[0] == 0xAA)
		{
			#if 1
			if ((buf[3] == 0x04) && (buf[4] == 0x01))
			{
				send_alarm_answer();
				if (AlarmCallBackFunc)				// ����״̬�ϱ�
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
  Description: 	����1���մ����߳�(���ռҵ�ģ�鷢��������)
  Input: 		��
  Output:		��
  Return:		��
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
		//send_485_pack(buf, len);						// ���ڵ���,���յ������ݷ��ͻش���

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
  Description: 	���ڲ��Գ���
  Input: 		��
  Output:		��
  Return:		��
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
  Description:  ���õ�Ƭ���ӹ����������
  Input:       	
  	1.flag: 	TRUE: ���� FALSE: ����				
  Output:       ��
  Return:       ��
  Others:
*************************************************/
void hw_set_lcd_pwm0(uint8 flag)
{   
    uint8 buf[7] = {0xAA, 0x8F, 0x04, 0x00, 0x00, 0xF1, 0x00};
	
    if(flag == TRUE)
    {
        buf[5] = 0xF1;//���õ�Ƭ���ӹ����������
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
  Description: 	��������ι��
  Input: 		��
  Output:		��
  Return:		��
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

		// ���ô�����,ֻҪ�з�����,��Ƭ����ʹ�ܿ��Ź�
		//send_enable_dog();
	}
#endif
	return 0;
}

/*************************************************
  Function:		init_alarm_callback
  Description:	��ʼ�������ص�����(����״̬�ɵ�Ƭ�����)
  Input: 
	1.func		�����ص�����ָ��
  Output:
  Return:		�ɹ� = 0������ֵʧ��
  Others:		����8�������˿�״̬�ص�,
  	�����˿�״̬�б仯ʱ,��Ƭ�����ϱ�
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
  Description:	��ʼ���ҵ�ص�����
  Input: 
	1.func		�ҵ�ص�����ָ��
  Output:
  Return:		�ɹ� = 0������ֵʧ��
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

		// ����ע�͵� �ᵼ�°���λ���Ͷϵ�������������
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

