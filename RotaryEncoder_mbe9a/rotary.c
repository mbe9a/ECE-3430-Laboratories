/*
 * rotary.c
 *
 *  Created on: Oct 20, 2015
 *      Author: Michael Eller mbe9a
 */
#include "rotary.h"
#include "driver.h"

void IncrementDuty() {
	TACCR1 += 4;
}

void DecrementDuty() {
	TACCR1 -= 4;
}

EncoderState stateMachine(EncoderDefinitions *myRotorEncoder, EncoderState myEncoderState) {
	switch (myEncoderState) {
	case Initialize:
		myRotorEncoder->count = 508;
		if(myRotorEncoder->SwitchA == High) {
			if(myRotorEncoder->SwitchB == High) {
				myEncoderState = AhighBhigh;
			}
			else {
				myEncoderState = AhighBlow;
			}
		}
		else {
			if(myRotorEncoder->SwitchB == High) {
				myEncoderState = AlowBhigh;
			}
			else {
				myEncoderState = AlowBlow;
			}
		}
		break;
	case AlowBlow:
		if(myRotorEncoder->SwitchA == High){
			myEncoderState = AhighBlow;
			myRotorEncoder->count++;
		}
		else if (myRotorEncoder->SwitchB == High) {
			myEncoderState = AlowBhigh;
			myRotorEncoder->count--;
		}
		break;
	case AhighBlow:
		if(myRotorEncoder->SwitchB == High) {
			myEncoderState = AhighBhigh;
			myRotorEncoder->count++;
		}
		else if (myRotorEncoder->SwitchA == Low) {
			myEncoderState = AlowBlow;
			myRotorEncoder->count--;
			DecrementDuty();
		}
		break;
	case AhighBhigh:
		if(myRotorEncoder->SwitchA == Low) {
			myEncoderState = AlowBhigh;
			myRotorEncoder->count++;
		}
		else if (myRotorEncoder->SwitchB == Low) {
			myEncoderState = AhighBlow;
			myRotorEncoder->count--;
		}
		break;
	case AlowBhigh:
		if(myRotorEncoder->SwitchB == Low) {
			myEncoderState = AlowBlow;
			myRotorEncoder->count++;
			IncrementDuty();
		}
		else if (myRotorEncoder->SwitchA == High){
			myEncoderState = AhighBhigh;
			myRotorEncoder->count--;
		}
		break;
	default:
		myEncoderState = Initialize;
	}
	return myEncoderState;
}

EncoderState GetEncoder(EncoderDefinitions *myRotorEncoder) {
	if(myRotorEncoder->SwitchA == High) {
		if(myRotorEncoder->SwitchB == High) {
			return AhighBhigh;
		}
		else {
			return AhighBlow;
		}
	}
	else {
		if(myRotorEncoder->SwitchB == High) {
			return AlowBhigh;
		}
		else {
			return AlowBlow;
		}
	}
}
