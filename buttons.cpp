#include "buttons.h"

// LCD Pin Configuration (based on your wiring table)
const int pin_RS = 8;
const int pin_EN = 9;
const int pin_d4 = 4;
const int pin_d5 = 5;
const int pin_d6 = 6;
const int pin_d7 = 7;

// Initialize LCD
LiquidCrystal lcd(pin_RS, pin_EN, pin_d4, pin_d5, pin_d6, pin_d7);

// Menu options
const char* menuItems[] = {"Hit", "Stay", "Fold", "Bet", "View Hand"};
int menuIndex = 0; // Tracks the currently selected menu item

void setupButtons() {
    pinMode(BTN_PIN, INPUT);
    lcd.begin(16, 2);
    updateMenu();
}

void checkButtons() {
    int buttonValue = analogRead(BTN_PIN);

    if (buttonValue < 50) { // Right (Unused)
        Serial.println("Right button pressed!");
    } else if (buttonValue < 200) { // UP - Move menu up
        menuIndex--;
        if (menuIndex < 0) menuIndex = 4; // Wrap around to bottom option
        updateMenu();
        delay(300);
    } else if (buttonValue < 400) { // DOWN - Move menu down
        menuIndex++;
        if (menuIndex > 4) menuIndex = 0; // Wrap around to top option
        updateMenu();
        delay(300);
    } else if (buttonValue < 600) { // LEFT (Unused)
        Serial.println("Left button pressed!");
    } else if (buttonValue < 800) { // SELECT - Choose menu option
        Serial.print("Selected: ");
        Serial.println(menuItems[menuIndex]);

        // Simulate the action for now
        lcd.clear();
        lcd.print("Selected:");
        lcd.setCursor(0, 1);
        lcd.print(menuItems[menuIndex]);
        delay(1000); // Show selection for 1 second
        updateMenu(); // Return to menu
    }
}

void updateMenu() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Choose Action:");
    lcd.setCursor(0, 1);
    lcd.print(menuItems[menuIndex]); // Show the selected menu item
}

