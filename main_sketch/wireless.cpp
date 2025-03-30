#include <SoftwareSerial.h>
#include "wireless.h"
#include "cards.h"
#include "actions.h"
#include "buttons.h"
#include <LiquidCrystal.h>
#include "display.h"
#include <Arduino.h>
#include <Arduino.h>
#include "wireless.h"
#include "cards.h"
#include <SoftwareSerial.h>

#define rx = 11, tx = 12

#define BAUDRATE 9600 // Globally delcared baud rate

// HC-12 instance (declared globally)
SoftwareSerial HC12(tx, rx);  // TX: 10, RX: 11 in #define

// test function (to see if the header files and #includes worked)
int test_fun() {
    return 178;
}

bool startUp(int playerNumber){
    //ping player and wait for ack response
    CommandPacket packet;
    packet.command = CMD_PING; // Set command to PING
    packet.ID = playerNumber; // Set ID to player number
    packet.betAmount = 0; // Set bet amount to 0
    packet.card.suit = 'X'; // Set suit to 'X'
    packet.card.value = -1; // Set value to -1
    packet.card.friendlyName = 'X'; // Set friendly name to 'X'
    hc12_send(packet); // Send the packet
    delay(2000); // Wait for a response
    // Check for a response from the other player
    if (HC12.available()) {
        CommandPacket rec_pckt = hc12_receive(1); // Receive the packet in mode 1 (returns the packet)
        if(rec_pckt.command == CMD_ACK) { // Check if the received command is ACK
            Serial.println("ACK received"); // Print debug message
            return true; // Return success
        } 
        else return false; // Return failure if not ACK
 
        }

    return false;
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
CommandPacket hc12_receive(int mode = 0) {
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
    if (mode = 0) processCommand(packet);
    if (mode = 1) return packet;
    return -1;
}

// Function to handle received commands
void processCommand(CommandPacket packet) {
    switch (packet.command) { // switch case through the commands enum
        case CMD_PING:
            Serial.println("Received: PING"); // debug print
            break;
        case CMD_ACK:
            Serial.println("Received: ACK");// debug print
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
