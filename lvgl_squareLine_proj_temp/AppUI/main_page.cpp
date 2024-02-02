#include "main_page.h"
#include "all_event_cb_fun.h"

#include "app_temp.h"

BtnCbTemp::BtnCbTemp(void)
{
    std::cout << " - BtnCbTemp say hello~" << std::endl;
}

BtnCbTemp::~BtnCbTemp(void)
{
    std::cout << " - BtnCbTemp say bye~" << std::endl;
}

BtnCbTemp & BtnCbTemp::inst()
{
    static BtnCbTemp BtnCbTemp_;
    return BtnCbTemp_;
}

/****************************************************
 *  main page callback function
 ****************************************************/
void ui_btn_1_cb(lv_event_t * e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t * obj = lv_event_get_target(e);
	void * user_data = lv_event_get_user_data(e);
    (void)code;
	(void)obj;
	(void)user_data;
    LV_LOG_USER("nozzleTempBtnClickedEventFun() LV_EVENT_CLICKED");

    AppTemp::inst().home_btn_exec();
}

void ui_switch_checkbox_1_cb(lv_event_t * e)
{
    bool isOn = lv_obj_has_state(lv_event_get_target(e), LV_STATE_CHECKED);
    const char* str = isOn ? "On" : "Off";
    LV_LOG_USER("onfilamentSwitchChanged() LV_EVENT_VALUE_CHANGED %s", str);
}

void ui_choose_cb(lv_event_t * e)
{
    (void)e;
}

