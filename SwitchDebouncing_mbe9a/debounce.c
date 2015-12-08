/*
 * debounce.c
 *
 *  Created on: Oct 18, 2015
 *      Author: Michael Eller mbe9a
 *
 *	This file contains all interrupt routines (button and timer A). The main debouncing
 *	logic and state machine are implemented here.
 */
#include <msp430.h>
#include "debounce.h"
#include "init.h"

unsigned int gButtonPresses;  											// Number of button presses from all switches
unsigned int g500mSTimer;												// used to toggle green LED ever 0.5 s
unsigned int g10STimer;													// used to reset the ports every 10 seconds
unsigned int g1mSTimer; 												// This variable is incremented in ManageSoftwareTimers
unsigned int g1mSTimeout;												// incremented by timer A routine, used to implement software timers

void InitializeVariables() {											// set all variables to 0 initially
	g1mSTimer = 0;
	g1mSTimeout = 0;
	gButtonPresses = 0;
	g500mSTimer = 0;
	g10STimer = 0;
}

#pragma vector = TIMER0_A0_VECTOR  										// Timer A interrupt service routine

__interrupt void Timer_A0_routine(void) {
	g1mSTimeout++;														// increment every 1 ms
}

#pragma vector = PORT1_VECTOR											// Button interrupt service routine, add code here for POST LAB

__interrupt void Button_routine(void) {
	P1IFG &= ~BIT3;														// clear the interrupt flag because it is shared on all bits of P1
}

void GetSwitch(SwitchDefine *Switch) {									// get current value of switch from defined switch pin and port
	if(*(Switch->SwitchPort) & Switch->SwitchPin) {						// if switch pin is 1 (Active Low)
		Switch->CurrentSwitchReading = Low;								// set reading to Low
	}
	else {
		Switch->CurrentSwitchReading = High;							// otherwise set to High
	}
}

void ManageSoftwareTimers(void) {										// main method to create software timers
	while (g1mSTimeout != 0) {											// if time has passed
		g1mSTimer++;													// increment global counter
		g1mSTimeout--;													// decrement timeout
	}
	while (g1mSTimer >= 500) {											// if 500 ms has passed
		g500mSTimer++;													// increment 500 ms timer
		g1mSTimer -= 500;												// decrement glabal counter by 500 ms
		P1OUT ^= BIT6;													// toggle green LED
	}
	while (g500mSTimer >= 20) {											// if 10 seconds have passed
		g10STimer++;													// increment 10 s counter
		g500mSTimer -= 20;												// decrement 500 ms counter by 10 seconds
		InitPorts();													// reinitialize ports
	}
}

SwitchStatus Debouncer(SwitchDefine *Switch) {							// main debouncing method/ state machine
	switch (Switch->ControlState) {
		case DbExpectHigh:												// default unpressed state
			if(Switch->CurrentSwitchReading == High) {					// if the reading from the switch is logic High
				Switch->ControlState = DbValidateHigh;					// change state to validate the High
				Switch->start = g1mSTimer;								// start time for validation is recorded
			}
		break ;
		case DbValidateHigh:											// state to check if the button was pressed
			if(g1mSTimer - Switch->start < Switch->validHigh) {			// if if has not been High long enough
				if(Switch->CurrentSwitchReading == Low){				// check if the switch reading is Low
					Switch->ControlState = DbExpectHigh;				// if it is Low, go back to starting state
				}
				break;
			}
			P1OUT ^= BIT0;												// if it has been High long enough, toggle red LED
			Switch->CurrentValidState = High;							// valid state of button is now High
			Switch->ControlState = DbExpectLow;							// change to next state, which expects a Low
		break ;
		case DbExpectLow:
			if(Switch->CurrentSwitchReading == Low) {					// if current switch reading is Low
				Switch->ControlState = DbValidateLow;					// change to next state, validate Low
				Switch->start = g1mSTimer;								// start time for validation is recorded
			}
		break ;
		case DbValidateLow:
			if(g1mSTimer - Switch->start < Switch->validLow) {			// if it has not been Low long enough
				if(Switch->CurrentSwitchReading == High) {				// check if the reading has changed to High
					Switch->ControlState = DbExpectLow;					// if so, go back to the previous state
				}
				break;
			}
			gButtonPresses++;											// increment global button presses counter
			Switch->CurrentValidState = Low;							// button state is logic Low
			Switch->ControlState = DbExpectHigh;						// return to default state
		break ;
		default: Switch->ControlState = DbExpectHigh ;					// if error, just go to default state
	}
	return Switch->CurrentValidState;									// debounced state
}
