#pragma once
#include "Utils.h"
#include "Label.h"

namespace PLDuinoGUI
{

	Label::Label (
		const String &text,
		uint16_t textcolor,
		uint16_t background,
		unsigned char alignmode
	)
		: Widget(textcolor, background),
		  text_(text),
		  alignmode_(alignmode),
		  font_size_(FONTSIZE)
	{
	}

	
	void Label::updateText (const String &newtext, Adafruit_GFX &tft)
	{
		if (newtext != text_)
			erase(tft), text_ = newtext, draw(tft);
	}


	void Label::updateTextAndColor (const String &newtext, uint16_t newcolor, Adafruit_GFX &tft)
	{
		if (newtext != text_ || color_ != newcolor)
			erase(tft), text_=newtext, color_=newcolor, draw(tft);
	}


	void Label::draw(Adafruit_GFX &tft) const
	{
		Utils::drawAlignedText(
			tft,
			text_,
			x(), y(), width(), height(),
			color_,
			font_size_,
			alignmode_
		);
	}
	

	void Label::erase (Adafruit_GFX &tft) const
	{
		Utils::drawAlignedText(
			tft,
			text_,
			x(), y(), width(), height(),
			backcolor_,
			font_size_,
			alignmode_
		);
	}


} // namespace
