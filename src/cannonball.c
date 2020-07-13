#include "cannonball.h"
#include "TUM_Draw.h"
#include "stdio.h"

void init_cannonballs(){
    for(int i=0; i<MAX_NUM_CANNONBALLS; i++){
        CannonBalls[i].x_coord = 0;
        CannonBalls[i].y_coord = 0;
        CannonBalls[i].exists = false;
    }
}

void paint_cannonballs(){
    for(int i=0; i<MAX_NUM_CANNONBALLS; i++){
        if(CannonBalls[i].exists)
            tumDrawFilledBox(CannonBalls[i].x_coord, CannonBalls[i].y_coord, CANNONBALL_WIDTH, CANNONBALL_HEIGHT, White);
    }
}

void create_cannonball(int x, int y){
    for(int i=0; i<MAX_NUM_CANNONBALLS; i++){
        if(!CannonBalls[i].exists){
            CannonBalls[i].exists = true;
            CannonBalls[i].x_coord = x;
            CannonBalls[i].y_coord = y;
            return;
        }
    }
}

void move_cannonballs(){
    for(int i=0; i<MAX_NUM_CANNONBALLS; i++){
        if(CannonBalls[i].exists){
            CannonBalls[i].exists = true;
            CannonBalls[i].y_coord -= CANNONBALL_VELOCITY;
            if(CannonBalls[i].y_coord < 0)
                CannonBalls[i].exists = false;
        }
    }
}


void xPaintCannonballs(){
    if(xSemaphoreTake(CannonballHandle, ( TickType_t ) 10 ) == pdTRUE){
            paint_cannonballs();
            xSemaphoreGive(CannonballHandle);
    }else{
        //do some error handling
        printf("error in painintg cannonball");
    }
}

void xCreateCannonball(int x, int y){
    if(xSemaphoreTake(CannonballHandle, ( TickType_t ) 10 ) == pdTRUE){
            create_cannonball(x, y);
            xSemaphoreGive(CannonballHandle);
    }else{
        //do some error handling
        printf("error in painintg player");
    }
}

void xMoveCannonballs(){
    if(xSemaphoreTake(CannonballHandle, ( TickType_t ) 10 ) == pdTRUE){
            move_cannonballs();
            xSemaphoreGive(CannonballHandle);
    }else{
        //do some error handling
        printf("error in painintg player");
    }
}


void xResetCannonballs(){
    if(xSemaphoreTake(CannonballHandle, ( TickType_t ) 10 ) == pdTRUE){
            reset_cannonballs();
            xSemaphoreGive(CannonballHandle);
    }else{
        //do some error handling
        printf("error in painintg player");
    }
}

void reset_cannonballs(){
    for(int i=0; i<MAX_NUM_CANNONBALLS; i++){
        CannonBalls[i].x_coord = 0;
        CannonBalls[i].y_coord = 0;
        CannonBalls[i].exists = false;
    }
}
