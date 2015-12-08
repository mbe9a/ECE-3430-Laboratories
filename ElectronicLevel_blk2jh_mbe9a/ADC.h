/*
 * Brendon Koch (blk2jh)
 * Michael Eller (mbe9a)
 */

#ifndef ADC_H_
#define ADC_H_

void InitializeADC();		// initialize ADC settings and add first 8 values to buffer
void sample();				// adds the next value to the buffer
void filter();				// calculates the cumulative sum and average of the samples buffer
int getX();					// returns the average X value
int getY();					// returns the average Y value
int getZ();					// returns the average Z value
int getSample(int i);		// returns a value from the CurrentSample buffer

#endif /* ADC_H_ */
