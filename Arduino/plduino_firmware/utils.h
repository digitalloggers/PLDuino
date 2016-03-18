#pragma once
#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include <TMRpcm_PLDuino.h>
#include <PLDTouch.h>

String getTimeStr();
String getDateStr();
void wrapvalue (int &value, int min, int max);
bool isLeapYear (int year);
int daysInMonth (int year, int month);
void waitForTouch(PLDTouch &touch);
bool waitForTouchOrTimeout (PLDTouch &touch, int seconds);
void playSound (TMRpcm &tmrpcm, const char *filename);
void bmpDraw(Adafruit_ILI9341 &tft, const char *filename, uint8_t x, uint16_t y);

enum RunMode {
  MODE_DEMO,
  MODE_TEST,
  MODE_WIFI
};

