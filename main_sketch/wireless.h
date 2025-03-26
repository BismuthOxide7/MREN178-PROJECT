#ifndef _WIRELESS_H
#define _WIRELESS_H

#include <Arduino.h>
#include <stdint.h>
#include "cards.h"

// Define available commands
typedef enum {
    CMD_NONE = 0,           // Default / No command
    CMD_PING,               // Ping command
    CMD_ACK,                // Acknowledge
    CMD_RECEIVE_THIS_CARD,  // Send card data
    CMD_HIT,                // Player requests a hit
    CMD_STAY,               // Player chooses to stay
    CMD_FOLD,               // Player folds
    CMD_WIN,                // Player wins this game
    CMD_LOSE                // Player lost this game
} CommandType;

// Struct to hold a command and optional card data
typedef struct {
    const short int ID; // Player ID
    int betAmount; // bet amount
    CommandType command;  // Command type
    Card_struct card;     // Card (optional)
} CommandPacket;

// Function prototypes
int test_fun();
void processCommand(CommandPacket packet);
int initialise_transciever(int tx_pin, int rx_pin);
void hc12_send(CommandType cmd, Card_struct card = {'X', -1, 'X'});
void hc12_receive();

#endif
