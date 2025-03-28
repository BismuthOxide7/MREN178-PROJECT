#ifndef _ACTIONS_H
#define _ACTIONS_H

#include "cards.h"
#include <Arduino.h>

typedef struct player {
    uint8_t playerNumber;  // Changed to uint8_t for consistency
    int totalMoney;
    int totalSum;          
    struct Node* head;  // Hand of cards
    bool outOfGame;
    int totalBet;
    bool hasResponded;
    unsigned long timeout;
} player;

// Global player array declaration
extern player* players[4];
extern uint8_t currentPlayer;

// Function prototypes
void hit(player* currPlay, Deck_struct* deck);
void stand(player* currPlay);
void fold(player* currPlay);
int initialBet(player* currPlay);
int doubleDown(player* currPlay);
void cleanup_player(player* p);

#endif
