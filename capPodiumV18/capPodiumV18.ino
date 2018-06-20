/*

  Podium vote tally machine for Steve Lambert's "Capitalism Works for Me"

  see <https://github.com/robzach/capitalism-works-for-me> for further information, including schematic

  sketch by Robert Zacharias, rz@rzach.me, 2018

*/

#include <AltSoftSerial.h> // for RS485 communication
#include <EEPROM.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

AltSoftSerial altSerial;
Adafruit_7segment matrix = Adafruit_7segment();

const bool VERBOSESERIALFEEDBACK = true; // switch true or false as wanted

const int TRUEBUTTONPIN = 2; // wired to high side of true button
const int FALSEBUTTONPIN = 3; // wired to high side of true button
const int THREEEIGHTS = 4; // pushbutton to display 888 as votes for LED diagnosis
const int ZEROOUT = 5; // pushbutton to reset all votes to 0

const int SECONDSTIMER = 15; // wait between votes, in seconds
const unsigned long WAIT = 1000; // wait between clock ticks on the countdown, in milliseconds
const unsigned long RETRANSMISSIONWAIT = 15000; // wait between retransmissions in absence of new votes, in milliseconds

bool readyForNextVote;

const int EEPROMTRUEADDRESS = 0; // address to write true vote record
const int EEPROMFALSEADDRESS = EEPROMTRUEADDRESS + sizeof(unsigned int); // next available EEPROM slot
unsigned int trueVote = EEPROM.get(EEPROMTRUEADDRESS, trueVote); // retrieve last trueVote stored
unsigned int falseVote = EEPROM.get(EEPROMFALSEADDRESS, falseVote); // retrieve last falseVote stored

void setup() {
  pinMode(TRUEBUTTONPIN, INPUT_PULLUP);
  pinMode(FALSEBUTTONPIN, INPUT_PULLUP);
  pinMode(THREEEIGHTS, INPUT_PULLUP);
  pinMode(ZEROOUT, INPUT_PULLUP);

  Serial.begin(9600); // initialize Serial communication for debugging
  Serial.println("Podium vote tally machine for Steve Lambert's 'Capitalism Works for Me', firmware v.18");
  Serial.println("see <https://github.com/robzach/capitalism-works-for-me> for further information, including schematic");
  Serial.println("sketch by Robert Zacharias, rz@rzach.me, 2018\n");
  Serial.print("verbose feedback mode: "); Serial.println(VERBOSESERIALFEEDBACK);
  Serial.print("EEPROM stored true, false votes: "); Serial.print(trueVote); Serial.print(", "); Serial.println(falseVote);

  altSerial.begin(9600); // initialize RS485 communication to sign
  pinMode(7, OUTPUT); // pull pin 7 high to set RS485 as outgoing
  digitalWrite(7, HIGH);

  matrix.begin(0x70);

  // transmit diagnostic 888 to both sides for five seconds at startup
  altSerial.print("t888\nf888");
  delay(5000);

  // send votes after diagnostic 888
  transmitVote();
}

void loop() {

  static int seconds = SECONDSTIMER;
  static unsigned long timer;
  static unsigned long retransmissiontimer;

  static boolean drawDots = true;
  static boolean blinker = true;

  // once per second
  if (millis() - timer >= WAIT) {
    timer = millis();

    // write current countdown time to podium 7-segment display
    {

      if (seconds % 2 == 0) {
        drawDots = true; // blink colon on during even seconds
      }
      else {
        drawDots = false; // blink colon off during odd seconds
      }

      // will print in format "0:12", leaving first digit blank
      matrix.writeDigitNum(1, 0);
      matrix.drawColon(drawDots);
      matrix.writeDigitNum(3, seconds / 10);
      matrix.writeDigitNum(4, seconds % 10);


      if (VERBOSESERIALFEEDBACK) {
        Serial.println("seconds = " + String(seconds) + "; tens = " + String(seconds / 10) + "; ones = " + String(seconds % 10));
      }

      if (seconds != 0) {
        seconds--;
        readyForNextVote = false;
      }

      // at time = 0, blink the zeros to show it's ready
      else {
        readyForNextVote = true;
        blinker = !blinker;
        if (blinker) { // zeros and colon on
          matrix.writeDigitNum(1, 0);
          matrix.drawColon(drawDots);
          matrix.writeDigitNum(3, 0);
          matrix.writeDigitNum(4, 0);
        }
        else { // only way I could see to blank out the display (all segments off)
          matrix.writeDigitRaw(1, 0x0);
          matrix.writeDigitRaw(2, 0x0);
          matrix.writeDigitRaw(3, 0x0);
          matrix.writeDigitRaw(4, 0x0);
        }
      }
    }
    matrix.writeDisplay();
  }

  // resend votes occasionally in case prior transmission was garbled
  if (millis() - retransmissiontimer >= RETRANSMISSIONWAIT) {
    transmitVote();
    retransmissiontimer = millis();
  }

  if (readyForNextVote) {
    if (!digitalRead(TRUEBUTTONPIN)) { // if "true" button pushed
      trueVote++;
      EEPROM.put(EEPROMTRUEADDRESS, trueVote);
      readyForNextVote = false;
      seconds = SECONDSTIMER; // reset timer
      transmitVote(); // immediately transmit votes
      Serial.print("true vote recorded; current trueVote value = "); Serial.println(trueVote);
    }
    else if (!digitalRead(FALSEBUTTONPIN)) { // if "false" button pushed
      falseVote++;
      EEPROM.put(EEPROMFALSEADDRESS, falseVote);
      readyForNextVote = false;
      seconds = SECONDSTIMER; // reset timer
      transmitVote(); // immediately transmit votes
      Serial.print("false vote recorded; current falseVote value = "); Serial.println(falseVote);
    }
  }

  // set displays to 888 for segment testing. Note this does *not* affect the EEPROM memory vote count.
  // Therefore, to test digits without losing current vote totals:
  // 1. Push the 888 button
  // 2. Observe digits as wanted
  // 3. Power cycle podium
  if (!digitalRead(THREEEIGHTS)) {
    trueVote = 888;
    falseVote = 888;
    transmitVote();
    Serial.println("THREEEIGHTS button pushed");
  }

  // reset vote counts to zero, in EEPROM memory as well as on the displays
  if (!digitalRead(ZEROOUT)) {
    trueVote = 0;
    falseVote = 0;
    transmitVote();
    EEPROM.put(EEPROMTRUEADDRESS, trueVote);
    EEPROM.put(EEPROMFALSEADDRESS, falseVote);
    Serial.println("ZEROOUT button pushed");
  }
}

void transmitVote() {
  // transmit current vote counts via RS485
  // format: t123\nf456 transmits a true vote of 123 and a false vote of 456 (note the newline separating the two records)

  altSerial.print('t');
  altSerial.println(trueVote);
  altSerial.print('f');
  altSerial.println(falseVote);
}

