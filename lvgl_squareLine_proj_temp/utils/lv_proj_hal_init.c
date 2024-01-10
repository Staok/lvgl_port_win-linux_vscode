#include "lv_proj_hal_init.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static pthread_mutex_t lv_task_thread_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_t lv_task_thread_tid;

#define PERIODIC_TIME_IN_MS 10

bool lvgl_exit_flag = false;

static void *lv_task_thread(void *in_arg)
{
    (void)in_arg;
    pthread_detach(pthread_self());
    LV_LOG_USER("run lv_task_thread()~");
    for(;;)
    {

        #if !LV_TICK_CUSTOM
            pthread_mutex_lock(&lv_task_thread_mutex);
                lv_tick_inc(PERIODIC_TIME_IN_MS);
            pthread_mutex_unlock(&lv_task_thread_mutex);
        #endif

        #if USE_SDL
            // the "sdl_quit_qry" is in "sdl_common.c"
            if(sdl_quit_qry)
            {
                lvgl_exit_flag = true;
                break;
            }
        #endif

        #if USE_WINDOWS
            // the "lv_win_exit_flag" is in "win_drv.c"
            if(lv_win_exit_flag)
            {
                lvgl_exit_flag = true;
                break;
            }
        #endif

        usleep(PERIODIC_TIME_IN_MS * 1000);

    }
    LV_LOG_USER("exit lv_task_thread()~");
    pthread_exit(0);
    return NULL;
}

int8_t lv_proj_hal_init(void)
{
    // 先到 CMakeLists.txt 或 lv_drv_conf.h 里面打开相应的 option 或 macro
    //  对于 win，CMakeLists.txt 里有 USE_WIN_SDL2LIB 或 USE_WIN_WINDRV，或 lv_drv_conf.h 里有 USE_SDL 或 USE_WINDOWS，选择一个，推荐使用 SDL2
    //  对于 linux，CMakeLists.txt 里有 USE_LINUX_DRM 或 USE_LINUX_FB，或 lv_drv_conf.h 里面同理，选择一个 做显示用

    int ret = -1;
    ret = pthread_create(&lv_task_thread_tid, NULL, lv_task_thread, NULL);
        if(ret < 0) { LV_LOG_ERROR("pthread_create() failure"); return ret; }

    lvgl_exit_flag = false;

    #ifdef __linux__

        #if USE_DRM
            ret = drm_hal_init();
            if(ret < 0) {
                LV_LOG_ERROR("drm_hal_init() err, ret: %d", ret);
                return -1;
            } else {
                LV_LOG_USER("drm_hal_init() ok");
                return 0;
            }
        #endif

        #if USE_FBDEV
            ret = fbdev_hal_init();
            if(ret < 0) {
                LV_LOG_ERROR("fbdev_hal_init() err, ret: %d", ret);
                return -2;
            } else {
                LV_LOG_USER("fbdev_hal_init() ok");
                return 0;
            }
        #endif

        #if USE_EVDEV
            ret = evdev_hal_init();
            if(ret < 0) {
                LV_LOG_ERROR("evdev_hal_init() err, ret: %d", ret);
                return -3;
            } else {
                LV_LOG_USER("evdev_hal_init() ok");
                return 0;
            }
        #endif

    #elif _WIN32

        #if USE_SDL
            ret = sdl_hal_init();
            if(ret < 0) {
                LV_LOG_ERROR("sdl_hal_init() err, ret: %d", ret);
                return -1;
            } else {
                LV_LOG_USER("sdl_hal_init() ok");
                return 0;
            }
        #endif

        #if USE_WINDOWS
            ret = windrv_init();
            if(ret < 0) {
                LV_LOG_ERROR("windrv_init() err, ret: %d", ret);
                return -2;
            } else {
                LV_LOG_USER("windrv_init() ok");
                return 0;
            }
        #endif

    #endif

    LV_LOG_ERROR("no drv init");
    ret = -10;
    return ret;
}

#if LV_TICK_CUSTOM

// lv_conf.h 里面不打开 "LV_TICK_CUSTOM" 时，即 使用 线程 lv_task_thread() 里面 运行 lv_tick_inc()
// lv_conf.h 里面打开 "LV_TICK_CUSTOM" 时，即使用 custom_tick_get() 返回当前 ms 数
//  此时 lv_tick_inc() 就不能用了，在 lv_task_thread() 里面的注释掉

