/*
simplest test to run on sign to see if any data is coming 
over Serial2 from the podium

4-12-18 rz@rzach.me
*/

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
}

void loop() {
  if(Serial2.available()){
    int inval = Serial2.read();
    Serial.println(inval);
  }
}
