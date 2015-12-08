/*
 * lfsr.h
 *
 *  Created on: Oct 7, 2015
 *      Author: Michael Eller mbe9a
 *
 *      Taken from code provided
 */

#ifndef LFSR_H_
#define LFSR_H_


#define DIVISOR_POLYNOMIAL 0xB400
#define INITIAL_STATE 0x0001

unsigned int LFSR(unsigned int);
void AAI();


#endif /* LFSR_H_ */
