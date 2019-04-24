#include <Arduino.h>
#include <FS.h>
#include "Settings.h"


bool Settings::save (const String &filename) {
  File f = SPIFFS.open(SETTINGS_FILENAME, "w");
  f.println(getWifiSSID());
  f.println(getWifiPassword());
  f.println(isWifiAP());
  for(int i=0; i<6; ++i) {
    f.println(getRelayName(i));
    f.println(getRelayEchoName(i));
    f.println(getRelayEchoOn(i));
  }
  for(int i=0; i<8; ++i)
    f.println(getDOUTName(i));
  for(int i=0; i<8; ++i)
    f.println(getDINName(i));
  for(int i=0; i<8; ++i)
    f.println(getAINName(i));
  f.close();
  
  return true;
}


String readStr (File &f) {
  String res = "";
  while(true) {
    int b = f.read();
    if (b == -1) return res;
    
    char c = b;
    if (c == 13 || c == 10) {
      char nextch = f.peek();
      if (nextch!=c && (nextch==10 || nextch==13))
        f.read();
      return res;
    } else {
      res += c;
    }
  }
}

bool Settings::load (const String &filename) {
  File f = SPIFFS.open(SETTINGS_FILENAME, "r");
  if (!f) return false;
  
  setWifiSSID(readStr(f));
  setWifiPassword(readStr(f));
  setWifiAP(readStr(f) == "1");
  for(int i=0; i<6; ++i) {
    setRelayName(i, readStr(f));
    setRelayEchoName(i, readStr(f));
    setRelayEchoOn(i, readStr(f) == "1");
  }
  for(int i=0; i<8; ++i)
    setDOUTName(i, readStr(f));
  for(int i=0; i<8; ++i)
    setDINName(i, readStr(f));
  for(int i=0; i<8; ++i)
    setAINName(i, readStr(f));
  f.close();
  
  return true;
}

