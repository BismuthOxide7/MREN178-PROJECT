#include "game.h"
#include <EEPROM.h>

// Global variables
bool isDealer = false;
int ID;

extern Deck_struct* deck = NULL;

// Player structs
player* This_Player_Struct = NULL;  // Pointer to the player struct
player* Dealer_Struct = NULL;      // Pointer to the dealer struct

// Other globals
int currTurn = 1;  // Index of current player
player** circleQueueHead; // Points to the current player 
player* playerQueue[4];   // Queue of all possible players

EEPROM.get(0, ID); // Read the ID from EEPROM

void setup() {
  Serial.begin(9600); // Serial port to computer
  if(ID == 0){
    isDealer = true;
    //DEALER ARDUINO, handles wireless transmission and deck
    dealer_init_game(); //initialize the dealer
  } else {
    //PLAYER ARDUINO, handles buttons and display
    player_init_game(); //initialize the player
  }
}

void player_init_game(){
  //make player struct for local player
  player* localPlayer = (player *)malloc(sizeof(player)); //allocate memory for the player
  localPlayer->playerNumber = ID; //set the player number to the ID
  localPlayer->totalMoney = 100; //set the starting money for the player
  localPlayer->totalSum = 0; //set the starting sum for the player
  localPlayer->head = NULL; //set the head of the hand to null
  localPlayer->over21 = false; //shows the player didnt lose
  localPlayer->stand = false; //set the player to not out of game
  localPlayer->fold = false; //set the player to not folded
  localPlayer->totalBet = 0; //set the player's total bet to 0
  This_Player_Struct = localPlayer; //set the player struct to the local player

  //Start wireless and wait for dealer ping
  initialise_transciever(); //initialize the transceiver

  //gameplay loop 
  while(1){
    if(HC12.available()){
      hc12_receive(0); //receive and process a command packet
    }
    //Show menu on LCD
    checkButtons(); //check for and handle button presses
    //check for dealer message
    //check for player actions and update display
    //check for game over conditions and update display
    //thing to call funcions_ ask ben!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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
  dealer->over21 = false; //shows the dealer didnt lose
  dealer->stand = false; //set the dealer to not out of game
  dealer->fold = false; //set the dealer to not folded
  dealer->totalBet = 0; //set the dealer's total bet to 0
  This_Player_Struct = dealer; //set the player struct to the dealer
  playerQueue[0] = dealer; //set the dealer as the first player in the queue
  
    
  //use startUp to ping every player and determine how many are in the game
  //if a player is available, instatiate a player struct and enrol in the queue
  for(int i = 1; i < 4; i++){
    if(startUp(i) == true){
      //player is available, add to queue
      player* newPlayer = (player *)malloc(sizeof(player)); //allocate memory for the new player
      newPlayer->playerNumber = i; //set the player number
      newPlayer->totalMoney = 100; //set the starting money for the player
      newPlayer->totalSum = 0; //set the starting sum for the player
      newPlayer->head = NULL; //set the head of the hand to null
      newPlayer->over21 = false; //shows the player didnt lose
      newPlayer->stand = false; //set the player to not out of game
      newPlayer->fold = false; //set the player to not folded
      newPlayer->totalBet = 0; //set the player's total bet to 0
      playerQueue[i] = newPlayer; //add the player to the queue
      
    }
    }

  circleQueueHead = &playerQueue[0]; //starting with the dealer player
  
  //initialize the deck and shuffle it
  initialize_deck(deck); //initialize the deck with 52 cards
  shuffle_deck(deck); //shuffle the deck using Fisher-Yates algorithm


  //main gameplay loop
  while(1){
    if(HC12.available()){
      hc12_receive(0); //receive and process a command packet
    }
    //operate the queue in the background. ping a player when it is their turn
    //PUT QUEUE FUNCTIONALITY HERE
    //ping the player when it is their turn
    CommandPacket packet;
    packet.command = CMD_PING; // Set command to PING
    packet.ID = (*circleQueueHead)->playerNumber; // Set ID to player number
    packet.betAmount = 0; // Set bet amount to 0
    packet.card.suit = 'X'; // Set suit to 'X'
    packet.card.value = -1; // Set value to -1
    packet.card.friendlyName = 'X'; // Set friendly name to 'X'
    hc12_send(packet); // Send the packet
    free(&packet); //free the packet memory
    delay(2000); // Wait for a response
    hc12_receive(1); //receive the packet in mode 0 to process the command
    
    circleQueueHead = &playerQueue[currTurn];
    while (*circleQueueHead == NULL && currTurn < 4) {
        currTurn = (currTurn + 1) % 4;
        circleQueueHead = &playerQueue[currTurn];
    }

    currTurn = (*circleQueueHead == playerQueue[3]) ? 0 : (currTurn + 1) % 4;

    //Show menu on LCD
    checkButtons(); //check for and handle button presses    
  }
  //free memory for each player
}

void loop() {
  //Never gets reached - all game logic is in the setup function
}
