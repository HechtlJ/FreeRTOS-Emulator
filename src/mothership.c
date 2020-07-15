#include "mothership.h"

#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "TUM_Draw.h"
#include "string.h"

#include "player.h"
#include "game.h"
#include "cannonball.h"
#include "semphr.h"

#define UDP_BUFFER_SIZE 1024
#define UDP_RECEIVE_PORT 1234
#define UDP_TRANSMIT_PORT 1235

aIO_handle_t udp_soc_receive = NULL, udp_soc_transmit = NULL;
TaskHandle_t MothershipTask;

void handle(size_t read_size, char *buffer, void *args)
{
	//BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	int msg;
	if (strcmp(buffer, "INC") == 0) {
		msg = MOTHERSHIP_SPEED;
	} else if (strcmp(buffer, "DEC") == 0) {
		msg = -MOTHERSHIP_SPEED;
	} else {
		msg = 0;
	}
	//xQueueSendFromISR(OpponentQueue, (void *)&msg, &xHigherPriorityTaskWoken);
	xQueueSendFromISR(OpponentQueue, (void *)&msg, NULL);
	//portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void init_udp()
{
    srand(time(NULL));
	Mothership.img =
		tumDrawLoadScaledImage("../resources/img/mothership.bmp", 1.0);
	Mothership.wait = MOTHERSHIP_MAX_WAIT;
    Mothership.x_coord = 50;
    Mothership.exists = false;
    
	//HandleUDP = xSemaphoreCreateMutex();
	OpponentQueue = xQueueCreate(3, sizeof(int));
	static char buf[50];
	char *addr = NULL; // Loopback
	in_port_t port = UDP_RECEIVE_PORT;
	udp_soc_receive =
		aIOOpenUDPSocket(addr, port, UDP_BUFFER_SIZE, handle, NULL);

	printf("UDP socket opened on port %d\n", port);

	aIOSocketPut(UDP, NULL, UDP_TRANSMIT_PORT, buf, strlen(buf));

	if (xTaskCreate(vMothershipTask, "MothershipTask",
			mainGENERIC_STACK_SIZE * 2, NULL,
			configMAX_PRIORITIES - 1, &MothershipTask) != pdPASS) {
		//PRINT_TASK_ERROR("RenderTask");
		//goto err_rendertask;
		printf("error in creating mothership task\n");
	}
}

void vMothershipTask(void *pvParameters)
{

	for (;;) {
        if(State==STATE_SINGLEPLAYER){
            handle_mothership();
        }else if(State==STATE_MULTIPLAYER){
            taskENTER_CRITICAL();
            handle_opponent();
            taskEXIT_CRITICAL();
        }
		vTaskDelay((TickType_t)(1000 / 30));
	}
}

void handle_mothership(){
    if(!Mothership.exists){
        if(Mothership.wait==0){
            create_mothership();
        }
        Mothership.wait--;
    }else{
        Mothership.x_coord += Mothership.movement;

        if(Mothership.x_coord < -MOTHERSHIP_WIDTH-2){
            kill_mothership();
        }else if(Mothership.x_coord > SCREEN_WIDTH+2){
            kill_mothership();
        }
    }
}


void kill_mothership(){
    Mothership.exists = false;
    Mothership.wait = rand() % MOTHERSHIP_MAX_WAIT;
}


void create_mothership(){
    bool left = rand() % 2;
    if(left){
        Mothership.x_coord = -MOTHERSHIP_WIDTH;
        Mothership.movement = MOTHERSHIP_SPEED;
    }else{  //right
        Mothership.x_coord = SCREEN_WIDTH;
        Mothership.movement = -MOTHERSHIP_SPEED;
    }
    Mothership.exists = true;
    
}


void init_opponent(){

}


void handle_opponent()
{
    if(!Mothership.exists){
        if(Mothership.wait==0){
            create_mothership();
        }
        Mothership.wait--;
    }
    
	if (OpponentQueue != NULL) {
		if (xQueueReceive(OpponentQueue, &msg, (TickType_t)10) ==
		    pdPASS) {
			Mothership.movement = msg;
		}
	}
	Mothership.x_coord += Mothership.movement;

	if (Mothership.x_coord > SCREEN_WIDTH) {
		Mothership.x_coord -= SCREEN_WIDTH;
	} else if (Mothership.x_coord < MOTHERSHIP_WIDTH) {
		Mothership.x_coord += SCREEN_WIDTH;
	}
	if (Mothership.cycle == 0) {
		opponent_send_player_position();
		Mothership.cycle = 5;
	}
	Mothership.cycle--;
}





void opponent_pause()
{
	char *buf = "PAUSE";
	aIOSocketPut(UDP, NULL, UDP_TRANSMIT_PORT, buf, strlen(buf));
}

void opponent_resume()
{
	char *buf = "RESUME";
	aIOSocketPut(UDP, NULL, UDP_TRANSMIT_PORT, buf, strlen(buf));
}

void opponent_set_difficulty(int difficulty)
{
	char *buf;
	switch (difficulty) {
	case 1:
		buf = "D1";
		break;
	case 2:
		buf = "D2";
		break;
	case 3:
		buf = "D3";
		break;
	default:
		return;
	}
	aIOSocketPut(UDP, NULL, UDP_TRANSMIT_PORT, buf, strlen(buf));
}

void opponent_send_player_position()
{
	int num = Mothership.x_coord - Player.x_coord;
	char buf[10];
	sprintf(buf, "%d", num);
	aIOSocketPut(UDP, NULL, UDP_TRANSMIT_PORT, buf, strlen(buf));
}

void opponent_send_player_attacking()
{
	char *buf = "ATTACKING";
	aIOSocketPut(UDP, NULL, UDP_TRANSMIT_PORT, buf, strlen(buf));
}
void opponent_send_player_passive()
{
	char *buf = "PASSIVE";
	aIOSocketPut(UDP, NULL, UDP_TRANSMIT_PORT, buf, strlen(buf));
}

void draw_mothership()
{
    if(!Mothership.exists){
        return;
    }
	tumDrawLoadedImage(Mothership.img, Mothership.x_coord,
			   MOTHERSHIP_Y_COORD);

    if(State==STATE_SINGLEPLAYER){
        return;
    }

	if (Mothership.x_coord > SCREEN_WIDTH - MOTHERSHIP_WIDTH) {
		tumDrawLoadedImage(Mothership.img,
				   Mothership.x_coord - SCREEN_WIDTH,
				   MOTHERSHIP_Y_COORD);
	} else if (Mothership.x_coord < 0) {
		tumDrawLoadedImage(Mothership.img,
				   Mothership.x_coord + SCREEN_WIDTH,
				   MOTHERSHIP_Y_COORD);
	}
}

int mothership_check_hit(int x, int y, int height){
    if(Mothership.exists == false){
        return 0;
    }

    int hit_y = y - MOTHERSHIP_Y_COORD;
    int hit_x = x - Mothership.x_coord;

    if(hit_y > MOTHERSHIP_HEIGHT || hit_y < -height){
        return 0; // No Hit
    }

    if(hit_x < 0 || hit_x > MOTHERSHIP_WIDTH){
        return 0;
    }
    
    if(xSemaphoreTake(PlayerHandle, ( TickType_t ) 10 ) == pdTRUE){
        Player.Points += MOTHERSHIP_POINTS;
        xSemaphoreGive(PlayerHandle);
    }
    kill_mothership();
    return 1;
}
