#include "Point.h"


Point::Point()
  : x(0), y(0),
    valid(false)
{
}


void Point::set (long x0, long y0)
{
    x = x0;
    y = y0;
    valid = true;
}


Point::Point(long x0, long y0)
{
    set(x0, y0);
}


Point::Point(const Point &pt)
{
    x = pt.x;
    y = pt.y;
    valid = pt.valid;
}


bool Point::isValid() const
{
    return valid;
}
