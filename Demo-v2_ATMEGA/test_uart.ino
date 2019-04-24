#include <Arduino.h>
#include "utils.h"

void testUARTLoopback()
{
  tft.fillScreen(ILI9341_BLACK);
  
  Label lblResult("UART LOOPBACK TEST", ILI9341_WHITE, ILI9341_BLACK, VERT_CENTER|HORZ_CENTER);
  lblResult.setPositionAndSize(2*CW,5*RH, 22*CW, RH); lblResult.draw(tft);
  
  Label lblHint("Connect RX3 and TX3", ILI9341_LIGHTGREY, ILI9341_BLACK, VERT_CENTER|HORZ_CENTER);
  lblHint.setPositionAndSize(2*CW,15*RH, 22*CW, RH); lblHint.draw(tft);
  
  Label lblStart("[START]", ILI9341_GREEN, ILI9341_BLACK, VERT_CENTER|HORZ_CENTER);
  lblStart.setPositionAndSize(2*CW,10*RH, 10*CW, RH); lblStart.draw(tft);
  
  Label lblNext("[NEXT]", ILI9341_GREEN, ILI9341_BLACK, VERT_CENTER|HORZ_CENTER);
  lblNext.setPositionAndSize(12*CW,10*RH, 10*CW, RH); lblNext.draw(tft);
  
  while(true)
  {
    if (touch.dataAvailable())
    {
      Point pt = touch.read();
      if (lblStart.isTouched(pt.x, pt.y))
      {
        bool failed = false;
        Serial3.begin(115200);
        Serial3.flush();
        while(Serial3.available()) Serial3.read();
        for(long ch = 0x00; ch <= 0xff && !failed; ++ch)
        {
          lblResult.updateTextAndColor(
            "Testing pattern " + String(ch, HEX),
            ILI9341_WHITE,
            tft
          );
          Serial3.write((unsigned char)ch);
          
          long start_time = millis();
          while(!failed)
          {
            if (millis() - start_time > 3000)
              failed = true;
            
            if (Serial3.available())
            {
              unsigned char rc = Serial3.read();
              if (ch != rc)
              {
                lblResult.updateTextAndColor(String(rc, HEX) + " MUST BE " + String(ch, HEX), ILI9341_RED, tft);
                delay(2000);
                failed = true;
              }
              else
              {
                lblResult.updateText(String(rc, HEX) + " OK", tft);
                delay(50);
                break;
              }
            }
          }
        }
        
        lblResult.updateTextAndColor(
          (failed? "FAILED" : "PASSED"),
          (failed? ILI9341_RED : ILI9341_GREEN),
          tft
        );
      }
      if (lblNext.isTouched(pt.x, pt.y))
        break;
    }
    delay(10);
  }
}

