/*
 * Brendon Koch (blk2jh)
 * Michael Eller (mbe9a)
 */
#include <msp430.h>
#include "usci_spi.h"

#ifndef DISPLAY_H_
#define DISPLAY_H_

#define MAX			1000
#define SIXTYPERC 	200
#define THIRTYPERC 	10
#define NW			0
#define W			1
#define SW			2
#define S			3
#define SE			4
#define E			5
#define NE			6
#define N			7
#define DELAY		1600000

typedef enum LEDStates {
	Start, Calibrate, Level, Flat
} LEDState;

typedef struct LEDStruct {
	int Period;									// Frequency of the LEDs
	int Onticks[8];								// time until LED is turned off for each LED
	LEDState state;								// state that the level is currently in
	unsigned char ledbyte;						// byte to send to the LED controller
	int centLED;								// the LED closest to the direction of tilt
	int max;									// brightness of the center LED
	int adjacent;								// brighness of the next LEDs over
	int outer;									// brighness of the LEDs 2 positions from the center LED
} LEDinfo;

void EnableDisplay();							// Enables the display and SPI
void UpdateDisplay(unsigned char byte);			// Updates the display with a new byte
void setledstates(LEDinfo *LEDs);				// sets the Onticks array and ledbyte based on the center LED
void lightLEDS(LEDinfo *LEDs);					// lights the LEDs according to the current state
void setBrightness(LEDinfo *LEDs, int tilt);	// set brightness of the LEDs as a function of the tilt


#endif /* DISPLAY_H_ */
