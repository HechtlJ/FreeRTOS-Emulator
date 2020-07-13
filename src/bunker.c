#include "bunker.h"
#include "stdio.h"



void init_bunkers(){
    
    int x_step = SCREEN_WIDTH - 2*BUNKER_RIM - BUNKER_WIDTH;
    x_step = x_step/(NUM_BUNKERS-1.0);
    for(int i=0; i<NUM_BUNKERS; i++){
        Bunkers[i].x_coord = BUNKER_RIM + i*x_step;
        Bunkers[i].y_coord = BUNKER_Y_COORD;
    }

    reset_bunkers();
}

void paint_bunkers(){
    int bunker_x;
    int bunker_y;
    for(int i=0; i<NUM_BUNKERS; i++){
        bunker_x = Bunkers[i].x_coord;
        bunker_y = Bunkers[i].y_coord;
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

/*
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
}*/


int checkBunkerHit(int x, int y, bool moving_up, int damage){
    if(moving_up){
        int hit_x;
        if(y > BUNKER_Y_COORD+BUNKER_HEIGHT)
            return 0; // No Hit

        for(int i=0; i<NUM_BUNKERS; i++){
            if(x >= Bunkers[i].x_coord && x < Bunkers[i].x_coord+BUNKER_WIDTH){
                hit_x = x-Bunkers[i].x_coord;
                hit_x = hit_x/4;
                for(int hit_y=NUM_BUNKER_BLOCK_Y-1; hit_y>=0; hit_y--){
                    if(Bunkers[i].BunkerBlocks[hit_x][hit_y]==true){
                        //Bunkers[i].BunkerBlocks[hit_x][hit_y]=false;
                        damageBunker(&Bunkers[i], hit_x, hit_y, damage);
                        return 1;
                   }
               }
            }
        }
    }else{
        int hit_x;
        if(y < BUNKER_Y_COORD)
            return 0; //No hit

        for(int i=0; i<NUM_BUNKERS; i++){
            if(x >= Bunkers[i].x_coord && x < Bunkers[i].x_coord+BUNKER_WIDTH){
                hit_x = x-Bunkers[i].x_coord;
                hit_x = hit_x/4;
                for(int hit_y=0-1; hit_y<NUM_BUNKER_BLOCK_Y; hit_y++){
                    if(Bunkers[i].BunkerBlocks[hit_x][hit_y]==true){
                        //Bunkers[i].BunkerBlocks[hit_x][hit_y]=false;
                        damageBunker(&Bunkers[i], hit_x, hit_y, damage);
                        return 1;
                   }
               }
            }
        }
    }
    return 0; // No Hit
}

void try_bunker_destruction(bunker_t * bunker, int x, int y){
    if(x<0 || y<0){
        return;
    }
    if(x>=NUM_BUNKER_BLOCK_X || y>=NUM_BUNKER_BLOCK_Y){
        return;
    }
    bunker->BunkerBlocks[x][y] = false;
}
/*
void damageBunker(bunker_t * bunker, int hit_x, int hit_y, int damage){
    int i= 0;
    for(int x= hit_x - damage; x<= hit_x; x++){
        for(int y = hit_y-i; i<=hit_y+i; y++){
            try_bunker_destruction(bunker, x, y);
        }
        i++;
    }
    i=
    for(int x= hit_x - damage; x<= hit_x; x++){
        for(int y = hit_y-i; i<=hit_y+i; y++){
            try_bunker_destruction(bunker, x, y);
        }
        i++;
    }
}*/

void damageBunker(bunker_t * bunker, int hit_x, int hit_y, int damage){
    if(damage == DAMAGE_CANNONBALL || true){
        try_bunker_destruction(bunker, hit_x, hit_y);
        try_bunker_destruction(bunker, hit_x-1, hit_y);
        try_bunker_destruction(bunker, hit_x+1, hit_y);
        try_bunker_destruction(bunker, hit_x, hit_y-1);
        try_bunker_destruction(bunker, hit_x, hit_y+1);
        try_bunker_destruction(bunker, hit_x-1, hit_y+1);
        try_bunker_destruction(bunker, hit_x+1, hit_y+1);
    }
}


void reset_bunkers(){
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

