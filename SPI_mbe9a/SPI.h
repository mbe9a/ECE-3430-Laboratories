/*
 * SPI.h
 *
 *  Created on: Oct 4, 2015
 *      Author: Michael Eller mbe9a
 */

#ifndef SPI_H_
#define SPI_H_

// define statements
#define CE 				BIT5				// chip enable
#define SI				BIT2				// input
#define SCK				BIT4				// clock
#define RSR				0x05				// read-status-register op-code
#define SO				BIT1				// output
#define HOLD			BIT2				// hold pin
#define WP				BIT1				// write protection
#define WSR				0x01				// write-status-register op-code
#define EWSR			0x50				// enable-write-status-register
#define CLEAR			0x60				// chip erase
#define READ			0x03				// read op-code
#define WE				0x06				// write-enable
#define WD				0x04				// write-disable


// function prototypes
void chipEnable();
void chipDisable();
void initializeSPI();
void chipErase();
void AAI();
void readAll();
void wait_while_busy();
void sendData(unsigned char data);
unsigned char readData();
void enable_Write_Status_Register();
void write_Enable();
void write_Disable();
void writeSR(unsigned char data);
unsigned char readSR();

#endif /* SPI_H_ */
