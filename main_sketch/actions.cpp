#include "cards.h"
#include "actions.h"
#include <Arduino.h>  // Include Arduino-specific functions
#include <stdlib.h>

// Global variables
int tokens = 100;  // Default starting tokens for players
int numPlayers = 4;

// Function to initialize players based on availability
int startUp(bool p2, bool p3, bool p4) {
    if (!p2) {
        numPlayers--;
    }
    if (!p3) {
        numPlayers--;
    }
    if (!p4) {
        numPlayers--;
    }
    return numPlayers;
}

// Function to handle "hit" action
void hit(player *currPlay, Deck_struct *deck) {
    // Draw a card from the deck
    Card_struct newCard = draw_card(deck);

    // Create a new node for the linked list
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->card = newCard;
    newNode->next = NULL;

    // Add the new node to the player's hand
    if (currPlay->head == NULL) {
        currPlay->head = newNode;  // First card in the hand
    } else {
        struct Node* traverse = currPlay->head;
        while (traverse->next != NULL) {
            traverse = traverse->next;
        }
        traverse->next = newNode;  // Add card to the end of the hand
    }

    // Update the player's total sum
    currPlay->totalSum += newCard.value;

    // Check if the player is out of the game
    if (currPlay->totalSum > 21) {
        currPlay->outOfGame = true;  // Player is out of the game
        Serial.print("Player ");
        Serial.print(currPlay->playerNumber);
        Serial.println(" is out of the game!");
    }
}

// Function to handle "stand" action
void stand(player *currPlay) {
    Serial.print("Player ");
    Serial.print(currPlay->playerNumber);
    Serial.println(" stands.");
}

// Function to handle "fold" action
void fold(player *currPlay) {
    currPlay->totalBet /= 2;  // Half the bet is retracted
    currPlay->outOfGame = true;  // Player folds
    Serial.print("Player ");
    Serial.print(currPlay->playerNumber);
    Serial.println(" folds.");
}

// Function to handle initial betting
int initialBet(player *currPlay) {
    Serial.print("Player ");
    Serial.print(currPlay->playerNumber);
    Serial.println(", place your initial bet:");

    int bet = 10;  // Example: Default bet amount
    currPlay->totalBet = bet;
    currPlay->totalMoney -= bet;

    Serial.print("Player ");
    Serial.print(currPlay->playerNumber);
    Serial.print(" bet ");
    Serial.print(bet);
    Serial.println(" tokens.");
    return bet;
}

// Function to handle "double down" action
int doubleDown(player *currPlay) {
    if (currPlay->totalMoney >= currPlay->totalBet) {
        currPlay->totalBet *= 2;
        currPlay->totalMoney -= currPlay->totalBet / 2;
        Serial.print("Player ");
        Serial.print(currPlay->playerNumber);
        Serial.println(" doubles down!");
        return currPlay->totalBet;
    } else {
        Serial.println("Not enough money to double down!");
        return -1;
    }
}