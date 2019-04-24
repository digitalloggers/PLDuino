#ifdef ARDUINO_ARCH_ESP8266
#include "SimpleCommandProcessor.h"
#include "ESP8266WebServer.h"

String dins_state = "HHHHHHHH";

SimpleCommandProcessor processor(
  [](const String &cmd) -> bool {
    if (cmd.startsWith("DINS")) {
      dins_state = cmd.substring(4); 
      return true;
    } else {
      return false;
    }
  },
  [](const String &data) {
    // send data between commands back to 2560
    Serial.print(data);
  }
);


void connectWiFi();
void setupHTTP();
void processHTTP();

void setup() {
  delay(250);
  Serial.begin(115200); Serial.println();
  connectWiFi();
  setupHTTP();
}

void loop() {
  processHTTP();
  if (Serial.available()) {
    processor.analyze(String(
      (char)Serial.read()
    ));
  }
}

#endif
