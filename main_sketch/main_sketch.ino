#include "wireless.h"
#include "cards.h"
#include "actions.h"
#include <EEPROM.h>

int ID;
//global variables

int currTurn = 0;  //index of current player- ease of print later on
player* circleQueueHead; //points to the current player


void setup() {
  EEPROM.get(0, ID); // Read the ID from EEPROM

  Serial.begin(9600); // Serial port to computer
  bool players[3] = {false, false, false}; //array to keep track of who is playing
  getPlayersAvailable(&players); //get the number of players
  numPlayers = actions.startUp(players)/////////////////////////////////////////////////////////////////////
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
