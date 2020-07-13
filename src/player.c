#include "player.h"
#include "TUM_Draw.h"
#include <stdio.h>
#include "cannonball.h"

void playerInit(){
    Player.img = tumDrawLoadScaledImage("../resources/img/player.bmp", 1);
}


void paint_player(){
    tumDrawLoadedImage(Player.img, Player.x_coord, PLAYER_Y_COORD);
}

void player_move_right(){
    Player.x_coord += PLAYER_SPEED;
    if(Player.x_coord > PLAYER_MAX_X){
        Player.x_coord = PLAYER_MAX_X;
    }
}

void player_move_left(){
    Player.x_coord -= PLAYER_SPEED;
    if(Player.x_coord < PLAYER_MIN_X){
        Player.x_coord = PLAYER_MIN_X;
    }
}

void player_shoot(){
    int x = Player.x_coord + 0.5*PLAYER_WIDTH - 0.5 * CANNONBALL_WIDTH ;
    int y = PLAYER_Y_COORD;
    create_cannonball(x, y);
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

void xPlayerShoot(){
    if(xSemaphoreTake(PlayerHandle, NULL) == pdTRUE){
            player_shoot();
            xSemaphoreGive(PlayerHandle);
    }else{
        //do some error handling
    }
}

void xResetPlayer(){
    if(xSemaphoreTake(PlayerHandle, NULL) == pdTRUE){
            reset_player();
            xSemaphoreGive(PlayerHandle);
    }else{
        //do some error handling
    }
}


void reset_player(){
    Player.x_coord = 200;
    Player.Points = 0;
    Player.Life = 2;
}