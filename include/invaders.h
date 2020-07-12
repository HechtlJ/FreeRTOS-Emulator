#ifndef INVADERS_H
#define INVADERS_H

#include "FreeRTOS.h"
#include "task.h"
#include "stdbool.h"


#define mainGENERIC_PRIORITY (tskIDLE_PRIORITY)
#define mainGENERIC_STACK_SIZE ((unsigned short)2560)

#define NUM_INVADER_ROWS 5
#define NUM_INVADER_COLUMNS 11
#define INVADER_WIDTH 36
#define INVADER_HEIGHT 35
#define INVADER_SPACE_HORIZONTAL 9
#define INVADER_SPACE_VERTICAL 10
#define INVADER_SPACE_LEFT 5
#define INVADER_SPACE_TOP 160

#define INVADER_TYPE_A 0
#define INVADER_TYPE_B 1
#define INVADER_TYPE_C 2

#define INVADER_SIDE_STEP 10
#define INVADER_DOWN_STEP 20

#define INVADER_MAX_SHIFT_DOWN 100

#define INVADER_START_DELAY 1000
#define INVADER_DELAY_DECREASE INVADER_START_DELAY / (NUM_INVADER_ROWS * NUM_INVADER_COLUMNS)



extern TaskHandle_t InvaderTask;


void invaderInit();


typedef struct {
    int points;
    void * bmp1;
    void * bmp2;
}invadertype_t;


invadertype_t invaderTypes[3];

bool show_bmp1;

#define MOVING_RIGHT 0
#define MOVING_LEFT 1
bool movement;


int invaderDelay;


typedef struct {
    invadertype_t * type;
    int x;
    int y;
    bool alive;
}invader_t;

invader_t Invaders[NUM_INVADER_ROWS][NUM_INVADER_COLUMNS];



void paint_invaders();
void move_invaders();
//int invaders_check_hit(int x, int y, int w, int h);

void invader_shoot();
int invaders_all_dead();

void xInvaderReset(int offset);
void invader_reset(int offset);

#endif /* INVADERS_H */