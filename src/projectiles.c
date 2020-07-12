#include "rendering.h"
#include "player.h"
#include "semphr.h"
#include "cannonball.h"
#include "projectiles.h"
#include "stdio.h"
#include "bunker.h"
#include "invaders.h"

TaskHandle_t ProjectileTask;

void playerShoot(int * time_to_reload){
    if(*time_to_reload==0){
        xPlayerShoot();
        *time_to_reload = RELOAD_TIME;
    }
}

void vHandleProjectiles(void *pvParameters){
    unsigned int msg;
    int time_to_reload=0;
    int animationTimer = ANIMATION_DURATION;

    for(;;){
        animationTimer--;
        if(animationTimer==0){
            missilesSwitchAnimation();
            animationTimer=ANIMATION_DURATION;
        }
        moveMissiles();
        

        xMoveCannonballs();
        //TODO: One or multiple cannonballs?


        if(CannonBalls[0].exists){
            /*if(hitBunker(CannonBalls[0].x_coord, CannonBalls[0].y_coord, CANNONBALL_WIDTH, CANNONBALL_HEIGHT)){
                CannonBalls[0].exists = false;
            }*/
            if(checkBunkerHit(CannonBalls[0].x_coord, CannonBalls[0].y_coord, true, 0)){
                CannonBalls[0].exists = false;
            }
            if(invaders_check_hit(CannonBalls[0].x_coord, CannonBalls[0].y_coord, CANNONBALL_WIDTH, CANNONBALL_HEIGHT)){
                CannonBalls[0].exists = false;
            }
        }

        if(time_to_reload > 0){
            time_to_reload--;
        }

        if(ProjectilesQueue != NULL){
            if(xQueueReceive(ProjectilesQueue, &msg, (TickType_t) 10) == pdPASS){
                switch(msg){
                    case CMD_PLAYER_SHOOT: playerShoot(&time_to_reload); break;
                    default: printf("Unknown Command\n"); break;
                }
            }
        }        

        vTaskDelay((TickType_t) (1000/60));
    }

}

void projectileInit(){
    printf("starting projectile task");
    if(xTaskCreate(vHandleProjectiles, "ProjectileTask", mainGENERIC_STACK_SIZE*2, NULL,
     configMAX_PRIORITIES-1, &ProjectileTask) != pdPASS) {
         //PRINT_TASK_ERROR("RenderTask");
         //goto err_rendertask;
        printf("error in creating projectile task\n");
    }
    ProjectilesQueue = xQueueCreate(2, sizeof(unsigned int));
    if(ProjectilesQueue == NULL){
        // error handling
    }

    for(int i=0; i<NUM_MAX_MISSILES; i++){
        Missiles[i].exists = false;
    }

    Missiles[0].exists=true;
    Missiles[0].x_coord = 50;
    Missiles[0].y_coord = 50;
    Missiles[0].animation = 0;
    Missiles[0].type = MISSILE_TYPE_A;

}



void paintMissile(missile_t * missile){
    if(!missile->exists)
        return;
    switch (missile->type){
    case MISSILE_TYPE_A:
            paintMissileTypeA(missile->x_coord, missile->y_coord, missile->animation);
            break;
    
    default:
        break;
    }
}


void paintMissileTypeA(int x, int y, char animation){
    if(animation==0){
        tumDrawFilledBox(x+2, y, 1, MISSILE_HEIGHT-1, White);
    }else if(animation==1){
        tumDrawFilledBox(x+2, y, 1, MISSILE_HEIGHT-1, White);
        tumDrawFilledBox(x, y+12, MISSILE_WIDTH-1, 1, White);
    }else if(animation==2){
        tumDrawFilledBox(x+2, y, 1, MISSILE_HEIGHT-1, White);
        tumDrawFilledBox(x, y+6, MISSILE_WIDTH-1, 1, White);
    }else if(animation==3){
        tumDrawFilledBox(x+2, y, 1, MISSILE_HEIGHT-1, White);
        tumDrawFilledBox(x, y, MISSILE_WIDTH-1, 1, White);
    }else{
        //error
        return;
    }
}

void missilesSwitchAnimation(){
    for(int i=0; i<NUM_MAX_MISSILES; i++){
        Missiles[i].animation++;
        Missiles[i].animation = Missiles[i].animation % 4;
    }
}

void moveMissiles(){
    int speed;
    for(int i=0; i<NUM_MAX_MISSILES; i++){
        if(Missiles[i].exists){
            switch(Missiles[i].type){
                case MISSILE_TYPE_A: speed = MISSILE_TYPE_A_SPEED; break;
                case MISSILE_TYPE_B: speed = MISSILE_TYPE_B_SPEED; break;
                case MISSILE_TYPE_C: speed = MISSILE_TYPE_C_SPEED; break;
                default: break;
            }
            Missiles[i].y_coord += speed;
            if(Missiles[i].y_coord > MISSILE_MAX_Y){
                Missiles[i].exists = false;
            }
        }
    }
}

void paintMissiles(){
    for(int i=0; i<NUM_MAX_MISSILES; i++){
        paintMissile(&Missiles[i]);
    }
}