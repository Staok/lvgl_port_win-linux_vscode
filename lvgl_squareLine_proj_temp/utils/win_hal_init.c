#include "win_hal_init.h"
#include "periodic_running_basic.h"

#include <stdio.h>

static int tick_thread(void *data)
{
    (void)data;
    while(1) {
        SDL_Delay(PERIODIC_TIME_IN_MS);
        lv_tick_inc(PERIODIC_TIME_IN_MS);
        periodic_running_basic_inc(PERIODIC_TIME_IN_MS);
    }
    return 0;
}

/*  
    tabView 没有动画过渡了
    在 lv_tabView.c 里面，将 LV_ANIM_OFF 都替换成 LV_ANIM_ON 即可~~
    按需要改吧，其它控件估计也有默认设置为不动画过渡的问题，应该都是源码里写死的，按需改源码吧..尽量不

*/
bool win_hal_init(void)
{
    /* Use the 'monitor' driver which creates window on PC's monitor to simulate a display*/
    sdl_init();

    /*
        手动加上了：
        SDL_CreateThread(tick_thread, "tick", NULL);
        和 static int tick_thread(void *data)，才能正常跑
    */
    SDL_CreateThread(tick_thread, "tick", NULL);

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