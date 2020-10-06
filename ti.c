#include "ls.h"
#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int alarm_hour = -1;
int alarm_min = -1;
bool alarm_status = 1;

void toggle_music();
void toggle_alarm();
void add_min();
void add_mins(int);
void add_hours(int);
void sub_min();

const char* dir;
char fp[100];
bool tc = false;

void toggle_music()
{
    music = !music;
}

void toggle_alarm()
{
    alarm_status = !alarm_status;
}

void add_min()
{
    tc = 1;
    add_mins(5);
}

void sub_min()
{
    tc = 1;
    add_mins(-5);
}

//the alarm_status increase functionallity
void add_mins(int amount)
{
    alarm_min += amount;
    if (alarm_min > 59) {
        add_hours(1);
        alarm_min = alarm_min % 60;
    }
    if (alarm_min < 0) {
        add_hours(-1);
        alarm_min = 60 + alarm_min;
    }
}

void add_hours(int amount)
{
    alarm_hour += amount;
    if (alarm_hour > 23)
        alarm_hour = 0;
    if (alarm_hour < 0)
        alarm_hour = 23;
}

void time_settings(){
    while(1){
        sleep(60);
        if(tc){
            sleep(60);

            FILE* f = fopen(fp, "w");
            if (f == NULL) {
                fprintf(stderr, "Cant Open Config File under: %s", fp);
                exit(-1);
            }
            fprintf(f,"%u:%u\n", alarm_hour, alarm_min);
            fclose(f);
            tc = false;
        }
    }
}

void time_setup()
{
    alarm_status = 1;
    if ((dir = getenv("XDG_CONFIG_HOME")) == NULL) {
        if ((dir = getenv("HOME")) == NULL) {
            dir = getpwuid(getuid())->pw_dir;
        }
        strcpy(fp, dir);
        strcat(fp, "/.config/");
    } else {
        strcpy(fp, dir);
    }
    strcat(fp, "alarm.conf");
    FILE* f = fopen(fp, "r");
    if (f == NULL) {
        fprintf(stderr, "Cant Open Config File under: %s", fp);
        exit(-1);
    }
    fscanf(f, "%u:%u\n", &alarm_hour, &alarm_min);
    fclose(f);
}