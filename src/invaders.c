#include "invaders.h"
#include "TUM_Draw.h"
#include "stdio.h"
#include "missile.h"
#include "stdlib.h"
#include "time.h"
#include "game.h"

//TODO: switch to queue for points
#include "player.h"


TaskHandle_t InvaderTask;



void vHandleInvaders(void *pvParameters){


    for(;;){
        if(show_bmp1){
            show_bmp1=false;
        }else{
            show_bmp1=true;
        }
        move_invaders();
        invader_shoot();
        vTaskDelay((TickType_t) invaderDelay);
    }
}

void invaderInit(){
    srand (time(NULL));
    invaderDelay = INVADER_START_DELAY+10;
    show_bmp1 = true;
    movement = 0;

    invaderTypes[INVADER_TYPE_A].points = 10;
    invaderTypes[INVADER_TYPE_A].bmp1 = tumDrawLoadScaledImage("../resources/img/invaderA1.bmp", 1);
    invaderTypes[INVADER_TYPE_A].bmp2 = tumDrawLoadScaledImage("../resources/img/invaderA2.bmp", 1);

    invaderTypes[INVADER_TYPE_B].points = 20;
    invaderTypes[INVADER_TYPE_B].bmp1 = tumDrawLoadScaledImage("../resources/img/invaderB1.bmp", 1);
    invaderTypes[INVADER_TYPE_B].bmp2 = tumDrawLoadScaledImage("../resources/img/invaderB2.bmp", 1);

    invaderTypes[INVADER_TYPE_C].points = 40;
    invaderTypes[INVADER_TYPE_C].bmp1 = tumDrawLoadScaledImage("../resources/img/invaderC1.bmp", 1);
    invaderTypes[INVADER_TYPE_C].bmp2 = tumDrawLoadScaledImage("../resources/img/invaderC2.bmp", 1);



    invader_reset(0);

    

    if(xTaskCreate(vHandleInvaders, "InvaderTask", mainGENERIC_STACK_SIZE*2, NULL,
     configMAX_PRIORITIES-1, &InvaderTask) != pdPASS) {
         //PRINT_TASK_ERROR("RenderTask");
         //goto err_rendertask;
        printf("error in creating invader task\n");
    }
    vTaskSuspend(InvaderTask);
}

void paint_invaders(){

    invader_t * invader;
    for(int r=0; r<NUM_INVADER_ROWS; r++){
        for(int c=0; c<NUM_INVADER_COLUMNS; c++){
            invader = &Invaders[r][c];
            if(invader->alive){
                if (show_bmp1){
                    tumDrawLoadedImage(invader->type->bmp1, invader->x, invader->y);
                }else{
                    tumDrawLoadedImage(invader->type->bmp2, invader->x, invader->y);
                }
            }
            
        }
    }
}

void move_invaders_right(){
    for(int r=0; r<NUM_INVADER_ROWS; r++){
        for(int c=0; c<NUM_INVADER_COLUMNS; c++){
            Invaders[r][c].x+=INVADER_SIDE_STEP;
        }
    }
    //shiftRight += INVADER_SIDE_STEP;
}

void move_invaders_left(){
    for(int r=0; r<NUM_INVADER_ROWS; r++){
        for(int c=0; c<NUM_INVADER_COLUMNS; c++){
            Invaders[r][c].x-=INVADER_SIDE_STEP;
        }
    }
    //shiftRight -= INVADER_SIDE_STEP;
}

void move_invaders_down(){
    for(int r=0; r<NUM_INVADER_ROWS; r++){
        for(int c=0; c<NUM_INVADER_COLUMNS; c++){
            Invaders[r][c].y+=INVADER_DOWN_STEP;
            if(Invaders[r][c].y > INVADER_MAX_Y && Invaders[r][c].alive){
                switchToGameOver();
            }
        }
    }
}

#define MOVING_RIGHT 0
#define MOVING_LEFT 1

