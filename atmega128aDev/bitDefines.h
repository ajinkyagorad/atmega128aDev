/*
 * bitDefines.h
 *
 * Created: 28-04-2015 01:13:09 AM
 *  Author: Ajinkya
 */ 


#ifndef BITDEFINES_H_
#define BITDEFINES_H_

#define setBit(reg,bit)		reg|=(1<<bit)
#define clrBit(reg,bit)		reg&=~(1<<bit)
#define toggleBit(reg,bit)	reg^=(1<<bit)
#define chkBit(reg,bit)		reg&(1<<bit)
#define ifHigh(reg,bit)		reg&(1<<bit)
#define ifLow(reg,bit)		~(reg&(1<<bit))


#endif /* BITDEFINES_H_ */