#include "lv_proj_hal_init.h"

#include <pthread.h>

#ifdef __linux__

    #define DISP_HOR_RES 1280
    #define DISP_VER_RES 720

    #if USE_DRM
        #include "lv_drivers/display/drm.h"
        int8_t drm_hal_init_with_evdev(void);
    #endif

    #if USE_FBDEV
        #include "lv_drivers/display/fbdev.h"
        int8_t fbdev_hal_init_with_evdev(void);
    #endif

    #if USE_EVDEV
        #include "lv_drivers/indev/evdev.h"
    #endif

#elif _WIN32

    #if USE_SDL
        #define SDL_MAIN_HANDLED /*To fix SDL's "undefined reference to WinMain" issue*/
        #include <SDL2/SDL.h>
        #include "lv_drivers/sdl/sdl.h"
        int8_t sdl_hal_init(void);
    #endif

    #if USE_WINDOWS // disp update slower than SDL2...
        #include "lv_drivers/win_drv.h"
    #endif

#endif

/* ---------------- test code ---------------- */

#include <iostream>

int8_t test(void)
{
    std::cout << " - test begin" << std::endl;

    std::cout << " - test end" << std::endl;

    return 0;
}

/* ------------- hal init -------------- */

static pthread_mutex_t lv_task_thread_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_t lv_task_thread_tid;

#define PERIODIC_TIME_IN_MS 10

bool lvgl_exit_flag = false;

static void *lv_task_thread(void *in_arg)
{
    (void)in_arg;
    pthread_detach(pthread_self());
    LV_LOG_USER("run lv_task_thread()");
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
    LV_LOG_USER("exit lv_task_thread()");
    pthread_exit(0);
    return NULL;
}

int32_t lv_proj_hal_init(void)
{
    // 先到 CMakeLists.txt 或 lv_drv_conf.h 里面打开相应的 option 或 macro
    //  对于 win，CMakeLists.txt 里有 USE_WIN_SDL2LIB 或 USE_WIN_WINDRV，或 lv_drv_conf.h 里有 USE_SDL 或 USE_WINDOWS，选择一个，推荐使用 SDL2
    //  对于 linux，CMakeLists.txt 里有 USE_LINUX_DRM 或 USE_LINUX_FB，或 lv_drv_conf.h 里面同理，选择一个 做显示用

    int32_t ret = -1;
    ret = pthread_create(&lv_task_thread_tid, NULL, lv_task_thread, NULL);
    if(ret < 0) { LV_LOG_ERROR("pthread_create() failure"); return ret; }

    lvgl_exit_flag = false;

    #ifdef __linux__

        #if USE_DRM
            ret = drm_hal_init_with_evdev();
            if(ret < 0) {
                LV_LOG_ERROR("drm_hal_init_with_evdev() err, ret: %d", ret);
                return -1;
            } else {
                LV_LOG_USER("drm_hal_init_with_evdev() ok");
            }
        #endif

        #if USE_FBDEV
            ret = fbdev_hal_init_with_evdev();
            if(ret < 0) {
                LV_LOG_ERROR("fbdev_hal_init_with_evdev() err, ret: %d", ret);
                return -2;
            } else {
                LV_LOG_USER("fbdev_hal_init_with_evdev() ok");
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
            }
        #endif

        #if USE_WINDOWS
            void* rett = NULL;
            rett = windrv_init();
            if(!rett) {
                LV_LOG_ERROR("windrv_init() err");
                return -2;
            } else {
                LV_LOG_USER("windrv_init() ok");
            }
        #endif

    #endif

    return 0;
}

