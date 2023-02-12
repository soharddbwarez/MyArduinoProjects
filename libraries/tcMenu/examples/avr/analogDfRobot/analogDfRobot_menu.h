/*
    The code in this file uses open source libraries provided by thecoderscorner

    DO NOT EDIT THIS FILE, IT WILL BE GENERATED EVERY TIME YOU USE THE UI DESIGNER
    INSTEAD EITHER PUT CODE IN YOUR SKETCH OR CREATE ANOTHER SOURCE FILE.

    All the variables you may need access to are marked extern in this file for easy
    use elsewhere.
 */

#ifndef MENU_GENERATED_CODE_H
#define MENU_GENERATED_CODE_H

#include <Arduino.h>
#include <tcMenu.h>
#include <LiquidCrystalIO.h>
#include "tcMenuLiquidCrystal.h"
#include <DfRobotInputAbstraction.h>
#include <ScrollChoiceMenuItem.h>
#include <RuntimeMenuItem.h>
#include <EditableLargeNumberMenuItem.h>
#include <IoAbstraction.h>
#include <EepromItemStorage.h>

// variables we declare that you may need to access
extern const PROGMEM ConnectorLocalInfo applicationInfo;
extern LiquidCrystal lcd;
extern LiquidCrystalRenderer renderer;

// Any externals needed by IO expanders, EEPROMs etc


// Global Menu Item exports
extern AnalogMenuItem menuCommits;
extern ScrollChoiceMenuItem menuChooseItem;
extern TextMenuItem menuText;
extern EditableLargeNumberMenuItem menuLgeNum;
extern BooleanMenuItem menuL2;
extern BooleanMenuItem menuL1;
extern BackMenuItem menuBackLEDStates;
extern SubMenuItem menuLEDStates;
extern AnalogMenuItem menuValueA0;

// Provide a wrapper to get hold of the root menu item and export setupMenu
inline MenuItem& rootMenuItem() { return menuValueA0; }
void setupMenu();

// Callback functions must always include CALLBACK_FUNCTION after the return type
#define CALLBACK_FUNCTION

int fnChooseItemRtCall(RuntimeMenuItem* item, uint8_t row, RenderFnMode mode, char* buffer, int bufferSize);
void CALLBACK_FUNCTION onLed1(int id);
void CALLBACK_FUNCTION onLed2(int id);

#endif // MENU_GENERATED_CODE_H
