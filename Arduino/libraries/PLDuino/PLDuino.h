#pragma once
#include <Arduino.h>

namespace PLDuino
{
	enum Pin
	{
		RELAY1 = 2,
		RELAY2 = 3,
		RELAY3 = 4,
		RELAY4 = 5,
		RELAY5 = 6,
		RELAY6 = 7,

		UART_RXD = 14,
		UART_TXD = 15,

		SD_CS = 53,

		LED_PIN = 13,

		SPEAKER_PIN = 9,

		RTC_INT = 19,
		RTC_SDA = 20,
		RTC_SCL = 21,

		LCD_PWR 	= 27,
		LCD_DC 		= 49,
		LCD_CS 		= 47,
		LCD_MOSI 	= 51,
		LCD_CLK 	= 52,
		LCD_RST 	= 48,
		LCD_MISO 	= 50,
		LCD_BACKLIGHT = 46,

		TOUCH_IRQ = 18,
		TOUCH_DO  = 50,
		TOUCH_DIN = 51,
		TOUCH_CS  = 28,
		TOUCH_CLK = 52,

		ESP_PWR		= 26,
		ESP_CHPD	= 25,
		ESP_GPIO0	= 24,
		ESP_GPIO2	= 23,
		ESP_RST		= 22
	};

	void enableESP();
	void disableESP();
	void resetESP();

	void enableLCD(); // also powers on SD card
	void disableLCD();

	void init();


} // namespace PLDuino
