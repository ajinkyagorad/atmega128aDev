/*
 * atmega128aDev.cpp
 *
 * Created: 27-04-2015 11:28:38 PM
 *  Author: Ajinkya
 */ 

#include "include.h"

#include "OLED.h"

int main(void)
{
	_delay_ms(50);
	OLED lcd;
    while(1)
    {
			
			lcd.initPins();
			lcd.initOLED();
			lcd.fill(0xFF);
			_delay_ms(200);
			lcd.PrintBMP(0,0,127,7,bitmap);
			_delay_ms(200);
			/*DDRC=0xAA;
			PORTC=0xFF;
			_delay_ms(200);
			PORTC=0x00;
			_delay_ms(200);*/
				
     }
}