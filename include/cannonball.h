#ifndef CANNONBALL_H
#define CANNONBALL_H

#include <stdlib.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "semphr.h"

#define CANNONBALL_VELOCITY 10
#define CANNONBALL_WIDTH 5
#define CANNONBALL_HEIGHT 15
#define MAX_NUM_CANNONBALLS 50
#define RELOAD_TIME 20

typedef struct {
    int x_coord;
    int y_coord;
    bool exists;
} cannonball_t;

cannonball_t CannonBalls[MAX_NUM_CANNONBALLS];
xSemaphoreHandle CannonballHandle;

void xPaintCannonballs();
void xCreateCannonball(int x, int y);
void xMoveCannonballs();
void init_cannonballs();
void paint_cannonballs();
void create_cannonball(int x, int y);
void move_cannonballs();


#endif /* CANNONBALL_H */