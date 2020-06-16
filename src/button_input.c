#include "button_input.h"
#include "TUM_Event.h"
#include "semphr.h"
#include <SDL2/SDL_scancode.h>
#include "player.h"

TaskHandle_t ButtonTask;


typedef struct buttons_buffer {
    unsigned char buttons[SDL_NUM_SCANCODES];
    SemaphoreHandle_t lock;
} buttons_buffer_t;

static buttons_buffer_t buttons = { 0 };

#define KEYCODE(CHAR) SDL_SCANCODE_##CHAR


void xGetButtonInput(void)
{
    if (xSemaphoreTake(buttons.lock, 0) == pdTRUE) {
        xQueueReceive(buttonInputQueue, &buttons.buttons, 0);
        xSemaphoreGive(buttons.lock);
    }
}



void vHandleButtons(void *pvParameters){


    for(;;){
        tumEventFetchEvents(FETCH_EVENT_NONBLOCK | FETCH_EVENT_NO_GL_CHECK); // Query events backend for new events, ie. button presses
        xGetButtonInput(); // Update global input

        if (xSemaphoreTake(buttons.lock, 0) == pdTRUE) {
            if (buttons.buttons[KEYCODE(
                                    Q)]) { // Equiv to SDL_SCANCODE_Q
                exit(EXIT_SUCCESS);
            }else if(buttons.buttons[SDL_SCANCODE_LEFT]){
                xPlayerMoveLeft();
            }else if(buttons.buttons[SDL_SCANCODE_RIGHT]){
                xPlayerMoveRight();
            }
            xSemaphoreGive(buttons.lock);
        }
        vTaskDelay((TickType_t) (1000/30));
    }

}

void buttonInit(){
    if(xTaskCreate(vHandleButtons, "ButtonTask", mainGENERIC_STACK_SIZE*2, NULL,
     configMAX_PRIORITIES-1, &ButtonTask) != pdPASS) {
         //PRINT_TASK_ERROR("RenderTask");
         //goto err_rendertask;
    }



    buttons.lock = xSemaphoreCreateMutex(); // Locking mechanism
    if (!buttons.lock) {
        //PRINT_ERROR("Failed to create buttons lock");
    }
}