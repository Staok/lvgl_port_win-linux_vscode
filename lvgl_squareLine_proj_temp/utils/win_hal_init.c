#include "win_hal_init.h"
#include "periodic_running_basic.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

static pthread_mutex_t lv_task_thread_mutex;
static pthread_t lv_task_thread_tid;

bool lvgl_exit_flag = false;

static void *lv_task_thread(void *in_arg)
{
    (void*)in_arg;
    pthread_detach(pthread_self());
    LV_LOG_USER("run lv_task_thread()~");
    for(;;)
    {
        /*
            这个 lv_task_handler() 放到线程里就是跑不起来。。
            lv_tick_inc 应该执行优先级高于 lv_task_handler，后者放到线程里，那么前者必须放到更高优先级的地方可以打断后者才行
        */
        // unsigned int delay_ms;
        // pthread_mutex_lock(&lv_task_thread_mutex);
        // delay_ms = lv_task_handler();
        // pthread_mutex_unlock(&lv_task_thread_mutex);

        // lv_tick_inc(delay_ms); 
        // periodic_running_basic_inc(delay_ms);
        // usleep( delay_ms * 1000 );

        #if !LV_TICK_CUSTOM
            pthread_mutex_lock(&lv_task_thread_mutex);
                lv_tick_inc(PERIODIC_TIME_IN_MS);
            pthread_mutex_unlock(&lv_task_thread_mutex);
        #endif

        usleep(PERIODIC_TIME_IN_MS * 1000);

        #if USE_WINDOWS
            if(lv_win_exit_flag)
            {
                lvgl_exit_flag = true;
                break;
            }
        #endif

        // #if USE_WIN32DRV
        //     if(lv_win32_quit_signal)
        //     {
        //         lvgl_exit_flag = true;
        //         break;
        //     }
        // #endif
    }
    LV_LOG_USER("exit lv_task_thread()~");
    pthread_exit(0);
}

bool win_hal_init(void)
{
    // 先到 lv_drv_conf.h 里面打开相应的宏
    // 对于 win，有 USE_SDL 或 USE_WINDOWS

    // 创建 lvgl 轮询的线程，建个互斥锁，每次在 线程中用到 lvgl 的 api 都要用这个 互斥锁包住
    int ret = -1;
    lv_task_thread_mutex = PTHREAD_MUTEX_INITIALIZER;
    ret = pthread_create(&lv_task_thread_tid, NULL, lv_task_thread, NULL);
    if(ret < 0) { LV_LOG_ERROR("pthread_create() failure"); return false; }

    bool lvgl_exit_flag = false;
    /*
        USE_SDL 使用：
            lv_conf.h 里面不打开 "LV_TICK_CUSTOM"，即 使用 线程 lv_task_thread() 里面 运行 lv_tick_inc()
        或者：
            lv_conf.h 里面打开 "LV_TICK_CUSTOM"，即使用 custom_tick_get() 返回当前 ms 数
            此时 lv_tick_inc() 就不能用了，lv_task_thread() 里面的注释掉
    */
    #if USE_SDL
        return sdl_hal_init();
    #endif

    /*
        USE_WINDOWS 使用：
            1、在一个 loop 里面循环执行 win_drv_loop_run()，目前是在 线程 lv_task_thread() 里面 运行
                即要循环执行 win_drv.c 里面的 msg_handler()
                win_drv.c 里面 原来使用 lv_task_create() 循环执行 msg_handler()，但是现在 lvgl 不支持 lv_task_create() 了，将那里注释掉，这里循环调用
            2、lv_conf.h 里面不要打开 "LV_TICK_CUSTOM"，win_drv_loop_run() 里面的 WndProc() 会循环执行，会调用 lv_tick_inc()
            3、退出信号 lv_win_exit_flag
    */
    #if USE_WINDOWS
        char* windrv_init_ret = (char*)windrv_init();
        if(!windrv_init_ret) 
            return false;
        else
            return true;
    #endif

    /*
        USE_WIN32DRV 使用： refer to https://github.com/lvgl/lv_port_pc_visual_studio
            1、lv_conf.h 里面要打开 "LV_TICK_CUSTOM"，即 使用 custom_tick_get() 里面的 GetTickCount() 返回当前 ms 数
            2、退出信号 lv_win32_quit_signal
            可能是平台关系， win32drv.c 里面 一堆找不到的符号，MinGW + cmake 编译不过去。可能在 VS 平台可以。。
    */
    // #if USE_WIN32DRV
    //     if (!lv_win32_init(
    //         GetModuleHandleW(NULL),
    //         SW_SHOW,
    //         800,
    //         480,
    //         LoadIconW(GetModuleHandleW(NULL), MAKEINTRESOURCE(IDI_LVGL_WINDOWS))))
    //     {
    //         return false;
    //     }
    //     lv_win32_add_all_input_devices_to_group(NULL);
    //     return true;
    // #endif

    LV_LOG_ERROR("no drv init");
    return false;
}

#if LV_TICK_CUSTOM

#include <time.h>
#include <sys/time.h>

