#include "button_input.h"
#include "TUM_Event.h"
#include "semphr.h"
#include <SDL2/SDL_scancode.h>
#include "player.h"
#include "projectiles.h"
#include "rendering.h"
#include "TUM_Draw.h"
#include "stdio.h"
#include "game.h"



#define mainGENERIC_PRIORITY (tskIDLE_PRIORITY)
#define mainGENERIC_STACK_SIZE ((unsigned short)2560)


TaskHandle_t ButtonTask;
int mouseDebounce;


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

void buttonInit(){
    // Initializing all the Key Actions:
    for(int i=0; i<NUM_KEY_ACTIONS; i++){
        for(int l=0; l<NUM_STATES; l++){
            KeyActions[i].activeForState[l] = false;
        }
    }
    
    for(int i=0; i<NUM_STATES; i++){
        KeyActions[ExitAction].activeForState[i] = true;
    }
    KeyActions[ExitAction].callback=clean_exit;
    KeyActions[ExitAction].debounceDelay=0;
    KeyActions[ExitAction].lastButtonState=false;
    KeyActions[ExitAction].waitTime=0;
    KeyActions[ExitAction].keycode = KEYCODE(Q);

    KeyActions[PauseAction].activeForState[STATE_SINGLEPLAYER] = true;
    KeyActions[PauseAction].activeForState[STATE_MULTIPLAYER] = true;
    KeyActions[PauseAction].activeForState[STATE_PAUSE_SINGLEPLAYER] = true;
    KeyActions[PauseAction].activeForState[STATE_PAUSE_MULTIPLAYER] = true;
    KeyActions[PauseAction].callback=xTogglePause;
    KeyActions[PauseAction].debounceDelay=5;
    KeyActions[PauseAction].lastButtonState=false;
    KeyActions[PauseAction].waitTime=0;
    KeyActions[PauseAction].keycode = KEYCODE(P);

    KeyActions[MovePlayerLeftAction].activeForState[STATE_SINGLEPLAYER] = true;
    KeyActions[MovePlayerLeftAction].activeForState[STATE_MULTIPLAYER] = true;
    KeyActions[MovePlayerLeftAction].callback=xPlayerMoveLeft;
    KeyActions[MovePlayerLeftAction].debounceDelay=0;
    KeyActions[MovePlayerLeftAction].lastButtonState=false;
    KeyActions[MovePlayerLeftAction].waitTime=0;
    KeyActions[MovePlayerLeftAction].keycode = SDL_SCANCODE_LEFT;

    KeyActions[MovePlayerRightAction].activeForState[STATE_SINGLEPLAYER] = true;
    KeyActions[MovePlayerRightAction].activeForState[STATE_MULTIPLAYER] = true;
    KeyActions[MovePlayerRightAction].callback=xPlayerMoveRight;
    KeyActions[MovePlayerRightAction].debounceDelay=0;
    KeyActions[MovePlayerRightAction].lastButtonState=false;
    KeyActions[MovePlayerRightAction].waitTime=0;
    KeyActions[MovePlayerRightAction].keycode = SDL_SCANCODE_RIGHT;

    KeyActions[PlayerShootAction].activeForState[STATE_SINGLEPLAYER] = true;
    KeyActions[PlayerShootAction].activeForState[STATE_MULTIPLAYER] = true;
    KeyActions[PlayerShootAction].callback=shootCallback;
    KeyActions[PlayerShootAction].debounceDelay=0;
    KeyActions[PlayerShootAction].lastButtonState=false;
    KeyActions[PlayerShootAction].waitTime=0;
    KeyActions[PlayerShootAction].keycode = SDL_SCANCODE_SPACE;

    KeyActions[CheatsDecreaseStartingScoreAction].activeForState[STATE_CHEATS] = true;
    KeyActions[CheatsDecreaseStartingScoreAction].callback=xCheatsDecreaseStartingScore;
    KeyActions[CheatsDecreaseStartingScoreAction].debounceDelay=0;
    KeyActions[CheatsDecreaseStartingScoreAction].lastButtonState=false;
    KeyActions[CheatsDecreaseStartingScoreAction].waitTime=0;
    KeyActions[CheatsDecreaseStartingScoreAction].keycode = KEYCODE(W);

    KeyActions[CheatsIncreaseStartingScoreAction].activeForState[STATE_CHEATS] = true;
    KeyActions[CheatsIncreaseStartingScoreAction].callback=xCheatsIncreaseStartingScore;
    KeyActions[CheatsIncreaseStartingScoreAction].debounceDelay=0;
    KeyActions[CheatsIncreaseStartingScoreAction].lastButtonState=false;
    KeyActions[CheatsIncreaseStartingScoreAction].waitTime=0;
    KeyActions[CheatsIncreaseStartingScoreAction].keycode = KEYCODE(T);

    KeyActions[CheatsDecreaseStartingScoreActionSlow].activeForState[STATE_CHEATS] = true;
    KeyActions[CheatsDecreaseStartingScoreActionSlow].callback=xCheatsDecreaseStartingScore;
    KeyActions[CheatsDecreaseStartingScoreActionSlow].debounceDelay=1;
    KeyActions[CheatsDecreaseStartingScoreActionSlow].lastButtonState=false;
    KeyActions[CheatsDecreaseStartingScoreActionSlow].waitTime=0;
    KeyActions[CheatsDecreaseStartingScoreActionSlow].keycode = KEYCODE(E);

    KeyActions[CheatsIncreaseStartingScoreActionSlow].activeForState[STATE_CHEATS] = true;
    KeyActions[CheatsIncreaseStartingScoreActionSlow].callback=xCheatsIncreaseStartingScore;
    KeyActions[CheatsIncreaseStartingScoreActionSlow].debounceDelay=1;
    KeyActions[CheatsIncreaseStartingScoreActionSlow].lastButtonState=false;
    KeyActions[CheatsIncreaseStartingScoreActionSlow].waitTime=0;
    KeyActions[CheatsIncreaseStartingScoreActionSlow].keycode = KEYCODE(R);


    buttons.lock = xSemaphoreCreateMutex(); // Locking mechanism
    if (!buttons.lock) {
        //PRINT_ERROR("Failed to create buttons lock");
    }

    // Starting the Task
    if(xTaskCreate(vHandleButtons, "ButtonTask", mainGENERIC_STACK_SIZE*2, NULL,
     configMAX_PRIORITIES, &ButtonTask) != pdPASS) {
         //PRINT_TASK_ERROR("RenderTask");
         //goto err_rendertask;
    }

}

