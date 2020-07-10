/**
 * @file button_input.h
 * @author Johannes Hechtl
 * @date 10.07.2020
 * @brief API for handling the button inputs for Space Invaders.
 * API for handling the button inputs for Space Invaders. 
 * Uses the TUM_Event API.
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




#ifndef BUTTON_INPUT_H
#define BUTTON_INPUT_H

#include "FreeRTOS.h"
#include "task.h"

#define mainGENERIC_PRIORITY (tskIDLE_PRIORITY)
#define mainGENERIC_STACK_SIZE ((unsigned short)2560)


extern TaskHandle_t ButtonTask;


void buttonInit();




#endif /* BUTTON_INPUT_H */