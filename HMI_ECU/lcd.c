/******************************************************************************
 *
 * Module: LCD
 *
 * File Name: lcd.c
 *
 * Description: Source file for the LCD driver
 *
 * Author: Hazem Montasser
 *
 *******************************************************************************/

#include "lcd.h"
#include "gpio.h"
#include "util/delay.h"
#include <stdlib.h>	/*To use itoa function*/

/*Description: Sends a command to the LCD as given by a_data */
void LCD_sendCommand(uint8 a_data) {

	/*RS = 0 to send command*/
	GPIO_writePin(LCD_CONTROL_PORT, LCD_RS, LOGIC_LOW);

	/*R/W = 0 to write to LCD*/
	GPIO_writePin(LCD_CONTROL_PORT, LCD_RW, LOGIC_LOW);

	/*tas = 50ns*/
	_delay_us(20);

	/*Enable = 1 before sending data*/
	GPIO_writePin(LCD_CONTROL_PORT, LCD_ENABLE, LOGIC_HIGH);

	/*tpw - tdsw = 190ns*/
	_delay_us(20);
#if (LCD_4BIT_MODE==4 && LCD_FIRST_4_PINS==1)

	/*Send higher 4 bits first and other bit states*/
	GPIO_writePort(LCD_DATA_PORT,
			(a_data >> LCD_4BIT_MODE) | (GPIO_readPort(LCD_DATA_PORT) & 0xF0));
#elif	 (LCD_4BIT_MODE==4 && LCD_LAST_4_PINS==1)
	GPIO_writePort(LCD_DATA_PORT,
			(a_data & 0xF0) | (GPIO_readPort(LCD_DATA_PORT) & 0x0F));

#else
	GPIO_writePort(LCD_DATA_PORT, (a_data));
#endif
	/*tdsw = 100ns*/
	_delay_us(20);

	/*Enable == 0 after sending data*/
	GPIO_writePin(LCD_CONTROL_PORT, LCD_ENABLE, LOGIC_LOW);
	_delay_us(20);
#if (LCD_4BIT_MODE==4 && LCD_FIRST_4_PINS==1)

	/*Enable = 1 before sending data*/
	GPIO_writePin(LCD_CONTROL_PORT, LCD_ENABLE, LOGIC_HIGH);

	/*tpw - tdsw = 190ns*/
	_delay_us(20);

	/*Send lower 4 bits first and preserve other bit states*/
	GPIO_writePort(LCD_DATA_PORT,
			(a_data & 0x0F) | (GPIO_readPort(LCD_DATA_PORT) & 0xF0));
	/*tdsw = 100ns*/
	_delay_us(20);

	/*Enable == 0 after sending data*/
	GPIO_writePin(LCD_CONTROL_PORT, LCD_ENABLE, LOGIC_LOW);
#elif	 (LCD_4BIT_MODE==4 && LCD_LAST_4_PINS==1)
	/*Enable = 1 before sending data*/
	GPIO_writePin(LCD_CONTROL_PORT, LCD_ENABLE, LOGIC_HIGH);

	/*tpw - tdsw = 190ns*/
	_delay_us(20);
	GPIO_writePort(LCD_DATA_PORT,
			(a_data << LCD_4BIT_MODE) | (GPIO_readPort(LCD_DATA_PORT) & 0x0F));

	/*tdsw = 100ns*/
	_delay_us(20);

	/*Enable == 0 after sending data*/
	GPIO_writePin(LCD_CONTROL_PORT, LCD_ENABLE, LOGIC_LOW);
#endif

	/*Safety*/
	_delay_us(20);
}

/*Description: Displays a single character given by a_char */
void LCD_displayCharacter(uint8 a_data) {

	/*RS = 1 to send data*/
	GPIO_writePin(LCD_CONTROL_PORT, LCD_RS, LOGIC_HIGH);

	/*R/W = 0 to write to LCD*/
	GPIO_writePin(LCD_CONTROL_PORT, LCD_RW, LOGIC_LOW);

	/*tas = 50ns*/
	_delay_us(20);

	/*Enable = 1 before sending data*/
	GPIO_writePin(LCD_CONTROL_PORT, LCD_ENABLE, LOGIC_HIGH);

	/*tpw - tdsw = 190ns*/
	_delay_us(20);
#if (LCD_4BIT_MODE==4 && LCD_FIRST_4_PINS==1)

	/*Send higher 4 bits first and other bit states*/
	GPIO_writePort(LCD_DATA_PORT,
			(a_data >> LCD_4BIT_MODE) | (GPIO_readPort(LCD_DATA_PORT) & 0xF0));
#elif	 (LCD_4BIT_MODE==4 && LCD_LAST_4_PINS==1)
	GPIO_writePort(LCD_DATA_PORT,
			(a_data & 0xF0) | (GPIO_readPort(LCD_DATA_PORT) & 0x0F));

#else
	GPIO_writePort(LCD_DATA_PORT, (a_data));
#endif
	/*tdsw = 100ns*/
	_delay_us(20);

	/*Enable == 0 after sending data*/
	GPIO_writePin(LCD_CONTROL_PORT, LCD_ENABLE, LOGIC_LOW);
	_delay_us(20);
#if (LCD_4BIT_MODE==4 && LCD_FIRST_4_PINS==1)

	/*Enable = 1 before sending data*/
	GPIO_writePin(LCD_CONTROL_PORT, LCD_ENABLE, LOGIC_HIGH);

	/*tpw - tdsw = 190ns*/
	_delay_us(20);

	/*Send lower 4 bits first and preserve other bit states*/
	GPIO_writePort(LCD_DATA_PORT,
			(a_data & 0x0F) | (GPIO_readPort(LCD_DATA_PORT) & 0xF0));
	/*tdsw = 100ns*/
	_delay_us(20);

	/*Enable == 0 after sending data*/
	GPIO_writePin(LCD_CONTROL_PORT, LCD_ENABLE, LOGIC_LOW);
#elif	 (LCD_4BIT_MODE==4 && LCD_LAST_4_PINS==1)
	/*Enable = 1 before sending data*/
	GPIO_writePin(LCD_CONTROL_PORT, LCD_ENABLE, LOGIC_HIGH);

	/*tpw - tdsw = 190ns*/
	_delay_us(20);
	GPIO_writePort(LCD_DATA_PORT,
			(a_data << LCD_4BIT_MODE) | (GPIO_readPort(LCD_DATA_PORT) & 0x0F));

	/*tdsw = 100ns*/
	_delay_us(20);

	/*Enable == 0 after sending data*/
	GPIO_writePin(LCD_CONTROL_PORT, LCD_ENABLE, LOGIC_LOW);
#endif

	/*Safety*/
	_delay_us(20);
}

