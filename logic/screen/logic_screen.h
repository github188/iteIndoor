/*************************************************
  Copyright (C), 2006-2016, Aurine
  File name:  	logic_screen.h
  Author:     	chenbh
  Version:    	1.0
  Date: 		
  Description:  ��Ļ���� ������������
  
  History:        
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#ifndef _LOGIC_SCREEN_H_
#define _LOGIC_SCREEN_H_
#include "logic_include.h"

typedef enum
{
	LCD_STATE_OFF		= 0x00,			// LCD ����
	LCD_STATE_OPEN		= 0x01,			// LCD ����			
	LCD_STATE_PROTECT	= 0x02			// LCD ����

}LCD_STATE_E;



void ScreenOff(void);
void ScreenOn(void);
int ScreenSaverCheck(void);
bool ScreenSaverIsScreenSaving(void);
void ScreenSaverRefresh(void);

void ScreenProtect(void);
void ScreenInit(void);

#endif
