#ifndef PROJECTILES_H
#define PROJECTILES_H

#include "FreeRTOS.h"
#include "task.h"
#include "stdbool.h"

#define mainGENERIC_PRIORITY (tskIDLE_PRIORITY)
#define mainGENERIC_STACK_SIZE ((unsigned short)2560)

#define CMD_PLAYER_SHOOT 1


extern TaskHandle_t ProjectileTask;

QueueHandle_t ProjectilesQueue;


#define MISSILE_WIDTH 6
#define MISSILE_HEIGHT 14
#define MISSILE_TYPE_A 1
#define MISSILE_TYPE_A_SPEED 5
#define MISSILE_TYPE_B 2
#define MISSILE_TYPE_B_SPEED 5
#define MISSILE_TYPE_C 3
#define MISSILE_TYPE_C_SPEED 8
#define NUM_MAX_MISSILES 3

#define MISSILE_MAX_Y 600


#define ANIMATION_DURATION 15

typedef struct {
    unsigned int x_coord;
    unsigned int y_coord;
    unsigned char animation;
    unsigned char type;
    bool exists;
} missile_t;



missile_t Missiles[NUM_MAX_MISSILES];






void projectileInit(void);

void paintMissile(missile_t * missile);
void paintMissiles();

void paintMissileTypeA(int x, int y, char animation);
void missilesSwitchAnimation();

void moveMissiles();

#endif /* PROJECTILES_H */