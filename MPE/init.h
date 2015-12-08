/*
 * Brendon Koch (blk2jh)
 * Michael Eller (mbe9a)
 * Mike Verdicchio (mpv3ms)
 * MPE
 * 12/07/15
 */

#include <msp430.h>

#ifndef INIT_H_
#define INIT_H_

#define TX_DATA 	BIT5
#define RX_DATA 	BIT0
#define TR			BIT6
#define RX_DATA2	BIT1
#define ERRORLED	BIT0

#define BITS_IN_TRANSMISSION 32; // Needed by both TX and RX

void Init_TX_Ports();
void Init_RX_Ports();

#endif /* INIT_H_ */
