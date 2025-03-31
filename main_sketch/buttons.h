#ifndef BUTTONS_H_
#define BUTTONS_H_

#include <Arduino.h>
#include "display.h"  // This gives us LCD functionality

// LCD Keypad Shield uses A0 for button input
#define BTN_PIN A0  

// Menu variables
extern const char* menuItems[];
extern int menuIndex;

// Menu navigation
void checkButtons();
void updateMenu();

#endif /* BUTTONS_H_ */
