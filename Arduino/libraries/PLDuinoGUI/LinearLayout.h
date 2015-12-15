#pragma once
#include "Layout.h"


namespace PLDuinoGUI
{

	class LinearLayout : public Layout
	{
	protected:
		uint16_t padding, spacing;
		bool is_vertical;
	public:
		LinearLayout (
			Sizable **children,
			bool is_vertical_ = true,
			uint16_t spacing_ = 6,
			uint16_t padding_ = 0
		);
	protected:
		void doLayout (uint16_t newx, uint16_t newy, uint16_t newwidth, uint16_t newheight);
	};


} // namespace 
