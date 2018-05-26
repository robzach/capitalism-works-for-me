/*
  individual 7-segment segments are lit manually via Serial commands

  intended for driving three Evil Mad Scientist Big Digit Drivers, chained together
  via SPI bus.
  
  enter 0 in the Serial monitor to turn off all segments
  enter 1 in the Serial monitor to rotate which individual segment is lit
  
  enter a to operate on the rightmost digit
  enter b to operate on the center digit
  enter c to operate on the leftmost digit
*/

const int GNDPIN = A2; // using Arduino's A2 as a ground

char digitSelect = 'a';

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
  static byte command = 1;
  
  if (Serial.available() > 0) {
    char inChar = Serial.read();
    if (inChar == 'a'){
      digitSelect = 'a';
      Serial.println("writing to rightmost digit");
    }
    
    if (inChar == 'b'){
      digitSelect = 'b';
      Serial.println("writing to center digit");
    }
    if (inChar == 'c'){
      digitSelect = 'c';
      Serial.println("writing to leftmost digit");
    }
    if (inChar == '0') { // turn off all segments
      command = 0;
    }
    
    else if (inChar == '1') { // rotate through 0b0, 0b10, 0b100,...,0b1000000 to light one segment at a time
      command <<= 1;
      if (command == 0b10000000 || command == 0) command = 1; // if overflow, or 0, reset to 1
    }

    else if (inChar == '2') { // increment commands by 1, to walk through entire range
      command += 1;
      if (command == 0) command = 1; // if overflow, or 0, reset to 1

    }

    Serial.print("command = "); Serial.println(command);
  }
  
  segout(command);
}


void segout(byte data) {

  if (digitSelect == 'a') { // to operate on rightmost digit
    // send out two zeros first for the left and center digits
    shiftOut(11, 13, MSBFIRST, 0);
    shiftOut(11, 13, MSBFIRST, 0);

    // send out the wanted data, which will land on the rightmost digit
    shiftOut(11, 13, MSBFIRST, data);
  }

  else if (digitSelect == 'b'){
    shiftOut(11, 13, MSBFIRST, 0);
    shiftOut(11, 13, MSBFIRST, data);
    shiftOut(11, 13, MSBFIRST, 0);
  }

  else if (digitSelect == 'c'){
    shiftOut(11, 13, MSBFIRST, data);
    shiftOut(11, 13, MSBFIRST, 0);
    shiftOut(11, 13, MSBFIRST, 0);
  }

  //Pulse the latch clock to load the output into all digits
  digitalWrite(12, HIGH);
  digitalWrite(12, LOW);
}
