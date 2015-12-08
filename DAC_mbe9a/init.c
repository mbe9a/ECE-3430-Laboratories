/*
 * init.c
 *
 *  Created on: Nov 10, 2015
 *      Author: Michael
 */
#include "init.h"

void InitSPI(){
	UCA0CTL1 |= UCSWRST;												// Software Reset
	UCA0CTL0 |= UCMSB | UCSYNC | UCMST | UCMODE_0 | UCCKPH;				// MSB first, UART mode
																		// Sync mode: Master Select
																		// USCI Mode 0, Clock phase
	UCA0CTL0 &= ~(UC7BIT | UCCKPL);										// 8 bits, clock polarity
	UCA0CTL1 |= UCSSEL_2;												// Clock source 2
	UCA0BR0 |= 0x01;													// Baud Rate 0
	UCA0BR1 |= 0x00;													// baud Rate 1
	UCA0CTL1 &= ~UCSWRST;												// Software Reset disabled
}

void InitPorts(){
	P1DIR |= SDI | CLK | CS | CLR;										// Set chip input, clock, chip select, clear
	P1DIR &= ~SDO;														// Set chip output as input
	P1OUT |= SDI | CS;													// Set chip input and chip select as high
	P1OUT &= ~CLK;														// Set clock low
	P1OUT |= CLR;														// Set clear high

	P1SEL |= SDO + SDI + CLK;											// Port 1 select for outputs
	P1SEL2 |= SDO + SDI + CLK;											// Port 1 selection 2

	P2DIR |= DEBUG_PIN;													// For testing the timer interrupt
	P2OUT &= ~DEBUG_PIN;
}

void InitTimer() {
	TACCR0 = 714;														// Timer limit
	TACCTL0 |= CCIE;													// Enable interrupt
	TACTL = TASSEL_2 | ID_0 | MC_1 | TACLR;								// count up by one, clock source: SMCLK
}

void InitHardware(){
	DCOCTL = CALDCO_16MHZ;  											// |Set clock speed to 16 MHz|
	BCSCTL1 = CALBC1_16MHZ;  											// |                         |
	InitPorts();
	InitSPI();
	InitTimer();
}