void lv_proj_hal_deinit(void)
{
    #ifdef __linux__

        #if USE_DRM
            drm_exit();
        #endif

        #if USE_FBDEV
            fbdev_exit();
        #endif

        return ;

    #endif
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

int8_t drm_hal_init_with_evdev(void)
{
    drm_init();

    lv_coord_t width;
    lv_coord_t height;
    uint32_t dpi;
    drm_get_sizes(&width, &height, &dpi);
    LV_LOG_USER("drm_get_sizes() width: %d, height: %d, dpi: %d", width, height, dpi);

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
    disp_drv.sw_rotate = 0;
    disp_drv.rotated = LV_DISP_ROT_NONE; // LV_DISP_ROT_90
    lv_disp_t * disp = lv_disp_drv_register(&disp_drv);
        if(!disp) return -1;

    // lv_theme_t * th = lv_theme_default_init( disp,
    //         lv_palette_main(LV_PALETTE_GREY),
    //         lv_palette_main(LV_PALETTE_BLUE),
    //         LV_THEME_DEFAULT_DARK, LV_FONT_DEFAULT
    //     );
    //     if(!th) return -2;
    // lv_disp_set_theme(disp, th);

    // must put disp init and indev init together, or fault
    #if USE_EVDEV
        evdev_init();
        static lv_indev_drv_t indev_drv_1;
        lv_indev_drv_init(&indev_drv_1); /*Basic initialization*/
        indev_drv_1.type = LV_INDEV_TYPE_POINTER;
        indev_drv_1.read_cb = evdev_read;
        lv_indev_t *mouse_indev = lv_indev_drv_register(&indev_drv_1);
            if(!mouse_indev) return -3;
    #endif // USE_EVDEV

    return 0;
}

#endif // USE_DRM

#if USE_FBDEV

int8_t fbdev_hal_init_with_evdev(void)
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
    disp_drv.sw_rotate = 0;
    disp_drv.rotated = LV_DISP_ROT_NONE;
    lv_disp_t * disp = lv_disp_drv_register(&disp_drv);
        if(!disp) return -1;

    // lv_theme_t * th = lv_theme_default_init( disp,
    //         lv_palette_main(LV_PALETTE_GREY),
    //         lv_palette_main(LV_PALETTE_BLUE),
    //         LV_THEME_DEFAULT_DARK, LV_FONT_DEFAULT
    //     );
    //     if(!th) return -2;
    // lv_disp_set_theme(disp, th);

    // must put disp init and indev init together, or fault
    #if USE_EVDEV
        evdev_init();
        static lv_indev_drv_t indev_drv_1;
        lv_indev_drv_init(&indev_drv_1); /*Basic initialization*/
        indev_drv_1.type = LV_INDEV_TYPE_POINTER;
        indev_drv_1.read_cb = evdev_read;
        lv_indev_t *mouse_indev = lv_indev_drv_register(&indev_drv_1);
            if(!mouse_indev) return -3;
    #endif // USE_EVDEV

    return 0;
}

#endif // USE_FBDEV

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

#if LV_USE_DEMOS
    #include "lv_demos.h"
#endif

#if LV_BUILD_EXAMPLES
    #include "lv_examples.h"
#endif

