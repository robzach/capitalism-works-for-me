
#include <SPI.h>
#include <Wire.h> 

//eeprom setup
// 0x50 or decimal 80 is the default address with the A0, A1, and A2 pins grounded
// You can have up to six chips 
const byte disk1 = 80;
unsigned int eepromAddress=0;
const int voteCountTrueTotalAddress = 8;
const int voteCountFalseTotalAddress = 16;
const int voteCountTrueLastAddress = 24;
const int voteCountFalseLastAddress = 36;

//eeprom vars
unsigned long eepromTotalVoteCountTrue;
unsigned long eepromTotalVoteCountFalse;
unsigned long eepromLastVoteCountTrue;
unsigned long eepromLastVoteCountFalse;

//number comm setup
const int sevenSegLatchPinTrue = 22;
const int sevenSegClockPinTrue = 24;
const int sevenSegDataPinTrue = 26;  
const int sevenSegLatchPinFalse = 28;
const int sevenSegClockPinFalse = 30;
const int sevenSegDataPinFalse = 32; 

//bell
const int bellPin = 33;

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
  B1111 };
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

  pinMode(bellPin, OUTPUT);
  digitalWrite(bellPin, HIGH);

  pinMode(rs485RsPin, OUTPUT);
  digitalWrite(rs485RsPin, LOW); //set recieve

  Serial.println("Sign program, by Alexander Reben 2011 alex@areben.com");
  eepromPrintLastValues();

  //populate last counts from eeprom
  lastVoteCountTrue = eepromReadUnsignedLong(disk1, voteCountTrueLastAddress) - 1;
  lastVoteCountFalse = eepromReadUnsignedLong(disk1, voteCountFalseLastAddress) - 1;

  voteCountTrue = lastVoteCountTrue + 1 ;
  voteCountFalse = lastVoteCountFalse + 1;

  //displayTrueCount(voteCountTrue);
  //displayFalseCount(voteCountFalse);

}

