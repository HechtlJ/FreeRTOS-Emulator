#ifndef RENDERING_H
#define RENDERING_H

#include "FreeRTOS.h"
#include "task.h"
#include "stdbool.h"


#define mainGENERIC_PRIORITY (tskIDLE_PRIORITY)
#define mainGENERIC_STACK_SIZE ((unsigned short)2560)


#define FPS 60
#define RENDERDELAY 1000/FPS

#define STATE_MENU 0
#define STATE_PLAYING 1
#define STATE_PAUSE 2
#define STATE_HIGHSCORE 3


#define BUTTON_WIDTH 300
#define BUTTON_HEIGHT 50

typedef void (*function)();

typedef struct {
    char * txt;
    int y_coord;
    function action;
    bool active;
    bool hover;
}button_t;



extern TaskHandle_t RenderingTask;

void renderInit(void);

void drawMenuScreen();
void drawHighscoreScreen();

void drawButton(button_t * button);
void drawButtons();


#endif /* RENDERING_H */