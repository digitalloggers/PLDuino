#include <PLDuino.h>
#include <SPI.h>
#include <SD.h>
#define FILENAME "test.tmp"

void setup()
{
  // Initializing Serial.
  Serial.begin(115200);

  // Initializing SD card.
  if (!SD.begin(PLDuino::SD_CS))
  {
    Serial.println("Failed to initialize SD card. Please check it.");
    return;
  }

  // Trying to create a file
  {
    Serial.println("Creating a file...");

    // Opening a file for writing...
    File f = SD.open(FILENAME, FILE_WRITE);
    if (!f)
    {
      // Stop on error
      Serial.println("Failed to create " FILENAME " file. Stop.");
      return;
    }

    // Write some data there
    f.println("Test string");
    
    f.close(); // close when done
  }

  // Trying to read a file
  {
    Serial.println("Re-opening the file for reading...");

    // Trying to open our file.
    File f = SD.open(FILENAME, FILE_READ);
    if (!f)
    {
      // Stop on error
      Serial.println("Failed to open newly created file " FILENAME ". Stop.");
      return;
    }
    
    Serial.println("File contents: ");
    // Read out file contents and print them to Serial.
    while(!f.available())
      Serial.write(f.read());
    Serial.println(); Serial.println("--end--");
    f.close(); // close when done
  }
}

void loop()
{
}

