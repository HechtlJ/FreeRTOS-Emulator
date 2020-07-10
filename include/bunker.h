#ifndef BUNKER_H
#define BUNKER_H

#include <stdlib.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include "TUM_Draw.h"

#define NUM_BUNKERS 4
#define BUNKER_WIDTH 90.0
#define BUNKER_RIM 30.0
#define BUNKER_Y_COORD 320


typedef struct {
    image_handle_t img;
    int x;
    int y;
} bunker_t;

bunker_t Bunkers[NUM_BUNKERS];
xSemaphoreHandle BunkerHandle;

void init_bunkers();
void paint_bunkers();


void xPaintBunkers();



#endif /* BUNKER_H */