/*
 * lfsr.c
 *
 *  Created on: Oct 7, 2015
 *      Author: Michael Eller mbe9a
 *
 *      Taken from code provided
 */
#include <msp430.h>
#include "lfsr.h"

unsigned int LFSR(unsigned int input_value) {
    unsigned int dividend;
    unsigned int lsb;

    // If the LSB of dividend = 1, add the divisor polynomial (using the XOR operation).
    dividend = input_value;
    lsb = dividend & 1;
    dividend >>= 1;
    dividend ^= (-lsb) & DIVISOR_POLYNOMIAL;

    return dividend;
}
