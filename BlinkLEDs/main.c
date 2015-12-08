#include <msp430.h>
#include "LED.h"

// Function prototypes
void ConfigureClockModule(void);

void main(void)
{
	unsigned int number_of_blinks;
	
	// Stop the watchdog timer, and configure the clock module.
	WDTCTL = WDTPW + WDTHOLD;
	ConfigureClockModule();
	    
    // Initialize port pins associated with the LEDs, and then turn off LEDs.
    InitializeLEDPortPins();
    SetLEDState(LED1,OFF);
    SetLEDState(LED2,OFF);

	// Infinite loop
	number_of_blinks = 0;
  	while (1)
  	{
  		// Turn on the LEDs
  		SetLEDState(LED1,ON);
  		SetLEDState(LED2,ON);
  		
  		// Wait for approximately 1/4 second
  		//_delay_cycles(250000);
  		unsigned int i;
  		for(i = 0; i < 41800; i++){
  			//delay
  		}
  		
  		// Turn off LEDs
  		SetLEDState(LED1,OFF);
  		SetLEDState(LED2,OFF);
  		
  		// Wait for approximately 1/4 second
  		//_delay_cycles(250000);
  		unsigned int j;
  		for(j = 0; j < 41800; j++){
  		  	//delay
  		}
  		
  		number_of_blinks++;
  	}
}

void ConfigureClockModule(void)
{
    // Configure Digitally Controlled Oscillator (DCO) for 16 MHz using factory
    // calibrations.
	DCOCTL  = CALDCO_1MHZ;
	BCSCTL1 = CALBC1_1MHZ;
}
