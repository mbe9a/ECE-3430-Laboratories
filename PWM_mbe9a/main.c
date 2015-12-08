#include <msp430.h>
#include "timer.h"
#include "led.h"

/*
 * main.c
 * Michael Eller mbe9a
 * ECE 3430 Lab 5
 * 09-28-2015
 */

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;									// Stop watchdog timer
	
	DCOCTL = CALDCO_1MHZ;  										// |Set clock speed to 1 MHz|
	BCSCTL1 = CALBC1_1MHZ;  									// |                        |

	P1DIR |= BIT4;												// set up the SMCLK as an output on pin 4
	P1SEL |= BIT4;

	InitializeLEDs(1,1);										// initialize red and green LEDs

	ConfigureTimer();											// defined in timer.c

	ConfigureButton();											// defined in timer.c

	_BIS_SR(GIE);												// Enable all interrupts

	while(1) {													// infinite loop

	}

	return 0;
}
