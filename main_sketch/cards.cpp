#include "cards.h"

// Function to create a card
Card_struct make_card(char suit, int value, char friendlyName) {
  Card_struct card; // init card struct
  card.suit = suit; // assign suit
  card.value = value; // assign value 
  card.friendlyName = friendlyName; // assign friendlyname
  card.next = NULL; // set next to null
  return card;
}

// Function to initialize the deck with 52 cards
void initialize_deck(Deck_struct* deck) {
  char suits[] = {'H', 'D', 'C', 'S'};  // Hearts, Diamonds, Clubs, Spades
  char names[] = {'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K'};  // Card values
  
  deck->top_card = 51;  // Stack starts full (top card is at index 51, the last card)

  int index = 0;
  // Loop through all suits (4)
  for (int s = 0; s < 4; s++) {
    // Loop through all values (1-13) for each suit
    for (int v = 0; v < 13; v++) {
      deck->cards[index] = make_card(suits[s], v + 1, names[v]);  // Create card and add to deck
      index++;
    }
  }
}

// Function to draw a card from the deck (stack behavior)
Card_struct draw_card(Deck_struct* deck) {
  if (deck->top_card < 0) {
    Serial.println("Deck is empty!");  // debug print, error message if deck is empty
    return make_card('X', -1, 'X');  // Return an invalid card
  }
  return deck->cards[deck->top_card--];  // Return top card and decrement index (stack behavior)
}

// Function to shuffle the deck using Fisher-Yates shuffle
void shuffle_deck(Deck_struct* deck) {
  for (int i = 51; i > 0; i--) {  // Start from the last card
    int j = random(0, i + 1);  // Pick a random index between 0 and i
    Card_struct temp = deck->cards[i];  // Swap the cards
    deck->cards[i] = deck->cards[j];
    deck->cards[j] = temp;
  }
}
