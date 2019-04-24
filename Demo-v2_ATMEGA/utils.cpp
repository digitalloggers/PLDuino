#include "utils.h"
#include <HardwareSerial.h>
#include <SPI.h>
#include <SD.h>
#include <DS3232RTC.h>
#include <Time.h>
#include <Wire.h>


// Assembles "HH:MM:SS" string (e.g. "09:30:00") from current time values.
String getTimeStr()
{
  char buffer[30];
  sprintf(buffer, "%02d:%02d:%02d", hour(), minute(), second());
  return String(buffer);
}


// Assembles "mm:dd:yy" string (e.g. "02 29 16", i.e. February 29th, 2016) from current date values.
String getDateStr()
{
  char buffer[30];
  sprintf(buffer, "%02d %02d %02d", month(), day(), year()%100);
  return String(buffer);
}


void wrapvalue (int &value, int min, int max)
{
  if (value < min) value = max;
  if (value > max) value = min;
}


bool isLeapYear (int year)
{
  return (year%4==0 && year%100!=0) || year%400==0;
}


// Returns number of days in the given month of the given year
int daysInMonth (int year, int month) // 1==january
{
  static const int days[] = {
    31, -1, 31, 30, // february -- see below
    31, 30, 31, 31,
    30, 31, 30, 31
  };
  if (month == 2)
    return (isLeapYear(year)? 29 : 28);
  else
    return days[month-1];
}


// Just pauses execution until the screen is touched.
void waitForTouch (PLDTouch &touch)
{
  while(!touch.dataAvailable());
  while(touch.dataAvailable()) touch.read();
}


// Waits until the user touches the screen, or timeout expires.
// Returns true if touched, otherwise false (i.e. exit by timeout)
bool waitForTouchOrTimeout (PLDTouch &touch, int seconds)
{
  unsigned long starttime = millis();
  while(millis() - starttime < seconds*1000ul)
  {
    if (touch.dataAvailable())
    {
      while(touch.dataAvailable()) touch.read();
      return true;
    }
  }
  return false;
}


void playSound (TMRpcm &tmrpcm, const char *filename)
{
  tmrpcm.play(const_cast<char *>(filename));
  while(tmrpcm.isPlaying());
}



#define BUFFPIXEL 80

uint16_t read16(File &f);
uint32_t read32(File &f);

// Draws BMP image on screen.
// x, y - position where to draw.
void bmpDraw(Adafruit_ILI9341 &tft, const char *filename, uint8_t x, uint16_t y)
{
  File     bmpFile;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();

  if (x>=tft.width() || y>=tft.height()) return;

/*
  Serial.println();
  Serial.print(F("Loading image '"));
  Serial.print(filename);
  Serial.println('\'');
*/
  // Open requested file on SD card
  if ((bmpFile = SD.open(filename)) == NULL)
  {
    Serial.print(F("File not found"));
    return;
  }

  // Parse BMP header
  if (read16(bmpFile) == 0x4D42)
  {
    // BMP signature
    //Serial.print(F("File size: ")); Serial.println(
    read32(bmpFile);
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    //Serial.print(F("Image Offset: ")); Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    //Serial.print(F("Header size: ")); Serial.println(
    read32(bmpFile);
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if (read16(bmpFile) == 1)
    {
      // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      //Serial.print(F("Bit Depth: ")); Serial.println(bmpDepth);
      if((bmpDepth == 24) && (read32(bmpFile) == 0))
      {
        // 0 = uncompressed
        goodBmp = true; // Supported BMP format -- proceed!
        /*
        Serial.print(F("Image size: "));
        Serial.print(bmpWidth);
        Serial.print('x');
        Serial.println(bmpHeight);
*/
        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if(bmpHeight < 0)
        {
          bmpHeight = -bmpHeight;
          flip      = false;
        }

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if((x+w-1) >= tft.width())  w = tft.width()  - x;
        if((y+h-1) >= tft.height()) h = tft.height() - y;

        // Set TFT address window to clipped image bounds
        tft.setAddrWindow(x, y, x+w-1, y+h-1);

        for(row=0; row<h; row++)
        {
          if(flip)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          if (bmpFile.position() != pos)
          {
            // Need seek?
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }

          for(col=0; col<w; col++)
          {
            if (buffidx >= sizeof(sdbuffer))
            {
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
            }

            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            tft.pushColor(tft.color565(r,g,b));
          }
        }
        /*
        Serial.print(F("Loaded in "));
        Serial.print(millis() - startTime);
        Serial.println(" ms");*/
      }
    }
  }

  bmpFile.close();
  if (!goodBmp) Serial.println(F("BMP format not recognized."));
}

uint16_t read16(File &f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(File &f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}

