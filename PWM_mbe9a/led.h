#include <msp430.h>

/*
 * led.h
 *
 *  Created on: Sep 27, 2015
 *      Author: Michael Eller mbe9a
 */

#ifndef LED_H_
#define LED_H_

#define GREEN 		BIT6
#define RED 		BIT0

void InitializeLEDs(int red, int green);			// function prototypes
void SetLEDstate(int bit, int state);

#endif /* LED_H_ */
