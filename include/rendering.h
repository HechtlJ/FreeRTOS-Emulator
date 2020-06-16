#ifndef RENDERING_H
#define RENDERING_H

#include "FreeRTOS.h"
#include "task.h"


#define mainGENERIC_PRIORITY (tskIDLE_PRIORITY)
#define mainGENERIC_STACK_SIZE ((unsigned short)2560)


#define FPS 60
#define RENDERDELAY 1000/FPS



extern TaskHandle_t RenderingTask;

void renderInit(void);



#endif /* RENDERING_H */