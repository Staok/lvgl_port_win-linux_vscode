#include "lv_demosAndExamples.h"

#if LV_USE_DEMOS || LV_BUILD_EXAMPLES

void lvgl_demosAndExamples_show()
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

#endif