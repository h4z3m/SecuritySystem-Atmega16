/******************************************************************************
 *
 * Module: Timer
 *
 * File Name: timer.c
 *
 * Description: Source file for the AVR Timers driver
 *
 * Date Created: 18/10/2021
 *
 * Author: Hazem Montasser
 *
 *******************************************************************************/

#include "timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/*******************************************************************************
 *                         Global Variables(Private)                           *
 *******************************************************************************/

/**** Callback pointer to function variables used in ISR's****/
volatile static void (*g_Timer0_callbackNotif)(void) = NULL_PTR;
volatile static void (*g_Timer1_callbackNotif)(void) = NULL_PTR;
volatile static void (*g_Timer2_callbackNotif)(void) = NULL_PTR;

/**** Saves the current clock pre-scaler to be used in Timer_resume function****/
volatile static Timer01_Clock Timer0_Current_Clock = 0;
volatile static Timer01_Clock Timer1_Current_Clock = 0;
volatile static Timer2_Clock Timer2_Current_Clock = 0;

/*******************************************************************************
 *                              ISR's Definitions                              *
 *******************************************************************************/

/**********Timer 0 ISR's**********/
ISR(TIMER0_COMP_vect) {
	if (g_Timer0_callbackNotif != NULL_PTR) {
		(*g_Timer0_callbackNotif)();
	}
}
ISR(TIMER0_OVF_vect) {
	if (g_Timer0_callbackNotif != NULL_PTR) {
		(*g_Timer0_callbackNotif)();
	}
}

/**********Timer 1 ISR's**********/
ISR(TIMER1_OVF_vect) {
	if (g_Timer1_callbackNotif != NULL_PTR) {
		(*g_Timer1_callbackNotif)();
	}
}
ISR(TIMER1_COMPA_vect) {
	if (g_Timer1_callbackNotif != NULL_PTR) {
		(*g_Timer1_callbackNotif)();
	}
}

/**********Timer 2 ISR's**********/
ISR(TIMER2_OVF_vect) {
	if (g_Timer2_callbackNotif != NULL_PTR) {
		(*g_Timer2_callbackNotif)();
	}
}
ISR(TIMER2_COMP_vect) {
	if (g_Timer2_callbackNotif != NULL_PTR) {
		(*g_Timer2_callbackNotif)();
	}
}

/*******************************************************************************
 *                              Functions Definitions                          *
 *******************************************************************************/

void Timer_init(const Timer_ConfigType *Config) {

	switch (Config->Timer_ID) {

	/********************************* Timer 0 Setup *********************************/
	case TIMER0_ID:
		/**************** Clearing timer/counter register ****************/

		TCNT0 = 0;

		/**************** Configuring Mode, Interrupts & Pre-scaler ****************/

		if (Config->Mode == Timer_Mode_Compare) {
			/* Load compare value into OCR0 register*/
			OCR0 = Config->Compare_Value;

			/* Enable/Disable interrupt for compare mode*/
			TIMSK |= (Config->Interrupt_Enable << OCIE0);

		} else if (Config->Mode == Timer_Mode_Normal) {

			/* Enable/Disable interrupt for overflow mode*/
			TIMSK |= (Config->Interrupt_Enable << TOIE0);
		}
		Timer0_Current_Clock = Config->Prescaler;
		TCCR0 = (1 << FOC0) | (Config->Mode << WGM01)
				| (Config->Prescaler & 0x07);

		break;

		/********************************* Timer 1 Setup *********************************/
	case TIMER1_ID:

		/**************** Clearing timer/counter register ****************/
		TCNT1 = 0;

		/**************** Configuring FOC bits ****************/

		/*
		 * -FOC1A,B = 1 -> Needed for any non-PWM mode.
		 * */
		TCCR1A = (1 << FOC1A) | (1 << FOC1B);

		/**************** Configuring Mode, Interrupts & Pre-scaler ****************/

		if (Config->Mode == Timer_Mode_Compare) {
			/* Load compare value into OCR0 register*/
			OCR1A = Config->Compare_Value;

			/* Enable/Disable interrupt for compare mode*/
			TIMSK |= (Config->Interrupt_Enable << OCIE1A);
		} else if (Config->Mode == Timer_Mode_Normal) {
			/* Enable/Disable interrupt for overflow mode*/
			TIMSK |= (Config->Interrupt_Enable << TOIE1);
		}
		Timer1_Current_Clock = Config->Prescaler;
		/* -WGM12 = Mode-> To set mode to either normal/CTC
		 * 	For normal mode: WGM1 2:0 = 000
		 * 	For CTC    mode: WGM1 2:0 = 100
		 * -Insert pre-scaler bits CS1 2:0 in the first 3 bits
		 * */
		TCCR1B = (Config->Mode << WGM12) | (Config->Prescaler & 0x07);
		break;
		/********************************* Timer 2 Setup *********************************/
	case TIMER2_ID:

		/**************** Clearing timer/counter register ****************/

		TCNT2 = 0;

		/**************** Configuring Mode, Interrupts & Pre-scaler ****************/

		if (Config->Mode == Timer_Mode_Compare) {
			/* Load compare value into OCR0 register*/
			OCR2 = Config->Compare_Value;

			/* Enable/Disable interrupt for compare mode*/
			TIMSK |= (Config->Interrupt_Enable << OCIE2);

		} else if (Config->Mode == Timer_Mode_Normal) {

			/* Enable/Disable interrupt for overflow mode*/
			TIMSK |= (Config->Interrupt_Enable << TOIE2);
		}
		Timer2_Current_Clock = Config->Prescaler;
		/* -FOC2 = 0  -> Needed for any non-PWM mode
		 * -Insert pre-scaler bits CS2 2:0 in the first 3 bits
		 * -WGM21 = Mode-> To set mode to either normal/CTC
		 * 	For normal mode: WGM2 1:0 = 00
		 * 	For CTC    mode: WGM2 1:0 = 10
		 * */
		TCCR2 = (1 << FOC2) | (Config->Mode << WGM21)
				| ((Config->Prescaler) & 0x07);

		break;

	}

}
void Timer_setCallback(uint8 a_Timer_ID, void (*a_ptrToCallback)(void)) {
	/* Set the callback function of a timer according to timer ID*/
	switch (a_Timer_ID) {
	case TIMER0_ID:
		g_Timer0_callbackNotif = a_ptrToCallback;
		break;
	case TIMER1_ID:
		g_Timer1_callbackNotif = a_ptrToCallback;
		break;
	case TIMER2_ID:
		g_Timer2_callbackNotif = a_ptrToCallback;
		break;
	}
}

