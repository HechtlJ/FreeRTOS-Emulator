/**
 * @file cannonball.h
 * @author Johannes Hechtl
 * @date 10.07.2020
 * @brief API to create cannonballs.
 *
 * @verbatim
   ----------------------------------------------------------------------
    Copyright (C) Johannes Hechtl, 2020
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
   ----------------------------------------------------------------------
@endverbatim
 */


/**
 * @defgroup cannonball SI Cannonball API
 * 
 * Functions to generate a Cannonball for Space Invaders and handle his actions and stats.
 * 
 * @{
 */


#ifndef CANNONBALL_H
#define CANNONBALL_H

#include <stdlib.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "semphr.h"

#define CANNONBALL_VELOCITY 14
#define CANNONBALL_WIDTH 3
#define CANNONBALL_HEIGHT 12
#define MAX_NUM_CANNONBALLS 1
#define RELOAD_TIME 20

typedef struct {
    int x_coord;
    int y_coord;
    bool exists;
} cannonball_t;

cannonball_t CannonBalls[MAX_NUM_CANNONBALLS];
xSemaphoreHandle CannonballHandle;

void xPaintCannonballs();
void xCreateCannonball(int x, int y);
void xMoveCannonballs();
void init_cannonballs();
void paint_cannonballs();
void create_cannonball(int x, int y);
void move_cannonballs();


#endif /* CANNONBALL_H */