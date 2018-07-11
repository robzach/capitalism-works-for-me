/*
   Three 7-segment digit driver for Steve Lambert's "Capitalism Works for Me"

   see <https://github.com/robzach/capitalism-works-for-me> for further information, including schematic

   sketch by Robert Zacharias, rz@rzach.me, 2018
*/

//receive software serial data (TX pin 9, RX pin 8 on Arduino Nano)
#include <AltSoftSerial.h>
AltSoftSerial altSerial;

// tie this pin to gnd to mark this as the "FALSE" side, or
// leave this pin floating to mark this as the "TRUE" side
const int SIDEPIN = A5;
bool trueside; // this bool will be set in the setup() by physical wiring

// SPI pins
const int segmentClock = 13;  // SCK pin on Arduino Nano
const int segmentLatch = 12; // MISO pin on Arduino Nano
const int segmentData = 11; // MOSI pin on Arduino Nano

const int RS485RECEIVEENABLEPIN = 7; // RS485 transceiver: pull this pin low to receive

const unsigned long WAIT = 1000; // millisecond wait between instances of pushing SPI data
unsigned long timer;

void setup() {
  Serial.begin(9600); // hardware serial for local USB debugging
  altSerial.begin(9600); // software serial for receiving remote signal

  Serial.println("7-digit display module for Steve Lambert's 'Capitalism Works for Me', firmware v.18");
  Serial.println("see <https://github.com/robzach/capitalism-works-for-me> for further information, including schematic");
  Serial.println("sketch by Robert Zacharias, rz@rzach.me, 2018\n");

  // this pin, configured in hardware, tells the board whether it's the TRUE or FALSE side
  pinMode(SIDEPIN, INPUT_PULLUP);
  trueside = digitalRead(SIDEPIN);

  // required for receiving data on RS485
  pinMode(RS485RECEIVEENABLEPIN, OUTPUT);
  digitalWrite(RS485RECEIVEENABLEPIN, LOW);

  // SPI communication
  pinMode(segmentClock, OUTPUT);
  pinMode(segmentLatch, OUTPUT);
  pinMode(segmentData, OUTPUT);

  digitalWrite(segmentClock, LOW);
  digitalWrite(segmentData, LOW);
  digitalWrite(segmentLatch, HIGH);

  // display 888 for 5 seconds at startup as a diagnostic
  ledDisplay(888);
  delay(5000);
}

void loop() {
  static int inVal = 0;

  // listen for incoming instructions
  // format: "t123\nf456\n" means the true count is 123 and the false count is 456
  if (altSerial.available() > 0) {
    if (trueside && altSerial.peek() == 't') {
      inVal = altSerial.parseInt();
      ledDisplay(inVal);
      Serial.print("true value received: ");
      Serial.println(inVal);
    }
    else if (!trueside && altSerial.peek() == 'f') {
      inVal = altSerial.parseInt();
      ledDisplay(inVal);
      Serial.print("false value received: ");
      Serial.println(inVal);
    }
    else altSerial.read(); // discard any data that arrived mangled
  }

  // print the value on the local 7-segment display
  //    if (millis() - timer > WAIT) ledDisplay(inVal);
  //  ledDisplay(inVal);
}

void ledDisplay(int num) {
  int ones = num % 10;
  int tens = (num % 100) / 10;
  int hundreds = (num % 1000) / 100;

  postNumber(segLookup(hundreds));
  postNumber(segLookup(tens));
  postNumber(segLookup(ones));
}

void postNumber(byte segs)
{

  //  Clock these bits out to the drivers
  for (byte x = 0 ; x < 8 ; x++)
  {
    digitalWrite(segmentClock, LOW);
        digitalWrite(segmentLatch, LOW);
    digitalWrite(segmentData, segs & 1 << (7 - x));
    digitalWrite(segmentClock, HIGH); //Data transfers to the register on the rising edge of SRCK
        digitalWrite(segmentLatch, HIGH);
  }

}

byte segLookup(int digitIn) {

#define a  1<<1
#define b  1<<7
#define c  1<<6
#define d  1<<5
#define e  1<<4
#define f  1<<2
#define g  1<<3
#define dp 1<<8

  byte segments;

  if (digitIn == 1) segments = b | c;
  if (digitIn == 2) segments = a | b | d | e | g;
  if (digitIn == 3) segments = a | b | c | d | g;
  if (digitIn == 4) segments = f | g | b | c;
  if (digitIn == 5) segments = a | f | g | c | d;
  if (digitIn == 6) segments = a | f | g | e | c | d;
  if (digitIn == 7) segments = a | b | c;
  if (digitIn == 8) segments = a | b | c | d | e | f | g;
  if (digitIn == 9) segments = a | b | c | d | f | g;
  if (digitIn == 0) segments = a | b | c | d | e | f;

  return segments;
}
