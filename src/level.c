#include "player.h"
#include "rendering.h"
#include "button_input.h"
#include "cannonball.h"
#include "projectiles.h"
#include "bunker.h"
#include "invaders.h"
#include "missile.h"
#include "level.h"


void init_levels(){
    Level = xSemaphoreCreateCounting(9999, 1);
}

void next_level(){
    xSemaphoreGive(Level);  //increasing Level by 1

    init_missiles();
    int offset = 0; //TODO
    invader_reset(offset);
}


void game_over();