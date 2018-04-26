const boolean RZDEBUG = true; // set this true or false to turn additional serial debugging on or off

#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>

//eeprom setup
const int eepromStartAddress = 0;

// struct definition to contain data on eeprom
struct vote {
  int total, last;
} voteprototype;

// declaring global structs to hold the vote counts
struct vote truevote, falsevote;

                  //const int voteCountTrueTotalAddress = 8;
                  //const int voteCountFalseTotalAddress = 16;
                  //const int voteCountTrueLastAddress = 24;
                  //const int voteCountFalseLastAddress = 36;
                  //
                  ////eeprom vars
                  //unsigned long eepromTotalVoteCountTrue;
                  //unsigned long eepromTotalVoteCountFalse;
                  //unsigned long eepromLastVoteCountTrue;
                  //unsigned long eepromLastVoteCountFalse;

//number comm setup
const int sevenSegLatchPinTrue = 22;
const int sevenSegClockPinTrue = 24;
const int sevenSegDataPinTrue = 26;
const int sevenSegLatchPinFalse = 28;
const int sevenSegClockPinFalse = 30;
const int sevenSegDataPinFalse = 32;

//global current vote count
int voteCountTrue = 0;
int voteCountFalse = 0;
int lastVoteCountTrue = -1;
int lastVoteCountFalse = -1;

//dip

const int dp8Pin = 34;
const int dp7Pin = 35;
const int dp6Pin = 36;
const int dp5Pin = 37;
const int dp4Pin = 38;
const int dp3Pin = 39;
const int dp2Pin = 40;
const int dp1Pin = 41;

//button
const int buttonAPin = 42;
const int buttonBPin = 43;
const int buttonCPin = 44;

const int buttonResetPin = 3;
const int buttonResetGroundPin = 4;

//LED
const int ledAPin = 45;
const int ledBPin = 46;
const int ledCPin = 47;
const int ledDPin = 48;

//timing
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

//rs485
const int rs485RsPin = 5;

//relay
const int relayAPin = 6;
const int relayBPin = 7;
const int relayCPin = 8;
const int relayDPin = 9;
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
  Serial2.begin(9600);
  pinMode(sevenSegLatchPinTrue, OUTPUT);
  pinMode(sevenSegClockPinTrue, OUTPUT);
  pinMode(sevenSegDataPinTrue, OUTPUT);
  pinMode(sevenSegLatchPinFalse, OUTPUT);
  pinMode(sevenSegClockPinFalse, OUTPUT);
  pinMode(sevenSegDataPinFalse, OUTPUT);

  pinMode(buttonResetPin, INPUT);
  digitalWrite(buttonResetPin, HIGH);
  pinMode(buttonResetGroundPin, OUTPUT);
  digitalWrite(buttonResetGroundPin, LOW);

  pinMode(buttonAPin, INPUT);
  pinMode(buttonBPin, INPUT);
  pinMode(buttonCPin, INPUT);

  pinMode(relayAPin, OUTPUT);
  pinMode(relayBPin, OUTPUT);
  pinMode(relayCPin, OUTPUT);
  pinMode(relayDPin, OUTPUT);

  pinMode(rs485RsPin, OUTPUT);
  digitalWrite(rs485RsPin, LOW); //set recieve

  Serial.println("Sign program, by Alexander Reben 2011 alex@areben.com");
  Serial.println("modified by Robert Zacharias (rz@rzach.me) in 2018; see <https://github.com/robzach/capitalism-works-for-me>");

  // load prior eeprom data, if any
  truevote = EEPROM.get(eepromStartAddress, voteprototype);
  falsevote = EEPROM.get(eepromStartAddress + sizeof(vote), voteprototype);
  
  eepromPrintLastValues();

                //  //populate last counts from eeprom
                //  lastVoteCountTrue = eepromReadUnsignedLong(disk1, voteCountTrueLastAddress) - 1;
                //  lastVoteCountFalse = eepromReadUnsignedLong(disk1, voteCountFalseLastAddress) - 1;
                //
                //  voteCountTrue = lastVoteCountTrue + 1 ;
                //  voteCountFalse = lastVoteCountFalse + 1;

  //displayTrueCount(voteCountTrue);
  //displayFalseCount(voteCountFalse);

}

