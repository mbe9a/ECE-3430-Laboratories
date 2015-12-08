/* lab 1 part 4 illustrates operation lengths
 *
 */
#include <msp430.h> 
#include <math.h>

void main(void)
{
 	WDTCTL = WDTPW + WDTHOLD;  			// Stop WDT
 	BCSCTL2 &= ~BIT3;					// SMCLK=DCOCLK
	TA0CTL = TASSEL_2 + MC_1 + TACLR;   // SMCLK, up mode, reset tar register
     TACCR0 = 0xFFFF;


    // change these to 'volatile int' when instructed
      int a = 0;
      int b = 0;

    // SET UP PIN FOR OUTPUT

 	P1DIR |= BIT2;
 	_delay_cycles(10000);


 	TA0R  &= 0x00;      // reset counter
 	a = a+b;            // Addition function (place breakpoint here when asked)

 	TA0R  &= 0x00;      // reset counter
 	a = a*9000;         // Multiplication function (place breakpoint here when asked)

 	TA0R  &= 0x00;      // reset counter
 	a = a/9000;         // Division function (place breakpoint here when asked)

 	TA0R  &= 0x00;      // reset counter
 	a = log(999);       // LOG function (place breakpoint here when asked)


 	while(1){
        P1OUT |= BIT2;      // Set output on P1.2 to TRUE  (high on the scope)

        // Put each of the functions in here and run individually.


        P1OUT &= ~BIT2;     // Set output on P1.2 to FALSE (low on the scope)
 		_delay_cycles(1000);
 	}

}
