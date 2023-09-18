#ifndef WIN_HAL_INIT_H
#define WIN_HAL_INIT_H

#define _DEFAULT_SOURCE /* needed for usleep() */
#include <unistd.h>

#define SDL_MAIN_HANDLED /*To fix SDL's "undefined reference to WinMain" issue*/
#include <SDL2/SDL.h>

#include "lvgl/lvgl.h"
#include "lv_drivers/sdl/sdl.h"

bool win_hal_init(void);

#endif