void loop() {
  currentMillis = millis();

  // TOP red tactile button on podium breadboard
  // reset live vote counts to 0
  if (!digitalRead(buttonAPin))
  {
    Serial.println("buttonAPin pressed; resetting current vote counts to 0");
    voteCountTrue = 0;
    voteCountFalse = 0;
  }

  // MIDDLE red tactile button on podium breadboard
  // reset EEPROM vote counts to 0
  if (!digitalRead(buttonBPin)){
    Serial.println("buttonBPin pressed; resetting EEPROM vote counts to 0");
    truevote.total = 0;
    truevote.last = 0;
    falsevote.total = 0;
    falsevote.last = 0;
    EEPROM.put(eepromStartAddress, truevote);
    EEPROM.put(eepromStartAddress + sizeof(voteprototype), falsevote);
    eepromPrintLastValues();
  }

  // BOTTOM red tactile button on podium breadboard
  // reset vote counts to 888 (for LED digit testing)
  if (!digitalRead(buttonCPin))
  {
    Serial.println("buttonCPin pressed; resetting current vote counts to 888");
    voteCountTrue = 888;
    voteCountFalse = 888;
  }
  updatePodiumCommWired();
  updateCounts();

  updateLightBlink(patternA, 328, 150);

  eepromPopulateLastValues();


}

void updateLightBlink (byte input[], int length, int interval)
{
  if (currentMillis - previousMillis > interval) {
    digitalWrite(relayAPin, (input[currentPattern] & B00001000));
    digitalWrite(relayBPin, (input[currentPattern] & B00000100));
    digitalWrite(relayCPin, (input[currentPattern] & B00000010));
    digitalWrite(relayDPin, (input[currentPattern] & B00000001));


    if (currentPattern < length - 1)
    {
      currentPattern++;
    }
    else
    {
      currentPattern = 0;
    }

    previousMillis = currentMillis;
  }

  return;
}

void updatePodiumCommWired()
{
  if (RZDEBUG) {
    if (Serial2.available()) {
      int inByte = Serial2.peek();
      Serial.print("data received from podium: ");
      Serial.println(inByte);
    }
  }
  byte podiumComm = Serial2.read();

  if (podiumComm != 255)
  {
    if (podiumComm == 1)
    {
      voteCountTrue++;
      if (RZDEBUG) Serial.println("podiumComm message 1 received");
    }
    else if (podiumComm == 2)
    {
      voteCountFalse++;
      if (RZDEBUG) Serial.println("podiumComm message 2 received");
    }
  }
  return;
}
void displayTrueCount(int count)
{
  sevenSegDisplayString("^^^", true);
  sevenSegDisplayString((String)count, true);
  return;
}

void displayFalseCount(int count)
{
  sevenSegDisplayString("^^^", false);
  sevenSegDisplayString((String)count, false);
  return;
}

//check to see if vote has changed, if so change numbers, set eeprom
void updateCounts()
{

  if (lastVoteCountTrue != voteCountTrue)
  {
    displayTrueCount(voteCountTrue);
          //    eepromWriteUnsignedLong(disk1, voteCountTrueLastAddress, voteCountTrue); //update last count
    truevote.last = voteCountTrue;
          //    eepromWriteUnsignedLong(disk1, voteCountTrueTotalAddress, (eepromReadUnsignedLong(disk1, voteCountTrueTotalAddress)) + 1);
    truevote.total++;
    EEPROM.put(eepromStartAddress, truevote);
    //increment total vote count in eeprom
    
    lastVoteCountTrue = voteCountTrue;
    Serial.print("Current TRUE : ");
    Serial.println(voteCountTrue);
    Serial.print("Total TRUE : ");
    //Serial.println(eepromReadUnsignedLong(disk1, voteCountTrueTotalAddress));
    eepromPrintLastValues();
  }
  if (lastVoteCountFalse != voteCountFalse)
  {
    displayFalseCount(voteCountFalse);
          //    eepromWriteUnsignedLong(disk1, voteCountFalseLastAddress, voteCountFalse);
    falsevote.last = voteCountFalse;
          //    eepromWriteUnsignedLong(disk1, voteCountFalseTotalAddress, (eepromReadUnsignedLong(disk1, voteCountFalseTotalAddress)) + 1);
    falsevote.total++;
    EEPROM.put(eepromStartAddress + sizeof(voteprototype), falsevote);
    //increment total vote count in eeprom
    lastVoteCountFalse = voteCountFalse;
    Serial.print("Current FALSE : ");
    Serial.println(voteCountFalse);
    Serial.print("Total FALSE : ");
    //Serial.println(eepromReadUnsignedLong(disk1, voteCountTrueTotalAddress));
    eepromPrintLastValues();
  }
}

