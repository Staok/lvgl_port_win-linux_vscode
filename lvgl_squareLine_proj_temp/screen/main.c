#include <stdio.h>
#include <stdlib.h>

#include "lvgl/lvgl.h"
#include "ui/ui.h"

#include "win_hal_init.h"
#include "demosAndExamples.h"
#include "periodic_running_basic.h"

/*
    编译 & 运行 方式：
        第一种：cmake 手动编译：见 根目录的 CMakeLists.txt 的注释
        第二种：VsCode 执行 task.json，即 "运行和调试窗口" 直接 点 "Run"，或者 按 F5
*/

/*
    若要修改 配置文件 lv_conf.h、lv_drv_conf.h 等，要用对比工具辅助着来
*/ 

/*  
    tabView 没有动画过渡了
    在 lv_tabView.c 里面，将 LV_ANIM_OFF 都替换成 LV_ANIM_ON 即可~~
    按需要改吧，其它控件估计也有默认设置为不动画过渡的问题，应该都是源码里写死的，按需改源码吧..尽量不

*/

int main(int argc, char **argv)
{
    (void)argc; /*Unused*/
    (void)argv; /*Unused*/

    /*Initialize LVGL*/
    lv_init();

    /*Initialize the HAL (display, input devices, tick) for LVGL*/
    if(!win_hal_init()) { LV_LOG_ERROR("win_hal_init() failure"); return -1;}

    LV_LOG_USER("start show ui~");

    lvgl_demosAndExamples_show();
    // ui_init();

    for(;;)
    {
        lv_timer_handler_run_in_period(PERIODIC_TIME_IN_MS);

        #if USE_WINDOWS
            win_drv_loop_run();
        #endif

        // 屏幕休眠
        // https://docs.lvgl.io/master/porting/sleep.html

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

        if(lvgl_exit_flag) break;
    }

    LV_LOG_USER("THE END~");
    return 0;
}


