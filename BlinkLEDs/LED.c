#include <msp430g2553.h>
#include "LED.h"

void InitializeLEDPortPins (void)
{
	// Set the LEDs as outputs
	LED1_DDR |= LED1_BIT;
	LED2_DDR |= LED2_BIT;
}
void SetLEDState(char choice, char state)
{
	switch (choice) {
		case LED1:
			if (state)
				LED1_PORT |= LED1_BIT; // Turn on LED
			else
				LED1_PORT &= ~LED1_BIT; // Turn off LED
			break;
		case LED2:
			if (state)
				LED2_PORT |= LED2_BIT; // Turn on LED
			else
				LED2_PORT &= ~LED2_BIT; // Turn off LED
			break;
		default:
			break;
	}
	return;
}
