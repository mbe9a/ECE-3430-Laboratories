/*
 * DAC.c
 *
 *  Created on: Nov 13, 2015
 *      Author: Michael Eller mbe9a
 */
#include "DAC.h"

void SendByte(unsigned char byte){						// MSP Hardware SPI
	UCA0TXBUF = byte;									// Set the transmit buffer
	while (UCBUSY & UCA0STAT);							// Check to see if it's busy
}

unsigned char getMSB(int data) {						// Function to get first 8 bits
	return (data >> 8) & 0x00FF;
}

unsigned char getLSB(int data) {						// Function to get the last 8 bits
	return data & 0x00FF;
}
