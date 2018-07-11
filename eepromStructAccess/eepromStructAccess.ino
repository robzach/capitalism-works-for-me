#include <EEPROM.h>

int startAddress = 0;

struct vote{
  int total, last;
} voteprototype;

void setup() {
  Serial.begin(9600);
  struct vote truevote = EEPROM.get(startAddress, voteprototype);
  struct vote falsevote = EEPROM.get(startAddress + sizeof(vote), voteprototype);
  Serial.println(truevote.total);
  Serial.println(truevote.last);
  Serial.println(falsevote.total);
  Serial.println(falsevote.last);
}

void loop() {
  // put your main code here, to run repeatedly:

}
