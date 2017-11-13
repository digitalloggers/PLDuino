#include <SPI.h>
#include "PLDTouch.h"
#include "calibration.h"
word ctl(byte command);


PLDTouch::PLDTouch(byte tcs, byte irq, long dispw, long disph)
  : T_CS(tcs), T_IRQ(irq),
    rotation(PLDTOUCH_NO_ROTATION),
    display_width(dispw), display_height(disph)
{
    updateCalibrationData(
        DEFAULT_CAL_LEFT, DEFAULT_CAL_TOP,
        DEFAULT_CAL_RIGHT, DEFAULT_CAL_BOTTOM
    );
}


void PLDTouch::updateCalibrationData(long left, long top, long right, long bottom)
{
    cal_left = left;
    cal_top = top;
    cal_right = right;
    cal_bottom = bottom;
}


void PLDTouch::init(byte rotation)
{
    pinMode(T_IRQ, INPUT); digitalWrite(T_IRQ, HIGH);
    pinMode(T_CS, OUTPUT); digitalWrite(T_CS, HIGH);
    this->rotation = rotation;
}


Point PLDTouch::read(int precision)
{
    long tmpx=0, tmpy=0;
    int datacount = 0;
    
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV32);
    digitalWrite(T_CS, LOW);
    for(int i=0; datacount<precision && i<precision*5; i++)
    {
        long x = static_cast<long>(ctl(0x90));
        long y = static_cast<long>(ctl(0xD0));
        if (x>=min(cal_left, cal_right) && x<=max(cal_left, cal_right) &&
            y>=min(cal_top, cal_bottom) && y<=max(cal_top, cal_bottom))
        {
            tmpx += x;
            tmpy += y;
            datacount++;
        }
    }
    digitalWrite(T_CS, HIGH);
    SPI.end();

    return datacount<precision? Point() : touch_to_display(Point(tmpx/datacount, tmpy/datacount));
}


Point PLDTouch::touch_to_display(const Point &touch) const
{
    const long touch_width = cal_right - cal_left;
    const long touch_height = cal_bottom - cal_top;
    const long display_x = (touch.x - cal_left) * display_width / touch_width;
    const long display_y = (touch.y - cal_top) * display_height / touch_height;
    switch(rotation)
    {
        case PLDTOUCH_NO_ROTATION:
            return Point(display_x, display_y);
            break;
        case PLDTOUCH_ROTATE_90:
            return Point(display_y, display_width - display_x);
            break;
        case PLDTOUCH_ROTATE_180:
            return Point(display_width - display_x, display_height - display_y);
            break;
        case PLDTOUCH_ROTATE_270:
            return Point(display_height - display_y, display_x);
            break;
    }
    return Point();
}


bool PLDTouch::dataAvailable()
{
    return !digitalRead(T_IRQ);
}


Point PLDTouch::readRaw()
{
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV32);
    digitalWrite(T_CS, LOW);
    long tx = static_cast<long>(ctl(0x90));
    long ty = static_cast<long>(ctl(0xD0));
    digitalWrite(T_CS, HIGH);
    SPI.end();
    return Point(tx, ty);
}


word ctl(byte command)
{
    SPI.transfer(command);
    byte f1 = SPI.transfer(0x00);
    byte f2 = SPI.transfer(0x00);
    return word(f1, f2) >> 3;
}
