/*

  7-segment display driver with TPIC6C594 adapted from:

  "
  Controlling large 7-segment displays
  By: Nathan Seidle
  SparkFun Electronics
  Date: February 25th, 2015
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
  "

  wiring:
  TPIC6C594 pin | Arduino pin
         SER IN | 11
         SRCK   | 12
         RCK    | 13


  Robert Zacharias, rz@rzach.me, 2018
  see <https://github.com/robzach/capitalism-works-for-me/> for much more information.
*/

byte segmentClock = 13;
byte segmentLatch = 12;
byte segmentData = 11;

void setup()
{
  Serial.begin(9600);

  pinMode(segmentClock, OUTPUT);
  pinMode(segmentData, OUTPUT);
  pinMode(segmentLatch, OUTPUT);


  digitalWrite(segmentClock, LOW);
  digitalWrite(segmentData, LOW);
  digitalWrite(segmentLatch, HIGH);
}

void loop()
{
  static int x = 0;

  delay(1000);
  postNumber(x);
  x++;
  if (x > 9) x = 0;
}

void postNumber(byte number)
{
  //    -  A
  //   / / F/B
  //    -  G
  //   / / E/C
  //    -. D/DP
  //

  // THE BLOCK OF #DEFINES WHICH COST ME LIKE FIVE HOURS OF TROUBLESHOOTING
  // these are all off by 1. I simply had to increment them by 1 to make the display work properly.
  //#define a  1<<0
  //#define b  1<<6
  //#define c  1<<5
  //#define d  1<<4
  //#define e  1<<3
  //#define f  1<<1
  //#define g  1<<2
  //#define dp 1<<7

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
  }

  //  Clock these bits out to the drivers
  for (byte x = 0 ; x < 8 ; x++)
  {
    digitalWrite(segmentClock, LOW);
    digitalWrite(segmentLatch, LOW);
    digitalWrite(segmentData, segments & 1 << (7 - x));
    digitalWrite(segmentClock, HIGH); //Data transfers to the register on the rising edge of SRCK
    digitalWrite(segmentLatch, HIGH);

  }
}

