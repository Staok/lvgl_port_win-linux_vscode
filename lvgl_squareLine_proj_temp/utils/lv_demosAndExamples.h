#ifndef LVGL_DEMOS_AND_EXAMPLES_H
#define LVGL_DEMOS_AND_EXAMPLES_H

#include "lv_conf.h"

#if LV_USE_DEMOS
    #include "lv_demos.h"
#endif

#if LV_BUILD_EXAMPLES
    #include "lv_examples.h"
#endif

void lvgl_demosAndExamples_show();

#endif