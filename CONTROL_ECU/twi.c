/******************************************************************************
 *
 * Module: TWI / I2C
 *
 * File Name: twi.c
 *
 * Description: Source file for TWI driver.
 *
 * Date Created: 21/10/2021
 *
 * Author: Hazem Montasser
 *
 *******************************************************************************/

#include "twi.h"
#include "common_macros.h"
#include <avr/io.h>
#include <avr/interrupt.h>
/*******************************************************************************
 *                        Global Variables(Private)                            *
 *******************************************************************************/

static volatile void (*g_TWI_callbackNotif)(void) = NULL_PTR;

/*******************************************************************************
 *                             ISR Definition                                  *
 *******************************************************************************/
ISR(TWI_vect) {
	if (g_TWI_callbackNotif != NULL_PTR) {
		(*g_TWI_callbackNotif)();
	}
}

/*******************************************************************************
 *                             Functions Definitions                           *
 *******************************************************************************/
/*
 * Description :
 * Private utility function that calculates a number raised to a power.
 * */
static uint32 power(uint16 base, uint16 index) {
	uint32 result = 1;
	for (uint8 i = 0; i < index; ++i) {
		result *= base;
	}
	return result;
}

void TWI_init(const TWI_ConfigType *Config) {

	/* Set slave address for MCU*/
	TWAR = Config->SlaveAddress;
	/* Insert pre-scaler bits into first 2 bits in TWSR register*/
	TWSR = Config->Prescaler & 0x03;
	/* Calculate bit rate
	 *
	 * SCL =       (FCPU)
	 *         --------------
	 * 		  16 + 2*TWBR*4^TWPS
	 *
	 * TWBR =    FCPU
	 *        ( ------  - 16 )
	 *            SCL
	 *         -----------------
	 *              2*4^TWPS
	 * TWI common speeds: 400kb/s, 1Mb/s, 1.7Mb/s, 3.4MB/s
	 *
	 * Technically on ATmega16 with max FCPU of 16MHz, 400kb/s would be the maximum SCL speed
	 * for TWI that gives valid value for TWBR with TWPS at the minimum value of 0.
	 * */

	TWBR = (uint8) (((F_CPU / Config->BitRate) - (16))
			/ (2 * power(4, (TWSR & 0x03))));
	/* Enable TWI, enable/disable interrupts */
	TWCR = (1 << TWEN) | (TWI_INTERRUPT_ENABLE << TWIE);
}

void TWI_start(void) {
	/* TWINT = 1 -> Clear flag by logic 1
	 * TWEN  = 1 -> To keep module enabled
	 * TWSTA = 1 -> Send start bit
	 **/
	TWCR = (1 << TWEN) | (1 << TWSTA) | (1 << TWINT);
	while (BIT_IS_CLEAR(TWCR, TWINT))
		;
}

void TWI_stop(void) {
	/* TWINT = 1 -> Clear flag by logic 1
	 * TWEN  = 1 -> To keep module enabled
	 * TWSTO = 1 -> Send stop bit
	 **/
	TWCR = (1 << TWEN) | (1 << TWSTO) | (1 << TWINT);
}

void TWI_writeByte(uint8 a_data) {
	/* Load data into register */
	TWDR = a_data;

	/* TWINT = 1 -> Clear flag by logic 1
	 * TWEN  = 1 -> To keep module enabled
	 **/
	TWCR = (1 << TWINT) | (1 << TWEN);

	/* Poll until TWI is ready to send */
	while (BIT_IS_CLEAR(TWCR, TWINT))
		;

}

uint8 TWI_readByteWithAck(void) {
	/* TWINT = 1 -> Clear flag by logic 1
	 * TWEN  = 1 -> To keep module enabled
	 * TWEA  = 1 -> Enable ACK
	 **/
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);

	/* Poll until TWI is ready to send */
	while (BIT_IS_CLEAR(TWCR, TWINT))
		;
	return TWDR;
}

uint8 TWI_readByteWithNack(void) {
	/* TWINT = 1 -> Clear flag by logic 1
	 * TWEN  = 1 -> To keep module enabled
	 **/
	TWCR = (1 << TWINT) | (1 << TWEN);

	/* Poll until TWI is ready to send */
	while (BIT_IS_CLEAR(TWCR, TWINT))
		;
	return TWDR;
}

uint8 TWI_getStatus(void) {
	/* Mask TWSR register to clear first 3 bits */
	return (TWSR & (0xF8));
}
void TWI_setCallback(void (*a_PtrToFunc)(void)) {
	/* Set callback pointer*/
	g_TWI_callbackNotif = a_PtrToFunc;
}

void TWI_DeInit(void) {
	/* Clear TWI registers */
	TWAR = 0;
	TWBR = 0;
	TWCR = 0;
	TWSR = 0;
}
