#include <SPI.h>
#include "PLDTouch.h"

#define CAL_X 0x00378F66UL
#define CAL_Y 0x03C34155UL
#define CAL_S 0x000EF13FUL


PLDTouch::PLDTouch(byte tcs, byte irq)
{
    T_CS         = tcs;
    T_IRQ        = irq;
}


void PLDTouch::init(byte rotation)
{
	this->rotation			= rotation;
	_default_orientation	= CAL_S >> 31;
	touch_x_left			= (CAL_X >> 14) & 0x3FFF;
	touch_x_right			= CAL_X & 0x3FFF;
	touch_y_top				= (CAL_Y >> 14) & 0x3FFF;
	touch_y_bottom			= CAL_Y & 0x3FFF;
	disp_x_size				= (CAL_S >> 12) & 0x0FFF;
	disp_y_size				= CAL_S & 0x0FFF;

	pinMode(T_IRQ, INPUT); digitalWrite(T_IRQ, HIGH);
    pinMode(T_CS, OUTPUT); digitalWrite(T_CS, HIGH);
}


word ctl(byte command)
{
	SPI.transfer(command);
	byte f1 = SPI.transfer(0x00);
	byte f2 = SPI.transfer(0x00);
	return word(f1, f2) >> 3;
}


Point PLDTouch::read (int precision)
{
	unsigned long tx=0;
	unsigned long ty=0;
	int datacount=0;
	
	SPI.begin();
	SPI.setClockDivider(SPI_CLOCK_DIV32);
	digitalWrite(T_CS, LOW);
	for(int i=0; datacount<precision && i<precision*2; i++)
	{
		unsigned long x = ctl(0x90);
		unsigned long y = ctl(0xD0);
		if (!(x>max(touch_x_left, touch_x_right) || x==0 || y>max(touch_y_top, touch_y_bottom) || y==0))
		{
			ty += x;
			tx += y;
			datacount++;
		}
	}
	digitalWrite(T_CS, HIGH);
	SPI.end();

	if (datacount==0) return Point();

	long touch_width = touch_x_right - touch_x_left;
	long touch_height = touch_y_bottom - touch_y_top;

	long tpx = long(tx / datacount - touch_x_left) * disp_x_size / touch_width;
	long tpy = long(ty / datacount - touch_y_top) * disp_y_size / touch_height;

	switch(rotation)
	{
		case NO_ROTATION:	return Point(tpx, tpy);
		case ROTATE_90:		return Point(tpy, disp_x_size-tpx);
		case ROTATE_180:	return Point(disp_x_size-tpx, disp_y_size-tpy);
		case ROTATE_270:	return Point(disp_y_size-tpy, tpx);
	}
}


bool PLDTouch::dataAvailable()
{
	return !digitalRead(T_IRQ);
}


Point PLDTouch::readRaw()
{
	SPI.begin();
	digitalWrite(T_CS, LOW);
	unsigned long tx=ctl(0x90);
	unsigned long ty=ctl(0xD0);
	digitalWrite(T_CS, HIGH);
	SPI.end();
	return Point(tx, ty);
}
