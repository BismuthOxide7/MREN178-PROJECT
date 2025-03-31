#ifndef ACTIONS_H_
#define ACTIONS_H_

#include "cards.h"
#include <Arduino.h>

// Player struct definition
typedef struct player {
    int playerNumber;
    int totalMoney;
    int totalSum;          
    Card_struct* head;  // Changed from Card_Struct to Card_struct
    bool over21;
    bool stand;
    bool fold;
    int totalBet;
} player;

// Function prototypes
player* makePlayer(int number, Card_struct* heads);
void hit(player* currPlay);
void stand(player* currPlay);
void fold(player* currPlay);
void initialBet(player* currPlay);
void doubleDown(player* currPlay);

#endif /* ACTIONS_H_ */
