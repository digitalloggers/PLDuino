#include <Arduino.h>
#include <PLDuino.h>
#define ESP_BAUD 115200

String getResponse()
{
  String response = "";
  for(int i=0; i<10; ++i)
  {
    while(Serial2.available())
      response += char(Serial2.read());
    delay(100);
  }
  return response;
}


bool waitUntilStringReceived (String ptn, int timeout)
{
  String response = "";
  long start_time = millis();
  while(millis() - start_time < timeout)
  {
    if (Serial2.available())
    {
      response += (char)Serial2.read();
      if (response.length() >= ptn.length())
      {
        response = response.substring(response.length() - ptn.length());
        if (response == ptn) return true;
      }
    }
  }
  return false;
}

void cancelWiFiTestStartup()
{
  if (waitUntilStringReceived("type \"qw\"", 10000))
  {
    Serial.println("sending qw...");
    Serial2.println("qwqwqw;\n");
    Serial.flush();
  }
  Serial2.flush();
}

void setup()
{
  using namespace PLDuino;
  
  PLDuino::init();
  enableESP();
  Serial2.begin(ESP_BAUD);
  Serial.begin(ESP_BAUD);
  cancelWiFiTestStartup();

  while(1)
  {
    if (Serial2.available()) Serial.write(Serial2.read());
    if (Serial.available()) Serial2.write(Serial.read());
  }
}

void loop(){}

