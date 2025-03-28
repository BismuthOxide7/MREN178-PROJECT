#include <SoftwareSerial.h>
#include "wireless.h"
#include "cards.h"
#include "actions.h"
#include "buttons.h"
#include <LiquidCrystal.h>
#include "display.h"
#include <Arduino.h>
#include <EEPROM.h>

// Global variables
int currTurn = 0;  // Index of the current player
player* playerQueue[4];  // Circular queue for players
int numPlayers = 4;  // Total number of players
player* circleQueueHead;  // Points to the current player

void setup() {
  // Initialize ID from EEPROM
  EEPROM.get(0, PLAYER_ID);

  Serial.begin(9600);  // Serial port to computer

  // Initialize wireless communication
  bool players[3] = {false, false, false};  // Array to track active players
  getPlayersAvailable(players);  // Get the number of players available

  // Initialize players
  numPlayers = startUp(players);  // Initialize players and get the count
  for (int i = 0; i < numPlayers; i++) {
    playerQueue[i] = (player*)malloc(sizeof(player));  // Allocate memory for each player
    actions.hit(playerQueue[i], &deck);  // Pass the deck to the hit function
    actions.hit(playerQueue[i]);  // Second starting card
    playerQueue[i]->playerNumber = i + 1;
  }
  circleQueueHead = playerQueue[0];  // Start with the first player

  // Initialize display
  new_game();  // Clear the display for a new game
}

void loop() {
  // Check if the current player is still in the game
  if (!circleQueueHead->outOfGame) {
    // Display the current player's turn
    Serial.print("Player ");
    Serial.print(circleQueueHead->playerNumber);
    Serial.println("'s turn!");

    // Check buttons for player action
    checkButtons();

    // Perform action based on button press
    switch (menuIndex) {
      case 0:  // Hit
        actions.hit(circleQueueHead);
        break;
      case 1:  // Stand
        actions.stand(circleQueueHead);
        break;
      case 2:  // Fold
        actions.fold(circleQueueHead);
        break;
      case 3:  // Double Down
        actions.doubleDown(circleQueueHead);
        break;
      default:
        break;
    }

    // Update the display
    //update_LCD();
  }

  // Move to the next player in the circular queue
  if (circleQueueHead == playerQueue[numPlayers - 1]) {
    circleQueueHead = playerQueue[0];
    currTurn = 0;
  } else {
    circleQueueHead = playerQueue[++currTurn];
  }
}
