/**
 * @file cannonball.h
 * @author Johannes Hechtl
 * @date 10.07.2020
 * @brief API to create and handle missiles.
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
 * @defgroup player SI Player API
 * 
 * Functions to generate a Player for Space Invaders and handle his actions and stats.
 * 
 * @{
 */


#ifndef MISSILE_H
#define MISSILE_H

#include "stdbool.h"
#include "FreeRTOS.h"
#include "semphr.h"

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

typedef struct {
    unsigned int x_coord;
    unsigned int y_coord;
    unsigned char animation;
    unsigned char type;
    bool exists;
} missile_t;


missile_t Missiles[NUM_MAX_MISSILES];
xSemaphoreHandle MissileHandle;

void init_missiles();

int xCreateMissile(int x, int y, char type);
void xPaintMissiles();
void xMoveMissiles();
void xMissilesSwitchAnimation();


int create_missile(int x, int y, char type);
void paint_missiles();
void move_missiles();
void missiles_switch_animation();


void paint_missile_type_A(int x, int y, char animation);
void paint_missile_type_B(int x, int y, char animation);
void paint_missile_type_C(int x, int y, char animation);

void reset_missiles();
void xResetMissiles();


#endif /* MISSILE_H */