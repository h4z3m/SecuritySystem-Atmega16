/******************************************************************************
 *
 * Module: TWI / I2C
 *
 * File Name: twi.c
 *
 * Description: Header file for TWI driver.
 *
 * Date Created: 21/10/2021
 *
 * Author: Hazem Montasser
 *
 *******************************************************************************/

#ifndef TWI_H_
#define TWI_H_
#include "std_types.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define TWI_INTERRUPT_ENABLE TRUE

/*--Master Transmit Start/Rep start--*/
#define TWI_MT_START 	 	(0x08)
#define TWI_MT_REP_START 	(0x10)
/*--Master Transmit Slave address w/ WRITE & (ACK/NACK)--*/
#define TWI_MT_SLA_W_ACK 	(0x18)
#define TWI_MT_SLA_W_NACK 	(0x20)
/*--Master Transmit Slave address w/ READ & (ACK/NACK)--*/
#define TWI_MT_SLA_R_ACK 	(0x40)
#define TWI_MT_SLA_R_NACK 	(0x48)
/*--Master Transmit Data & (ACK/NACK)--*/
#define TWI_MT_DATA_ACK 	(0x28)
#define TWI_MT_DATA_NACK 	(0x38)
/*--Master Receive Data & (ACK/NACK)--*/
#define TWI_MR_DATA_ACK 	(0x50)
#define TWI_MR_DATA_NACK 	(0x58)

/*******************************************************************************
 *                                Types Declarations                           *
 *******************************************************************************/
/*Enum for pre-scaler bits for TWI*/
typedef enum {
	TWI_F_CPU_1, TWI_F_CPU_4, TWI_F_CPU_16, TWI_F_CPU_64
} TWI_PRESCALER;
/******************************************************************************
 *
 * Structure Name: TWI_ConfigType
 *
 * Structure Description:  Configuration struct for TWI settings.
 *
 *******************************************************************************/
typedef struct {
	uint32 BitRate; /* TWI required bitrate (400kb/s, 1Mb/s, etc...)*/
	uint8 SlaveAddress;/* Slave address for the MC in case of being a slave device */
	uint8 Prescaler; /* Pre-scaler for SCL */
} TWI_ConfigType;

/*******************************************************************************
 *                                Functions Prototypes                         *
 *******************************************************************************/

/******************************************************************************
 *
 * Function Name: TWI_init
 *
 * Description:  Initializes TWI module using the configuration struct.
 *
 * Args:
 *
 * 		[in] const TWI_ConfigType *Config
 * 			Configuration structure which includes pre-scaler, bit-rate & slave address.
 * 		[out] N/A
 * Returns: void
 *******************************************************************************/
void TWI_init(const TWI_ConfigType *Config);

/******************************************************************************
 *
 * Function Name: TWI_start
 *
 * Description:  Sends start bit.
 * 		If start is sent twice, then it counts as repeated start.
 * Args: void
 * Returns: void
 *******************************************************************************/
void TWI_start(void);

/******************************************************************************
 *
 * Function Name: TWI_stop
 *
 * Description: Sends stop bit to terminate frame.
 * Args: void
 * Returns: void
 *******************************************************************************/
void TWI_stop(void);

/******************************************************************************
 *
 * Function Name: TWI_writeByte
 *
 * Description:  Sends a byte of data from TWI.
 * Args:
 *
 * 		[in] a_data
 * 			Byte which is sent from TWI.
 * 		[out] N/A
 * Returns: void
 *******************************************************************************/
void TWI_writeByte(uint8 a_data);

/******************************************************************************
 *
 * Function Name: TWI_readByteWithAck
 *
 * Description:  Reads a byte from TWI and awaits ACK from slave.
 *
 * Args: void
 * Returns: uint8
 *
 *******************************************************************************/
uint8 TWI_readByteWithAck(void);

/******************************************************************************
 *
 * Function Name: TWI_readByteWithAck
 *
 * Description:  Reads a byte from TWI and awaits NACK from slave.
 *
 * Args: void
 * Returns: uint8
 *
 *******************************************************************************/
uint8 TWI_readByteWithNack(void);

/******************************************************************************
 *
 * Function Name: TWI_getStatus
 *
 * Description:  Reads TWI status from status register.
 *
 * Args: void
 * Returns: uint8
 *
 *******************************************************************************/
uint8 TWI_getStatus(void);
/******************************************************************************
 *
 * Function Name: TWI_setCallback
 *
 * Description:  Sets a callback function
 * Args:
 * 		[in] void (*a_PtrToFunc)(void)
 * 			Pointer to function that executes upon ISR.
 *		[out] N/A
 * Returns: void
 *******************************************************************************/
void TWI_setCallback(void (*a_PtrToFunc)(void));

/******************************************************************************
 *
 * Function Name: TWI_DeInit
 *
 * Description:  De-initializes TWI module by clearing all registers
 * 			and turns off interrupts.
 * Args: void
 * Returns: void
 *******************************************************************************/
void TWI_DeInit(void);

#endif /* TWI_H_ */
