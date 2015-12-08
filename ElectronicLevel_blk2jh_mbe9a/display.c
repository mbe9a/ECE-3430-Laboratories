/*
 * Brendon Koch (blk2jh)
 * Michael Eller (mbe9a)
 */
#include "display.h"

void EnableDisplay() {
	P1DIR |= BIT4;									// BLANK pin to output
	P1OUT &= ~BIT4;									// BLANK pin to low
	P2DIR |= BIT0;									// LATCH pin to ouput

	SPI_Init();										// Intialize SPI
}

void UpdateDisplay(unsigned char byte) {
	SPISendByte(byte);								// send LED byte using SPI
	P2OUT |= BIT0;									// Toggle LATCH
	P2OUT &= ~BIT0;
}

/*
 * Sets the Onticks array based on the center LED. This function also generates the byte to send to the LED controller.
 */
void setledstates(LEDinfo *LEDs) {
    int index;
    int led = LEDs->centLED;
    unsigned char code = 0x00;
	index = led;
	LEDs->Onticks[index] = LEDs->max;
	code |= 1<<index;
	index = led + 1;
	if (index > 7) {index -= 8;}
	LEDs->Onticks[index] = LEDs->adjacent;
	code |= 1<<index;
	index = led + 2;
	if (index > 7) {index -= 8;}
	LEDs->Onticks[index] = LEDs->outer;
	code |= 1<<index;
	index = led + 3;
	if (index > 7) {index -= 8;}
	LEDs->Onticks[index] = 0;
	index = led + 4;
	if (index > 7) {index -= 8;}
	LEDs->Onticks[index] = 0;
	index = led + 5;
	if (index > 7) {index -= 8;}
	LEDs->Onticks[index] = 0;
	index = led + 6;
	if (index > 7) {index -= 8;}
	LEDs->Onticks[index] = LEDs->outer;
	code |= 1<<index;
	index = led + 7;
	if (index > 7) {index -= 8;}
	LEDs->Onticks[index] = LEDs->adjacent;
	code |= 1<<index;

    LEDs->ledbyte = code;
    LEDs->Period = 1000;
}

void lightLEDS(LEDinfo *LEDs) {
	switch(LEDs->state) {

	/*
	 * This state iterates through each value in onticks. If it is less than 0 than the corresponding LED is turned off
	 * Each value in Onticks and the period are decremented. When the period reaches 0, setledstates is called.
	 * The display is then updated with the new byte.
	 */
	case Level:
	{
		int i = 0;
		for (i = 0; i < 8; i++) {
			if (LEDs->Onticks[i] <= 0) {
				LEDs->ledbyte &= ~(1<<i);
			}
			LEDs->Onticks[i]-= 10;
			if (LEDs->Period <= 0){
				setledstates(LEDs);
			}
		}
		LEDs->Period -= 10;
		UpdateDisplay(LEDs->ledbyte);
		break;
	}

	/*
	 * Updates the display with the LED corresponding to the current direction being calibrated.
	 */
	case Calibrate:
		switch(LEDs->centLED) {
		case NW:
			UpdateDisplay(0xAA);
			break;
		case W:
			UpdateDisplay(0x02);
			break;
		case SW:
			UpdateDisplay(0x55);
			break;
		case S:
			UpdateDisplay(0x08);
			break;
		case SE:
			UpdateDisplay(0x10);
			break;
		case E:
			UpdateDisplay(0x20);
			break;
		case NE:
			UpdateDisplay(0x40);
			break;
		case N:
			UpdateDisplay(0x80);
			break;
		}
		break;

	// Turns on all LEDs
	case Flat:
		UpdateDisplay(0xFF);
	}
}

// sets the duty cycles for the LEDs based on the tilt value calculated with CORDIC.
void setBrightness(LEDinfo *LEDs, int tilt) {
	if (tilt < 4000) {
		LEDs->max = 200;
		LEDs->adjacent = 20;
		LEDs->outer = 10;
	} else if (tilt < 6000) {
		LEDs->max = 600;
		LEDs->adjacent = 60;
		LEDs->outer = 20;
	} else {
		LEDs->max = 1000;
		LEDs->adjacent = 200;
		LEDs->outer = 50;
	}
}
