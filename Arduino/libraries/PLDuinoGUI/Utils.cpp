#include <Arduino.h>
#include "Utils.h"
#include "Adafruit_GFX.h"

namespace PLDuinoGUI
{

	void Utils::drawButton (
		Adafruit_GFX &tft,
		uint16_t x, uint16_t y,
		uint16_t width, uint16_t height,
		const String &text,
		uint16_t color,
		uint16_t textcolor,
		bool transparent
	)
	{
		if (!transparent) tft.fillRoundRect(x, y, width, height, 9, color);
		drawAlignedText(tft, text, x, y, width, height, textcolor, FONTSIZE, VERT_CENTER|HORZ_CENTER);
	}


	void Utils::drawAlignedText (
		Adafruit_GFX &tft,
		const String &text,
		uint16_t x, uint16_t y,
		uint16_t width, uint16_t height,
		uint16_t color,
		unsigned char textsize,
		unsigned char alignMode
	)
	{
		uint16_t textwidth = getTextWidth(text, textsize);
		uint16_t textx = 0;	
		if (alignMode & HORZ_CENTER) textx = x + (width - textwidth)/2;
		else if (alignMode & HORZ_LEFT) textx = x;
		else textx = x + width - textwidth;

		uint16_t textheight = getTextHeight(text, textsize);
		uint16_t texty = 0;
		if (alignMode & VERT_CENTER) texty = y + (height - textheight)/2;
		else if (alignMode & VERT_TOP) texty = y;
		else texty = y + height - textheight;

		tft.setCursor(textx, texty);
		tft.setTextColor(color);
		tft.setTextSize(textsize);
		tft.print(text);
	}


} // namespace