#include "player.h"
#include "TUM_Draw.h"
#include <stdio.h>


#define PLAYER_WIDTH 60
#define PLAYER_BODY_HEIGTH 25
#define PLAYER_CANNON_WIDTH 8
#define PLAYER_CANNON_HEIGTH 20
#define PLAYER_MAX_X SCREEN_WIDTH - 10 - PLAYER_WIDTH
#define PLAYER_MIN_X 10


void paint_player(){
    
    //draw Player Base
    tumDrawFilledBox(Player.x_coord, PLAYER_Y_COORD, PLAYER_WIDTH, PLAYER_BODY_HEIGTH, Green);

    //draw Cannon
    int x_coord = Player.x_coord + (int)((1.0/2.0) * (PLAYER_WIDTH - PLAYER_CANNON_WIDTH));
    int y_coord = PLAYER_Y_COORD - PLAYER_CANNON_HEIGTH;
    tumDrawFilledBox(x_coord, y_coord, PLAYER_CANNON_WIDTH, PLAYER_CANNON_HEIGTH, Green);
}

void player_move_right(){
    if(Player.x_coord < PLAYER_MAX_X){
        Player.x_coord++;
    }
}

void player_move_left(){
    if(Player.x_coord > PLAYER_MIN_X){
        Player.x_coord--;
    }
}

void xPaintPlayer(){
    if(xSemaphoreTake(PlayerHandle, ( TickType_t ) 10 ) == pdTRUE){
            paint_player();
            xSemaphoreGive(PlayerHandle);
    }else{
        //do some error handling
        printf("eroor in painintg player");
    }
}


void xPlayerMoveRight(){
    if(xSemaphoreTake(PlayerHandle, NULL) == pdTRUE){
            player_move_right();
            xSemaphoreGive(PlayerHandle);
    }else{
        //do some error handling
    }
}


void xPlayerMoveLeft(){
    if(xSemaphoreTake(PlayerHandle, NULL) == pdTRUE){
            player_move_left();
            xSemaphoreGive(PlayerHandle);
    }else{
        //do some error handling
    }
}