void loop(){
  currentMillis = millis();

  if(!digitalRead(buttonAPin))
  {
    voteCountTrue = 0;
    voteCountFalse = 0;
  }
  
  if(!digitalRead(buttonBPin))
  {
    ringBell();
  }

  if(!digitalRead(buttonCPin))
  {
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
  if(currentMillis - previousMillis > interval) {
    digitalWrite(relayAPin, (input[currentPattern] & B00001000));
    digitalWrite(relayBPin, (input[currentPattern] & B00000100));
    digitalWrite(relayCPin, (input[currentPattern] & B00000010));
    digitalWrite(relayDPin, (input[currentPattern] & B00000001));


    if(currentPattern < length - 1)
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
  byte podiumComm = Serial2.read();
  if(podiumComm != 255)
  {
    if(podiumComm == 1)
    {
      voteCountTrue++;
    }
    else if(podiumComm == 2)
    {
      voteCountFalse++;
    }
  }
  return;
}
void displayTrueCount(int count)
{
  sevenSegDisplayString("^^^",true);
  sevenSegDisplayString(count, true);
  return;
}

void displayFalseCount(int count)
{
  sevenSegDisplayString("^^^",false);
  sevenSegDisplayString(count, false);
  return;
}

void ringBell()
{
  digitalWrite(bellPin,LOW);
  delay(100);
  digitalWrite(bellPin,HIGH);
  return;
}

//check to see if vote has changed, if so change numbers, set eeprom, ring bell
void updateCounts()
{

  if (lastVoteCountTrue != voteCountTrue)
  {
    displayTrueCount(voteCountTrue);
    eepromWriteUnsignedLong(disk1,voteCountTrueLastAddress,voteCountTrue); //update last count
    eepromWriteUnsignedLong(disk1,voteCountTrueTotalAddress,(eepromReadUnsignedLong(disk1, voteCountTrueTotalAddress))+1); //increment total vote count in eeprom 
    lastVoteCountTrue = voteCountTrue;
    ringBell();
    Serial.print("Current TRUE : ");
    Serial.println(voteCountTrue);
    Serial.print("Total TRUE : ");
    //Serial.println(eepromReadUnsignedLong(disk1, voteCountTrueTotalAddress));
    eepromPrintLastValues();
  }
  if (lastVoteCountFalse != voteCountFalse)
  {
    displayFalseCount(voteCountFalse);
    eepromWriteUnsignedLong(disk1,voteCountFalseLastAddress,voteCountFalse);
    eepromWriteUnsignedLong(disk1,voteCountFalseTotalAddress,(eepromReadUnsignedLong(disk1, voteCountFalseTotalAddress))+1); //increment total vote count in eeprom 
    lastVoteCountFalse = voteCountFalse;
    ringBell();
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
  eepromTotalVoteCountTrue = eepromReadUnsignedLong(disk1, voteCountTrueTotalAddress);
  eepromTotalVoteCountFalse = eepromReadUnsignedLong(disk1, voteCountFalseTotalAddress);
  eepromLastVoteCountTrue = eepromReadUnsignedLong(disk1, voteCountTrueLastAddress);
  eepromLastVoteCountFalse = eepromReadUnsignedLong(disk1, voteCountFalseLastAddress);
  return;
}

void eepromPrintLastValues()
{
  Serial.print("Total vote count true eeprom : ");
  Serial.println(eepromReadUnsignedLong(disk1, voteCountTrueTotalAddress));
  Serial.print("Total vote count false eeprom: ");
  Serial.println(eepromReadUnsignedLong(disk1, voteCountFalseTotalAddress));
  Serial.print("Last vote count true eeprom: ");
  Serial.println(eepromReadUnsignedLong(disk1, voteCountTrueLastAddress));
  Serial.print("Last vote count false eeprom: ");
  Serial.println(eepromReadUnsignedLong(disk1, voteCountFalseLastAddress));
  return;
}

/*
Function: break down string into chars and send 
 */
void sevenSegDisplayString(String displayString, boolean group)
{
  Serial.println(displayString); //send string req
  for(int i = 0; i < displayString.length(); i++) 
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
Function: find proper code and pass it to be sent
 Note: 
 0b(dp)GFEDCBA
 */
void sevenSegDisplayChar(char displayChar, boolean group)
{
  switch(displayChar) //find proper code
  {
  case '^': //space
    if(!group)
    {
      //0
      digitalWrite(sevenSegDataPinFalse , LOW); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , LOW); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , LOW); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , LOW); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , LOW); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , LOW); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , LOW); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , LOW); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 

      digitalWrite(sevenSegLatchPinFalse, LOW);
      digitalWrite(sevenSegLatchPinFalse, HIGH);

    }
    else
    {
      sevenSegSendRaw(0b00000000, group); 
    }
    break;
  case '0': 
  case 'O':
    if(!group)
    {
      //0
      digitalWrite(sevenSegDataPinFalse , LOW); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , LOW); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 

      digitalWrite(sevenSegLatchPinFalse, LOW);
      digitalWrite(sevenSegLatchPinFalse, HIGH);

    }
    else
    {
      sevenSegSendRaw(0b00111111, group);
    }
    break;
  case '1':
    if(!group)
    {
      //1
      digitalWrite(sevenSegDataPinFalse , LOW); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , LOW); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , LOW); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , LOW); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , LOW); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , LOW); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 

      digitalWrite(sevenSegLatchPinFalse, LOW);
      digitalWrite(sevenSegLatchPinFalse, HIGH);

    }
    else
    {
      sevenSegSendRaw(0b00000110, group);
    }
    break;
  case '2':
    if(!group)
    {
      //2
      digitalWrite(sevenSegDataPinFalse , LOW); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , LOW); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , LOW); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 

      digitalWrite(sevenSegLatchPinFalse, LOW);
      digitalWrite(sevenSegLatchPinFalse, HIGH);
    }
    else
    {
      sevenSegSendRaw(0b01011011, group);
    }
    break;
  case '3':
    if(!group)
    {
      //3
      digitalWrite(sevenSegDataPinFalse , LOW); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , LOW); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , LOW); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 

      digitalWrite(sevenSegLatchPinFalse, LOW);
      digitalWrite(sevenSegLatchPinFalse, HIGH);

    }
    else
    {
      sevenSegSendRaw(0b01001111, group);
    }
    break;
  case '4':
    if(!group)
    {
      //4
      digitalWrite(sevenSegDataPinFalse , LOW); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , LOW); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , LOW); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , LOW); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 

      digitalWrite(sevenSegLatchPinFalse, LOW);
      digitalWrite(sevenSegLatchPinFalse, HIGH);
    }
    else
    {
      sevenSegSendRaw(0b01100110, group);
    }
    break;
  case '5': 
  case 'S':
    if(!group)
    {
      //5
      digitalWrite(sevenSegDataPinFalse , LOW); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , LOW); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , LOW); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 

      digitalWrite(sevenSegLatchPinFalse, LOW);
      digitalWrite(sevenSegLatchPinFalse, HIGH);

    }
    else
    {
      sevenSegSendRaw(0b01101101, group);
    }
    break;
  case '6':
    if(!group)
    {
      //6
      digitalWrite(sevenSegDataPinFalse , LOW); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , LOW); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 

      digitalWrite(sevenSegLatchPinFalse, LOW);
      digitalWrite(sevenSegLatchPinFalse, HIGH);

    }
    else
    {
      sevenSegSendRaw(0b01111101, group);
    }
    break;
  case '7':
    if(!group)
    {
      //7

      digitalWrite(sevenSegDataPinFalse , LOW); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , LOW); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , LOW); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , LOW); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , LOW); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 

      digitalWrite(sevenSegLatchPinFalse, LOW);
      digitalWrite(sevenSegLatchPinFalse, HIGH);

    }
    else
    {
      sevenSegSendRaw(0b00000111, group);
    }
    break;
  case '8':
    if(!group)
    {
      //8
      digitalWrite(sevenSegDataPinFalse , LOW); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 

      digitalWrite(sevenSegLatchPinFalse, LOW);
      digitalWrite(sevenSegLatchPinFalse, HIGH);

    }
    else
    {
      sevenSegSendRaw(0b01111111, group);
    }
    break;
  case '9': 
  case 'g':
    if(!group)
    {
      digitalWrite(sevenSegDataPinFalse , LOW); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , LOW); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 
      digitalWrite(sevenSegDataPinFalse , HIGH); //
      digitalWrite(sevenSegClockPinFalse, LOW);
      digitalWrite(sevenSegClockPinFalse, HIGH); 

      digitalWrite(sevenSegLatchPinFalse, LOW);
      digitalWrite(sevenSegLatchPinFalse, HIGH);
    }
    else
    {
      sevenSegSendRaw(0b01101111, group);
    }
    break;  
  case 'A':
    sevenSegSendRaw(0b01110111, group);
    break;  
  case 'b':
    sevenSegSendRaw(0b01111100, group);
    break;  
  case 'C':
    sevenSegSendRaw(0b00111001, group);
    break;  
  case 'd':
    sevenSegSendRaw(0b01011110, group);
    break;  
  case 'E':
    sevenSegSendRaw(0b01111001, group);
    break;  
  case 'F':
    sevenSegSendRaw(0b01110001, group);
    break;  
  case 'J':
    sevenSegSendRaw(0b00011110, group);
    break;
  case 'o':
    sevenSegSendRaw(0b01011100, group);
    break;
  case 'u':
    sevenSegSendRaw(0b00011100, group);
    break;
  case 'y':
    sevenSegSendRaw(0b01101110, group);
    break;
  case '-':
    sevenSegSendRaw(0b01000000, group);
    break;
  default:  
    sevenSegSendRaw(0b00000000, group); 
  }
  return;
}

