#include <Arduino.h>
#include <PLDuino.h>
#define FLASH_ESP_BAUD 115200

void setup()
{
  using namespace PLDuino;
  
  PLDuino::init();
  enableESP();
  Serial2.begin(FLASH_ESP_BAUD);
  Serial.begin(FLASH_ESP_BAUD);
  
  while(1)
  {
    if (Serial2.available()) Serial.write(Serial2.read());
    if (Serial.available()) Serial2.write(Serial.read());
  }
}

void loop(){}

