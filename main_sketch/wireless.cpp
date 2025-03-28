#include <SoftwareSerial.h>
#include "wireless.h"
#include "cards.h"
#include "actions.h"
#include "buttons.h"
#include <LiquidCrystal.h>
#include "display.h"
#include <Arduino.h>

#define tx 12
#define rx 11

#define BAUDRATE 9600 // Globally delcared baud rate

// HC-12 instance (declared globally)
SoftwareSerial HC12(tx, rx);  // TX: 10, RX: 11 in #define

// Global variables
int i;
bool hasAcknowledged = false;
int temp_ID;



// test function (to see if the header files and #includes worked)
int test_fun() {
    return 178;
}

// Function to initialize the transceiver
int initalise_transciever() {
    HC12.begin(BAUDRATE);  // Start communication
    delay(100); // Wait
    if(HC12.available()){
        return 0;  // Return success
    } else return 1; // return failure. 
}

// Function to send a command packet
void hc12_send(CommandPacket packet) {
    // Message order: <ID, CMD, BET, (SUIT, VALUE, FRIENDLYNAME)>
    String message = '<' + String(packet.ID) + "," + String(packet.command) + "," + String(packet.betAmount) + "," + '(' + String(packet.card.suit) + ',' 
    + String(packet.card.value) + ',' + String(packet.card.friendlyName) + ')' + '>';    
    //Serial.println("sent: " + message);
    // Send the string via HC12
    HC12.print(message);
}

// Function to initialize HC-12
void hc12_init() {
    HC12.begin(BAUDRATE);
    delay(100);
    Serial.println("HC-12 Initialized");
}

// Function to send a card to a player
void hc12_send_card(byte playerID, Card_struct card) {
    CommandPacket packet;
    packet.ID = playerID;
    packet.command = CMD_RECEIVE_THIS_CARD;
    packet.card = card;
    packet.betAmount = 0;
    hc12_send(packet);
}

// Function to send turn information to a player
void hc12_send_turn(byte playerID) {
    CommandPacket packet;
    packet.ID = playerID;
    packet.command = CMD_PLAYER_TURN;
    packet.betAmount = 0;
    packet.card = make_card('0', 0, '0');  // Dummy card
    hc12_send(packet);
}

// Function to receive a command packet
CommandPacket hc12_receive(void) {
    CommandPacket packet = {0};  // Initialize empty packet
    String data = "";
    
    if (HC12.peek() != '<') {
        while(HC12.available() && HC12.peek() != '<') {
            HC12.read();  // Discard until start marker
        }
        return packet;
    }

    // Read until end marker
    while (HC12.available()) {        
        char c = (char)HC12.read();
        if (c == '>') break;
        data += c;
    }

    if (data.length() < 5) return packet;  // Invalid packet

    // Parse packet
    data = data.substring(1);  // Remove '<'
    packet.playerID = data.substring(0, data.indexOf(",")).toInt();
    data = data.substring(data.indexOf(",") + 1);
    packet.command = (CommandType)data.substring(0, data.indexOf(",")).toInt();
    data = data.substring(data.indexOf(",") + 1);
    packet.betAmount = data.substring(0, data.indexOf(",")).toInt();
    
    // Parse card data
    int cardStart = data.indexOf("(") + 1;
    if (cardStart > 0) {
        String cardData = data.substring(cardStart, data.indexOf(")"));
        String cardParts[3];
        int idx = 0;
        int lastComma = -1;
        
        for (int i = 0; i < cardData.length() && idx < 3; i++) {
            if (cardData[i] == ',') {
                cardParts[idx++] = cardData.substring(lastComma + 1, i);
                lastComma = i;
            }
        }
        if (idx == 2) {
            cardParts[2] = cardData.substring(lastComma + 1);
        }

        packet.card.suit = cardParts[0][0];
        packet.card.value = cardParts[1].toInt();
        packet.card.friendlyName = cardParts[2][0];
    }

    return packet;
}

// Function to handle received commands
extern void processCommand(CommandPacket packet) {
    switch (packet.command) { // switch case through the commands enum
        case CMD_PING:
            Serial.println("Received: PING"); // debug print
            //return an ACK
            if(packet.ID == PLAYER_ID){
                HC12.print("<0,1,0,(0,0,0)>"); //Send back to 0 all players
            }
            break;
        case CMD_ACK:
            Serial.println("Received: ACK");// debug print
            if(PLAYER_ID == 0){
                hasAcknowledged = true;
                temp_ID = packet.ID;
            }
            break;
        case CMD_RECEIVE_THIS_CARD:
            Serial.println("Processing RECEIVE_CARD command...");
            // Extract player ID and card details from the packet
            int playerID = packet.ID;  // Player number
            Card_struct receivedCard;
            receivedCard.friendlyName = packet.card.friendlyName;  // Card rank (e.g., 2-10, J, Q, K, A)
            receivedCard.suit = packet.card.suit;  // Card suit (e.g., Hearts, Diamonds, etc.)
            receivedCard.value = packet.card.value; // Card value (e.g., 10 for face cards, 11 for Ace)

        // Add the card to the player's hand
            //NEED FUNCTION
            break;
        case CMD_HIT:
            Serial.println("Processing HIT command...");
            hit(&playerQueue[packet.ID]);  // Call the hit function for the player
            break;
        case CMD_STAY:
            Serial.println("Processing STAY command...");
            stand(&playerQueue[packet.ID]);  // Call the stand function for the player
            break;
        case CMD_FOLD:
            Serial.println("Processing FOLD command...");
            fold(&playerQueue[packet.ID]);  // Call the fold function for the player
            break;
        case CMD_BET:
            Serial.println("Processing BET command...");
            initialBet(&playerQueue[packet.ID]);  // Call the initialBet function for the player
            break;
        case CMD_DOUBLE_DOWN:
            Serial.println("Processing DOUBLE DOWN command...");
            doubleDown(&playerQueue[packet.ID]);  // Call the doubleDown function for the player
            break;
        default:
            Serial.println("Unknown command received.");
            break;
    }
}

// Function to get the number of players available
bool getPlayersAvailable(bool* players) {
    // Get the number of players available
    // For now, we will assume that there are 3 players
    //check all players availability - slight delay between each response to avoid collision
   
    for(i = 1; i <= 3; i++){
        HC12.print("<" + String(i) + ",1,0,(0,0,0)>"); //Send PING player i 
        while(!HC12.available());
        String data = "";
        while (HC12.available()){
            hc12_receive();
            delay(100);
            if(hasAcknowledged){
                hasAcknowledged = false;
                players[temp_ID] = true;
                break;
            }
        }
        delay(200);
    }
    return true;
}
