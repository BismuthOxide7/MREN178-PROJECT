#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>

#define BTN_PIN A0  // All buttons use A0 on the LCD shield

void setupButtons();
void checkButtons();

#endif
