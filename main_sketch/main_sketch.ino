#include "wireless.h"
#include "cards.h"
#include <EEPROM.h>

int ID;

void setup() {
  Serial.begin(9600); // Serial port to computer
  EEPROM.get(0, ID); // Read the ID from EEPROM

}

void loop() {

  delay(500);
}
