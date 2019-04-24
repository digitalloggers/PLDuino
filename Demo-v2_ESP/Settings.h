#pragma once
#include <Arduino.h>


class Settings {
public:
  Settings()
    : relay_name {
        "Relay #1", "Relay #2", "Relay #3",
        "Relay #4", "Relay #5", "Relay #6"
      },
      relay_echo_name {
        "Relay1", "Relay2", "Relay3",
        "Relay4", "Relay5", "Relay6"
      },
      relay_echo_on { 1,1,1,1,1,1 },
      din_name {
        "Input #1", "Input #2", "Input #3", "Input #4",
        "Input #5", "Input #6", "Input #7", "Input #8"
      },
      ain_name {
        "Analog #1", "Analog #2", "Analog #3", "Analog #4",
        "Analog #5", "Analog #6", "Analog #7", "Analog #8"
      },
      dout_name {
        "Output #1", "Output #2", "Output #3", "Output #4",
        "Output #5", "Output #6", "Output #7", "Output #8"
      },
      wifi_ssid("Wi-Fi Test"),
      wifi_password("password"),
      wifi_is_ap(true)
  {
  }
  
  void setRelayName(int index, const String &name) { relay_name[index] = name; }
  const String &getRelayName(int index) const { return relay_name[index]; }
  void setRelayEchoName(int index, const String &name) { relay_echo_name[index] = name; }
  const String &getRelayEchoName(int index) const { return relay_echo_name[index]; }
  void setRelayEchoOn(int index, bool is_on) { relay_echo_on[index] = is_on; }
  bool getRelayEchoOn(int index) const { return relay_echo_on[index]; }
  void setDOUTName(int index, const String &name) { dout_name[index] = name; }
  const String &getDOUTName(int index) const { return dout_name[index]; }
  void setDINName(int index, const String &name) { din_name[index] = name; }
  const String &getDINName(int index) const { return din_name[index]; }
  void setAINName(int index, const String &name) { ain_name[index] = name; }
  const String &getAINName(int index) const { return ain_name[index]; }

  const String &getWifiSSID() const { return wifi_ssid; }
  void setWifiSSID (const String &ssid) { wifi_ssid = ssid; }
  const String &getWifiPassword() const { return wifi_password; }
  void setWifiPassword (const String &pwd) { wifi_password = pwd; }
  bool isWifiAP() const { return wifi_is_ap; }
  void setWifiAP (bool is_ap) { wifi_is_ap = is_ap; }
  
  bool save (const String &filename);
  bool load (const String &filename);
  
private:
  String wifi_ssid;
  String wifi_password;
  bool wifi_is_ap;
  
  String relay_name[6];
  String relay_echo_name[6];
  bool relay_echo_on[6];
  
  String dout_name[8];
  String din_name[8];
  String ain_name[8];
};

#define SETTINGS_FILENAME "/config.txt"

