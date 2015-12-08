/* part 2 of lab 1 to illustrate debugging */

#include <msp430.h> 

void initialize_clock();
void initialize_ports();

void main(void)
{
 	initialize_clock();
 	initialize_ports();

 		while(1){
 		_delay_cycles(1);
 	}

}

void initialize_clock(){
	WDTCTL 	= WDTPW + WDTHOLD;  		//Stop WDT
	BCSCTL2 &= ~BIT3;					//SMCLK=DCOCLK
	TA0CTL 	= TASSEL_2 + MC_1 + TACLR;  //SMCLK, up mode, reset tar register
	TA0CCR0 = 100; 						//An arbitrarily large value to facilitate counting.
}


void initialize_ports(){

}
