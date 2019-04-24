#include <Arduino.h>
#include <PLDuinoGUI.h>
#include <PLDTouch.h>
#include "utils.h"

#define WAIT_UNTIL_DEFAULT_MODE 15
#define SPLASH_BGR "/splash.bmp"

class GearsAnimation
{
public:
  const int FPS = 7;
  const int FRAME_TIME = 1000 / FPS;
public:
  GearsAnimation(int x, int y);
  void update();
private:
  static const int NUM_FRAMES = 3;
  void drawFrame(int frame) const;
  unsigned long prev_time_;
  int frame_;
  int x_, y_;
};

GearsAnimation::GearsAnimation(int x, int y)
  : x_(x), y_(y),
    frame_(0),
    prev_time_(millis() - FRAME_TIME)
{
}

void GearsAnimation::drawFrame (int frame) const
{
  bmpDraw(tft, ("/gears/gears" + String(frame*2+0) + ".bmp").c_str(), x_+13, y_);
  bmpDraw(tft, ("/gears/gears" + String(frame*2+1) + ".bmp").c_str(), x_, y_+13);
}

void GearsAnimation::update()
{
  long cur_time = millis();
  if (cur_time - prev_time_ >= FRAME_TIME)
  {
    prev_time_ = cur_time;
    frame_ = (frame_ + 1) % NUM_FRAMES;
    drawFrame(frame_);
  }
}


RunMode showModeSelectionScreen (RunMode default_mode, int wait_until_default = WAIT_UNTIL_DEFAULT_MODE)
{
  bool is_splash_present = SD.exists(SPLASH_BGR);

  Button btnDemo((is_splash_present? "" : "DEMO" ), ILI9341_GREEN , ILI9341_BLACK , ILI9341_WHITE, is_splash_present);
  Button btnTest((is_splash_present? "" : "TEST" ), ILI9341_RED   , ILI9341_WHITE , ILI9341_WHITE, is_splash_present);
  Button btnWiFi((is_splash_present? "" : "Wi-Fi"), ILI9341_CYAN  , ILI9341_BLACK , ILI9341_WHITE, is_splash_present);

  Label lblCounter("", ILI9341_BLACK, ILI9341_WHITE);

  if (is_splash_present)
  {
    // arrange the widgets according to image
    lblCounter.setPositionAndSize(255, (default_mode==MODE_DEMO? 40 : 105), 50, 60);
    btnDemo.setPositionAndSize(75,  40, 165, 60);
    btnTest.setPositionAndSize(75, 105, 165, 60);
    btnWiFi.setPositionAndSize(75, 175, 165, 60);

    // load the background
    bmpDraw(tft, SPLASH_BGR, 0, 0);
  }
  else
  {
    // arrange the widgets automatically into 2x3 grid
    Placeholder dummy;
    Sizable *allctls[] = {
      &btnDemo, (default_mode==MODE_DEMO? (Sizable *)&lblCounter : (Sizable *)&dummy),
      &btnTest, (default_mode==MODE_DEMO? (Sizable *)&dummy : (Sizable *)&lblCounter),
      &btnWiFi, &dummy,
      NULL
    };
    GridLayout(allctls, 2, 20, 0).setPositionAndSize(10, 10, 300, 220);

    // white background if no SPLASH_FILE is on the SD card
    tft.fillScreen(ILI9341_WHITE);
  }

  btnDemo.draw(tft);
  btnTest.draw(tft);
  btnWiFi.draw(tft);
  
  unsigned long start_time = millis();
  RunMode mode = default_mode;
  GearsAnimation gears(196, 123);
  bool radio_light = true;
  while(true)
  {
    int seconds_passed = (millis() - start_time) / 1000;
    lblCounter.updateText(String(max(wait_until_default - seconds_passed, 0)) + "s", tft);
    if (seconds_passed >= wait_until_default) { mode = default_mode; break; }
    
    if (touch.dataAvailable())
    {
      Point pt = touch.read();
      Serial.println("Touch");
      if (btnDemo.isTouched(pt.x, pt.y)) { mode = MODE_DEMO; break; }
      if (btnTest.isTouched(pt.x, pt.y)) { mode = MODE_TEST; break; }
      if (btnWiFi.isTouched(pt.x, pt.y)) { mode = MODE_WIFI; break; }
    }

    if (is_splash_present) {
      radio_light = !radio_light;
      tft.fillCircle(192, 192, 2, radio_light? ILI9341_GREEN : ILI9341_BLACK);
      gears.update();
    }
  }
  tft.fillScreen(ILI9341_BLACK);
  return mode;
}

