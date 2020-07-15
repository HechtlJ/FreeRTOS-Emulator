#include "player.h"
#include "TUM_Draw.h"
#include <stdio.h>
#include "cannonball.h"
#include "game.h"
#include "missile.h"

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
        printf("error in painting player");
    }
}


void xPlayerMoveRight(){
    if(xSemaphoreTake(PlayerHandle, ( TickType_t ) 10) == pdTRUE){
            player_move_right();
            xSemaphoreGive(PlayerHandle);
    }else{
        //do some error handling
    }
}


void xPlayerMoveLeft(){
    if(xSemaphoreTake(PlayerHandle, ( TickType_t ) 10) == pdTRUE){
            player_move_left();
            xSemaphoreGive(PlayerHandle);
    }else{
        //do some error handling
    }
}

void xPlayerShoot(){
    if(xSemaphoreTake(PlayerHandle, ( TickType_t ) 10) == pdTRUE){
            player_shoot();
            xSemaphoreGive(PlayerHandle);
    }else{
        //do some error handling
    }
}

void xResetPlayer(){
    if(xSemaphoreTake(PlayerHandle, ( TickType_t ) 10) == pdTRUE){
            reset_player();
            xSemaphoreGive(PlayerHandle);
    }else{
        //do some error handling
    }
}

int xCheckPlayerHit(int x, int y, int height){
    if(xSemaphoreTake(PlayerHandle, ( TickType_t ) 10) == pdTRUE){
            int ret = check_player_hit(x,y, height);
            xSemaphoreGive(PlayerHandle);
            return ret;
    }else{
        //do some error handling
    }
}

int check_player_hit(int x, int y, int height){
    if((y + height) < PLAYER_Y_COORD){
        return 0; // No Hit
    }if(y > (PLAYER_Y_COORD + PLAYER_HEIGTH)){
        return 0; // No Hit
    }if(x < Player.x_coord){
        return 0; // No Hit
    }if(x > (Player.x_coord + PLAYER_WIDTH)){
        return 0; // No Hit
    }
    player_reduce_life();
    return 1;
}



void reset_player(){
    Player.x_coord = 200;
    Player.Points = 0;
    Player.Life = 2;
}


void player_reduce_life(){
    bool unlimited_lives;
    if(xSemaphoreTake(CheatsHandle, ( TickType_t ) 10 ) == pdTRUE){
            unlimited_lives = Cheats.unlimitedLives;
            xSemaphoreGive(CheatsHandle);
    }
    if(unlimited_lives){
        return;
    }
    
    if(Player.Life==0){
        switchToGameOver();
    }else{
        reset_missiles();
        Player.Life--;
    }

}
