#include <PLDuino.h>
using namespace PLDuino;

void setup()
{
  // Initializing Serial.
  Serial.begin(115200);
  
  // A necessary call.
  // It sets pin modes according to their function.
  PLDuino::init();
}

void loop()
{
  // Simply use Arduino's analogRead() call to read AINs.
  Serial.println("A0: " + String(analogRead(A0)));
  delay(100);
}

