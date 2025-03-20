#include "buttons.h"

void setupButtons() {
    pinMode(BTN_HIT, INPUT_PULLUP);
    pinMode(BTN_STAY, INPUT_PULLUP);
    pinMode(BTN_FOLD, INPUT_PULLUP);
    pinMode(BTN_BET, INPUT_PULLUP);
    pinMode(BTN_VIEW, INPUT_PULLUP);
}

void checkButtons() {
    if (digitalRead(BTN_HIT) == LOW) {
        Serial.println("Hit button pressed!");
        hc12_send(CMD_HIT);
        delay(300);
    }

    if (digitalRead(BTN_STAY) == LOW) {
        Serial.println("Stay button pressed!");
        hc12_send(CMD_STAY);
        delay(300);
    }

    if (digitalRead(BTN_FOLD) == LOW) {
        Serial.println("Fold button pressed!");
        hc12_send(CMD_FOLD);
        delay(300);
    }

    if (digitalRead(BTN_BET) == LOW) {
        Serial.println("Bet button pressed!");
        hc12_send(CMD_BET);
        delay(300);
    }

    if (digitalRead(BTN_VIEW) == LOW) {
        Serial.println("View Other Hand button pressed!");
        hc12_send(CMD_RECEIVE_THIS_CARD);
        delay(300);
    }
}
