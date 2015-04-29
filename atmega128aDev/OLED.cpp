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
	initOLED();
}
void OLED::initOLED()
{
	//unsigned char i;
	//make scl high
	setBit(OLED_scl_port,OLED_scl_pin);
	//make rst low
	clrBit(OLED_rst_port,OLED_rst_pin);
	//delay 50 ms
	_delay_ms(50);
	//make rst high
	setBit(OLED_rst_port,OLED_rst_pin);
	setDisplayOnOff(0x00);		  // Display Off (0x00/0x01)
	setDisplayClock(0x80);		  // Set Clock as 100 Frames/Sec
	setMultiplexRatio(0x3F);	  // 1/64 Duty (0x0F~0x3F)
	setDisplayOffset(0x00);		  // Shift Mapping RAM Counter (0x00~0x3F)
	setStartLine(0x00);			  // Set Mapping RAM Display Start Line (0x00~0x3F)
	setChargePump(0x04);		  // Enable Embedded DC/DC Converter (0x00/0x04)
	setAddressingMode(0x02);	  // Set Page Addressing Mode (0x00/0x01/0x02)
	setSegmentRemap(0x01);	  // Set SEG/Column Mapping
	setCommonRemap(0x08);	  // Set COM/Row Scan Direction
	setCommonConfig(0x10);	  // Set Sequential Configuration (0x00/0x10)
	setContrastControl(0xCF); // Set SEG Output Current
	setPrechargePeriod(0xF1);	  // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	setVCOMH(0x40);			  // Set VCOM Deselect Level
	setEntireDisplay(0x00);		  // Disable Entire Display On (0x00/0x01)
	setInverseDisplay(0x00);	  // Disable Inverse Display On (0x00/0x01)
	setDisplayOnOff(0x01);		  // Display On (0x00/0x01)
	fill(0x00);                               //clear all
	setPos(0,0);
}
void OLED::fill(unsigned char data)
{
	unsigned char x,y;
	for(y=0;y<8;y++)
	{
		writeCmd(0xb0+y);
		writeCmd(0x00);
		writeCmd(0x10);
		for(x=0;x<128;x++)
			writeData(data);
	}
}
void OLED::setPos(unsigned char x,unsigned char y)		//x is the column xE[0,127];y is the page number yE[0,7]
{
	writeCmd(0xb0+y);
	writeCmd(((x&0xF0)>>4)|0x10);		//send high nibble
	writeCmd((x&0x0F)|0x00 );			//send low nibble
}
void OLED::clrscr()
{
	fill(0x00);
}
void OLED::initPins(void)
{
	//set all pins to output
	setBit(OLED_dc_ddr,OLED_dc_pin);
	setBit(OLED_rst_ddr,OLED_rst_pin);
	setBit(OLED_scl_ddr,OLED_scl_pin);
	setBit(OLED_sda_ddr,OLED_sda_pin);
}
void OLED::write(char data,bool isData)
{
	if(isData==false)
	{
		//execute command
		//make DC pin low
		clrBit(OLED_dc_port,OLED_dc_pin);
		
		
	}
	else
	{
		//execute data
		//make DC pin high
		setBit(OLED_dc_port,OLED_dc_pin);
	}
	
	//make scl low
	clrBit(OLED_scl_port,OLED_scl_pin);
	unsigned char i=8;
	while(i--)
	{
		if(data & 0x80)
		{
			//make data pin high
			setBit(OLED_sda_port,OLED_sda_pin);
		}
		else 
		{
			//make data pin low
			clrBit(OLED_sda_port,OLED_sda_pin);
		}
		//delay
		_delay_us(usDelay);
		//make scl pin high
		setBit(OLED_scl_port,OLED_scl_pin);
		//delay
		_delay_us(usDelay);
		//make scl pin low
		clrBit(OLED_scl_port,OLED_scl_pin);
		data<<=1;		//left shift data by 1
	}
	
}
void OLED::cursor(unsigned char column,unsigned char row)
{
	if(row != 0)
	{
		if(column == 1) setPos(0, row + 2);
		else  setPos(80 + (column - 2)*6, row + 2);
		writeData(0xFF);
		writeData(0xFF);
		writeData(0xFF);
		writeData(0xFF);
		writeData(0xFF);
		writeData(0xFF);
	}
}
void OLED::writeData(char data)
{
	write(data,true);
}
void OLED::writeCmd(char cmd)
{
	write(cmd,false);
}

