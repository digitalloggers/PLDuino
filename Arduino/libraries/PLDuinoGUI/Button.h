#pragma once
#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "Widget.h"

namespace PLDuinoGUI
{

	class Button: public Widget
	{
	public:
		Button(
			const String &text,
			uint16_t color,
			uint16_t text_color = ILI9341_WHITE,
			uint16_t backcolor = ILI9341_BLACK,
			bool is_transparent = false
		);
		void draw (Adafruit_GFX &tft) const;
		void setTextColor (uint16_t color) { textcolor_ = color; }
		void setText (const String &text) { text_ = text; }
	private:
		bool is_transparent_;
		uint16_t textcolor_;
		String text_;
	};


} // namespace
