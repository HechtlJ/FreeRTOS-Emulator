/**
 * @file player.h
 * @author Johannes Hechtl
 * @date 10.07.2020
 * @brief API to create a player for the game space invaders and handle all actions of the player.
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

#ifndef PLAYER_H
#define PLAYER_H

/**
 * @defgroup player SI Player API
 * 
 * Functions to generate a Player for Space Invaders and handle his actions and stats.
 * 
 * @{
 */

#include "FreeRTOS.h"
#include "semphr.h"

/**
 * @def PLAYER_Y_COORD 
 * The y coordinate of the top of the player at all times, since the player con only move in x-direction.
 * 
 * @def PLAYER_WIDTH
 * 
 * @def PLAYER_BODY_HEIGTH
 * 
 * @def PLAYER_CANNON_WIDTH
 * 
 * @def PLAYER_CANNON_HEIGTH
 * 
 * @def PLAYER_MIN_X
 * The Minimum value of the x-coordinate of the player. The x-coord will not drop below this value.
 * 
 * @def PLAYER_MAX_X
 * The Maximum Value of the x-coordinate of the player. Is calculated from SCREEN_WIDTH and PLAYER_WIDTH.
 * 
 * @def PLAYER_SPEED
 * The amount of pixels the player will move per call of one of the player move functions.
 */
#define PLAYER_Y_COORD 560
#define PLAYER_WIDTH 39
#define PLAYER_HEIGTH 25
#define PLAYER_MAX_X SCREEN_WIDTH - PLAYER_WIDTH
#define PLAYER_MIN_X 0
#define PLAYER_SPEED 5

/**
 * @brief Object to represent a player.
 * 
 * This Object is a container for all stats about the player.
 * 
 */
typedef struct {
    int Life;
    int Points;
    int x_coord;
    void * img;
} player_t;

player_t Player;
xSemaphoreHandle PlayerHandle;


/**
 * @brief Paints the player using TUM_Draw
 * 
 * Thread-Safe function for painting the Player. The calling thread needs to have bound TUM_Draw.
 * 
 */
void xPaintPlayer();

/**
 * @brief Moves the player in positive x direction.
 * 
 * Thread-Safe function for moving the Player. Moves the Player to the right by PLAYER_SPEED Pixels.
 * 
 */
void xPlayerMoveRight();

/**
 * @brief Moves the player in negative x direction.
 * 
 * Thread-Safe function for moving the Player. Moves the Player to the left by PLAYER_SPEED Pixels.
 * 
 */
void xPlayerMoveLeft();


void xPlayerShoot();
void player_shoot();
void player_reduce_life();

void playerInit();

void xResetPlayer();
void reset_player();

int xCheckPlayerHit(int x, int y, int height);
int check_player_hit(int x, int y, int height);


#endif /* PLAYER_H */
