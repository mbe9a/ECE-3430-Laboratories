/*
 * init.c
 *
 *  Created on: Oct 20, 2015
 *      Author: Michael
 */

#include "debounce.h"
#include "init.h"
#include "driver.h"
#include "usci_spi.h"

SwitchDefine InitButton(char* port, char bit, int press, int release) {	// To further generalize the code
	SwitchDefine button;												// temp button
	button.ControlState = DbExpectHigh;									// Start at default state
	button.SwitchPort = port;											// port of the switch
	button.SwitchPin = bit;												// which pin on the msp the button is connected to
	button.validHigh = press;											// how long debounce should check for a High
	button.validLow = release;											// how long debounce should check for a Low
	button.start = 0;													// each button can keep track of its own start time
																		// further generalizing the code for multiple buttons
	button.CurrentValidState = Low;
	return button;														// button built
}

void InitPorts() {
	P1REN |= BIT3;														// Enable resistor
	P1OUT |= BIT3;														// set to pull-up
	P1DIR &= ~BIT3;														// set button as input
	P1IE |= BIT3;														// enable interrupts on pin 1.3

	P2DIR &= ~BIT4;
	P2DIR &= ~BIT3;

	P1DIR |= BIT6;
	P1OUT &= ~BIT6;
}

void InitTimerSystem() {
	TACCR0 = 1000;														// Upper limit of count for TAR (125 * 8 = 1000 -> every 1 ms
	TACCTL0 |= CCIE;													// Enable interupts on Compare 0
																		// ID_3 means divide by 8, MC_1 means count up by 1
																		// TACLR clears the clock
	TACCR1 = 492;
	TACCTL1 = CCIE;
	TACTL = TASSEL_2 | ID_0 | MC_1 | TACLR;								// Set up  the timer, TASSEL_2 is clock source 2
}

void InitializeHardware(void) {
	DCOCTL = CALDCO_1MHZ;  												// |Set clock speed to 1 MHz|
	BCSCTL1 = CALBC1_1MHZ;  											// |                        |
	InitPorts();														// used to set all port/pin directions
	InitTimerSystem();													// set up the timer A interrupt
	InitDisplay();
	SPI_Init();
}
