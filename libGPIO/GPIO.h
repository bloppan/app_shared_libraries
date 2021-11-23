/*
 * GPIO.h
 *
 *  Created on: Jul 23, 2021
 *      Author: bernardo
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <dirent.h>
#include <dlfcn.h>
#include "../../app_includes/app_typedef.h"

#define		GPIO_LIBRARY_DIRECTORY		"/usr/local/lib/"
#define		GPIO_LIBRARY_NAME			"libGPIO.so"

#define 	GPIO_NUMBER(bank, index)    ((((bank)-1)*32)+((index)&31))


/*
 * GPIO Definition
 */
#define		SIM_CTRL				GPIO_NUMBER(5, 7)		// Multiplexor de tarjeta SIM 					(id 135, J2.37 <-- 127)
#define		PORTASIM_PRES			GPIO_NUMBER(2, 4)		// Detector del PortaSIM 						(id 36,  J2.80 <-- 170)
#define		EN_5V_USB_MOB			GPIO_NUMBER(5, 9)		// Conector USB 5V del terminal movil			(id 137, J2.49 <-- 139)
#define		EN_4V2					GPIO_NUMBER(1, 4)		// Conector 4V2 bateria del terminal movil		(id 4,   J2.52 <-- 142)
#define		PWDN_CURR_SENSOR		GPIO_NUMBER(5, 1)		// Enable PAC1932								(id 129, J2.64 <-- 154)
#define		ALERT_CURR_SENSOR		GPIO_NUMBER(5, 8)		// Alert PAC1932								(id 136, J2.39 <-- 129)
#define		TEMP_DRDY				GPIO_NUMBER(1, 0)		// Temp data ready								(id 0,   J2.60 <-- 150)
#define		LED_RST					GPIO_NUMBER(1, 2)		// Enable PCA9532 								(id 2,   J2.48 <-- 138)


/*
 * GPIO Errors
 */
#define		OPENING_EXPORT_FILE			1
#define		WRITING_EXPORT_FILE			2
#define		OPENING_DIRECTION_FILE		3
#define		WRITING_DIRECTION_FILE		4
#define		INVALID_OUTPUT_VALUE		5
#define		OPENING_UNEXPORT_FILE		6
#define		WRITING_UNEXPORT_FILE		7
#define		OPENING_VALUE_FILE			8
#define		INVALID_DIRECTION_VALUE		9
#define		WRITING_VALUE_FILE			10
#define		READING_VALUE_FILE			11
#define		READING_DIRECTION_FILE		12


#endif /* GPIO_H_ */
