#include <Arduino.h>
#include "Button.h"
#include "Utils.h"

namespace PLDuinoGUI
{

	Button::Button (const String &text, uint16_t color, uint16_t text_color, uint16_t backcolor)
		: Widget(color,backcolor), textcolor_(text_color), text_(text)
	{
	}


	void Button::draw (Adafruit_GFX &tft) const
	{
		Utils::drawButton(tft, x(), y(), width(), height(), text_, color_, textcolor_);
	}


} // namespace
