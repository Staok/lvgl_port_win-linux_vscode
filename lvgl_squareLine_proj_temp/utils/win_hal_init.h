#ifndef WIN_HAL_INIT_H
#define WIN_HAL_INIT_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LV_DRV_NO_CONF
#ifdef LV_CONF_INCLUDE_SIMPLE
#include "lv_drv_conf.h"
#else
#include "../../lv_drv_conf.h"
#endif
#endif

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

bool win_hal_init(void);
extern bool lvgl_exit_flag;

#if USE_SDL
    #define _DEFAULT_SOURCE /* needed for usleep() */
    #include <unistd.h>
    #define SDL_MAIN_HANDLED /*To fix SDL's "undefined reference to WinMain" issue*/
    #include <SDL2/SDL.h>
    #include "lv_drivers/sdl/sdl.h"

    SDL_mutex *sdl_mutex_lvgl;
    bool sdl_hal_init(void);
#endif

#if USE_WINDOWS
    #include "lv_drivers/win_drv.h"
#endif

// #if USE_WIN32DRV
//     #define IDI_LVGL_WINDOWS    101
//     #include "lv_drivers/win32drv/win32drv.h"
// #endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif