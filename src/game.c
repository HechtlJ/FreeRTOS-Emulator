#include "player.h"
#include "rendering.h"
#include "button_input.h"
#include "cannonball.h"
#include "projectiles.h"
#include "bunker.h"
#include "invaders.h"
#include "missile.h"
#include "game.h"
#include "mothership.h"

void test_func(){
    printf("testfunc \n");
}

void init_levels(){
    Level = xSemaphoreCreateCounting(9999, 1);
}

void next_level(){
    xSemaphoreGive(Level);  //increasing Level by 1

    init_missiles();
    int offset = 0; //TODO
    invader_reset(offset);
}


void game_over();


void initStates(){
    State = STATE_MAIN_MENU;
    HighscoreHandle = xSemaphoreCreateMutex();
    CheatsHandle = xSemaphoreCreateMutex();
    Cheats.startingLevel = 1;
    Cheats.startingScore = 0;
    Cheats.unlimitedCannonballs = false;
    Cheats.unlimitedLives = false;
    highScoreSingleplayer = 0;
    highScoreMultiplayer = 0;
    initMainMenu();
    initHighscore();
    initSingleplayer();
    initMultiplayer();
    initPause();
    initCheats();
}

void initMainMenu(){
    state_t * state = &States[STATE_MAIN_MENU];
    state->num_buttons = 4;
    state->Buttons = malloc(sizeof(button_t) * state->num_buttons);

    state->Buttons[0].txt = "SINGLEPLAYER";
    state->Buttons[0].y_coord = 100;
    state->Buttons[0].active = true;
    state->Buttons[0].action = switchToSingleplayer;
    state->Buttons[0].colour = Red;
    

    state->Buttons[1].txt = "MULTIPLAYER";
    state->Buttons[1].y_coord = 200;
    state->Buttons[1].active = true;
    state->Buttons[1].action = switchToMultiplayer;
    state->Buttons[1].colour = Red;

    state->Buttons[2].txt = "HIGHSCORES";
    state->Buttons[2].y_coord = 300;
    state->Buttons[2].active = true;
    state->Buttons[2].action = switchToHighscore;
    state->Buttons[2].colour = Red;

    state->Buttons[3].txt = "CHEATS";
    state->Buttons[3].y_coord = 400;
    state->Buttons[3].active = true;
    state->Buttons[3].action = switchToCheats;
    state->Buttons[3].colour = Red;


    state->paintFunc = drawMenuScreen;

}


void initHighscore(){
    state_t * state = &States[STATE_HIGHSCORE];
    state->num_buttons = 1;
    state->Buttons = malloc(sizeof(button_t) * state->num_buttons);

    state->Buttons[0].txt = "BACK";
    state->Buttons[0].y_coord = 400;
    state->Buttons[0].active = false;
    state->Buttons[0].hover = false;
    state->Buttons[0].action = switchToMainMenu;
    state->Buttons[0].colour = Red;

    state->paintFunc = drawHighscoreScreen;
}


void initSingleplayer(){
    state_t * state = &States[STATE_SINGLEPLAYER];
    state->num_buttons = 0;
    state->paintFunc = drawSingleplayerScreen;
}


void initMultiplayer(){
    state_t * state = &States[STATE_MULTIPLAYER];
    state->num_buttons = 0;
    state->paintFunc = drawSingleplayerScreen;
}


void initPause(){
    state_t * state = &States[STATE_PAUSE];
    state->num_buttons = 2;
    state->Buttons = malloc(sizeof(button_t) * state->num_buttons);

    state->Buttons[0].txt = "Continue";
    state->Buttons[0].y_coord = 150;
    state->Buttons[0].active = false;
    state->Buttons[0].hover = false;
    state->Buttons[0].action = switchToSingleplayer;
    state->Buttons[0].colour = Red;

    state->Buttons[1].txt = "QUIT TO MENU";
    state->Buttons[1].y_coord = 250;
    state->Buttons[1].active = false;
    state->Buttons[1].hover = false;
    state->Buttons[1].action = switchToMainMenu;
    state->Buttons[1].colour = Red;

    state->paintFunc = drawSingleplayerScreen;
}

void initCheats(){
    state_t * state = &States[STATE_CHEATS];
    state->num_buttons = 3;
    state->Buttons = malloc(sizeof(button_t) * state->num_buttons);

    state->Buttons[0].txt = "Off";
    state->Buttons[0].y_coord = 130;
    state->Buttons[0].active = false;
    state->Buttons[0].hover = false;
    state->Buttons[0].action = xCheatsToggleLives;
    state->Buttons[0].colour = Red;

    state->Buttons[1].txt = "Off";
    state->Buttons[1].y_coord = 240;
    state->Buttons[1].active = false;
    state->Buttons[1].hover = false;
    state->Buttons[1].action = xCheatsToggleCannonballs;
    state->Buttons[1].colour = Red;

    state->Buttons[2].txt = "BACK";
    state->Buttons[2].y_coord = 550;
    state->Buttons[2].active = false;
    state->Buttons[2].hover = false;
    state->Buttons[2].action = switchToMainMenu;
    state->Buttons[2].colour = Red;

    state->paintFunc = drawCheatScreen;
}


