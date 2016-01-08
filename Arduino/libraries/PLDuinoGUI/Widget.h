#pragma once
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "Sizable.h"
#include "Drawable.h"
#include "Settings.h"


namespace PLDuinoGUI
{

	class Widget: public Sizable, public Drawable
	{
	public:
		// Expected to be used with Sizers, so no xywh are passed
		Widget (uint16_t forecolor = ILI9341_WHITE, uint16_t backcolor = ILI9341_BLACK);

		// Drawable
		virtual void erase (Adafruit_GFX &tft) const;
		
		inline uint16_t color() const { return color_; }
		inline void setColor (uint16_t forecolor) { color_ = forecolor; }
		inline uint16_t backcolor() const { return backcolor_; }
		inline void setBackColor (uint16_t backcolor) { backcolor_ = backcolor; }
		inline void setColors (uint16_t forecolor, uint16_t backcolor) { color_ = forecolor, backcolor_ = backcolor; }

		virtual bool isTouched (uint16_t touchx, uint16_t touchy) const;

	protected:
		uint16_t color_, backcolor_;
	};


} // namespace
