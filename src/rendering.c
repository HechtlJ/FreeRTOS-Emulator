#include "rendering.h"
#include "TUM_Draw.h"
#include "player.h"
#include "semphr.h"
#include "cannonball.h"
#include "bunker.h"
#include "invaders.h"
#include "projectiles.h"
#include "stdlib.h"


TaskHandle_t RenderingTask;


#define UI_HEIGHT 40

void paintUI(){
    tumDrawLine(0, SCREEN_HEIGHT-UI_HEIGHT, SCREEN_WIDTH-1, 
        SCREEN_HEIGHT-UI_HEIGHT, 2, White);

    int y = SCREEN_HEIGHT-UI_HEIGHT + 5;
    char text[12];
    sprintf(text, "%d", Player.Points);
    tumDrawText(text, 5, y, White);
}

void xPaintUI(){

}

void vRender(void *pvParameters){
    //player_t player;
    //player.x_coord = 200;

    // Needed such that Gfx library knows which thread controlls drawing
    // Only one thread can call tumDrawUpdateScreen while and thread can call
    // the drawing functions to draw objects. This is a limitation of the SDL
    // backend.
    tumDrawBindThread();


    for(;;){
        tumDrawClear(Black); // Clear screen
        xPaintPlayer();
        xPaintCannonballs();
        xPaintBunkers();

        paint_invaders();

        paintUI();

        //paintMissileTypeA(50, 50, 3);
        paintMissiles();
        tumDrawUpdateScreen(); // Refresh the screen to draw string


        vTaskDelay((TickType_t)RENDERDELAY);
    }

}



void renderInit(void)
{
    if(xTaskCreate(vRender, "RenderingTask", mainGENERIC_STACK_SIZE*2, NULL,
     configMAX_PRIORITIES, &RenderingTask) != pdPASS) {
         //PRINT_TASK_ERROR("RenderTask");
         goto err_rendertask;
    }

    return;

    err_rendertask:
        tumDrawExit();
}
