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

// Function implimentations
int add_player_card(struct Card_struct &card){
  return 0;
}

int add_hidden_dealer_card(struct Card_struct &card){
  hidden_dealer_card = card;
  return 0;
}
int add_visable_dealer_card(struct Card_struct &card){
  return 0;
}
int new_game (){
  return 0;
}
