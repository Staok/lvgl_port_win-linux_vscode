#include <stdio.h>

#include "lvgl/lvgl.h"
#include "ui/ui.h"

#include "lvgl/src/misc/lv_log.h"

#include "win_hal_init.h"
#include "demosAndExamples.h"
#include "periodic_running_basic.h"

/*
    编译 & 运行 方式：
        1   cmake 手动编译：见 根目录的 CMakeLists.txt 的注释
        2   VsCode 执行 task.json，即 "运行和调试窗口" 直接 点 "Run"，或者 按 F5
*/

int main(int argc, char **argv)
{
    (void)argc; /*Unused*/
    (void)argv; /*Unused*/

    /*Initialize LVGL*/
    lv_init();

    /*Initialize the HAL (display, input devices, tick) for LVGL*/
    if(!win_hal_init()) { LV_LOG_ERROR("win_hal_init()"); return -1;} 

    LV_LOG_USER("start show ui~");

    lvgl_demosAndExamples_show();
    // ui_init();

    while(1) {
        lv_timer_handler_run_in_period(5);

        if(get_is_50ms())
        {
            clear_is_50ms_flag();
            // do something 50ms periodically
        }

        if(get_is_100ms())
        {
            clear_is_100ms_flag();
            // do something 100ms periodically
        }

        if(get_is_500ms())
        {
            clear_is_500ms_flag();
            // do something 500ms periodically
        }

        if(get_is_1s())
        {
            clear_is_1s_flag();
            // do something 1s periodically
            // LV_LOG_USER("1s passed~");
        }
    }

    return 0;
}


