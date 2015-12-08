/*
 * init.h
 *
 *  Created on: Oct 18, 2015
 *      Author: Michael Eller mbe9a
 */

#ifndef INIT_H_
#define INIT_H_

SwitchDefine InitButton(char bit, int press, int release);				// function to generalize all buttons, pass the bit that is connected to switch
void InitializeHardware(void);					// Set up all hardware subsystems (LEDs, button etc...)
void InitTimerSystem(); 						// Sets up a periodic interrupt at a 1 mS rate using SMCLK as the clock source.
void InitPorts(); 								// Called from ManageSoftwareTimers every 10 seconds


#endif /* INIT_H_ */
