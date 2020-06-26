#include "rendering.h"
#include "TUM_Draw.h"
#include "player.h"
#include "semphr.h"
#include "cannonball.h"

TaskHandle_t RenderingTask;

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
