#include "button_input.h"
#include "TUM_Event.h"
#include "semphr.h"
#include <SDL2/SDL_scancode.h>
#include "player.h"
#include "projectiles.h"
#include "rendering.h"
#include "TUM_Draw.h"
#include "stdio.h"
#include "level.h"

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



void handleButton(button_t * button){
    int mouse_x = tumEventGetMouseX();
    int mouse_y = tumEventGetMouseY();
    int min_x = (SCREEN_WIDTH - BUTTON_WIDTH)/2;
    int max_x = (SCREEN_WIDTH + BUTTON_WIDTH)/2;

    if(mouse_x < min_x || mouse_x > max_x){
        button->hover = false;
        return; 
    }
    if(button->active){
        int min_y = button->y_coord;
        int max_y = button->y_coord + BUTTON_HEIGHT;
        if(mouse_y >= min_y && mouse_y <= max_y){
            // hit
            if(tumEventGetMouseLeft()){
                button->hover = false;
                button->action();
            }else{
                button->hover = true;
            }
        }else{
            button->hover = false;
        }
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
            if(buttons.buttons[SDL_SCANCODE_UP]){
                unsigned int msg = CMD_PLAYER_SHOOT;
                /* Block time of 0 says don't block if the queue is already full.*/
                if( xQueueSend( ProjectilesQueue, ( void * ) &msg, ( TickType_t ) 0 ) != pdPASS )
                {
                    /* Failed to post the message, even after 10 ticks. */
                }
            }
            state_t * state = &States[State];
            for(int i=0; i<state->num_buttons; i++){
                handleButton(&state->Buttons[i]);
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

