/**
 * @file Checks.h Declarations for the Checks module.
 * 
 * @copyright Copyright (c) 2022 John Scott
 */
#pragma once
#include "Config.h"

/**
 * Structure for remembering the state of each button. Used to compare to the
 * current state to track different checks. 
 */
struct ButtonState {
  bool isPressed; ///< Button is pressed.
  bool isHeld; ///< Button is held.
  bool isIdle; ///< Button is idle.
# if TESTABLE_CLASS >= TESTABLE_CLICKER
    bool isClicked; ///< Button clicked (presed+released).
# endif
# if TESTABLE_CLASS >= TESTABLE_DOUBLECLICKER
    bool isDoubleClicked; ///< Button double-clicked.
# endif
# if TESTABLE_CALLBACK
    bool wasStarted; ///< Button received begin callback.
    bool wasPressed; ///< Button received pressed callback.
    bool wasReleased; ///< Button received released callback.
    bool wasHeld; ///< Button received held callback.
    bool wasIdle; ///< Button received idle callback.
# endif
};

extern struct ButtonState btnState[NUM_BUTTONS];

//
// Definitions of Button checks...
//
void checkButtonSetup(Button *btn);
void checkButtonJustPressed(Button *btn);
void checkButtonJustReleased(Button *btn);
void checkButtonJustHeld(Button *btn);
void checkButtonJustIdle(Button *btn);
void checkButtonJustClicked(Button *btn);
void checkButtonJustDoubleClicked(Button *btn);
void checkButtonIntegrity(Button *btn, ButtonState &state);
void displayButtonChanges(int index);

//
// Definitions of ButtonList checks...
//
void checkButtonListIntegrity();
