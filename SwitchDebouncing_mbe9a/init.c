/*
 * init.c
 *
 *  Created on: Oct 18, 2015
 *      Author: Michael Eller mbe9a
 *
 *	This file contains most of the hardware and timer setup. The function InitButton was created
 *	to improve generalizability. You can create any number of buttons/switches by using this method
 *	and passing which bit/pin the swithc is connected to.
 */

#include "debounce.h"
#include "init.h"

SwitchDefine InitButton(char bit, int press, int release) {										// To further generalize the code
	SwitchDefine button;												// temp button
	button.ControlState = DbExpectHigh;									// Start at default state
	button.SwitchPort = (char*) &(P1IN);								// port of the switch
	button.SwitchPin = bit;												// which pin on the msp the button is connected to
	button.validHigh = press;											// how long debounce should check for a High
	button.validLow = release;											// how long debounce should check for a Low
	button.start = 0;													// each button can keep track of its own start time
																		// further generalizing the code for multiple buttons
	return button;														// button built
}

void InitializeHardware(void) {
	DCOCTL = CALDCO_1MHZ;  												// |Set clock speed to 1 MHz|
	BCSCTL1 = CALBC1_1MHZ;  											// |                        |
	InitPorts();														// used to set all port/pin directions
	InitTimerSystem();													// set up the timer A interrupt
}

void InitPorts() {
	P1REN |= BIT3;														// Enable resistor
	P1OUT |= BIT3;														// set to pull-up
	P1DIR &= ~BIT3;														// set button as input
	P1IE |= BIT3;														// enable interrupts on pin 1.3

	P1DIR |= BIT0;														// set red LED as output
	P1OUT &= ~BIT0;														// initialize to off
	P1DIR |= BIT6;														// set green LED as output
	P1OUT &= ~BIT6;														// initialize to off
}

void InitTimerSystem() {
	TACCR0 = 125;														// Upper limit of count for TAR (125 * 8 = 1000 -> every 1 ms
	TACCTL0 |= CCIE;													// Enable interupts on Compare 0
																		// ID_3 means divide by 8, MC_1 means count up by 1
																		// TACLR clears the clock
	TACTL = TASSEL_2 | ID_3 | MC_1 | TACLR;								// Set up  the timer, TASSEL_2 is clock source 2
}


