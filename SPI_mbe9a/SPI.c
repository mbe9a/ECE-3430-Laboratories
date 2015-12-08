/*
 * SPI.c
 *
 *  Created on: Oct 4, 2015
 *      Author: Michael Eller mbe9a
 */

#include "SPI.h"
#include "lfsr.h"
#include "led.h"
#include <msp430.h>

void chipEnable() {
	P1OUT &= ~CE;							// drive low
}

void chipDisable() {
	P1OUT |= CE;							// drive high
}

void chipErase() {							// send chip erase op-code
	sendData(CLEAR);
}

void write_Enable() {						// send write enable op-code
	sendData(WE);
}

void write_Disable() {						// send write disable op-code
	sendData(WD);
}

unsigned char readData() {					// form 8-bit char from chip output
	unsigned char output = 0x00;			// initialize to 0
	int i;
	for(i = 0; i < 8; i++) {
		P1OUT &= ~SCK;						// drive clock low
		output <<= 1;						// shift left
		output &= ~0x01;					// set to 0 by default
		if(P1IN & 0x02) output |= 0x01;		// set to 1 if the bit received is 1
		P1OUT |= SCK;						// drive clock high
	}
	return output;							// return 8-bit char
}

void sendData(unsigned char data) {			// send 'data' in single bits
	int i;
	for(i = 0; i < 8; i++){
		P1OUT &= ~SCK;						// drive clock low
		P1OUT &= ~SI;						// set the input to low by default
		if(data & 0x80) P1OUT |= SI;		// set the input to high if the input bit is 1
		data <<= 1;							// left shift
		P1OUT |= SCK;						// drive clock high
	}
}

void initializeSPI() {
    P1DIR |= SCK;							// clock is output
    P1DIR |= CE;							// chip enable is output
    P1DIR |= SI;							// chip input is board output
    P1DIR &= ~SO;							// chip output is board input
    P1OUT &= ~SI;							// initialize input to low
    P1OUT &= ~SCK;							// initialize clock to low
    P1OUT |= CE;							// initialize chip enable to high
    P2DIR |= HOLD;							// hold is output
    P2OUT |= HOLD;							// drive hold pin to high
    P2DIR |= WP;							// write protection is an output
    P2OUT |= WP;							// initialize write protection to high
    InitializeLEDs(1,1);					// set LEDs as outputs
    SetLEDstate(RED, 0);
    SetLEDstate(GREEN, 0);
}

void writeSR(unsigned char data) {
	chipEnable();							// enable chip
	sendData(EWSR);							// enable writing to status-register
	chipDisable();							// disable chip
	chipEnable();							// enable chip to start new process
	sendData(WSR);							// send write-status-register
	sendData(data);							// send data to write
	chipDisable();							// disable chip
}

unsigned char readSR() {
	chipEnable();
	sendData(RSR);							// send read-status-register code
	unsigned char sr = readData();			// sr is read from status register
	chipDisable();
	return sr;
}

void wait_while_busy() {
	unsigned char busy = (readSR() & BIT1); // get busy bit
	while(busy == 1) {
		busy = (readSR() & BIT1);			// check busy bit
	}
}

void AAI() {
	chipEnable();
	sendData(0xAF);
	sendData(0x00);
	sendData(0x00);
	sendData(0x00);

	unsigned int x = LFSR(INITIAL_STATE);		// 2 bytes
	unsigned char data = x;						// 1 byte

	sendData(data);
	chipDisable();
	wait_while_busy();

	while(readSR() & BIT1){
		chipEnable();

		x = LFSR(x);
		data = x;

		sendData(0xAF);
		sendData(data);
		chipDisable();
		wait_while_busy();
	}

	chipEnable();
	sendData(0x04);
	chipDisable();
}

void readAll() {
	chipEnable();
	sendData(READ);
	sendData(0x00);								// initial address
	sendData(0x00);
	sendData(0x00);

	unsigned int x = LFSR(INITIAL_STATE);
	unsigned char data = x;

	SetLEDstate(GREEN, 1);
	int i;
	for (i = 0; i < 65536; i++) {
		x = LFSR(x);
		data = x;
		volatile unsigned char test = readData();
		if(data != test) {
			SetLEDstate(GREEN, 0);
			SetLEDstate(RED, 1);
		}
	}
	SetLEDstate(RED, 0);
	SetLEDstate(GREEN, 0);
	chipDisable();
}
