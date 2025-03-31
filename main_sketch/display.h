#ifndef _display_h
#define _display_h

#include <Arduino.h>
#include "cards.h"
#include <LiquidCrystal.h>

// LCD pin definitions
extern const int pin_RS;
extern const int pin_EN; 
extern const int pin_d4;
extern const int pin_d5;
extern const int pin_d6;
extern const int pin_d7;
extern const int pin_BL;
extern LiquidCrystal lcd;

// Node structure for linked lists
typedef struct node {
    struct node* next;
    struct Card_struct card;
} Node;

// Function prototypes
void Show_Hands();
Node createNode(struct Card_struct card);
int add_player_card(struct Card_struct &card);
int add_hidden_dealer_card(struct Card_struct &card);
int add_visable_dealer_card(struct Card_struct &card);
int clear_list(struct Node* p_head);
int new_game();

// External variables
extern struct Card_struct hidden_dealer_card;
extern struct Card_struct* player_cards;
extern struct Card_struct* dealer_cards;
extern int is_hidden;

#endif
