const int RZDEBUG = true;

#include <SPI.h>

//rs485
const int rs485RsPin = 5;

//buttons
const int trueButtonPin = 7;
const int falseButtonPin = 9;

const int trueButtonLedPin = 11;
const int falseButtonLedPin = 12;

//dip

const int dp8Pin = 30;
const int dp7Pin = 31;
const int dp6Pin = 32;
const int dp5Pin = 33;
const int dp4Pin = 34;
const int dp3Pin = 35;
const int dp2Pin = 36;
const int dp1Pin = 37;

//flags
boolean delayFlag = true;

//delay
int ledState = LOW;            
unsigned long previousMillis = 0;       
unsigned long interval = 500; 

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial1.write(0x76); //clear display


  pinMode(dp8Pin, INPUT);
  pinMode(dp7Pin, INPUT);
  pinMode(dp6Pin, INPUT);
  pinMode(dp5Pin, INPUT);
  pinMode(dp4Pin, INPUT);
  pinMode(dp3Pin, INPUT);
  pinMode(dp2Pin, INPUT);
  pinMode(dp1Pin, INPUT);

  //set DIP pullups
  digitalWrite(dp8Pin, HIGH);
  digitalWrite(dp7Pin, HIGH);
  digitalWrite(dp6Pin, HIGH);
  digitalWrite(dp5Pin, HIGH);
  digitalWrite(dp4Pin, HIGH);
  digitalWrite(dp3Pin, HIGH);
  digitalWrite(dp2Pin, HIGH);
  digitalWrite(dp1Pin, HIGH);

  //rs-485
  pinMode(rs485RsPin, OUTPUT);
  digitalWrite(rs485RsPin, HIGH); //set transmit

  //buttons
  pinMode(trueButtonPin, INPUT);
  pinMode(falseButtonPin, INPUT);
  digitalWrite(trueButtonPin, HIGH);
  digitalWrite(falseButtonPin, HIGH);
  pinMode(trueButtonLedPin, OUTPUT);
  pinMode(falseButtonLedPin, OUTPUT);


}

void loop(){
  int delayTime = 0;
  if(!digitalRead(dp8Pin) && !digitalRead(dp7Pin))
  {
    delayTime = 60;
  }
  else if(!digitalRead(dp8Pin))
  {
    delayTime = 15;
  }

  else if(!digitalRead(dp7Pin))
  {
    delayTime = 30;
  }

  else
  {
    delayTime = 0;
  }

  if(delayFlag)
  {
    //turn off button led
    digitalWrite(trueButtonLedPin, LOW);
    digitalWrite(falseButtonLedPin, LOW);

    for(int i = delayTime; i >= 0 ; i--)
    {
      ledSendInt(i, true);
      Serial.println(i);
      delay(1000); //delay 1 sec
    }
    delayFlag = false;
  }

  if(!digitalRead(trueButtonPin))
  {
    Serial2.write(1); //send rs-485
    Serial.println("True button press");
    delayFlag = true;

  }

  else if (!digitalRead(falseButtonPin))
  {
    Serial2.write(2); //send rs-485
    Serial.println("False button press");
    delayFlag = true;
  }

  //blink led
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;   
    if(ledState == LOW)
    {
      ledState = HIGH;
    }
    else
    {
      ledState = LOW;
    }
    //ledState != ledState; //toggle
    digitalWrite(trueButtonLedPin, ledState);
    digitalWrite(falseButtonLedPin, ledState);
  }
}

void ledSendInt(int number, boolean colon)
{
  byte ledBuffer[4];
  //Serial1.write(0x76); //clear
  if(colon)
  {
    //colon
    Serial1.write(0x77);
    Serial1.write(16);
  }
  else
  {
    //colon
    Serial1.write(0x77);
    Serial1.write(byte(0x00));
  }
  ledBuffer[0] = ((number%10000) - (number%1000))/1000;
  ledBuffer[1] = ((number%1000) - (number%100))/100;
  ledBuffer[2] = ((number%100) - (number%10))/10;
  ledBuffer[3] = number%10;
  Serial1.write(ledBuffer,4);
  return;
}














