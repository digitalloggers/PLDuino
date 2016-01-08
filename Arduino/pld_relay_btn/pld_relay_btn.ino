#include <PLDuino.h>

void setup()
{
  // Necessary initialization.
  PLDuino::init();
}

void loop()
{
  // Read digital pin D34
  int state = digitalRead(34);
  // Set RELAY1 state according to D34
  digitalWrite(PLDuino::RELAY1, state);
}
