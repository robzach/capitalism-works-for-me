/*
   Three 7-segment digit driver for Steve Lambert's "Capitalism Works for Me"

   see <https://github.com/robzach/capitalism-works-for-me> for further information, including schematic

   sketch by Robert Zacharias, rz@rzach.me, 2018
*/

//receive software serial data (TX pin 9, RX pin 8 on Arduino Nano)
#include <AltSoftSerial.h>
AltSoftSerial altSerial;

// declare output pins to drive display--SPI?

// tie this pin to gnd to mark this as the "FALSE" side, or
// leave this pin floating to mark this as the "TRUE" side
const int SIDEPIN = A5;
bool TRUESIDE; // this bool will be set by wiring in the setup()

// SPI pins
const int SCK = 13;
const int MISO = 12;
const int MOSI = 11;

const int RS485RECEIVEENABLEPIN = 7; // RS485 transceiver: pull this pin low to receive

void setup() {
  Serial.begin(9600); // hardware serial for local USB debugging
  altSerial.begin(9600); // software serial for receiving remote signal

  pinMode(SIDEPIN, INPUT_PULLUP);
  TRUESIDE = digitalRead(SIDEPIN);

  pinMode(RS485RECEIVEENABLEPIN, OUTPUT);
  digitalWrite(RS485RECEIVEENABLEPIN, LOW);

  // set up pin modes for SPI(?) output
}

void loop() {
  static int inVal = 0;

  // listen for incoming instructions
  // format: "t123\nf456\n" means the true count is 123 and the false count is 456
  if (altSerial.available() > 0) {
    if (TRUESIDE && altSerial.peek() == 't') {
      inVal = altSerial.parseInt();
      Serial.print("true value received: ");
      Serial.println(inVal);
    }
    else if (!TRUESIDE && altSerial.peek() == 'f') {
      inVal = altSerial.parseInt();
      Serial.print("false value received: ");
      Serial.println(inVal);
    }
    else altSerial.read(); // discard any data that arrived mangled
  }

  // print the value on the local 7-segment display
  ledDisplay(inVal);
}

void ledDisplay(int num) {
  // write this function!
}

