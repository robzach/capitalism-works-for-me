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
bool trueside; // this bool will be set in the setup() by physical wiring

// SPI pins
const int SCKpin = 13;
const int MISOpin = 12;
const int MOSIpin = 11;
const int GND = A2; // using pin A2 as a logic ground since otherwise none is broken out

const int RS485RECEIVEENABLEPIN = 7; // RS485 transceiver: pull this pin low to receive

const unsigned long WAIT = 1000; // millisecond wait between instances of pushing SPI data
unsigned long timer;

void setup() {
  Serial.begin(9600); // hardware serial for local USB debugging
  altSerial.begin(9600); // software serial for receiving remote signal

  // this pin, configured in hardware, tells the board whether it's the TRUE or FALSE side
  pinMode(SIDEPIN, INPUT_PULLUP);
  trueside = digitalRead(SIDEPIN);

  // required for receiving data on RS485
  pinMode(RS485RECEIVEENABLEPIN, OUTPUT);
  digitalWrite(RS485RECEIVEENABLEPIN, LOW);

  // SPI communication
  pinMode(SCKpin, OUTPUT);
  pinMode(MISOpin, OUTPUT);
  pinMode(MOSIpin, OUTPUT);
  pinMode(GND, OUTPUT);
  digitalWrite(GND, LOW);

  //Blank out the register
  shiftOut(MOSIpin, SCKpin, MSBFIRST, 0x00);
  digitalWrite(MISOpin, HIGH);
  digitalWrite(MISOpin, LOW);
}

void loop() {
  static int inVal = 0;

  // listen for incoming instructions
  // format: "t123\nf456\n" means the true count is 123 and the false count is 456
  if (altSerial.available() > 0) {
    if (trueside && altSerial.peek() == 't') {
      inVal = altSerial.parseInt();
      Serial.print("true value received: ");
      Serial.println(inVal);
    }
    else if (!trueside && altSerial.peek() == 'f') {
      inVal = altSerial.parseInt();
      Serial.print("false value received: ");
      Serial.println(inVal);
    }
    else altSerial.read(); // discard any data that arrived mangled
  }

  // print the value on the local 7-segment display
  //  if (millis() - timer > WAIT) ledDisplay(inVal);
  ledDisplay(inVal);
}

void ledDisplay(int num) {
  int ones = num % 10;
  int tens = (num % 100) / 10;
  int hundreds = (num % 1000) / 100;

  shiftOut(MOSIpin, SCKpin, MSBFIRST, segLookup(ones));
  shiftOut(MOSIpin, SCKpin, MSBFIRST, segLookup(tens));
  shiftOut(MOSIpin, SCKpin, MSBFIRST, segLookup(hundreds));

  digitalWrite(MISOpin, HIGH);
  digitalWrite(MISOpin, LOW);

  timer = millis();
}

byte segLookup(int digitIn) {
  // turn a numeral into a seven-segment pattern
  if (digitIn == 0) return 0x3f;
  if (digitIn == 1) return 0x06;
  if (digitIn == 2) return 0x5b;
  if (digitIn == 3) return 0x4f;
  if (digitIn == 4) return 0x66;
  if (digitIn == 5) return 0x6d;
  if (digitIn == 6) return 0x7d;
  if (digitIn == 7) return 0x07;
  if (digitIn == 8) return 0x7f;
  if (digitIn == 9) return 0x6f;
}
