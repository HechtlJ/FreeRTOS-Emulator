#include "missile.h"
#include "TUM_Draw.h"
#include "stdio.h"


void init_missiles(){
    for(int i=0; i<NUM_MAX_MISSILES; i++){
        Missiles[i].exists = false;
    }
}


int xCreateMissile(int x, int y, char type)
{
	if (xSemaphoreTake(MissileHandle, (TickType_t)10) == pdTRUE) {
		int ret = create_missile(x, y, type);
		xSemaphoreGive(MissileHandle);
        return ret;
	} else {
		// error handling
		return 1;
	}
}


void xPaintMissiles()
{
	if (xSemaphoreTake(MissileHandle, (TickType_t)10) == pdTRUE) {
		paint_missiles();
		xSemaphoreGive(MissileHandle);
	} else {
		// error handling
		return;
	}
}

void xMoveMissiles()
{
	if (xSemaphoreTake(MissileHandle, (TickType_t)10) == pdTRUE) {
		move_missiles();
		xSemaphoreGive(MissileHandle);
	} else {
		// error handling
		return;
	}
}


void xMissilesSwitchAnimation(){
    if (xSemaphoreTake(MissileHandle, (TickType_t)10) == pdTRUE) {
		missiles_switch_animation();
		xSemaphoreGive(MissileHandle);
	} else {
		// error handling
		return;
	}
}


int create_missile(int x, int y, char type){
    for(int i=0; i<NUM_MAX_MISSILES; i++){
        if(!Missiles[i].exists){
            Missiles[i].x_coord = x;
            Missiles[i].y_coord = y;
            Missiles[i].type = type;
            Missiles[i].exists = true;
            Missiles[i].animation = 0;
            return 0;
        }
    }

    return 1;
}


void paint_missiles()
{
	for (int i = 0; i < NUM_MAX_MISSILES; i++) {
        missile_t * missile = &Missiles[i];
		if (!missile->exists)
			return;
		switch (missile->type) {
		case MISSILE_TYPE_A:
			paint_missile_type_A(missile->x_coord, missile->y_coord,
					  missile->animation);
			break;
        case MISSILE_TYPE_B:
            paint_missile_type_B(missile->x_coord, missile->y_coord,
					  missile->animation);
        case MISSILE_TYPE_C:
            paint_missile_type_C(missile->x_coord, missile->y_coord,
					  missile->animation);
		default:
			break;
		}
	}
}


void move_missiles(){
    int speed;
    for(int i=0; i<NUM_MAX_MISSILES; i++){
        if(Missiles[i].exists){
            switch(Missiles[i].type){
                case MISSILE_TYPE_A: speed = MISSILE_TYPE_A_SPEED; break;
                case MISSILE_TYPE_B: speed = MISSILE_TYPE_B_SPEED; break;
                case MISSILE_TYPE_C: speed = MISSILE_TYPE_C_SPEED; break;
                default: break;
            }
            Missiles[i].y_coord += speed;
            if(Missiles[i].y_coord > MISSILE_MAX_Y){
                Missiles[i].exists = false;
            }
        }
    }
}


void missiles_switch_animation(){
    for(int i=0; i<NUM_MAX_MISSILES; i++){
        Missiles[i].animation++;
        Missiles[i].animation = Missiles[i].animation % 4;
    }
}


void paint_missile_type_A(int x, int y, char animation){
    if(animation==0){
        tumDrawFilledBox(x+2, y, 1, MISSILE_HEIGHT-1, White);
    }else if(animation==1){
        tumDrawFilledBox(x+2, y, 1, MISSILE_HEIGHT-1, White);
        tumDrawFilledBox(x, y+12, MISSILE_WIDTH-1, 1, White);
    }else if(animation==2){
        tumDrawFilledBox(x+2, y, 1, MISSILE_HEIGHT-1, White);
        tumDrawFilledBox(x, y+6, MISSILE_WIDTH-1, 1, White);
    }else if(animation==3){
        tumDrawFilledBox(x+2, y, 1, MISSILE_HEIGHT-1, White);
        tumDrawFilledBox(x, y, MISSILE_WIDTH-1, 1, White);
    }else{
        //error
        return;
    }
}

void paint_missile_type_B(int x, int y, char animation){
    if(animation==0){
        tumDrawFilledBox(x+2, y, 1, MISSILE_HEIGHT-1, White);
    }else if(animation==1){
        tumDrawFilledBox(x+2, y, 1, MISSILE_HEIGHT-1, White);
        tumDrawFilledBox(x, y+4, 1, 1, White);
        tumDrawFilledBox(x, y+10, 1, 1, White);
        tumDrawFilledBox(x+4, y+6, 1, 1, White);
        tumDrawFilledBox(x+4, y+12, 1, 1, White);
    }else if(animation==2){
        tumDrawFilledBox(x+2, y, 1, MISSILE_HEIGHT-1, White);
    }else if(animation==3){
        tumDrawFilledBox(x+2, y, 1, MISSILE_HEIGHT-1, White);
        tumDrawFilledBox(x, y+2, 1, 1, White);
        tumDrawFilledBox(x+4, y, 1, 1, White);
        tumDrawFilledBox(x, y+8, 1, 1, White);
        tumDrawFilledBox(x+4, y+6, 1, 1, White);
    }else{
        //error
        return;
    }
}


//TODO: Paint proper missile
void paint_missile_type_C(int x, int y, char animation){
    if(animation==0){
        tumDrawFilledBox(x+2, y, 1, MISSILE_HEIGHT-1, White);
    }else if(animation==1){
        tumDrawFilledBox(x+2, y, 1, MISSILE_HEIGHT-1, White);
    }else if(animation==2){
        tumDrawFilledBox(x+2, y, 1, MISSILE_HEIGHT-1, White);
    }else if(animation==3){
        tumDrawFilledBox(x+2, y, 1, MISSILE_HEIGHT-1, White);
    }else{
        //error
        return;
    }
}


void reset_missiles(){
    for(int i=0; i<NUM_MAX_MISSILES; i++){
        Missiles[i].exists = false;
    }
}

void xResetMissiles(){
    if (xSemaphoreTake(MissileHandle, (TickType_t)10) == pdTRUE) {
		reset_missiles();
		xSemaphoreGive(MissileHandle);
	} else {
		// error handling
		return;
	}
}
