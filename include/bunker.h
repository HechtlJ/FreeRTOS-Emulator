#ifndef BUNKER_H
#define BUNKER_H

#include <stdlib.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include "TUM_Draw.h"

#define NUM_BUNKERS 4
#define BUNKER_WIDTH 80
#define BUNKER_RIM 30
#define BUNKER_Y_COORD 320

#define BUNKER_HEIGHT 30


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

int hitBunker(int x, int y, int w, int h);



#endif /* BUNKER_H */