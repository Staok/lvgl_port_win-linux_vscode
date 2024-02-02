#ifndef BTN_CB_TEMP_H
#define BTN_CB_TEMP_H

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

class BtnCbTemp
{
public:
    explicit BtnCbTemp(void);
    virtual ~BtnCbTemp(void);
    static BtnCbTemp & inst();

private:

};

#endif // BTN_CB_TEMP_H
