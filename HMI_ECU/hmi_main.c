/******************************************************************************
 *
 * File Name: hmi_main.c
 *
 * Description: Source file for HMI (Human Machine Interface) ECU
 * 				for door locking security system.
 * 				Responsible for displaying messages and gathering user input key
 * 				presses then sending it to Control ECU and await instructions.
 *
 * Date Created: 10/27/2021
 *
 * Author: Hazem Montasser
 *
 *******************************************************************************/

#define F_CPU (8000000UL)
#include <avr/io.h>
#include <util/delay.h>

/* Module headers */
#include "uart.h"
#include "lcd.h"
#include "keypad.h"
#include "std_types.h"
#include "../CONTROL_ECU/system_modes.h"
/*******************************************************************************
 *                            Global Variables (Private)				       *
 *******************************************************************************/
static uint8 g_password_buffer[PASSWORD_LENGTH]; /* Buffer for entered passwords by the user*/
static uint8 HMI_status = MODE_FIRST_BOOT; /* HMI initial status */
/*******************************************************************************
 *                           Functions Definitions (Private)      		       *
 *******************************************************************************/
/*
 * Description :
 * Gets the password from the user through keypad presses.
 * Displays * on the screen for every press entered.
 * Sends Sync and password string to control ECU for processing.
 *
 * UART_SENDS# = 2
 * UART_REC#   = 0
 */
static void getSendPasswordSync(void) {
	/* For every keypad press, print asterisk and store pressed key*/
	for (uint8 i = 0; i < PASSWORD_LENGTH - 2; i++) {
		g_password_buffer[i] = KEYPAD_getPressedKey();
		/* Adjust number to contain ASCII if its a number so that the number '0'
		 * is not counted as a null terminator character,
		 * if it's a symbol like '#','+','*'... then no need to adjust.
		 * */
		if (g_password_buffer[i] < 10) {
			g_password_buffer[i] += '0';
		}
		LCD_displayCharacter('*');
		_delay_ms(400);
	};
	/* Send password after syncing with control */
	UART_sendByte(SYNC_HANDSHAKE);
	UART_sendString(g_password_buffer);

}
/*
 * Description :
 * Displays first boot menu for password entry try #1
 */
static void printFirstBootMenu1(void) {
	LCD_clearScreen();
	LCD_displayString((const uint8*) "Please enter");
	LCD_displayStringRowColumn(LCD_ROW_1, 0, (const uint8*) "new pass: ");
}
/*
 * Description :
 * Displays first boot menu for password entry try #2
 */
static void printFirstBootMenu2(void) {
	LCD_clearScreen();
	LCD_displayString((const uint8*) "Please re-enter");
	LCD_displayStringRowColumn(LCD_ROW_1, 0, (const uint8*) "new pass: ");
}
/*
 * Description :
 * Print options for the user on LCD.
 */
static void printMainMenu(void) {
	LCD_clearScreen();
	LCD_displayString((const uint8*) "+ : Open door");
	LCD_displayStringRowColumn(LCD_ROW_1, 0, (const uint8*) "- : Change pass");
}
/*
 * Description :
 * Prints password prompt message for the user on LCD.
 */
static void printLockedMenu(void) {
	LCD_clearScreen();
	LCD_displayString((const uint8*) "Enter pass: ");
	LCD_moveCursor(LCD_ROW_1, 0);
}
/*
 * Description :
 * Error message for alarm mode when password is entered incorrectly
 */
static void printAlarmMessage(void) {
	LCD_clearScreen();
	LCD_displayString((const uint8*) "   !!!ERROR!!!");
}
/*
 * Description :
 * Prints the door status for the user on LCD while unlocking.
 */
static void printDoorUnlockingMessage(void) {
	LCD_clearScreen();
	LCD_displayString((const uint8*) "Door is");
	LCD_displayStringRowColumn(LCD_ROW_1, 0, (const uint8*) "unlocking...");
}
/*
 * Description :
 * Prints the door status for the user on LCD while locking.
 */
static void printDoorLockingMessage(void) {
	LCD_clearScreen();
	LCD_displayString((const uint8*) "Door is");
	LCD_displayStringRowColumn(LCD_ROW_1, 0, (const uint8*) "locking...");
}

/*
 * Description :
 * Allows the user to attempt the password until control ECU sends confirmation or
 * triggers alarm mode.
 *
 * UART_SENDS# = 2
 * UART_REC#   = 2
 */
