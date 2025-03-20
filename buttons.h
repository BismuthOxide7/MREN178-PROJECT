#ifndef _BUTTONS_H
#define _BUTTONS_H

#include <Arduino.h>
#include "wireless.h"  // Needed to send commands

#define BTN_HIT 2  // Define button pins
#define BTN_STAY 3
#define BTN_FOLD 4
#define BTN_BET 5
#define BTN_VIEW 6

void setupButtons();
void checkButtons();

#endif
