#include "dealer.h"
#include "wireless.h"
#include "cards.h"
#include <Arduino.h>

// Global variables
GameState gameState = GAME_WAITING;
PlayerStatus playerStatus[4] = {0};  // Initialize all players
uint8_t currentPlayer = 0;
Deck_struct gameDeck;

static byte currentPlayer = 0;
static unsigned long lastBackgroundCheck = 0;
const unsigned long BACKGROUND_INTERVAL = 100; // 100ms between checks

GameState currentGameState = GAME_INIT;
PlayerStatus playerQueue[4];
byte currentPlayerIndex = 0;

// Response timeout (3 seconds)
const unsigned long RESPONSE_TIMEOUT = 3000;

void dealer_init(void) {
    // Initialize player status
    for(int i = 0; i < 4; i++) {
        playerStatus[i].deviceID = i;
        playerStatus[i].isActive = true;
        playerStatus[i].totalBet = 0;
        playerStatus[i].hasResponded = false;
        playerStatus[i].timeout = 0;
    }
    
    gameState = GAME_WAITING;
    initialize_deck(&gameDeck);
    shuffle_deck(&gameDeck);

    // Initialize player queue
    for (byte i = 0; i < 4; i++) {
        playerQueue[i].playerID = i + 1;
        playerQueue[i].isActive = true;
        playerQueue[i].totalBet = 0;
        playerQueue[i].hasResponded = false;
        playerQueue[i].timeout = 0;
    }

    currentGameState = DEALING_CARDS;
}

void dealer_manage_game() {
    switch (currentGameState) {
        case GAME_INIT:
            dealer_init();
            break;

        case DEALING_CARDS:
            dealer_deal_initial_cards();
            currentGameState = PLAYER_TURN;
            break;

        case PLAYER_TURN:
            // Check for timeouts
            dealer_check_timeout();
            
            // If current player has responded or timed out
            if (playerQueue[currentPlayerIndex].hasResponded || 
                playerQueue[currentPlayerIndex].timeout < millis()) {
                dealer_next_player();
            }
            break;

        case DEALER_TURN:
            // Dealer's automated play
            dealer_play_hand();
            if (dealer_is_game_over()) {
                currentGameState = GAME_END;
            }
            break;

        case GAME_END:
            dealer_calculate_winners();
            currentGameState = GAME_INIT;  // Ready for next game
            break;
    }
}

void dealer_handle_response(CommandPacket packet) {
    // Find player in queue
    for (byte i = 0; i < 4; i++) {
        if (playerQueue[i].playerID == packet.playerID) {
            playerQueue[i].hasResponded = true;
            
            // Process command
            switch (packet.command) {
                case CMD_HIT:
                    // Deal card to player
                    Card_struct card = draw_card(&gameDeck);
                    hc12_send_card(packet.playerID, card);
                    break;

                case CMD_STAY:
                    playerQueue[i].isActive = false;
                    dealer_next_player();
                    break;

                case CMD_FOLD:
                    playerQueue[i].isActive = false;
                    playerQueue[i].totalBet /= 2;
                    dealer_next_player();
                    break;

                case CMD_BET:
                    playerQueue[i].totalBet += packet.betAmount;
                    break;
            }
            break;
        }
    }
}

void dealer_deal_initial_cards() {
    // Deal two cards to each active player
    for (int round = 0; round < 2; round++) {
        for (byte i = 0; i < 4; i++) {
            if (playerQueue[i].isActive) {
                Card_struct card = draw_card(&gameDeck);
                hc12_send_card(playerQueue[i].playerID, card);
                delay(100);  // Small delay between deals
            }
        }
    }
}

void dealer_next_player() {
    // Reset current player's response status
    playerQueue[currentPlayerIndex].hasResponded = false;
    
    // Find next active player
    do {
        currentPlayerIndex = (currentPlayerIndex + 1) % 4;
        if (currentPlayerIndex == 0) {  // Back to dealer
            currentGameState = DEALER_TURN;
            return;
        }
    } while (!playerQueue[currentPlayerIndex].isActive);

    // Set timeout for new player
    playerQueue[currentPlayerIndex].timeout = millis() + RESPONSE_TIMEOUT;
    
    // Notify new player it's their turn
    hc12_send_turn(playerQueue[currentPlayerIndex].playerID);
}

