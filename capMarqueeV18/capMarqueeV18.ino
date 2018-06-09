/*
   Marquee light driver for Steve Lambert's "Capitalism Works for Me"

   see <https://github.com/robzach/capitalism-works-for-me> for further information, including schematic

   A subset of Alex Reben's 2010 "capSign" sketch.

   Robert Zacharias, rz@rzach.me, 2018
*/

//input buttons
const int MARQUEE_MODE_BUTTON = 8;
const int ALL_ON_BUTTON = 9;
bool marqueeMode = true;

//timing
unsigned long previousMillis = 0;
const unsigned long MARQUEE_WAIT = 150; // milliseconds between marquee animation steps

//relay
const int RELAY_A_PIN = 2;
const int RELAY_B_PIN = 3;
const int RELAY_C_PIN = 4;
const int RELAY_D_PIN = 5;
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
  pinMode(RELAY_A_PIN, OUTPUT);
  pinMode(RELAY_B_PIN, OUTPUT);
  pinMode(RELAY_C_PIN, OUTPUT);
  pinMode(RELAY_D_PIN, OUTPUT);

  pinMode(MARQUEE_MODE_BUTTON, INPUT_PULLUP);
  pinMode(ALL_ON_BUTTON, INPUT_PULLUP);

  Serial.begin(9600);

  Serial.println("Sign program, by Alexander Reben 2011 alex@areben.com");
  Serial.println("modified by Robert Zacharias (rz@rzach.me) in 2018; see <https://github.com/robzach/capitalism-works-for-me>");
}

void loop() {

  if (!digitalRead(MARQUEE_MODE_BUTTON)) marqueeMode = true;
  if (!digitalRead(ALL_ON_BUTTON)) marqueeMode = false;

  //regular mode
  if (marqueeMode) updateLightBlink(patternA, 328);

  //all-on mode (for confirming all the bulbs are good, photographs, etc.)
  else {
    digitalWrite(RELAY_A_PIN, HIGH);
    digitalWrite(RELAY_B_PIN, HIGH);
    digitalWrite(RELAY_C_PIN, HIGH);
    digitalWrite(RELAY_D_PIN, HIGH);
  }
}

void updateLightBlink (byte input[], int length) {
  if (millis() - previousMillis > MARQUEE_WAIT) {
    digitalWrite(RELAY_A_PIN, (input[currentPattern] & B00001000));
    digitalWrite(RELAY_B_PIN, (input[currentPattern] & B00000100));
    digitalWrite(RELAY_C_PIN, (input[currentPattern] & B00000010));
    digitalWrite(RELAY_D_PIN, (input[currentPattern] & B00000001));

    if (currentPattern < length - 1) currentPattern++;
    else currentPattern = 0;
    previousMillis = millis();
  }

  return;
}
