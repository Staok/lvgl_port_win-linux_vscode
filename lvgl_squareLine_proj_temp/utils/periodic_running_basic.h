#ifndef PERIODIC_RUNNING_BASIC_H
#define PERIODIC_RUNNING_BASIC_H

#define PERIODIC_TIME_IN_MS 5

// extern unsigned int count_ms;

// extern char is_50ms;
// extern char is_100ms;
// extern char is_500ms;
// extern char is_1s;

void periodic_running_basic_inc(unsigned int inc_ms);

char get_is_50ms();
void clear_is_50ms_flag();

char get_is_100ms();
void clear_is_100ms_flag();

char get_is_500ms();
void clear_is_500ms_flag();

char get_is_1s();
void clear_is_1s_flag();

#endif
