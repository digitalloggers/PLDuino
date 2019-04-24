#include <PLDuino.h>

void setup()
{
  // Necessary initialization.
  PLDuino::init();
  
  // Initialization of USB-attached serial.
  Serial.begin(115200);
  
  // Initialization of external Serial (RS-232).
  Serial3.begin(115200);
}

void loop()
{
  // Simply transfer data from Serial to Serial3 and back.
  if (Serial3.available())
    Serial.write(Serial3.read());
  if (Serial.available())
    Serial3.write(Serial.read());
}

