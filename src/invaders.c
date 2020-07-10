#include "invaders.h"
#include "TUM_Draw.h"


TaskHandle_t InvaderTask;



void vHandleInvaders(void *pvParameters){


    for(;;){


        vTaskDelay((TickType_t) 1000);
    }
}

void invaderInit(){
    show_bmp1 = true;

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
}

void paint_invaders(){

    invader_t * invader;
    for(int r=0; r<NUM_INVADER_ROWS; r++){
        for(int c=0; c<NUM_INVADER_COLUMNS; c++){
            invader = &Invaders[r][c];
            if (show_bmp1){
                tumDrawLoadedImage(invader->type->bmp1, invader->x, invader->y);
            }else{
                tumDrawLoadedImage(invader->type->bmp2, invader->x, invader->y);
            }
            
            
        }
    }
}


