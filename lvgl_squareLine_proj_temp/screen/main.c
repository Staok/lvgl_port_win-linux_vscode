#include <stdio.h>
#include <stdlib.h>

#include "lvgl/lvgl.h"
#include "ui/ui.h"

#include "lvgl/src/misc/lv_log.h"

#include "win_hal_init.h"
#include "demosAndExamples.h"

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
        /* Periodically call the lv_task handler.
        * It could be done in a timer interrupt or an OS task too.*/

        // 下面两种方式都可以
        // lv_timer_handler(); usleep( 5000 );  // 方式一。若 写成 usleep( lv_timer_handler() ); 会很占 cpu
        lv_timer_handler_run_in_period(5);      // 方式二, 省 cpu
    }

    return 0;
}


