#ifdef ARDUINO_ARCH_AVR
#include <PLDuino.h>
#include "SimpleCommandProcessor.h"

void setup() {
  PLDuino::init();
  PLDuino::enableESP();
  Serial2.begin(115200);
  Serial.begin(115200);
  pinMode(30, INPUT);
  digitalWrite(30, HIGH);
}

void loop() {
  while(Serial2.available()) Serial.write(Serial2.read());
  while(Serial.available()) Serial2.write(Serial.read());
  delay(100);
  
  // Send current state to ESP8266
  String state = "";
  for(int i=0; i<8; ++i)
    state += (digitalRead(30+i)==HIGH? "H" : "L");
  Serial2.print(SimpleCommandProcessor::makeCommand("DINS" + state));
}

#endif

