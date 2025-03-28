#include <Arduino.h>
#include "cards.h"
#include "display.h"
#include <LiquidCrystal.h>

// Initialize the library by associating any needed LCD interface pin
// with the Arduino pin number it is connected to
const int pin_RS = 8;
const int pin_EN = 9;
const int pin_d4 = 4;
const int pin_d5 = 5;
const int pin_d6 = 6;
const int pin_d7 = 7;
const int pin_BL = 10;
LiquidCrystal lcd(pin_RS, pin_EN, pin_d4, pin_d5, pin_d6, pin_d7);  // Properly named object

// Define global variables
Card_struct hidden_dealer_card;
Node* player_cards = NULL;
Node* dealer_cards = NULL;

// Create node
Node* createNode(Card_struct card) {
  Node* newNode = (Node*)malloc(sizeof(Node));
  newNode->card = card;
  newNode->next = NULL;
  return newNode;
}

// Update LCD display with linked lists and hidden card
void update_LCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  Node* transverser = player_cards;
  while (transverser != NULL) {
    lcd.print(transverser->card.friendlyName);
    transverser = transverser->next;
  }
  lcd.setCursor(1, 0);
  lcd.print("H");  // Printing hidden card - later needs to be shown
  lcd.setCursor(1, 1);
  transverser = dealer_cards;
  while (transverser != NULL) {
    lcd.print(transverser->card.friendlyName);
    transverser = transverser->next;
  }
}

// Function implementations for player cards linked list
int add_player_card(Card_struct* card) {
  Node* node = createNode(*card);
  if (player_cards == NULL) {
    player_cards = node;
  } else {
    node->next = player_cards;
    player_cards = node;
  }
  update_LCD();
  return 0;
}

// Function implementations for hidden dealer card
int add_hidden_dealer_card(Card_struct* card) {
  hidden_dealer_card = *card;
  update_LCD();
  return 0;
}

// Function implementations for dealer visible cards linked list
int add_visible_dealer_card(Card_struct* card) {
  Node* node = createNode(*card);
  if (dealer_cards == NULL) {
    dealer_cards = node;
  } else {
    node->next = dealer_cards;
    dealer_cards = node;
  }
  update_LCD();
  return 0;
}

// Clear linked list
int clear_list(Node* p_head) {
  Node* p_temp;
  while (p_head != NULL) {
    p_temp = p_head;
    p_head = p_head->next;
    free(p_temp);
  }
  return 0;
}

// Delete all pre-existing cards in linked lists and hidden dealer card
int new_game() {
  clear_list(player_cards);
  clear_list(dealer_cards);
  hidden_dealer_card = {0};  // Reset hidden dealer card
  update_LCD();
  return 0;
}
