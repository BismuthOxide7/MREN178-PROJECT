#include <SoftwareSerial.h>
#include "wireless.h"
#include "cards.h"
#include "actions.h"
#include "buttons.h"
#include <LiquidCrystal.h>
#include "display.h"
#include <Arduino.h>





// Add at the top with other global variables
const char* menuItems[] = {
    "View Hands",
    "Hit",
    "Stay", 
    "Fold"
};

int menuIndex = 0;  // Current menu selection

void checkButtons() {
    int buttonValue = analogRead(BTN_PIN);

    if (buttonValue == BTN_UP ||  buttonValue == BTN_RIGHT) { // UP - Move menu up
        menuIndex--;
        if (menuIndex < 0) menuIndex = 4;
        updateMenu();
        delay(300);
    } else if (buttonValue == BTN_DOWN || buttonValue == BTN_LEFT){ // DOWN - Move menu down
        menuIndex++;
        if (menuIndex > 4) menuIndex = 0;
        updateMenu();
        delay(300);
    } else if (buttonValue == BTN_SELECT) { // SELECT - Choose menu option
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
                //Send a command to hit
                break;
            case 1:  // Stay
                Serial.println("Player chose STAY!");
                //Send a command to stay
                break;
            case 2:  // Fold
                Serial.println("Player chose FOLD!");
                //Send a command to fold
                break;
            case 3:  // Bet
                //send a command with a bet
                break;
            case 4:  // View Hand
                //View the player and dealer's hands
                while(!Show_Hands());
                break;
        }

        // After the action is processed, return to the menu
        updateMenu();
    }
}

void updateMenu(){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Menu: ");
    lcd.setCursor(0, 1);
    lcd.print(menuItems[menuIndex]);
}