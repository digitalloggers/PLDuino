#include <Arduino.h>
#include <PLDuino.h>
using namespace PLDuino;

#define FLASH_ESP_BAUD 115200

void powerOnESP02ForProgramming()
{
  pinMode(ESP_CHPD, OUTPUT); digitalWrite(ESP_CHPD, HIGH);
  pinMode(ESP_GPIO0, OUTPUT); digitalWrite(ESP_GPIO0, LOW);
  pinMode(ESP_GPIO2, OUTPUT); digitalWrite(ESP_GPIO2, HIGH);
  pinMode(ESP_RST, OUTPUT); digitalWrite(ESP_RST, LOW);
  pinMode(ESP_PWR, OUTPUT); digitalWrite(ESP_PWR, HIGH);
  delay(300);
  pinMode(ESP_RST, OUTPUT); digitalWrite(ESP_RST,HIGH);
}

void flashESPMode()
{
  powerOnESP02ForProgramming();
  Serial2.begin(FLASH_ESP_BAUD);
  Serial.begin(FLASH_ESP_BAUD);
  while(1)
  {
    if (Serial2.available())
      Serial.write(Serial2.read());
    if (Serial.available())
      Serial2.write(Serial.read());
  }
}

