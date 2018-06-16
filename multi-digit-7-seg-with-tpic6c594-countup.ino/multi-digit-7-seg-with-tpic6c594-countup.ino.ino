/*

  Multi-digit 7-segment display driver with TPIC6C594 adapted with only tiny change from:

  "
  Controlling large 7-segment displays
  By: Nathan Seidle
  SparkFun Electronics
  Date: February 25th, 2015
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
  "
  wiring for first TPIC6C594:
  TPIC6C594 pin | Arduino pin
         SER IN | 11
         SRCK   | 12
         RCK    | 13

  The second TPIC6C594 is identical to above, except its SER IN comes from the SER OUT of the first TPIC6C594.

  
  Robert Zacharias, rz@rzach.me, 2018
  see <https://github.com/robzach/capitalism-works-for-me/> for much more information.

*/

//GPIO declarations
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
byte segmentClock = 13;
byte segmentLatch = 12;
byte segmentData = 11;

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void setup()
{
  Serial.begin(9600);
  Serial.println("Large Digit Driver Example");

  pinMode(segmentClock, OUTPUT);
  pinMode(segmentData, OUTPUT);
  pinMode(segmentLatch, OUTPUT);

  digitalWrite(segmentClock, LOW);
  digitalWrite(segmentData, LOW);
  digitalWrite(segmentLatch, LOW);
}

int number = 0;

void loop()
{
  showNumber(number); //Test pattern
  number++;
  number %= 100; //Reset x after 99

  Serial.println(number); //For debugging

  delay(500);
}

//Takes a number and displays 2 numbers. Displays absolute value (no negatives)
void showNumber(float value)
{
  int number = abs(value); //Remove negative signs and any decimals

  //Serial.print("number: ");
  //Serial.println(number);

  for (byte x = 0 ; x < 2 ; x++)
  {
    int remainder = number % 10;

    postNumber(remainder, false);

    number /= 10;
  }

  //Latch the current segment data
  digitalWrite(segmentLatch, LOW);
  digitalWrite(segmentLatch, HIGH); //Register moves storage register on the rising edge of RCK
}

//Given a number, or '-', shifts it out to the display
void postNumber(byte number, boolean decimal)
{
  //    -  A
  //   / / F/B
  //    -  G
  //   / / E/C
  //    -. D/DP
#define a  1<<1
#define b  1<<7
#define c  1<<6
#define d  1<<5
#define e  1<<4
#define f  1<<2
#define g  1<<3
#define dp 1<<8

  byte segments;

  switch (number)
  {
    case 1: segments = b | c; break;
    case 2: segments = a | b | d | e | g; break;
    case 3: segments = a | b | c | d | g; break;
    case 4: segments = f | g | b | c; break;
    case 5: segments = a | f | g | c | d; break;
    case 6: segments = a | f | g | e | c | d; break;
    case 7: segments = a | b | c; break;
    case 8: segments = a | b | c | d | e | f | g; break;
    case 9: segments = a | b | c | d | f | g; break;
    case 0: segments = a | b | c | d | e | f; break;
    case ' ': segments = 0; break;
    case 'c': segments = g | e | d; break;
    case '-': segments = g; break;
  }

  if (decimal) segments |= dp;

  //Clock these bits out to the drivers
  for (byte x = 0 ; x < 8 ; x++)
  {
    digitalWrite(segmentClock, LOW);
    digitalWrite(segmentLatch, LOW);
    digitalWrite(segmentData, segments & 1 << (7 - x));
    digitalWrite(segmentClock, HIGH); //Data transfers to the register on the rising edge of SRCK
    digitalWrite(segmentLatch, HIGH);
  }
}


