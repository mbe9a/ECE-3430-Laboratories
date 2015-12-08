/*
 * Brendon Koch (blk2jh)
 * Michael Eller (mbe9a)
 */
#include "debounce.h"

#ifndef INIT_H_
#define INIT_H_

SwitchDefine InitButton(char* port, char bit, int press, int release);				// function to generalize all buttons, pass the bit that is connected to switch
void InitializeHardware(void);														// Set up all hardware subsystems (LEDs, button etc...)
void InitTimerSystem(); 															// Sets up a periodic interrupt at a 1 mS rate using SMCLK as the clock source.
void InitPorts();

#endif /* INIT_H_ */
