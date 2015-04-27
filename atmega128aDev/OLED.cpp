/*
 * OLED.cpp
 *
 * Created: 27-04-2015 11:30:34 PM
 *  Author: Ajinkya
 */ 
#include "OLED.h"
OLED::OLED()
{
	initPins();
	initLED();
}

void OLED::write(char data,bool isData)
{
	if(isData==false)
	{
		//execute command
		//make DC pin low
	}
	else
	{
		//execute data
		//make DC pin high
	}
	
	//make scl low
	for(int i=0;i<8;i++)
	{
		if()
	}
}