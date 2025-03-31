#ifndef BUTTONS_H_
#define BUTTONS_H_

#include <Arduino.h>
#include "display.h"  // This gives us LCD functionality

// LCD Keypad Shield uses A0 for button input
#define BTN_PIN A0  

#define BTN_RIGHT  0    // ADC value for Right button
#define BTN_UP     99   // ADC value for Up button
#define BTN_DOWN   255  // ADC value for Down button
#define BTN_LEFT   410  // ADC value for Left button
#define BTN_SELECT 640  // ADC value for Select button
#define BTN_NONE   1023 // No button pressed

// Menu variables
extern const char* menuItems[];
extern int menuIndex;

// Menu navigation
void checkButtons();
void updateMenu();

#endif /* BUTTONS_H_ */
