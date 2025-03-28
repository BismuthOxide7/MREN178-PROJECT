#ifndef _DEALER_H
#define _DEALER_H

#include <Arduino.h>
#include "cards.h"
#include "wireless.h"
#include "display.h"

typedef enum {
    GAME_WAITING,    // Changed from WAITING_FOR_PLAYERS
    GAME_DEALING,    // Changed from DEALING_CARDS
    GAME_PLAYING,    // Changed from GAMEPLAY
    GAME_ENDING      // Changed from GAME_END
} GameState;

// Player status structure
typedef struct {
    uint8_t deviceID;        // Changed from playerID
    bool isActive;
    int totalBet;
    bool hasResponded;
    unsigned long timeout;
} PlayerStatus;

// External declarations
extern GameState gameState;
extern PlayerStatus playerStatus[4];  // Changed from playerQueue
extern uint8_t currentPlayer;         // Changed from currentPlayerIndex

// Function prototypes
void dealer_init(void);
void dealer_background_task(void);
void dealer_handle_command(CommandPacket packet);
void start_new_game(void);

#endif