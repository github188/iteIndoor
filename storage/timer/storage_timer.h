/*************************************************
  Copyright (C), 2006-2016, Aurine
  File name:  	storage_timer.c
  Author:     	luofl
  Version:    	2.0
  Date: 		2014-09-06
  Description:  ʱ�亯��ʵ��ͷ�ļ�

  History:                   
    1. Date:			 
       Author:				
       Modification:	
    2. ...
*********************************************************/

#ifndef __STORAGE_TIMER_H__
#define __STORAGE_TIMER_H__

#include "storage_include.h"

#define RTC_USE				0
#define IsLeapYear(AYear)   (((AYear) % 4 == 0) && (((AYear) % 100 != 0) || ((AYear) % 400 == 0)))

#if 0
typedef struct
{
	uint32 year;
	uint8 month;
	uint8 day;
	uint8 hour;
	uint8 min;
	uint8 sec;	
	uint8 week;
}ZONE_DATE_TIME, * PZONE_DATE_TIME;					// ʱ��ṹ��
#endif

/*************************************************
  Function:		WeekDay
  Description: 	�ض������ն�Ӧ��������
  Input: 		
  	1.AYear		���
  	2.AMonth	��
  	3.ADay		��
  Output:		��
  Return:		��
  Others:
*************************************************/
uint16 WeekDay(int16 AYear, int16 AMonth, int16 ADay);

/*************************************************
  Function:		MonthDays
  Description: 	��ȡ��ǰ����·ݵ�����
  Input: 		
  	1.iYear		���
  	2.iMonth	��
  Output:		��
  Return:		��
  Others:
*************************************************/
uint16 MonthDays(int16 iYear, int16 iMonth);

/*************************************************
  Function:		YearDays
  Description: 	��ȡ��ǰ��ݵ�����
  Input: 		
  	1.iYear		���
  Output:		��
  Return:		��
  Others:
*************************************************/
uint16 YearDays(int16 iYear);

/*************************************************
  Function:		init_rtc_timer
  Description: 	��ȡRTCʱ��
  Input: 		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void init_rtc_timer(void);

/*************************************************
  Function:		set_rtc_timer
  Description: 	����RTCʱ��
  Input: 		
  	1.datatime	��ȡ��ǰʱ��
  Output:		��
  Return:		ECHO_VALUE
  Others:
*************************************************/
uint32 set_rtc_timer(PZONE_DATE_TIME datatime);

/*************************************************
  Function:		get_timer
  Description: 	��ȡ��ǰʱ��
  Input: 		
  	1.datatime	��ȡ��ǰʱ��
  Output:		��
  Return:		��
  Others:
*************************************************/
void get_timer(PZONE_DATE_TIME datatime);

/*************************************************
  Function:		get_timer
  Description: 	��ȡ����ʱ�� û�м���ʱ��
  Input: 		
  	1.datatime	��ȡ��ǰʱ��
  Output:		��
  Return:		��
  Others:
*************************************************/
void get_gmtime(PZONE_DATE_TIME datatime);

/*************************************************
  Function:		get_timeofday
  Description: 	��ȡ��ǰʱ��
  Input: 		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void get_timeofday(void);

/*************************************************
  Function:		add_sys_timer
  Description: 	��ʱʱ��
  Input: 		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void add_sys_timer(void);

/*************************************************
  Function:		init_timer
  Description: 	��ʼ��ʱ��
  Input: 		��
  Output:		��
  Return:		��
  Others:
*************************************************/
void init_timer(void);

/*************************************************
  Function:			get_utc_time
  Description: 		��ȡutcʱ��
   1.DateTime		����ʱ��
   2.TimeZone		ʱ�� (0-12)
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
void get_utc_time(PZONE_DATE_TIME DateTime, uint8 TimeZone);

/*************************************************
  Function:			get_local_time
  Description: 		��ȡ����ʱ��
   1.DateTime		UTCʱ��
   2.TimeZone		ʱ�� (0-12)
  Input: 			��
  Output:			��
  Return:			��
  Others:
*************************************************/
void get_local_time(PZONE_DATE_TIME DateTime, uint8 TimeZone);
#endif

