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

#include <stdint.h>
#include <unistd.h> // for usleep() .etc

#ifdef __linux__

    #define DISP_HOR_RES 1280
    #define DISP_VER_RES 720

    #if USE_DRM
        #include "lv_drivers/display/drm.h"
        int8_t drm_hal_init(void);
    #endif

    #if USE_FBDEV
        #include "lv_drivers/display/fbdev.h"
        int8_t fbdev_hal_init(void);
    #endif

    #if USE_EVDEV
        #include "lv_drivers/indev/evdev.h"
        int8_t evdev_hal_init();
    #endif

#elif _WIN32

    #if USE_SDL
        #define SDL_MAIN_HANDLED /*To fix SDL's "undefined reference to WinMain" issue*/
        #include <SDL2/SDL.h>
        #include "lv_drivers/sdl/sdl.h"
        int8_t sdl_hal_init(void);
    #endif

    #if USE_WINDOWS // 使用这个比较卡，可能是其初始化的内部机制，绘制和事件更新速率慢，不如 SDL2 ...
        #include "lv_drivers/win_drv.h"
    #endif

#endif

int8_t lv_proj_hal_init(void);
extern bool lvgl_exit_flag;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
