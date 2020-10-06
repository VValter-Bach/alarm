#include "digits.h"
#include "ls.h"
#include "pins.h"
#include "ti.h"
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <wiringPi.h>

#define SLEEP 2222

// Pin Constellation of Number: 0,         1,             2,             3,            4,             5,             6,             7,              8,              9,
bool pinvalues[70] = { 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1 };
// Array of Segments in Order to fit Above
char pins[8] = { SEGMENT1, SEGMENT2, SEGMENT3, SEGMENT4, SEGMENT5, SEGMENT6, SEGMENT7, SEGMENT8 };

void display();
void nom(int, bool);
void num_on_digit(int);
void num_on_digit_point(int);
void select_digit(int);
void time_display(int, int);
void status_display(int);

void display()
{

    time_t rt; // Raw Time in Seconds from 1970
    struct tm* ti; // Stuctured Time Information

    while (1) {
        time(&rt);
        ti = localtime(&rt);
        time_display(ti->tm_min, ti->tm_hour);
        if (ti->tm_min == alarm_min && ti->tm_hour == alarm_hour) music = 1;
        status_display(ti->tm_wday);
    }
}

// Controlls the LEDS
void status_display(int day)
{
    digitalWrite(ALARM, alarm_status);
    digitalWrite(WEEKEND, (day > 4));
    digitalWrite(MUSIC, music);
}

// This iterates through the single Digits
void time_display(int mins, int hour)
{
    // for 1...8 (the 8 Digits)
    for (int i = 1; i < 9; i++) {
        select_digit(i);
        switch (i) {
        case 1: // The first Digit
            if (hour < 10)
                num_on_digit(0); // If the number is smaller then 10 the we want there a 06:XX on the clock, so the first digit is replaced with a zero
            else
                num_on_digit(hour / 10); // Else the first number 13:XX (at index 0 of the string) gets displayed
            break;
        case 2: // The second Digit
            // The true is set because of the seperator we want for example 16:30 on the display not 1630
            if (hour < 10)
                num_on_digit_point(hour); // If the number is smaller then 10 we need to take the first position of the string
            else
                num_on_digit_point(hour % 10); // Else the second number at the String (at index 1)
            break;
        case 3: // Same as above
            if (mins < 10)
                num_on_digit(0);
            else
                num_on_digit(mins / 10);
            break;
        case 4:
            if (mins < 10)
                num_on_digit(mins);
            else
                num_on_digit(mins % 10);
            break;
        case 5:
            if (alarm_hour < 10)
                num_on_digit(0);
            else
                num_on_digit(alarm_hour / 10);
            break;
        case 6:
            if (alarm_hour < 10)
                num_on_digit_point(alarm_hour);
            else
                num_on_digit_point(alarm_hour % 10);
            break;
        case 7:
            if (alarm_min < 10)
                num_on_digit(0);
            else
                num_on_digit(alarm_min / 10);
            break;
        case 8:
            if (alarm_min < 10)
                num_on_digit(alarm_min);
            else
                num_on_digit(alarm_min % 10);
            break;
        }
        usleep(SLEEP); //The sleep command for the pins to change according to the selected Number
    }
}

// The Idea behind this is if the out put is set to 0 it is usable as a Ground.
// The pin of the digit-segment must be high
// Here it iterates throught the digit and set only the active one on low the rest is high
void select_digit(int i)
{
    digitalWrite(DIGIT1, i != 1);
    digitalWrite(DIGIT2, i != 2);
    digitalWrite(DIGIT3, i != 3);
    digitalWrite(DIGIT4, i != 4);
    digitalWrite(DIGIT5, i != 5);
    digitalWrite(DIGIT6, i != 6);
    digitalWrite(DIGIT7, i != 7);
    digitalWrite(DIGIT8, i != 8);
}

void num_on_digit(int number)
{
    nom(number, false);
}
void num_on_digit_point(int number)
{
    nom(number, true);
}
// This takes a int and wirtes the segments according to the array
void nom(int number, bool point)
{
    for (int i = 0; i < 7; i++) {
        digitalWrite(pins[i], pinvalues[number * 7 + i]);
    }
    digitalWrite(pins[7], point);
}
