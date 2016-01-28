#include <PLDuino.h>
#include <Time.h>
#include <DS3232RTC.h>
#include <Wire.h>

String zpad (long num, int width)
{
  String res = String(num);
  while(res.length() < width)
    res = "0" + res;
  return res;
}

const char time_packet_example[] = "2013" "01" "15" "T" "20" "30" "40"; // ISO format
const int TIME_PACKET_SIZE = sizeof(time_packet_example)-1; // -1 is NULL terminator

void setup()
{
  Serial.begin(9600);
  PLDuino::init();
  setSyncProvider(RTC.get);
  if (timeStatus() != timeSet)
    while(true) Serial.println("ERR ");

  String buf = "";
  while(true)
  {
    if (Serial.available())
      buf += String((char)Serial.read());
    while(buf.length() >= TIME_PACKET_SIZE)
    {
      // Accept a string in "yyyymmddThhmmss" format.
      tmElements_t tm;
      {
        // Fill the structure.
        int y = buf.substring(0, 4).toInt();
        tm.Year = (y>=1000? CalendarYrToTm(y) : y2kYearToTm(y));
        tm.Month = buf.substring(4, 6).toInt();
        tm.Day = buf.substring(6, 8).toInt();
        tm.Hour = buf.substring(9, 11).toInt();
        tm.Minute = buf.substring(11, 13).toInt();
        tm.Second = buf.substring(13, 15).toInt();
      }
      // Set the time.
      time_t t = makeTime(tm);
      RTC.set(t); setTime(t);

      // Respond with (newly set) current time re-read from RTC (same format).
      Serial.print(
        zpad(year(), 4) +
        zpad(month(), 2) +
        zpad(day(), 2) +
        "T" +
        zpad(hour(), 2) +
        zpad(minute(), 2) +
        zpad(second(), 2)
      );

      // Remove current packet.
      buf = buf.substring(TIME_PACKET_SIZE, buf.length());
    }
  }
}

void loop()
{
}

