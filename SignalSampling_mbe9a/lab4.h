/*
 * Michael Eller
 * mbe9a
 * ECE 3430
 * Lab 4 Signal Sampling
 */

#ifndef LAB4_H
#define LAB4_H

#define DELAY 260									// timer upper limit
													// counting using division by 8
													// and a clock speed of 1 MHz

#define NUM_SAMPLES 32								// number of samples to keep in data array

#define NUM_SHIFT 5 								// log base 2 of NUM_SAMPLES
													// used to maintain accuracy in averaging

void ConfigureAdcTempSensor(int sensor);			// function prototypes
void ConfigureTimer();

#endif /* LAB4_H */
