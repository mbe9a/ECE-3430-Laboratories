/*
 * Michael Eller mbe9a
 * ECE 3430 DAC Lab
 * This program uses the LTC2604 chip to generate a square wave
 * with four sinusoids.
 */


#include <msp430.h> 
#include "init.h"
#include "ArrayValues.h"
#include "DAC.h"

unsigned int index_32 = 0;
unsigned int index_224 = 0;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;											// Stop watchdog timer

    InitHardware();														// Defined in "Init.h"

	_BIS_SR(GIE);														// Enable all interrupts

	while(1){

	}

	return 0;
}

#pragma vector = TIMER0_A0_VECTOR  										// Timer A interrupt service routine

__interrupt void Timer_A0_routine(void) {								// Main program here
	if(index_32 > 31) index_32 = 0;										// Loop indices
	if(index_224 > 223) index_224 = 0;

	CS_LOW;																// Drive low to enable SCK
	SendByte(DACA);														// Send command and address first
	SendByte(getMSB(array700[index_32]));								// Send most significant byte
	SendByte(getLSB(array700[index_32]));								// Then send least significant byte
	CS_HIGH;															// Drive high to complete operation

	CS_LOW;																// Repeat for each DAC
	SendByte(DACB);
	SendByte(getMSB(array100[index_224]));
	SendByte(getLSB(array100[index_224]));
	CS_HIGH;

	CS_LOW;
	SendByte(DACC);
	SendByte(getMSB(array300[index_224]));
	SendByte(getLSB(array300[index_224]));
	CS_HIGH;

	CS_LOW;
	SendByte(DACD);
	SendByte(getMSB(array500[index_224]));
	SendByte(getLSB(array500[index_224]));
	CS_HIGH;

	index_32 += 1;														// Inncrement indices
	index_224 += 1;
}
