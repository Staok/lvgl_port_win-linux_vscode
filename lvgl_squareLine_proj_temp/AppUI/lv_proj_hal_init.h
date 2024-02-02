#ifndef LV_PROJ_HAL_INIT_H
#define LV_PROJ_HAL_INIT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lv_conf.h"

#ifndef LV_DRV_NO_CONF
#ifdef LV_CONF_INCLUDE_SIMPLE
#include "lv_drv_conf.h"
#else
#include "../../lv_drv_conf.h"
#endif
#endif

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

// #include <stdio.h>
// #include <stdlib.h>
#include <stdint.h>
#include <unistd.h> // for usleep() .etc

int32_t lv_proj_hal_init(void);
extern bool lvgl_exit_flag;

void lv_proj_hal_deinit(void);

void lv_demosAndExamples_show();

#ifdef __cplusplus
} /* extern "C" */
#endif

int8_t test(void);

#endif
