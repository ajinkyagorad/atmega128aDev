/*
 * OLED.h
 *
 * Created: 27-04-2015 11:30:48 PM
 *  Author: Ajinkya
 */ 


#ifndef OLED_H_
#define OLED_H_
#include "include.h"
#include "lookupOLED.h"
//reference  http://www.buydisplay.com/download/democode/ER-OLED0.96-1_I2C_DemoCode.txt
//datasheet http://www.buydisplay.com/download/ic/SSD1306.pdf
#define OLED_dc_ddr		DDRC
#define OLED_rst_ddr	DDRC
#define OLED_sda_ddr	DDRC
#define OLED_scl_ddr	DDRC


#define OLED_dc_port	PORTC
#define OLED_rst_port	PORTC
#define OLED_sda_port	PORTC
#define OLED_scl_port	PORTC

#define OLED_dc_pin		0
#define OLED_rst_pin	1
#define OLED_sda_pin	2
#define OLED_scl_pin	3

#define OLED_dc_read	PINC
#define OLED_rst_read	PINC
#define OLED_sda_read	PINC
#define OLED_scl_read	PINC
class OLED{
	
	public:
		OLED();
		void initPins(void);
		void initOLED(void);
		void write(char data,bool isData);
		unsigned char read(void);
		void writeData(char data);
		void writeCmd(char cmd);
		void fill(unsigned char data);
		void setPos(unsigned char x,unsigned char y);
		void clrscr(void);
		void cursor(unsigned char column,unsigned char row);
		
		void P6x8Char(unsigned char x,unsigned char y,unsigned char ch);
		void P6x8Str(unsigned char x,unsigned char y,char ch[]);
		void P8x16Str(unsigned char x,unsigned char y,char ch[]);
		void PrintBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,const unsigned char bmp[]);
		void PrintValue(unsigned char x, unsigned char y, char data);
		void PrintValue(unsigned char x, unsigned char y, int data);
		void PrintValue(unsigned char x, unsigned char y, unsigned int data, unsigned char num);
		void PrintValue(unsigned char x, unsigned char y, float data, unsigned char num);
		
		
	private:
		void setStartColumn(unsigned char d);
		void setAddressingMode(unsigned char d);
		void setColumnAddress(unsigned char a, unsigned char b);
		void SetPageAddress(unsigned char a, unsigned char b);
		void setStartLine(unsigned char d);
		void setContrastControl(unsigned char d);
		void setChargePump(unsigned char d);
		void setSegmentRemap(unsigned char d);
		void setEntireDisplay(unsigned char d);
		void setInverseDisplay(unsigned char d);
		void setMultiplexRatio(unsigned char d);
		void setDisplayOnOff(unsigned char d);
		void setStartPage(unsigned char d);
		void setCommonRemap(unsigned char d);
		void setDisplayOffset(unsigned char d);
		void setDisplayClock(unsigned char d);
		void setPrechargePeriod(unsigned char d);
		void setCommonConfig(unsigned char d);
		void setVCOMH(unsigned char d);
		void setNOP(void);
		
	private:
		static const int usDelay=5;
		
		
	
	
};



#endif /* OLED_H_ */