void dealer_check_timeout() {
    // Check if current player has timed out
    if (!playerQueue[currentPlayerIndex].hasResponded && 
        millis() > playerQueue[currentPlayerIndex].timeout) {
        // Force "STAY" on timeout
        playerQueue[currentPlayerIndex].isActive = false;
        dealer_next_player();
    }
}

bool dealer_is_game_over() {
    // Check if all players are inactive or have stayed
    for (byte i = 1; i < 4; i++) {  // Skip dealer (index 0)
        if (playerQueue[i].isActive) {
            return false;
        }
    }
    return true;
}

void dealer_calculate_winners() {
    int dealerSum = 0;
    Node* current = dealer_cards;
    
    // Calculate dealer's hand value
    while (current != NULL) {
        dealerSum += current->card.value;
        current = current->next;
    }
    dealerSum += hidden_dealer_card.value;

    // Compare with each player
    for (byte i = 1; i < 4; i++) {
        if (!playerQueue[i].isActive) continue;

        int playerSum = 0;
        current = playerQueue[i].head;
        
        while (current != NULL) {
            playerSum += current->card.value;
            current = current->next;
        }

        CommandPacket result;
        result.ID = playerQueue[i].playerID;
        
        if (playerSum > 21) {
            result.command = CMD_LOSE;
        } else if (dealerSum > 21 || playerSum > dealerSum) {
            result.command = CMD_WIN;
            result.betAmount = playerQueue[i].totalBet * 2;
        } else if (playerSum == dealerSum) {
            result.command = CMD_DRAW;
            result.betAmount = playerQueue[i].totalBet;
        } else {
            result.command = CMD_LOSE;
        }

        hc12_send(result);
    }
}

void dealer_play_hand() {
    // Dealer must hit on 16 or below, stand on 17 or above
    int dealerSum = 0;
    Node* current = dealer_cards;
    
    while (current != NULL) {
        dealerSum += current->card.value;
        current = current->next;
    }
    dealerSum += hidden_dealer_card.value;

    while (dealerSum < 17) {
        Card_struct newCard = draw_card(&gameDeck);
        add_visible_dealer_card(&newCard);
        dealerSum += newCard.value;
    }
}

void dealer_background_task(void) {
    if (!isDealer || millis() - lastBackgroundCheck < BACKGROUND_INTERVAL) {
        return;
    }
    lastBackgroundCheck = millis();

    switch (gameState) {
        case WAITING_FOR_PLAYERS:
            // Wait for player ready signals
            break;

        case DEALING_CARDS:
            // Deal initial cards to all players
            for (byte i = 0; i < 4; i++) {
                if (i != 0) { // Skip dealer's automated cards
                    Card_struct card = draw_card(&gameDeck);
                    hc12_send_card(i, card);
                    delay(100);
                }
            }
            gameState = GAMEPLAY;
            break;

        case GAMEPLAY:
            // Game is handled by normal player actions
            break;

        case GAME_END:
            // Handle game end, calculate winners
            start_new_game();
            break;
    }
}

void dealer_handle_command(byte command, byte fromPlayer) {
    if (!isDealer) return;

    switch (command) {
        case CMD_HIT:
            Card_struct card = draw_card(&gameDeck);
            hc12_send_card(fromPlayer, card);
            break;

        case CMD_STAY:
        case CMD_FOLD:
            // Move to next player
            currentPlayer = (currentPlayer + 1) % 4;
            if (currentPlayer == 0) {
                gameState = GAME_END;
            }
            break;
    }
}

void start_new_game(void) {
    if (!isDealer) return;
    
    shuffle_deck(&gameDeck);
    currentPlayer = 1; // Start with first player after dealer
    gameState = DEALING_CARDS;
}