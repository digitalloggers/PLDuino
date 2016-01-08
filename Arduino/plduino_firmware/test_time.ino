#include <Arduino.h>
#include <DS3232RTC.h>
#include <Time.h>
#include <Wire.h>
#include "utils.h"

void testClock()
{
  setSyncProvider(RTC.get);
  if (timeStatus() != timeSet) 
  {
    tft.fillScreen(ILI9341_RED);
    Label lblError("UNABLE TO SYNC WITH RTC", ILI9341_YELLOW, ILI9341_BLACK, HORZ_CENTER|VERT_CENTER);
    lblError.setPositionAndSize(0,0, 300,120);
    Label lblTap1("(tap to proceed)", ILI9341_WHITE, ILI9341_BLACK, HORZ_CENTER|VERT_CENTER);
    lblTap1.setPositionAndSize(0,120, 300,120);
    lblError.draw(tft); lblTap1.draw(tft);
    
    while(touch.dataAvailable()) touch.read();
    while(!touch.dataAvailable());  touch.read();
    tft.fillScreen(ILI9341_BLACK);
  }

  //////////////////////////////
  //
  //    Time     Date (m/d/y)
  //
  //   ^^  ^^^   ^^  ^^  ^^^^
  //   hr  min   mm  dd  yyyy
  //   vv  vvv   vv  vv  vvvv
  //
  //
  //  [SET]         [BACK]
  //
  //////////////////////////////
  
  Label lblTime("Time", ILI9341_RED, ILI9341_BLACK, HORZ_CENTER|VERT_CENTER);
  lblTime.setPositionAndSize(3*CW, 3*RH, 6*CW, RH); lblTime.draw(tft);
  
  Label lblHourUp(" ^^ ", ILI9341_WHITE, ILI9341_BLACK);
  Label lblHour  ( "01", ILI9341_WHITE, ILI9341_BLACK);
  Label lblHourDn(" vv ", ILI9341_WHITE, ILI9341_BLACK);
  lblHourUp.setPositionAndSize(2*CW, 6*RH, 4*CW,RH);
  lblHour  .setPositionAndSize(3*CW, 8*RH, 4*CW,RH);
  lblHourDn.setPositionAndSize(2*CW,10*RH, 4*CW,RH);
  lblHourUp.draw(tft); lblHour.draw(tft); lblHourDn.draw(tft);

  Label lblMinuteUp(" ^^ ", ILI9341_WHITE, ILI9341_BLACK);
  Label lblMinute  ( "01", ILI9341_WHITE, ILI9341_BLACK);
  Label lblMinuteDn(" vv ", ILI9341_WHITE, ILI9341_BLACK);
  lblMinuteUp.setPositionAndSize(6*CW, 6*RH, 4*CW,RH);
  lblMinute  .setPositionAndSize(7*CW, 8*RH, 4*CW,RH);
  lblMinuteDn.setPositionAndSize(6*CW,10*RH, 4*CW,RH);
  lblMinuteUp.draw(tft); lblMinute.draw(tft); lblMinuteDn.draw(tft);

  
  Label lblDate("Date (m/d/y)", ILI9341_RED, ILI9341_BLACK, VERT_CENTER|HORZ_CENTER);
  lblDate.setPositionAndSize(12*CW,3*RH, 12*CW,RH); lblDate.draw(tft);
  
  Label lblMonthUp(" ^^ ", ILI9341_WHITE, ILI9341_BLACK);
  Label lblMonth  ( "01", ILI9341_WHITE, ILI9341_BLACK);
  Label lblMonthDn(" vv ", ILI9341_WHITE, ILI9341_BLACK);
  lblMonthUp.setPositionAndSize(11*CW, 6*RH, 4*CW,RH);
  lblMonth  .setPositionAndSize(12*CW, 8*RH, 4*CW,RH);
  lblMonthDn.setPositionAndSize(11*CW,10*RH, 4*CW,RH);
  lblMonthUp.draw(tft); lblMonth.draw(tft); lblMonthDn.draw(tft);
  
  Label lblDayUp(" ^^ ", ILI9341_WHITE, ILI9341_BLACK);
  Label lblDay  ( "01", ILI9341_WHITE, ILI9341_BLACK);
  Label lblDayDn(" vv ", ILI9341_WHITE, ILI9341_BLACK);
  lblDayUp.setPositionAndSize(15*CW, 6*RH, 4*CW,RH);
  lblDay  .setPositionAndSize(16*CW, 8*RH, 4*CW,RH);
  lblDayDn.setPositionAndSize(15*CW,10*RH, 4*CW,RH);
  lblDayUp.draw(tft); lblDay.draw(tft); lblDayDn.draw(tft);
  
  Label lblYearUp("^^^^", ILI9341_WHITE, ILI9341_BLACK);
  Label lblYear  ("2008", ILI9341_WHITE, ILI9341_BLACK);
  Label lblYearDn("vvvv", ILI9341_WHITE, ILI9341_BLACK);
  lblYearUp.setPositionAndSize(20*CW, 6*RH, 4*CW,RH);
  lblYear  .setPositionAndSize(20*CW, 8*RH, 4*CW,RH);
  lblYearDn.setPositionAndSize(20*CW,10*RH, 4*CW,RH);
  lblYearUp.draw(tft); lblYear.draw(tft); lblYearDn.draw(tft);
  
  Label btnSet("[SET]", ILI9341_GREEN, ILI9341_BLACK);
  btnSet.setPositionAndSize(4*CW,14*RH, 5*CW,RH); btnSet.draw(tft);
  
  Label btnBack("[NEXT]", ILI9341_GREEN, ILI9341_BLACK);
  btnBack.setPositionAndSize(16*CW,14*RH, 6*CW,RH); btnBack.draw(tft);
  
  
  char buf[10];
  int hours=hour(), minutes=minute();
  int years=year(), months=month(), days=day();
  while(true)
  {
    long prevstate = hours+minutes+months+days+years;
    if (touch.dataAvailable())
    {
      Point pt = touch.read();
      int tx = pt.x, ty = pt.y;
      
      if (lblHourUp.isTouched(tx, ty)) hours++;
      if (lblHourDn.isTouched(tx, ty)) hours--;
      if (lblMinuteUp.isTouched(tx, ty)) minutes++;
      if (lblMinuteDn.isTouched(tx, ty)) minutes--;
      
      if (lblMonthUp.isTouched(tx, ty)) months++;
      if (lblMonthDn.isTouched(tx, ty)) months--;
      if (lblDayUp.isTouched(tx, ty)) days++;
      if (lblDayDn.isTouched(tx, ty)) days--;
      if (lblYearUp.isTouched(tx, ty)) years++;
      if (lblYearDn.isTouched(tx, ty)) years--;
      
      if (btnSet.isTouched(tx, ty))
      {
        // assemble time_t
        tmElements_t tm;
        tm.Year = (years>=1000? CalendarYrToTm(years) : y2kYearToTm(years));
        tm.Month = months;
        tm.Day = days;
        tm.Hour = hours;
        tm.Minute = minutes;
        tm.Second = 0;
        time_t t = makeTime(tm);
        
        RTC.set(t);
        setTime(t);
        break;
      }
      
      if (btnBack.isTouched(tx, ty))
        break;
        
    }
    wrapvalue(hours, 0, 23);
    wrapvalue(minutes, 0, 59);
    wrapvalue(months, 1, 12);
    wrapvalue(years, 1980, 2200);
    wrapvalue(days, 1, daysInMonth(years, months));
    
    sprintf(buf, "%02d", hours); lblHour.updateText(buf, tft);
    sprintf(buf, "%02d", minutes); lblMinute.updateText(buf, tft);
    sprintf(buf, "%02d", months); lblMonth.updateText(buf, tft);
    sprintf(buf, "%02d", days); lblDay.updateText(buf, tft);
    sprintf(buf, "%04d", years); lblYear.updateText(buf, tft);
    
    int newstate = hours+minutes+months+days+years;
    if (prevstate != newstate) delay(300);
  }
}