void handleKeyAction(key_action_t * KeyAction){
    if(KeyAction->activeForState[State]==false){
        return;
    }
    if(KeyAction->debounceDelay==0){
        if(buttons.buttons[KeyAction->keycode]){
            KeyAction->callback();
            return;
        }
    }

    if(KeyAction->waitTime==0){
        if(buttons.buttons[KeyAction->keycode]){
            if(KeyAction->lastButtonState == false){
                KeyAction->lastButtonState=true;
                KeyAction->callback();
                KeyAction->waitTime = KeyAction->debounceDelay;
            }
        }else{
            if(KeyAction->lastButtonState==true){
                KeyAction->lastButtonState=false;
                KeyAction->waitTime = KeyAction->debounceDelay;
            }
        }
    }else{
        KeyAction->waitTime--;
    }
}



void handleButton(button_t * button){
    if(mouseDebounce>0){
        return;
    }
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
                mouseDebounce = MOUSE_DEBOUNCE_LENGTH;
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

    mouseDebounce=0;
    for(;;){
        tumEventFetchEvents(FETCH_EVENT_NONBLOCK | FETCH_EVENT_NO_GL_CHECK); // Query events backend for new events, ie. button presses
        xGetButtonInput(); // Update global input

        
        if(mouseDebounce>0){
            mouseDebounce--;
        }

        if (xSemaphoreTake(buttons.lock, 0) == pdTRUE) {
            for(int i=0; i<NUM_KEY_ACTIONS; i++){
                handleKeyAction(&KeyActions[i]);
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





void clean_exit(){
    exit(EXIT_SUCCESS);
}


void shootCallback(){
    unsigned int msg = CMD_PLAYER_SHOOT;
    if( xQueueSend( ProjectilesQueue, ( void * ) &msg, ( TickType_t ) 0 ) != pdPASS )
    {
        // Failed to post the message, even after 10 ticks. 
    }
}

