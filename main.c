#include <unistd.h>
#include "SDL/SDL.h"
#include "ite/itp.h"
#include "scene.h"
#include "SysGlobal.h"
#include "storage_include.h"
#include "driver_include.h"

int SDL_main(int argc, char *argv[])
{
    int ret = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());

	logic_init();	
    SceneInit();
    SceneLoad();
    //AudioInit();
    ret = SceneRun();

    SceneExit();

    exit(ret);
    return ret;
}
