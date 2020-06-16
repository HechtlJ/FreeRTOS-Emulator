#ifndef BUTTON_INPUT_H
#define BUTTON_INPUT_H

#include "FreeRTOS.h"
#include "task.h"

#define mainGENERIC_PRIORITY (tskIDLE_PRIORITY)
#define mainGENERIC_STACK_SIZE ((unsigned short)2560)


extern TaskHandle_t ButtonTask;


void buttonInit();




#endif /* BUTTON_INPUT_H */