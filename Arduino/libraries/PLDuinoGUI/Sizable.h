#pragma once
#include <Arduino.h>

namespace PLDuinoGUI
{

	class Sizable
	{
	public:
		// override this
		inline virtual void setPositionAndSize (uint16_t newx, uint16_t newy, uint16_t newwidth, uint16_t newheight)
		{
			m_x = newx, m_y = newy;
			m_width = newwidth, m_height = newheight;
		}
	public:
		Sizable() : m_x(0), m_y(0), m_width(80), m_height(25) {}
		Sizable (uint16_t x, uint16_t y, uint16_t width, uint16_t height)
			: m_x(x), m_y(y), m_width(width), m_height(height) {}
	public:
		inline void setSize (uint16_t newwidth, uint16_t newheight) { setPositionAndSize(m_x, m_y, newwidth, newheight); }
		inline void setPosition (uint16_t newx, uint16_t newy) { setPositionAndSize(newx, newy, m_width, m_height); }
		// setters: implemented via setPositionAndSize
		inline void setWidth (uint16_t newwidth) { setPositionAndSize(m_x, m_y, newwidth, m_height); }
		inline void setHeight (uint16_t newheight) { setPositionAndSize(m_x, m_y, m_width, newheight); }
		inline void setX (uint16_t newx) { setPositionAndSize(newx, m_y, m_width, m_height); }
		inline void setY (uint16_t newy) { setPositionAndSize(m_x, newy, m_width, m_height); }
		// getters
		inline uint16_t x() const { return m_x; }
		inline uint16_t y() const { return m_y; }
		inline uint16_t width() const { return m_width; }
		inline uint16_t height() const { return m_height; }
	private:
		uint16_t m_x, m_y, m_width, m_height;
	};


} // namespace
