#ifndef _WIRELESS_H
#define _WIRELESS_H

#include <Arduino.h>
#include "cards.h"
#include <SoftwareSerial.h>

// Global player ID
extern uint8_t deviceID;  // Changed from PLAYER_ID for consistency

// Command definitions
typedef enum {
    CMD_NONE = 0x00,
    CMD_PING = 0x01,
    CMD_ACK = 0x02,
    CMD_HIT = 0x03,
    CMD_STAY = 0x04,
    CMD_FOLD = 0x05,
    CMD_BET = 0x06,
    CMD_RECEIVE_THIS_CARD = 0x07,
    CMD_PLAYER_TURN = 0x08,
    CMD_WIN = 0x09,
    CMD_LOSE = 0x0A,
    CMD_DRAW = 0x0B,
    CMD_DOUBLE_DOWN = 0x0C
} CommandType;

typedef struct {
    byte playerID;
    CommandType command;
    int betAmount;
    Card_struct card;
} CommandPacket;

// External declarations
extern SoftwareSerial HC12;

// Function prototypes
void hc12_init(void);
CommandPacket hc12_receive(void);
void hc12_send(CommandPacket packet);
void hc12_send_card(byte playerID, Card_struct card);
void hc12_send_turn(byte playerID);

#endif
