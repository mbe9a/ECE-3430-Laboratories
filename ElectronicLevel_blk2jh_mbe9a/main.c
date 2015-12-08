/*
 * Brendon Koch (blk2jh)
 * Michael Eller (mbe9a)
 */
#include <msp430.h>
#include "ADC.h"
#include "CORDIC.h"
#include "init.h"
#include "debounce.h"
#include "accelerometer.h"
#include "usci_spi.h"
#include "display.h"

LEDinfo LEDs;

void main(void) {
	WDTCTL = WDTPW + WDTHOLD;												// turn off watchdog timer
	InitializeADC();														// intialize ADC and puts first 8 values into buffer
	InitializeHardware();													// initialize the clock speed and timer settings
	EnableDisplay();														// enable the LED display

	SwitchDefine push_button = InitButton((char*) &(P1IN), BIT3, 10, 20);	// set the button to be PIN1.3 with 10 ms debounce for press and 20 ms debounce for release
	ElectronicLevel myLevel;
	myLevel.current_state = Start;											// initialize level state to Start
	myLevel.cal_step = 0;													// initialize calibration state

	LEDs.state = Start;														// initialize LED state to Start (shows light sequence)

	while(1) {
		ManageSoftwareTimers();												// update the 1 ms timer
		filter();															// calculates average and cumulative sum for each axis
		Debouncer(&push_button, &myLevel);									// debounce the button
		stateMachine(&myLevel, &LEDs);
	}
}

#pragma vector = TIMER0_A0_VECTOR  										// Timer A interrupt service routine

__interrupt void Timer_A0_routine(void) {
	g1mSTimeout++;														// increment every 0.1 ms
	lightLEDS(&LEDs);													// light the LEDs according to the current state
}