/*
Function: send code to display
 */
void sevenSegSendRaw(int code, boolean group)
{
  if(group)
  {
    shiftOut(sevenSegDataPinTrue, sevenSegClockPinTrue, MSBFIRST, code); //msb shift
  }
  else
  {
    shiftOut(sevenSegDataPinFalse, sevenSegClockPinFalse, MSBFIRST, code); //msb shift
  }
  return;
}

void eepromWrite(int theDeviceAddress, unsigned int theMemoryAddress, int theByteCount, byte* theByteArray) 
{
  for (int theByteIndex = 0; theByteIndex < theByteCount; theByteIndex++) 
  {
    Wire.beginTransmission(theDeviceAddress);
    Wire.send((byte)((theMemoryAddress + theByteIndex) >> 8));
    Wire.send((byte)((theMemoryAddress + theByteIndex) >> 0));
    Wire.send(theByteArray[theByteIndex]);
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
    Wire.send((byte)((theMemoryAddress + theByteIndex) >> 8));
    Wire.send((byte)((theMemoryAddress + theByteIndex) >> 0));
    Wire.endTransmission();
    delay(3);
    Wire.requestFrom(theDeviceAddress, sizeof(byte));
    theByteArray[theByteIndex] = Wire.receive();
  }
  return;
}


void eepromWriteUnsignedLong(int theDeviceAddress, unsigned int theMemoryAddress, unsigned long theLong) 
{
  byte theByteArray[sizeof(unsigned long)] = {
    (byte)(theLong >> 32),(byte)(theLong >> 16),(byte)(theLong >> 8), (byte)(theLong >> 0)          
    };
    eepromWrite(theDeviceAddress, theMemoryAddress, sizeof(unsigned long), theByteArray);
  eepromAddress=eepromAddress+2;
 Serial.println("Write long eeprom");
 return;
}

unsigned long eepromReadUnsignedLong(int theDeviceAddress, unsigned int theMemoryAddress) 
{
  byte theByteArray[sizeof(unsigned long)];
  eepromRead(theDeviceAddress, theMemoryAddress, sizeof(unsigned long), theByteArray);
  return (unsigned long)(((theByteArray[0] << 32)) | (unsigned long)((theByteArray[1] << 16)) | (unsigned long)((theByteArray[2] << 8)) | (unsigned long)((theByteArray[3] << 0)));
}




