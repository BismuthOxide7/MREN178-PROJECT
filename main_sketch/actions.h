#ifndef _actions_h
#define _actions_h

#include "cards.h"
#include <Arduino.h>

typedef struct player {
    int playerNumber;
    int totalMoney;
    int totalSum;          
    struct Node* head;  // Hand of cards
    bool outOfGame;
    int totalBet;
    bool hasResponded;
    unsigned long timeout;
} player;

// Add these declarations
extern player* players[4];  // Array of all players
extern byte currentPlayerIndex;

// Function prototypes
void hit(player *currPlay, Deck_struct *deck);
void stand(player *currPlay);
void fold(player *currPlay);
int initialBet(player *currPlay);
int doubleDown(player *currPlay);
void cleanup_player(player* p);

#endif
