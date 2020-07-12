#ifndef BUNKER_H
#define BUNKER_H

#include <stdlib.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include "TUM_Draw.h"

#define NUM_BUNKERS 4
#define BUNKER_WIDTH 30
#define BUNKER_RIM 60
#define BUNKER_Y_COORD 485

#define BUNKER_HEIGHT 30

#define NUM_BUNKER_BLOCK_X 20
#define NUM_BUNKER_BLOCK_Y 8


typedef struct {
    image_handle_t img;
    bool BunkerBlocks[NUM_BUNKER_BLOCK_X][NUM_BUNKER_BLOCK_Y];
    int x;
    int y;
} bunker_t;

bunker_t Bunkers[NUM_BUNKERS];
xSemaphoreHandle BunkerHandle;

void init_bunkers();
void paint_bunkers();


void xPaintBunkers();

int hitBunker(int x, int y, int w, int h);
void damageBunker(int x, int y, int w, int h, int bunker);

int checkBunkerHit(int x, int y, bool moving_up, int damage);



#endif /* BUNKER_H */