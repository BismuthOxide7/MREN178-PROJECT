#ifndef CARDS_H_
#define CARDS_H_

#include <Arduino.h>

// Card struct definition
typedef struct card_struct {
  char suit;          // 'H' = Hearts, 'D' = Diamonds, 'C' = Clubs, 'S' = Spades
  int value;          // 1-10 (Ace to King)
  char friendlyName;  // 'A' for Ace, '2'-'9', 'T' for 10, 'J' for Jack, 'Q' for Queen, 'K' for King
  card_struct* next;  // Changed from Card_Struct to Card_struct
} Card_struct;

// Deck struct (Stack Implementation)
typedef struct {
  Card_struct cards[52];  // Array to hold 52 cards
  int top_card;           // Index of the top card in the stack (starts at 51 for a full deck)
} Deck_struct;

// Function prototypes
Card_struct make_card(char suit, int value, char friendlyName);  // Create a card
void initialize_deck(Deck_struct* deck);  // Initialize deck with 52 cards
Card_struct draw_card(Deck_struct* deck);  // Draw a card from the deck
void shuffle_deck(Deck_struct* deck);  // Shuffle the deck using Fisher-Yates algorithm

#endif /* CARDS_H_ */
