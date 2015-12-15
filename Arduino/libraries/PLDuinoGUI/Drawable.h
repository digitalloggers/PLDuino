#pragma once
#include <Arduino.h>
#include "Adafruit_GFX.h"


namespace PLDuinoGUI
{

	class Drawable
	{
	public:
		virtual void draw (Adafruit_GFX &tft) const = 0;
		virtual void erase (Adafruit_GFX &tft) const = 0;
	};


} // namespace
