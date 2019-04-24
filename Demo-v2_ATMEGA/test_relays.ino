#include <Arduino.h>
#include "utils.h"
using namespace PLDuino;

void testRelays()
{
  Label lblBack("[NEXT]", ILI9341_GREEN);
  lblBack.setPositionAndSize(111, 195, 100, 35); lblBack.draw(tft);
  
  Label lblTitle("Relays Test", ILI9341_WHITE);
  lblTitle.setPositionAndSize(0, 0, 320, 40); lblTitle.draw(tft);
  
  Label lblRelay[] = {
    Label("--", ILI9341_WHITE, ILI9341_BLACK, HORZ_CENTER|VERT_CENTER),
    Label("--", ILI9341_WHITE, ILI9341_BLACK, HORZ_CENTER|VERT_CENTER),
    Label("--", ILI9341_WHITE, ILI9341_BLACK, HORZ_CENTER|VERT_CENTER),
    Label("--", ILI9341_WHITE, ILI9341_BLACK, HORZ_CENTER|VERT_CENTER),
    Label("--", ILI9341_WHITE, ILI9341_BLACK, HORZ_CENTER|VERT_CENTER),
    Label("--", ILI9341_WHITE, ILI9341_BLACK, HORZ_CENTER|VERT_CENTER),
  };
  Sizable *allctls[] = {
    &lblRelay[0], &lblRelay[1], &lblRelay[2],
    &lblRelay[3], &lblRelay[4], &lblRelay[5],
    NULL, NULL, NULL, NULL
  };
  GridLayout(allctls, 3, 0, 0).setPositionAndSize(0, 60-25/2, 320, 130+25);
  for(int i=0; i<6; ++i)
    lblRelay[i].draw(tft);
    
  while(true)
  {
    if (touch.dataAvailable())
    {
      Point pt = touch.read();
      int x = pt.x;
      int y = pt.y;
      
      if (lblBack.isTouched(x, y))
        break;

      for(int i=0; i<6; ++i)
        if (lblRelay[i].isTouched(x, y))
          digitalWrite(PLDuino::RELAY1+i, (digitalRead(PLDuino::RELAY1+i)==HIGH? LOW : HIGH));
      
      while(touch.dataAvailable()) touch.read();
    }
    
    // Update display according to relays' states
    for(int i=0; i<6; ++i)
    {
      bool switched_on = (digitalRead(PLDuino::RELAY1+i) == HIGH);
      lblRelay[i].updateTextAndColor(
        String("RLY") + String(i+1) + String(switched_on? "on" : "Off"),
        (switched_on? ILI9341_YELLOW : ILI9341_BLUE),
        tft
      );
      lblRelay[i].draw(tft);
    }
    
    delay(50);
  }
  tft.fillScreen(ILI9341_BLACK);
}

