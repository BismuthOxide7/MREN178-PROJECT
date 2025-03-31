#ifndef _actions_h
#define _actions_h

#include "cards.h"
#include <Arduino.h>

extern typedef struct player {
    int playerNumber;
    int totalMoney = 100;
    int totalSum = 0;          
    *Card_struct head;
    *Card_struct next;
    bool stand = false;
    bool fold = false;
    int totalBet = 0;
  } player;

  //function prototypes
  
  makePlayer(int number, *Card_struct heads)

#endif
