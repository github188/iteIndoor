#include <sys/time.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "SDL/SDL.h"
#include "linphone/linphone_castor3.h"
#include "scene.h"
#include "SysGlobal.h"
#include "storage_include.h"
#include "logic_include.h"
#include "gui_include.h"

#ifdef _WIN32
    #include <crtdbg.h>
#endif

#define MS_PER_FRAME                17     
#define MAX_COMMAND_QUEUE_SIZE      8
#define GESTURE_THRESHOLD           40
#define MOUSEDOWN_LONGPRESS_DELAY   1000

#define _MAIN_MENU_LAYER_			"mainMenuLayer"

// command
typedef enum
{
    CMD_NONE,
    CMD_LOAD_SCENE,   	
    CMD_CALLIN_CALLBAK,
	CMD_CALLREQUEST_CALLBAK,
    CMD_CALLOUT_CALLBAK,
    CMD_MONITOR_CALLBAK,
    CMD_RTSP_CALLBAK,
    CMD_GOTO_MAINMENU,
	CMD_GOTO_BECALL,
    CMD_CHANGE_LANG,
    CMD_PREDRAW
} CommandID;

#define MAX_STRARG_LEN 50

typedef struct
{
    CommandID   id;
    int         arg1;
    int         arg2;
    char        strarg1[MAX_STRARG_LEN];
} Command;


ITUScene theScene;

static QuitValue g_QuitValue = QUIT_NONE;
static bool g_isReady = false;
static mqd_t g_CommandQueue = -1;
static ITUSurface* g_ScreenSurf = NULL;
static SDL_Window *g_window = NULL;

extern ITUActionFunction actionFunctions[];


/*************************************************
  Function:			af_callback_gui
  Description: 		
  Input:			
  Output:			��
  Return:			
  Others:			
*************************************************/
int32 af_callback_gui(int32 Param1,int32 Param2)
{
	return 1;
}

/*************************************************
Function:		callrequest_state_callbak
Description:	呼叫请求回调函数
Input:
Output:			
Return:
Others:
*************************************************/
void callrequest_state_callbak(uint32 param1, uint32 param2)
{
	Command cmd;
	char tmp[31] = { 0 };
	INTER_CALLBACK callbak_data;
	PFULL_DEVICE_NO dev = storage_get_devparam();

	if (g_CommandQueue == -1)
	{
		return;
	}

	memset(&cmd, 0, sizeof(Command));
	memset(&callbak_data, 0, sizeof(INTER_CALLBACK));
	cmd.id = CMD_CALLREQUEST_CALLBAK;
	callbak_data.InterState = (uint8)param1;

	switch (param1)
	{
		case CALL_STATE_REQUEST:
			memcpy(tmp, (char *)param2, sizeof(tmp));
			if (tmp[0] == 0)
			{
				callbak_data.DataLen = 1;
			}
			else
			{
				callbak_data.DataLen = dev->DevNoLen;
				strcpy(callbak_data.Buf, tmp+1);
			}
			break;

		case CALL_STATE_END:
			callbak_data.DataLen = 4;
			sprintf(callbak_data.Buf, "%d", param2);
			break;

		default:
			break;
	}

	memcpy(cmd.strarg1, &callbak_data, sizeof(INTER_CALLBACK));
	mq_send(g_CommandQueue, (const char*)&cmd, sizeof (Command), 0);
}

/*************************************************
  Function:			callout_state_callbak
  Description: 		
  Input:			
  Output:			��
  Return:			
  Others:			
*************************************************/
void callout_state_callbak(uint32 param1, uint32 param2)
{
	Command cmd;
	INTER_CALLBACK callbak_data;
	
	if (g_CommandQueue == -1)
	{
		return;
	}
   
	memset(&cmd, 0, sizeof(Command));
	memset(&callbak_data, 0, sizeof(INTER_CALLBACK));
    cmd.id = CMD_CALLOUT_CALLBAK;
	callbak_data.InterState = (uint8)param1;

	switch (param1)
	{
		case CALL_STATE_REQUEST:			
			break;
			
		case CALL_STATE_CALLING:
			break;
			
		case CALL_NEW_CALLING:
			if (0 == param2)
			{
				callbak_data.DataLen = 0;
			}
			else
			{
				callbak_data.DataLen = sizeof(INTER_INFO_S);
				memcpy(callbak_data.Buf, (INTER_INFO_S *)param2, sizeof(INTER_INFO_S));
			}
			break;
			
		case CALL_STATE_RECORDHINT:
			break;
			
		case CALL_STATE_RECORDING:
			break;
			
		case CALL_STATE_TALK:
			break;
			
		case CALL_STATE_END:	
			break;

		case CALL_TIMER:
			callbak_data.DataLen = sizeof(uint32);
			sprintf(callbak_data.Buf, "%d", param2);
			break;

		default:
			break;
	}
	memcpy(cmd.strarg1, &callbak_data, sizeof(INTER_CALLBACK));
	mq_send(g_CommandQueue, (const char*)&cmd, sizeof (Command), 0);
}

