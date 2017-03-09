#pragma once
#include "Adafruit_GFX.h"
#include "Settings.h"


namespace PLDuinoGUI
{

	// text alignment flags
	const int HORZ_CENTER 	= (1 << 0);
	const int HORZ_LEFT		= (1 << 1);
	const int HORZ_RIGHT	= (1 << 2);
	const int VERT_CENTER	= (1 << 3);
	const int VERT_BOTTOM	= (1 << 4);
	const int VERT_TOP		= (1 << 5);


	namespace Utils
	{
		void drawButton(
			Adafruit_GFX &tft,
			uint16_t x, uint16_t y,
			uint16_t width, uint16_t height,
			const String &text,
			uint16_t color,
			uint16_t textcolor,
			bool transparent
		);
		void drawAlignedText(
			Adafruit_GFX &tft,
			const String &text,
			uint16_t x, uint16_t y,
			uint16_t width, uint16_t height,
			uint16_t color,
			unsigned char textsize = FONTSIZE,
			unsigned char alignMode = 0//HORZ_CENTER | VERT_CENTER
		);
	};

	inline uint16_t getTextWidth (const String &text, unsigned char size) { return CHARWIDTH * size * text.length() - 1; }
	inline uint16_t getTextHeight (const String &text, unsigned char size) { return CHARHEIGHT * size; }


} // namespace
