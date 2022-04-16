/******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name: buzzer.c
 *
 * Description: Source file for buzzer driver.
 *
 * Date Created: 26/10/2021
 *
 * Author: Hazem Montasser
 *
 *******************************************************************************/

#include "buzzer.h"
#include "gpio.h"
#include <util/delay.h>

void Buzzer_init(void) {
	GPIO_setupPinDirection(BUZZER_PORT, BUZZER_PIN, PIN_OUTPUT);
}

void Buzzer_pulse_ms(uint32 duration) {
	GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_HIGH);
	_delay_ms(duration);
	GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_LOW);
}

void Buzzer_ON(void) {
	GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_HIGH);
}

void Buzzer_OFF(void) {
	GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_LOW);
}
