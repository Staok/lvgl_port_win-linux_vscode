#pragma once

#include <stdint.h>
#include <iostream>

#include "proj_conf.h"
#include "lv_proj_hal_init.h"

#include "app_temp.h"
#include "main_page.h"

#ifdef __cplusplus
extern "C" {
#endif

    #ifdef LV_LVGL_H_INCLUDE_SIMPLE
    #include "lvgl.h"
    #else
    #include "lvgl/lvgl.h"
    #endif
    #include "ui.h"

#ifdef __cplusplus
} /* extern "C" */
#endif

int32_t proj_init(void);
