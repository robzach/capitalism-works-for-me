byte data = 0x00;
int state = 0;

const byte ZERO = 0x3f;
const byte ONE = 0x06;
const byte TWO = 0x5b;
const byte THREE = 0x4f;
const byte FOUR = 0x66;
const byte FIVE = 0x6d;
const byte SIX = 0x7d;
const byte SEVEN = 0x07;
const byte EIGHT = 0x7f;
const byte NINE = 0x6f;

const unsigned long WAIT = 1000;
unsigned long timer;

const int GNDPIN = A2;

void setup() {
  Serial.begin(9600);
  
  pinMode(13, OUTPUT);    //SH_CP/SCK    Shift register clock
  pinMode(12, OUTPUT);    //SR_CP/MISO   Latch Clock
  pinMode(11, OUTPUT);    //DS/MOSI      Serial data in

  pinMode(GNDPIN, OUTPUT);
  digitalWrite(GNDPIN, LOW);

  //Blank out the register
  shiftOut(11, 13, MSBFIRST, 0x00);
  digitalWrite(12, HIGH);
  digitalWrite(12, LOW);
}

void loop() {
  static int x = 0;

  if (millis() - timer >= WAIT) {
    x++;
    Serial.println(x);
    x %= 10;
    timer = millis();

    if (x == 0) segout(ZERO);
    else if (x == 1) segout(ONE);
    else if (x == 2) segout(TWO);
    else if (x == 3) segout(THREE);
    else if (x == 4) segout(FOUR);
    else if (x == 5) segout(FIVE);
    else if (x == 6) segout(SIX);
    else if (x == 7) segout(SEVEN);
    else if (x == 9) segout(NINE);
  }

  /*
  switch (state) {
    case 0:
      data = 0xAA;   //b10101010
      state = 1;
      break;

    case 1:
      data = 0x55;  //b01010101
      state = 2;
      break;

    case 2:
      data = 0xFF;  //b11111111
      state = 3;
      break;

    case 3:
      data = 0x00;  //b00000000
      state = 0;
      break;

  }//End switch

  //Write the data to the shift register
  

  delay(2000);
  */

}//End loop

void segout(byte data) {
  //Shift data out to shift register
  shiftOut(11, 13, MSBFIRST, data);

  //Pulse the latch clock to load the output
  digitalWrite(12, HIGH);
  digitalWrite(12, LOW);
}
