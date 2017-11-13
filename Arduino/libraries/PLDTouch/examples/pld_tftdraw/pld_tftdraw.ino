#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <PLDuino.h>
#include <PLDTouch.h>

Adafruit_ILI9341 tft = Adafruit_ILI9341(PLDuino::LCD_CS, PLDuino::LCD_DC);
PLDTouch touch(PLDuino::TOUCH_CS, PLDuino::TOUCH_IRQ);


void setup()
{
  // Set pin modes and initialize stuff
  // NB: This line is necessary in all sketches which use PLDuino library stuff.
  PLDuino::init();
  
  // Power-on the LCD and set it up
  PLDuino::enableLCD();
  tft.begin();
  tft.setRotation(3);

  // Initialize the touch screen
  touch.init(PLDTOUCH_ROTATE_180);

  // Clear the screen.
  tft.fillScreen(ILI9341_BLACK);
}

Point prev_pt;
void loop()
{
  // Check if user touched the screen
  if (touch.dataAvailable())
  {
    // Read touch position
    Point pt = touch.read();

    // Draw a line between current touch and previous touch, if any
    if (prev_pt.isValid() && pt.isValid())
      tft.drawLine(
        prev_pt.x, prev_pt.y,
        pt.x, pt.y,
        ILI9341_WHITE
      );
    
    // Update previous touch position. Now it is current touch.
    prev_pt = pt;
  }
  else
  {
    // Screen isn't touched currently. 
    // Clear previous touch position.
    if (prev_pt.isValid())
      prev_pt = Point();
  }
}
