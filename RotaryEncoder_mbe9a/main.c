#include <msp430.h> 
#include "rotary.h"
#include "debounce.h"
#include "init.h"
#include "usci_spi.h"
#include "driver.h"

EncoderDefinitions rotor;

#pragma vector = PORT1_VECTOR											// Button interrupt service routine, add code here for POST LAB
__interrupt void Button_routine(void) {
	P1IFG &= ~BIT3;														// clear the interrupt flag because it is shared on all bits of P1
	rotor.count = 508;
	TACCR1 = 492;
}

#pragma vector = TIMER0_A1_VECTOR										// A1 routine

__interrupt void Timer_A1_routine(void) {
	TACCTL1 &= ~TAIFG;													// clear the interrupt flag
	if(TACCR1 >= 1000){
		return;
	}
	else {
		P1OUT &= ~BIT6;
	}
}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;															// Stop watchdog timer
	
    InitializeVariables();
    InitializeHardware();

	SwitchDefine Reset = InitButton((char*) &(P1IN), BIT3, 10, 20);
	SwitchDefine SwitchA = InitButton((char*) &(P2IN), BIT4, 5, 5);
	SwitchDefine SwitchB = InitButton((char*) &(P2IN), BIT3, 5, 5);

	EncoderState CurrentState = Initialize;

	_BIS_SR(GIE);																		// Enable all interrupts

	while(1) {
		ManageSoftwareTimers();															// Software timers, found in 'init.c'
		Debouncer(&Reset);
		Debouncer(&SwitchA);
		Debouncer(&SwitchB);
		rotor.Reset = Reset.CurrentValidState;											// Get the value of the 'switch', in this case -> onboard button															// Function that debounces button press
		rotor.SwitchA = SwitchA.CurrentValidState;
		rotor.SwitchB = SwitchB.CurrentValidState;
		CurrentState = stateMachine(&rotor, CurrentState);
		UpdateDisplay(&rotor);
	}
	return 0;
}
