#include "app_temp.h"

AppTemp::AppTemp(void)
{
    std::cout << " - AppTemp say hello~" << std::endl;
}

AppTemp::~AppTemp(void)
{
    std::cout << " - AppTemp say bye~" << std::endl;
}

AppTemp & AppTemp::inst()
{
    static AppTemp AppTemp_;
    return AppTemp_;
}

void AppTemp::home_btn_exec(void)
{
    std::cout << " - AppTemp say Home btn exec~" << std::endl;
    // do things
}

