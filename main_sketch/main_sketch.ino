#include <SoftwareSerial.h>
#include "wireless.h"
#include "cards.h"
#include "actions.h"
#include "buttons.h"
#include "display.h"
#include "dealer.h"
#include <EEPROM.h>

// Global variables
uint8_t deviceID;  // Global device ID
bool isDealer = false;
player* players[4] = {NULL, NULL, NULL, NULL};
byte currentPlayerIndex = 0;

void setup() {
    Serial.begin(9600);
    randomSeed(analogRead(0));  // Initialize random seed
    
    // Read device ID from EEPROM
    deviceID = EEPROM.read(0);
    isDealer = (deviceID == 0);
    
    // Initialize HC-12
    hc12_init();
    
    // Initialize LCD and buttons
    lcd.begin(16, 2);
    setupButtons();
    
    // Initialize players
    for (int i = 0; i < 4; i++) {
        players[i] = (player*)malloc(sizeof(player));
        players[i]->playerNumber = i;
        players[i]->totalMoney = 100;  // Starting money
        players[i]->totalSum = 0;
        players[i]->head = NULL;
        players[i]->outOfGame = false;
        players[i]->totalBet = 0;
        players[i]->hasResponded = false;
        players[i]->timeout = 0;
    }
    
    // Initialize dealer if applicable
    if (isDealer) {
        dealer_init();
    }
    
    // Clear display for new game
    new_game();
}

void loop() {
    // Check buttons for player input
    checkButtons();
    
    // Handle incoming messages
    if (HC12.available()) {
        CommandPacket packet = hc12_receive();
        
        if (packet.command != CMD_NONE) {  // Valid packet received
            if (isDealer) {
                dealer_handle_command(packet);
            }
            if (packet.playerID == deviceID || packet.playerID == 255) {  // 255 for broadcast
                processCommand(packet);
            }
        }
    }
    
    // Run dealer background tasks if applicable
    if (isDealer) {
        dealer_background_task();
    }
}
