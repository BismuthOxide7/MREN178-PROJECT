#include <SoftwareSerial.h>
#include "wireless.h"
#include "cards.h"
#include "actions.h"
#include "buttons.h"
#include <LiquidCrystal.h>
#include "display.h"
#include <Arduino.h>

// Global variables
extern LiquidCrystal lcd;
static uint8_t menuIndex = 0;
static const char* const menuItems[] = {"Hit", "Stay", "Fold", "Bet", "View Hand"};
static bool menuActive = false;

void setupButtons() {
    pinMode(BTN_PIN, INPUT);
    updateMenu();
}

void updateMenu() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(">");
    lcd.print(menuItems[menuIndex]);
    
    // Display next menu item if available
    if (menuIndex < 4) {
        lcd.setCursor(0, 1);
        lcd.print(" ");
        lcd.print(menuItems[menuIndex + 1]);
    } else {
        lcd.setCursor(0, 1);
        lcd.print(" ");
        lcd.print(menuItems[0]);
    }
}

void showActionResult(const char* action, bool success) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(action);
    lcd.setCursor(0, 1);
    lcd.print(success ? "Success!" : "Failed!");
    delay(1000);
    updateMenu();
}

void checkButtons() {
    int buttonValue = analogRead(BTN_PIN);
    static unsigned long lastButtonPress = 0;
    const unsigned long DEBOUNCE_DELAY = 200;

    if (millis() - lastButtonPress < DEBOUNCE_DELAY) {
        return;
    }

    if (buttonValue < 100) {  // RIGHT button
        menuIndex = (menuIndex + 1) % 5;
        updateMenu();
        lastButtonPress = millis();
    }
    else if (buttonValue < 200) {  // UP button
        menuIndex = (menuIndex + 4) % 5;
        updateMenu();
        lastButtonPress = millis();
    }
    else if (buttonValue < 400) {  // DOWN button
        menuIndex = (menuIndex + 1) % 5;
        updateMenu();
        lastButtonPress = millis();
    }
    else if (buttonValue < 600) {  // LEFT button
        menuIndex = (menuIndex + 4) % 5;
        updateMenu();
        lastButtonPress = millis();
    }
    else if (buttonValue < 800) {  // SELECT button
        handleMenuSelection();
        lastButtonPress = millis();
    }
}

void handleMenuSelection(void) {
    CommandPacket packet;
    packet.playerID = deviceID;  // Using consistent deviceID
    packet.betAmount = 0;
    packet.card = make_card('0', 0, '0');  // Dummy card

    switch (menuIndex) {
        case 0:  // Hit
            packet.command = CMD_HIT;
            break;
        case 1:  // Stay
            packet.command = CMD_STAY;
            break;
        case 2:  // Fold
            packet.command = CMD_FOLD;
            break;
        case 3:  // Bet
            packet.command = CMD_BET;
            packet.betAmount = 10;
            break;
        case 4:  // View Hand
            showPlayerHand();
            return;
    }

    hc12_send(packet);
    showActionResult(menuItems[menuIndex], true);
}

void showPlayerHand() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Your hand:");
    lcd.setCursor(0, 1);
    
    Node* current = player_cards;
    while (current != NULL) {
        lcd.print(current->card.friendlyName);
        lcd.print(current->card.suit);
        lcd.print(" ");
        current = current->next;
    }
    
    delay(2000);
    updateMenu();
}
