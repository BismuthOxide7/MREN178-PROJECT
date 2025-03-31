#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <Arduino.h>
#include <LiquidCrystal.h>
#include "cards.h"

// LCD pin definitions
extern const int pin_RS;  // = 8
extern const int pin_EN;  // = 9
extern const int pin_d4;  // = 4
extern const int pin_d5;  // = 5
extern const int pin_d6;  // = 6
extern const int pin_d7;  // = 7
extern const int pin_BL;  // = 10

// Declare LCD object as extern
extern LiquidCrystal lcd;

// Function prototypes
bool Show_Hands();
int add_player_card(Card_struct* card);
int add_hidden_dealer_card(Card_struct* card);
int add_visable_dealer_card(Card_struct *card);
int clear_list(Card_struct* p_head);
int new_game();

// External variables
extern Card_struct hidden_dealer_card;
extern Card_struct* player_cards;
extern Card_struct* dealer_cards;
extern int is_hidden;

#endif /* DISPLAY_H_ */
