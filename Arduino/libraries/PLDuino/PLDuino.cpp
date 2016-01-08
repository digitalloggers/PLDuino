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

		pinMode(ESP_RST, OUTPUT); digitalWrite(ESP_RST, LOW);
		delay(250);
		pinMode(ESP_RST, OUTPUT); digitalWrite(ESP_RST, HIGH);
	}


	void enableLCD()
	{
	    pinMode(TOUCH_IRQ, INPUT);

	    pinMode(LCD_PWR, OUTPUT); digitalWrite(LCD_PWR, LOW);
	    pinMode(LCD_BACKLIGHT, OUTPUT); digitalWrite(LCD_BACKLIGHT, HIGH);

	    pinMode(TOUCH_CS, OUTPUT); digitalWrite(TOUCH_CS, HIGH); 

	    delay(1500); // wait a little
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
		pinMode(DIN1, INPUT); digitalWrite(DIN1, HIGH);
		pinMode(DIN2, INPUT); digitalWrite(DIN2, HIGH);
		pinMode(DIN3, INPUT); digitalWrite(DIN3, HIGH);
		pinMode(DIN4, INPUT); digitalWrite(DIN4, HIGH);
		pinMode(DIN5, INPUT); digitalWrite(DIN5, HIGH);
		pinMode(DIN6, INPUT); digitalWrite(DIN6, HIGH);
		pinMode(DIN7, INPUT); digitalWrite(DIN7, HIGH);
		pinMode(DIN8, INPUT); digitalWrite(DIN8, HIGH);

		pinMode(AIN1, INPUT); digitalWrite(AIN1, LOW);
		pinMode(AIN2, INPUT); digitalWrite(AIN2, LOW);
		pinMode(AIN3, INPUT); digitalWrite(AIN3, LOW);
		pinMode(AIN4, INPUT); digitalWrite(AIN4, LOW);
		pinMode(AIN5, INPUT); digitalWrite(AIN5, LOW);
		pinMode(AIN6, INPUT); digitalWrite(AIN6, LOW);
		pinMode(AIN7, INPUT); digitalWrite(AIN7, LOW);
		pinMode(AIN8, INPUT); digitalWrite(AIN8, LOW);

		pinMode(DOUT1, OUTPUT);
		pinMode(DOUT2, OUTPUT);
		pinMode(DOUT3, OUTPUT);
		pinMode(DOUT4, OUTPUT);
		pinMode(DOUT5, OUTPUT);
		pinMode(DOUT6, OUTPUT);
		pinMode(DOUT7, OUTPUT);
		pinMode(DOUT8, OUTPUT);

		pinMode(RELAY1, OUTPUT);
		pinMode(RELAY2, OUTPUT);
		pinMode(RELAY3, OUTPUT);
		pinMode(RELAY4, OUTPUT);
		pinMode(RELAY5, OUTPUT);
		pinMode(RELAY6, OUTPUT);

		pinMode(LED_PIN, OUTPUT);

	    DDRH |= (1<<PH2); // BOE on PH2 = output
	    PORTH |= (1<<PH2); // set BOE High
	    delay(200);
	    PORTH &= ~(1<<PH2); // BOE low
	}


} // namespace PLDuino
