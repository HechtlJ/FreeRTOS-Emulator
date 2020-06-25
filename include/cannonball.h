#ifndef CANNONBALL_H
#define CANNONBALL_H

#include <stdlib.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "semphr.h"

#define CANNONBALL_VELOCITY
#define CANNONBALL_WIDTH 5
#define CANNONBALL_HEIGHT 15
#define MAX_NUM_CANNONBALLS 50
#define RELOAD_TIME 30

typedef struct {
    unsigned int x_coord;
    unsigned int y_coord;
    bool exists;
} cannonball_t;

cannonball_t CannonBalls[MAX_NUM_CANNONBALLS];
xSemaphoreHandle CannonballHandle;

void xPaintCannonballs();
void xCreateCannonball(int x, int y);
void init_cannonballs();
void paint_cannonballs();
void create_cannonball(int x, int y);


#endif /* CANNONBALL_H */