#include <PLDuino.h>

void setup()
{
  // Necessary initialization.
  PLDuino::init();

  // Initializing Serial (you can monitor it via USB).
  Serial.begin(115200);

  // Send example message.
  Serial.println("Hello world!");
}

void loop()
{
  // Check if any data was sent by user
  if (Serial.available())
  {
    // Read it
    char data = Serial.read();
    // and send it back
    Serial.write(data);
  }
}

