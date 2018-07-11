#include <SPI.h>

const int VCCPIN = A2; // using A2 as 7-seg display's VCC
const int GNDPIN = A3; // using A3 as 7-seg display's GND

const unsigned long WAIT = 1000;
unsigned long timer;

bool readyForNextVote;

void setup() {
  pinMode(VCCPIN, OUTPUT);
  digitalWrite(VCCPIN, HIGH);
  pinMode(GNDPIN, OUTPUT);
  digitalWrite(GNDPIN, LOW);
  SPI.begin();
}

void loop() {
  static int seconds = 15;
  if (millis() - timer >= WAIT) {
    timer = millis();
    SPI.beginTransaction(SPISettings(250000, MSBFIRST, SPI_MODE0));

    if (seconds % 2 == 0){
      SPI.transfer(0x77); SPI.transfer(0b10000); // blink colon during even seconds
    }
    else {
      SPI.transfer(0x77); SPI.transfer(0); // turn off colon during odd seconds
    }
    
    // send two blanks, then the two-digit value
    SPI.transfer(0x78); // blank
    SPI.transfer(0x78); // blank
    SPI.transfer(seconds / 10); // tens digit
    SPI.transfer(seconds % 10); // ones digit

    if (seconds != 0) {
      seconds--;
      readyForNextVote = false;
    }
    else readyForNextVote = true;

    SPI.endTransaction();
  }
}

