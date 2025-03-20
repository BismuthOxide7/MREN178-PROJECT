#define ID 0 // Player ID 0 to 3 valid
#include <EEPROM.h>

void setup() {
    // Write the ID to EEPROM
    EEPROM.update(0, ID);    
    // turn on the LED to indicate success
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
}