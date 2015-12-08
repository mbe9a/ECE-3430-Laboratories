/*
 * rotary.h
 *
 *  Created on: Oct 20, 2015
 *      Author: Michael
 */

#ifndef ROTARY_H_
#define ROTARY_H_

#include "debounce.h"

typedef enum {
	AlowBlow, AhighBlow, AlowBhigh, AhighBhigh, Initialize
} EncoderState;

typedef struct {
	SwitchStatus SwitchA;
	SwitchStatus SwitchB;
	SwitchStatus Reset;
	int count;
} EncoderDefinitions;

EncoderState stateMachine(EncoderDefinitions *myRotorEncoder, EncoderState myEncoderState);
EncoderState GetEncoder(EncoderDefinitions *myRotorEncoder);
void IncrementDuty();
void DecrementDuty();

#endif /* ROTARY_H_ */
