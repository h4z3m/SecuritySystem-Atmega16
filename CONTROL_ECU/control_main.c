/******************************************************************************
 *
 * File Name: control_main.c
 *
 * Description: Source file for Control ECU
 * 				for door locking security system.
 *				Responsible for making decisions to open the door, trigger alarm,
 *				change/set password in EEPROM, and communicate with HMI through UART.
 *
 * Date Created: 10/27/2021
 *
 * Author: Hazem Montasser
 *
 *******************************************************************************/

#define F_CPU (8000000UL)
#include <avr/io.h>
#include <avr/interrupt.h>	/* To use sei() */
#include <math.h>			/* To use round() */
#include <util/delay.h>

/* Module headers */
#include "timer.h"
#include "twi.h"
#include "external_eeprom.h"
#include "uart.h"
#include "dc_motor.h"
#include "buzzer.h"
#include "system_modes.h"

/*******************************************************************************
 *                                      Definitions                            *
 *******************************************************************************/
#define TIMER_TOP_VALUE 7812UL				/* Timer compare top value used for delays of min time = 1s*/
#define TIMER_PRESCALER_VALUE (1024.0)		/* Decimal value of timer pre-scaler used in calculations of delay*/
#define EEPROM_PASSWORD_ADDRESS (0x0320)	/* Address in EEPROM where password will be stored*/
/*******************************************************************************
 *                            Global Variables (Private)			           *
 *******************************************************************************/
static uint8 g_password[PASSWORD_LENGTH] = { 0 }; /* Contains final password */
static uint8 g_pass_buffer1[PASSWORD_LENGTH] = { 0 }; /* Buffer for password entry #1*/
static uint8 g_pass_buffer2[PASSWORD_LENGTH] = { 0 }; /* Buffer for password entry #2*/
static uint8 HMI_status = MODE_FIRST_BOOT; /* Application status for HMI ECU*/
static uint8 timer_ticks = 0; /* Timer ticks delay_over is set to TRUE */
static uint8 delay_over = FALSE; /* Used to check if timer delay is over by the application*/
/*******************************************************************************
 *               Application Callback Functions Definitions     		       *
 *******************************************************************************/

/*
 * Description :
 * Callback function for Timer delay interrupt.
 */
void Control_Delay_Callback(void) {
	static uint8 tick = 0;
	/* Increment ticks every interrupt until it equals specified ticks */
	if (tick == timer_ticks) {
		/* Reset tick since it's static variable */
		tick = 0;
		/* Set flag when ticks are over to end the delay*/
		delay_over = TRUE;
	} else {
		++tick;
	}
}

/*******************************************************************************
 *                           Functions Definitions (Private)      		       *
 *******************************************************************************/

/*
 * Description :
 * Delay function of minimum delay of 1s
 */
static void delay_sec(uint8 a_sec) {
	/* Ttimer = 1/(FCPU/Prescaler)
	 *
	 * Tdelay = (Prescaler/FCPU) * no. of ticks * compare_value
	 * */
	/* Calculate no. of timer ticks needed for a particular delay of 's' seconds,
	 * round the result to avoid getting 0 ticks when it yields 0.99 */
	timer_ticks = (float) round(
			((a_sec / ((TIMER_PRESCALER_VALUE / F_CPU) * TIMER_TOP_VALUE))));

	/* Reset timer value before starting */
	Timer_resetTimerValue(TIMER1_ID);
	/* Start timer count */
	Timer_resume(TIMER1_ID);
	/* Poll for flag */
	while (!delay_over)
		;
	/* Stop timer */
	Timer_stop(TIMER1_ID);
	/* Reset flag for next delay */
	delay_over = FALSE;

}
/*
 * Description :
 * Compares 2 passwords and returns true if they match exactly, false if otherwise.
 */
static boolean pass_compare(const uint8 *a_arr1, const uint8 *a_arr2) {
	/* Loop on PASSWORD_LENGTH - 2 elements in array
	 * Last 2 characters are UART end of string and null terminator
	 * characters.
	 * */
	for (uint8 i = 0; i < PASSWORD_LENGTH - 2; i++) {
		/* Any element not matching the other returns false*/
		if (a_arr1[i] != a_arr2[i]) {
			return FALSE;
		}
	}
	/* Array contents match exactly */
	return TRUE;
}
/*
 * Description :
 * Copies given array into global password variable and writes it in EEPROM.
 */
static void set_password(const uint8 *a_arr) {
	for (uint8 i = 0; i < PASSWORD_LENGTH - 2; i++) {
		g_password[i] = a_arr[i];
	}
	EEPROM_writeString(EEPROM_PASSWORD_ADDRESS, g_password,
	PASSWORD_LENGTH-2);
}
/*
 * Description :
 * Confirms password attempts that the user enters through HMI.
 * If user enters maximum number of tries incorrectly, change mode to alarm.
 * If user enters the password correctly, change mode to given success state.
 *
 * UART_SENDS# = 2
 * UART_REC#   = 2
 */
