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

// Define pin constants
const int pin_RS = 8;
const int pin_EN = 9;
const int pin_d4 = 4;
const int pin_d5 = 5;
const int pin_d6 = 6;
const int pin_d7 = 7;
const int pin_BL = 10;

// Create the LCD object
LiquidCrystal lcd(pin_RS, pin_EN, pin_d4, pin_d5, pin_d6, pin_d7);

// Define global variables 
Card_struct hidden_dealer_card;
Card_struct* player_cards = NULL;
Card_struct* dealer_cards = NULL;
int is_hidden = 1;

// Create node
Node createNode(Card_struct card) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->card = card;
    return *newNode;
}

// Update LCD display with linked lists and hidden card 
bool Show_Hands() {
    lcd.clear();
    
    // Display player's cards
    lcd.setCursor(0,0);
    lcd.print("P: ");
    Card_struct* curr_player = player_cards;
    while(curr_player != NULL) {
        lcd.print(curr_player->friendlyName);
        curr_player = curr_player->next;
    }
    
    // Display dealer's cards
    lcd.setCursor(0,1);
    lcd.print("D: ");
    
    // First show hidden card
    if(is_hidden == 1) {
        lcd.print("H"); // Hidden card
    } else {
        lcd.print(hidden_dealer_card.friendlyName);
        lcd.print(hidden_dealer_card.suit);
    }
    
    // Then show visible dealer cards
    Card_struct* curr_dealer = dealer_cards;
    while(curr_dealer != NULL) {
        lcd.print(curr_dealer->friendlyName);
        curr_dealer = curr_dealer->next;
    }

    // Check for button press to return to menu
    int buttonValue = analogRead(BTN_PIN);
    if (buttonValue < 800) { // SELECT - Choose menu option
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Returning to Menu");
        delay(1000);  // Show selection for 1 second
        return true;
    }
    return false;
}

// Function implementations player cards linked list 
int add_player_card(Card_struct &card) {
    Card_struct* newCard = (Card_struct*)malloc(sizeof(Card_struct));
    if (!newCard) return -1;  // Memory allocation failed
    
    *newCard = card;
    newCard->next = NULL;

    if (player_cards == NULL) {
        player_cards = newCard;
    }
    else {
        newCard->next = player_cards;
        player_cards = newCard;
    }
    return 0;
}

//Function implimentations hidden dealer card
int add_hidden_dealer_card(Card_struct &card){
  hidden_dealer_card = card;
  Show_Hands(); 
  return 0;
  
}
//Function implimentations dealer visable cards linked list 
int add_visable_dealer_card(Card_struct &card) {
    Card_struct* newCard = (Card_struct*)malloc(sizeof(Card_struct));
    if (!newCard) return -1;  // Memory allocation failed
    
    *newCard = card;
    newCard->next = NULL;

    if (dealer_cards == NULL) {
        dealer_cards = newCard;
    }
    else {
        newCard->next = dealer_cards;
        dealer_cards = newCard;
    }
    
    Show_Hands(); // Changed from update_LCD() to match current implementation
    return 0;
}
int clear_list(Node* p_head){
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
  return 0;
}
// delete all pre-existing cards in linked lists and hidden dealer card
int new_game (){
  clear_list(player_cards);
  clear_list(dealer_cards);
  hidden_dealer_card = NULL;
  
  update_LCD(); 
  return 0;
}
