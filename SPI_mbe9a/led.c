#include "led.h"

/*
 * led.c
 *
 *  Created on: Sep 27, 2015
 *      Author: Michael Eller mbe9a
 */

void InitializeLEDs(int red, int green){							// initialize red and green LEDs
	if (red) {
		P1OUT &= ~BIT0;  											// Set Pin 1.0 (red LED) to 0
		P1DIR |= BIT0; 												// Make BIT0 (red LED) an output
	}
	if (green) {
		P1OUT &= ~BIT6;												// Set Pin 1.6 (green LED) to 0
		P1DIR |= BIT6;												// Make BIT6 (green LED) an output
	}
}

void SetLEDstate(int bit, int state) {								// turn red/green LED on/off
	if(state) {
		P1OUT |= bit;												// turn on
	}
	else {
		P1OUT &= ~bit;												// turn off
	}
}


