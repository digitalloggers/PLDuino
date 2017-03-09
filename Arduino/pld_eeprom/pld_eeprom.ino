#include <PLDuino.h>
#include <Wire.h>

// I2C address of EEPROM IC.
#define EEPROM_DEV_ADDR 0x52

// Necessary functions' prototypes (see below).
byte readEEPROM (int deviceaddress, unsigned int eeaddress);
void writeEEPROM (int deviceaddress, unsigned int eeaddress, byte data);


void setup()
{
  // Initialize PLDuino.
  // NB: This line is necessary in all PLDuino's sketches.
  PLDuino::init();

  // Initializing Serial.
  Serial.begin(115200);

  // Initializing Wire library to communicate with EEPROM IC via I2C bus.
  Wire.begin();

  // Just some value which we will use to fill and verify EEPROM.
  #define VALUE 0xAB
  
  // Fill EEPROM with the value and verify if it's written as expected.
  for(int i=0; i<100; ++i)
  {
    Serial.println("address: 0x" + String(i, HEX));
    Serial.println("writing 0x" + String(VALUE, HEX));
    writeEEPROM(EEPROM_DEV_ADDR, i, VALUE);
    Serial.println("read 0x" + String(readEEPROM(EEPROM_DEV_ADDR, i), HEX));
    Serial.println();
  }
}


void loop() {}


// A function to write a byte to EEPROM.
// deviceaddress is the I2C address of EEPROM IC.
// eeaddress is an address in EEPROM where we want to write.
// data is the new value we want to write there.
void writeEEPROM (int deviceaddress, unsigned int eeaddress, byte data)
{
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));
  Wire.write((int)(eeaddress & 0xFF));
  Wire.write(data);
  Wire.endTransmission();
  delay(5);
}


// A function to read from EEPROM. Returns a byte read from EEPROM.
// deviceaddress is the I2C address of EEPROM IC
// eeaddress is an address in EEPROM from where we want to read a byte. 
byte readEEPROM (int deviceaddress, unsigned int eeaddress)
{
  byte rdata = 0xFF;
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));
  Wire.write((int)(eeaddress & 0xFF));
  Wire.endTransmission();
  Wire.requestFrom(deviceaddress,1);
  if (Wire.available()) rdata = Wire.read();
  return rdata;
}


