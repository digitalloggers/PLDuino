#include <Arduino.h>
#include "utils.h"

void testDOUTs()
{
  Label lblBack("[NEXT]", ILI9341_GREEN);
  lblBack.setPositionAndSize(111, 195, 100, 35); lblBack.draw(tft);
  
  Label lblTitle("Digital outputs test", ILI9341_WHITE, ILI9341_BLACK);
  lblTitle.setPositionAndSize(0,0,320, 40); lblTitle.draw(tft);
  
  Label lblDOUT[] = {
    Label("--", ILI9341_WHITE, ILI9341_BLACK, HORZ_CENTER|VERT_CENTER),
    Label("--", ILI9341_WHITE, ILI9341_BLACK, HORZ_CENTER|VERT_CENTER),
    Label("--", ILI9341_WHITE, ILI9341_BLACK, HORZ_CENTER|VERT_CENTER),
    Label("--", ILI9341_WHITE, ILI9341_BLACK, HORZ_CENTER|VERT_CENTER),
    Label("--", ILI9341_WHITE, ILI9341_BLACK, HORZ_CENTER|VERT_CENTER),
    Label("--", ILI9341_WHITE, ILI9341_BLACK, HORZ_CENTER|VERT_CENTER),
    Label("--", ILI9341_WHITE, ILI9341_BLACK, HORZ_CENTER|VERT_CENTER),
    Label("--", ILI9341_WHITE, ILI9341_BLACK, HORZ_CENTER|VERT_CENTER),
  };
  Sizable *allctls[] = {
    &lblDOUT[0], &lblDOUT[1],
    &lblDOUT[2], &lblDOUT[3],
    &lblDOUT[4], &lblDOUT[5],
    &lblDOUT[6], &lblDOUT[7],
    NULL
  };
  GridLayout(allctls, 4, 0, 0).setPositionAndSize(0, 30, 320, 130+25);
  
  for(int i=0; allctls[i]; ++i)
    ((Label *)allctls[i])->draw(tft);
  
  bool soundPlayed = false;
  while(true)
  {

    if (touch.dataAvailable())
    {
      Point pt = touch.read();
      int x = pt.x;
      int y = pt.y;
      
      if (lblBack.isTouched(x, y))
        break;

      for(int i=0; i<8; ++i)
        if (lblDOUT[i].isTouched(x, y))
          digitalWrite(38+i, (digitalRead(38+i)==HIGH? LOW : HIGH));
      
      while(touch.dataAvailable()) touch.read();
    }
    
    // Redraw labels according to outlets state
    for(int i=0; i<8; ++i)
    {
      bool switched_on = (digitalRead(38+i) == HIGH);
      lblDOUT[i].updateTextAndColor(
        "D" + String(38+i),
        (switched_on? ILI9341_YELLOW : ILI9341_BLUE),
        tft
      );
    }
    
    delay(50);
  }
  tft.fillScreen(ILI9341_BLACK);
}

