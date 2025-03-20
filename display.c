#include <Arduino.h>
#include "cards.h"
#include "display.h"
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to      
const int pin_RS = 8; 
const int pin_EN = 9; 
const int pin_d4 = 4; 
const int pin_d5 = 5; 
const int pin_d6 = 6; 
const int pin_d7 = 7; 
const int pin_BL = 10; 
LiquidCrystal lcd(pin_RS,  pin_EN,  pin_d4,  pin_d5,  pin_d6,  pin_d7);

// Define global variables 
struct Card_struct hidden_dealer_card;
struct Card_struct* player_cards = NULL;
struct Card_struct* dealer_cards = NULL;

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
  lcd.clear();
  lcd.setCursor(0,0);
  struct Node* transverser = player_cards;
  while(transverser != NULL){
    lcd.print(transverser -> card.friendlyName);
    transverser = transverser.next;
  }
  lcd.setCursor(1,0);
  //need to replace with if statement with a flag to check if the hidden card needs to be flipped over 
  lcd.print ("H"); //printing hidden card - later needs to be shown 
  lcd.setCursor(1,1);
  transverser = dealer_cards;
  while(transverser != NULL){
    lcd.print(transverser -> card.friendlyName);
    transverser = transverser.next;
  }
  // TODO: Update display with linked lists and hidden cards 
}

// Function implimentations
int add_player_card(struct Card_struct &card){
  struct node = createNode(card); 
  // TODO: Add card to player cards linked list 
  if (player_cards == NULL){
    player_cards = &node;
  }
  else {
    node.next = play_cards;
    player_cards = &node;
  }
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
