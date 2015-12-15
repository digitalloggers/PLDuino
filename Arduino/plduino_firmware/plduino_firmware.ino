#include <TMRpcm.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <PLDuino.h>
#include <PLDTouch.h>
#include <PLDuinoGUI.h>
#include <using_namespace_PLDuinoGUI.h>
#include <DS3232RTC.h>
#include <Time.h>
#include <Wire.h>
#include <avr/io.h>

#define VERSION "1.2.1.0"

// Setup fuse bits
#define LOW_FUSE 0xFF
#define HIGH_FUSE 0xD8
#define EXT_FUSE 0xFD
FUSES = { LOW_FUSE, HIGH_FUSE, EXT_FUSE };

Adafruit_ILI9341 tft = Adafruit_ILI9341(PLDuino::LCD_CS, PLDuino::LCD_DC);
PLDTouch touch(PLDuino::TOUCH_CS, PLDuino::TOUCH_IRQ);
TMRpcm tmrpcm;
Sd2Card card; bool card_initialized;


void blinkLED()
{
  for(int i=0; i<3; ++i)
  {
    digitalWrite(PLDuino::LED_PIN, LOW); delay(200);
    digitalWrite(PLDuino::LED_PIN, HIGH); delay(200);
  }
}


void setup()
{
  #define LOG(msg) { tft.println(msg); Serial.println(msg); }
  
  // Set pin modes and initialize stuff
  PLDuino::init();
  
  blinkLED();
  
  // Power-on LCD and set it up
  PLDuino::enableLCD();
  tft.begin();
  tft.setRotation(3);

  // Choose mode
  if (!digitalRead(PLDuino::DIN2))
  {
    // ESP programming mode
    tft.fillScreen(ILI9341_RED);
    tft.setTextColor(ILI9341_YELLOW);
    tft.setTextSize(3);
    tft.setCursor(30,100);
    tft.println("ESP prog mode");
    flashESPMode();
  }
  else
  {
    // Test or Demo mode selection

    Serial.begin(115200);
    
    tft.fillScreen(ILI9341_BLACK);
    LOG("PLDuino firmware v" VERSION ", built " __DATE__)
    LOG("")
  
    // We need to initialize SD card at startup!
    LOG("initializing SD card...")
    card_initialized = card.init(SPI_HALF_SPEED, PLDuino::SD_CS);
    if (!SD.begin(PLDuino::SD_CS))
      LOG("ERROR: Can't initialize SD card!")
  
    LOG("Initializing touch...")
    touch.init(1);
  
    LOG("Initializing RTC...")
    setSyncProvider(RTC.get);
    if (timeStatus() != timeSet)
      LOG("ERROR: Unable to sync with the RTC")
  
    tmrpcm.speakerPin = 9;
  
    LOG("")
    LOG("Initialization complete.")
    LOG("")
    LOG("-- Touch to keep the log on screen --")
    delay(1500);
    while(touch.dataAvailable()) touch.read();
    tft.fillScreen(ILI9341_BLACK);
  
    // Enter demo mode if no DINs are connected.
    bool testmode = false;
    for(int i=1; i<8; ++i)
      testmode |= !digitalRead(PLDuino::DIN1+i);
    if (testmode)
      test();
    else
      demo();
  }
}

void loop(){}

