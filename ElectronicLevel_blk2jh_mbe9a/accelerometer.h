/*
 * Brendon Koch (blk2jh)
 * Michael Eller (mbe9a)
 */
#include "display.h"
#include "ADC.h"
#include "CORDIC.h"

#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

#define TOLERANCE 1440											// the tolerance for being level. MAX = 36000, 4 degrees = 1440

typedef struct {
	LEDState current_state;										// the current state of the accelerometer state machine
	int cal_array[4][3];										// contains the max, min, and mid values for each axis
	int cal_step;												// the current calibration state (0-5)
	int norm_array[3][2];										// contains the max and min values for each axis after shifting to be around 0
} ElectronicLevel;

void Calibration(ElectronicLevel *myLevel);						// records the calibration data for each axis (called in debounce)
void stateMachine(ElectronicLevel *myLevel, LEDinfo *LEDs);		// state machine for the level

#endif /* ACCELEROMETER_H_ */
