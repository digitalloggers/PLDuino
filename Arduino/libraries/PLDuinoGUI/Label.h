#pragma once
#include "Widget.h"
#include "Utils.h"

namespace PLDuinoGUI
{

	class Label : public Widget
	{
	public:
		Label (
			const String &text,
			uint16_t textcolor = ILI9341_WHITE,
			uint16_t background = ILI9341_BLACK,
			unsigned char alignmode = HORZ_LEFT|VERT_CENTER
		);
		virtual void setFontSize (unsigned char size) { font_size_ = size; }
		virtual unsigned char fontSize() const { return font_size_; }
		virtual void updateText (const String &newtext, Adafruit_GFX &tft);
		virtual void updateTextAndColor (const String &newtext, uint16_t newcolor, Adafruit_GFX &tft);
		virtual void draw(Adafruit_GFX &tft) const;
		virtual void erase (Adafruit_GFX &tft) const;
	private:
		unsigned char font_size_;
		String text_;
		unsigned char alignmode_;
	};


} // namespace
