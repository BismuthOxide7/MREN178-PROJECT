#include <Arduino.h>
#include "cards.h"
#include "display.h"

// Define global variables 
struct Card_struct hidden_dealer_card;

// Struct to define linked list for added player cards and added visable dealer cards with two different heads 
struct Node {
  struct Node* next;
  struct Card_struct card;
};

// Create node
struct Node createNode(struct Card_struct card){
  struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
  newNode->card = card;
  return newNode;
}

// Update LCD 
void update_LCD(){
  // TODO: Update display with linked lists and hidden cards 
}

// Function implimentations
int add_player_card(struct Card_struct &card){
  // TODO: Add card to player cards linked list 
  update_LCD(); 
  return 0;
}

int add_hidden_dealer_card(struct Card_struct &card){
  hidden_dealer_card = card;
  update_LCD(); 
  return 0;
}
int add_visable_dealer_card(struct Card_struct &card){
  // TODO: Add card to dealer visable cards linked list 
  update_LCD();
  return 0;
}
int new_game (){
  // TODO: delete all pre-existing cards in linked lists and hidden dealer card
  update_LCD(); 
  return 0;
}
