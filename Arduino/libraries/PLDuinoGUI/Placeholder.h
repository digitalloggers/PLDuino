#pragma once
#include "Widget.h"

namespace PLDuinoGUI
{

	class Placeholder: public Widget
	{
	public:
		Placeholder() : Widget(ILI9341_WHITE, ILI9341_BLACK) {}
		void draw (Adafruit_GFX &tft) const {}
	};


} // namespace 