#include <time.h>
#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif
// refer to https://github.com/lvgl/lv_port_linux_frame_buffer
// use this instead of periodically calling "lv_tick_inc()", so that no need let a new thread to run it
/* Set in lv_conf.h as `LV_TICK_CUSTOM_SYS_TIME_EXPR` */
// compatible with linux and win
uint32_t custom_tick_get(void)
{
    // #if USE_WINDOWS
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
    return time_ms;
    // #endif
}
#ifdef __cplusplus
}
#endif

#endif

#ifdef __linux__

#if USE_DRM

int8_t drm_hal_init(void)
{
    drm_init();

    lv_coord_t width;
    lv_coord_t height;
    uint32_t dpi;
    drm_get_sizes(&width, &height, &dpi);
    LV_LOG_INFO("drm_get_sizes() width: %d, height: %d, dpi: %d", width, height, dpi);

    static lv_disp_draw_buf_t disp_buf;
    static lv_color_t buf1[DISP_HOR_RES * DISP_VER_RES];
    static lv_color_t buf2[DISP_HOR_RES * DISP_VER_RES];
    lv_disp_draw_buf_init(&disp_buf, buf1, buf2, DISP_HOR_RES * DISP_VER_RES);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf = &disp_buf;
    disp_drv.flush_cb = drm_flush;
    disp_drv.hor_res = DISP_HOR_RES;
    disp_drv.ver_res = DISP_VER_RES;
    disp_drv.antialiasing = 1;
    disp_drv.full_refresh = 0;
    disp_drv.screen_transp = 0;
    disp_drv.direct_mode = 0;
    disp_drv.sw_rotate = 1;
    disp_drv.rotated = LV_DISP_ROT_90;
    lv_disp_t * disp = lv_disp_drv_register(&disp_drv);
        if(!disp) return -1;

    // lv_theme_t * th = lv_theme_default_init( disp,
    //         lv_palette_main(LV_PALETTE_GREY),
    //         lv_palette_main(LV_PALETTE_BLUE),
    //         LV_THEME_DEFAULT_DARK, LV_FONT_DEFAULT
    //     );
    //     if(!th) return -2;
    // lv_disp_set_theme(disp, th);

    return 0;
}

#endif // USE_DRM

#if USE_FBDEV

int8_t fbdev_hal_init(void)
{
    fbdev_init();

    uint32_t width;
    uint32_t height;
    uint32_t dpi;
    fbdev_get_sizes(&width, &height, &dpi);
    LV_LOG_INFO("fbdev_get_sizes() width: %d, height: %d, dpi: %d", width, height, dpi);

    static lv_disp_draw_buf_t disp_buf;
    static lv_color_t buf1[DISP_HOR_RES * DISP_VER_RES];
    static lv_color_t buf2[DISP_HOR_RES * DISP_VER_RES];
    lv_disp_draw_buf_init(&disp_buf, buf1, buf2, DISP_HOR_RES * DISP_VER_RES);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf = &disp_buf;
    disp_drv.flush_cb = fbdev_flush;
    disp_drv.hor_res = DISP_HOR_RES;
    disp_drv.ver_res = DISP_VER_RES;
    disp_drv.antialiasing = 1;
    disp_drv.full_refresh = 0;
    disp_drv.screen_transp = 0;
    disp_drv.direct_mode = 0;
    disp_drv.sw_rotate = 1;
    disp_drv.rotated = LV_DISP_ROT_90;
    lv_disp_t * disp = lv_disp_drv_register(&disp_drv);
        if(!disp) return -1;

    // lv_theme_t * th = lv_theme_default_init( disp,
    //         lv_palette_main(LV_PALETTE_GREY),
    //         lv_palette_main(LV_PALETTE_BLUE),
    //         LV_THEME_DEFAULT_DARK, LV_FONT_DEFAULT
    //     );
    //     if(!th) return -2;
    // lv_disp_set_theme(disp, th);

    return 0;
}

#endif // USE_FBDEV

#if USE_EVDEV

