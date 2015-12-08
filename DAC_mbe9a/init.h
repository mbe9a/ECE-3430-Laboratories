/*
 * init.h
 *
 *  Created on: Nov 10, 2015
 *      Author: Michael
 */
#include <msp430.h>
#include "DAC.h"

#ifndef INIT_H_
#define INIT_H_

void InitSPI();
void InitPorts();
void InitTimer();
void InitHardware();

#endif /* INIT_H_ */
