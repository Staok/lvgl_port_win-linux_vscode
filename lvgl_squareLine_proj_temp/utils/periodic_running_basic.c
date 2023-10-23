#include "periodic_running_basic.h"

unsigned int count_ms = 0;

char is_50ms    = 0;
char is_100ms   = 0;
char is_500ms   = 0;
char is_1s      = 0;

void periodic_running_basic_inc(unsigned int inc_ms)
{
    count_ms += inc_ms;

    if((count_ms % 50) == 0) is_50ms = 1;
    if((count_ms % 100) == 0) is_100ms = 1;
    if((count_ms % 500) == 0) is_500ms = 1;
    if((count_ms % 1000) == 0) is_1s = 1;
}

char get_is_50ms() { return is_50ms; }
void clear_is_50ms_flag() { is_50ms = 0; }

char get_is_100ms() { return is_100ms; }
void clear_is_100ms_flag() { is_100ms = 0; }

char get_is_500ms() { return is_500ms; }
void clear_is_500ms_flag() { is_500ms = 0; }

char get_is_1s() { return is_1s; }
void clear_is_1s_flag() { is_1s = 0; }
