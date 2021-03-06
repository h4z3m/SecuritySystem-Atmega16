/******************************************************************************
 *
 * Module: External EEPROM (M24C16)
 *
 * File Name: external_eeprom.h
 *
 * Description: Header file for external EEPROM driver.
 *
 * Date Created: 22/10/2021
 *
 * Author: Hazem Montasser
 *
 *******************************************************************************/

#ifndef EXTERNAL_EEPROM_H_
#define EXTERNAL_EEPROM_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define ERROR 				 (0x00)
#define SUCCESS 			 (0x01)
#define EEPROM_SLAVE_ADDRESS (0xA0)	/* EEPROM's slave address used to communicate with EEPROM*/

/*******************************************************************************
 *                            Functions Prototypes                             *
 *******************************************************************************/

/******************************************************************************
 *
 * Function Name: EEPROM_writeByte
 *
 * Description:  Writes a byte to a specific address in EEPROM.
 *
 * Args:
 *
 * 		[in] uint16 a_addr
 * 				To store the 10-bit address
 * 			 uint8 a_data
 * 			 	Actual byte to store in the EEPROM address location
 * 		[out] N/A
 * Returns: uint8 (SUCCESS/ERROR)
 *
 *******************************************************************************/
uint8 EEPROM_writeByte(uint16 a_addr, uint8 a_data);

/******************************************************************************
 *
 * Function Name: EEPROM_writeByte
 *
 * Description:  Reads a byte from a specific address in EEPROM.
 * 	---Note: Data is returned in form of a_data which is passed by address.
 *
 * Args:
 *
 * 		[in] uint16 a_addr
 * 				To store the 10-bit address
 * 			 uint8 *a_data
 *				Pointer to variable in which the byte is received from EEPROM
 * 		[out] N/A
 * Returns: uint8 (SUCCESS/ERROR)
 *
 *******************************************************************************/
uint8 EEPROM_readByte(uint16 a_addr, uint8 *a_data);

/******************************************************************************
 *
 * Function Name: EEPROM_writeString
 *
 * Description:  Writes an array of bytes to a specific address in EEPROM.
 *		---Note: If an error occurs during writing process, the data will be
 *				 partially stored inside EEPROM.
 * Args:
 *
 * 		[in] uint16 a_addr
 * 				To store the 10-bit address
 * 			 const uint8 *a_data
 * 			 	Actual array to store in the EEPROM address location
 * 		[out] N/A
 * Returns: uint8 (SUCCESS/ERROR)
 *
 *******************************************************************************/
uint8 EEPROM_writeString(uint16 a_addr, const uint8 *str, const uint8 size);

/******************************************************************************
 *
 * Function Name: EEPROM_readString
 *
 * Description:  Reads an array of bytes starting from a specific address in EEPROM.
 * 	---Notes: 1- Data is returned in form of a_data which is passed by address.
 * 			  2- If an error occurs during reading process, the rest of the elements
 * 			  in the array will be unchanged.
 * Args:
 *
 * 		[in] uint16 a_addr
 * 				To store the 10-bit address
 * 			 uint8 *a_data
 * 			 	Actual array to store data in from the EEPROM address location
 * 		[out] N/A
 * Returns: uint8 (SUCCESS/ERROR)
 *
 *******************************************************************************/
uint8 EEPROM_readString(uint16 a_addr, uint8 *str, uint8 size);

#endif /* EXTERNAL_EEPROM_H_ */
