#include <Arduino.h>
#include "wireless.h"
#include "cards.h"
#include <SoftwareSerial.h>

#define BAUDRATE 9600  // Corrected #DEFINE to #define

// HC-12 instance (declared globally)
SoftwareSerial HC12(10, 11);  // TX: 10, RX: 11

// test function (to see if the header files and #includes worked)
int test_fun() {
    return 178;
}

// Function to initialize the transceiver
int initialise_transciever(int tx_pin, int rx_pin) {
    HC12.begin(BAUDRATE);  // Start communication
    return 0;  // Return success
}

// Function to send a command packet
void hc12_send(CommandType cmd, Card_struct card = {'X',-1,'X'}) {
    CommandPacket packet; //init packet struct
    packet.command = cmd; //assign command
    packet.card = card; // assign card (default is the null card X,-1,X)
    
    HC12.write((uint8_t*)&packet, sizeof(CommandPacket));  // Send as bytes
}

// Function to receive a command packet
void hc12_receive() {
    CommandPacket packet; //init packet
    static uint8_t buffer[sizeof(CommandPacket)];  // Buffer for incoming data
    static uint8_t index = 0;  // Tracks received bytes

    while (HC12.available()) {
        buffer[index] = HC12.read();  // Read byte
        index++; // move to next buffer spot

        // If we have a full packet, process it
        if (index >= sizeof(CommandPacket)) {
            memcpy(&packet, buffer, sizeof(CommandPacket));  // Copy buffer data into the packet
            processCommand(packet);  // Handle received command
            index = 0;  // Reset buffer index
        }
    }
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
