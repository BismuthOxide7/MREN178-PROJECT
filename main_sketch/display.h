#ifndef _display_h
#define _display_h

#include <Arduino.h>
#include "cards.h"



// Function prototypes 
int add_player_card(Card_struct* card); // Display draws card from the deck
int add_hidden_dealer_card(Card_struct* card); // Displayed dealers hidden card
int add_visable_dealer_card(Card_struct* card); // Displayed dealer card
int new_game (); // Display cards cleared and ready for new game  

#endif 
