/*
 * Michael Eller
 * mbe9a
 * ECE 3430
 * Lab 3 CORDIC
 */

#include <msp430.h> 
#include "cordic.h"

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    //test cases
    calculations c1 = {50, 60, 0};
    calculations c2 = {1, 0, 67.67};

    //run CORDIC algorithm
    Cordic(&c1, 1);
    Cordic(&c2, 0);

	return 0;
}
