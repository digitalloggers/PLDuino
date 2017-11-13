#pragma once
#include "Arduino.h"

struct Point
{
    long x, y;
    bool valid;

    Point();
    Point(long x0, long y0);
    Point(const Point &pt);

    bool isValid() const;
    void set (long x0, long y0);
};
