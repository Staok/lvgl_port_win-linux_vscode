#ifndef ALL_EVENT_CB_FUN_H
#define ALL_EVENT_CB_FUN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#include "proj_conf.h"

/****************************************************
 *  main page callback function
 ****************************************************/
void ui_btn_1_cb(lv_event_t * e);
void ui_switch_checkbox_1_cb(lv_event_t * e);
void ui_choose_cb(lv_event_t * e);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // ALL_EVENT_CB_FUN_H
