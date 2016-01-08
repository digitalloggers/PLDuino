#include <PLDuino.h>
using namespace PLDuino;

void setup()
{
  // A necessary call.
  // It sets pin modes according to their function.
  PLDuino::init();
}

void loop()
{
  // Simple switch RELAY1 on and off with 1 second timeout.
  digitalWrite(PLDuino::RELAY1, HIGH);
  delay(1000);
  digitalWrite(PLDuino::RELAY1, LOW);
  delay(1000);
}
