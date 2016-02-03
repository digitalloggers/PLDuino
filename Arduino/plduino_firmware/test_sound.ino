#include <Arduino.h>
#include <TMRpcm_PLDuino.h>
#include <SPI.h>
#include "utils.h"

void testSound()
{
  tmrpcm.speakerPin = SPEAKER_PIN;

  Label lblTitle("Speaker test", ILI9341_WHITE, ILI9341_BLACK);
  lblTitle.setPositionAndSize(0,0,320, 40); lblTitle.draw(tft);
  
  Label lblPlay("PLAYING A TEST SOUND", ILI9341_GREEN, ILI9341_BLACK, VERT_CENTER|HORZ_CENTER);
  lblPlay.setPositionAndSize(0, 50, 320, 70); lblPlay.draw(tft);
  
  tone(SPEAKER_PIN, 1000); delay(500); noTone(SPEAKER_PIN);

  tmrpcm.play("part1.wav"); while(tmrpcm.isPlaying());
}

