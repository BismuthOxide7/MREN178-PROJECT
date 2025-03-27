#include <Arduino.h>
#include "wireless.h"
#include "cards.h"
#include <SoftwareSerial.h>

#define rx = 11, tx = 12

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



// Function to receive a command packet
void hc12_receive() {
    String data = "";
  
    // If HC-12 has data, read it until '>'
    if (HC12.peek() == '<') {
        while (HC12.available()) {        
            char receivedChar = (char)HC12.read();
            data += receivedChar;
            if (receivedChar == '>'){
              //DEBUG Serial.println("rcv: " + data);
              break;  // Stop reading when '>' is found
            }
        }
    } else {
      while(HC12.peek() != '<') Serial.print(HC12.read()); // consume char
    }

    CommandPacket packet; // Create a new packet
    packet.ID = data.substring(1, data.indexOf(",")).toInt(); // Extract ID
    data = data.substring(data.indexOf(",") + 1); // Remove ID from string
    packet.command = (CommandType)data.substring(0, data.indexOf(",")).toInt(); // Extract command
    data = data.substring(data.indexOf(",") + 1); // Remove command from string
    packet.betAmount = data.substring(0, data.indexOf(",")).toInt(); // Extract bet amount
    data = data.substring(data.indexOf("(") + 1); // Remove bet amount from string
    packet.card.suit = data.substring(0, data.indexOf(",")).charAt(0); // Extract suit
    data = data.substring(data.indexOf(",") + 1); // Remove suit from string
    packet.card.value = data.substring(0, data.indexOf(",")).toInt(); // Extract value
    data = data.substring(data.indexOf(",") + 1); // Remove value from string
    packet.card.friendlyName = data.substring(0, data.indexOf(",")).charAt(0); // Extract friendly name
        
    // Process command or do something with the packet here
    processCommand(packet);
    return;
}

// Function to handle received commands
void processCommand(CommandPacket packet) {
    switch (packet.command) { // switch case through the commands enum
        case CMD_PING:
            Serial.println("Received: PING"); // debug print
            //return an ACK
            if(packet.ID == ID){
                HC12.print("<0,1,0,(0,0,0)>"); //Send back to 0 all players
            }
            break;
        case CMD_ACK:
            Serial.println("Received: ACK");// debug print
            if(ID == 0){
                hasAcknowledged = true;
                temp_ID = packet.ID;
            }
            break;
        case CMD_RECEIVE_THIS_CARD:
            Serial.print("Received Card: ");// debug print
            Serial.print(packet.card.suit);// debug print
            Serial.print(" ");// debug print
            Serial.println(packet.card.value);// debug print
            break;
        case CMD_HIT:
            Serial.println("Received: HIT");// debug print
            break;
        case CMD_STAY:
            Serial.println("Received: STAY");// debug print
            break;
        case CMD_FOLD:
            Serial.println("Received: FOLD");// debug print
            break;
        case CMD_WIN:
            Serial.println("Received: WIN");// debug print
            break;
        case CMD_LOSE:
            Serial.println("Received: LOSE");// debug print
            break;
        default:
            Serial.println("Unknown command received");// debug print
            break;
    }
}

// Function to get the number of players available
bool getPlayersAvailable(bool *players) {
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
                *players[temp_ID] = true;
                break;
            }
        }
        delay(200);
    }
    return true;
}
