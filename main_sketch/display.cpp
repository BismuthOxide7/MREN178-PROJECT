#include <SoftwareSerial.h>
#include "wireless.h"
#include "cards.h"
#include "actions.h"
#include "buttons.h"
#include <LiquidCrystal.h>
#include "display.h"
#include <Arduino.h>
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

// Update LCD display with linked lists and hidden card 
void update_LCD(){
  lcd.clear();
  lcd.setCursor(0,0);
  struct Node* transverser = player_cards;
  while(transverser != NULL){
    lcd.print(transverser -> card.friendlyName);
    transverser = transverser.next;
  }
  lcd.setCursor(1,0);
  // TODO; need to replace with if statement with a flag to check if the hidden card needs to be flipped over 
  lcd.print ("H"); //printing hidden card - later needs to be shown 
  lcd.setCursor(1,1);
  transverser = dealer_cards;
  while(transverser != NULL){
    lcd.print(transverser -> card.friendlyName);
    transverser = transverser.next;
  } 
}

// Function implimentations player cards linked list 
int add_player_card(struct Card_struct &card){
  struct node = createNode(card); 
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

//Function implimentations hidden dealer card
int add_hidden_dealer_card(struct Card_struct &card){
  hidden_dealer_card = card;
  update_LCD(); 
  return 0;
  
}
//Function implimentations dealer visable cards linked list 
int add_visable_dealer_card(struct Card_struct &card){
  struct node = createNode(card); 
  if (dealer_cards == NULL){
    dealer_cards = &node;
  }
  else {
    node.next = dealer_cards;
    dealer_cards = &node;
  }
  update_LCD(); 
  return 0;
}

int clear_list(struct Node* p_head){
  Node *p_temp;
  Node *p_tail;
  while (p_head != NULL) {
      p_temp = p_head;
      // Checking if there is only one node in the list and if so deleting it 
      if(p_temp -> next == NULL){
        p_head = NULL;
        free(p_temp);
      }
      // Search for the node preceeding the tail node. Once found, date the old tail node, and update this preceeding node to become 
      // The new tail node by setting p_next_node to null and setting p_tail to point to this node 
      else{
        while (p_temp -> next -> next != NULL){
          p_temp=p_temp->next;
        }
        p_tail = p_temp -> next;
        p_temp -> next = NULL;
        free(p_tail); 
      }
    }
}
// delete all pre-existing cards in linked lists and hidden dealer card
int new_game (){
  clear_list(player_list);
  clear_list(dealer_list);
  hidden_dealer_card = NULL;
  
  update_LCD(); 
  return 0;
}
