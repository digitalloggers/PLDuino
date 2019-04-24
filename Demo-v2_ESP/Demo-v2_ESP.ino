#include <UPnPResponder.h>
#include <WeMoSwitchEmulator.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <functional>
#include <FS.h>
#include <SimpleCommandProcessor.h>
#include "Settings.h"
#include <ESP8266WebFileManager.h>
#include <ESP8266mDNS.h>
void httpState();
void httpSwitchDOUT();
void httpSwitchRelay();
void httpSetConfiguration();
void httpConfiguration();
void httpNotFound();
void initializeWiFi();
void setupWeMo();

#define HOSTNAME "plduino"

ESP8266WebServer http(80);
UPnPResponder upnp(ECHO_IP, ECHO_PORT);
Settings settings;


void handle_file (const String &filename, const String &content_type) {
  Serial.println("streaming " + filename);
  if (SPIFFS.exists(filename)) {
    File file = SPIFFS.open(filename, "r");
    size_t sent = http.streamFile(file, content_type);
    file.close();
  } else {
    http.send(404, "text/plain", "File not found");
  }
}

void handler_index() {
  handle_file("/index.html", "text/html");
}

void startHTTPServer() {
  http.on("/", handler_index); http.on("/index.html", handler_index); http.on("/index.htm", handler_index);
  http.on("/logo.gif", [](){ handle_file("/logo.gif", "image/gif"); });
  http.on("/styles.css", [](){ handle_file("/styles.css", "text/css"); });
  http.on("/settings.html", [](){ handle_file("/settings.html", "text/html"); });
  http.on("/settings.js", [](){ handle_file("/settings.js", "text/html"); });
  http.on("/index.js", [](){ handle_file("/index.js", "text/javascript"); });
  http.on("/util.js", [](){ handle_file("/util.js", "text/javascript"); });
  http.on("/dout", httpSwitchDOUT);
  http.on("/relay", httpSwitchRelay);
  http.on("/set-configuration", httpSetConfiguration);
  http.on("/configuration", httpConfiguration);
  http.on("/state", httpState);
  http.onNotFound(httpNotFound);

  http.begin();
}

void setup() {
  delay(250);
  Serial.begin(115200);
  Serial.println("\n\nStarting ...");
  
  SPIFFS.begin();
  Serial.println("SPIFFS initialized.");
  
  settings.load(SETTINGS_FILENAME);
  Serial.println("Settings loaded.");
  
  initializeWiFi();
  Serial.println("Wi-Fi initialized.");
  
  setupWeMo();
  Serial.println("Echo initialized.");

  //MDNS.begin(HOSTNAME);
  //Serial.println("PLDuino's web gui is at http://" HOSTNAME ".local/");
  
  startHTTPServer();
  Serial.println("HTTP started.");

  ESP8266WebFileManager::begin(9999);
  Serial.println("WebFileManager started on port 9999");

  //MDNS.addService("http", "tcp", 80);
  //MDNS.addService("http", "tcp", 9999);
}

void loop() {
  http.handleClient();
  upnp.handlePackets();
  ESP8266WebFileManager::handleClient();
  delay(10);
}

