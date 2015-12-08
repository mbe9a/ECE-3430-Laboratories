/*
 * Brendon Koch (blk2jh)
 * Michael Eller (mbe9a)
 */
#include <msp430.h>
#include "debounce.h"

void InitializeVariables() {											// set all variables to 0 initially
	g1mSTimer = 0;
	g1mSTimeout = 0;
}

SwitchStatus GetSwitch(SwitchDefine *Switch) {							// get current value of switch from defined switch pin and port
	if(*(Switch->SwitchPort) & Switch->SwitchPin) {						// if switch pin is 1 (Active Low)
		Switch->CurrentSwitchReading = Low;								// set reading to Low
		return Low;
	}
	else {
		Switch->CurrentSwitchReading = High;							// otherwise set to High
		return High;
	}
}

void ManageSoftwareTimers(void) {										// main method to create software timers
	while (g1mSTimeout >= 10) {											// if time has passed
		g1mSTimer++;													// increment global counter
		g1mSTimeout-= 10;													// decrement timeout
	}
}

SwitchStatus Debouncer(SwitchDefine *Switch, ElectronicLevel *myLevel) {							// main debouncing method/ state machine
	GetSwitch(Switch);
	switch (Switch->ControlState) {
		case DbExpectHigh:												// default unpressed state
			if(Switch->CurrentSwitchReading == High) {					// if the reading from the switch is logic High
				Switch->ControlState = DbValidateHigh;					// change state to validate the High
				Switch->start = g1mSTimer;								// start time for validation is recorded
			}
			else {
				Switch->CurrentValidState = Low;
			}
		break ;
		case DbValidateHigh:											// state to check if the button was pressed
			if(g1mSTimer - Switch->start >= Switch->validHigh) {
				if(Switch->CurrentSwitchReading == High){
					Switch->CurrentValidState = High;
					Switch->ControlState = DbExpectLow;
					if (myLevel->current_state == Calibrate){
						Calibration(myLevel);
					}
				}
				else {
					Switch->ControlState = DbExpectHigh;
					Switch->CurrentValidState = Low;
				}
			}
		break ;
		case DbExpectLow:
			if(Switch->CurrentSwitchReading == Low) {					// if current switch reading is Low
				Switch->ControlState = DbValidateLow;					// change to next state, validate Low
				Switch->start = g1mSTimer;								// start time for validation is recorded
			}
		break ;
		case DbValidateLow:
			if(g1mSTimer - Switch->start >= Switch->validLow) {
				if (Switch->CurrentSwitchReading == Low) {
					Switch->CurrentValidState = Low;
					Switch->ControlState = DbExpectHigh;
				}
				else {
					Switch->ControlState = DbExpectLow;
					Switch->CurrentValidState = High;
				}
			}
		break ;
		default: Switch->ControlState = DbExpectHigh ;					// if error, just go to default state
	}
	return Switch->CurrentValidState;									// debounced state
}