unsigned char OLED::read(void)
{
	char data=0;
	clrBit(OLED_scl_port,OLED_scl_pin);
	setBit(OLED_sda_port,OLED_sda_pin);	//make sda high
	clrBit(OLED_sda_port,OLED_sda_pin);	//make sda input
	for(int i=0 ; i<8 ;i++)
	{
		//make scl high
		setBit(OLED_scl_port,OLED_scl_pin);
		_delay_us(usDelay);//delay
		if(ifHigh(OLED_sda_read,OLED_sda_pin))	//check if sda then update data
		{
			data|=0x01;
		}
		clrBit(OLED_scl_port,OLED_scl_pin);		//make scl low 
		_delay_us(usDelay);						//delay
		data=data<<1;							//left shift data
	}
	//not sending ack!!
	return data;								//return data
}


void OLED::setStartColumn(unsigned char d)
{
	writeCmd(0x00+d%16);	// Set Lower Column Start Address for Page Addressing Mode
	// Default => 0x00
	writeCmd(0x10+d/16);	// Set Higher Column Start Address for Page Addressing Mode
	// Default => 0x10
}

void OLED::setAddressingMode(unsigned char d)
{
	writeCmd(0x20);			// Set Memory Addressing Mode
	writeCmd(d);			// Default => 0x02
	// 0x00 => Horizontal Addressing Mode
	// 0x01 => Vertical Addressing Mode
	// 0x02 => Page Addressing Mode
}

void OLED::setColumnAddress(unsigned char a, unsigned char b)
{
	writeCmd(0x21);			// Set Column Address
	writeCmd(a);			// Default => 0x00 (Column Start Address)
	writeCmd(b);			// Default => 0x7F (Column End Address)
}

void OLED::SetPageAddress(unsigned char a, unsigned char b)
{
	writeCmd(0x22);			// Set Page Address
	writeCmd(a);			// Default => 0x00 (Page Start Address)
	writeCmd(b);			// Default => 0x07 (Page End Address)
}

void OLED::setStartLine(unsigned char d)
{
	writeCmd(0x40|d);		// Set Display Start Line
	// Default => 0x40 (0x00)
}

void OLED::setContrastControl(unsigned char d)
{
	writeCmd(0x81);			// Set Contrast Control
	writeCmd(d);			// Default => 0x7F
}

void OLED::setChargePump(unsigned char d)
{
	writeCmd(0x8D);			// Set Charge Pump
	writeCmd(0x10|d);		// Default => 0x10
	// 0x10 (0x00) => Disable Charge Pump
	// 0x14 (0x04) => Enable Charge Pump
}

void OLED::setSegmentRemap(unsigned char d)
{
	writeCmd(0xA0|d);		// Set Segment Re-Map
	// Default => 0xA0
	// 0xA0 (0x00) => Column Address 0 Mapped to SEG0
	// 0xA1 (0x01) => Column Address 0 Mapped to SEG127
}

void OLED::setEntireDisplay(unsigned char d)
{
	writeCmd(0xA4|d);		// Set Entire Display On / Off
	// Default => 0xA4
	// 0xA4 (0x00) => Normal Display
	// 0xA5 (0x01) => Entire Display On
}

void OLED::setInverseDisplay(unsigned char d)
{
	writeCmd(0xA6|d);		// Set Inverse Display On/Off
	// Default => 0xA6
	// 0xA6 (0x00) => Normal Display
	// 0xA7 (0x01) => Inverse Display On
}

void OLED::setMultiplexRatio(unsigned char d)
{
	writeCmd(0xA8);			// Set Multiplex Ratio
	writeCmd(d);			// Default => 0x3F (1/64 Duty)
}

