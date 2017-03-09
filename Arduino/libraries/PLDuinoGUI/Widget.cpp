#pragma once
#include <Arduino.h>
#include "Widget.h"
#include "Settings.h"

namespace PLDuinoGUI
{

	Widget::Widget (uint16_t forecolor, uint16_t backcolor)
		: color_(forecolor), backcolor_(backcolor)
	{
	}


	void Widget::erase (Adafruit_GFX &tft) const
	{
		tft.fillRect(x(), y(), width(), height(), backcolor_);
	}


	bool Widget::isTouched (uint16_t touchx, uint16_t touchy) const
	{
		return touchx >= x() && touchx <= (x() + width())
			&& touchy >= y() && touchy <= (y() + height());
	}


} // namespace
