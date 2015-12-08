/*
 * Brendon Koch (blk2jh)
 * Michael Eller (mbe9a)
 * Mike Verdicchio (mpv3ms)
 * MPE
 * 12/07/15
 */

#include "init.h"

void Init_TX_Ports() {
	P1DIR |= TX_DATA + TR + BIT4;
	P1OUT &= ~(TX_DATA+ TR + BIT4);
}

void Init_RX_Ports() {
	P1OUT |= TR;
	P1DIR |= TR;
	P2DIR &= ~(RX_DATA + RX_DATA2);
	P2SEL |= RX_DATA + RX_DATA2;
	P2SEL2 &= ~(RX_DATA2 + RX_DATA);

	P1OUT &= ~ERRORLED;
	P1DIR |= ERRORLED;
}
