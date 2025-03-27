#include "wireless.h"
#include "cards.h"
#include "player.h"

//global variables

int currTurn = 0;  //index of current player- ease of print later on
player* circleQueueHead; //points to the current player


void setup() {
  Serial.begin(9600); // Serial port to computer
  numPlayers = actions.startUp(/*BENS SIGNALS*/)/////////////////////////////////////////////////////////////////////
  player *playerQueue[numPlayers]; 
  for (int i = 0; i < numPlayers; i++) {
    playerQueue[i] = (player *)malloc(sizeof(player)); //setting up the hand for each- allocating mem
    actions.hit(playerQueue[i]); //first starting card
    actions.hit(playerQueue[i]); //second starting card
    playerQueue[i]->playerNumber = i+1;
  }
  circleQueueHead = playerQueue[0]; //starting with the first player
}

void loop() {
 if( circleQueueHead->outOfGame == false){
  //talk to erin and yasmine
 }

 if(*circleQueueHead == playerQueue[numPlayers -1]) //chaning turn (typo but im pissed off at the curser)
 {
  circleQueueHead = playerQueue[0];
  currPlayer = 1;
 }
 else{
  circleQueueHead = playerQueue[currPlayers]; //currplayer is always one higher to account for the 0th spot in the array, so this increases the turn by 1
  currPlayer++;
}

