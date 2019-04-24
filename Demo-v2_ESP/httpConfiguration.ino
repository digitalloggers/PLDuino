void setupWeMo();


void initializeWiFi();

void httpSetConfiguration() {
  {
    auto fmt = [](String value, int width, char fill='0') {
      String r = value;
      while(r.length() < width)
        r = fill + r;
      return r;
    };
  
    if (http.hasArg("datetime-year") &&
        http.hasArg("datetime-month") &&
        http.hasArg("datetime-day") &&
        http.hasArg("datetime-hour") &&
        http.hasArg("datetime-minute"))
    {
      Serial.println("{{~time" + 
        fmt(http.arg("datetime-year"), 4) + 
        fmt(http.arg("datetime-month"), 2) +
        fmt(http.arg("datetime-day"), 2) +
        "T" +
        fmt(http.arg("datetime-hour"), 2) +
        fmt(http.arg("datetime-minute"), 2) +
        "~}}"
      );
    }
  }
  {
    for(int i=0; i<http.args(); ++i)
      Serial.println(http.argName(i) + "=" + http.arg(i));
    bool wifi_changed = false;
    if (http.hasArg("wifi-ssid")) {
      settings.setWifiSSID(http.arg("wifi-ssid"));
      wifi_changed = true;
    }
    if (http.hasArg("wifi-password")) {
      settings.setWifiPassword(http.arg("wifi-password"));
      wifi_changed = true;
    }
    if (http.hasArg("wifi-mode") && http.arg("wifi-mode")=="wifi-mode-ap") {
      Serial.println("Switching to AP");
      settings.setWifiAP(true);
      wifi_changed = true;
    }
    if (http.hasArg("wifi-mode") && http.arg("wifi-mode")=="wifi-mode-sta") {
      Serial.println("Switching to STA");
      settings.setWifiAP(false);
      wifi_changed = true;
    }
    settings.save(SETTINGS_FILENAME);
    if (wifi_changed)
      initializeWiFi();
  }
  for(int i=0; i<6; ++i) {
    String arg_name = "relay-name-" + String(i+1);
    if (http.hasArg(arg_name)) {
      settings.setRelayName(i, http.arg(arg_name));
      String arg_echo_name = "relay-echo-name-" + String(i+1);
      if (http.hasArg(arg_echo_name))
        settings.setRelayEchoName(i, http.arg(arg_echo_name));
      String arg_echo_enabled = "relay-echo-enabled-" + String(i+1);
      settings.setRelayEchoOn(i, http.hasArg(arg_echo_enabled)? (http.arg(arg_echo_enabled)=="on") : false);
    }
  }
  for(int i=0; i<8; ++i) {
    String arg_name = "din-name-" + String(i+1);
    if (http.hasArg(arg_name))
      settings.setDINName(i, http.arg(arg_name));
  }
  for(int i=0; i<8; ++i) {
    String arg_name = "dout-name-" + String(i+1);
    if (http.hasArg(arg_name))
      settings.setDOUTName(i, http.arg(arg_name));
  }
  for(int i=0; i<8; ++i) {
    String arg_name = "ain-name-" + String(i+1);
    if (http.hasArg(arg_name))
      settings.setAINName(i, http.arg(arg_name));
  }
  setupWeMo();
  settings.save(SETTINGS_FILENAME);
  http.sendHeader("Location", "/settings.html", true);
  http.send(302, "text/plain", "");
}


void httpConfiguration() {
  String out = "";
  out += "{\n";
    out += "\"relay_name\": ["; for(int i=0; i<6; ++i) out += String(i? "," : "") + "\"" + settings.getRelayName(i) + "\""; out += "],\n";
    out += "\"relay_echo_name\": ["; for(int i=0; i<6; ++i) out += String(i? "," : "") + "\"" + settings.getRelayEchoName(i) + "\""; out += "],\n";
    out += "\"relay_echo_on\": ["; for(int i=0; i<6; ++i) out += String(i? "," : "") + "\"" + (settings.getRelayEchoOn(i)? "1" : "0") + "\""; out += "],\n";
    out += "\"ain_name\": ["; for(int i=0; i<8; ++i) out += String(i? "," : "") + "\"" + settings.getAINName(i) + "\""; out += "],\n";
    out += "\"din_name\": ["; for(int i=0; i<8; ++i) out += String(i? "," : "") + "\"" + settings.getDINName(i) + "\""; out += "],\n";
    out += "\"dout_name\": ["; for(int i=0; i<8; ++i) out += String(i? "," : "") + "\"" + settings.getDOUTName(i) + "\""; out += "],\n";
    out += "\"wifi\": {\n"
      "\"ssid\": \"" + settings.getWifiSSID() + "\",\n"
      "\"password\": \"" + settings.getWifiPassword() + "\",\n"
      "\"mode\": \"" + (settings.isWifiAP()? "ap" : "sta") + "\"\n"
    "}\n";
  out += "}";
  http.send(200, "text/javascript", out);  
}

