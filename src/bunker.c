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

    for(int x=0; x<NUM_BUNKER_BLOCK_X; x++){
        for(int y=0; y<NUM_BUNKER_BLOCK_Y; y++){
            //TODO: for schleife
            Bunkers[0].BunkerBlocks[x][y] = true;
            Bunkers[1].BunkerBlocks[x][y] = true;
            Bunkers[2].BunkerBlocks[x][y] = true;
            Bunkers[3].BunkerBlocks[x][y] = true;
        }
    }
    
}

void paint_bunkers(){
    int bunker_x;
    int bunker_y;
    for(int i=0; i<NUM_BUNKERS; i++){
        bunker_x = Bunkers[i].x;
        bunker_y = Bunkers[i].y;
        //tumDrawLoadedImage(Bunkers[i].img, Bunkers[i].x, Bunkers[i].y);
        for(int x=0; x<NUM_BUNKER_BLOCK_X; x++){
            for(int y=0; y<NUM_BUNKER_BLOCK_Y; y++){
                if(Bunkers[i].BunkerBlocks[x][y])
                    tumDrawFilledBox(bunker_x+x*4, bunker_y+y*4, 4, 4, Green);
            }
        }
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

int hitBunker(int x, int y, int w, int h){
    // Check height
    int y_min=y;
    int y_max=y+h;

    if(y_max < BUNKER_Y_COORD || y_min > BUNKER_Y_COORD+BUNKER_HEIGHT)
        return 0; //no collision
   

    int x_min = x;
    int x_max = x + w;
    
    for(int i=0; i<NUM_BUNKERS; i++){
        if(x_max < Bunkers[i].x || x_min > Bunkers[i].x + BUNKER_WIDTH){
            
            
        }else{
            printf("collision\n");
            damageBunker(x, y, w, h, i);
            return 1; // collision
        }

    }
    return 0;
}
 

void damageBunker(int x_global, int y_global, int w, int h, int bunker){
    int x = x_global - Bunkers[bunker].x;
    x += 0.5*w;
    int y = y_global - Bunkers[bunker].y;
    y += 0.5*h;
    x = x/4;
    y=y/4;
    printf("Destroying %d %d", x, y);
    Bunkers[bunker].BunkerBlocks[x][y]=false;
}



int checkBunkerHit(int x, int y, bool moving_up, int damage){
    int hit_x;
    if(y > BUNKER_Y_COORD+BUNKER_HEIGHT)
        return 0; // No Hit

    for(int i=0; i<NUM_BUNKERS; i++){
        if(x >= Bunkers[i].x && x < Bunkers[i].x+BUNKER_WIDTH){
            hit_x = x-Bunkers[i].x;
            hit_x = hit_x/4;
            for(int hit_y=NUM_BUNKER_BLOCK_Y-1; hit_y>=0; hit_y--){
                if(Bunkers[i].BunkerBlocks[hit_x][hit_y]==true){
                    Bunkers[i].BunkerBlocks[hit_x][hit_y]=false;
                    return 1;
                }
            }
        }
    }
    return 0; // No Hit
}
