#pragma once
#include "Arduino.h"

#define NO_ROTATION	0
#define ROTATE_90	1
#define ROTATE_180	2
#define ROTATE_270	3


struct Point {
	unsigned long x, y;
	Point() { x=-1, y=-1; }
	Point(unsigned long x0, unsigned long y0) : x(x0), y(y0) {}
	Point(const Point &pt) { x=pt.x; y=pt.y; }
	bool isValid() const { return !(x==-1 && y==-1); }
};


class PLDTouch
{
public:
	PLDTouch (byte tcs, byte irq);
	void init (byte rotation = NO_ROTATION);

	bool dataAvailable();
	Point read (int precision = 100);
	Point readRaw();

private:
	byte T_CS, T_IRQ;
	byte rotation, _default_orientation;
	long disp_x_size, disp_y_size;
	long touch_x_left, touch_x_right, touch_y_top, touch_y_bottom;
};
