/**
 * @file bunker.h
 * @author Johannes Hechtl
 * @date 10.07.2020
 * @brief API for handling bunkers
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

#ifndef BUNKER_H
#define BUNKER_H

/**
 * @defgroup bunker SI Bunker API
 * 
 * API for handling the Bunkers in Space Invaders
 * 
 * @{
 */

#include <stdlib.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include "TUM_Draw.h"


/**
 * @def NUM_BUNKER_BLOCK_X
 * Width of a single bunker in Bunkerblocks, which are the destructable Elements of a bunker.
 * 
 * @def NUM_BUNKER_BLOCK_Y
 * Height of a single bunker in Bunkerblocks, which are the destructable Elements of a bunker.
 */
#define NUM_BUNKER_BLOCK_X 16
#define NUM_BUNKER_BLOCK_Y 8

/**
 * @def BUNKER_Y_COORD
 * Y-Coordinate of the top of the bunkers
 */
#define BUNKER_Y_COORD 485

/**
 * @def NUM_BUNKERS
 * Number of bunkers that are generated. Bunkers are automatically distributed along the x-axis.
 * 
 * @def BUNKER_WIDTH
 * Width of a single bunker in Pixels.
 * 
 * @def BUNKER_RIM
 * Distance in Pixels between the outer bunkers and the screen borders.
 * 
 * @def BUNKER_HEIGHT
 * Height of a single bunker in Pixels.
 */
#define NUM_BUNKERS 4
#define BUNKER_WIDTH NUM_BUNKER_BLOCK_X * 4
#define BUNKER_RIM 60
#define BUNKER_HEIGHT NUM_BUNKER_BLOCK_Y * 4

/**
 * @def DAMAGE_CANNONBALL
 * Damage Type: Damage inflicted by a cannonball.
 * 
 * @def DAMAGE_MISSILE
 * Damage Type: Damage inflicted by a missile.
 */
#define DAMAGE_CANNONBALL 0
#define DAMAGE_MISSILE 1


/**
 * @struct bunker_t
 * Struct for saving the state of a single Bunker.
 * 
 * 
 */
typedef struct {
    /**
     * @var BunkerBlocks
     * Array for holding the state of all the BunkerBlocks. When true, the block exists, otherwise it will not be painted or recognized by hit detection.
     * 
     * @var x_coord
     * X-Coordinate of the top left corner of the bunker.
     * 
     * @var y_coord
     * Y-Coordinate of the top left corner of the bunker.
     */
    bool BunkerBlocks[NUM_BUNKER_BLOCK_X][NUM_BUNKER_BLOCK_Y];
    int x_coord;
    int y_coord;
} bunker_t;

/**
 * @var Bunkers
 * Holds the bunker_t Bunkers. Should only be read or manipulated when owning BunkerHandle.
 * 
 * @var BunkerHandle
 * Semaphor for making the Bunkers structure thread safe.
 */
bunker_t Bunkers[NUM_BUNKERS];
xSemaphoreHandle BunkerHandle;


/** @brief Initializes Bunkers.
 * 
 * Initializes the Bunkers structure. Initilaizes the bunkers coordinates and BunkerBlocks.
 * Only needs to be called once at the beginning of the session, afterwards call reset_bunkers() or xResetBunkers() to restore the bunkers.
 */
void init_bunkers();


/** @brief Paints the bunkers in Bunkers
 * 
 * Uses TUM_Draw to draw all the BunkerBlocks that are true. 
 * Not thread safe, for a thread safe variant call xPaintBunkers()
 */
void paint_bunkers();

/** @brief Paints the bunkers in Bunkers
 * 
 * Uses TUM_Draw to draw all the BunkerBlocks that are true. 
 * Thread safe. For a faster variant without taking the semaphor call paint_bunkers()
 */
void xPaintBunkers();

/** @brief Checks for bunker hit and handles the damage
 * 
 * Checks if a moving projectile has hit a bunker. 
 * Only works for straight up or down travelling projectiles. In order to prevent tunneling,
 * the length of the projectile is presumed infinite. It is also presumed, that the 
 * projectiles width is only 1 Pixel. For a wider projectile call this function multiple times 
 * with different x coordinates. The specified y coordinate must be the tip of the projectile,
 * meaning the upper end for an upwards travelling projectile and the lower end for a downwards
 * travelling projectile.
 * 
 * @param x X-Coordinate of the tip of the projectile
 * @param y Y-Coordinate of the tip of the projectile
 * @param moving_up Specifies wether or not the projectile is moving up or down. 
 * @param damage_type Specifies the type of damage a hit inflicts on the bunker. Can be 
 * DAMAGE_CANNONBALL or DAMAGE_MISSILE
 * @return Returns 1 when a hit is detected, otherwise 0.
 */
int check_bunker_hit(int x, int y, bool moving_up, int damage_type);

/** @brief Thread safe variant of check_bunker_hit()
 * 
 * Thread safe variant of check_bunker_hit(). Check check_bunker_hit() for description.
 * 
 * @param x X-Coordinate of the tip of the projectile
 * @param y Y-Coordinate of the tip of the projectile
 * @param moving_up Specifies wether or not the projectile is moving up or down. 
 * @param damage_type Specifies the type of damage a hit inflicts on the bunker. Can be 
 * DAMAGE_CANNONBALL or DAMAGE_MISSILE
 * @return Returns 1 when a hit is detected, otherwise 0.
 */
int xCheckBunkerHit(int x, int y, bool moving_up, int damage_type);


void damageBunker(bunker_t * bunker, int x, int y, int damage_type);


void reset_bunkers();
//TODO
//void xResetBunkers();


/** @}*/
#endif /* BUNKER_H */