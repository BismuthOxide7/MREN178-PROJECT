#include "wireless.h"
#include "cards.h"
#include "actions.h"
#include <LiquidCrystal.h>


void checkButtons() {
    int buttonValue = analogRead(BTN_PIN);

    if (buttonValue < 200) { // UP - Move menu up
        menuIndex--;
        if (menuIndex < 0) menuIndex = 4;
        updateMenu();
        delay(300);
    } else if (buttonValue < 400) { // DOWN - Move menu down
        menuIndex++;
        if (menuIndex > 4) menuIndex = 0;
        updateMenu();
        delay(300);
    } else if (buttonValue < 800) { // SELECT - Choose menu option
        Serial.print("Selected: ");
        Serial.println(menuItems[menuIndex]);

        // Show the selected action on the LCD
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Selected:");
        lcd.setCursor(0, 1);
        lcd.print(menuItems[menuIndex]);
        delay(1000);  // Show selection for 1 second

        // Perform action based on selection
        switch (menuIndex) {
            case 0:  // Hit
                Serial.println("Player chose HIT!");
                hc12_send(CMD_HIT);
                break;
            case 1:  // Stay
                Serial.println("Player chose STAY!");
                hc12_send(CMD_STAY);
                break;
            case 2:  // Fold
                Serial.println("Player chose FOLD!");
                hc12_send(CMD_FOLD);
                break;
            case 3:  // Bet
                Serial.println("Player chose BET!");
                hc12_send(CMD_BET);
                break;
            case 4:  // View Hand
                Serial.println("Player chose VIEW HAND!");
                hc12_send(CMD_RECEIVE_THIS_CARD);
                break;
        }

        // After the action is processed, return to the menu
        updateMenu();
    }
}

