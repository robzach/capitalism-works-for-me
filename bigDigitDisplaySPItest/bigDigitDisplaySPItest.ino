//This sketch will push the string '123456' out to a series of Big Digit Drivers.
// from https://wiki.evilmadscientist.com/Using_The_Big_Digit_Driver#Display_123456


void setup() {                
  pinMode(13, OUTPUT);    //SH_CP/SCK    Shift register clock
  pinMode(12, OUTPUT);    //SR_CP/MISO   Latch Clock
  pinMode(11, OUTPUT);    //DS/MOSI      Serial data in
 
  
  pinMode(A2, OUTPUT);
  digitalWrite(A2, LOW); // use pin A2 as a fake ground (by pulling it low)
  
  //Blank out the register
  shiftOut(11, 13, MSBFIRST, 0x00);
  digitalWrite(12, HIGH);
  digitalWrite(12, LOW);
}

void loop() {
  shiftOut(11, 13, MSBFIRST, 0x06);  //'1'; segments B and C, or b00000110
  shiftOut(11, 13, MSBFIRST, 0x5B);  //'2'; segments A, B, D, E, G or b01011011
  shiftOut(11, 13, MSBFIRST, 0x4F);  //'3'; segments A, B, C, D, G, or b01001111
  shiftOut(11, 13, MSBFIRST, 0x66);  //'4'; segments B, C, F, G, or b01100110
  shiftOut(11, 13, MSBFIRST, 0x6D);  //'5'; segments A, C, D, F, G, or b01101101
  shiftOut(11, 13, MSBFIRST, 0x7D);  //'6'; segments A, C, D, E, F, G, or b01111101

  //Pulse the latch clock to load the output
  digitalWrite(12, HIGH);  
  digitalWrite(12, LOW);

  delay(1000);

}//End loop
