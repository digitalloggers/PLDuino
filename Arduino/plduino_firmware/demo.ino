#include <TMRpcm.h>
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


void showSlide (const char *picfile, const char *wavfile)
{
  long start = millis();
  bmpDraw(picfile, 0, 0);
  playSound(wavfile);
}

#define NORMAL_TIMEOUT        60
#define PRESENTATION_TIMEOUT   5

void demo()
{
  bool touched = true;
  for(;;)
  {
    tft.fillScreen(ILI9341_BLACK);
    showSlide("slide1.bmp", "part1.wav");
    showSlide("slide2.bmp", "part2.wav");
    showSlide("slide3.bmp", "part3.wav"); touched = waitForTouchOrTimeout(touched? NORMAL_TIMEOUT : PRESENTATION_TIMEOUT);
    showInputs(touched? NORMAL_TIMEOUT : PRESENTATION_TIMEOUT);
    showSlide("slide5.bmp", "part5.wav"); touched = waitForTouchOrTimeout(touched? NORMAL_TIMEOUT : PRESENTATION_TIMEOUT);
    showRelays(touched? NORMAL_TIMEOUT : PRESENTATION_TIMEOUT);
    showSlide("features.bmp", "part7.wav");
  }
}

