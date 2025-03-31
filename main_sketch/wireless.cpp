#include <SoftwareSerial.h>
#include "wireless.h"
#include "cards.h"
#include "actions.h"
#include "buttons.h"
#include <LiquidCrystal.h>
#include "display.h"
#include <Arduino.h>

// Declare the ID variable as extern to reference the one in main.ino
extern int ID;

extern bool isPinged = false; // Flag to check if pinged

// Fix the pin definitions
#define RX_PIN 11
#define TX_PIN 12

#define BAUDRATE 9600 // Globally declared baud rate

// Define the HC12 object
SoftwareSerial HC12(TX_PIN, RX_PIN);  // TX, RX

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

    Serial.print("Sending PING to player ");
    Serial.println(playerNumber);
    hc12_send(packet); // Send the packet
    unsigned long startTime = millis();
    while (millis() - startTime < 500) {
        // Non-blocking delay
    }

    // Check for a response from the other player
    if (HC12.available()) {
        Serial.println("Data available on HC12, receiving...");
        CommandPacket rec_pckt = hc12_receive(1); // Receive the packet in mode 1 (returns the packet)
        if(rec_pckt.command == CMD_ACK) { // Check if the received command is ACK
            Serial.println("ACK received"); // Print debug message
            return true; // Return success
        } else {
            Serial.print("Received non-ACK command: ");
            Serial.println(rec_pckt.command);
            return false; // Return failure if not ACK
        }
    } else {
        Serial.println("No data available on HC12 after ping.");
        return false; // Return failure if not ACK
    }
    isPinged = true;
    return false;
}


// Function to send a command packet
void hc12_send(CommandPacket packet) {
    // Message order: <ID, CMD, BET, (SUIT, VALUE, FRIENDLYNAME)>
    String message = '<' + String(packet.ID) + "," + String(packet.command) + "," + String(packet.betAmount) + "," + '(' + String(packet.card.suit) + ',' 
    + String(packet.card.value) + ',' + String(packet.card.friendlyName) + ')' + '>';    
    Serial.println("sent: " + message);
    // Send the string via HC12
    HC12.print(message);
}

// Function to receive a command packet
CommandPacket hc12_receive(int mode) {
    String data = "";
  
    // If HC-12 has data, read it until '>'
    if (HC12.peek() == '<') {
        while (HC12.available()) {        
            char receivedChar = (char)HC12.read();
            data += receivedChar;
            if (receivedChar == '>'){
              Serial.println("rcv: " + data);
              break;  // Stop reading when '>' is found
            }
        }
    } else {
      while(HC12.peek() != '<') (HC12.read()); // consume char
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
        
    if (mode == 0) processCommand(packet);
    if (mode == 1) return packet;
}

// Function to handle received commands
void processCommand(CommandPacket packet) {
    switch (packet.command) { // switch case through the commands enum
        case CMD_PING:
            Serial.println("Received: PING"); // debug print
            // Send ACK back to sender if its the starup ping
            if(!isPinged){ // Check if the player is pinged
                CommandPacket ackPacket; // Create a new packet for ACK
                ackPacket.command = CMD_ACK; // Set command to ACK
                ackPacket.ID = packet.ID; // Set ID to sender's ID
                ackPacket.betAmount = 0; // Set bet amount to 0
                ackPacket.card.suit = 'X'; // Set suit to 'X'
                ackPacket.card.value = -1; // Set value to -1
                ackPacket.card.friendlyName = 'X'; // Set friendly name to 'X'
                Serial.println("Sending ACK");
                hc12_send(ackPacket); // Send ACK packet
                Serial.println("ACK sent"); // debug print
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("Added to Game");
                lcd.setCursor(1,1);
                lcd.print("Wait for Turn");

            } else {
                //print to LCD that it's the player's turn
                lcd.clear(); // Clear the LCD
                lcd.setCursor(0, 0); // Set cursor to first line
                lcd.print("Your turn!"); // Print "Your turn!" to the LCD
                lcd.setCursor(0, 1); // Set cursor to second line
                lcd.print("Press Select"); // Print to the LCD
                checkButtons();
            }
            
            break;
        case CMD_ACK:
            Serial.println("Received: ACK");// debug print
            //do nothing, ack should get picked up in mode 1 receives
            break;
        case CMD_RECEIVE_THIS_CARD:
            //add card to player_struct hand if packet id = player id 
            Card_struct newCard;
            newCard.suit = packet.card.suit; // Set suit to received card's suit
            newCard.value = packet.card.value; // Set value to received card's value
            newCard.friendlyName = packet.card.friendlyName; // Set friendly name to received card's friendly name
            // Add the card to the player's hand
            if(packet.ID == ID) {
                // Add the card to the player's hand
                //add_player_card(&newCard); // Uncomment this line to add the card to the player's hand
            } else if (packet.ID == 0) {
                // Add the card to the dealer's hand
                if(is_hidden == 1) {
                    //add_hidden_dealer_card(&newCard); 
                    is_hidden = 0; // Set is_hidden to 0 if the dealer's card is hidden
                } else return;//add_visable_dealer_card(&newCard); // Add the card to the dealer's visible hand
            }

            Serial.print("Received Card: ");// debug print
            Serial.print(packet.card.suit);// debug print
            Serial.print(" ");// debug print
            Serial.print(packet.card.value);// debug print
            Serial.print(" ");// debug print
            Serial.println(packet.card.friendlyName);// debug print
            break;
        case CMD_HIT:
            Serial.println("Received: HIT");// debug print
            // Call hit function with the current player
            //hit(circleQueueHead); // Uncomment this line to call the hit function
            // Add the card to the player's hand
            //add_player_card(packet.card); // Uncomment this line to add the card to the player's hand
            break;
        case CMD_STAY:
            Serial.println("Received: STAY");// debug print
            //dealer moves on to next player
            // Call stay function with the current player
            //stay(circleQueueHead); // Uncomment this line to call the stay function
            break;
        case CMD_FOLD:
            Serial.println("Received: FOLD");// debug print
            //Player folds, dealer removes it from queue
            break;
        case CMD_WIN:
            Serial.println("Received: WIN");// debug print
            //Print that you won to the LCD, and update the player struct with the new money
            //Add the bet amount to the player's total money
            //lcd.print("You won!"); // Uncomment this line to print "You won!" to the LCD
            //lcd.print("You have: "); // Uncomment this line to print "You have:" to the LCD
            //print total money adding the packet bet amount to the player structs money
            //lcd.print(packet.betAmount + player_struct.totalMoney); // Uncomment this line to print the total money to the LCD
            //player_struct.totalMoney += packet.betAmount; // Uncomment this line to update the player's total money
            break;
        case CMD_LOSE:
            Serial.println("Received: LOSE");// debug print
            //Print that you lost to the LCD, and update the player struct with the new money
            //lcd.print("You lost!"); // Uncomment this line to print "You lost!" to the LCD
            //lcd.print("You have: "); // Uncomment this line to print "You have:" to the LCD
            //print total money to the lcd
            //lcd.print(player_struct.totalMoney); // Uncomment this line to print the total money to the LCD
            //free all allocated memory (hands in display, etc)
            //rerun setup function
            break;
        default:
            Serial.println("Unknown command received");// debug print
            break;
    }
}