//read the last values from the eeprom and update them
void eepromPopulateLastValues()
{

    // load prior eeprom data, if any
  truevote = EEPROM.get(eepromStartAddress, voteprototype);
  falsevote = EEPROM.get(eepromStartAddress + sizeof(vote), voteprototype);
  
//        eepromTotalVoteCountTrue = eepromReadUnsignedLong(disk1, voteCountTrueTotalAddress);
//        eepromTotalVoteCountFalse = eepromReadUnsignedLong(disk1, voteCountFalseTotalAddress);
//        eepromLastVoteCountTrue = eepromReadUnsignedLong(disk1, voteCountTrueLastAddress);
//        eepromLastVoteCountFalse = eepromReadUnsignedLong(disk1, voteCountFalseLastAddress);
  
  return;
}

void eepromPrintLastValues()
{
  Serial.print("total true votes from eeprom: ");
  Serial.println(truevote.total);
  Serial.print("total false votes from eeprom: ");
  Serial.println(falsevote.total);
  Serial.print("last true votes from eeprom: ");
  Serial.println(truevote.last);
  Serial.print("last false votes from eeprom: ");
  Serial.println(falsevote.last);
            //    
            //  Serial.print("Total vote count true eeprom : ");
            //  Serial.println(eepromReadUnsignedLong(disk1, voteCountTrueTotalAddress));
            //  Serial.print("Total vote count false eeprom: ");
            //  Serial.println(eepromReadUnsignedLong(disk1, voteCountFalseTotalAddress));
            //  Serial.print("Last vote count true eeprom: ");
            //  Serial.println(eepromReadUnsignedLong(disk1, voteCountTrueLastAddress));
            //  Serial.print("Last vote count false eeprom: ");
            //  Serial.println(eepromReadUnsignedLong(disk1, voteCountFalseLastAddress));
  return;
}

/*
  Function: break down string into chars and send
*/
void sevenSegDisplayString(String displayString, boolean group)
{
  // RZ COMMENTED OUT LINE BELOW
  //  Serial.println(displayString); //send string req
  for (int i = 0; i < displayString.length(); i++)
    //for(int i = 5; i >= 0; i--) //assumption of a 6 char string
  {
    sevenSegDisplayChar(displayString.charAt(i), group);
  }
  if (group)
  {
    digitalWrite(sevenSegLatchPinTrue, LOW); //toggle latch
    digitalWrite(sevenSegLatchPinTrue, HIGH);
  }
  else
  {
    digitalWrite(sevenSegLatchPinFalse, LOW); //toggle latch
    digitalWrite(sevenSegLatchPinFalse, HIGH);
  }
  return;
}



/*

void eepromWrite(int theDeviceAddress, unsigned int theMemoryAddress, int theByteCount, byte* theByteArray)
{
  for (int theByteIndex = 0; theByteIndex < theByteCount; theByteIndex++)
  {
    Wire.beginTransmission(theDeviceAddress);
    Wire.write((byte)((theMemoryAddress + theByteIndex) >> 8));
    Wire.write((byte)((theMemoryAddress + theByteIndex) >> 0));
    Wire.write(theByteArray[theByteIndex]);
    Wire.endTransmission();
    delay(4);
  }
  return;
}


void eepromRead(int theDeviceAddress, unsigned int theMemoryAddress, int theByteCount, byte* theByteArray)
{
  for (int theByteIndex = 0; theByteIndex < theByteCount; theByteIndex++)
  {
    Wire.beginTransmission(theDeviceAddress);
    Wire.write((byte)((theMemoryAddress + theByteIndex) >> 8));
    Wire.write((byte)((theMemoryAddress + theByteIndex) >> 0));
    Wire.endTransmission();
    delay(3);
    Wire.requestFrom(theDeviceAddress, sizeof(byte));
    theByteArray[theByteIndex] = Wire.read();
  }
  return;
}



            void eepromWriteUnsignedLong(int theDeviceAddress, unsigned int theMemoryAddress, unsigned long theLong)
            {
              byte theByteArray[sizeof(unsigned long)] = {
                (byte)(theLong >> 32), (byte)(theLong >> 16), (byte)(theLong >> 8), (byte)(theLong >> 0)
              };
              eepromWrite(theDeviceAddress, theMemoryAddress, sizeof(unsigned long), theByteArray);
              eepromAddress = eepromAddress + 2;
              Serial.println("Write long eeprom");
              return;
            }
            
            unsigned long eepromReadUnsignedLong(int theDeviceAddress, unsigned int theMemoryAddress)
            {
              byte theByteArray[sizeof(unsigned long)];
              eepromRead(theDeviceAddress, theMemoryAddress, sizeof(unsigned long), theByteArray);
              return (unsigned long)(((theByteArray[0] << 32)) | (unsigned long)((theByteArray[1] << 16)) |
                                     (unsigned long)((theByteArray[2] << 8)) | (unsigned long)((theByteArray[3] << 0)));
            }
*/




