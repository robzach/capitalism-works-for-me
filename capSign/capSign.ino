#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h>

// struct to store vote counts
struct vote {
  int truevotes, falsevotes;
} votes = { 0, 0 }; // set to zeros by default

//number comm setup
const int sevenSegLatchPinTrue = 22;
const int sevenSegClockPinTrue = 24;
const int sevenSegDataPinTrue = 26;
const int sevenSegLatchPinFalse = 28;
const int sevenSegClockPinFalse = 30;
const int sevenSegDataPinFalse = 32;

//buttons
const int BUTTON_A_PIN = 42;
const int BUTTON_B_PIN = 43;
const int BUTTON_C_PIN = 44;

//timing
unsigned long previousMillis = 0;
const unsigned long MARQUEE_WAIT = 150; // milliseconds between marquee animation steps

//rs485
const int rs485RsPin = 5;

// software serial
const int SW_SERIAL_RX = 2;
const int SW_SERIAL_TX = 3;
SoftwareSerial swSerial(SW_SERIAL_RX, SW_SERIAL_TX);

//relay
const int RELAY_A_PIN = 6;
const int RELAY_B_PIN = 7;
const int RELAY_C_PIN = 8;
const int RELAY_D_PIN = 9;
static int currentPattern = 0;
static int cuttentProgram = 0;
static boolean relaySwitchFlag = false;

//Babcd
byte patternAllOn[1] = {
  B1111
};
byte patternA[352] = {
  B1111, B0111, B0011, B0001, B0000, B1000, B1100, B1110,
  B1111, B0111, B0011, B0001, B0000, B1000, B1100, B1110,
  B1111, B0111, B0011, B0001, B0000, B1000, B1100, B1110,
  B1111, B0111, B0011, B0001, B0000, B1000, B1100, B1110,
  B1111, B0111, B0011, B0001, B0000, B1000, B1100, B1110,
  B1111, B0111, B0011, B0001, B0000, B1000, B1100, B1110,
  B1111, B0111, B0011, B0001, B0000, B1000, B1100, B1110,
  B1111, B0111, B0011, B0001, B0000, B1000, B1100, B1110,
  B1111, B0111, B0011, B0001, B0000, B1000, B1100, B1110,
  B1111, B0111, B0011, B0001, B0000, B1000, B1100, B1110,

  B1111, B1111, B0000, B1111, B1111, B0000, B1111, B1111,
  B1111, B1111, B0000, B1111, B1111, B0000, B1111, B1111,
  B1111, B1111, B0000, B1111, B1111, B0000, B1111, B1111,
  B1111, B1111, B0000, B1111, B1111, B0000, B1111, B1111,

  B1000, B0100, B0010, B0001, B1000, B0100, B0010, B0001,
  B1000, B0100, B0010, B0001, B1000, B0100, B0010, B0001,
  B1000, B0100, B0010, B0001, B1000, B0100, B0010, B0001,
  B1000, B0100, B0010, B0001, B1000, B0100, B0010, B0001,
  B1000, B0100, B0010, B0001, B1000, B0100, B0010, B0001,
  B1000, B0100, B0010, B0001, B1000, B0100, B0010, B0001,
  B1000, B0100, B0010, B0001, B1000, B0100, B0010, B0001,
  B1000, B0100, B0010, B0001, B1000, B0100, B0010, B0001,
  B1000, B0100, B0010, B0001, B1000, B0100, B0010, B0001,
  B1000, B0100, B0010, B0001, B1000, B0100, B0010, B0001,

  B1100, B0110, B0011, B1001, B1100, B0110, B0011, B1001,
  B1100, B0110, B0011, B1001, B1100, B0110, B0011, B1001,
  B1100, B0110, B0011, B1001, B1100, B0110, B0011, B1001,
  B1100, B0110, B0011, B1001, B1100, B0110, B0011, B1001,
  B1100, B0110, B0011, B1001, B1100, B0110, B0011, B1001,
  B1100, B0110, B0011, B1001, B1100, B0110, B0011, B1001,
  B1100, B0110, B0011, B1001, B1100, B0110, B0011, B1001,
  B1100, B0110, B0011, B1001, B1100, B0110, B0011, B1001,
  B1100, B0110, B0011, B1001, B1100, B0110, B0011, B1001,
  B1100, B0110, B0011, B1001, B1100, B0110, B0011, B1001,

  B1110, B0111, B1011, B1101, B1110, B0111, B1011, B1101,
  B1110, B0111, B1011, B1101, B1110, B0111, B1011, B1101,
  B1110, B0111, B1011, B1101, B1110, B0111, B1011, B1101,
  B1110, B0111, B1011, B1101, B1110, B0111, B1011, B1101,
  B1110, B0111, B1011, B1101, B1110, B0111, B1011, B1101,
  B1110, B0111, B1011, B1101, B1110, B0111, B1011, B1101,
  B1110, B0111, B1011, B1101, B1110, B0111, B1011, B1101,
  B1110, B0111, B1011, B1101, B1110, B0111, B1011, B1101,
  B1110, B0111, B1011, B1101, B1110, B0111, B1011, B1101,
  B1110, B0111, B1011, B1101, B1110, B0111, B1011, B1101,
};

