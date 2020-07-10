#include "bunker.h"
#include "stdio.h"



void init_bunkers(){
    for(int i=0; i<NUM_BUNKERS; i++){
        Bunkers[i].img = tumDrawLoadImage("../resources/img/bunker.bmp");
        Bunkers[i].y = BUNKER_Y_COORD;
    }

    //TODO Fix that NUM_BUNKERS doesnt affect this
    int x_step = SCREEN_WIDTH - 2*BUNKER_RIM - BUNKER_WIDTH;
    x_step = x_step/(NUM_BUNKERS-1.0);

    for(int i=0; i<NUM_BUNKERS; i++){
        Bunkers[i].x = BUNKER_RIM + i*x_step;
    }

    printf("bunker init \n");
    
}

void paint_bunkers(){
    for(int i=0; i<NUM_BUNKERS; i++){
        tumDrawLoadedImage(Bunkers[i].img, Bunkers[i].x, Bunkers[i].y);
    }
}

void xPaintBunkers(){
    if(xSemaphoreTake(BunkerHandle, ( TickType_t ) 10 ) == pdTRUE){
            paint_bunkers();
            xSemaphoreGive(BunkerHandle);
    }else{
        //do some error handling
        printf("error in painting bunker");
    }
}