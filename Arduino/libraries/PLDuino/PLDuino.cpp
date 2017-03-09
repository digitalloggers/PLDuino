#include "PLDuino.h"


namespace PLDuino
{

	void disableESP()
	{
		pinMode(ESP_CHPD, INPUT); digitalWrite(ESP_CHPD, HIGH);
		pinMode(ESP_GPIO0, INPUT); digitalWrite(ESP_GPIO0, HIGH);
		pinMode(ESP_GPIO2, INPUT); digitalWrite(ESP_GPIO2, HIGH);
		pinMode(ESP_RST, INPUT); digitalWrite(ESP_RST, HIGH);
		pinMode(ESP_PWR, OUTPUT); digitalWrite(ESP_PWR, LOW);
	}


	void enableESP()
	{
		pinMode(ESP_CHPD, OUTPUT); digitalWrite(ESP_CHPD, HIGH);
		pinMode(ESP_GPIO0, OUTPUT); digitalWrite(ESP_GPIO0, HIGH);
		pinMode(ESP_GPIO2, OUTPUT); digitalWrite(ESP_GPIO2, HIGH);
		pinMode(ESP_RST, OUTPUT); digitalWrite(ESP_RST, HIGH);
		pinMode(ESP_PWR, OUTPUT); digitalWrite(ESP_PWR, HIGH);
		resetESP();
	}


	void resetESP()
	{
		digitalWrite(ESP_RST, LOW);
		digitalWrite(ESP_CHPD, LOW);
		delay(250);
		digitalWrite(ESP_CHPD, HIGH);
		digitalWrite(ESP_RST, HIGH);
	}


	void enableLCD()
	{
	    pinMode(TOUCH_IRQ, INPUT);

	    pinMode(LCD_PWR, OUTPUT); digitalWrite(LCD_PWR, LOW);
	    pinMode(LCD_BACKLIGHT, OUTPUT); digitalWrite(LCD_BACKLIGHT, HIGH);

	    pinMode(TOUCH_CS, OUTPUT); digitalWrite(TOUCH_CS, HIGH); 

	    delay(1500); // give LCD enough time to reset completely
	    pinMode(LCD_PWR, OUTPUT); digitalWrite(LCD_PWR, HIGH);
	    
	    delay(250);
	    digitalWrite(LCD_RST, LOW);
	    delay(50);
	    digitalWrite(LCD_RST, HIGH);
	    delay(50);

	    digitalWrite(LCD_BACKLIGHT, LOW);
	}


	void init()
	{
		// Setup digital input pins
		for(int i=0; i<8; ++i)
		{
			pinMode(30+i, INPUT);
			digitalWrite(30+i, HIGH);
		}

		// Setup analog input pins
		for(int i=0; i<8; ++i)
		{
			pinMode(A0+i, INPUT);
			digitalWrite(A0+i, LOW);
		}

		// Setup digital output pins
		for(int i=0; i<8; ++i)
			pinMode(38+i, OUTPUT);

		// Setup relays
		pinMode(RELAY1, OUTPUT);
		pinMode(RELAY2, OUTPUT);
		pinMode(RELAY3, OUTPUT);
		pinMode(RELAY4, OUTPUT);
		pinMode(RELAY5, OUTPUT);
		pinMode(RELAY6, OUTPUT);

		// Setup LED
		pinMode(LED_PIN, OUTPUT);

		// Enable 5v-3.3v converter
	    DDRH |= (1<<PH2); // BOE on PH2 = output
	    PORTH |= (1<<PH2); // set BOE High
	    delay(200);
	    PORTH &= ~(1<<PH2); // BOE low
	    
		pinMode(PLDuino::LCD_CS, OUTPUT);
		digitalWrite(PLDuino::LCD_CS, HIGH);
	}


} // namespace PLDuino
