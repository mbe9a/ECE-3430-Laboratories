/*
 * driver.h
 *
 *  Created on: Oct 22, 2015
 *      Author: Michael
 */

#ifndef DRIVER_H_
#define DRIVER_H_


#include <msp430.h>
#include "rotary.h"

#define BLANK		BIT4
#define XLAT		BIT0		// port 2

void InitDisplay();
void UpdateDisplay(EncoderDefinitions *myRotorEncoder);

#endif /* DRIVER_H_ */