static void attemptPassword(uint8 *password_match) {
	do {
		/* Get and send password to CONTROL ECU */
		printLockedMenu();
		_delay_ms(300);
		getSendPasswordSync();

		*password_match = UART_receiveByte();
		/* Receive next status from CONTROL ECU */
		HMI_status = UART_receiveByte();
	} while (!(*password_match) && (HMI_status != MODE_ALARM_MODE));

}

int main(void) {
	/* Initialize last element in buffer with null terminator for UART send string function to work correctly*/
	g_password_buffer[PASSWORD_LENGTH - 1] = '\0';
	/* Initialize element before last element in buffer with UART end of string character: '#'
	 *  for UART receive string function to work correctly*/
	g_password_buffer[PASSWORD_LENGTH - 2] = UART_EOS;
	boolean password_match = FALSE;
	uint8 keyPressed;
	/* Modules configurations */

	/*
	 * UART init:
	 * 	Baudrate = 9600 bps
	 * 	Character size = 8 bits
	 * 	Parity type = Disabled
	 * 	1 Stop bit
	 * */
	UART_ConfigType conf = {9600,{0,PARITY_DISABLED,UART_CH_SIZE_8}};

	/* Modules initialization */
	UART_init(&conf);
	LCD_init();
	/*Super loop*/
	for (;;) {
		switch (HMI_status) {

		/************************** First boot, setting up new password  **************************/
		case MODE_FIRST_BOOT:
			/* Get first password  */
			printFirstBootMenu1();
			getSendPasswordSync();
			/* Get second password */
			printFirstBootMenu2();
			getSendPasswordSync();
			/* SYNC UART */
			while (UART_receiveByte() != SYNC_HANDSHAKE) {
			}
			password_match = UART_receiveByte();
			/* If passwords match, change mode to main menu mode */
			if (password_match) {
				password_match = FALSE;
				HMI_status = MODE_NORMAL_BOOT_MAIN;
			}
			break;
		case MODE_NORMAL_BOOT_LOCKED:
			/*********************** Mode for password attempts by the user ***********************/
			attemptPassword(&password_match);
			break;
		case MODE_NORMAL_BOOT_MAIN:
			printMainMenu();
			/* Await +/- to be pressed by user */
			do {
				keyPressed = KEYPAD_getPressedKey();
			} while (keyPressed != '+' && keyPressed != '-');

			/* Delay to avoid de-bounce that triggers a wrong keystroke when attempting password*/
			_delay_ms(400);

			/* User wants to open the door (pressed '+' key),
			 * request old password first*/
			if (keyPressed == '+') {
				/* Send open door request to Control ECU and attempt password entry*/
				UART_sendByte(SYNC_HANDSHAKE);
				/* Send request to Control ECU to open the door */
				UART_sendByte(OPEN_DOOR_REQUEST);
				/* Allow user to attempt the password before proceeding */
				attemptPassword(&password_match);
				/* Password attempt was successful */

				if (HMI_status != MODE_ALARM_MODE) {
					printDoorUnlockingMessage();
					/* Wait until Control ECU opens the door */
					while (UART_receiveByte() != SYNC_HANDSHAKE)
						;
					printDoorLockingMessage();
					/* Wait until Control ECU closes the door */
					while (UART_receiveByte() != SYNC_HANDSHAKE)
						;
					/* Password attempt was unsuccessful */
				} else {
					/* Skip over to alarm mode*/
				}
				/* Reset key press to receive new press */
				keyPressed = 0;

			}

			/* User wants to change password (pressed '-' key,
			 * request old password first*/
			else {
				UART_sendByte(SYNC_HANDSHAKE);
				/* Send request to Control ECU to change the password */
				UART_sendByte(CHANGE_PASS_REQUEST);
				/* Allow user to attempt the password before proceeding */
				attemptPassword(&password_match);

				/* Reset key press to receive new press */
				keyPressed = 0;
			}
			break;
			/******** Alarm mode triggered by wrong password entry, displays error message ********/
		case MODE_ALARM_MODE:
			printAlarmMessage();
			/* Wait for a notification from Control ECU to exit alarm mode */
			while (UART_receiveByte() != SYNC_HANDSHAKE)
				;
			HMI_status = UART_receiveByte();
			break;
		}
	}
}
