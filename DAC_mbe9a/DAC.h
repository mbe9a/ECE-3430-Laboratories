/*
 * DAC.h
 *
 *  Created on: Nov 13, 2015
 *      Author: Michael Eller mbe9a
 */
#include <msp430.h>

#ifndef DAC_H_
#define DAC_H_

#define SDO			BIT1
#define CLR			BIT5
#define CS			BIT3
#define SDI			BIT2
#define CLK			BIT4
#define DEBUG_PIN	BIT4
#define DACA		0x30
#define DACB		0x31
#define DACC		0x32
#define DACD		0x33
#define CS_LOW		P1OUT &= ~CS
#define CS_HIGH		P1OUT |= CS

void SendByte(unsigned char byte);
unsigned char getMSB(int data);
unsigned char getLSB(int data);

#endif /* DAC_H_ */
