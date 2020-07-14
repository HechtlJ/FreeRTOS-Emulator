#ifndef MOTHERSHIP_H
#define MOTHERSHIP_H


#include "FreeRTOS.h"
#include "task.h"
#include "AsyncIO.h"
#include "queue.h"
#include "stdbool.h"

#define mainGENERIC_PRIORITY (tskIDLE_PRIORITY)
#define mainGENERIC_STACK_SIZE ((unsigned short)2560)

extern TaskHandle_t MothershipTask;
QueueHandle_t OpponentQueue;

int msg;
//static SemaphoreHandle_t HandleUDP = NULL;

#define MOTHERSHIP_Y_COORD 50
#define MOTHERSHIP_WIDTH 64
#define MOTHERSHIP_HEIGHT 28
#define MOTHERSHIP_SPEED 4
#define MOTHERSHIP_MAX_WAIT 260
#define MOTHERSHIP_POINTS 400


typedef struct {
    int x_coord;
    void * img;
    bool exists;
    int movement;
    int wait;
    int cycle;
} mothership_t;

mothership_t Mothership;

void vMothershipTask(void * pvParameters);


void opponent_pause();
void opponent_resume();
void opponent_set_difficulty(int difficulty);
void opponent_send_player_position();
void opponent_send_player_attacking();
void opponent_send_player_passive();



void handle_mothership();
void reset_mothership();
void create_mothership();
void kill_mothership();



void handle_opponent();
void init_opponent();
void exit_opponent();

int mothership_check_hit(int x, int y, int height);

void draw_mothership();

#endif /* MOTHERSHIP_H */