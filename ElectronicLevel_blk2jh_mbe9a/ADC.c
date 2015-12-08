/*
 * Brendon Koch (blk2jh)
 * Michael Eller (mbe9a)
 */
#include <msp430.h>
#include "ADC.h"

unsigned int index;
int CumulativeSum[3];
int Average[3];
int CurrentSample[3];
int samples[8][3];

void InitializeADC() {
	P1DIR &= ~BIT0 + ~BIT1 + ~BIT2;

	ADC10CTL1 = INCH_2 + CONSEQ_3;            // A2/A1/A0, repeat multi channel
	ADC10CTL0 = ADC10SHT_2 + MSC + ADC10ON + ADC10IE;
	ADC10AE0 = (BIT0 + BIT1 + BIT2);          // P1.0,1, 2 Analog enable
	ADC10DTC1 = 0x03;                         // number of conversions

	sample();
	unsigned int k;
	for(k = 0; k < 3; k++){
		Average[k] = CurrentSample[k];
		CumulativeSum[k] = CurrentSample[k] << 3;
	}

    unsigned int i;
    unsigned int j;
    for(j = 0; j < 8; j++){
    	for(i = 0; i < 3; i++) {
    		samples[j][i] = CurrentSample[i];
    	}
    	sample();
    }
}

void sample(){
    ADC10CTL0 &= ~ENC;
    while (ADC10CTL1 & BUSY);               // Wait if ADC10 core is active
    ADC10SA = (int) &(CurrentSample);       // CurrentSample buffer start
    ADC10CTL0 |= ENC + ADC10SC;            	// Sampling and conversion ready
    __bis_SR_register(GIE);       			// LPM0, ADC10_ISR will force exit
}

void filter() {
	sample();															// get new accelerometer readings
	_delay_cycles(100);
	unsigned int i;
	for(i = 0; i < 3; i++){
		CumulativeSum[i] += (CurrentSample[i] - samples[index][i]);		// add new value and subract old value
		samples[index][i] = CurrentSample[i];							// replace old value with new value
		Average[i] = CumulativeSum[i] >> 3;								// calculate the average
	}
	index += 1;
	if(index > 7) {
		index = 0;
	}
}

int getX() {
	return Average[2];
}

int getY() {
	return Average[1];
}

int getZ() {
	return Average[0];
}

int getSample(int i) {
	return CurrentSample[i];
}

// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void){

}
