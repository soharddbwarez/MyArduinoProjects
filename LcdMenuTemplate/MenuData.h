#ifndef _sampleMenu_
#define _sampleMenu_
#include "MenuManager.h"
#include <avr/pgmspace.h>

/*

Generated using LCD Menu Builder at https://lcd-menu-bulder.cohesivecomputing.co.uk/
For more information, visit https://www.cohesivecomputing.co.uk/hackatronics/arduino-lcd-menu-library/

All our hackatronics projects are free for personal use. If you find them helpful or useful, please consider
making a small donation to our hackatronics fund using the donate buttons on our web pages. Thank you.
    
*/

enum sampleMenuCommandId
{
  mnuCmdBack = 0,
  mnuCmdItem1,
  mnuCmdItem2,
  mnuCmdItem3,
  mnuCmdItem4,
  mnuCmdItem5
};

PROGMEM const char sampleMenu_back[] = "Back";
PROGMEM const char sampleMenu_exit[] = "Exit";

PROGMEM const char sampleMenu_1[] = "Item 1";
PROGMEM const char sampleMenu_2[] = "Item 2";
PROGMEM const char sampleMenu_3[] = "Item 3";
PROGMEM const char sampleMenu_4[] = "Item 4";
PROGMEM const char sampleMenu_5[] = "Item 5";
PROGMEM const MenuItem sampleMenu_Root[] = {{mnuCmdItem1, sampleMenu_1}, {mnuCmdItem2, sampleMenu_2}, {mnuCmdItem3, sampleMenu_3}, {mnuCmdItem4, sampleMenu_4}, {mnuCmdItem5, sampleMenu_5}, {mnuCmdBack, sampleMenu_exit}};

/*
case mnuCmdItem1 :
  break;
case mnuCmdItem2 :
  break;
case mnuCmdItem3 :
  break;
case mnuCmdItem4 :
  break;
case mnuCmdItem5 :
  break;
*/

/*
<?xml version="1.0"?>
<RootMenu xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema">
  <Config IdPrefix="mnuCmd" VarPrefix="sampleMenu" UseNumbering="false" IncludeNumberHierarchy="false"
          MaxNameLen="15" MenuBackFirstItem="false" BackText="Back" ExitText="Exit" AvrProgMem="true"/>
  <MenuItems>
    <Item Id="Item1" Name="Item 1"/>
    <Item Id="Item2" Name="Item 2"/>
    <Item Id="Item3" Name="Item 3"/>
    <Item Id="Item4" Name="Item 4"/>
    <Item Id="Item5" Name="Item 5"/>
  </MenuItems>
</RootMenu>

*/
#endif