// refer to https://github.com/lvgl/lv_port_linux_frame_buffer
// use this instead of periodically calling "lv_tick_inc()", so that no need let a new thread to run it
/* Set in lv_conf.h as `LV_TICK_CUSTOM_SYS_TIME_EXPR` */
// compatible with linux and win
uint32_t custom_tick_get(void)
{
    // #if USE_WIN32DRV
    //     return GetTickCount(); // from #include <windows.h>
    // #else
    static uint64_t start_ms = 0;
    if(start_ms == 0) {
        struct timeval tv_start;
        gettimeofday(&tv_start, NULL);
        start_ms = (tv_start.tv_sec * 1000000 + tv_start.tv_usec) / 1000;
    }
    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    uint64_t now_ms;
    now_ms = (tv_now.tv_sec * 1000000 + tv_now.tv_usec) / 1000;
    uint32_t time_ms = now_ms - start_ms;

    periodic_running_basic_inc(time_ms);
    
    return time_ms;
    // #endif
}
#endif

#if USE_SDL


// static int lv_task_thread(void* data)
// {
//     (void)data;
//     for(;;)
//     {
//         unsigned int delay_ms;
//         SDL_mutexP(sdl_mutex_lvgl);
//             delay_ms = lv_task_handler(); can not work..
//         SDL_mutexV(sdl_mutex_lvgl);
//         SDL_Delay(delay_ms);
//         lv_tick_inc(delay_ms);
//         periodic_running_basic_inc(delay_ms);
//     }
//     return 0;
// }

bool sdl_hal_init(void)
{
    /* Use the 'monitor' driver which creates window on PC's monitor to simulate a display*/
    sdl_init();

    /*
        lvgl 的 api 是 线程不安全的，每次在 线程里面 调用 lvgl 的 api，需要用同一个互斥锁包起来
        相关文档
            https://docs.lvgl.io/master/porting/os.html#os-interrupt
            https://www.bilibili.com/read/cv20086443/
        SDL 互斥锁使用
            https://blog.csdn.net/sz76211822/article/details/73649295
            https://blog.csdn.net/tonychan129/article/details/127580760
    */
    // 目前使用 pthread，不使用 SDL 创建线程
    // sdl_mutex_lvgl = SDL_CreateMutex();
    // if(!sdl_mutex_lvgl)
    // {
    //     LV_LOG_ERROR("SDL_CreateMutex failure");
    //     return false;
    // }

    // SDL_CreateThread(lv_task_thread, "lv_task", NULL);

    /*Create a display buffer*/
    static lv_disp_draw_buf_t disp_buf1;
    static lv_color_t buf1_1[SDL_HOR_RES * SDL_VER_RES];
    static lv_color_t buf1_2[SDL_HOR_RES * SDL_VER_RES];
    lv_disp_draw_buf_init(&disp_buf1, buf1_1, buf1_2, SDL_HOR_RES * SDL_VER_RES);

    /*Create a display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv); /*Basic initialization*/
    disp_drv.draw_buf = &disp_buf1;
    disp_drv.flush_cb = sdl_display_flush;
    disp_drv.hor_res = SDL_HOR_RES;
    disp_drv.ver_res = SDL_VER_RES;
    disp_drv.antialiasing = 1;
    disp_drv.sw_rotate = 0;
    lv_disp_t * disp = lv_disp_drv_register(&disp_drv);
        if(!disp) return false;

    /* theme */
    lv_theme_t * th = lv_theme_default_init(disp, lv_palette_main(LV_PALETTE_GREY), lv_palette_main(LV_PALETTE_BLUE), LV_THEME_DEFAULT_DARK, LV_FONT_DEFAULT);
        if(!th) return false;
    lv_disp_set_theme(disp, th);

    /* group */
    lv_group_t * g = lv_group_create();
        if(!g) return false;
    lv_group_set_default(g);

    /* Add a mouse as input device */
    static lv_indev_drv_t indev_drv_1;
    lv_indev_drv_init(&indev_drv_1);
    indev_drv_1.type = LV_INDEV_TYPE_POINTER;
    indev_drv_1.read_cb = sdl_mouse_read;
    lv_indev_t * ptr_indev = lv_indev_drv_register(&indev_drv_1);
        if(!ptr_indev) return false;

    /* Add a mousewheel as input device */
    static lv_indev_drv_t indev_drv_2;
    lv_indev_drv_init(&indev_drv_2);
    indev_drv_2.type = LV_INDEV_TYPE_ENCODER;
    indev_drv_2.read_cb = sdl_mousewheel_read;
    lv_indev_t * enc_indev = lv_indev_drv_register(&indev_drv_2);
        if(!enc_indev) return false;
    lv_indev_set_group(enc_indev, g);

    /* Add a keynoard as input device */
    static lv_indev_drv_t indev_drv_3;
    lv_indev_drv_init(&indev_drv_3);
    indev_drv_3.type = LV_INDEV_TYPE_KEYPAD;
    indev_drv_3.read_cb = sdl_keyboard_read;
    lv_indev_t *kb_indev = lv_indev_drv_register(&indev_drv_3);
        if(!kb_indev) return false;
    lv_indev_set_group(kb_indev, g);

    return true;
}

#endif // USE_SDL

#if USE_WINDOWS
#endif // USE_WINDOWS
