#pragma once
#include "Sizable.h"

namespace PLDuinoGUI
{

	class Layout : public Sizable
	{
	public:
		Layout();
		Layout (Sizable **children);
	public:
		void setChildren (Sizable **children);
		virtual void setPositionAndSize (uint16_t newx, uint16_t newy, uint16_t newwidth, uint16_t newheight);
	protected:
		virtual void doLayout (uint16_t newx, uint16_t newy, uint16_t newwidth, uint16_t newheight) = 0;
		Sizable **children;
		uint16_t numchildren;
	};


} // namespace
