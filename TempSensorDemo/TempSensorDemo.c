#include <msp430.h>

/*
 * main.c
 * TEMPERATURE SENSOR ADC10 DEMO CODE
 *
 * MSP-EXP430G2-LaunchPad User Experience Application
 *
 * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/


/******************************************************************************
 *                  MSP-EXP430G2-LaunchPad User Experience Application
 *
 * 1. Device starts up in LPM3 + blinking LED to indicate device is alive
 *    + Upon first button press, device transitions to application mode
 * 2. Application Mode
 *    + Continuously sample ADC Temp Sensor channel, compare result against
 *      initial value
 *    + Set PWM based on measured ADC offset: Red LED for positive offset, Green
 *      LED for negative offset
 *    + Transmit temperature value via TimerA UART to PC
 *    + Button Press --> Calibrate using current temperature
 *                       Send character '°' via UART, notifying PC
 *
 * Changes:
 *
 * 1.2  + Updated register naming conventions to reflect latest standard by TI
 * 	         e.g.: CCR0 --> TACCR0, CCTL0 --> TACCTL0
 * 	    + Changed method to capture TAR value into TACCR0 by using capture a
 * 	      SW-triggered event. [Changing TACCR input from GND to VCC]
 * 1.1  + LED1 & LED2 labels changed so that Green LED(LED2) indicates sampled
 *        temperature colder than calibrated temperature and vice versa
 *        with Red LED (LED1).
 *      + Turn off peripheral function of TXD after transmitting byte to
 *        eliminate the extra glitch at the end of UART transmission
 * 1.0  Initial Release Version
 *
 * Texas Instruments, Inc.
 ******************************************************************************/

//#include  "msp430g2553.h"

#define     LED1                  BIT0
#define     LED2                  BIT6
#define     LED_DIR               P1DIR
#define     LED_OUT               P1OUT



#define     BUTTON                BIT3
#define     BUTTON_OUT            P1OUT
#define     BUTTON_DIR            P1DIR
#define     BUTTON_IN             P1IN
#define     BUTTON_IE             P1IE
#define     BUTTON_IES            P1IES
#define     BUTTON_IFG            P1IFG
#define     BUTTON_REN            P1REN

#define     TXD                   BIT1                      // TXD on P1.1
#define     RXD                   BIT2                      // RXD on P1.2

#define     APP_STANDBY_MODE      0
#define     APP_APPLICATION_MODE  1

#define     TIMER_PWM_MODE        0
#define     TIMER_UART_MODE       1
#define     TIMER_PWM_PERIOD      2000
#define     TIMER_PWM_OFFSET      20

#define     TEMP_SAME             0
#define     TEMP_HOT              1
#define     TEMP_COLD             2

#define     TEMP_THRESHOLD        5

//   Conditions for 9600/4=2400 Baud SW UART, SMCLK = 1MHz
#define     Bitime_5              0x05*4                      // ~ 0.5 bit length + small adjustment
#define     Bitime                13*4//0x0D

#define     UART_UPDATE_INTERVAL  1000


unsigned char BitCnt;


unsigned char applicationMode = APP_STANDBY_MODE;
unsigned char timerMode = TIMER_PWM_MODE;

unsigned char tempMode;
unsigned char calibrateUpdate = 0;
unsigned char tempPolarity = TEMP_SAME;
unsigned int TXByte;

/* Using an 8-value moving average filter on sampled ADC values */
long tempMeasured[8];
unsigned char tempMeasuredPosition=0;
long tempAverage;

long tempCalibrated, tempDifference;



void InitializeLeds(void);
void InitializeButton(void);
void PreApplicationMode(void);                     // Blinks LED, waits for button press
void ConfigureAdcTempSensor(void);
void ConfigureTimerPwm(void);
void InitializeClocks(void);


