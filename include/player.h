#include "FreeRTOS.h"
#include "semphr.h"


#define PLAYER_Y_COORD 400
//TODO: add include guards 

typedef struct {
    unsigned int Life;
    unsigned int x_coord;
} player_t;

player_t Player;
xSemaphoreHandle PlayerHandle;



void xPaintPlayer();
void xPlayerMoveRight();
void xPlayerMoveLeft();
void xPlayerShoot();
void player_shoot();
void player_reduce_life();
