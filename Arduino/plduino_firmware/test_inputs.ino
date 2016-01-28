#include <Arduino.h>
#include "utils.h"

void testInputs()
{
  Label lblBack("[NEXT]", ILI9341_RED);
  lblBack.setPositionAndSize(111, 195, 100, 35); lblBack.draw(tft);
  
  Label lblTime("", ILI9341_WHITE, ILI9341_BLACK);
  lblTime.setPositionAndSize(10, 200, 100, 25);
  lblTime.draw(tft);
  
  Label lblDate("", ILI9341_WHITE, ILI9341_BLACK);
  lblDate.setPositionAndSize(218, 200, 100, 25);
  lblDate.draw(tft);
  
  Label lblAINtitle[] = {
    Label("A0 = ", ILI9341_WHITE, ILI9341_BLACK),
    Label("A1 = ", ILI9341_WHITE, ILI9341_BLACK),
    Label("A2 = ", ILI9341_WHITE, ILI9341_BLACK),
    Label("A3 = ", ILI9341_WHITE, ILI9341_BLACK),
    Label("A4 = ", ILI9341_WHITE, ILI9341_BLACK),
    Label("A5 = ", ILI9341_WHITE, ILI9341_BLACK),
    Label("A6 = ", ILI9341_WHITE, ILI9341_BLACK),
    Label("A7 = ", ILI9341_WHITE, ILI9341_BLACK),
  };
  Label lblAIN[] = {
    Label("---", ILI9341_WHITE, ILI9341_BLACK),
    Label("---", ILI9341_WHITE, ILI9341_BLACK),
    Label("---", ILI9341_WHITE, ILI9341_BLACK),
    Label("---", ILI9341_WHITE, ILI9341_BLACK),
    Label("---", ILI9341_WHITE, ILI9341_BLACK),
    Label("---", ILI9341_WHITE, ILI9341_BLACK),
    Label("---", ILI9341_WHITE, ILI9341_BLACK),
    Label("---", ILI9341_WHITE, ILI9341_BLACK),
  };
  
  Label lblDINtitle[] = {
    Label("D30 = ", ILI9341_WHITE, ILI9341_BLACK),
    Label("D31 = ", ILI9341_WHITE, ILI9341_BLACK),
    Label("D32 = ", ILI9341_WHITE, ILI9341_BLACK),
    Label("D33 = ", ILI9341_WHITE, ILI9341_BLACK),
    Label("D34 = ", ILI9341_WHITE, ILI9341_BLACK),
    Label("D35 = ", ILI9341_WHITE, ILI9341_BLACK),
    Label("D36 = ", ILI9341_WHITE, ILI9341_BLACK),
    Label("D37 = ", ILI9341_WHITE, ILI9341_BLACK),
  };
  Label lblDIN[] = {
    Label("-", ILI9341_WHITE, ILI9341_BLACK, HORZ_CENTER|VERT_CENTER),
    Label("-", ILI9341_WHITE, ILI9341_BLACK, HORZ_CENTER|VERT_CENTER),
    Label("-", ILI9341_WHITE, ILI9341_BLACK, HORZ_CENTER|VERT_CENTER),
    Label("-", ILI9341_WHITE, ILI9341_BLACK, HORZ_CENTER|VERT_CENTER),
    Label("-", ILI9341_WHITE, ILI9341_BLACK, HORZ_CENTER|VERT_CENTER),
    Label("-", ILI9341_WHITE, ILI9341_BLACK, HORZ_CENTER|VERT_CENTER),
    Label("-", ILI9341_WHITE, ILI9341_BLACK, HORZ_CENTER|VERT_CENTER),
    Label("-", ILI9341_WHITE, ILI9341_BLACK, HORZ_CENTER|VERT_CENTER),
  };
  Sizable *allctls[] = {
    &lblAINtitle[0], &lblAIN[0], &lblDINtitle[0], &lblDIN[0],
    &lblAINtitle[1], &lblAIN[1], &lblDINtitle[1], &lblDIN[1],
    &lblAINtitle[2], &lblAIN[2], &lblDINtitle[2], &lblDIN[2],
    &lblAINtitle[3], &lblAIN[3], &lblDINtitle[3], &lblDIN[3],
    &lblAINtitle[4], &lblAIN[4], &lblDINtitle[4], &lblDIN[4],
    &lblAINtitle[5], &lblAIN[5], &lblDINtitle[5], &lblDIN[5],
    &lblAINtitle[6], &lblAIN[6], &lblDINtitle[6], &lblDIN[6],
    &lblAINtitle[7], &lblAIN[7], &lblDINtitle[7], &lblDIN[7],
    NULL
  };
  float cw[4] = { 0.9, 1.5, 0.9, 1.0 };
  GridLayout(allctls, 4, 0, 0, cw).setPositionAndSize(35, 45, 280, 140);
  
  for(int i=0; allctls[i]; ++i)
    ((Label *)allctls[i])->draw(tft);
  
  char buffer[10];
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
    }
    
    // update inputs display
    for(int i=0; i<8; ++i)
    {
      lblDIN[i].updateText((digitalRead(30+i) == HIGH? "1" : "0"), tft);
      lblAIN[i].updateText(String(analogRead(A0+i)), tft);
    }
  }
  tft.fillScreen(ILI9341_BLACK);  
}

