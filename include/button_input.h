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

/**
 * @defgroup button SI Button API
 * 
 * API for handling all the Input for Space Invaders
 * 
 * @{
 */

#include "FreeRTOS.h"
#include "task.h"
#include "game.h"



#define MOUSE_DEBOUNCE_LENGTH 5


/**
 * @struct key_action_t
 * Struct for managing Key Actions
 */
typedef struct {
  /**
   * @var keycode
   * SDL Scancode for the button that triggers this Key Action
   * 
   * @var activeForState
   * Defines in which Game States the KeyAction is active
   * 
   * @var callback
   * Callback Function that gets called when the Action is triggered
   * 
   * @var debounceDelay
   * Length of the delay until the action can get triggered again. If 0, no debounce is active.
   * 
   * @var lastButtonState
   * The last detected state of the button, used for debouncing
   * 
   * @var waitTime
   * The time until the action can be triggered again, used for debouncing
   */
  int keycode;
  bool activeForState[NUM_STATES];
  function callback;
  int debounceDelay;
  bool lastButtonState;
  int waitTime;
}key_action_t;  


/**
 * @def Number of Key actions
 */
#define NUM_KEY_ACTIONS 9

/**
 * @var KeyActions
 * Holds all the KeyActions.
 */
key_action_t KeyActions[NUM_KEY_ACTIONS];

/* ***********************************
 * defining Names of Key Actions
 **************************************/
#define ExitAction 0
#define PauseAction 1
#define MovePlayerLeftAction 2
#define MovePlayerRightAction 3
#define PlayerShootAction 4
#define CheatsDecreaseStartingScoreAction 5
#define CheatsIncreaseStartingScoreAction 6
#define CheatsDecreaseStartingScoreActionSlow 7
#define CheatsIncreaseStartingScoreActionSlow 8

/**
 * @brief handles a KeyAction
 * 
 * Handles a KeyAction, meaning triggering its callback and managing debounce etc
 */
void handleKeyAction(key_action_t * KeyAction);

/**
 * @brief Exits the program cleanly
 * 
 * Exits the program cleanly. Used as Callback for the ExitAction.
 */
void clean_exit();

/**
 * @brief Shoots a cannonball
 * 
 * Shoots a cannonball. Used as Callback for the PlayerShootAction.
 */
void shootCallback();

/**
 * @brief Handles a Single Button.
 * 
 * Handles a Single Button. Will change Graphics when hovering over the button and
 * will call the callback when the button is clicked.
 * 
 * @param button The button to be handled
 */
void handleButton(button_t * button);

/**
 * Task for handling all the input for Space Invaders
 */
void vHandleButtons(void *pvParameters);


extern TaskHandle_t ButtonTask;

/**
 * @brief Initializes the Buttons and KeyActions and the button Task.
 * 
 * Initializes the Buttons and KeyActions and the ButtonTask.
 */
void buttonInit();


#endif /* BUTTON_INPUT_H */