void lv_demosAndExamples_show()
{
    /*
        open LV_BUILD_EXAMPLES or LV_USE_DEMOS in lv_conf.h to use these fun
    */

    /* --------------------- demos ------------------------- */

#if LV_USE_DEMOS

    #if LV_USE_DEMO_WIDGETS
        lv_demo_widgets();
    #endif

    /*Demonstrate the usage of encoder and keyboard*/
    #if LV_USE_DEMO_KEYPAD_AND_ENCODER
        // lv_demo_keypad_encoder();
    #endif

    /*Benchmark your system*/
    #if LV_USE_DEMO_BENCHMARK
        // lv_demo_benchmark();
    #endif

    /*Stress test for LVGL*/
    #if LV_USE_DEMO_STRESS
        // lv_demo_stress();
    #endif

    /*Music player demo*/
    #if LV_USE_DEMO_MUSIC
        // lv_demo_music();
    #endif

#endif

    /* -------------------- examples------------------------ */
   
#if LV_BUILD_EXAMPLES

    /* --- scroll --- */

    // lv_example_scroll_1();
    // lv_example_scroll_2();
    // lv_example_scroll_3();
    // lv_example_scroll_4();
    // lv_example_scroll_5();
    // lv_example_scroll_6();

    /* --- layouts: flex --- */

    // lv_example_flex_1();
    // lv_example_flex_2();
    // lv_example_flex_3();
    // lv_example_flex_4();
    // lv_example_flex_5();
    // lv_example_flex_6();

    /* --- layouts: grid --- */
    // lv_example_grid_1();
    // lv_example_grid_2();
    // lv_example_grid_3();
    // lv_example_grid_4();
    // lv_example_grid_5();
    // lv_example_grid_6();

    /* --- style --- */
    // lv_example_style_1();
    // lv_example_style_2();
    // lv_example_style_3();
    // lv_example_style_4();
    // lv_example_style_5();
    // lv_example_style_6();
    // lv_example_style_7();
    // lv_example_style_8();
    // lv_example_style_9();
    // lv_example_style_10();
    // lv_example_style_11();
    // lv_example_style_12();
    // lv_example_style_13();
    // lv_example_style_14();
    // lv_example_style_15();

    /* --- anim --- */
    // lv_example_anim_1();
    // lv_example_anim_2();
    // lv_example_anim_3();
    // lv_example_anim_timeline_1();

    /* --- event --- */
    // lv_example_event_1();
    // lv_example_event_2();
    // lv_example_event_3();
    // lv_example_event_4();

    /* --- extra examples in "libs" dir --- 
        bmp
        ffmpeg
        freetype
        gif
        png
        qrcode
        rlottie
        sjpg */

    /* --- extra examples in "others" dir --- 
        fragment
        gridnav
        ime
        imgfont
        monkey
        msg
        snapshot */

    /* --- widgets --- */

    // lv_example_arc_1();
    // lv_example_arc_2();

    // lv_example_animimg_1();

    // lv_example_bar_1();
    // lv_example_bar_2();
    // lv_example_bar_3();
    // lv_example_bar_4();
    // lv_example_bar_5();
    // lv_example_bar_6();

    // lv_example_btn_1();
    // lv_example_btn_2();
    // lv_example_btn_3();

    // lv_example_btnmatrix_1();
    // lv_example_btnmatrix_2();
    // lv_example_btnmatrix_3();

    // lv_example_calendar_1();

    // lv_example_canvas_1();
    // lv_example_canvas_2();

    // lv_example_chart_1();
    // lv_example_chart_2();
    // lv_example_chart_3();
    // lv_example_chart_4();
    // lv_example_chart_5();
    // lv_example_chart_6();
    // lv_example_chart_7();
    // lv_example_chart_8();
    // lv_example_chart_9();

    // lv_example_checkbox_1();
    // lv_example_checkbox_2();

    // lv_example_colorwheel_1();

    // lv_example_dropdown_1();
    // lv_example_dropdown_2();
    // lv_example_dropdown_3();

    // lv_example_img_1();
    // lv_example_img_2();
    // lv_example_img_3();
    // lv_example_img_4();

    // lv_example_imgbtn_1();

    // lv_example_keyboard_1();

    // lv_example_label_1();
    // lv_example_label_2();
    // lv_example_label_3();
    // lv_example_label_4();
    // lv_example_label_5();

    // lv_example_led_1();

    // lv_example_line_1();

    // lv_example_list_1();
    // lv_example_list_2();

    // lv_example_menu_1();
    // lv_example_menu_2();
    // lv_example_menu_3();
    // lv_example_menu_4();
    // lv_example_menu_5();

    // lv_example_meter_1();
    // lv_example_meter_2();
    // lv_example_meter_3();
    // lv_example_meter_4();

    // lv_example_msgbox_1();

    // lv_example_obj_1();
    // lv_example_obj_2();

    // lv_example_roller_1();
    // lv_example_roller_2();
    // lv_example_roller_3();

    // lv_example_slider_1();
    // lv_example_slider_2();
    // lv_example_slider_3();

    // lv_example_spinbox_1();

    // lv_example_spinner_1();

    // lv_example_switch_1();

    // lv_example_table_1();
    // lv_example_table_2();

    // lv_example_tabview_1();
    // lv_example_tabview_2();

    // lv_example_textarea_1();
    // lv_example_textarea_2();
    // lv_example_textarea_3();

    // lv_example_tileview_1();

    // lv_example_win_1();

    // lv_example_span_1();
#endif

}


