#include "rendering.h"
#include "TUM_Draw.h"
#include "player.h"
#include "semphr.h"
#include "cannonball.h"
#include "bunker.h"
#include "invaders.h"
#include "projectiles.h"
#include "stdlib.h"
#include "missile.h"
#include "game.h"
#include "stdio.h"
#include "TUM_Font.h"
#include "mothership.h"

TaskHandle_t RenderingTask;


#define UI_HEIGHT 40

void paintUI(){
    tumDrawLine(0, SCREEN_HEIGHT-UI_HEIGHT, SCREEN_WIDTH-1, 
        SCREEN_HEIGHT-UI_HEIGHT, 2, White);

    int y = SCREEN_HEIGHT-UI_HEIGHT + 5;
    char text[12];

    if(xSemaphoreTake(PlayerHandle, ( TickType_t ) 10 ) == pdTRUE){
        sprintf(text, "SCORE: %d", Player.Points);
        tumDrawText(text, 5, 5, White);
        sprintf(text, "Level: %d", (int)uxSemaphoreGetCount(Level));
        tumDrawText(text, 400, y, White);
        sprintf(text, "%d", Player.Life+1);
        tumDrawText(text, 5, y, White);

        if(Player.Life >= 1){
            tumDrawLoadedImage(Player.img, 23, y);
        }
        if(Player.Life >= 2){
            tumDrawLoadedImage(Player.img, 29 + PLAYER_WIDTH, y);
        }

        xSemaphoreGive(PlayerHandle);
    }
    


    tumDrawText("HI_SCORE: ", 300, 5, White);
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
    tumFontLoadFont("IBMPlexSans-SemiBold.ttf", 18);
    tumFontSelectFontFromName("IBMPlexSans-SemiBold.ttf");


    int fpsCount=0;
    TickType_t lastFPSCount = xTaskGetTickCount();
    TickType_t WakeTime;
    TickType_t diffTime;
    char fps_str[5];
    TickType_t tick_diff;
    TickType_t delay;
    TickType_t startTime;
    TickType_t endTime;


    
    
    for(;;){
        WakeTime = xTaskGetTickCount();
        diffTime = WakeTime - lastFPSCount;
        fpsCount++;
        if(diffTime > configTICK_RATE_HZ){
            sprintf(fps_str, "%d", fpsCount);
            fpsCount = 0;
            lastFPSCount = WakeTime;
        }
        
        startTime = xTaskGetTickCount();



        state_t * state = &States[State];

        state->paintFunc();
        //sprintf(fps_str, "%d", tick_diff);
        tumFontSetSize(30);
        tumDrawText(fps_str, 250, SCREEN_HEIGHT - 36, Yellow);
        tumDrawUpdateScreen(); // Refresh the screen to draw string

        endTime = xTaskGetTickCount();
        tick_diff = endTime - startTime;
        delay = RENDERDELAY - tick_diff;
        vTaskDelay(delay);
    }

}

void drawButton(button_t * button){
    tumFontSetSize(32);
    
    int button_x = (SCREEN_WIDTH - BUTTON_WIDTH)/2;
    tumDrawFilledBox(button_x, button->y_coord, BUTTON_WIDTH, BUTTON_HEIGHT, button->colour);

    int txt_width;
    int txt_height;
    tumGetTextSize(button->txt, &txt_width, &txt_height);
    int txt_x = (SCREEN_WIDTH - txt_width)/2;
    int txt_y = button->y_coord + (BUTTON_HEIGHT-txt_height)/2;
    tumDrawText(button->txt, txt_x, txt_y, Black);

    if(button->hover){
        tumDrawBox(button_x + 4, button->y_coord + 4, BUTTON_WIDTH - 7, BUTTON_HEIGHT - 7, Black);
        tumDrawBox(button_x + 3, button->y_coord + 3, BUTTON_WIDTH - 5, BUTTON_HEIGHT - 5, Black);
        tumDrawBox(button_x + 2, button->y_coord + 2, BUTTON_WIDTH - 3, BUTTON_HEIGHT - 3, Black);
    }
}



void renderInit(void)
{
    if(xTaskCreate(vRender, "RenderingTask", mainGENERIC_STACK_SIZE*2, NULL,
     configMAX_PRIORITIES-1, &RenderingTask) != pdPASS) {
         //PRINT_TASK_ERROR("RenderTask");
         goto err_rendertask;
    }


    return;

    err_rendertask:
        tumDrawExit();
}

