#include <Arduino.h>
#include "wireless.h"
#include "cards.h"
#include <SoftwareSerial.h>
#include <string.h>

#define BAUDRATE 9600  // Corrected #DEFINE to #define

// HC-12 instance (declared globally)
SoftwareSerial HC12(12, 11);  // TX: 10, RX: 11

// test function (to see if the header files and #includes worked)
int test_fun() {
    return 178;
}

// Function to initialize the transceiver
int initialise_transciever(int tx_pin, int rx_pin) {
    HC12.begin(BAUDRATE);  // Start communication
    return 0;  // Return success
}

void config_transciever(int num_player){
    //Probabry dont need to actually do this is the config is done beforehand
    return;
}


// Function to send a command packet
void hc12_send(CommandPacket packet) {
    //Message order: <ID, CMD, BET, (SUIT, VALUE, FRIENDLYNAME)>
    String message = '<' String(packet.ID) + "," + String(packet.command) + "," + String(packet.betAmount) + "," + '(' + packet.card.suit + ',' 
    + packet.card.value + ',' + packet.card.friendlyName + ')' + '>';    
    
    // Send the string via HC12
    HC12.print(message);
}

// Function to receive a command packet
void hc12_receive() {
    String data = "";
  
    // If HC-12 has data, read it
    switch(HC12.peek()){
        case '<':
            while (HC12.available()) {        
                data += (char)HC12.read();      // Read the byte from HC12 and add to the string
            }
            break;
        default:
            return;
    }

    CommandPacket packet; // Create a new packet
    packet.ID = data.substring(1, data.indexOf(",")).toInt(); // Extract ID
    data = data.substring(data.indexOf(",") + 1); // Remove ID from string
    packet.command = (CommandType)data.substring(0, data.indexOf(",")).toInt(); // Extract command
    data = data.substring(data.indexOf(",") + 1); // Remove command from string
    packet.betAmount = data.substring(0, data.indexOf(",")).toInt(); // Extract bet amount
    data = data.substring(data.indexOf("(") + 1); // Remove bet amount from string
    packet.card.suit = data.substring(0, data.indexOf(",")).toInt(); // Extract suit
    data = data.substring(data.indexOf(",") + 1); // Remove suit from string
    packet.card.value = data.substring(0, data.indexOf(",")).toInt(); // Extract value
    data = data.substring(data.indexOf(",") + 1); // Remove value from string
    packet.card.friendlyName = data.substring(0, data.indexOf(")")); // Extract friendly name
        
    processCommand(packet); // Process the command
    return;
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
