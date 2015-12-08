#ifndef LED_H
#define LED_H

#define ON 1
#define OFF 0

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *                 LED #1
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *   Color     :  Red
 *   GPIO      :  P1.0
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define LED1 1
#define LED1_PORT	P1OUT 	// Output port for LED 1
#define LED1_DDR	P1DIR 	// Data Direction Register (DDR) for LED 1
#define LED1_BIT	BIT0	// Port pin bit location for LED 1

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *                 LED #2
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *   Color     :  Green
 *   GPIO      :  P1.6
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define LED2 2
#define LED2_PORT   P1OUT	// Output for LED 2
#define LED2_DDR    P1DIR	// Data Direction Register (DDR) for LED 2
#define LED2_BIT	BIT6	// Port pin bit location for LED 2

// Prototypes
void InitializeLEDPortPins(void);
void SetLEDState(char choice, char state);

#endif // LED_H