/*************************************************
  Function:			callin_state_callbak
  Description: 		
  Input:			
  Output:			��
  Return:			
  Others:			
*************************************************/
void callin_state_callbak(uint32 param1, uint32 param2)
{
	Command cmd;
	INTER_CALLBACK callbak_data;
	
	if (g_CommandQueue == -1)
	{
		return;
	}

	memset(&cmd, 0, sizeof(Command));
	memset(&callbak_data, 0, sizeof(INTER_CALLBACK));
	cmd.id = CMD_CALLIN_CALLBAK;
	callbak_data.InterState = (uint8)param1;

	switch (param1)
	{			
		case CALL_FORMSHOW_CALLING:
			{
				cmd.id = CMD_GOTO_BECALL;
				memcpy(cmd.strarg1, (PINTER_INFO_S)param2, sizeof(INTER_INFO_S));
				dprintf("cmd.strarg1[0]: %d strarg1[1] : %x strarg1 : %s\n", cmd.strarg1[0], cmd.strarg1[1], cmd.strarg1+2);
				mq_send(g_CommandQueue, (const char*)&cmd, sizeof (Command), 0);
				return;
			}
			break;
			
		case CALL_STATE_CALLING:			
			break;

		case CALL_NEW_CALLING:
			if (0 == param2)
			{
				callbak_data.DataLen = 0;
			}
			else
			{
				callbak_data.DataLen = sizeof(INTER_INFO_S);
				memcpy(callbak_data.Buf, (INTER_INFO_S *)param2, sizeof(INTER_INFO_S));
			}
			break;
			
		case CALL_STATE_RECORDHINT:
			break;
			
		case CALL_STATE_RECORDING:
			break;
			
		case CALL_STATE_TALK:		
			break;
			
		case CALL_STATE_END:			
			break;
			
		case CALL_TIMER:
			callbak_data.DataLen = sizeof(uint32);
			sprintf(callbak_data.Buf, "%d", param2);
			break;

		default:
			return;
			
	}
	memcpy(cmd.strarg1, &callbak_data, sizeof(INTER_CALLBACK));
	mq_send(g_CommandQueue, (const char*)&cmd, sizeof (Command), 0);
}

/*************************************************
  Function:			monitor_state_callbak
  Description: 		
  Input:			
  Output:			��
  Return:			
  Others:			
*************************************************/
void monitor_state_callbak(uint32 param1, uint32 param2)
{
	Command cmd;
	uint16 callbak_cmd;
	
	if (g_CommandQueue == -1)
	{
		return;
	}

    callbak_cmd = param1;
	memset(&cmd, 0, sizeof(Command));
	memcpy(cmd.strarg1, &callbak_cmd, sizeof(uint16));		// ǰ�������ֽڴ洢�ص����� ����Ϊ���Ĳ���
	cmd.id = CMD_MONITOR_CALLBAK;
	
	switch (param1)
	{
		default:
			return;
	}
	mq_send(g_CommandQueue, (const char*)&cmd, sizeof (Command), 0);
}

