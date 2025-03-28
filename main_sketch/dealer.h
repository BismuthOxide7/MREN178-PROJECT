#ifndef _DEALER_H
#define _DEALER_H

#include <Arduino.h>
#include "cards.h"
#include "wireless.h"
#include "display.h"
#include "actions.h"

typedef enum {
    GAME_WAITING,
    GAME_DEALING,
    GAME_PLAYING,
    GAME_ENDING
} GameState;

// External declarations
extern GameState gameState;
extern player* players[4];  // Use the player struct from actions.h
extern uint8_t currentPlayer;

// Function prototypes
void dealer_init(void);
void dealer_background_task(void);
void dealer_handle_command(CommandPacket packet);
void start_new_game(void);

#endif