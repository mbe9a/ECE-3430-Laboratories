/*
 * Michael Eller
 * mbe9a
 * ECE 3430
 * Lab 4 Signal Sampling
 */

#include <msp430.h>
#include "lab4.h"

int input = 0;													// var used to store current input from the source
int samples[NUM_SAMPLES];										// array used to store all current data points
int index = 0;													// value at index is replaced with new input
int average = 430;												// var to store moving average, initialized to 430- expected but arbitrary here
int total = 0;													// var to store current total in NUM_SAPMPLES,
																// initialized truly in ConfigureAdcTempSensor()

void filter() {													// filter routine, main sampling function
	P1OUT |= BIT6;												// turn green LED ON
	if(index == NUM_SAMPLES){
		index = 0;												// reset index to the beginning of the array
	}
	else {
		index++;												// increment index
	}
	total -= samples[index];									// remove value @ index from total
	samples[index] = input;										// set the value @ index to new input (reading from source)
	total += samples[index];									// add that value to the running total
	average = total >> NUM_SHIFT;								// divide by the number ot elements in samples[]
	P1OUT &= ~BIT6;												// turn green LED OFF
}

#pragma vector = TIMER0_A0_VECTOR  								// Timer A interrupt service routine

__interrupt void Timer_A0_routine(void) {						// timer routine, called everytime the timer reaches DELAY
	P1OUT |= BIT0;												// turn red LED ON
	input = ADC10MEM;											// set input to current sample reading
	filter();													// call filter routine
	P1OUT &= ~BIT0;												// turn red LED OFF
}

void ConfigureAdcTempSensor(int sensor)							// function used to set up analog input
																// either temp sensor (sensor = 0) or
																// external input on pin 7 (sensor = 1)
{
  unsigned char i;
  if(sensor){													// analog input
  	  ADC10CTL1 = INCH_7 + ADC10DIV_3;							// changed 10 to 7 for analog input, ADC10DIV_3 is clock divider select 3
  	  ADC10CTL0 = SREF_0 + ADC10SHT_3 + REFON + ADC10ON;		// SREF_0 -> set reference to VCC, turn reference on and enable ADC10
  	  ADC10AE0 |= BIT7;											// enable analog input on pin 7
  }
  else {														// temp sensor
  	  ADC10CTL1 = INCH_10 + ADC10DIV_3;							// INCH_10 selects temp sensor
  	  ADC10CTL0 = SREF_1 + ADC10SHT_3 + REFON + ADC10ON;		// turn ADC10 on like above
  }

  __delay_cycles(1000);											// allow time for setup
  ADC10CTL0 |= ENC + ADC10SC;									// read from ADC10

  for (i = 0; i < NUM_SAMPLES; i++){
	  samples[i] = ADC10MEM;									// initialize samples array
	  total += ADC10MEM;										// get initial total
  }
}

void ConfigureTimer() {
	TACCR0 = DELAY;												// Upper limit of count for TAR
	TACCTL0 = CCIE;												// Enable interupts on Compare 0
	TACTL = TASSEL_2 | ID_3 | MC_1 | TACLR; 					// Set up  the timer, TASSEL_2 is clock source 2
																// ID_3 means divide by 8, MC_1 means count up by 1
																// TACLR clears the clock
}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;									// Stop watchdog timer

	DCOCTL = CALDCO_1MHZ;  										// |Set clock speed to 1 MHz|
	BCSCTL1 = CALBC1_1MHZ;  									// |                        |

	P1OUT &= ~BIT0;  											// Set Pin 1.0 (red LED) to 0
	P1DIR |= BIT0; 												// Make BIT0 (red LED) an output
	P1OUT &= ~BIT6;												// Set Pin 1.6 (green LED) to 0
	P1DIR |= BIT6;												// Make BIT6 (green LED) an output

	//ConfigureAdcTempSensor(1);								// input: 1 for pin 7, 0 for temp sensor
	ConfigureTimer();
	_BIS_SR(GIE);												// Enables all interrupts
	unsigned int count = 0;
	while(1) {
		count = 0;
		P1DIR |= BIT7;
		P1OUT |= BIT7;
		_delay_cycles(100);
		P1OUT &= ~BIT7;
		P1DIR &= ~BIT7;
		_delay_cycles(800);
		while(P1IN & BIT7){
			count += 1;
		}

		//ConfigureAdcTempSensor(1);								// configure temp sensor and fill samples with initial values
		//P1DIR &= ~BIT7;
		//ADC10CTL0 |= ENC + ADC10SC;								//take sample
	}

	return 0;
}
