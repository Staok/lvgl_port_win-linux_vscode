#ifndef APP_TEMP_H
#define APP_TEMP_H

#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif

    #include <stdio.h>
    #include <stdlib.h>
    #include <stdint.h>

#ifdef __cplusplus
} /* extern "C" */
#endif

class AppTemp
{
public:
    explicit AppTemp(void);
    virtual ~AppTemp(void);
    static AppTemp & inst();

    void home_btn_exec(void);

private:

};

#endif // APP_TEMP_H
