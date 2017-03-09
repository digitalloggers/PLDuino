#include <Arduino.h>
#include "Button.h"
#include "Utils.h"

namespace PLDuinoGUI
{

	Button::Button (const String &text, uint16_t color, uint16_t text_color, uint16_t backcolor, bool is_transparent)
		: Widget(color,backcolor), textcolor_(text_color), text_(text), is_transparent_(is_transparent)
	{
	}


	void Button::draw (Adafruit_GFX &tft) const
	{
		Utils::drawButton(tft, x(), y(), width(), height(), text_, color_, textcolor_, is_transparent_);
	}


} // namespace
