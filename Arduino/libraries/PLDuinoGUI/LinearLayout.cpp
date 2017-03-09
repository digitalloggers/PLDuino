#pragma once
#include "LinearLayout.h"


namespace PLDuinoGUI
{

	LinearLayout::LinearLayout (
		Sizable **children,
		bool is_vertical_,
		uint16_t spacing_,
		uint16_t padding_
	)
		: Layout(children),
		  spacing(spacing_),
		  padding(padding_),
		  is_vertical(is_vertical_)
	{
	}


	void LinearLayout::doLayout (uint16_t newx, uint16_t newy, uint16_t newwidth, uint16_t newheight)
	{
		if (is_vertical)
		{
			uint16_t ctlheight = (newheight - 2*padding - (numchildren-1)*spacing) / numchildren;
			for(int i=0; i<numchildren; ++i)
				children[i]->setPositionAndSize(
					newx + padding,
					newy + i*(ctlheight+spacing) + padding,
					newwidth - 2*padding,
					ctlheight
				);
		}
		else
		{
			uint16_t ctlwidth = (newwidth - 2*padding - (numchildren-1)*spacing) / numchildren;
			for(int i=0; i<numchildren; ++i)
				children[i]->setPositionAndSize(
					newx + i*(ctlwidth+spacing) + padding,
					newy + padding,
					ctlwidth,
					newheight - 2*padding
				);
		}
	}


} // namespace 
