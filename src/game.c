#include "player.h"
#include "rendering.h"
#include "button_input.h"
#include "cannonball.h"
#include "projectiles.h"
#include "bunker.h"
#include "invaders.h"
#include "missile.h"
#include "game.h"

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
    initMainMenu();
    initHighscore();
    initSingleplayer();
    initPause();
}

void initMainMenu(){
    state_t * state = &States[STATE_MAIN_MENU];
    state->num_buttons = 4;
    state->Buttons = malloc(sizeof(button_t) * 4);

    state->Buttons[0].txt = "SINGLEPLAYER";
    state->Buttons[0].y_coord = 100;
    state->Buttons[0].active = true;
    state->Buttons[0].action = switchToSingleplayer;
    

    state->Buttons[1].txt = "MULTIPLAYER";
    state->Buttons[1].y_coord = 200;
    state->Buttons[1].active = true;
    state->Buttons[1].action = test_func;

    state->Buttons[2].txt = "HIGHSCORES";
    state->Buttons[2].y_coord = 300;
    state->Buttons[2].active = true;
    state->Buttons[2].action = switchToHighscore;

    state->Buttons[3].txt = "CHEATS";
    state->Buttons[3].y_coord = 400;
    state->Buttons[3].active = true;
    state->Buttons[3].action = test_func;


    state->paintFunc = drawMenuScreen;

}


void initHighscore(){
    state_t * state = &States[STATE_HIGHSCORE];
    state->num_buttons = 1;
    state->Buttons = malloc(sizeof(button_t) * 1);

    state->Buttons[0].txt = "BACK";
    state->Buttons[0].y_coord = 400;
    state->Buttons[0].active = false;
    state->Buttons[0].hover = false;
    state->Buttons[0].action = switchToMainMenu;

    state->paintFunc = drawHighscoreScreen;
}

void initSingleplayer(){
    state_t * state = &States[STATE_SINGLEPLAYER];
    state->num_buttons = 0;
    state->paintFunc = drawSingleplayerScreen;
}


void initPause(){
    state_t * state = &States[STATE_PAUSE];
    state->num_buttons = 2;
    state->Buttons = malloc(sizeof(button_t) * 2);

    state->Buttons[0].txt = "Continue";
    state->Buttons[0].y_coord = 150;
    state->Buttons[0].active = false;
    state->Buttons[0].hover = false;
    state->Buttons[0].action = switchToSingleplayer;

    state->Buttons[1].txt = "QUIT TO MENU";
    state->Buttons[1].y_coord = 250;
    state->Buttons[1].active = false;
    state->Buttons[1].hover = false;
    state->Buttons[1].action = switchToMainMenu;

    state->paintFunc = drawSingleplayerScreen;
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
}

void switchToPause(){
    disable_buttons(&States[State]);
    State = STATE_PAUSE;
    enable_buttons(&States[State]);
    vTaskSuspend(InvaderTask);
    vTaskSuspend(ProjectileTask);
}


void startSingleplayer(){
    xResetPlayer();
    xResetCannonballs;
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