void main(void)
{

 unsigned char i;

  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  InitializeClocks();
  InitializeButton();
  InitializeLeds();
  PreApplicationMode();                     // Blinks LEDs, waits for button press

  /* Application Mode begins */
  applicationMode = APP_APPLICATION_MODE;
  /*****************************************
  set up the ADC10CTL registers!!
  ******************************************/
  ConfigureAdcTempSensor();
  ConfigureTimerPwm();


  __enable_interrupt();                     // Enable interrupts.

  /* Main Application Loop */
  while(1)
  {
	/************************************
	Sampling and conversion start
	ENC = enable conversion
	ADC10SC = start conversion
	we can start to take readings now!
	*************************************/
    ADC10CTL0 |= ENC + ADC10SC;

    __bis_SR_register(CPUOFF + GIE);        // LPM0 with interrupts enabled
	/******************************************
	Note: for our labs, we DON'T use Low Power Mode - keep the CPU on
	Just use the global interrupt enable that you learned in class:
	_BIS_SR(GIE);
	*******************************************/


    /******************************************************
	* Moving average filter out of 8 values to somewhat stabilize sampled ADC
	* optimize the following code for the postlab
	 ******************************************************/
    tempMeasured[tempMeasuredPosition++] = ADC10MEM; /* store the newest converted value from ADC10 in the circular buffer
													 (ACD10MEM = holds the newest converted value)
													 & advance the index */
    if (tempMeasuredPosition == 8)					/* if you are past the array indices after incrementing, position := zero*/
      tempMeasuredPosition = 0;						// if statement and assignment == circular buffer mechanism
    tempAverage = 0;
    for (i = 0; i < 8; i++)							// loop over all data in the buffer (should be current- newest value input above)
      tempAverage += tempMeasured[i];				// compute a total
    tempAverage >>= 3;								// Divide by buffer size to get average ADC10 converted value
													// NOTE: the bitshift is already an optimization= keep the buffer size a power of 2!

	// Auxiliary code - don't need this for the lab

    tempDifference = tempAverage - tempCalibrated;
    if (tempDifference < -TEMP_THRESHOLD)
    {
      tempDifference = -tempDifference;
      tempPolarity = TEMP_COLD;
      LED_OUT &= ~ LED1;
    }
    else
    if (tempDifference > TEMP_THRESHOLD)
    {
      tempPolarity = TEMP_HOT;
      LED_OUT &= ~ LED2;
    }
    else
    {
      tempPolarity = TEMP_SAME;
      TACCTL0 &= ~CCIE;
      TACCTL1 &= ~CCIE;
      LED_OUT &= ~(LED1 + LED2);
    }

    if (tempPolarity != TEMP_SAME)
    {
      tempDifference <<= 3;
      tempDifference += TIMER_PWM_OFFSET;
      TACCR1 = ( (tempDifference) < (TIMER_PWM_PERIOD-1) ? (tempDifference) : (TIMER_PWM_PERIOD-1) );
      TACCTL0 |= CCIE;
      TACCTL1 |= CCIE;
    }
  }
}

void PreApplicationMode(void)
{
  LED_DIR |= LED1 + LED2;
  LED_OUT |= LED1;                          // To enable the LED toggling effect
  LED_OUT &= ~LED2;

  BCSCTL1 |= DIVA_1;                        // ACLK/2
  BCSCTL3 |= LFXT1S_2;                      // ACLK = VLO

  TACCR0 = 1200;                             //
  TACTL = TASSEL_1 | MC_1;                  // TACLK = SMCLK, Up mode.
  TACCTL1 = CCIE + OUTMOD_3;                // TACCTL1 Capture Compare
  TACCR1 = 600;
  __bis_SR_register(LPM3_bits + GIE);          // LPM0 with interrupts enabled
}

/* This function does all of the set-up necessary for the ADC10 TEMP SENSOR CHANNEL
* pay attention to the ctl registers, especially which parameters are set!
* note that YOU WILL NEED TO MODIFY THIS CODE for the post-lab
* not all of the settings can be copied directly - know what each setting means!
*/
void ConfigureAdcTempSensor(void)
{
  unsigned char i;
  /**********************************************************
  * ADC10 Control register 1
  *	INCH_10 = choose the temperature sensor as the analog input
  * ADC10DIV_3 = ADC10CLK divider, divide by 4
  ***********************************************************/
  ADC10CTL1 = INCH_10 + ADC10DIV_3;
  /**********************************************************
  NOTE: for the post lab, if you are using any of the
  ADC10 analog inputs A0-A7, need to set ---ADC10AE register-----
  ***********************************************************/
  /**********************************************************
  * ADC10 Control Register 0
  * SREF_1 = set reference voltage = V+ = VREF+ and V- = Vss (ground)
  *		- make sure that your SREF range covers the voltage signals that you expect to see!!
  *		- i.e. if you expect a signal that ranges from 0-3.3V, make sure that V- is GND and V+ is greater than 3.3 V
  *		- reference voltage levels (if used) are set with REF2_5V
  * ADC10SHT_3 = sample and hold time = 64 clock cycles of the ADC10Clock
  *		- this is NOT the sampling rate
  *		- represents a trade-off between a more current value and a more stabilized value
  * REFON = turns on the reference generator
  *		- you must do this if you set V+ as VREF+ in SREFx
  * ADC10ON = turns on the ADC10 Core
  * ADC10IE = enables ADC10 to trigger interrupts
  ***********************************************************/
  ADC10CTL0 = SREF_1 + ADC10SHT_3 + REFON + ADC10ON + ADC10IE;
  __delay_cycles(1000);                     // Wait for ADC Ref to settle
 /***********************************************************
 * Sampling and conversion start
 * Now that we have configured the CTL registers, we can enable conversion
 * do not set ENC until you are done modifying the control registers
 * ENC = enable conversion
 * ADC10SC = start the conversion, ADC10SC is reset automatically
 *************************************************************/
  ADC10CTL0 |= ENC + ADC10SC;
  __bis_SR_register(CPUOFF + GIE);          // LPM0 with interrupts enabled
  /* don't use low power mode, just enable the interrupts */

  /* initialize the values in the circular buffer, as well as the average temperature
  * useful to load data (not 0) into the array so that you can get an accurate data faster
  * during subsequent average computations
  */
  for (i=0; i < 8; i++)
    tempMeasured[i] = ADC10MEM;
  tempAverage = ADC10MEM;
}

