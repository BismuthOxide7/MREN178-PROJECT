#ifndef GAME_H_
#define GAME_H_

#include "cards.h"
#include "display.h"
#include "buttons.h"
#include "actions.h"
#include "wireless.h"

// LCD Keypad Shield uses A0 for button input
#define BTN_PIN A0  

#define BTN_RIGHT  0    // ADC value for Right button
#define BTN_UP     99   // ADC value for Up button
#define BTN_DOWN   255  // ADC value for Down button
#define BTN_LEFT   410  // ADC value for Left button
#define BTN_SELECT 640  // ADC value for Select button
#define BTN_NONE   1023 // No button pressed

// Function prototypes for game initialization
void player_init_game();
void dealer_init_game();

#endif /* GAME_H_ */