void move_invaders(){
    if(movement==MOVING_LEFT){
        for(int c=0; c<NUM_INVADER_COLUMNS; c++){
            if(Invaders[0][c].x < INVADER_SIDE_STEP){
                for(int r=0; r<NUM_INVADER_ROWS; r++){
                    if(Invaders[r][c].alive){
                        move_invaders_down();
                        movement = MOVING_RIGHT;
                        return;
                    }
                }
            }else{
                break;
            }
        }
        move_invaders_left();
        return;
    }else if(movement==MOVING_RIGHT){
        for(int c=NUM_INVADER_COLUMNS-1; c>=0; c--){
            if(Invaders[0][c].x > SCREEN_WIDTH-INVADER_SIDE_STEP-INVADER_WIDTH){
                for(int r=0; r<NUM_INVADER_ROWS; r++){
                    if(Invaders[r][c].alive){
                        move_invaders_down();
                        movement = MOVING_LEFT;
                        return;
                    }
                }
            }else{
                break;
            }
        }
        move_invaders_right();
        return;
    }
}

int invaders_check_hit(int x, int y, int w, int h){
    int min_x = x;
    int min_y = y;
    int max_x = x+w;
    int max_y = y+h;
    for(int r=0; r<NUM_INVADER_ROWS; r++){
        if(Invaders[r][0].y <= min_y && (Invaders[r][0].y+INVADER_HEIGHT) >= max_y){
            for(int c=0; c<NUM_INVADER_COLUMNS; c++){
                if(Invaders[r][c].x <= min_x && (Invaders[r][c].x+INVADER_WIDTH) >= max_x){
                    if(Invaders[r][c].alive){
                        Invaders[r][c].alive = false;
                        Player.Points += Invaders[r][c].type->points;
                        invaderDelay -= INVADER_DELAY_DECREASE;
                        return 1; //Hit
                    }
                }
            }
        }
    }
    return 0;
}

void invader_shoot(){
    int column = rand() % NUM_INVADER_COLUMNS + 1;
    char type = rand() % 4;  // There are 3 types

    int x_offset = INVADER_WIDTH/2 -1;
    int y_offset = INVADER_HEIGHT;

    for(int i=NUM_INVADER_ROWS; i>=0; i--){
        invader_t * invader = &Invaders[i][column];
        if(invader->alive){
            xCreateMissile(invader->x + x_offset, invader->y + y_offset, type);
            return;
        }
    }
}



int invaders_all_dead(){
    for(int r=0; r<NUM_INVADER_ROWS; r++){
        for(int c=0; c<NUM_INVADER_COLUMNS; c++){
            if(Invaders[r][c].alive){
                return 0;
            }
            
        }
    }
    return 1;
}


void invader_reset(int offset){

    invaderDelay = INVADER_START_DELAY;


    for(int c=0; c<NUM_INVADER_COLUMNS; c++){
        Invaders[0][c].type = &invaderTypes[INVADER_TYPE_C];
        Invaders[1][c].type = &invaderTypes[INVADER_TYPE_B];
        Invaders[2][c].type = &invaderTypes[INVADER_TYPE_B];
        Invaders[3][c].type = &invaderTypes[INVADER_TYPE_A];
        Invaders[4][c].type = &invaderTypes[INVADER_TYPE_A];
    }

    int y = INVADER_SPACE_TOP;
    int x;
    for(int r=0; r<NUM_INVADER_ROWS; r++){
        x = INVADER_SPACE_LEFT;
        for(int c=0; c<NUM_INVADER_COLUMNS; c++){
            Invaders[r][c].x = x;
            Invaders[r][c].y = y;
            Invaders[r][c].alive = true;
            x = x + INVADER_SPACE_HORIZONTAL + INVADER_WIDTH;
        }
        y+= INVADER_SPACE_VERTICAL;
        y+= INVADER_HEIGHT;
    }
}