void ConfigureTimerPwm(void)
{
  timerMode = TIMER_PWM_MODE;

  TACCR0 = TIMER_PWM_PERIOD;                              //
  TACTL = TASSEL_2 | MC_1;                  // TACLK = SMCLK, Up mode.
  TACCTL0 = CCIE;
  TACCTL1 = CCIE + OUTMOD_3;                // TACCTL1 Capture Compare
  TACCR1 = 1;
}




// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
  if (timerMode == TIMER_UART_MODE)
  {
    TACCR0 += Bitime;                         // Add Offset to TACCR0
    if (TACCTL0 & CCIS0)                      // TX on CCI0B?
    {
      if ( BitCnt == 0)
      {
        P1SEL &= ~(TXD+RXD);
        TACCTL0 &= ~ CCIE ;                   // All bits TXed, disable interrupt
      }

      else
      {
        TACCTL0 |=  OUTMOD2;                  // TX Space
        if (TXByte & 0x01)
        TACCTL0 &= ~ OUTMOD2;                 // TX Mark
        TXByte = TXByte >> 1;
        BitCnt --;
      }
    }
  }
  else
  {
    if (tempPolarity == TEMP_HOT)
      LED_OUT |= LED1;
    if (tempPolarity == TEMP_COLD)
      LED_OUT |= LED2;
    TACCTL0 &= ~CCIFG;
  }
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void ta1_isr(void)
{
  TACCTL1 &= ~CCIFG;
  if (applicationMode == APP_APPLICATION_MODE)
    LED_OUT &= ~(LED1 + LED2);
  else
    LED_OUT ^= (LED1 + LED2);

}

void InitializeClocks(void)
{

  BCSCTL1 = CALBC1_1MHZ;                    // Set range
  DCOCTL = CALDCO_1MHZ;
  BCSCTL2 &= ~(DIVS_3);                     // SMCLK = DCO = 1MHz
}

void InitializeButton(void)                 // Configure Push Button
{
  BUTTON_DIR &= ~BUTTON;
  BUTTON_OUT |= BUTTON;
  BUTTON_REN |= BUTTON;
  BUTTON_IES |= BUTTON;
  BUTTON_IFG &= ~BUTTON;
  BUTTON_IE |= BUTTON;
}


void InitializeLeds(void)
{
  LED_DIR |= LED1 + LED2;
  LED_OUT &= ~(LED1 + LED2);
}

/* *************************************************************
 * Port Interrupt for Button Press
 * 1. During standby mode: to exit and enter application mode
 * 2. During application mode: to recalibrate temp sensor
 * *********************************************************** */
#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
  BUTTON_IFG = 0;
  BUTTON_IE &= ~BUTTON;            /* Debounce */
  WDTCTL = WDT_ADLY_250;
  IFG1 &= ~WDTIFG;                 /* clear interrupt flag */
  IE1 |= WDTIE;

  if (applicationMode == APP_APPLICATION_MODE)
  {
    tempCalibrated = tempAverage;
    calibrateUpdate  = 1;
  }
  else
  {
    applicationMode = APP_APPLICATION_MODE; // Switch from STANDBY to APPLICATION MODE
    __bic_SR_register_on_exit(LPM3_bits);
  }
}

// WDT Interrupt Service Routine used to de-bounce button press
#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void)
{
    IE1 &= ~WDTIE;                   /* disable interrupt */
    IFG1 &= ~WDTIFG;                 /* clear interrupt flag */
    WDTCTL = WDTPW + WDTHOLD;        /* put WDT back in hold state */
    BUTTON_IE |= BUTTON;             /* Debouncing complete */
}

// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
  __bic_SR_register_on_exit(CPUOFF);        // Return to active mode
}

