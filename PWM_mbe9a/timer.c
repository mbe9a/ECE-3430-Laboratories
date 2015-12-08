#include <msp430.h>
#include "timer.h"
#include "led.h"

/*
 * timer.c
 *
 *  Created on: Sep 27, 2015
 *      Author: Michael Eller mbe9a
 */

int dir = 1;													// variable that determines whether the light
																// increases or decreases in brightness
int inc = 100;													// amount the limit of timer A1 increases or decreases
int count = 0;													// determines the total limit of timer A1
int count_inc = 1;												// used to turn off the incrementing for the button pause

void ConfigureTimer() {
	TACCR0 = PERIOD + 1;										// Upper limit of count for TAR
	TACCTL0 = CCIE;												// Enable interupts on Compare 0
																// ID_3 means divide by 8, MC_1 means count up by 1
																// TACLR clears the clock
	TACCR1 = 0;
	TACCTL1 = CCIE;
	TACTL = TASSEL_2 | ID_3 | MC_1 | TACLR;						// Set up  the timer, TASSEL_2 is clock source 2
}

void ConfigureButton() {
	P1REN |= BIT3;												// Enables resistor
	P1OUT |= BIT3;												// Set resistor to pull-up
	P1DIR &= ~BIT3;												// Set the button as input
	P1IE |= BIT3;												// Enable button interrupt
}

#pragma vector = TIMER0_A0_VECTOR  								// Timer A interrupt service routine

__interrupt void Timer_A0_routine(void) {						// timer routine, called everytime the timer reaches DELAY
	if(dir){													// get brighter
		if(count == 25){										// limit of TimerA1
			dir = 0;											// change direction (get dimmer)
			count = 0;											// reset counter
			SetLEDstate(RED, 1);								// turn red LED on
			SetLEDstate(GREEN, 1);								// turn green LED on
		}
		else {
			TACCR1 += inc;										// increment the A1 limit
			SetLEDstate(RED, 1);								// keep red LED on
			SetLEDstate(GREEN, 0);								// turn green LED off
		}
	}
	else {														// get dimmer
		if(count == 25){										// logic is analogous to above (just dimming)
			dir = 1;
			count = 0;
			SetLEDstate(RED, 0);								// red LED off
			SetLEDstate(GREEN, 1);
		}
		else {
			TACCR1 -= inc;
			SetLEDstate(RED, 1);								// red LED on
			SetLEDstate(GREEN, 0);								// turn green LED off
		}
	}
	count += count_inc;											// increment counter (unless button has been pressed- see button routine)
}

#pragma vector = TIMER0_A1_VECTOR								// A1 routine

__interrupt void Timer_A1_routine(void) {
	TACCTL1 &= ~TAIFG;											// clear the interrupt flag
	SetLEDstate(RED, 0);										// turn red LED off
}

#pragma vector = PORT1_VECTOR									// Button interrupt service routine, add code here for POST LAB

__interrupt void Button_routine(void) {
	P1IFG &= ~BIT3;												// clear the interrupt flag because it is shared on all bits of P1
	if(inc > 0){												// alternate between inc = 100 and 0, count_inc = 1 and 0
		inc = 0;
		count_inc = 0;
	}
	else {
		inc = 100;
		count_inc = 1;
	}
}