/*************************************************
  Function:			monitor_state_callbak
  Description: 		����״̬�ص�
  Input:			
  	1.param1		
  	2.param2
  Output:			��
  Return:			
  Others:			
*************************************************/
void rtsp_monitor_state_callbak(uint32 param1, uint32 param2)
{	
	Command cmd;
	uint16 callbak_cmd;
	
	if (g_CommandQueue == -1)
	{
		return;
	}

    callbak_cmd = param1;
	memset(&cmd, 0, sizeof(Command));
	memcpy(cmd.strarg1, &callbak_cmd, sizeof(uint16));		// ǰ�������ֽڴ洢�ص����� ����Ϊ���Ĳ���
    cmd.id = CMD_CALLIN_CALLBAK;
	
	
	switch (param1)
	{
		case MONITOR_SEARCH:
			break;

		case MONITOR_REQUEST:
			break;
		
		case MONITOR_MONITORING:			
			break;

		case MONITOR_END:
			/*
			switch (param2)
			{
				case MONITOR_SEARCH_ERR:			// �����豸ʧ��
					break;
					
				case MONITOR_REQUEST_TIMEOUT:		// ����ʧ��
					break;
					
				case MONITOR_BUSY:					// �Է�æ
					break;
					
				default:
					break;
			}*/
			break;

		case MONITOR_GETLIST:
			break;

		default:
			return;
	}

	if (param1 != MONITOR_TIMER)
	{
	}

	mq_send(g_CommandQueue, (const char*)&cmd, sizeof (Command), 0);
}

/*************************************************
  Function:			show_sys_event_hint
  Description: 		
  Input:			
  Output:			��
  Return:			
  Others:			
*************************************************/
void show_sys_event_hint(uint16 EventType)
{

}

/*************************************************
  Function:		ItuSetLanguage
  Description: 	����ϵͳ����
  Input:		
  	lang		����
  Output:		��
  Return:		��
  Others:
*************************************************/
static void ItuSetLanguage(LANGUAGE_E lang)
{
	ituSceneUpdate(&theScene, ITU_EVENT_LANGUAGE, lang, 0, 0);
	ituSceneUpdate(&theScene, ITU_EVENT_LAYOUT, 0, 0, 0);
}

/*************************************************
  Function:		GotoLayer
  Description: 	�л�ͼ��
  Input:		
  	name		ͼ���Ӧ���� ����Ψһ��
  Output:		��
  Return:		��
  Others:
*************************************************/
static void GotoLayer(const char *name)
{
    ITULayer* Layer = ituSceneFindWidget(&theScene, name);
    assert(Layer);
    ituLayerGoto(Layer);
}

/*************************************************
  Function:		LoadScene
  Description: 	��������UI ���Ե�
  Input:		
  Output:		��
  Return:		��
  Others:
*************************************************/
static void LoadScene(void)
{
    uint32_t tick1, tick2;

    if (theScene.root)
    {
        ituSceneExit(&theScene);
	    #ifdef _WIN32
	    //_CrtMemDumpAllObjectsSince(&state);
	    #endif
    }	
	#ifdef _WIN32
    else
    {
        //_CrtMemCheckpoint(&state);
        //_CrtSetBreakAlloc(8003);
    }
	#endif

    // load itu file
    tick1 = SDL_GetTicks();

    ituSceneLoadFile(&theScene, CFG_PRIVATE_DRIVE ":/doorbell_indoor.itu");

    tick2 = SDL_GetTicks();
    printf("itu loading time: %dms\n", tick2 - tick1);
    tick1 = tick2;
	
	//resetScene();
    ItuSetLanguage(storage_get_language());

    tick2 = SDL_GetTicks();
    printf("itu init time: %dms\n", tick2 - tick1);

}

