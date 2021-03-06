/******************************************************************************
 *
 * Module: External EEPROM (M24C16)
 *
 * File Name: external_eeprom.c
 *
 * Description: Source file for external EEPROM driver.
 *
 * Date Created: 22/10/2021
 *
 * Author: Hazem Montasser
 *
 *******************************************************************************/

#include "external_eeprom.h"
#include "twi.h"

/*******************************************************************************
 *                          Functions Definitions                              *
 *******************************************************************************/

uint8 EEPROM_writeByte(uint16 a_addr, uint8 a_data) {
	/* Send start bit */
	TWI_start();
	if (TWI_getStatus() != TWI_MT_START) {
		return ERROR;
	}

	/* Mask the slave address of EEPROM with the last 3 bits of	 * the memory address (A8,A9,A10) with R/W = 0 for write request*/

	uint8 slave_w_3addr = EEPROM_SLAVE_ADDRESS | ((a_addr & 0x0700) >> 7);

	TWI_writeByte((uint8) slave_w_3addr);

	if (TWI_getStatus() != TWI_MT_SLA_W_ACK) {
		return ERROR;
	}
	/* Send the rest of the address and await ACK*/
	TWI_writeByte((uint8) a_addr);
	if (TWI_getStatus() != TWI_MT_DATA_ACK) {
		return ERROR;
	}
	/* Send the data and await ACK*/
	TWI_writeByte((uint8) a_data);
	if (TWI_getStatus() != TWI_MT_DATA_ACK) {
		return ERROR;
	}
	/* End of frame */
	TWI_stop();
	return SUCCESS;
}
uint8 EEPROM_readByte(uint16 a_addr, uint8 *a_data) {
	/* Send start bit */
	TWI_start();
	if (TWI_getStatus() != TWI_MT_START) {
		return ERROR;
	}

	/* Mask the slave address of EEPROM with the last 3 bits of
	 * the memory address (A8,A9,A10) with R/W = 0 for write request*/
	uint8 slave_w_3addr = EEPROM_SLAVE_ADDRESS | ((a_addr & 0x0700) >> 7);

	TWI_writeByte((uint8) slave_w_3addr);

	if (TWI_getStatus() != TWI_MT_SLA_W_ACK) {
		return ERROR;
	}
	/* Send the rest of the address and await ACK*/
	TWI_writeByte((uint8) a_addr);

	if (TWI_getStatus() != TWI_MT_DATA_ACK) {
		return ERROR;
	}
	/* Send repeated start */
	TWI_start();
	if (TWI_getStatus() != TWI_MT_REP_START) {
		return ERROR;
	}
	/* Send slave address with R/W = 1 for read request and await ACK*/
	TWI_writeByte((uint8) (slave_w_3addr | 0x01));
	if (TWI_getStatus() != TWI_MT_SLA_R_ACK) {
		return ERROR;
	}
	/* Receive data in a_data without ACK to terminate the frame*/
	*a_data = TWI_readByteWithNack();
	if (TWI_getStatus() != TWI_MR_DATA_NACK) {
		return ERROR;
	}
	/* End of frame */
	TWI_stop();
	return SUCCESS;
}

uint8 EEPROM_writeString(uint16 a_addr, const uint8 *str, uint8 size) {
	/* Write (size-1) bytes to EEPROM, return if any error occurs */
	for (uint8 i = 0; i < size; i++) {
		if(!EEPROM_writeByte(a_addr + i, str[i])){
			return ERROR;
		}
	}
	return SUCCESS;
}
uint8 EEPROM_readString(uint16 a_addr, uint8 *str, uint8 size) {
	/* Read (size-1) bytes to EEPROM, return if any error occurs */
	for (uint8 i = 0; i < size; i++) {
		if(!EEPROM_readByte(a_addr + i,&str[i])){
			return ERROR;
		}
	}
	return SUCCESS;
}
