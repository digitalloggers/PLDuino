#include "Layout.h"

namespace PLDuinoGUI
{

	Layout::Layout() : numchildren(0)
	{
	}


	Layout::Layout (Sizable **children)
	{
		setChildren(children);
	}


	void Layout::setChildren (Sizable **children)
	{
		numchildren = 0;
		while(children[numchildren])
			++numchildren;
		this->children = children;
	}


	void Layout::setPositionAndSize (uint16_t newx, uint16_t newy, uint16_t newwidth, uint16_t newheight)
	{
		Sizable::setPositionAndSize(newx, newy, newwidth, newheight);
		doLayout(newx, newy, newwidth, newheight);
	}


} // namespace 
