/*
 * Brendon Koch (blk2jh)
 * Michael Eller (mbe9a)
 * Mike Verdicchio (mpv3ms)
 * MPE
 * 12/07/15
 */

// **************************************************************************************
// Includes Section
// ************************************************************************************

#include <msp430.h>
#include "tx.h"
#include "init.h"

// **************************************************************************************

//This routine manages the actual transmitter and is called every 500uS by a periodic interrupt.
//Comment Well
void Xmit(TransmitterData* TData) {
    enum XmitClockPhase Phase;
//Each 500 uS half bit period constitutes a separate clock "phase" for transmitter purposes.
    if (TData->Transmit_Clock_Phase == FullBit){
        TData->Transmit_Clock_Phase = MidBit ;
        P1OUT |= BIT4;
    }
    else  {
    	TData->Transmit_Clock_Phase = FullBit ;
    	P1OUT &= ~BIT4;
    }
    Phase = TData->Transmit_Clock_Phase ;
//Now do state machine
    switch(TData->Transmitter_State){

    	//Send a one followed by a zero and go to NormalXmit
        case StartBit :
        	TData->Ones = 0;
            switch(Phase) {
                case FullBit :

                break ;
                case MidBit :
                	if (P1OUT & TX_DATA) {
                		TX_LOW;
                		TData->Transmitter_State = NormalXmit;
                	} else {
                		TX_HIGH;
                	}
                break ;
            }


        break ;

        //If the bit is one send a low to high transition
        //If the bit is zero send a high to low transition
        case NormalXmit :
            switch(Phase) {
                case FullBit :
                	if ((TData->Transmit_Data >> (TData->Bits_Remaining - 1)) & 1) {
                		TX_LOW;
                	} else {
                		TX_HIGH;
                	}
                break ;
                case MidBit :
                	if ((TData->Transmit_Data >> (TData->Bits_Remaining - 1)) & 1) {
                		TX_HIGH;
                		TData->Ones = 1 - TData->Ones;
                	} else {
                		TX_LOW;
                	}
                	Xmit1.Bits_Remaining--;
                	if (TData->Bits_Remaining <= 1) {
                		TData->Transmitter_State = Parity;
                	}
                break ;
            }

        break ;

        //If there are an even number of ones in the data send a high to low transition
        //If the number of ones is odd send a low to high transition
        case Parity:
        	switch(Phase) {
        	case FullBit:
        		if (TData->Ones == 1) {
        			TX_LOW;
        		} else {
        			TX_HIGH;
        		}
        		break;

        	case MidBit:
        		if (TData->Ones == 1) {
        			TX_HIGH;
        		} else {
        			TX_LOW;
        		}
        		TData->Bits_Remaining--;
        		TData->Transmitter_State = InterWord;
        		break;
        	}
        case InterWord :
        	//After all 32 bits have been transmitted wait and reset all transmit variables
            switch(Phase) {
                case FullBit :
                break ;
                case MidBit :
                	TData->InterwordTimeout++;
                if (TData->InterwordTimeout >= INTERWORD_DELAY) {
                	TX_LOW;
                	TData->Transmitter_State = StartBit;
                	TData->InterwordTimeout = 0;
                	TData->Bits_Remaining = BITS_IN_TRANSMISSION;
                	TData->Transmit_Data = TData->Transmit_Data_Buffer;
                }
                break;
            }

        break ;
        default :
            TData->Transmitter_State = StartBit ;
//Other intitialization here.....
        break ;

    }
}

//Functions called via an  interrupt
//This is called every 500uS by the timer A0 interrupt function
void txinthandler(void) {
//Do whatever needs to be done on a periodic basis for tx here:
 Xmit(&Xmit1);
}
