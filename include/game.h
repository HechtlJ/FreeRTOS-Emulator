#ifndef LEVEL_H
#define LEVEL_H

#include "rendering.h"
#include "semphr.h"


typedef struct{
    button_t * Buttons;
    int num_buttons;
    function paintFunc;
    function switchTo;
}state_t;


#define STATE_MAIN_MENU 0
#define STATE_SINGLEPLAYER 1
#define STATE_MULTIPLAYER 2
#define STATE_PAUSE_SINGLEPLAYER 3
#define STATE_PAUSE_MULTIPLAYER 4
#define STATE_GAMEOVER_SINGLEPLAYER 5
#define STATE_GAMEOVER_MULTYPLAYER 6
#define STATE_HIGHSCORE 7
#define STATE_CHEATS 8
#define NUM_STATES 9

state_t States[NUM_STATES];
int State;


SemaphoreHandle_t  Level;


void init_levels();
void next_level();

#define LEVEL_CYCLE 11
#define LEVEL_INVADER_DISTANCE 10

void init_level(int level);

int highScoreSingleplayer;
int highScoreMultiplayer;
SemaphoreHandle_t HighscoreHandle;

//TODO add mutex for states


void initStates();

void initMainMenu();
void initHighscore();
void initSingleplayer();
void initMultiplayer();
void initPause();
void initCheats();
void initGameOver();

void switchToMainMenu();
void switchToHighscore();
void switchToSingleplayer();
void switchToMultiplayer();
void switchToPause();
void switchToCheats();
void switchToGameOver();


void startSingleplayer();
void startMultiplayer();


void enable_buttons(state_t * state);
void disable_buttons(state_t * state);

void draw_centered_string(char * str, int y, int colour);


typedef struct{
    bool unlimitedLives;
    bool unlimitedCannonballs;
    int startingScore;
    int startingLevel;
} cheats_t;

cheats_t Cheats;
SemaphoreHandle_t CheatsHandle;

void xCheatsToggleLives();
void xCheatsToggleCannonballs();
void xCheatsIncreaseStartingScore();
void xCheatsDecreaseStartingScore();


void xTogglePause();


#endif /* LEVEL_H */