/*************************************************
  Function:		ProcessCommand
  Description: 	��Ϣ����ִ�к���
  Input:		
  Output:		��
  Return:		��
  Others:
*************************************************/
static void ProcessCommand(void)
{
    Command cmd;

    while (mq_receive(g_CommandQueue, (char*)&cmd, sizeof(Command), 0) > 0)
    {
        switch (cmd.id)
        {
        	case CMD_LOAD_SCENE:
				{
            		LoadScene();
        		}
           		break;      

	        case CMD_GOTO_MAINMENU:
				{
					GotoLayer(_MAIN_MENU_LAYER_);
	        	}
	            break;

	        case CMD_CHANGE_LANG:
				{
					ItuSetLanguage(storage_get_language());		            
	        	}
	            break;

	        case CMD_PREDRAW:
				{
	            	ituScenePreDraw(&theScene, g_ScreenSurf);
	        	}
	            break;

			case CMD_CALLIN_CALLBAK:
				{	
					char buf[MAX_STRARG_LEN];
					memset(buf, 0, sizeof(buf));
					memcpy(buf, cmd.strarg1, MAX_STRARG_LEN);
					ituSceneSendEvent(&theScene, EVENT_CUSTOM13_CALLIN, buf);
				}
				break;

			case CMD_CALLREQUEST_CALLBAK:
				{
					char buf[MAX_STRARG_LEN];
					memset(buf, 0, sizeof(buf));
					memcpy(buf, cmd.strarg1, MAX_STRARG_LEN);
					ituSceneSendEvent(&theScene, EVENT_CUSTOM11_CALLREQUEST, buf);
				}
				break;

			case CMD_CALLOUT_CALLBAK:
				{	
					char buf[MAX_STRARG_LEN];
					memset(buf, 0, sizeof(buf));
					memcpy(buf, cmd.strarg1, MAX_STRARG_LEN);
					ituSceneSendEvent(&theScene, EVENT_CUSTOM12_CALLOUT, buf);
				}
				break;

			case CMD_MONITOR_CALLBAK:
				{	
					char buf[MAX_STRARG_LEN];
					memset(buf, 0, sizeof(buf));
					memcpy(buf, cmd.strarg1, MAX_STRARG_LEN);
					ituSceneSendEvent(&theScene, EVENT_CUSTOM14_MONITOR, buf);
				}
				break;

			case CMD_RTSP_CALLBAK:
				{	
					char buf[MAX_STRARG_LEN];
					memset(buf, 0, sizeof(buf));
					memcpy(buf, cmd.strarg1, MAX_STRARG_LEN);
					ituSceneSendEvent(&theScene, EVENT_CUSTOM15_RTSP_MONITOR, buf);
				}
				break;

			case CMD_GOTO_BECALL:
				{
					char buf[MAX_STRARG_LEN];
					memset(buf, 0, sizeof(buf));
					memcpy(buf, cmd.strarg1, sizeof(INTER_INFO_S));
					LogicShowWin(SHOW_BECALL_WIN, buf);
				}
				break;
				
			default:
				break;
        }
    }
}

/*************************************************
  Function:		CheckQuitValue
  Description: 	�˳���־���
  Input:		
  Output:		��
  Return:		��
  Others:
*************************************************/
static bool CheckQuitValue(void)
{
    if (g_QuitValue)
    {
    	/*
        if (ScreenSaverIsScreenSaving() && theConfig.screensaver_type == SCREENSAVER_BLANK)
            ScreenSaverRefresh();
            */

        if (g_QuitValue == QUIT_UPGRADE_ADDRESSBOOK || g_QuitValue == QUIT_UPGRADE_CARDLIST)
        {
            //ituSceneSendEvent(&theScene, EVENT_CUSTOM_UPGRADE, "1");
        }
        else
            return true;
    }
    return false;
}

