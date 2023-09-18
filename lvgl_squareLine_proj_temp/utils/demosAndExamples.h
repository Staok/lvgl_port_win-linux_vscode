#include "../screen/lv_conf.h"

    #if LV_USE_DEMOS || LV_BUILD_EXAMPLES

        #ifndef LVGL_DEMOS_AND_EXAMPLES_H
        #define LVGL_DEMOS_AND_EXAMPLES_H

        #if LV_USE_DEMOS
            #include "../screen/lvgl/demos/lv_demos.h"
        #endif

        #if LV_BUILD_EXAMPLES
            #include "../screen/lvgl/examples/lv_examples.h"
        #endif

        void lvgl_demosAndExamples_show();

    #endif

#endif