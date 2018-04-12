/*
   simplest test to see if the podium is sending data over
   Serial2 to the sign

   4-12-18 rz@rzach.me
*/


const int trueButtonPin = 7;
const int falseButtonPin = 9;
const int rs485RsPin = 5;


void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);

  pinMode(trueButtonPin, INPUT_PULLUP);
  pinMode(falseButtonPin, INPUT_PULLUP);

  pinMode(rs485RsPin, OUTPUT);
  digitalWrite(rs485RsPin, HIGH); //set transmit
}

void loop() {
  if (!digitalRead(trueButtonPin)) {
    Serial2.write(1);
    Serial.println("1 transmitted over Serial2");
  }
  if (!digitalRead(falseButtonPin)) {
    Serial2.write(2);
    Serial.println("2 transmitted over Serial2");
  }
}