void OLED::setDisplayOnOff(unsigned char d)
{
	writeCmd(0xAE|d);		// Set Display On/Off
	// Default => 0xAE
	// 0xAE (0x00) => Display Off
	// 0xAF (0x01) => Display On
}

void OLED::setStartPage(unsigned char d)
{
	writeCmd(0xB0|d);		// Set Page Start Address for Page Addressing Mode
	// Default => 0xB0 (0x00)
}

void OLED::setCommonRemap(unsigned char d)
{
	writeCmd(0xC0|d);		// Set COM Output Scan Direction
	// Default => 0xC0
	// 0xC0 (0x00) => Scan from COM0 to 63
	// 0xC8 (0x08) => Scan from COM63 to 0
}

void OLED::setDisplayOffset(unsigned char d)
{
	writeCmd(0xD3);			// Set Display Offset
	writeCmd(d);			// Default => 0x00
}

void OLED::setDisplayClock(unsigned char d)
{
	writeCmd(0xD5);			// Set Display Clock Divide Ratio / Oscillator Frequency
	writeCmd(d);			// Default => 0x80
	// D[3:0] => Display Clock Divider
	// D[7:4] => Oscillator Frequency
}

void OLED::setPrechargePeriod(unsigned char d)
{
	writeCmd(0xD9);			// Set Pre-Charge Period
	writeCmd(d);			// Default => 0x22 (2 Display Clocks [Phase 2] / 2 Display Clocks [Phase 1])
	// D[3:0] => Phase 1 Period in 1~15 Display Clocks
	// D[7:4] => Phase 2 Period in 1~15 Display Clocks
}

void OLED::setCommonConfig(unsigned char d)
{
	writeCmd(0xDA);			// Set COM Pins Hardware Configuration
	writeCmd(0x02|d);		// Default => 0x12 (0x10)
	// Alternative COM Pin Configuration
	// Disable COM Left/Right Re-Map
}

void OLED::setVCOMH(unsigned char d)
{
	writeCmd(0xDB);			// Set VCOMH Deselect Level
	writeCmd(d);			// Default => 0x20 (0.77*VCC)
}

void OLED::setNOP(void)
{
	writeCmd(0xE3);			// Command for No Operation
}



void OLED::P6x8Char(unsigned char x,unsigned char y,unsigned char ch)
{
	unsigned char c=0,i=0;//,j=0;
	
	c =ch-32;
	if(x>122)
	{
		x=0;
		y++;
	}
	setPos(x,y);
	for(i=0;i<6;i++)
	{
		writeData(F6x8[c][i]);
	}
}

void OLED::P6x8Str(unsigned char x,unsigned char y,char ch[])
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x>126)
		{
			x=0;
			y++;
		}
		setPos(x,y);
		for(i=0;i<6;i++)
		{
			writeData(F6x8[c][i]);
		}
		x+=6;
		j++;
	}
}

void OLED::P8x16Str(unsigned char x,unsigned char y,char ch[])
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x>120)
		{
			x=0;
			y++;
		}
		setPos(x,y);
		for(i=0;i<8;i++)
		{
			writeData(F8X16[(c<<4)+i]);
		}
		setPos(x,y+1);
		for(i=0;i<8;i++)
		{
			writeData(F8X16[(c<<4)+i+8]);
		}
		x+=8;
		j++;
	}
}



void OLED::PrintBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,const unsigned char bmp[])
{
	int ii=0;
	unsigned char x,y;
	for(y=y0;y<=y1;y++)
	{
		setPos(x0,y);
		for(x=x0;x<x1;x++)
		{
			writeData(bmp[ii++]);
		}
	}
}

void OLED::PrintValue(unsigned char x, unsigned char y, char data)
{
	unsigned char i,j,k;
	if(data < 0)
	{
		P6x8Char(x,y,'-');
		data = - data;
	}
	else
	{
		P6x8Char(x,y,'+');
	}
	i = data/100;
	j = (data%100)/10;
	k = data%10;
	P6x8Char(x+6,y,i+48);
	P6x8Char(x+12,y,j+48);
	P6x8Char(x+18,y,k+48);
}


