#ifndef _display_h
#define _display_h

#include <Arduino.h>
#include "cards.h"
#include <LiquidCrystal.h>

// External declarations
extern LiquidCrystal lcd;
extern Card_struct hidden_dealer_card;
extern Node* player_cards;
extern Node* dealer_cards;

// Function prototypes
int add_player_card(Card_struct* card);
int add_hidden_dealer_card(Card_struct* card);
int add_visible_dealer_card(Card_struct* card);
int new_game(void);
void update_LCD(void);

#endif
