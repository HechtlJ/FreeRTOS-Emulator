#include "player.h"
#include "TUM_Draw.h"

#define PLAYER_WIDTH 60
#define PLAYER_BODY_HEIGTH 25
#define PLAYER_CANNON_WIDTH 8
#define PLAYER_CANNON_HEIGTH 20


void paint_player(player_t *player){
    
    //draw Player Base
    tumDrawFilledBox(player->x_coord, PLAYER_Y_COORD, PLAYER_WIDTH, PLAYER_BODY_HEIGTH, Green);

    //draw Cannon
    x_coord = player->x_coord + (int)((1.0/2.0) * (PLAYER_WIDTH - PLAYER_CANNON_WIDTH));
    y_coord = PLAYER_Y_COORD - PLAYER_CANNON_HEIGTH;
    tumDrawFilledBox(x_coord, y_coord, PLAYER_CANNON_WIDTH, PLAYER_CANNON_HEIGTH, Green);
}