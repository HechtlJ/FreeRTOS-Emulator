#include "rendering.h"
#include "player.h"
#include "semphr.h"
#include "cannonball.h"
#include "projectiles.h"
#include "stdio.h"
#include "bunker.h"
#include "invaders.h"
#include "missile.h"
#include "game.h"

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
            xMissilesSwitchAnimation();
            animationTimer=ANIMATION_DURATION;
        }
        xMoveMissiles();
        

        xMoveCannonballs();
        //TODO: One or multiple cannonballs?



        for(int i=0; i<MAX_NUM_CANNONBALLS; i++){
            if(CannonBalls[i].exists){
            /*if(hitBunker(CannonBalls[0].x_coord, CannonBalls[0].y_coord, CANNONBALL_WIDTH, CANNONBALL_HEIGHT)){
                CannonBalls[0].exists = false;
            }*/
            if(checkBunkerHit(CannonBalls[i].x_coord, CannonBalls[i].y_coord, true, 0)){
                CannonBalls[i].exists = false;
            }
            if(invaders_check_hit(CannonBalls[i].x_coord, CannonBalls[i].y_coord, CANNONBALL_WIDTH, CANNONBALL_HEIGHT)){
                CannonBalls[i].exists = false;
                if(invaders_all_dead()){
                    next_level();
                }
            }
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
}






