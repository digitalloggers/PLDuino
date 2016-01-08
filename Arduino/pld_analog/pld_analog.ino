#include <PLDuino.h>
using namespace PLDuino;

void setup()
{
  // Initializing Serial.
  Serial.begin(9600);
  
  // A necessary call.
  // It sets pin modes according to their function.
  PLDuino::init();
}

void loop()
{
  // Simply use Arduino's analogRead() call to read AINs.
  Serial.println("AIN1: " + String(analogRead(PLDuino::AIN1)));
  delay(100);
}

