#include "player.h"
#include "TUM_Draw.h"

#define PLAYER_WIDTH 60
#define PLAYER_BODY_HEIGTH 25
#define PLAYER_CANNON_WIDTH 8
#define PLAYER_CANNON_HEIGTH 20
#define PLAYER_MAX_X SCREEN_WIDTH - 10 - PLAYER_WIDTH
#define PLAYER_MIN_X 10


void paint_player(player_t *player){
    
    //draw Player Base
    tumDrawFilledBox(player->x_coord, PLAYER_Y_COORD, PLAYER_WIDTH, PLAYER_BODY_HEIGTH, Green);

    //draw Cannon
    int x_coord = player->x_coord + (int)((1.0/2.0) * (PLAYER_WIDTH - PLAYER_CANNON_WIDTH));
    int y_coord = PLAYER_Y_COORD - PLAYER_CANNON_HEIGTH;
    tumDrawFilledBox(x_coord, y_coord, PLAYER_CANNON_WIDTH, PLAYER_CANNON_HEIGTH, Green);
}

void player_move_right(player_t *player){
    if(player->x_coord < PLAYER_MAX_X){
        player->x_coord++;
    }
}

void player_move_left(player_t *player){
    if(player->x_coord > PLAYER_MIN_X){
        player->x_coord--;
    }
}