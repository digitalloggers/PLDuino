#ifdef ARDUINO_ARCH_ESP8266
#include "ESP8266WebServer.h"

void connectWiFi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.softAPdisconnect(true);
  WiFi.begin(WIFI_SSID, WIFI_KEY);
  while(WiFi.status()!=WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to " WIFI_SSID);
  Serial.println(WiFi.localIP());
}

#endif