void setup() {
  Wire.begin();
  Serial.begin(9600);
  swSerial.begin(9600);
  
  pinMode(sevenSegLatchPinTrue, OUTPUT);
  pinMode(sevenSegClockPinTrue, OUTPUT);
  pinMode(sevenSegDataPinTrue, OUTPUT);
  pinMode(sevenSegLatchPinFalse, OUTPUT);
  pinMode(sevenSegClockPinFalse, OUTPUT);
  pinMode(sevenSegDataPinFalse, OUTPUT);

  pinMode(BUTTON_A_PIN, INPUT);
  pinMode(BUTTON_B_PIN, INPUT);
  pinMode(BUTTON_C_PIN, INPUT);

  pinMode(RELAY_A_PIN, OUTPUT);
  pinMode(RELAY_B_PIN, OUTPUT);
  pinMode(RELAY_C_PIN, OUTPUT);
  pinMode(RELAY_D_PIN, OUTPUT);

  pinMode(rs485RsPin, OUTPUT);
  digitalWrite(rs485RsPin, LOW); // pull low to set RS-485 chip into receive mode

  Serial.println("Sign program, by Alexander Reben 2011 alex@areben.com");
  Serial.println("modified by Robert Zacharias (rz@rzach.me) in 2018; see <https://github.com/robzach/capitalism-works-for-me>");
}

void loop() {
  if (swSerial.available() == sizeof(votes)){
    
  }

  /*
  // TOP red tactile button on podium breadboard
  // reset live vote counts to 0
  if (!digitalRead(BUTTON_A_PIN))
  {
    Serial.println("BUTTON_A_PIN pressed; resetting current vote counts to 0");
    voteCountTrue = 0;
    voteCountFalse = 0;
  }

  // MIDDLE red tactile button on podium breadboard
  // reset EEPROM vote counts to 0, and current vote counts to 0
  if (!digitalRead(BUTTON_B_PIN)) {
    Serial.println("BUTTON_B_PIN pressed; resetting EEPROM vote counts to 0");
    truevote.total = 0;
    truevote.last = 0;
    falsevote.total = 0;
    falsevote.last = 0;
    EEPROM.put(eepromStartAddress, truevote);
    EEPROM.put(eepromStartAddress + sizeof(voteprototype), falsevote);
    voteCountTrue = 0;
    voteCountFalse = 0;
    eepromPrintLastValues();
  }

  // BOTTOM red tactile button on podium breadboard
  // change vote counts to 888 (for LED digit testing)
  if (!digitalRead(BUTTON_C_PIN)) {
    Serial.println("BUTTON_C_PIN pressed; setting current vote counts to 888");
    voteCountTrue = 888;
    voteCountFalse = 888;
  }
  */

  displayTrueCount(votes.truevotes);
  displayFalseCount(votes.falsevotes);
  updateLightBlink(patternA, 328);
}

void updateLightBlink (byte input[], int length) {
  if (millis() - previousMillis > MARQUEE_WAIT) {
    digitalWrite(RELAY_A_PIN, (input[currentPattern] & B00001000));
    digitalWrite(RELAY_B_PIN, (input[currentPattern] & B00000100));
    digitalWrite(RELAY_C_PIN, (input[currentPattern] & B00000010));
    digitalWrite(RELAY_D_PIN, (input[currentPattern] & B00000001));

    if (currentPattern < length - 1) currentPattern++;
    else currentPattern = 0;
    previousMillis = millis();
  }

  return;
}

void displayTrueCount(int count) {
  sevenSegDisplayString("^^^", true);
  sevenSegDisplayString((String)count, true);
  return;
}

void displayFalseCount(int count) {
  sevenSegDisplayString("^^^", false);
  sevenSegDisplayString((String)count, false);
  return;
}

/*
  Function: break down string into chars and send
*/
void sevenSegDisplayString(String displayString, boolean group) {
  for (int i = 0; i < displayString.length(); i++) sevenSegDisplayChar(displayString.charAt(i), group);

  if (group) {
    digitalWrite(sevenSegLatchPinTrue, LOW); //toggle latch
    digitalWrite(sevenSegLatchPinTrue, HIGH);
  }
  else {
    digitalWrite(sevenSegLatchPinFalse, LOW); //toggle latch
    digitalWrite(sevenSegLatchPinFalse, HIGH);
  }
  return;
}
