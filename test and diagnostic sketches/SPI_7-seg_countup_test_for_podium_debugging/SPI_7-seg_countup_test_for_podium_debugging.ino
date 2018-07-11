/*
  Podium SPI communciation debugging sketch: should count up from 0. Change the value of WAIT
  to adjust the delay (in milliseconds) between incrementing

  Based on capPodiumV18, but with lots of functionality removed to focus on the display alone.
*/

#include <SPI.h> // for SPI communication
#include <AltSoftSerial.h> // for RS485 communication

AltSoftSerial altSerial;

const bool VERBOSESERIALFEEDBACK = true; // switch true or false as wanted

const int VCCPIN = A2; // using A2 as 7-seg display's VCC
const int GNDPIN = A3; // using A3 as 7-seg display's GND

const unsigned long WAIT = 100; // wait between clock ticks on the countdown, in milliseconds
unsigned long timer;

void setup() {
  pinMode(VCCPIN, OUTPUT);
  digitalWrite(VCCPIN, HIGH);
  pinMode(GNDPIN, OUTPUT);
  digitalWrite(GNDPIN, LOW);

  Serial.begin(9600); // initialize Serial communication for debugging

  altSerial.begin(9600); // initialize RS485 communication to sign
  pinMode(7, OUTPUT); // pull pin 7 high to set RS485 as outgoing
  digitalWrite(7, HIGH);

  SPI.begin();
}

void loop() {

  static int seconds = 0;

  if (millis() - timer >= WAIT) {
    timer = millis();

    // write current countdown time to podium 7-segment display
    {
      SPI.beginTransaction(SPISettings(250000, MSBFIRST, SPI_MODE0));


      SPI.transfer(0x78); // blank
      SPI.transfer(0x78); // blank
      SPI.transfer((seconds % 100) / 10); // tens digit
      SPI.transfer(seconds % 10); // ones digit

      seconds++;

      SPI.endTransaction();
    }
  }



}