/*************************************************
  Function:		SceneInit
  Description: 	������ʼ��
  Input:		
  Output:		��
  Return:		��
  Others:
*************************************************/
void SceneInit(void)
{
    struct mq_attr attr;

    g_window = SDL_CreateWindow("Doorbell Indoor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, CFG_LCD_WIDTH, CFG_LCD_HEIGHT, 0);
    if (!g_window)
    {
        printf("Couldn't create g_window: %s\n", SDL_GetError());
        return;
    }

    // init itu
    ituLcdInit();

#ifdef CFG_M2D_ENABLE
    ituM2dInit();
    ituFrameFuncInit();
#else
    ituSWInit();
#endif

    g_ScreenSurf = ituGetDisplaySurface();

    ituFtInit();
    ituFtLoadFont(0, CFG_PRIVATE_DRIVE ":/font/" CFG_FONT_FILENAME, ITU_GLYPH_8BPP);

    ituSceneInit(&theScene, NULL);
	ituSceneSetFunctionTable(&theScene, actionFunctions);

    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_COMMAND_QUEUE_SIZE;
    attr.mq_msgsize = sizeof(Command);

    g_CommandQueue = mq_open("scene", O_CREAT | O_NONBLOCK, 0644, &attr);
    assert(g_CommandQueue != -1);
   
}

/*************************************************
  Function:		SceneExit
  Description: 	��������ʼ��
  Input:		
  Output:		��
  Return:		��
  Others:
*************************************************/
void SceneExit(void)
{
    mq_close(g_CommandQueue);
    g_CommandQueue = -1;

    if (theScene.root)
    {
        ituSceneExit(&theScene);
    }
    ituFtExit();

#ifdef CFG_M2D_ENABLE
    ituM2dExit();
	#ifdef CFG_VIDEO_ENABLE
    ituFrameFuncExit();
	#endif // CFG_VIDEO_ENABLE
#else
    ituSWExit();
#endif // CFG_M2D_ENABLE

    SDL_DestroyWindow(g_window);
}

/*************************************************
  Function:		SceneLoad
  Description: 	
  Input:		
  Output:		��
  Return:		��
  Others:
*************************************************/
void SceneLoad(void)
{
    Command cmd;

    if (g_CommandQueue == -1)
        return;

    g_isReady  = false;

    cmd.id = CMD_LOAD_SCENE;
    mq_send(g_CommandQueue, (const char*)&cmd, sizeof (Command), 0);
}

/*************************************************
  Function:		SceneRun
  Description: 	
  Input:		
  Output:		��
  Return:		��
  Others:
*************************************************/
int SceneRun(void)
{
    SDL_Event ev;
    int delay, frames, lastx, lasty;
    uint32_t tick, dblclk, lasttick, mouseDownTick;

    /* Watch keystrokes */
    dblclk = frames = lasttick = lastx = lasty = mouseDownTick = 0;

    for (;;)
    {
        bool result = false;

        if (CheckQuitValue())
        {
        	debug_log(" CheckQuitValue: %d \n", CheckQuitValue());
            break;
        }

        ProcessCommand();       

        tick = SDL_GetTicks();

    #ifdef FPS_ENABLE
        frames++;
        if (tick - lasttick >= 1000)
        {
            printf("fps: %d\n", frames);
            frames = 0;
            lasttick = tick;
        }
    #endif // FPS_ENABLE

   
        while (SDL_PollEvent(&ev))
        {
            switch (ev.type)
            {
            	case SDL_KEYDOWN:
					{
		                //ScreenSaverRefresh();
		                result = ituSceneUpdate(&theScene, ITU_EVENT_KEYDOWN, ev.key.keysym.sym, 0, 0);
		                switch (ev.key.keysym.sym)
						{
						    case SDLK_UP:
						        {						            
						            result = true;
						        }
						        break;

						    case SDLK_DOWN:
						        {

								}						        
						        break;

						    case SDLK_LEFT:
						        {						            
						            result = true;
						        }
						        break;

						    case SDLK_RIGHT:
						        {
									result = true;
								}						        
						        break;

						    case SDLK_INSERT:
						        {						           
						            result = true;
						        }
						        break;

						}

						/*
		                if (result && !ScreenIsOff())
		                    LinphonePlayKeySound();
		                  */
		        }
                break;

            case SDL_KEYUP:
				{
                	result = ituSceneUpdate(&theScene, ITU_EVENT_KEYUP, ev.key.keysym.sym, 0, 0);
            	}
                break;

            case SDL_MOUSEMOTION:
				{
                	//ScreenSaverRefresh();
                	result = ituSceneUpdate(&theScene, ITU_EVENT_MOUSEMOVE, ev.button.button, ev.button.x, ev.button.y);
            	}
                break;

            case SDL_MOUSEBUTTONDOWN:
				{
	                //ScreenSaverRefresh();
	                {
	                    mouseDownTick = SDL_GetTicks();
	                #ifdef DOUBLE_KEY_ENABLE
	                    if (mouseDownTick - dblclk <= 200)
	                    {
	                        result = ituSceneUpdate(&theScene, ITU_EVENT_MOUSEDOUBLECLICK, ev.button.button, ev.button.x, ev.button.y);
	                        dblclk = 0;
	                    }
	                    else
	                #endif // DOUBLE_KEY_ENABLE					
	                    {
	                        result = ituSceneUpdate(&theScene, ITU_EVENT_MOUSEDOWN, ev.button.button, ev.button.x, ev.button.y);
	                        dblclk = mouseDownTick;
	                        lastx = ev.button.x;
	                        lasty = ev.button.y;
							/*
	                        if (result && !ScreenIsOff())
	                            LinphonePlayKeySound();
	                           */
	                    }
					
	                #ifdef CFG_SCREENSHOT_ENABLE
					/*
	                    if (ev.button.x < 50 && ev.button.y > CFG_LCD_HEIGHT - 50)
	                        Screenshot(g_ScreenSurf);
	                        */
	                #endif 
	                }
            	}
                break;

            case SDL_MOUSEBUTTONUP:
				{
	                if (SDL_GetTicks() - dblclk <= 200)
	                {
	                    int xdiff = abs(ev.button.x - lastx);
	                    int ydiff = abs(ev.button.y - lasty);

	                    if (xdiff >= GESTURE_THRESHOLD && xdiff > ydiff)
	                    {
	                        if (ev.button.x > lastx)
	                        {
	                            printf("mouse: slide to right\n");
	                            result |= ituSceneUpdate(&theScene, ITU_EVENT_TOUCHSLIDERIGHT, 1, ev.button.x, ev.button.y);
	                        }
	                        else
	                        {
	                            printf("mouse: slide to left\n");
	                            result |= ituSceneUpdate(&theScene, ITU_EVENT_TOUCHSLIDELEFT, 1, ev.button.x, ev.button.y);
	                        }
	                    }
	                    else if (ydiff >= GESTURE_THRESHOLD)
	                    {
	                        if (ev.button.y > lasty)
	                        {
	                            printf("mouse: slide to down\n");
	                            result |= ituSceneUpdate(&theScene, ITU_EVENT_TOUCHSLIDEDOWN, 1, ev.button.x, ev.button.y);
	                        }
	                        else
	                        {
	                            printf("mouse: slide to up\n");
	                            result |= ituSceneUpdate(&theScene, ITU_EVENT_TOUCHSLIDEUP, 1, ev.button.x, ev.button.y);
	                        }
	                    }
	                }
	                result |= ituSceneUpdate(&theScene, ITU_EVENT_MOUSEUP, ev.button.button, ev.button.x, ev.button.y);
	                mouseDownTick = 0;
            	}
                break;

            case SDL_FINGERMOTION:
				{
	                //ScreenSaverRefresh();
	                //printf("touch: move %d, %d\n", ev.tfinger.x, ev.tfinger.y);
	                result = ituSceneUpdate(&theScene, ITU_EVENT_MOUSEMOVE, 1, ev.tfinger.x, ev.tfinger.y);
            	}
                break;

            case SDL_FINGERDOWN:
				{
	                //ScreenSaverRefresh();
	                //printf("touch: down %d, %d\n", ev.tfinger.x, ev.tfinger.y);
	                {
	                    mouseDownTick = SDL_GetTicks();
	                #ifdef DOUBLE_KEY_ENABLE
	                    if (mouseDownTick - dblclk <= 200)
	                    {
	                        printf("double touch!\n");
	                        result = ituSceneUpdate(&theScene, ITU_EVENT_MOUSEDOUBLECLICK, 1, ev.tfinger.x, ev.tfinger.y);
	                        dblclk = mouseDownTick = 0;
	                    }
	                    else
	                #endif // DOUBLE_KEY_ENABLE
	                    {
	                        result = ituSceneUpdate(&theScene, ITU_EVENT_MOUSEDOWN, 1, ev.tfinger.x, ev.tfinger.y);
	                        dblclk = mouseDownTick;
	                        lastx = ev.tfinger.x;
	                        lasty = ev.tfinger.y;

							/*
	                        if (result && !ScreenIsOff())
	                            LinphonePlayKeySound();
	                            */
	                    }
					
	                #ifdef CFG_SCREENSHOT_ENABLE
					/*
	                    if (ev.tfinger.x < 50 && ev.tfinger.y > CFG_LCD_HEIGHT - 50)
	                        Screenshot(g_ScreenSurf);
	                        */
	                #endif
	                }
            	}
                break;

            case SDL_FINGERUP:
				{
	                //printf("touch: up %d, %d\n", ev.tfinger.x, ev.tfinger.y);
	                if (SDL_GetTicks() - dblclk <= 300)
	                {
	                    int xdiff = abs(ev.tfinger.x - lastx);
	                    int ydiff = abs(ev.tfinger.y - lasty);

	                    if (xdiff >= GESTURE_THRESHOLD && xdiff > ydiff)
	                    {
	                        if (ev.tfinger.x > lastx)
	                        {
	                            printf("touch: slide to right %d %d\n", ev.tfinger.x, ev.tfinger.y);
	                            result |= ituSceneUpdate(&theScene, ITU_EVENT_TOUCHSLIDERIGHT, 1, ev.tfinger.x, ev.tfinger.y);
	                        }
	                        else
	                        {
	                            printf("touch: slide to left %d %d\n", ev.button.x, ev.button.y);
	                            result |= ituSceneUpdate(&theScene, ITU_EVENT_TOUCHSLIDELEFT, 1, ev.tfinger.x, ev.tfinger.y);
	                        }
	                    }
	                    else if (ydiff >= GESTURE_THRESHOLD)
	                    {
	                        if (ev.tfinger.y > lasty)
	                        {
	                            printf("touch: slide to down %d %d\n", ev.tfinger.x, ev.tfinger.y);
	                            result |= ituSceneUpdate(&theScene, ITU_EVENT_TOUCHSLIDEDOWN, 1, ev.tfinger.x, ev.tfinger.y);
	                        }
	                        else
	                        {
	                            printf("touch: slide to up %d %d\n", ev.tfinger.x, ev.tfinger.y);
	                            result |= ituSceneUpdate(&theScene, ITU_EVENT_TOUCHSLIDEUP, 1, ev.tfinger.x, ev.tfinger.y);
	                        }
	                    }
	                }
	                result |= ituSceneUpdate(&theScene, ITU_EVENT_MOUSEUP, 1, ev.tfinger.x, ev.tfinger.y);
	                mouseDownTick = 0;
            	}
                break;
            }
        }

		#if 0
        if (callReceived || callConnected || SceneCameraIsViewing())
            ScreenSaverRefresh();

        if (!ScreenIsOff())
        {
            if (mouseDownTick > 0 && (SDL_GetTicks() - mouseDownTick >= MOUSEDOWN_LONGPRESS_DELAY))
            {
                printf("long press: %d %d\n", lastx, lasty);
                result |= ituSceneUpdate(&theScene, ITU_EVENT_MOUSELONGPRESS, 1, lastx, lasty);
                mouseDownTick = 0;
            }
            result |= ituSceneUpdate(&theScene, ITU_EVENT_TIMER, 0, 0, 0);

			//printf("%d\n", result);
        	#ifndef _WIN32
            if (result)
        	#endif
            {
                ituSceneDraw(&theScene, g_ScreenSurf);
                ituFlip(g_ScreenSurf);
            }

            if (theConfig.screensaver_type != SCREENSAVER_NONE && ScreenSaverCheck())
            {
                ituSceneSendEvent(&theScene, EVENT_CUSTOM_SCREENSAVER, "0");

                if (theConfig.screensaver_type == SCREENSAVER_BLANK)
                {
                    // have a change to flush action commands
                    ituSceneUpdate(&theScene, ITU_EVENT_TIMER, 0, 0, 0);

                    // draw black screen
                    ituSceneDraw(&theScene, g_ScreenSurf);
                    ituFlip(g_ScreenSurf);

                    ScreenOff();
                }
            }
        }
		#else
		result |= ituSceneUpdate(&theScene, ITU_EVENT_TIMER, 0, 0, 0);
        #ifndef _WIN32
        if (result)
    	#endif
        {
            ituSceneDraw(&theScene, g_ScreenSurf);
            ituFlip(g_ScreenSurf);
        }
		#endif
		
        delay = MS_PER_FRAME - (SDL_GetTicks() - tick);
        //printf("scene loop delay=%d\n", delay);
        if (delay > 0)
            SDL_Delay(delay);
        else
            sched_yield();
    }

    return g_QuitValue;
}

/*************************************************
  Function:		SceneSetQuitValue
  Description: 	�����˳���־
  Input:		
  Output:		��
  Return:		��
  Others:
*************************************************/
void SceneSetQuitValue(QuitValue value)
{
    g_QuitValue = value;
}

/*************************************************
  Function:		SceneGetQuitValue
  Description: 	��ȡ�˳���־
  Input:		
  Output:		��
  Return:		��
  Others:
*************************************************/
QuitValue SceneGetQuitValue(void)
{
    return g_QuitValue;
}