/*Description: Initializes the LCD data bus & control pins
 * according to the pre-dfined preprocessor definitions
 * */
void LCD_init(void) {
	GPIO_setupPinDirection(LCD_CONTROL_PORT, LCD_RS, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_CONTROL_PORT, LCD_RW, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_CONTROL_PORT, LCD_ENABLE, PIN_OUTPUT);
#if (LCD_MODE==8)
	GPIO_setupPortDirection(LCD_DATA_PORT, PORT_OUTPUT);
#elif (LCD_MODE==4 && LCD_FIRST_4_PINS==1)
	GPIO_setupPinDirection(LCD_DATA_PORT, PIN0_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PORT, PIN0_ID+1, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PORT, PIN0_ID+2, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PORT, PIN0_ID+3, PIN_OUTPUT);
#elif (LCD_MODE==4 && LCD_LAST_4_PINS==1)
	GPIO_setupPinDirection(LCD_DATA_PORT, PIN4_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PORT, PIN4_ID+1, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PORT, PIN4_ID+2, PIN_OUTPUT);
 	GPIO_setupPinDirection(LCD_DATA_PORT, PIN4_ID+3, PIN_OUTPUT);
#endif

#if (LCD_MODE==4 )
	LCD_sendCommand(LCD_COMMAND_4BIT_INIT);
#endif

	LCD_sendCommand(LCD_COMMAND_MODE);
	LCD_sendCommand(LCD_COMMAND_CURSOR_OFF);
	LCD_sendCommand(LCD_COMMAND_CLEAR);

}

/*Description: Displays a string on the LCD from a character array
 * If the number of characters overflow the maximum LCD columns,
 * the function will print from the current cursor position until end of row only
 * */
void LCD_displayString(const uint8 *str) {
	uint8 counter = 0;

	while (str[counter] != '\0') {
		LCD_displayCharacter(str[counter]);
		++counter;
	}

}

/*
 * Description: Moves the cursor to the specified row, column coordinates on the LCD
 * If a_row is more than the available rows, the function places the cursor to the first line only
 * if a_col is more than the available columns, the function places the cursor starting from the first column only
 * */
void LCD_moveCursor(uint8 a_row, uint8 a_col) {
	/* Variable to hold position the cursor on the screen to be sent to the LCD data bus
	 * First 7 bits hold the column address
	 * Last bit (MSB) forces cursor to the position*/

	uint8 lcd_memory_register = LCD_REG_COL_0;
	/* Check if the given column is more than the maximum*/
	if (a_col > LCD_MAX_COLS) {

	} else {
		switch (a_row) {
		case LCD_ROW_0:
			lcd_memory_register = a_col + LCD_REG_COL_0;
			break;

		case LCD_ROW_1:
			lcd_memory_register = a_col + LCD_REG_COL_1;
			break;

		case LCD_ROW_2:
			lcd_memory_register = a_col + LCD_REG_COL_2;
			break;

		case LCD_ROW_3:
			lcd_memory_register = a_col + LCD_REG_COL_3;
			break;
		}
	}
	LCD_sendCommand(lcd_memory_register | (LCD_COMMAND_FORCE_CURSOR));
}

/*
 * Description: Prints a string starting from the given row and column positions
 * If a_row is more than the available rows, the function prints to the first line only
 * if a_col is more than the available columns, the function prints starting from the first column only
 * */
void LCD_displayStringRowColumn(uint8 a_row, uint8 a_col, const uint8 *a_str) {
	LCD_moveCursor(a_row, a_col);
	LCD_displayString(a_str);

}

/*
 * Description: Clears the LCD by sending the LCD_COMMAND_CLEAR command to the LCD
 * */
void LCD_clearScreen(void) {
	LCD_sendCommand(LCD_COMMAND_CLEAR);
}

/*
 * Description: Converts a given integer (+ve, -ve) to an array of characters
 * to display it on the LCD
 * Numbers given are in Base - 10
 * */
void LCD_integerToString(int a_data) {
	/* Array of characters to hold the integer numbers */
	uint8 buff[LCD_MAX_COLS];

	/* Convert incoming integer into an array of characters and store it in buff,
	 * casting to void ptr to remove warning */
	itoa(a_data, (void*) buff, 10);

	/* Display the converted integer */
	LCD_displayString(buff);
}
