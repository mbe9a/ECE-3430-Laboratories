/*
 * debounce.h
 *
 *  Created on: Oct 13, 2015
 *      Author: Michael Eller mbe9a
 */

#ifndef DEBOUNCE_H_
#define DEBOUNCE_H_

#define MAKEDEBOUNCETIME	10
#define RELEASEBOUNCETIME	20

#include <msp430.h>

typedef enum {															// definition for state machine states
	DbExpectHigh, DbValidateHigh, DbExpectLow, DbValidateLow
} DbState;

typedef enum {															// definition for possible switch values
	High, Low
} SwitchStatus;

typedef struct {														// struct to hold all button/switch information
	DbState ControlState;												// current state in state machine
	SwitchStatus CurrentSwitchReading; 									// {instantaneous, from GetSwitch}
	SwitchStatus CurrentValidState;    									// {debounced value of the switch from state machine}
	char* SwitchPort;													// port the switch is on, for reading switch value
	char SwitchPin;														// switch pin for initialization
	int validHigh;														// time interval to determine a valid High
	int validLow;														// time interval to determine a valie Low
	unsigned int start;													// start time to measure elapsed time
} SwitchDefine;

void GetSwitch(SwitchDefine *Switch);									// This function returns the instantaneous value of the selected switch
SwitchStatus Debouncer(SwitchDefine *Switch);							// This function debounces a switch input
void ManageSoftwareTimers(void);										// This deals with software timers off of your 1 ms global counter.
void InitializeVariables();												// Initialize all timer variables

#endif /* DEBOUNCE_H_ */