void switchToMainMenu(){
    disable_buttons(&States[State]);
    State = STATE_MAIN_MENU;
    enable_buttons(&States[State]);
}


void switchToHighscore(){
    disable_buttons(&States[State]);
    State = STATE_HIGHSCORE;
    enable_buttons(&States[State]);
}


void switchToSingleplayer(){
    disable_buttons(&States[State]);
    State = STATE_SINGLEPLAYER;
    enable_buttons(&States[State]);
    vTaskResume(InvaderTask);
    vTaskResume(ProjectileTask);
    vTaskResume(MothershipTask);
    opponent_resume();
}


void switchToMultiplayer(){
    disable_buttons(&States[State]);
    State = STATE_MULTIPLAYER;
    enable_buttons(&States[State]);
    vTaskResume(InvaderTask);
    vTaskResume(ProjectileTask);
    vTaskResume(MothershipTask);
    opponent_resume();
}


void switchToPause(){
    disable_buttons(&States[State]);
    State = STATE_PAUSE;
    enable_buttons(&States[State]);
    vTaskSuspend(InvaderTask);
    vTaskSuspend(ProjectileTask);
    vTaskSuspend(MothershipTask);
    opponent_pause();
}


void switchToCheats(){
    disable_buttons(&States[State]);
    State = STATE_CHEATS;
    enable_buttons(&States[State]);
}



void startSingleplayer(){
    xResetPlayer();
    xResetCannonballs();
    //TODO
    //xResetBunkers;
    reset_bunkers();
    xResetMissiles();
    invader_reset(0);
    // init_level???
}


void enable_buttons(state_t * state){
    for(int i=0; i<state->num_buttons; i++){
        state->Buttons[i].active=true;
    }
}


void disable_buttons(state_t * state){
    for(int i=0; i<state->num_buttons; i++){
        state->Buttons[i].active=false;
    }
}


void xCheatsToggleLives(){
    if (xSemaphoreTake(CheatsHandle, (TickType_t)10) == pdTRUE) {
        if(Cheats.unlimitedLives==true){
            Cheats.unlimitedLives = false;
            States[STATE_CHEATS].Buttons[0].colour = Red;
            States[STATE_CHEATS].Buttons[0].txt = "OFF";
        }else{
            Cheats.unlimitedLives = true;
            States[STATE_CHEATS].Buttons[0].colour = Green;
            States[STATE_CHEATS].Buttons[0].txt = "ON";
        }
		xSemaphoreGive(CheatsHandle);
	} 
}


void xCheatsToggleCannonballs(){
    if (xSemaphoreTake(CheatsHandle, (TickType_t)10) == pdTRUE) {
        if(Cheats.unlimitedCannonballs==true){
            Cheats.unlimitedCannonballs = false;
            States[STATE_CHEATS].Buttons[1].colour = Red;
            States[STATE_CHEATS].Buttons[1].txt = "OFF";
            if(xSemaphoreTake(CannonballHandle, (TickType_t)10) == pdTRUE) {
                maxNumCannonballs = 1;
                xSemaphoreGive(CannonballHandle);
            }
        }else{
            Cheats.unlimitedCannonballs = true;
            States[STATE_CHEATS].Buttons[1].colour = Green;
            States[STATE_CHEATS].Buttons[1].txt = "ON";
            if(xSemaphoreTake(CannonballHandle, (TickType_t)10) == pdTRUE) {
                maxNumCannonballs = 20;
                xSemaphoreGive(CannonballHandle);
            }
        }
		xSemaphoreGive(CheatsHandle);
	} 
}

void xCheatsIncreaseStartingScore(){
    if (xSemaphoreTake(CheatsHandle, (TickType_t)10) == pdTRUE) {
        Cheats.startingScore++;
		xSemaphoreGive(CheatsHandle);
	} 
}


void xCheatsDecreaseStartingScore(){
    if (xSemaphoreTake(CheatsHandle, (TickType_t)10) == pdTRUE) {
        Cheats.startingScore--;
        if(Cheats.startingScore<0){
            Cheats.startingScore = 0;
        }
		xSemaphoreGive(CheatsHandle);
	} 
}


void xTogglePause(){
    switchToPause();
}