/*
 * masking.h
 *
 *  Created on: Sep 3, 2015
 *  Author: Michael
 */

#ifndef MASKING_H
#define MASKING_H


//all were found in either user's guide or data sheet
#define wdt_ctl			(int*)0x0120			//watchdog timer address (pointer)
#define wdt_pw			0x5A00					//password
#define wdt_hold		0x0080					//hold

#define LED_PORT		(int*)0x021 			// Output port for LED 1
#define LED_DDR			(int*)0x022				// Data Direction Register (DDR) for LED 1
#define LED_BIT			0x0001					// Port pin bit location for LED 1

void ConfigureClockModule(void) {
    // Configure Digitally Controlled Oscillator (DCO) for 16 MHz using factory
    // calibrations.
	DCOCTL  = CALDCO_1MHZ;
	BCSCTL1 = CALBC1_1MHZ;
}

#endif /* MASKING_H_ */
