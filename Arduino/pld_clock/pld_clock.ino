#include <PLDuino.h>
#include <Time.h>
#include <TimeLib.h>
#include <DS3232RTC.h>
#include <Wire.h>
using namespace PLDuino;


// Utility function which prepares a number for printing
// by padding it with leading zeros until width is reached
String zeropad (long num, int width)
{
  String res = String(num);
  while (res.length() < width)
    res = "0" + res;
  return res;
}


// Returns textual representation of time in "MM/DD/YYYY hh:mm" format
String getDateTimeStr()
{
  return zeropad(month(),2) + "/" + zeropad(day(),2) + "/" + zeropad(year(),4)
       + " "
       + zeropad(hour(),2) + ":" + zeropad(minute(),2);
}


void setup()
{
  // Initializing Serial.
  Serial.begin(115200);
  
  // A necessary call.
  // It sets pin modes according to their function.
  PLDuino::init();

  // Set up RTC clock.
  setSyncProvider(RTC.get);
  if (timeStatus() != timeSet)
  {
    // Seems that the hardware is broken, or disconnected for some reason.
    Serial.println("Unable to sync with RTC. Please check if it is connected properly.");
  }

  // Let's print current time
  Serial.println("Current time: " + getDateTimeStr());
  delay(1000);

  // Adjust the clock. 10/26/2015 07:28 is used just for demonstration.
  // You can, for example, receive it via ESP's Wi-Fi.
  Serial.print("Setting clock to 10/26/2015 07:28... ");
  tmElements_t tm;
  const int y = 2015;
  tm.Year = (y>=1000? CalendarYrToTm(y) : y2kYearToTm(y));
  tm.Month = 10;
  tm.Day = 26;
  tm.Hour = 7;
  tm.Minute = 28;
  tm.Second = 0;
  time_t t = makeTime(tm);
  RTC.set(t); setTime(t);
  Serial.println("Done.");

  // Print adjusted time
  Serial.println("Updated time: " + getDateTimeStr());
}

void loop()
{
}

