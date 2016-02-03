#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <PLDuino.h>
#include <PLDTouch.h>
#include <PLDuinoGUI.h>
#include <using_namespace_PLDuinoGUI.h>
#include <DS3232RTC.h>
#include <Time.h>
#include <Wire.h>
#include "utils.h"

void showRelays (unsigned long timeout_seconds)
{
  bmpDraw(tft, "relays.bmp", 0, 0);
  Label lblBack("", ILI9341_RED);
  lblBack.setPositionAndSize(111, 195, 100, 35);
  
  Label lblRelay[] = {
    Label("--", ILI9341_WHITE, tft.color565(200, 200, 200), HORZ_CENTER|VERT_CENTER),
    Label("--", ILI9341_WHITE, tft.color565(200, 200, 200), HORZ_CENTER|VERT_CENTER),
    Label("--", ILI9341_WHITE, tft.color565(200, 200, 200), HORZ_CENTER|VERT_CENTER),
    Label("--", ILI9341_WHITE, tft.color565(200, 200, 200), HORZ_CENTER|VERT_CENTER),
    Label("--", ILI9341_WHITE, tft.color565(200, 200, 200), HORZ_CENTER|VERT_CENTER),
    Label("--", ILI9341_WHITE, tft.color565(200, 200, 200), HORZ_CENTER|VERT_CENTER),
  };
  Sizable *allctls[] = {
    &lblRelay[0], &lblRelay[1], &lblRelay[2],
    &lblRelay[3], &lblRelay[4], &lblRelay[5],
    NULL
  };
  GridLayout(allctls, 3, 0, 0).setPositionAndSize(0, 60-25/2, 320, 130+25);
  
  for(int i=0; allctls[i]; ++i)
    ((Label *)allctls[i])->draw(tft);
  
  bool soundPlayed = false;
  unsigned long starttime = millis();
  while(true)
  {
    if (touch.dataAvailable())
    {
      starttime = millis();
      
      Serial.println("ready");
      Point pt = touch.read();
      int x = pt.x;
      int y = pt.y;
      Serial.print("x = "); Serial.print(x); Serial.print("; "); Serial.print("y = "); Serial.println(y);
      
      tft.drawPixel(x, y, ILI9341_RED);
      
      if (lblBack.isTouched(x, y))
        break;

      for(int i=0; i<6; ++i)
        if (lblRelay[i].isTouched(x, y))
          digitalWrite(PLDuino::RELAY1+i, (digitalRead(PLDuino::RELAY1+i)==HIGH? LOW : HIGH));
      
      while(touch.dataAvailable()) touch.read();
    }

    // TODO: extract a class to remove code duplication (see demo_inputs).
    // Now Arduino IDE simply doesn't provide any tools needed to organize code.
    if (millis() - starttime >= 1000*timeout_seconds)
      break;

    for(int i=0; i<6; ++i)
    {
      bool switched_on = (digitalRead(PLDuino::RELAY1+i) == HIGH);
      lblRelay[i].updateTextAndColor(
        (switched_on? "ON" : "OFF"),
        (switched_on? ILI9341_RED : ILI9341_BLACK),
        tft
      );
    }
    
    if (!soundPlayed)
    {
      playSound(tmrpcm, "part6.wav");
      soundPlayed = true;
    }
    
    delay(50);
  }
  tft.fillScreen(ILI9341_BLACK);
}

