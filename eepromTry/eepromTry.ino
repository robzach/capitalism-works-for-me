#include <EEPROM.h>

const int BUTTONPIN = 3;
int eepromAddress = 0;

void setup() {
  pinMode(BUTTONPIN, INPUT_PULLUP);
  Serial.begin(9600);
//  Serial.print("last stored EEPROM value: ");
//  Serial.println(EEPROM.read(eepromAddress));
  Serial.print("last stored EEPROM unsigned int value: ");
  Serial.println(eepromReadInt());
  Serial.print("eeprom address 0, 1 values = ");
  Serial.print(EEPROM.read(0));
  Serial.print(", ");
  Serial.println(EEPROM.read(1));
}

void loop() {
  static unsigned int x;
  if (!digitalRead(BUTTONPIN)){
    Serial.println(x);
//    EEPROM.write(eepromAddress, x);
    eepromWriteInt(x);
    x++;
  }
}

void eepromWriteInt(unsigned int inVal){
  EEPROM.write(eepromAddress, inVal / 256);
  EEPROM.write(eepromAddress + 1, inVal % 256);
}

unsigned int eepromReadInt(void){
  int quotient = 256 * EEPROM.read(eepromAddress);
  int remainder = EEPROM.read(eepromAddress + 1);
  return(quotient + remainder);
}