void Timer_setCompareValue(uint8 a_Timer_ID, uint16 a_CompareVal) {
	/* Sets a new compare value in OCR register according to timer ID*/
	switch (a_Timer_ID) {
	case TIMER0_ID:
		OCR0 = a_CompareVal;
		break;
	case TIMER1_ID:
		OCR1A = a_CompareVal;
		break;
	case TIMER2_ID:
		OCR2 = a_CompareVal;
		break;
	}
}
uint16 Timer_getTimerValue(uint8 a_Timer_ID) {
	switch (a_Timer_ID) {
	case TIMER0_ID:
		return TCNT0;
		break;
	case TIMER1_ID:
		return TCNT1;
		break;
	case TIMER2_ID:
		return TCNT2;
		break;
	}
	return 0;
}
void Timer_stop(uint8 a_Timer_ID) {
	switch (a_Timer_ID) {
	case TIMER0_ID:
		/* Clear first 3 bits which set clock */
		TCCR0 &= (0xF8);
		break;
	case TIMER1_ID:
		/* Clear first 3 bits which set clock */
		TCCR1B &= (0xF8);
		break;
	case TIMER2_ID:
		/* Clear first 3 bits which set clock */
		TCCR2 &= (0xF8);
		break;
	}
}
void Timer_resume(uint8 a_Timer_ID) {
	switch (a_Timer_ID) {
	case TIMER0_ID:
		TCCR0 |= (Timer0_Current_Clock & 0x07);
		break;
	case TIMER1_ID:
		TCCR1B |= (Timer1_Current_Clock & 0x07);
		break;
	case TIMER2_ID:
		TCCR2 |= (Timer0_Current_Clock & 0x07);
		break;
	}
}

void Timer_resetTimerValue(uint8 a_Timer_ID) {
	switch (a_Timer_ID) {
	case TIMER0_ID:
		TCNT0 = 0;
		break;
	case TIMER1_ID:
		TCNT1 = 0;
		break;
	case TIMER2_ID:
		TCNT2 = 0;
		break;
	}
}

void Timer_DeInit(uint8 a_Timer_ID) {
	switch (a_Timer_ID) {
	case TIMER0_ID:
		/* Clear timer registers */
		TCCR0 = 0;
		TCNT0 = 0;
		OCR0 = 0;
		/* Disable interrupts */
		TIMSK &= ~((1 << TOIE0) | (1 << OCIE0));
		break;
	case TIMER1_ID:
		/* Clear timer registers */
		TCCR1A = TCCR1B = 0;
		TCNT1 = 0;
		OCR1A = 0;
		/* Disable interrupts */
		TIMSK &= ~((1 << TOIE1) | (1 << OCIE1A));
		break;
	case TIMER2_ID:
		/* Clear timer registers */
		TCCR0 = 0;
		TCNT0 = 0;
		OCR2 = 0;
		/* Disable interrupts */
		TIMSK &= ~((1 << TOIE2) | (1 << OCIE2));
		break;
	}
}