int8_t evdev_hal_init()
{
    evdev_init();
    static lv_indev_drv_t indev_drv_1;
    lv_indev_drv_init(&indev_drv_1); /*Basic initialization*/
    indev_drv_1.type = LV_INDEV_TYPE_POINTER;
    indev_drv_1.read_cb = evdev_read;
    lv_indev_t *mouse_indev = lv_indev_drv_register(&indev_drv_1);
        if(!mouse_indev) return -1;
}

#endif // USE_EVDEV

#elif _WIN32

#if USE_SDL

int8_t sdl_hal_init(void)
{
    /* Use the 'monitor' driver which creates window on PC's monitor to simulate a display*/
    sdl_init();

    /*Create a display buffer*/
    static lv_disp_draw_buf_t disp_buf;
    static lv_color_t buf_1[SDL_HOR_RES * SDL_VER_RES];
    static lv_color_t buf_2[SDL_HOR_RES * SDL_VER_RES];
    lv_disp_draw_buf_init(&disp_buf, buf_1, buf_2, SDL_HOR_RES * SDL_VER_RES);

    /*Create a display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv); /*Basic initialization*/
    disp_drv.draw_buf = &disp_buf;
    disp_drv.flush_cb = sdl_display_flush;
    disp_drv.hor_res = SDL_HOR_RES;
    disp_drv.ver_res = SDL_VER_RES;
    disp_drv.antialiasing = 1;
    disp_drv.full_refresh = 0;
    disp_drv.screen_transp = 0;
    disp_drv.direct_mode = 0;
    disp_drv.sw_rotate = 0;
    disp_drv.rotated = LV_DISP_ROT_NONE;
    lv_disp_t * disp = lv_disp_drv_register(&disp_drv);
        if(!disp) return -1;

    /* theme */
    // lv_theme_t * th = lv_theme_default_init( disp,
    //         lv_palette_main(LV_PALETTE_GREY),
    //         lv_palette_main(LV_PALETTE_BLUE),
    //         LV_THEME_DEFAULT_DARK, LV_FONT_DEFAULT
    //     );
    //     if(!th) return -2;
    // lv_disp_set_theme(disp, th);

    /* group */
    lv_group_t * g = lv_group_create();
        if(!g) return -3;
    lv_group_set_default(g);

    /* Add a mouse as input device */
    static lv_indev_drv_t indev_drv_1;
    lv_indev_drv_init(&indev_drv_1);
    indev_drv_1.type = LV_INDEV_TYPE_POINTER;
    indev_drv_1.read_cb = sdl_mouse_read;
    lv_indev_t * ptr_indev = lv_indev_drv_register(&indev_drv_1);
        if(!ptr_indev) return -4;

    // add mouse pointer a image, use lvgl internal image
    // lv_obj_t * mouse_cursor = lv_img_create(lv_scr_act());
    // lv_img_set_src(mouse_cursor, LV_SYMBOL_HOME);
    // lv_indev_set_cursor(ptr_indev, mouse_cursor);

    /* Add a mousewheel as input device */
    static lv_indev_drv_t indev_drv_2;
    lv_indev_drv_init(&indev_drv_2);
    indev_drv_2.type = LV_INDEV_TYPE_ENCODER;
    indev_drv_2.read_cb = sdl_mousewheel_read;
    lv_indev_t * enc_indev = lv_indev_drv_register(&indev_drv_2);
        if(!enc_indev) return -5;
    lv_indev_set_group(enc_indev, g);

    /* Add a keynoard as input device */
    static lv_indev_drv_t indev_drv_3;
    lv_indev_drv_init(&indev_drv_3);
    indev_drv_3.type = LV_INDEV_TYPE_KEYPAD;
    indev_drv_3.read_cb = sdl_keyboard_read;
    lv_indev_t *kb_indev = lv_indev_drv_register(&indev_drv_3);
        if(!kb_indev) return -6;
    lv_indev_set_group(kb_indev, g);

    /* use "lv_group_add_obj(group, obj)" to add widget objs to group 
        to let indevs in this group can control it */

    return 0;
}

#endif // USE_SDL

#if USE_WINDOWS
#endif // USE_WINDOWS

#endif // _WIN32


