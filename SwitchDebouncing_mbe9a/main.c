/*****************************************************************************
 Michael Eller mbe9a
 Lab 7 Switch Debugging

 This program is meant to 'debounce' the button onboard the msp430.
 Using software timers and a state machine, this program flashes the green
 LED every 500 ms and the debounced button toggles the red LED.
 *****************************************************************************/
#include <msp430.h>
#include "debounce.h"
#include "init.h"

void main(void) {
	WDTCTL = WDTPW | WDTHOLD;											// Stop watchdog timer

	InitializeVariables();												// Give initial values to all variables, mostly timing variables
	InitializeHardware();												// Initialize all hardware subsystems
	SwitchDefine gPushButton = InitButton(BIT3, MAKEDEBOUNCETIME, RELEASEBOUNCETIME);

	_BIS_SR(GIE);														// Enable all interrupts

	while (1) {
		ManageSoftwareTimers();											// Software timers, found in 'init.c'
		GetSwitch(&gPushButton);										// Get the value of the 'switch', in this case -> onboard button
		Debouncer(&gPushButton);										// Function that debounces button press
	}
}

