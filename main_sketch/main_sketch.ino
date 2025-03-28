#include <SoftwareSerial.h>
#include "wireless.h"
#include "cards.h"
#include "actions.h"
#include "buttons.h"
#include "display.h"
#include "dealer.h"
#include <EEPROM.h>
#include "wireless.h"

extern bool isDealer = false;

int ID;
//global variables

int currTurn = 0;  //index of current player- ease of print later on
player* circleQueueHead; //points to the current player


void setup() {
  EEPROM.get(0, ID); // Read the ID from EEPROM
  Serial.begin(9600); // Serial port to computer

  if(ID == 0){
    isDealer = true;
    //DEALER ARDUINO, handles wireless transmission and deck
    dealer_init_game(); //initialize the dealer
  }

  else{
    //PLAYER ARDUINO, handles buttons and display
    player_init_game(); //initialize the player
  }
}

void player_init_game(){
  //make player hand using hand struct

  //Start wireless and wait for dealer ping


  //gameplay loop 
  while(1){
    //check for button presses, transmit commands, and update display
    //check for dealer message
    //check for player actions and update display
    //check for game over conditions and update display
  }
  
}

void dealer_init_game(){
  //if this function is called, then the dealer is the first player
  //initialize the dealer with a player struct and set the ID to 0
  player* dealer = (player *)malloc(sizeof(player)); //allocate memory for the dealer
  dealer->playerNumber = 0; //set the player number to 0 for the dealer
  dealer->totalMoney = 100; //set the starting money for the dealer
  dealer->totalSum = 0; //set the starting sum for the dealer
  dealer->head = NULL; //set the head of the hand to null
  dealer->next = NULL; //set the next card to null
  dealer->outOfGame = false; //set the dealer to not out of game
  dealer->totalBet = 0; //set the dealer's total bet to 0
  playerQueue[0] = dealer; //set the dealer as the first player in the queue
  
  
  //use startUp to ping every player and determine how many are in the game
  //if a player is available, instatiate a player struct and enrol in the queue
  for(i = 1; i < 4; i++){
    if(startUp(i) == true){
      //player is available, add to queue
      player* newPlayer = (player *)malloc(sizeof(player)); //allocate memory for the new player
      newPlayer->playerNumber = i; //set the player number
      newPlayer->totalMoney = 100; //set the starting money for the player
      newPlayer->totalSum = 0; //set the starting sum for the player
      newPlayer->head = NULL; //set the head of the hand to null
      newPlayer->next = NULL; //set the next card to null
      playerQueue[i] = newPlayer; //add the player to the queue
      playerQueue[i]->outOfGame = false; //set the player to not out of game
      numPlayers++;
    }
    }

  circleQueueHead = playerQueue[0]; //starting with the dealer player
  
  //initialize the deck and shuffle it
  Deck_struct deck;
  initialize_deck(deck); //initialize the deck with 52 cards
  shuffle_deck(deck); //shuffle the deck using Fisher-Yates algorithm


  //main gameplay loop
  while(1){
    //check for player actions and update display
    //check for dealer actions and update display
    //check for game over conditions and update display
  }
  //free memory for each player
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
