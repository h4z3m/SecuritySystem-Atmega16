/******************************************************************************
 *
 * File Name: system_modes.h
 *
 * Description: Header file for common system modes and definitions
 * 				between HMI & Control ECU's.
 *
 * Date Created: 10/28/2021
 *
 * Author: Hazem Montasser
 *
 *******************************************************************************/

#ifndef SYSTEM_MODES_H_
#define SYSTEM_MODES_H_

#define PASSWORD_LENGTH 		(7)    /* Actual password length(5) + '#' + '\0' = 7 */

#define MAX_PASSWORD_TRIES 		(3)	   /* Maximum password tries that the user can enter before triggering the alarm buzzer*/
#define SYNC_HANDSHAKE 			(0xED) /* Used for synchronizing UART of both micro-controllers*/
#define UART_EOS				('#')  /* UART End of String character used by both micro-controllers for communication */

/* System modes */
#define MODE_FIRST_BOOT 		(0xFF) /* First boot of system, no password yet*/
#define MODE_NORMAL_BOOT_LOCKED (0x00) /* Password is setup, but user has not entered password yet*/
#define MODE_NORMAL_BOOT_MAIN	(0x02) /* Main menu options to either unlock the door or change password*/
#define MODE_ALARM_MODE 		(0x03) /* Alarm mode in which buzzer triggers when user has failed to enter the password multiple times correctly*/
/* Control requests */
#define OPEN_DOOR_REQUEST 		(0x04) /* Sent by HMI to notify Control to open the door*/
#define CHANGE_PASS_REQUEST 	(0x05) /* Sent by HMI to notify Control to change password*/

#define ERROR 					(0x00)
#define SUCCESS					(0x01)
#endif /* SYSTEM_MODES_H_ */
