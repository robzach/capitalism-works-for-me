
#include <SPI.h>

SPISettings settingsA(200000, MSBFIRST, SPI_MODE0);

const int GND = A2; // using pin A2 as a logic ground since otherwise none is broken out
const int MISOpin = 12;

const unsigned long WAIT = 1000;
unsigned long timer;
bool flipper = true;

void setup() {
  Serial.begin(9600);

  // start the SPI library:
  SPI.begin();

  pinMode(GND, OUTPUT);
  digitalWrite(GND, LOW);

  pinMode(MISOpin, OUTPUT);

}

void loop() {
  if (millis() - timer > WAIT) {
    flipper = !flipper;
    Serial.println("flipper value is " + String(flipper) + " at " + String(millis()));
    timer = millis();
  }

  SPI.beginTransaction(settingsA);
  
  if (flipper) {

    SPI.transfer(0x06);
    SPI.transfer(0x5B);
    SPI.transfer(0x4F);
  }

  else {
    SPI.transfer(0x7F);
    SPI.transfer(0x7F);
    SPI.transfer(0x7F);
  }

  SPI.endTransaction();
  digitalWrite(MISOpin, HIGH);
  digitalWrite(MISOpin, LOW);

  //  delay(10);

}


