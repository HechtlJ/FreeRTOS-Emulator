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



#define ANIMATION_DURATION 10




void projectileInit(void);

#endif /* PROJECTILES_H */