// lab 1 exercise 1 1 Mhz version

#include <msp430.h> 

#define PWM_PERIOD		0x02
#define PWM_DUTYCYCLE	0x01
#define OUTPUT_PORT		BIT2
#define RANGE 			CALBC1_1MHZ
#define MODULATION 		CALDCO_1MHZ

// Function prototypes
void ConfigureClockModule(void);
void ConfigureTimerA(void);
void ConfigurePorts(void);

void main(void)
{
	// Stop the watchdog timer, and configure the clock module.
	WDTCTL = WDTPW + WDTHOLD;

	// Now, configure the clock module, the I/O ports, and timer A.
	ConfigureClockModule();
 	ConfigurePorts();
    ConfigureTimerA();

    // Infinite loop
 	while(1){
 		_nop();
 	};
}

void ConfigureClockModule()
{
	// Clock setup.
 	BCSCTL1 = RANGE;       			// Set range
	DCOCTL 	= MODULATION;        	// Set DCO step and modulation
}

void ConfigureTimerA(void)
{
	// Select the SMCLK for Timer A, in Up Mode.
 	TACTL = TASSEL_2 + MC_1;

 	/* TASSEL1 = 0x0200	Timer A clock source select 0
 	 * TASSEL0 = 0x0100	Timer A clock source select 1
 	 *  00 = TACLK
 	 *  01 = ACLK
 	 *  10 = SMCLK
 	 *  11 = INCLK
 	 * ID1 = 0x0080		Timer A clock input divider 1
 	 * ID0 = 0x0040		Timer A clock input divider 0
 	 *  00 = /1
 	 *  01 = /2
 	 *  10 = /4
 	 *  11 = /8
 	 * MC1 = 0x0020		Timer A mode control 1
 	 * MC0 = 0x0010		Timer A mode control 0
 	 *  00 = Stop mode: the timer is halted.
 	 *  01 = Up mode: the timer counts up to TACCR0.
 	 *  10 = Continuous mode: the timer counts up to 0FFFFh.
 	 *  11 = Up/down mode: the timer counts up to TACCR0 then down to 0000h.
 	 * TACLR = 0x0004	Timer A counter clear
 	 * TAIE = 0x0002	Timer A counter interrupt enable
 	 *  0 = Interrupt disabled
 	 *  1 = Interrupt enabled
 	 * TAIFG = 0x0001	Timer A counter interrupt flag
 	 *  0 = No interrupt pending
 	 *  1 = Interrupt pending
 	 */

	// Select Output Mode 7: Reset/Set in Up Mode.
 	TACCTL1 = OUTMOD_7;

 	/* CM1 = 0x8000 	Capture mode 1
 	 * CM0 = 0x4000 	Capture mode 0
 	 *  00 = No capture
 	 *  01 = Capture on rising edge
 	 *  10 = Capture on falling edge
 	 *  11 = Capture on both rising and falling edges
 	 * CCIS1 = 0x2000 	Capture input select 1
 	 * CCIS0 = 0x1000 	Capture input select 0
 	 *  00 = CCIxA
 	 *  01 = CCIxB
 	 *  10 = GND
 	 *  11 = VCC
 	 * SCS = 0x0800		Capture sychronize
 	 * SCCI = 0x0400	Latched capture signal (read)
 	 * CAP = 0x0100		Capture mode: 1 /Compare mode : 0
 	 * OUTMOD2 = 0x0080	Output mode 2
 	 * OUTMOD1 = 0x0040 Output mode 1
 	 * OUTMOD0 = 0x0020 Output mode 0
 	 *  000 = OUT bit value
 	 *  001 = Set
 	 *  010 = Toggle/reset
 	 *  011 = Set/reset
 	 *  100 = Toggle
 	 *  101 = Reset
 	 *  110 = Toggle/set
 	 *  111 = Reset/set
 	 * CCIE = 0x0010	Capture/compare interrupt enable
 	 * CCI = 0x0008		Capture input signal (read)
 	 * OUT = 0x0004		PWM Output signal if output mode 0
 	 * COV = 0x0002		Capture/compare overflow flag
 	 * CCIFG = 0x0001	Capture/compare interrupt flag
 	 */

    // Note that the timer period in Up Mode is TACCR0+1.
    // So, one is subtracted to corrected the desired PWM period.
 	TACCR0 	= PWM_PERIOD;		// PWM period
 	TACCR1 	= PWM_DUTYCYCLE;    // PWM duty cycle
}

void ConfigurePorts()
{
	P1DIR  |= OUTPUT_PORT;             // P1.2 to output
 	P1SEL  |= OUTPUT_PORT;             // P1.2 to TA0.1
}
