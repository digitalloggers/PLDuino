#include <PLDuino.h>
#include <PLDuinoGUI.h>
#include <TMRpcm_PLDuino.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <PLDuino.h>
#include <PLDTouch.h>
#include <PLDuinoGUI.h>
#include <using_namespace_PLDuinoGUI.h>
#include <DS3232RTC.h>
#include <TimeLib.h>
#include <Wire.h>
#include <avr/io.h>

Adafruit_ILI9341 tft = Adafruit_ILI9341(PLDuino::LCD_CS, PLDuino::LCD_DC);
PLDTouch touch(PLDuino::TOUCH_CS, PLDuino::TOUCH_IRQ);


void resetESP()
{
  digitalWrite(PLDuino::ESP_RST, LOW);
  delay(1000);
  digitalWrite(PLDuino::ESP_RST, HIGH);
}


void showFlashControls()
{
  Button btnFlash("Flash", ILI9341_DARKGREY, ILI9341_BLACK);
  btnFlash.setColor(ILI9341_BLUE);
  btnFlash.setPositionAndSize(10, 10, 145, 40);
  btnFlash.draw(tft);

  Button btnLua("Lua", ILI9341_YELLOW, ILI9341_BLACK);
  btnLua.setPositionAndSize(165, 10, 145, 40);
  btnLua.draw(tft);

  Button btnReset("RESET ESP", ILI9341_RED, ILI9341_MAROON);
  btnReset.setPositionAndSize(10, 190, 130, 40);
  btnReset.draw(tft);

  Button btnQuit("QUIT", ILI9341_GREEN, ILI9341_DARKGREEN);
  btnQuit.setPositionAndSize(210, 190, 100, 40);
  btnQuit.draw(tft);

  Label lblNote("", ILI9341_WHITE, ILI9341_BLACK, HORZ_LEFT|VERT_TOP);
  lblNote.setPositionAndSize(10, 60, 300, 120);
  lblNote.draw(tft);

  bool progMode = false, lastProgMode = !progMode;
  while(true)
  {
    if (touch.dataAvailable())
    {
      Point pt = touch.read();
      int x = pt.x;
      int y = pt.y;
      
      if (btnReset.isTouched(x, y))
      {
        btnReset.setColor(ILI9341_DARKGREY);
        btnReset.setTextColor(ILI9341_BLACK);
        btnReset.setText("WAIT...");
        btnReset.draw(tft);
        resetESP();
        btnReset.setColor(ILI9341_RED);
        btnReset.setTextColor(ILI9341_MAROON);
        btnReset.setText("RESET ESP");
        btnReset.draw(tft);
      }
      
      if (btnLua.isTouched(x, y))
        progMode = false;
        
      if (btnFlash.isTouched(x, y))
        progMode = true;

      if (btnQuit.isTouched(x, y))
        break;
    }

    if (lastProgMode != progMode)
    {
      if (progMode)
      {
        digitalWrite(PLDuino::ESP_GPIO0, LOW);
        Serial.begin(115200);
        Serial2.begin(115200);
      }
      else
      {
        digitalWrite(PLDuino::ESP_GPIO0, HIGH);
        Serial.begin(115200);
        Serial2.begin(115200);
      }

      lblNote.updateText(
        progMode?
          "GPIO is low now, ESP has  been reset. You can pro-  gram it on 115200 rate."
        : "GPIO is high, ESP has     been reset. You can work with NodeMCU now on 115200 baud rate.",
        tft
      );
      
      btnFlash.setColor(progMode? ILI9341_YELLOW : ILI9341_DARKGREY);
      btnFlash.draw(tft);

      btnLua.setColor(!progMode? ILI9341_YELLOW : ILI9341_DARKGREY);
      btnLua.draw(tft);

      resetESP();
      
      lastProgMode = progMode;
    }

    if (Serial2.available()) Serial.write(Serial2.read());
    if (Serial.available()) Serial2.write(Serial.read());
  }
}


void show2400msg()
{
  tft.fillScreen(ILI9341_BLACK);

  Label lbl2400(
    "Don't  forget  to  disable"
    "auto-reset feature by con-"
    "necting to the PLC at 2400"
    "before continue.          "
    "                          "
    "Tap screen if ready.",
    ILI9341_WHITE
  );
  lbl2400.setPositionAndSize(0,10,280,20);
  lbl2400.draw(tft);
}

void show4800msg()
{
  Label lbl4800(
    "Now  turn  auto-reset  on "
    "by connecting  to the PLC "
    "on 4800 baud rate (if you "
    "need  to  upload  Arduino "
    "sketches).",
    ILI9341_WHITE
  );
  lbl4800.setPositionAndSize(0,10,280,20);
  lbl4800.draw(tft);
}


void setup()
{
  PLDuino::init();
  PLDuino::enableLCD();
  PLDuino::enableESP();
  tft.begin();
  tft.setRotation(3);
  touch.init(1);
  Serial.begin(115200);
  Serial2.begin(115200);

  show2400msg();
  while(touch.dataAvailable()) touch.read();
  while(!touch.dataAvailable()); touch.read();
  tft.fillScreen(ILI9341_BLACK);

  showFlashControls();
  tft.fillScreen(ILI9341_BLACK);

  show4800msg();
  while(1);
}

void loop() {}
