#pragma once
#include "GridLayout.h"

namespace PLDuinoGUI
{

	GridLayout::GridLayout (
		Sizable **children,
		uint16_t numcols,
		uint16_t spacing, uint16_t padding,
		const float *cw,
		const float *rw
	)
		: Layout(children),
		  rowweights(rw),
		  colweights(cw)
	{
		this->numcols = numcols;
		this->padding = padding;
		this->spacing = spacing;
	}


	void GridLayout::doLayout (uint16_t newx, uint16_t newy, uint16_t newwidth, uint16_t newheight)
	{
		uint16_t numrows = (numchildren / numcols) + (numchildren % numcols? 1 : 0);

		float colsum = 0;
		for(int i=0; i<numcols; ++i)
			colsum += colWeight(i);
		
		float rowsum = 0;
		for(int i=0; i<numrows; ++i)
			rowsum += rowWeight(i);

		uint16_t totalcolswidth = (newwidth - 2*padding - ((numcols-1)*spacing));
		uint16_t totalrowsheight = (newheight - 2*padding - ((numrows-1)*spacing));
		uint16_t i = 0;
		uint16_t rowpos = padding + newy;
		for(int r=0; r<numrows; ++r)
		{
			uint16_t colpos = padding + newx;
			for(int c=0; c<numcols; ++c)
			{
				int ctlwidth = colWeight(c) * (totalcolswidth / colsum);
				int ctlheight = rowWeight(r) * (totalrowsheight / rowsum);
				children[i++]->setPositionAndSize(colpos, rowpos, ctlwidth, ctlheight);
				if (i==numchildren) goto done;
				colpos += colWeight(c) * (totalcolswidth / colsum) + spacing;
			}
			rowpos += rowWeight(r) * (totalrowsheight / rowsum) + spacing;
		}
		done: return;
	}


	float GridLayout::colWeight (uint16_t col) const
	{
		return (colweights? colweights[col] : 1);
	}


	float GridLayout::rowWeight (uint16_t row) const
	{
		return (rowweights? rowweights[row] : 1);
	}


} // namespace
