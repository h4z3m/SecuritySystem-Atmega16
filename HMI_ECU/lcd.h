/******************************************************************************
 *
 * Module: LCD
 *
 * File Name: lcd.h
 *
 * Description: Header file for the LCD driver
 *
 * Author: Hazem Montasser
 *
 *******************************************************************************/

#ifndef LCD_H_
#define LCD_H_

#include "std_types.h"	/* To use uint8, etc...*/
#include "gpio.h"		/* For port & pins definitions */
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
/* LCD Data bits mode configuration, its value should be 4 or 8*/
#define LCD_MODE 8
/* LCD pin connections, either on first 4 pins (P0-P3) of the port or last 4 pins (P4-P7)*/
#define LCD_FIRST_4_PINS 1
#define LCD_LAST_4_PINS  0
#if((LCD_MODE != 4) && (LCD_MODE != 8))

#error "LCD setup pins should be either first or last port pins ONLY"

#elif ((LCD_FIRST_4_PINS==1) && (LCD_LAST_4_PINS==1)) ||((LCD_FIRST_4_PINS==0) && (LCD_LAST_4_PINS==0))

#error "LCD setup pins should be either first or last port pins ONLY"

#endif

/*******LCD pins*********/
#define LCD_DATA_PORT PORTC_ID
#define LCD_CONTROL_PORT PORTD_ID
#define LCD_RS PIN2_ID
#define LCD_RW PIN3_ID
#define LCD_ENABLE PIN4_ID
/*******LCD configurations*********/
#define LCD_MAX_ROWS 4
#define LCD_MAX_COLS 16
#define LCD_ROW_0 0
#define LCD_ROW_1 1
#define LCD_ROW_2 2
#define LCD_ROW_3 3
/*******LCD commands*********/
#if (LCD_MODE==4)
#define LCD_4BIT_MODE LCD_MODE
#define LCD_COMMAND_CLEAR 		 (0x01)	/* Clear screen command */
#define LCD_COMMAND_4BIT_INIT	 (0x02)	/* LCD initialization command in for 4 bit mode */
#define LCD_COMMAND_MODE 		 (0x28)	/* 5x7 2 lines mode */
#else
#define LCD_COMMAND_CLEAR 		 (0x01) /* Clear screen command */
#define LCD_COMMAND_MODE 		 (0x38) /* 5x7 2 lines mode */
#endif

#define LCD_COMMAND_CURSOR_BLINK (0x0F) /* Enable cursor blinking */
#define LCD_COMMAND_FORCE_CURSOR (0x80)	/* Force cursor position at 0,0 */
#define LCD_COMMAND_CURSOR_OFF 	 (0x0C)	/* Turn off blinking cursor */
#define LCD_COMMAND_DISPCURS_ON	 (0x0E) /* Display on, cursor on */
#define LCD_COMMAND_DISPLAY_OFF	 (0x08) /* Turn off display*/

/*******LCD registers*********/
#define LCD_REG_COL_0 (0x00)
#define LCD_REG_COL_1 (0x40)
#define LCD_REG_COL_2 (0x10)
#define LCD_REG_COL_3 (0x50)
/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
/*Description: Initializes the LCD data bus & control pins
 * according to the pre-dfined preprocessor definitions
 * */
void LCD_init(void);

/*Description: Sends a command to the LCD as given by a_data */
void LCD_sendCommand(uint8 a_data);

/*Description: Displays a single character given by a_char */
void LCD_displayCharacter(uint8 a_char);

/*Description: Displays a string on the LCD from a character array
 * If the number of characters overflow the maximum LCD columns,
 * the function will print from the current cursor position until end of row only
 * */
void LCD_displayString(const uint8 *str);

/*
 * Description: Moves the cursor to the specified row, column coordinates on the LCD
 * If a_row is more than the available rows, the function places the cursor to the first line only
 * if a_col is more than the available columns, the function places the cursor starting from the first column only
 * */
void LCD_moveCursor(uint8 a_row, uint8 a_col);

/*
 * Description: Prints a string starting from the given row and column positions
 * If a_row is more than the available rows, the function prints to the first line only
 * if a_col is more than the available columns, the function prints starting from the first column only
 * */
void LCD_displayStringRowColumn(uint8 a_row, uint8 a_col, const uint8 *a_str);

/*
 * Description: Clears the LCD by sending the Clear command to the LCD
 * */

void LCD_clearScreen(void);
/*
 * Description: Converts a given integer (+ve, -ve) to an array of characters
 * to display it on the LCD
 * Numbers given are in Base - 10
 * */

void LCD_integerToString(int a_data);

#endif /* LCD_H_ */
