/******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name: buzzer.h
 *
 * Description: Header file for buzzer driver.
 *
 * Date Created: 26/10/2021
 *
 * Author: Hazem Montasser
 *
 *******************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_
#include "std_types.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define BUZZER_PORT PORTD_ID
#define BUZZER_PIN PIN7_ID
#define BUZZER_DEFAULT_PULSE_MS (500U)
/*******************************************************************************
 *                           Functions Prototypes                              *
 *******************************************************************************/

/******************************************************************************
 *
 * Function Name: Buzzer_init
 *
 * Description:  Initializes buzzer o/p pin.
 *
 * Args: N/A
 *
 * Returns: void
 *
 *******************************************************************************/
void Buzzer_init();

/******************************************************************************
 *
 * Function Name: Buzzer_pulse_ms
 *
 * Description:  Pulses the buzzer pin for a specified duration of time in ms.
 * Args: N/A
 * Returns: void
 *******************************************************************************/
void Buzzer_pulse_ms(uint32 duration);

/******************************************************************************
 *
 * Function Name: Buzzer_ON
 *
 * Description:  Turns on buzzer by writing LOGIC HIGH to its pin.
 * Args: N/A
 * Returns: void
 *******************************************************************************/
void Buzzer_ON(void);

/******************************************************************************
 *
 * Function Name: Buzzer_OFF
 *
 * Description:  Turns off buzzer by writing LOGIC LOW to its pin.
 * Args: N/A
 * Returns: void
 *******************************************************************************/
void Buzzer_OFF(void);

#endif /* BUZZER_H_ */
