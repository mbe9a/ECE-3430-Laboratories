#include <msp430.h>
#include "SPI.h"

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;							// Stop watchdog timer

    initializeSPI();									// set up all pins

    volatile unsigned char received0 = 0x00;			//initialize to 0
    // Configure Digitally Controlled Oscillator (DCO) using factory calibrations
    DCOCTL  = CALDCO_16MHZ;
    BCSCTL1 = CALBC1_16MHZ;
    /*
     * test cases
     */

    //volatile unsigned char received1 = 0x00;

    //writeSR(0x0c);
    //writeSR(0x00);
    //received1 = readSR();
    //received0 = readSR();

    chipErase();

	chipEnable();
	sendData(WE);
	chipDisable();

    AAI();

    readAll();

    while(1){

    }

	return 0;
}
