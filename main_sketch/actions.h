#ifndef _actions_h
#define _actions_h

#include "cards.h"
#include <Arduino.h>

typedef struct {
    int playerNumber;
    int totalMoney;
    int totalSum;          
    struct Node* head;  // Use Node* for linked list
    bool outOfGame;
    int totalBet;
} player;

// Function prototypes
void hit(player *currPlay, Deck_struct *deck);
void stand(player *currPlay);
void fold(player *currPlay);
int initialBet(player *currPlay);
int doubleDown(player *currPlay);

#endif
