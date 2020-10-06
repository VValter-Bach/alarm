#include "digits.h"
#include "ls.h"
#include "pins.h"
#include "ti.h"
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <pthread.h>

void setup();

int main()
{
    pthread_t light_sound;
    pthread_t digits;
    pthread_t settings;
    setup();

    wiringPiISR(INPUT_MUSIC, INT_EDGE_FALLING, &(toggle_music));
    wiringPiISR(INPUT_MINUS, INT_EDGE_FALLING, &(add_min));
    wiringPiISR(INPUT_ALARM, INT_EDGE_FALLING, &(toggle_alarm));
    wiringPiISR(INPUT_PLUS, INT_EDGE_FALLING, &(sub_min));

    pthread_create(&settings, NULL, (void*)time_settings, NULL);
    pthread_create(&light_sound, NULL, (void* (*)(void*))ls, NULL);
    pthread_create(&digits, NULL, (void* (*)(void*))display, NULL);
    pthread_join(digits, NULL);
}

void setup()
{
    wiringPiSetup();

    if (wiringPiSPISetup(0, 800000) == -1) {
        exit(1);
    }

    pinMode(SEGMENT1, OUTPUT);
    pinMode(SEGMENT2, OUTPUT);
    pinMode(SEGMENT3, OUTPUT);
    pinMode(SEGMENT4, OUTPUT);
    pinMode(SEGMENT5, OUTPUT);
    pinMode(SEGMENT6, OUTPUT);
    pinMode(SEGMENT7, OUTPUT);
    pinMode(SEGMENT8, OUTPUT);

    pinMode(DIGIT1, OUTPUT);
    pinMode(DIGIT2, OUTPUT);
    pinMode(DIGIT3, OUTPUT);
    pinMode(DIGIT4, OUTPUT);
    pinMode(DIGIT5, OUTPUT);
    pinMode(DIGIT6, OUTPUT);
    pinMode(DIGIT7, OUTPUT);
    pinMode(DIGIT8, OUTPUT);

    pinMode(ALARM, OUTPUT);
    pinMode(WEEKEND, OUTPUT);
    pinMode(MUSIC, OUTPUT);

    ls_setup();
    time_setup();
}