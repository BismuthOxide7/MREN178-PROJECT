#ifndef _actions_h
#define _actions_h

#include "cards.h"
#include <Arduino.h>

typedef struct {
    int playerNumber;
    int totalMoney = 100;
    int totalSum = 100;          
    *Card_struct head;
    *Card_struct next;
    bool outOfGame = false;
    int totalBet = 0;
  } player;

  //function prototypes
  
  makePlayer(int number, *Card_struct heads)

#endif
