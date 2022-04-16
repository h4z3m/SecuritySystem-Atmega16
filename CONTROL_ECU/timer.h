/******************************************************************************
 *
 * Module: Timer
 *
 * File Name: timer.h
 *
 * Description: Header file for the AVR Timers driver
 *
 * Date Created: 18/10/2021
 *
 * Author: Hazem Montasser
 *
 *******************************************************************************/

#ifndef TIMER_H_
#define TIMER_H_
#include "std_types.h"

/*******************************************************************************
 *                                Types Declarations                           *
 *******************************************************************************/
/* Timer ID's for AVR Atmega16 */
typedef enum {
	TIMER0_ID, TIMER1_ID, TIMER2_ID
} Timer_ID;
/* Clock pre-scalers for Timers 0 & 1*/
typedef enum {
	NO_CLOCK_T01,
	FCPU_1_T01,
	FCPU_8_T01,
	FCPU_64_T01,
	FCPU_256_T01,
	FCPU_1024_T01,
	XTAL_T0_FALLING_T01,
	XTAL_T0_RISING_T01
} Timer01_Clock;

/* Clock pre-scalers for Timer 2*/
typedef enum {
	NO_CLOCK_T2,
	FCPU_1_T2,
	FCPU_8_T2,
	FCPU_32_T2,
	FCPU_64_T2,
	FCPU_128_T2,
	FCPU_256_T2,
	FCPU_1024_T2,

} Timer2_Clock;

/* Timer modes used to set certain bits in registers and load OCR values*/
typedef enum {
	Timer_Mode_Normal, Timer_Mode_Compare
} Timer_Mode;

/******************************************************************************
 *
 * Structure Name: Timer_ConfigType
 *
 * Structure Description: Structure responsible for configuring all 3 timers in
 *  Atmega16 uC.
 *
 *******************************************************************************/
typedef struct {
	uint8 Timer_ID; /* Timer ID: 0, 1 ,2*/
	uint8 Prescaler; /* Timer pre-scaler value */
	Timer_Mode Mode; /* OVF or COMP/Normal mode*/
	uint16 Compare_Value; /* Compare value */
	boolean Interrupt_Enable;/* Enable/disable interrupt for normal/compare modes*/
} Timer_ConfigType;

/*******************************************************************************
 *                           Functions Prototypes                              *
 *******************************************************************************/


/******************************************************************************
 *
 * Function Name: Timer_init
 *
 * Description:  Timer responsible for initializing AND starting any of the 3 timers
 * in Atmega16.
 * 		-If Mode in Timer_ConfigType is set to Timer_Mode_Normal, Compare_Value is ignored.
 * 		-Prescaler MUST be compatible with the Timer_ID chosen, see Timer clock enums.
 *
 * Args:
 *
 * 		[in] const Timer_ConfigType *Config
 * 			Pointer to structure which contains all needed configuration parameters
 * 			for timer.
 * 		[out] N/A
 * Returns: void
 *
 *******************************************************************************/
void Timer_init(const Timer_ConfigType *Config);


/******************************************************************************
 *
 * Function Name: Timer_setCallback
 *
 * Description:  Function responsible for setting the callback notification
 * 			for the selected timer.
 *
 * Args:
 *
 * 		[in] uint8 a_Timer_ID
				Timer ID  (0,1,2)
 * 			void (*a_ptrToCallback)(void)
 * 				Pointer to the callback function provided.
 * 		[out] N/A
 * Returns: void
 *
 *******************************************************************************/
void Timer_setCallback(uint8 a_Timer_ID, void (*a_ptrToCallback)(void));


/******************************************************************************
 *
 * Function Name: Timer_setCompareValue
 *
 * Description:  Sets the compare value in OCR register to a_CompareVal according
 * 				to the selected timer.
 *
 * Args:
 *
 * 		[in] uint8 a_Timer_ID
 *				Timer ID  (0,1,2)
 *			uint16 a_CompareVal
 *				New compare value to set in OCR register
 * 		[out] N/A
 * Returns: void
 *
 *******************************************************************************/
void Timer_setCompareValue(uint8 a_Timer_ID, uint16 a_CompareVal);


/******************************************************************************
 *
 * Function Name: Timer_getTimerValue
 *
 * Description:  Returns the current timer value
 *
 * Args:
 *
 * 		[in] uint8 a_Timer_ID
 *				Timer ID  (0,1,2)
 * 		[out] N/A
 *
 * Returns: uint16
 *
 *******************************************************************************/
uint16 Timer_getTimerValue(uint8 a_Timer_ID);


/******************************************************************************
 *
 * Function Name: Timer_stop
 *
 * Description:  Stops/pauses timer by changing the pre-scaler bits to 000.
 *		---Note: Does NOT clear timer register value.
 * Args:
 *
 * 		[in] uint8 a_Timer_ID
				Timer ID  (0,1,2)
 * 		[out] N/A
 *
 * Returns: void
 *
 *******************************************************************************/
void Timer_stop(uint8 a_Timer_ID);


/******************************************************************************
 *
 * Function Name: Timer_resume
 *
 * Description:  Resumes the timer by restoring the original pre-scaler bits.
 * 		---Note: Does NOT clear timer register value before resuming.
 * Args:
 *
 * 		[in] uint8 a_Timer_ID
				Timer ID  (0,1,2)
 * 		[out] N/A
 *
 * Returns: void
 *
 *******************************************************************************/
void Timer_resume(uint8 a_Timer_ID);


/******************************************************************************
 *
 * Function Name: Timer_resetTimerValue
 *
 * Description:  Resets timer counter register value according to selected timer.
 *
 * Args:
 *
 * 		[in] uint8 a_Timer_ID
				Timer ID  (0,1,2)
 * 		[out] N/A
 * Returns: void
 *
 *******************************************************************************/
void Timer_resetTimerValue(uint8 a_Timer_ID);


/******************************************************************************
 *
 * Function Name: Timer_DeInit
 *
 * Description:  De-initializes selected timer by clearing its counter, control
 * & OCR registers AND turns off both Timer overflow & Timer compare interrupts.
 * 		---Note: TIMSK bits are preserved, only the needed bits are zero'd.
 *
 * Args:
 *
 * 		[in] uint8 a_Timer_ID
 *				Timer ID  (0,1,2)
 * 		[out] N/A
 * Returns: void
 *
 *******************************************************************************/
void Timer_DeInit(uint8 a_Timer_ID);
#endif /* TIMER_H_ */
