#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <SD.h>
#include <PLDuino.h>
#include <PLDuinoGUI.h>
#include "utils.h"

String cardtype_to_str (uint8_t type)
{
  if (type == SD_CARD_TYPE_SD1) return "SD1";
  if (type == SD_CARD_TYPE_SD2) return "SD2";
  if (type == SD_CARD_TYPE_SDHC) return "SDHC";
  return "Unknown";
}

bool checkBlock (Sd2Card &card, uint64_t number);

void testSD()
{
  tft.fillScreen(ILI9341_BLACK);
  
  // SD card must be successfully initialized to proceed
  if (!card_initialized)
  {
    Label lbl1("NO SD CARD", ILI9341_RED, ILI9341_BLACK, VERT_CENTER|HORZ_CENTER);
    lbl1.setPositionAndSize(0,5*RH, 26*CW, RH); lbl1.draw(tft);
    
    Label lbl2("Touch to continue", ILI9341_LIGHTGREY, ILI9341_BLACK, VERT_CENTER|HORZ_CENTER);
    lbl2.setPositionAndSize(0,8*RH, 26*CW, RH); lbl2.draw(tft);

    waitForTouch(touch);    
    return;
  }
  
  Label lblStatus("SD CARD TEST", ILI9341_WHITE, ILI9341_BLACK, VERT_CENTER|HORZ_CENTER);
  lblStatus.setPositionAndSize(0,5*RH, 26*CW, RH); lblStatus.draw(tft);
  
  Label lblNotice("TEST destroys all data!!", ILI9341_MAGENTA, ILI9341_BLACK, VERT_CENTER|HORZ_CENTER);
  lblNotice.setPositionAndSize(0,0, 26*CW, RH); lblNotice.draw(tft);
  
  Label lblSizeNotice("Size: " + String(round(double(card.cardSize())/1024*512/1024/1024)) + "G", ILI9341_ORANGE, ILI9341_BLACK, VERT_CENTER|HORZ_CENTER);
  lblSizeNotice.setPositionAndSize(0,7*RH, 26*CW, RH); lblSizeNotice.draw(tft);
  
  Label lblTest("[TEST]", ILI9341_WHITE, ILI9341_BLACK, VERT_CENTER|HORZ_CENTER);
  lblTest.setPositionAndSize(2*CW,12*RH, 10*CW, RH); lblTest.draw(tft);
  
  Label lblNext("[NEXT]", ILI9341_WHITE, ILI9341_BLACK, VERT_CENTER|HORZ_CENTER);
  lblNext.setPositionAndSize(12*CW,12*RH, 10*CW, RH); lblNext.draw(tft);

  Serial.println("\nCard type: " + cardtype_to_str(card.type()));
  Serial.println("Card size: " + String(card.cardSize()));
  Serial.println("\n");
  
  while(true)
  {
    if (touch.dataAvailable())
    {
      Point pt = touch.read();
      if (lblTest.isTouched(pt.x, pt.y))
      {
        // test the card

        // Remove Test and Next buttons from screen
        lblTest.erase(tft); lblNext.erase(tft);
        
        // We need to check some blocks: 
        // first, we write some data into the block, and then read it back to verify if it's the same as written.

        uint32_t valid_blocks = 0;
        const uint32_t BLOCKS = 250;
        for(uint32_t i=0; i<BLOCKS; ++i)
        {
          // Choose a random block
          uint64_t block_number = random(0, card.cardSize());

          // Disable LCD and touchscreen to prevent interfering with SD
          digitalWrite(PLDuino::LCD_CS, HIGH);
          digitalWrite(PLDuino::TOUCH_CS, HIGH);
          
          // Check our block
          bool valid_block = checkBlock(card, block_number);
          // returns true (1) if valid, false (0) otherwise
          valid_blocks += valid_block;

          // Update display
          Serial.println("#" + String((uint32_t)i) + ": " + String((uint32_t)block_number) + ", " + (valid_block? "valid" : "FAILED"));
          lblStatus.updateTextAndColor(
            String(valid_blocks) + " valid of " + String(BLOCKS),
            (valid_blocks==i+1? ILI9341_GREEN : ILI9341_RED),
            tft
          );
        }
        // Report how many blocks were good/bad.
        Serial.println(String(valid_blocks) + " blocks of " + String(BLOCKS) + " are valid.");

        // Place the buttons back.
        lblTest.draw(tft); lblNext.draw(tft);
      }
      
      // Exit by touching Next button
      if (lblNext.isTouched(pt.x, pt.y))
        break;
    }
  }
}



// Function to generate data which we will write to the blocks being tested.
uint8_t generate_block_byte (uint32_t index)
{
  return (index % 2? 0xff : 0x00);
}

byte buf[512]; // Global buffer for checkBlock() function.
// Checks if block #number on card SD card is valid
bool checkBlock (Sd2Card &card, uint64_t number)
{
  // Prepare the buffer, fill it with data
  for(uint32_t i=0; i<512; ++i)
    buf[i] = generate_block_byte(i);

  // Overwrite our block with the data.
  if (!card.writeBlock(number, buf))
  {
    Serial.println("Can't write block");
    return false;
  }

  // Read the block back.
  // Use the same buffer, since we can generate the data again.
  if (!card.readBlock(number, buf))
  {
    // Can't read at all. Definitely an error.
    Serial.println("Can't read block");
    return false;
  }

  // Read operation succeeded. Let's check that read data is the same as written.
  for(uint32_t i=0; i<512; ++i)
    if (buf[i] != generate_block_byte(i))
    {
      // Bytes differ. The block is bad.
      Serial.println("Invalid data");
      return false;
    }

  // The block is fine.
  return true;
}