void OLED::PrintValue(unsigned char x, unsigned char y, int data)
{
	unsigned char i,j,k,l,m;
	if(data < 0)
	{
		P6x8Char(x,y,'-');
		data = - data;
	}
	else
	{
		P6x8Char(x,y,'+');
	}
	
	l  = data/10000;
	m= (data%10000)/1000;
	i = (data%1000)/100;
	j = (data%100)/10;
	k = data%10;
	P6x8Char(x+6,y,l+48);
	P6x8Char(x+12,y,m+48);
	P6x8Char(x+18,y,i+48);
	P6x8Char(x+24,y,j+48);
	P6x8Char(x+30,y,k+48);
}

void OLED::PrintValue(unsigned char x, unsigned char y, unsigned int data, unsigned char num)
{
	unsigned char m,i,j,k;
	P6x8Char(x, y, '.');
	m= data/1000;
	i = (data%1000)/100;
	j = (data%100)/10;
	k = data%10;
	switch(num)
	{
		case 1:  	P6x8Char(x+6,y,k+48);
		break;
		case 2:  	P6x8Char(x+6,y,j+48);
		P6x8Char(x+12,y,k+48);
		break;
		case 3:	P6x8Char(x+6,y,i+48);
		P6x8Char(x+12,y,j+48);
		P6x8Char(x+18,y,k+48);
		break;
		case 4: 	P6x8Char(x+6,y,m+48);
		P6x8Char(x+12,y,i+48);
		P6x8Char(x+18,y,j+48);
		P6x8Char(x+24,y,k+48);
		break;
	}
}


void OLED::PrintValue(unsigned char x, unsigned char y, float data, unsigned char num)
{
	unsigned char l,m,i,j,k;
	unsigned char databiti = 6;
	unsigned int tempdataui = 0;
	int tempdataii = (int)data;
	long int tempdatalp = (long int)((data - (int)data)*10000);
	

	if(data < 0.0000001)  P6x8Char(x, y,'-');
	else P6x8Char(x, y,'+');
	if(tempdataii < 0)tempdataii = - tempdataii;
	tempdataui = tempdataii;
	l  = tempdataui/10000;
	m= (tempdataui%10000)/1000;
	i = (tempdataui%1000)/100;
	j = (tempdataui%100)/10;
	k = tempdataui%10;
	if (l != 0)
	{
		P6x8Char(x+6,y,l+48);
		P6x8Char(x+12,y,m+48);
		P6x8Char(x+18,y,i+48);
		P6x8Char(x+24,y,j+48);
		P6x8Char(x+30,y,k+48);
	}
	else if(m != 0)
	{
		databiti = 5;
		P6x8Char(x+6,y,m+48);
		P6x8Char(x+12,y,i+48);
		P6x8Char(x+18,y,j+48);
		P6x8Char(x+24,y,k+48);
	}
	else if(i != 0)
	{
		databiti = 4;
		P6x8Char(x+6,y,i+48);
		P6x8Char(x+12,y,j+48);
		P6x8Char(x+18,y,k+48);
	}
	else if(j != 0)
	{
		databiti = 3;
		P6x8Char(x+6,y,j+48);
		P6x8Char(x+12,y,k+48);
	}
	else
	{
		databiti = 2;
		P6x8Char(x+6,y,k+48);
	}
	if(tempdatalp < 0)tempdatalp = - tempdatalp;
	switch(num)
	{
		case 0: break;
		case 1:  PrintValue(x + databiti * 6, y, (unsigned int)(tempdatalp / 1000),num);break;
		case 2:  PrintValue(x + databiti * 6, y, (unsigned int)(tempdatalp / 100),num);break;
		case 3:  PrintValue(x + databiti * 6, y, (unsigned int)(tempdatalp / 10),num);break;
		case 4:  PrintValue(x + databiti * 6, y, (unsigned int)(tempdatalp),num);break;
	}
}