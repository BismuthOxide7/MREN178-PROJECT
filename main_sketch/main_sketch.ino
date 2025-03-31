#include "game.h"
#include <EEPROM.h>

// Global variables
bool isDealer = false;
int ID;  // Will be set from EEPROM

extern Deck_struct* deck = NULL;
extern bool isPinged; // Flag to check if pinged

// Player structs
extern player* This_Player_Struct = NULL;  // Pointer to the player struct
player* Dealer_Struct = NULL;      // Pointer to the dealer struct

// Other globals
int currTurn = 1;  // Index of current player
player* circleQueueHead; // Points to the current player 
player* playerQueue[4];   // Queue of all possible players




void setup() {
  ID = (int)EEPROM.read(0); // Read the ID from EEPROM
  const int PLAYER_ID = ID;  // Make constant after read
  ID = PLAYER_ID;  // Assign back to ensure immutability
  Serial.begin(9600); // Serial port to computer
  HC12.begin(9600);
  lcd.begin(16,2);

  Serial.print("ID read from EEPROM: ");
  Serial.println(ID);

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
  Serial.println("Initializing player...");
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

  //print some startup info to the screen
  lcd.clear(); //clear the screen
  lcd.setCursor(0,0); //set the cursor to the first line
  lcd.print("Player ID: "); //print the player ID
  lcd.print(ID); //print the player ID
  lcd.setCursor(0,1); //set the cursor to the second line
  lcd.print("Waiting..."); //print waiting for
  //Start wireless and wait for dealer ping
  Serial.println("Initializing transceiver...");
  if(HC12.available()) Serial.println("Transceiver initialized.");
  while(!isPinged){
    if(HC12.available()){
      hc12_receive(0); //receive the packet in mode 0 to process the command
    }
  } // wait for the dealer to ping the player
  //gameplay loop 
  while(1){
    if(HC12.available()){
      Serial.println("Data available on HC12, receiving...");
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
  Serial.println("Initializing dealer...");
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
  
  //print some startup info to the screen
  lcd.clear(); //clear the screen
  lcd.setCursor(0,0); //set the cursor to the first line
  lcd.print("Player ID: "); //print the player ID
  lcd.print(ID); //print the player ID
  lcd.setCursor(0,1); //set the cursor to the second line
  lcd.print("Setting Up..."); //print waiting for
    
  //use startUp to ping every player and determine how many are in the game
  //if a player is available, instatiate a player struct and enrol in the queue
  int numPlayers = 1; // dealer always plays
  Serial.println("Starting player discovery...");
  for(int i = 1; i < 4; i++){
    Serial.print("Pinging player ");
    Serial.println(i);
    if(startUp(i) == true){
      Serial.print("Player ");
      Serial.print(i);
      Serial.println(" responded");
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
      numPlayers++;
      
      }
      else {
        Serial.print("Player ");
        Serial.print(i);
        Serial.println(" did not respond");
        playerQueue[i] = NULL;
      }
    }
  
  circleQueueHead = playerQueue[0]; //starting with the dealer player
  
  //initialize the deck and shuffle it
  Serial.println("Initializing deck...");
  deck = (Deck_struct*)malloc(sizeof(Deck_struct));
  if (!deck) {
    Serial.println("Failed to allocate deck");
    return;
  }
  initialize_deck(deck); //initialize the deck with 52 cards
  shuffle_deck(deck); //shuffle the deck using Fisher-Yates algorithm
  Serial.println("Deck initialized and shuffled.");


  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Setup Complete");
  lcd.setCursor(0,1);
  lcd.print(numPlayers);
  lcd.print(" Players");


  delay(2000); //wait for 2 seconds to show the setup is complete
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Game Start!");
  bool hasPlayed = false;
  //main gameplay loop
  while(1) {
    if(HC12.available()) {
      hc12_receive(0); //receive and process a command packet
    }
    
    //operate the queue in the background. ping a player when it is their turn
    if(circleQueueHead == playerQueue[0] && !hasPlayed) {
      static bool dealerTurnDisplayed = false;
      static bool menuDisplayed = false;
      static unsigned long lastButtonPress = 0;
      
      // Show initial "Your Turn" message
      if (!dealerTurnDisplayed && !menuDisplayed) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Your Turn!");
        lcd.setCursor(0, 1);
        lcd.print("Press Select");
        dealerTurnDisplayed = true;
      }

      int buttonValue = analogRead(BTN_PIN);
      unsigned long currentTime = millis();
      
      // First SELECT press - Show menu
      if ((buttonValue <= BTN_SELECT + 100 && buttonValue >= BTN_SELECT - 100) && 
          (currentTime - lastButtonPress > 250)) {
        if (dealerTurnDisplayed && !menuDisplayed) {
          Serial.println("SELECT PRESSED - SHOWING MENU");
          dealerTurnDisplayed = false;
          menuDisplayed = true;
          updateMenu();
          lastButtonPress = currentTime;
        } else if (menuDisplayed) {
          // Let checkButtons handle the menu selection
          checkButtons();
          hasPlayed = true;
          currTurn = (currTurn + 1) % 4;
          circleQueueHead = playerQueue[currTurn];
          menuDisplayed = false;
          lastButtonPress = currentTime;
        }
      } 
      
      // Menu navigation - use checkButtons instead of duplicating logic
      if (menuDisplayed && (currentTime - lastButtonPress > 250)) {
        if ((buttonValue <= BTN_UP + 100 && buttonValue >= BTN_UP - 100) || 
            (buttonValue <= BTN_DOWN + 100 && buttonValue >= BTN_DOWN - 100)) {
          checkButtons();  // Let buttons.cpp handle navigation
          lastButtonPress = currentTime;
        }
      }
    } else {
      //iterate if head is null
      while (circleQueueHead == NULL && currTurn < 4) {
        currTurn = (currTurn + 1) % 4;
        circleQueueHead = playerQueue[currTurn];
      }
      
      //player's turn, ping the player and wait for a response
      CommandPacket packet;
      packet.command = CMD_PING;
      packet.ID = (circleQueueHead)->playerNumber;
      packet.betAmount = 0;
      packet.card.suit = 'X';
      packet.card.value = -1;
      packet.card.friendlyName = 'X';
      hc12_send(packet);
      
      //wait for response
      while(HC12.available()) {
        hc12_receive(0);
      }
      hasPlayed = true;
      currTurn = (circleQueueHead == playerQueue[3]) ? 0 : (currTurn + 1) % 4;
    }
  }
}


void loop() {
  //Never gets reached - all game logic is in the setup function
}
