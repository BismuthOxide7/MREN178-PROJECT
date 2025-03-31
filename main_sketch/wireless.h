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
    CommandType command;  // Command type
    int betAmount;      // Bet amount (optional)
    Card_struct card;     // Card (optional)
    int bet;              // Bet amount (optional)
} CommandPacket;

// Function prototypes
int test_fun();
bool startUp(int playerNumber);
void processCommand(CommandPacket packet);
int initialise_transciever();
void hc12_send(CommandType cmd, Card_struct card = {'X', -1, 'X'});
void hc12_receive(int mode = 0);

#endif
