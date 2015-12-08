/*
 * driver.c
 *
 *  Created on: Oct 22, 2015
 *      Author: Michael Eller mbe9a
 */

#include "driver.h"
#include "rotary.h"
#include <msp430.h>
#include "usci_spi.h"

const char HEXARRAY[16] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

void InitDisplay() {
	P1DIR |= BLANK;
	P1OUT &= ~BLANK;

	P2DIR |= XLAT;
	P2OUT &= ~XLAT;
}

void UpdateDisplay(EncoderDefinitions *myRotorEncoder) {
	P1OUT |= BLANK;
	if(myRotorEncoder->count < 0 || (myRotorEncoder->count >> 2) > 255) {
		myRotorEncoder->count = 0;
		TACCR1 = 0;
	}
	SPISendByte(HEXARRAY[(myRotorEncoder->count >> 2) % 16]);
	SPISendByte(HEXARRAY[(myRotorEncoder->count >> 6)]);
	P2OUT &= ~XLAT;
	P2OUT |= XLAT;
	P1OUT &= ~BLANK;
}
