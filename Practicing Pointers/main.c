#include <msp430.h>
#include "masking.h"

/*
 * main.c
 */

int main(void) {
	*wdt_ctl = wdt_pw | wdt_hold;		//stop watchdog timer

	// Configure Digitally Controlled Oscillator (DCO) for 16 MHz using factory
	// calibrations.
	ConfigureClockModule();

	*LED_DDR |= LED_BIT;				//set LED as output
	*LED_PORT &= ~LED_BIT;				//turn LED off

	while(1){

		*LED_PORT |= LED_BIT;			//turn LED on

		_delay_cycles(500000);			//wait 1/2 second

		*LED_PORT &= ~LED_BIT;			//turn LED off

		_delay_cycles(500000);			//wait 1/2 second

	}

	return 0;
}