static boolean confirmPasswordAttempts(uint8 a_desired_success_state) {
	/* Loop 3 times for 3 password attempts,
	 *  3rd password attempt results in the alarm triggering*/
	for (uint8 i = 0; i < MAX_PASSWORD_TRIES; i++) {
		while (UART_receiveByte() != SYNC_HANDSHAKE)
			;
		UART_receiveString(g_pass_buffer1);
		/* Password correct, exit loop to success state*/
		if (pass_compare(g_pass_buffer1, g_password)) {
			HMI_status = a_desired_success_state;
			UART_sendByte(SUCCESS);
			UART_sendByte(HMI_status);
			return TRUE;
		} else if (i < MAX_PASSWORD_TRIES - 1) {
			/* Password incorrect for 1st & 2nd time (i=0,1),
			 * attempt another try*/
			HMI_status = MODE_NORMAL_BOOT_LOCKED;
			UART_sendByte(ERROR);
			UART_sendByte(HMI_status);
		}
	}
	/* This part is reached ONLY after 3 failed attempts */
	HMI_status = MODE_ALARM_MODE;
	UART_sendByte(ERROR);
	UART_sendByte(HMI_status);
	return FALSE;
}

int main(void) {
	/* Modules configurations */

	/*
	 * TWI init:
	 * 	Bitrate = 400Kb/s
	 * 	Slave address = 0x02
	 * 	Pre-scaler clock = F_CPU x 1
	 * */
	TWI_ConfigType TWI_CONFIG = { 400000, 0x02, TWI_F_CPU_1 };

	/*
	 * Timer init:
	 * 	Timer used = Timer1
	 * 	Pre-scaler clock = F_CPU/1024
	 * 	Mode = Compare mode (CTC)
	 * 	Compare value = 7812 (gives one interrupt every 1s for every compare match)
	 * 	Interrupt on compare match = TRUE for enabled
	 * */
	Timer_ConfigType TIMER_CONFIG = { TIMER1_ID, FCPU_1024_T01,
			Timer_Mode_Compare, 7812,
			TRUE };

	/*
	 * UART init:
	 * 	Baudrate = 9600
	 * 	Character size = 8 bits
	 * 	Parity type = Disabled
	 * 	1 Stop bit
	 * */
	UART_ConfigType conf = {9600,{0,PARITY_DISABLED,UART_CH_SIZE_8}};

	/* Modules initialization */
	UART_init(&conf);
	Buzzer_init();
	DcMotor_Init();
	TWI_init(&TWI_CONFIG);
	/* Set callback timer function and stop timer initially */
	Timer_setCallback(TIMER1_ID, Control_Delay_Callback);
	Timer_init(&TIMER_CONFIG);
	Timer_stop(TIMER1_ID);
	Timer_resetTimerValue(TIMER1_ID);
	/* Enable global interrupts */
	sei();
	/*Super loop*/
	for (;;) {
		switch (HMI_status) {

		/************************** First boot, setting up new password  **************************/
		case MODE_FIRST_BOOT:
			/*Synchronize before receiving first password */
			while (UART_receiveByte() != SYNC_HANDSHAKE) {
			}
			UART_receiveString(g_pass_buffer1);
			/*Synchronize before receiving first password */
			while (UART_receiveByte() != SYNC_HANDSHAKE) {
			}
			UART_receiveString(g_pass_buffer2);
			/* Compare both passwords,
			 * store in EEPROM if match,
			 * re-try if no match*/
			if (pass_compare(g_pass_buffer1, g_pass_buffer2)) {
				UART_sendByte(SYNC_HANDSHAKE);
				UART_sendByte(SUCCESS);
				HMI_status = MODE_NORMAL_BOOT_MAIN;
				set_password(g_pass_buffer1);

			} else {
				UART_sendByte(SYNC_HANDSHAKE);
				UART_sendByte(ERROR);
			}
			break;
			/*********************** Mode for password attempts by the user ***********************/
		case MODE_NORMAL_BOOT_LOCKED:
			confirmPasswordAttempts(MODE_NORMAL_BOOT_MAIN);
			break;
		case MODE_NORMAL_BOOT_MAIN:
			/* User wants to open the door (pressed '+' key),
			 * request old password first*/
			while (UART_receiveByte() != SYNC_HANDSHAKE)
				;
			if (UART_receiveByte() == OPEN_DOOR_REQUEST) {
				if (confirmPasswordAttempts(MODE_NORMAL_BOOT_MAIN)) {
					/* Rotate motor for 15s clockwise (Opening door)*/
					DcMotor_Rotate(CW);
					delay_sec(15);
					/* Wait for 10s */
					DcMotor_Rotate(STOP);
					delay_sec(3);

					/* Notify HMI ECU to print locking message*/
					UART_sendByte(SYNC_HANDSHAKE);
					/* Rotate motor for 15s anti-clockwise (Closing door)*/
					DcMotor_Rotate(ACW);
					delay_sec(15);

					DcMotor_Rotate(STOP);
					/* Notify HMI ECU that door is closed to proceed */
					UART_sendByte(SYNC_HANDSHAKE);
				}
			}
			/* User wants to change password (pressed '-' key,
			 * request old password first*/
			else {
				confirmPasswordAttempts(MODE_FIRST_BOOT);
			}
			break;
			/******** Alarm mode triggered by wrong password entry, turn on buzzer for 60s ********/
		case MODE_ALARM_MODE:
			/* Start buzzer for 60s */
			Buzzer_ON();
			delay_sec(60);
			Buzzer_OFF();
			/* Return to main menu options */
			HMI_status = MODE_NORMAL_BOOT_MAIN;
			/* Notify HMI ECU of new status*/
			UART_sendByte(SYNC_HANDSHAKE);
			UART_sendByte(HMI_status);

			break;
		}

	}
}
