#include <Arduino.h>
#include <PLDuinoGUI.h>
#include <PLDTouch.h>
#include "utils.h"

#define ON_EXIT_BLOCK class OnExit { public: ~OnExit() {
#define ON_EXIT_BLOCK_END }} __dummy_on_exit__;

RunMode showModeSelectionScreen (RunMode default_mode)
{
  ON_EXIT_BLOCK
    tft.fillScreen(ILI9341_BLACK);
  ON_EXIT_BLOCK_END
  
  Button btnDemo("DEMO", ILI9341_GREEN, ILI9341_BLACK, ILI9341_BLACK),
         btnTest("TEST", ILI9341_RED, ILI9341_WHITE, ILI9341_BLACK),
         btnWiFi("Wi-Fi", ILI9341_CYAN, ILI9341_BLACK, ILI9341_BLACK);
  Label lblCounter("10 sec");
  Placeholder dummy;
  Sizable *allctls[] = {
    &btnDemo, (default_mode==MODE_DEMO? (Sizable *)&lblCounter : (Sizable *)&dummy),
    &btnTest, (default_mode==MODE_DEMO? (Sizable *)&dummy : (Sizable *)&lblCounter),
    &btnWiFi, &dummy,
    NULL
  };
  GridLayout(allctls, 2, 20, 0).setPositionAndSize(10, 10, 300, 220);
  
  btnDemo.draw(tft);
  btnTest.draw(tft);
  btnWiFi.draw(tft);
  lblCounter.draw(tft);
  
  unsigned long start_time = millis();
  while(true)
  {
    int seconds_passed = (millis() - start_time) / 1000;
    lblCounter.updateText(String(max(10-seconds_passed, 0)) + " sec.", tft);
    if (seconds_passed >= 10) return default_mode;
    
    if (touch.dataAvailable())
    {
      Point pt = touch.read();
      if (btnDemo.isTouched(pt.x, pt.y)) return MODE_DEMO;
      if (btnTest.isTouched(pt.x, pt.y)) return MODE_TEST;
      if (btnWiFi.isTouched(pt.x, pt.y)) return MODE_WIFI;
    }
    delay(5);
  }
}

