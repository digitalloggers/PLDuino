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
  // A convenient macro which prints start-up messages to both LCD and Serial.
  #define LOG(msg) { tft.println(msg); Serial.println(msg); }
  
  // Set pin modes and initialize stuff
  // NB: This line is necessary in all sketches which use PLDuino library stuff.
  PLDuino::init();
  
  // Signal that the demo firmware is started.
  blinkLED();
  
  // Power-on LCD and set it up
  PLDuino::enableLCD();
  tft.begin();
  tft.setRotation(3);

  // Setup serials. Serial2 is connected to ESP-02 Wi-Fi module.
  Serial.begin(9600);
  Serial2.begin(9600);
  
  // Print version info.
  tft.fillScreen(ILI9341_BLACK);
  LOG("PLDuino firmware v" VERSION ", built " __DATE__)
  LOG("")

  // We need to initialize SD card at startup!
  LOG("Initializing SD card...")
  card_initialized = card.init(SPI_HALF_SPEED, PLDuino::SD_CS);
  if (!SD.begin(PLDuino::SD_CS))
    LOG("ERROR: Can't initialize SD card!")

  // Initializing touch screen.
  LOG("Initializing touch...")
  touch.init(1);

  // Initializing real-time clock.
  LOG("Initializing RTC...")
  setSyncProvider(RTC.get);
  if (timeStatus() != timeSet)
    LOG("ERROR: Unable to sync with the RTC")
  
  // Setup speaker pin to play sounds.
  tmrpcm.speakerPin = 9;
  
  // Initialization is complete. 
  LOG("")
  LOG("Initialization complete.")
  LOG("")
  LOG("-- Touch to keep the log on screen --")
  delay(1500);
  while(touch.dataAvailable()) touch.read();
  tft.fillScreen(ILI9341_BLACK);

  // Detect mode of operation.
  // Enter demo mode if no DINs are connected.
  bool testmode = false;
  for(int i=1; i<8; ++i)
    testmode |= !digitalRead(PLDuino::DIN1+i);
  if (testmode)
  {
    // In test mode, it provides a sequence of screens to check if PLD's facitilies work properly.
    // You need to activate any DIN to enter this mode (see above).
    test();
  }
  else
  {
    // The code shows a sequence of slides in demo mode.
    demo();
  }
}

void loop(){}

