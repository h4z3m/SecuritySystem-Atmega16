/******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.c
 *
 * Description: Source file for UART driver
 *
 * Date Created: 8/10/2021
 *
 * Author: Hazem Montasser
 *
 *******************************************************************************/

#include "uart.h"
#include "common_macros.h"
#include <avr/io.h>
#include <avr/interrupt.h>
/*******************************************************************************
 *                                Global Variables (Private)                   *
 *******************************************************************************/
static volatile void (*g_UART_TXC_Callback) = NULL_PTR;
static volatile void (*g_UART_RXC_Callback) = NULL_PTR;
static volatile void (*g_UART_UDRE_Callback) = NULL_PTR;
/*******************************************************************************
 *                                ISR's Definitions                            *
 *******************************************************************************/
#if (RX_INTERRUPT_ENABLE==TRUE)
ISR(USART_RXC_vect) {
	/* Invoke call to callback function */
	if (g_UART_RXC_Callback != NULL_PTR) {
		(*g_UART_RXC_Callback)();
	}
	/* Store incoming data into global variable */
	g_UART_RX_data = UDR;
}
#endif
#if (TX_INTERRUPT_ENABLE==TRUE)
ISR(USART_TXC_vect) {
	/* Invoke call to callback function */
	if (g_UART_TXC_Callback != NULL_PTR) {
		(*g_UART_TXC_Callback)();
	}
}
#endif
#if (UDRE_INTERRUPT_ENABLE==TRUE)
ISR(USART_UDRE_vect) {
	/* Invoke call to callback function */
	if (g_UART_UDRE_Callback != NULL_PTR) {
		(*g_UART_UDRE_Callback)();
	}
}
#endif

/*******************************************************************************
 *                              Function Definitions                           *
 *******************************************************************************/
void UART_init(UART_ConfigType *Config) {

	/* Set transmission speed either normal or double speed */
	UCSRA = (TRANSMISSION_SPEED_DOUBLE << U2X);

	/*
	 * RXEN,TXEN=1  -> Enable transmission and sending
	 * TXCIE,RXCIE,UDRIE -> Enable/disable interrupts on RX,TX,UDRE complete
	 *
	 * */

	UCSRB = (1 << RXEN) | (1 << TXEN) | (TX_INTERRUPT_ENABLE << TXCIE)
			| (RX_INTERRUPT_ENABLE << RXCIE) | (UDRE_INTERRUPT_ENABLE << UDRIE);

	/*
	 * URSEL=1 -> Enable write to UCSRC register
	 * USBS   -> Set stop bit: 1 or 2 bits
	 * */
	UCSRC = (1 << URSEL) | (Config->UDReg.Stop_Bit << USBS);

	/* Parity mode set*/

	if (Config->UDReg.Parity_type == PARITY_DISABLED) {
		/* Do nothing */
	} else {

		/* Extract parity and make sure only 2 bits then shift left to align with register */
		uint8 parity = ((Config->UDReg.Parity_type & 0x03) << UPM0);

		/* URSEL = 1 -> Enable write to UCSRC register */
		UCSRC |= (1 << URSEL) | parity;
	}

	/* Set UART character size: 5,6,7,8,9 bits*/
	UCSRB |= ((Config->UDReg.Char_size) & 0x04);
	UCSRC |= (1 << URSEL) | ((Config->UDReg.Char_size & 0x03) << UCSZ0);

	/* Calculate Baudrate and set it in UBRRL & UBBRH*/
	uint16 ubrr_value = (uint16) (((8000000 / (Config->BaudRate * 8UL))) - 1);

	/* Clear URSEL to select UBRR register*/
	UCSRC &= ~(1 << URSEL);
	UBRRH = ubrr_value >> 8;
	UBRRL = ubrr_value;
}
void UART_sendByte(uint8 a_data) {
	/* Poll until UDR register is empty*/
	while (BIT_IS_CLEAR(UCSRA, UDRE))
		;

	/* Send data */
	UDR = a_data;
}
uint8 UART_receiveByte(void) {
	/* Poll until byte is received */
	while (BIT_IS_CLEAR(UCSRA, RXC))
		;

	/* RXC flag is cleared once UDR register is read */
	return UDR;
}

void UART_sendString(uint8 *str) {

	/* Send each byte in array until null terminator*/
	while (*str != '\0') {
		UART_sendByte(*str);
		++str;
	}

}
void UART_receiveString(uint8 *str) {
	uint8 i = 0;
	/* Receive each byte until pre-defined End of string character */
	do {
		str[i] = UART_receiveByte();
		i++;
	} while (str[i] != UART_EOS);
	/* Add null terminator to string*/
	str[i] = '\0';
}

void UART_setTXCallback_Notif(void (*a_callBackNotif_ptr)(void)) {
	g_UART_TXC_Callback = a_callBackNotif_ptr;
}
void UART_setRXCallback_Notif(void (*a_callBackNotif_ptr)(void)) {
	g_UART_RXC_Callback = a_callBackNotif_ptr;
}
void UART_setUDRECallback_Notif(void (*a_callBackNotif_ptr)(void)) {
	g_UART_UDRE_Callback = a_callBackNotif_ptr;
}
void UART_DeInit(void) {
	UCSRA = UCSRB = UCSRC = 0;

}
