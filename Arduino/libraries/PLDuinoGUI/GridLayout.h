#pragma once
#include "Layout.h"


namespace PLDuinoGUI
{

	class GridLayout : public Layout
	{
		uint16_t numcols;
		uint16_t spacing, padding;
		const float *rowweights, *colweights;
	public:
		GridLayout (
			Sizable **children,
			uint16_t numcols,
			uint16_t spacing = 6, uint16_t padding = 0,
			const float *cw = NULL,
			const float *rw = NULL
		);
	protected:
		// override this to provide custom layouts
		virtual void doLayout (uint16_t newx, uint16_t newy, uint16_t newwidth, uint16_t newheight);
	private:
		float colWeight (uint16_t col) const;
		float rowWeight (uint16_t row) const;
	};


} // namespace
