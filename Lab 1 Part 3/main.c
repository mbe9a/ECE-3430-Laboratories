#include <msp430.h> 

/*
 * lab 1 part 3: modulating the width of a pulse
 */
#define SET_RESET_MODE 	OUTMOD_7
#define PWM_PERIOD		1000-1
#define PWM_DUTYCYCLE	500
#define PWM_OUTPUT_PORT	BIT2

void initialize_clock(void);
void initialize_ports(void);

void main(void)
{
	initialize_clock();
	initialize_ports();

	while(1){

	};
	
}

void initialize_clock(){
	WDTCTL 	= WDTPW + WDTHOLD;  // Stop WDT
 	TACTL 	= TASSEL_2 + MC_1;   	// SMCLK, up mode
 	CCTL1 	= SET_RESET_MODE;          	// CCR1 reset/set
 	CCR0 	= PWM_PERIOD;             	// PWM Period
 	CCR1 	= PWM_DUTYCYCLE;               	// CCR1 PWM duty cycle	 <-- EDIT THIS LINE
}


void initialize_ports(){
	P1DIR |= PWM_OUTPUT_PORT;             // P1.2 to output
 	P1SEL |= PWM_OUTPUT_PORT;             // P1.2 to TA0.1
}
