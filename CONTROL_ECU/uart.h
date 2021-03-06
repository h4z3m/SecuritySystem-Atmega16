/******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for UART driver
 *
 * Date Created: 8/10/2021
 *
 * Author: Hazem Montasser
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_
#include "std_types.h"
#include <avr/io.h>
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#ifndef UART_EOS
#define UART_EOS	   ('#') /* UART End of String character for UART_receiveString function*/
#endif

#define TRANSMISSION_SPEED_DOUBLE TRUE
#define UART_SET_CHAR_SIZE(size)  UCSRB|=((size)&0x04),\
UCSRC|=(1<<URSEL)|((size&0x03)<<1)\

#define RX_INTERRUPT_ENABLE   FALSE
#define TX_INTERRUPT_ENABLE   FALSE
#define UDRE_INTERRUPT_ENABLE FALSE

/*******************************************************************************
 *                               Global Variables                              *
 *******************************************************************************/
/* Stores the latest incoming data*/
extern volatile uint16 g_UART_RX_data;
/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
/******************************************************************************
 *
 * Structure Name: UART_ConfigType
 *
 * Structure Description:  Configuration struct for UART
 * 		Includes settings for Baudrate (4800,9600...)
 * 		Stop bit: 1 or 2 stop bits
 * 		Character size: 5,6,7,8 bits
 *
 *******************************************************************************/
typedef enum {
	PARITY_DISABLED = 0, PARITY_EVEN = 2, PARITY_ODD = 3
} UART_PARITY_TYPE;
typedef enum {
	UART_CH_SIZE_5,
	UART_CH_SIZE_6,
	UART_CH_SIZE_7,
	UART_CH_SIZE_8,
	UART_CH_SIZE_9 = 7
} UART_CHARACTER_SIZE;
typedef struct {
	uint16 BaudRate;
	struct {
		uint8 Stop_Bit :1;
		uint8 Parity_type :2;
		uint8 Char_size :3;
	} UDReg;

} UART_ConfigType;
/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/******************************************************************************
 *
 * Function Name:UART_init
 *
 * Description: Function for UART module initialization according to config.
 * in struct
 *
 * Args:
 *
 * 		[in] UART_ConfigType *Config
 * 			Struct which contains configuration data
 * 		[out] N/A
 *
 * Returns: void
 *
 *******************************************************************************/
void UART_init(UART_ConfigType *Config);

/******************************************************************************
 *
 * Function Name:UART_sendByte
 *
 * Description: Sends 1 byte through TXD pin by polling until UDR is empty
 *
 * Args:
 *
 * 		[in] a_data
 * 			Actual byte to send
 * 		[out] N/A
 * Returns:
 *
 *******************************************************************************/
void UART_sendByte(uint8 a_data);

/******************************************************************************
 *
 * Function Name: UART_receiveByte
 *
 * Description: Polls until a byte is received
 *
 * Args: void
 *
 * Returns: uint8
 *
 *******************************************************************************/
uint8 UART_receiveByte(void);

/******************************************************************************
 *
 * Function Name: UART_sendString
 *
 * Description: Sends an array of uint8 string through UART
 *
 * Args:
 *
 * 		[in] Pointer to the string array (Must contain null terminator)
 * 			uint8 *str
 * 		[out] N/A
 *
 * Returns: void
 *
 *******************************************************************************/

void UART_sendString(uint8 *str);
/******************************************************************************
 *
 * Function Name: UART_receiveString
 *
 * Description: Receives a string from UART
 *
 * Args:
 *
 * 		[in] N/A
 * 		[out] uint8 *str
 * 			Pointer to uint8 array which will contain the received
 * 			string from UART
 * Returns: void
 *
 *******************************************************************************/
void UART_receiveString(uint8 *str);

/******************************************************************************
 *
 * Function Name: UART_set...Callback_Notif
 *
 * Description: Functions which set callback notification functions to be called when
 * 		ISR is executed
 *
 * Args:
 *
 * 		[in] void (*a_callBackNotif_ptr)(void)
 * 			Pointer to callback function
 * 		[out] N/A
 *
 * Returns: void
 *
 *******************************************************************************/
void UART_setTXCallback_Notif(void (*a_callBackNotif_ptr)(void));
void UART_setRXCallback_Notif(void (*a_callBackNotif_ptr)(void));
void UART_setUDRECallback_Notif(void (*a_callBackNotif_ptr)(void));
#endif /* UART_H_ */
