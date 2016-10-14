/*************************************************
  Copyright (C), 2006-2016, Aurine
  File name:  	logic_screen.c
  Author:     	chenbh
  Version:    	1.0
  Date: 		
  Description:  屏幕操作 屏保开关屏等
  
  History:        
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#include "logic_screen.h"
#include "../scene.h"

static LCD_STATE_E g_ScreenState = LCD_STATE_OPEN;
static float g_ScreenLcdPowerDown;			
static float g_ScreenSaverCountDown;
static uint32_t g_ScreenSaverLastTick;
static bool g_ScreenSaverPause;

void ScreenInit(void)
{
	// 目前驱动那边不支持该设置
    //ioctl(ITP_DEVICE_BACKLIGHT, ITP_IOCTL_SET_BRIGHTNESS, (void*)theConfig.brightness);
    g_ScreenSaverLastTick = SDL_GetTicks();    
	g_ScreenSaverCountDown = (storage_get_screen_intime())*1.0f;
	g_ScreenLcdPowerDown = (storage_get_closelcd_time())*1.0f;
	g_ScreenState = LCD_STATE_OPEN;
    g_ScreenSaverPause = false;
}

bool ScreenIsOff(void)
{
    return (g_ScreenState == LCD_STATE_OFF);
}

void ScreenSetBrightness(int value)
{
	dprintf("value: %d\n", value);
	// 目前驱动那边不支持该设置
    //ioctl(ITP_DEVICE_BACKLIGHT, ITP_IOCTL_SET_BRIGHTNESS, (void*)value);
}

void ScreenOff(void)
{
	printf("Screen Off!\n");
	if (LCD_STATE_OFF != g_ScreenState)
	{
		hw_lcd_back_off();
		g_ScreenState = LCD_STATE_OFF;
		sys_set_hint_state(SYS_HINT_OPERATING, FALSE);
		hw_speak_off();								// 关喇叭
		hw_lcd_power_off();

		// add by chenbh back to mainlayer
		mainLayerShow();
	}
}

void ScreenOn(void)
{
	if (LCD_STATE_OFF == g_ScreenState)
	{
		hw_lcd_power_on();							// 开屏电源
		sys_set_hint_state(SYS_HINT_OPERATING, TRUE);
		g_ScreenState = LCD_STATE_OPEN;
		usleep(200*1000);
		hw_lcd_back_on();							// 开屏背光
		//hw_speak_on();							// 开喇叭
	}
	g_ScreenState = LCD_STATE_OPEN;
}

void ScreenProtect(void)
{
	ScreenSaverLayerInit();
	g_ScreenState = LCD_STATE_PROTECT;
}

void ScreenSaverRefresh(void)
{
    g_ScreenSaverLastTick = SDL_GetTicks();
    g_ScreenSaverCountDown = (storage_get_screen_intime())*1.0f;
	g_ScreenLcdPowerDown = (storage_get_closelcd_time())*1.0f;
	
    if (g_ScreenState == LCD_STATE_OFF)
    {
        ScreenOn();
        //NetworkSntpUpdate();
    }
	else if (g_ScreenState == LCD_STATE_PROTECT)
	{
		// back to mainlayer
		mainLayerShow();
		g_ScreenState = LCD_STATE_OPEN;
	}
}

int ScreenSaverCheck(void)
{
    uint32_t diff, tick = SDL_GetTicks();

    if (g_ScreenSaverPause)
    {
        g_ScreenSaverLastTick = tick;
        return 0;
    }

    if (tick >= g_ScreenSaverLastTick)
    {
        diff = tick - g_ScreenSaverLastTick;
    }
    else
    {
        diff = 0xFFFFFFFF - g_ScreenSaverLastTick + tick;
    }

    //printf("ScreenSaverCheck: tick: %d diff: %d countdown: %d  lcddowm: %s\n", tick, diff, (int)g_ScreenSaverCountDown, (int)g_ScreenLcdPowerDown);

    if (diff >= 1000)
    {   		
        g_ScreenSaverLastTick = tick;
		
		g_ScreenLcdPowerDown -= (float)diff / 1000.0f;
		if (g_ScreenLcdPowerDown <= 0.0f)
		{
			#ifdef _USE_FOR_SHOW_
			return -1;		// 展会版本不关屏
			#else
			return -2;
			#endif			
		}

		uint8 enable = storage_get_screen_enable();
		if (enable)
		{
			g_ScreenSaverCountDown -= (float)diff / 1000.0f;
			if (g_ScreenSaverCountDown <= 0.0f)
	        {
	            return -1;
	        }
		}		
    }
    return 0;
}

void ScreenSaverPause(void)
{
    g_ScreenSaverPause = true;
}

void ScreenSaverContinue(void)
{
    g_ScreenSaverPause = false;
}

bool ScreenSaverIsScreenSaving(void)
{
    return (LCD_STATE_OFF == g_ScreenState) || (LCD_STATE_PROTECT == g_ScreenState);
}

#ifdef CFG_SCREENSHOT_ENABLE
void Screenshot(void* lcdSurf)
{
    static int screenshotCount = 0;
    static bool overwrite = false;
    char filePath[PATH_MAX];

#ifdef _WIN32

    // get file path
    if (overwrite)
    {
        sprintf(filePath, CFG_TEMP_DRIVE ":/screenshot%04d.ppm", screenshotCount++);
    }
    else
    {
        do
        {
            sprintf(filePath, CFG_TEMP_DRIVE ":/screenshot%04d.ppm", screenshotCount++);
        } while (screenshotCount < 10000 && access(filePath, R_OK) == 0);
    }

    if (screenshotCount >= 10000)
    {
        screenshotCount = 0;
        overwrite = true;
    }

#else
    ITPDriveStatus* driveStatusTable;
    int i;

    // try to find the usb drive
    ioctl(ITP_DEVICE_DRIVE, ITP_IOCTL_GET_TABLE, &driveStatusTable);

    for (i = ITP_MAX_DRIVE - 1; i >= 0; i--)
    {
        ITPDriveStatus* driveStatus = &driveStatusTable[i];
        if (driveStatus->avail && driveStatus->disk >= ITP_DISK_MSC00 && driveStatus->disk <= ITP_DISK_MSC17)
        {
            // get file path
            if (overwrite)
            {
                sprintf(filePath, "%sscreenshot%04d.ppm", driveStatus->name, screenshotCount++);
            }
            else
            {
                do
                {
                    sprintf(filePath, "%sscreenshot%04d.ppm", driveStatus->name, screenshotCount++);
                } while (screenshotCount < 10000 && access(filePath, R_OK) == 0);
            }

            if (screenshotCount >= 10000)
            {
                screenshotCount = 0;
                overwrite = true;
            }
            break;
        }
    }
    if (i < 0)
    {
        printf("cannot find USB drive.\n");
        return;
    }
#endif // _WIN32
    ituScreenshot(lcdSurf, filePath);
}
#endif // CFG_SCREENSHOT_ENABLE
