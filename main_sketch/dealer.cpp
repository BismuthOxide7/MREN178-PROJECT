#include "dealer.h"
#include "wireless.h"
#include "cards.h"
#include <Arduino.h>

// Global variables
GameState gameState = GAME_WAITING;
uint8_t currentPlayer = 0;
Deck_struct gameDeck;  // Declare the game deck
bool isDealer = false; // Define isDealer and initialize it

static unsigned long lastBackgroundCheck = 0;
const unsigned long BACKGROUND_INTERVAL = 100; // 100ms between checks
const unsigned long RESPONSE_TIMEOUT = 3000;

GameState currentGameState = GAME_WAITING;
extern player* players[4];
byte currentPlayerIndex = 0;

void dealer_init(void) {
    // Initialize players
    for (int i = 0; i < 4; i++) {
        players[i]->playerNumber = i;
        players[i]->totalBet = 0;
        players[i]->hasResponded = false;
        players[i]->timeout = 0;
        players[i]->outOfGame = false;
    }

    gameState = GAME_WAITING;
    initialize_deck(&gameDeck);  // Initialize the game deck
    shuffle_deck(&gameDeck);

    currentGameState = GAME_DEALING;
}

void dealer_manage_game() {
    switch (currentGameState) {
        case GAME_WAITING:
            dealer_init();
            break;

        case GAME_DEALING:
            dealer_deal_initial_cards();
            currentGameState = GAME_PLAYING;
            break;

        case GAME_PLAYING:
            dealer_check_timeout();
            if (players[currentPlayerIndex]->hasResponded || 
                players[currentPlayerIndex]->timeout < millis()) {
                dealer_next_player();
            }
            break;

        case GAME_ENDING:
            dealer_calculate_winners();
            break;
    }
}

void dealer_handle_command(CommandPacket packet) {
    player* currentPlayer = players[packet.playerID];
    if (!currentPlayer) return;

    currentPlayer->hasResponded = true;

    switch (packet.command) {
        case CMD_HIT:
            hit(currentPlayer, &gameDeck);
            break;
        case CMD_STAY:
            currentPlayer->outOfGame = true;
            break;
        case CMD_FOLD:
            currentPlayer->outOfGame = true;
            currentPlayer->totalBet /= 2;
            break;
        case CMD_BET:
            currentPlayer->totalBet += packet.betAmount;
            break;
    }
}

void dealer_deal_initial_cards() {
    for (int round = 0; round < 2; round++) {
        for (byte i = 0; i < 4; i++) {
            if (!players[i]->outOfGame) {
                Card_struct card = draw_card(&gameDeck);
                hc12_send_card(players[i]->playerNumber, card);
                delay(100);
            }
        }
    }
}

void dealer_next_player() {
    players[currentPlayerIndex]->hasResponded = false;

    do {
        currentPlayerIndex = (currentPlayerIndex + 1) % 4;
        if (currentPlayerIndex == 0) {
            currentGameState = GAME_ENDING;
            return;
        }
    } while (players[currentPlayerIndex]->outOfGame);

    players[currentPlayerIndex]->timeout = millis() + RESPONSE_TIMEOUT;
    hc12_send_turn(players[currentPlayerIndex]->playerNumber);
}

void dealer_check_timeout() {
    if (!players[currentPlayerIndex]->hasResponded && 
        millis() > players[currentPlayerIndex]->timeout) {
        players[currentPlayerIndex]->outOfGame = true;
        dealer_next_player();
    }
}

bool dealer_is_game_over() {
    for (byte i = 1; i < 4; i++) {
        if (!players[i]->outOfGame) {
            return false;
        }
    }
    return true;
}

void dealer_calculate_winners() {
    int dealerSum = 0;
    Node* current = dealer_cards;

    while (current != NULL) {
        dealerSum += current->card.value;
        current = current->next;
    }
    dealerSum += hidden_dealer_card.value;

    for (byte i = 1; i < 4; i++) {
        if (players[i]->outOfGame) continue;

        int playerSum = 0;
        current = players[i]->head;

        while (current != NULL) {
            playerSum += current->card.value;
            current = current->next;
        }

        CommandPacket result;
        result.playerID = players[i]->playerNumber;

        if (playerSum > 21) {
            result.command = CMD_LOSE;
        } else if (dealerSum > 21 || playerSum > dealerSum) {
            result.command = CMD_WIN;
            result.betAmount = players[i]->totalBet * 2;
        } else if (playerSum == dealerSum) {
            result.command = CMD_DRAW;
            result.betAmount = players[i]->totalBet;
        } else {
            result.command = CMD_LOSE;
        }

        hc12_send(result);
    }
}

void dealer_play_hand() {
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
        case GAME_WAITING:
            break;

        case GAME_DEALING:
            dealer_deal_initial_cards();
            gameState = GAME_PLAYING;
            break;

        case GAME_PLAYING:
            break;

        case GAME_ENDING:
            start_new_game();
            break;
    }
}

void start_new_game(void) {
    if (!isDealer) return;

    shuffle_deck(&gameDeck);
    currentPlayerIndex = 1;
    gameState = GAME_DEALING;
}