#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h> 

extern bool alarm_status;
extern int alarm_hour; 
extern int alarm_min; 

void toggle_music();
void toggle_alarm();
void add_min();
void sub_min();
void time_setup();
void time_settings();

#endif // INPUT_H