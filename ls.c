#include "ls.h"
#include <dirent.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <wiringPiSPI.h>

#define PI 3.14f
//#define FUNC_P (void * (*)(void*))
// 194, 95, 0; 164, 0, 0; 59, 122, 0; 32, 74, 135; 0, 29, 194; 170, 51, 189;
unsigned char colours[18] = {194, 95, 0, 164, 0, 0, 59, 122, 0, 32, 74, 135, 0, 29, 194, 170, 51, 189};
// The global interface for the music
bool music;
bool music_play = false;
int colour_index = 0;

void led();
void play_song();
int random_wrapper(unsigned int);
void color_data(unsigned char*, int, int, int, int, int, float);

void ls_setup()
{
    music = true;
}

void ls()
{
    pthread_t leds = 0;
    void * data = NULL;
    while (1) {
        while (music) {
            colour_index = random_wrapper(6);
            music_play = true;
            if (leds == 0)
                pthread_create(&leds, NULL, &led, NULL);
            play_song();
        }
        music_play = false;
        pthread_join(leds, data);
        free(data);
        leds = 0;
    }
}

void led()
{
    int len = 77 * 3;
    unsigned char dat[len];

    for (unsigned long long i = 0; music_play; i++) {
        color_data(dat, colours[colour_index], colours[colour_index + 1], colours[colour_index + 2], i, len, 80.0f);
        wiringPiSPIDataRW(0, dat, len);
        nanosleep((const struct timespec[]){{0, 30000000}}, NULL); //sleeps for 300000 mikroseconds
    }

    for(int i = 0; i < len; i++) dat[i] = 0;
    wiringPiSPIDataRW(0, dat, len);
    pthread_exit(NULL);
}

//returns truly a random number for song selection maxium return value is max-1
int random_wrapper(unsigned int max)
{
    srand(time(NULL) * rand());
    return rand() % max;
}

//The u is the time varaible which let it pulsate
void color_data(unsigned char* dat, int R, int G, int B, int u, int len, float v)
{
    R = R + (R / 255.0f) * v * sin((u / 53.0f) * PI);
    G = G + (G / 255.0f) * v * sin((u / 53.0f) * PI);
    B = B + (B / 255.0f) * v * sin((u / 53.0f) * PI);
    unsigned char red = (R > 255) ? 255 : R;
    unsigned char green = (G > 255) ? 255 : G;
    unsigned char blue = (B > 255) ? 255 : B;
    for (int i = 0; i < len; i++) {
        if ((i % 3) == 0)
            dat[i] = red;
        if ((i % 3) == 1)
            dat[i] = green;
        if ((i % 3) == 2)
            dat[i] = blue;
    }
}

//this plays a song
void play_song()
{
    DIR* dir;
    struct dirent * ent;
    struct dirent * entries = malloc(300 * sizeof(struct dirent));
    if ((dir = opendir("/home/pi/Musik/")) == NULL) {
        fprintf(stderr, "ERROR");
        exit(-1);
    }
    int i;
    for (i = 1; (ent = readdir(dir)) != NULL; i++) {
        if(i < 3) continue;
        memcpy(entries + i, ent, sizeof(struct dirent));
    }
    closedir(dir);
    char cmd[500];
    strcpy(cmd, "mpv --af=volume=replaygain-track --no-terminal --volume=100 \"/home/pi/Musik/");
    strcat(cmd, (entries + random_wrapper(i))->d_name);
    strcat(cmd, "\"");
    system(cmd);
    free(entries);
}