void drawButtons(){
    state_t * state = &States[State];

    for(int i=0; i< state->num_buttons; i++){
        drawButton(&state->Buttons[i]);
    }
}

void drawMenuScreen(){
    tumFontSetSize(32);
    tumDrawClear(Black); // Clear screen
    drawButtons();
}

void drawSingleplayerScreen(){
    tumFontSetSize(20);
    tumDrawClear(Black); // Clear screen
    xPaintPlayer();
    xPaintCannonballs();
    xPaintBunkers();

    paint_invaders();
    draw_mothership();

    paintUI();

    //paintMissileTypeA(50, 50, 3);
    xPaintMissiles();
    drawButtons();
}


void drawHighscoreScreen(){
    tumDrawClear(Black); // Clear screen
    tumFontSetSize(32);
    draw_centered_string("HIGHSCORE SINGLEPLAYER", 80, White);
    char txt[15];
    sprintf(txt, "%d", highScoreSingleplayer);
    draw_centered_string(txt, 130, White);
    draw_centered_string("HIGHSCORE MULTIPLAYER", 200, White);
    sprintf(txt, "%d", highScoreMultiplayer);
    draw_centered_string(txt, 250, White);

    drawButtons();
}


void drawCheatScreen(){
    tumDrawClear(Black);
    tumFontSetSize(52);
    draw_centered_string("CHEATS", 5, White);

    tumFontSetSize(26);
    draw_centered_string("UNLIMITED LIVES", 90, White);
    draw_centered_string("UNLIMITED CANNONBALLS", 200, White);

    draw_centered_string("STARTING SCORE", 310, White);
    char * txt[50];
    sprintf(txt, "<<< [w] << [e] <   %d   > [r] >> [t] >>>", Cheats.startingScore);
    draw_centered_string(txt, 350, White);

    draw_centered_string("STARTING Level", 430, White);
    sprintf(txt, "<<< [s] << [d] <   %d   > [f] >> [g] >>>", Cheats.startingLevel);
    draw_centered_string(txt, 470, White);


    drawButtons();
}


void drawGameOverSingleplayerScreen(){
    tumDrawClear(Black);
    tumFontSetSize(52);
    draw_centered_string("GAME OVER", 5, White);
    int Highscore = 0;
    int PlayerScore=0;
    /*
    if(xSemaphoreTake(PlayerHandle, ( TickType_t ) 10 ) == pdTRUE){
        PlayerScore = Player.Points;
        xSemaphoreGive(PlayerHandle);
    }*/
    PlayerScore = Player.Points;
    

    tumFontSetSize(30);
    draw_centered_string("YOUR SCORE", 100, White);

    char txt[15];
    sprintf(txt, "%d", PlayerScore);
    draw_centered_string(txt, 150, White);


    if(xSemaphoreTake(HighscoreHandle, ( TickType_t ) 10 ) == pdTRUE){
        Highscore = highScoreSingleplayer;
        xSemaphoreGive(HighscoreHandle);
    }
    
    draw_centered_string("HIGHSCORE", 300, White);
    sprintf(txt, "%d", Highscore);
    draw_centered_string(txt, 350, White);

    drawButtons();

}


void drawGameOverMultiplayerScreen(){
    tumDrawClear(Black);
    tumFontSetSize(52);
    draw_centered_string("GAME OVER", 5, White);
    int Highscore = 0;
    int PlayerScore=0;
    /*
    if(xSemaphoreTake(PlayerHandle, ( TickType_t ) 10 ) == pdTRUE){
        PlayerScore = Player.Points;
        xSemaphoreGive(PlayerHandle);
    }*/
    PlayerScore = Player.Points;
    

    tumFontSetSize(30);
    draw_centered_string("YOUR SCORE", 100, White);

    char txt[15];
    sprintf(txt, "%d", PlayerScore);
    draw_centered_string(txt, 150, White);


    if(xSemaphoreTake(HighscoreHandle, ( TickType_t ) 10 ) == pdTRUE){
        Highscore = highScoreMultiplayer;
        xSemaphoreGive(HighscoreHandle);
    }
    
    draw_centered_string("HIGHSCORE", 300, White);
    sprintf(txt, "%d", Highscore);
    draw_centered_string(txt, 350, White);

    drawButtons();

}


void draw_centered_string(char * str, int y, int colour){
    int width;
    int height;
    tumGetTextSize(str, &width, &height);
    int x = (SCREEN_WIDTH - width) / 2;
    tumDrawText(str, x, y, colour);
}


void drawPauseScreen(){

}



