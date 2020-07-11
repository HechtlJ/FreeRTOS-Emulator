#include "invaders.h"
#include "TUM_Draw.h"
#include "stdio.h"

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
        vTaskDelay((TickType_t) 1000);
    }
}

void invaderInit(){
    show_bmp1 = true;
    movement = 0;

    invaderTypes[INVADER_TYPE_A].points = 10;
    invaderTypes[INVADER_TYPE_A].bmp1 = tumDrawLoadImage("../resources/img/invaderA1.bmp");
    invaderTypes[INVADER_TYPE_A].bmp2 = tumDrawLoadImage("../resources/img/invaderA2.bmp");

    invaderTypes[INVADER_TYPE_B].points = 20;
    invaderTypes[INVADER_TYPE_B].bmp1 = tumDrawLoadImage("../resources/img/invaderB1.bmp");
    invaderTypes[INVADER_TYPE_B].bmp2 = tumDrawLoadImage("../resources/img/invaderB2.bmp");

    invaderTypes[INVADER_TYPE_C].points = 40;
    invaderTypes[INVADER_TYPE_C].bmp1 = tumDrawLoadImage("../resources/img/invaderC1.bmp");
    invaderTypes[INVADER_TYPE_C].bmp2 = tumDrawLoadImage("../resources/img/invaderC2.bmp");





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
            x += INVADER_SPACE_HORIZONTAL + INVADER_WIDTH;
        }
        y+= INVADER_SPACE_VERTICAL + INVADER_HEIGHT;
    }

    if(xTaskCreate(vHandleInvaders, "InvaderTask", mainGENERIC_STACK_SIZE*2, NULL,
     configMAX_PRIORITIES-1, &InvaderTask) != pdPASS) {
         //PRINT_TASK_ERROR("RenderTask");
         //goto err_rendertask;
        printf("error in creating invader task\n");
    }
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
    shiftRight += INVADER_SIDE_STEP;
}

void move_invaders_left(){
    for(int r=0; r<NUM_INVADER_ROWS; r++){
        for(int c=0; c<NUM_INVADER_COLUMNS; c++){
            Invaders[r][c].x-=INVADER_SIDE_STEP;
        }
    }
    shiftRight -= INVADER_SIDE_STEP;
}

void move_invaders_down(){
    if(shiftDown >= INVADER_MAX_SHIFT_DOWN){
        // Game OVer
        printf("Game Over");
    }
    for(int r=0; r<NUM_INVADER_ROWS; r++){
        for(int c=0; c<NUM_INVADER_COLUMNS; c++){
            Invaders[r][c].y+=INVADER_DOWN_STEP;
        }
    }
    shiftDown += INVADER_DOWN_STEP;
}

#define MOVING_RIGHT 0
#define MOVING_LEFT 1

void move_invaders(){
    if(movement==MOVING_LEFT){
        if(shiftRight-INVADER_SIDE_STEP > 0){
            move_invaders_left();
        }else{
            move_invaders_down();
            movement = MOVING_RIGHT;
        }
    }else if(movement==MOVING_RIGHT){
        if(shiftRight+INVADER_SIDE_STEP < INVADER_MAX_SHIFT_RIGHT){
            move_invaders_right();
        }else{
            move_invaders_down();
            movement = MOVING_LEFT;
        }
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
                        return 1; //Hit
                    }
                }
            }
        }
    }
    return 0;
}


