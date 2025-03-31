#include <SoftwareSerial.h>
#include "wireless.h"
#include "cards.h"
#include "actions.h"
#include "buttons.h"
#include <LiquidCrystal.h>
#include "display.h"
#include <Arduino.h>

extern player This_Player_Struct;

extern int ID;

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
    static bool actionTaken = false;

    // Handle navigation buttons
    if (!actionTaken) {
        if ((buttonValue <= BTN_UP + 100 && buttonValue >= BTN_UP - 100) || 
            (buttonValue <= BTN_RIGHT + 100 && buttonValue >= BTN_RIGHT - 100)) {
            menuIndex--;
            if (menuIndex < 0) menuIndex = 3;
            updateMenu();
            delay(300);
        } 
        else if ((buttonValue <= BTN_DOWN + 100 && buttonValue >= BTN_DOWN - 100) || 
                 (buttonValue <= BTN_LEFT + 100 && buttonValue >= BTN_LEFT - 100)) {
            menuIndex++;
            if (menuIndex > 3) menuIndex = 0;
            updateMenu();
            delay(300);
        } 
        else if (buttonValue <= BTN_SELECT + 100 && buttonValue >= BTN_SELECT - 100) {
            // Only process select button if no action has been taken
            switch (menuIndex) {
                case 0:  // View Hands
                    Show_Hands();
                    delay(2000);  // Show hands for 2 seconds
                    actionTaken = false;  // Allow returning to menu
                    updateMenu();
                    break;
                    
                case 1:  // Hit
                    if(ID == 0) {
                        hit(*This_Player_Struct);
                    } else {
                        CommandPacket packet;
                        packet.command = CMD_HIT;
                        packet.ID = ID;
                        hc12_send(packet);
                    }
                    actionTaken = true;
                    break;
                    
                case 2:  // Stay
                    if(ID == 0) {
                        //stand(This_Player_Struct);
                    } else {
                        CommandPacket packet;
                        packet.command = CMD_STAY;
                        packet.ID = ID;
                        hc12_send(packet);
                    }
                    actionTaken = true;
                    break;
                    
                case 3:  // Fold
                    if(ID == 0) {
                        //fold(This_Player_Struct);
                    } else {
                        CommandPacket packet;
                        packet.command = CMD_FOLD;
                        packet.ID = ID;
                        hc12_send(packet);
                    }
                    actionTaken = true;
                    break;
            }
            delay(300);
        }
    }
}

void updateMenu(){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Menu: ");
    lcd.setCursor(0, 1);
    lcd.print(menuItems[menuIndex]);
}

// Add this function to reset the menu state
void resetMenuState() {
    menuIndex = 0;
    updateMenu();
}