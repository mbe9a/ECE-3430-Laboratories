#include <msp430.h> 

int led = 0;									//1 for red, 0 for green, blinking starts on red

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;					// Stop watchdog timer
	DCOCTL = CALDCO_1MHZ;  						// |Set clock speed to 1 MHz|
	BCSCTL1 = CALBC1_1MHZ;  					// |                        |

	P1OUT &= ~BIT0;  							// Set Pin 1.0 (red LED) to 0
	P1DIR |= BIT0; 								// Make BIT0 (red LED) an output
	P1OUT &= ~BIT6;								// Set Pin 1.6 (green LED) to 0
	P1DIR |= BIT6;								// Make BIT6 (green LED) an output

	P1REN |= BIT3;								// Enables resistor
	P1OUT |= BIT3;								// Set resistor to pull-up
	P1DIR &= ~BIT3;								// Set the button as input
	P1IE |= BIT3;								// Enable button interrupt

	TACCR0 = 62500;								// Upper limit of count for TAR
	TACCTL0 = CCIE;								// Enable interupts on Compare 0
	TACTL = TASSEL_2 | ID_3 | MC_1 | TACLR; 	// Set up  the timer

	_BIS_SR(GIE);								// Enables all interrupts

	while(1) {

	}
	
	return 0;
}


#pragma vector = TIMER0_A0_VECTOR  				// Timer A interrupt service routine, add code here for IN LAB

__interrupt void Timer_A0_routine(void) {
	if(led){
		P1OUT ^= BIT6;							//Use XOR to alternate between OFF and ON
		P1OUT &= ~BIT0;							//turn red OFF
	}
	else {
		P1OUT ^= BIT0;							//turn red ON
		P1OUT &= ~BIT6;							//turn green OFF
	}
}

#pragma vector = PORT1_VECTOR					// Button interrupt service routine, add code here for POST LAB

__interrupt void Button_routine(void) {
	P1IFG &= ~BIT3;								// clear the interrupt flag because it is shared on all bits of P1
	if(led) {
		led = 0;								// switch LED
	}
	else {
		led = 1;								// switch LED
	}
}
