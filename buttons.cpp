#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>
#include <LiquidCrystal.h> 

// LCD Keypad Shield uses A0 for button input
#define BTN_PIN A0  

// Menu navigation
void setupButtons();
void checkButtons();
void updateMenu();

#endif
