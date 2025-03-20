#include "wireless.h"
#include "cards.h"
#include "buttons.h"

void setup() {
    Serial.begin(9600);   // Start Serial Monitor
    setupButtons();       // Initialize buttons
    initialise_transciever(); // Initialize HC-12 wireless module
}

void loop() {
    checkButtons(); // Check if a button is pressed and take action
}
