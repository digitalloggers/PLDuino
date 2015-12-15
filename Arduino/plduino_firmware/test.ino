#include <SD.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <PLDuino.h>
#include <PLDTouch.h>
#include <PLDuinoGUI.h>
using namespace PLDuinoGUI;


void test()
{
  tft.fillScreen(ILI9341_BLACK);
  testSD(); tft.fillScreen(ILI9341_BLACK);
  testClock(); tft.fillScreen(ILI9341_BLACK);
  testInputs(); tft.fillScreen(ILI9341_BLACK);
  testRelays(); tft.fillScreen(ILI9341_BLACK);
  testDOUTs(); tft.fillScreen(ILI9341_BLACK);
  testSound(); tft.fillScreen(ILI9341_BLACK);
  testUARTLoopback(); tft.fillScreen(ILI9341_BLACK);
  testWiFi();
}

