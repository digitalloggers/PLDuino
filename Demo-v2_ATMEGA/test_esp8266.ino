#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <PLDuino.h>
#include <DS3232RTC.h>
#include <Time.h>
#include <Wire.h>
#include <SimpleCommandProcessor.h>
#include "utils.h"
using namespace PLDuino;
HardwareSerial &wifi = Serial2;

bool cmd_sendState()
{
  // an equivalent to (sstream << setw(width) << setfill(fill) << value).str()
  auto fmt = [](int value, int width, char fill='0') {
    String r = String(value);
    while(r.length() < width)
      r = fill + r;
    return r;
  };

  for(int i=0; i<6; ++i)
    wifi.write(digitalRead(RELAY1+i)==HIGH? "1" : "0");
  for(int i=0; i<8; ++i)
    wifi.write(digitalRead(30+i)==HIGH? "1" : "0");
  for(int i=0; i<8; ++i)
    wifi.write(digitalRead(38+i)==HIGH? "1" : "0");
  for(int i=0; i<8; ++i)
    wifi.write(fmt(analogRead(A0+i),4).c_str());
  wifi.write((
      fmt(hour(),2) +
      fmt(minute(), 2)
    ).c_str());
  wifi.write((
      fmt(year(), 4) +
      fmt(month(), 2) +
      fmt(day(), 2)
    ).c_str());
  wifi.write("e");
  wifi.flush();
  return true;
}

bool cmd_adjustTime (const String &t)
{
  if (t.length() == String("19990101T1010").length() && t[8]=='T')
  {
    tmElements_t tm; {
      int y = t.substring(0, 4).toInt();
      tm.Year = (y>=1000? CalendarYrToTm(y) : y2kYearToTm(y));
      tm.Month = t.substring(4, 6).toInt();
      tm.Day = t.substring(6, 8).toInt();
      tm.Hour = t.substring(9, 11).toInt();
      tm.Minute = t.substring(11, 13).toInt();
      tm.Second = 0;
    }
    time_t t = makeTime(tm);
    RTC.set(t); setTime(t);
    Serial.println("\n\nTime set!\n\n");
  }
  else
  {
    Serial.println("\n\nCan't set time!\n\n");
  }
  return true;
}

bool cmd_processSwitchCommand (const String &cmd)
{
  char op = cmd[0];
  int index = cmd[1] - '1';
  if (index < 0) return false;
  switch(op)
  {
    case 'r':
    case 'R':
      if (index >= 6) { return false; }
      Serial.println("relay" + String(index) + (op=='R'? "ON" : "off"));
      digitalWrite(RELAY1+index, (op=='R'? HIGH : LOW));
      return true;
      break;
    case 'o':
    case 'O':
      if (index >= 8) { return false; }
      Serial.println("dout" + String(index) + (op=='O'? "ON" : "off"));
      digitalWrite(38+index, (op=='O'? HIGH : LOW));
      return true;
      break;
  }
  return false;
}

SimpleCommandProcessor cmdprocessor(
  [](const String &cmd) -> bool {
    if (cmd == "state")
      return cmd_sendState();
    else if (cmd.substring(0, 4) == "time")
      return cmd_adjustTime(cmd.substring(4));
    else if (cmd.length() == 2) {
      tft.print(cmd);
      return cmd_processSwitchCommand(cmd);
    } else {
      Serial.println("\nbadcmd: " + cmd);
      return false;
    }
  },
  [](const String &data) {
    Serial.print(data);      
  }
);


#define CHUNK_SIZE 200

class ChunkedReader {
public:
  ChunkedReader (HardwareSerial *serialport, long time_out, void (*fn_out)(const char *chunk))
    : chunklen(0),
      last_time(millis()),
      timeout(time_out),
      serial(serialport),
      out(fn_out)
  {}
  void read();
private:
  long timeout;
  long last_time;
  int chunklen;
  char chunk[CHUNK_SIZE+1];
  HardwareSerial *serial;
  void (*out)(const char *chunk);
};

void ChunkedReader::read() {
  if (serial->available()) {
    chunk[chunklen] = (char)serial->read();
    chunk[chunklen+1] = 0;
    ++chunklen;
  }
  if (chunklen==CHUNK_SIZE || (chunklen>0 && millis()-last_time>1000)) {
    out(chunk);
    last_time = millis();
    chunklen = 0;
  }
}


void testWiFi()
{
  // Setup screen
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(3);
  tft.println("Wi-Fi test.");
  tft.setTextSize(1);
  tft.println();

  // Print Wi-Fi credentials for convenience
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_GREEN);  tft.println("Default settings:");
  tft.setTextColor(ILI9341_WHITE);  tft.print("1.Connect to \"");
  tft.setTextColor(ILI9341_YELLOW); tft.print("Wi-Fi Test");
  tft.setTextColor(ILI9341_WHITE);  tft.println("\"");
  tft.setTextColor(ILI9341_WHITE);  tft.print("2.Use password \"");
  tft.setTextColor(ILI9341_YELLOW); tft.print("password");
  tft.setTextColor(ILI9341_WHITE);  tft.println("\"");
  tft.setTextColor(ILI9341_WHITE);  tft.print("3.Open URL: ");
  tft.setTextColor(ILI9341_YELLOW); tft.println("192.168.4.1");
  
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(1);
  tft.println();
  
  tft.print("Initializing ESP8266... ");
  PLDuino::enableESP();
  PLDuino::resetESP();
  wifi.begin(115200);
  tft.println("done.");
  tft.println();

  ChunkedReader reader(&wifi, 1000, [](const char *chunk) {
    cmdprocessor.analyze(chunk);
  });
  while(true) {
    reader.read();
  }
}

