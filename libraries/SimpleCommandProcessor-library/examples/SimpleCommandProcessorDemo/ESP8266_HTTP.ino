#ifdef ARDUINO_ARCH_ESP8266
#include "ESP8266WebServer.h"

ESP8266WebServer http(80);

void setupHTTP() {
  Serial.println("Setting up HTTP...");
  http.on("/", [](){
    http.send(200, "text/html", INDEX_HTML);
  });
  http.on("/index.js", [](){
    http.send(200, "text/javascript", INDEX_JS);
  });
  http.on("/state", [](){
    http.send(200, "application/json",
      "{ \"dins\": \"" + dins_state + "\" }"
    );
  });
  http.begin();
}

void processHTTP() {
  http.handleClient();
}

#endif
