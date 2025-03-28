#include <SoftwareSerial.h>
#include "wireless.h"
#include "cards.h"
#include "actions.h"
#include "buttons.h"
#include <LiquidCrystal.h>
#include "display.h"
#include <Arduino.h>

// Declare gameDeck as an external variable
extern Deck_struct gameDeck;

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
    String message = '<' + String(packet.playerID) + "," + String(packet.command) + "," + String(packet.betAmount) + "," + '(' + String(packet.card.suit) + ',' 
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
    packet.playerID = playerID;
    packet.command = CMD_RECEIVE_THIS_CARD;
    packet.card = card;
    packet.betAmount = 0;
    hc12_send(packet);
}

// Function to send turn information to a player
void hc12_send_turn(byte playerID) {
    CommandPacket packet;
    packet.playerID = playerID;
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
    switch (packet.command) {
        case CMD_PING:
            Serial.println("Received: PING");
            if (packet.playerID == deviceID) {
                HC12.print("<0,1,0,(0,0,0)>");  // Send back to all players
            }
            break;
        case CMD_ACK:
            Serial.println("Received: ACK");
            if (deviceID == 0) {
                hasAcknowledged = true;
                temp_ID = packet.playerID;
            }
            break;
        case CMD_RECEIVE_THIS_CARD:
            Serial.println("Processing RECEIVE_CARD command...");
            // Extract player ID and card details from the packet
            Card_struct receivedCard = packet.card;

            // Add the card to the player's hand
            add_player_card(&receivedCard);
            break;
        case CMD_HIT:
            Serial.println("Processing HIT command...");
            hit(players[packet.playerID], &gameDeck);  // Use the external gameDeck
            break;
        case CMD_STAY:
            Serial.println("Processing STAY command...");
            stand(players[packet.playerID]);
            break;
        case CMD_FOLD:
            Serial.println("Processing FOLD command...");
            fold(players[packet.playerID]);
            break;
        case CMD_BET:
            Serial.println("Processing BET command...");
            players[packet.playerID]->totalBet += packet.betAmount;
            break;
        case CMD_DOUBLE_DOWN:
            Serial.println("Processing DOUBLE DOWN command...");
            doubleDown(players[packet.playerID]);
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
