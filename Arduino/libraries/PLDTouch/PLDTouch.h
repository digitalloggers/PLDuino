#pragma once
#include "Arduino.h"
#include "Point.h"

#define PLDTOUCH_NO_ROTATION 3
#define PLDTOUCH_ROTATE_90   0
#define PLDTOUCH_ROTATE_180  1
#define PLDTOUCH_ROTATE_270  2

// Legacy constants. Deprecated
#define NO_ROTATION PLDTOUCH_ROTATE_90
#define ROTATE_90   PLDTOUCH_ROTATE_180
#define ROTATE_180  PLDTOUCH_ROTATE_270
#define ROTATE_270  PLDTOUCH_NO_ROTATION

class PLDTouch
{
public:
    PLDTouch(byte tcs, byte irq, long display_width=320, long display_height=240);
    void init(byte rotation = PLDTOUCH_ROTATE_90);
    void updateCalibrationData(long left, long top, long right, long bottom);
    bool dataAvailable();
    Point read(int precision = 100);
    Point readRaw();
    Point touch_to_display(const Point &touchpt) const;
private:
    long cal_left, cal_top, cal_right, cal_bottom;
    byte T_CS, T_IRQ;
    byte rotation;
    long display_width, display_height;
};
