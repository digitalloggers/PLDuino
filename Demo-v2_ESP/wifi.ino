void setupAP (const String &ssid, const String &password) {
  //if (WiFi.isConnected()) { 
  WiFi.disconnect();
  
  WiFi.softAP(ssid.c_str(), password.c_str());
  
  Serial.print("AP,");
  Serial.print(ssid); Serial.print(",");
  Serial.print(password); Serial.print(",");
  Serial.println(WiFi.softAPIP());
  Serial.flush();
}

void initializeWiFi() {
  if (settings.isWifiAP()) {
    WiFi.disconnect();
    setupAP(
      settings.getWifiSSID(),
      settings.getWifiPassword()
    );
  }
  else {
    WiFi.softAPdisconnect(true);
    WiFi.begin(
      settings.getWifiSSID().c_str(),
      settings.getWifiPassword().c_str()
    );

    for(int i=0; WiFi.status()!=WL_CONNECTED; ++i) {
      if (i > 5*60*10) {
        Serial.println("Can't connect to Wi-Fi hotspot. Falling back to default AP.");
        setupAP("Wi-Fi Test", "password");
        return;
      }
      delay(100);
      if (i%10==0) { 
        Serial.print(".");
        Serial.flush();
      }
    }
    
    Serial.print("STA,");
    Serial.print(settings.getWifiSSID()); Serial.print(",");
    Serial.print(settings.getWifiPassword()); Serial.print(",");
    Serial.println(WiFi.localIP());
    Serial.flush();
  }
}


void setupWeMo() {
  upnp.removeAllDevices();
  uint32_t device_index = 0;
  for(int i=0; i<6; ++i)
    if (settings.getRelayEchoOn(i)) {
      uint32_t port = 50000 + i;
      upnp.addDevice(std::shared_ptr<WeMoSwitchEmulator>(new WeMoSwitchEmulator(
        port,
        ESP.getChipId()+port,
        settings.getRelayEchoName(i),
        [i](bool newstate) {
          Serial.print(SimpleCommandProcessor::makeCommand(
            String(newstate? "R" : "r") + String(i+1)
          ));
        }
      )));
      ++device_index;
    }
}

