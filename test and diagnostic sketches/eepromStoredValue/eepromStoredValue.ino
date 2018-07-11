#include <EEPROM.h>

const int BUTTONPIN = 3;
int eAddress = 10;
int storedValue = EEPROM.get(eAddress, storedValue);


void setup() {
  pinMode(BUTTONPIN, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.print("last stored EEPROM unsigned int value: ");
  Serial.println(storedValue);
}

void loop() {
  if (!digitalRead(BUTTONPIN)) {
    Serial.println(storedValue);
    EEPROM.put(eAddress, storedValue);
    storedValue++;
  }

}
