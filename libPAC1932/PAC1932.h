/*
 * PAC1932.h
 *
 *  Created on: Jul 6, 2021
 *      Author: bernardo
 */

#ifndef PAC1932_H_
#define PAC1932_H_


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <dirent.h>
#include <dlfcn.h>

#include "../libI2C/I2C_export.h"
#include "../libGPIO/GPIO_export.h"

#include "../../app_includes/app_typedef.h"
#include "../../app_includes/app_errors.h"


//#define	DEBUG

/*
 * PAC1932 library location
 */
#define		PAC1932_LIBRARY_DIRECTORY	"/usr/lib/"
#define		PAC1932_LIBRARY_NAME		"libPAC1932.so.1"

/*
 * PAC1932 Static Params
 */
#define		TOTAL_CHANNELS		4			// Numero total de canales que mide el sensor
#define		R_SENSE				0.22f 		// Valor en ohmnios de la resistencia externa
#define		DEN_UNIPOLAR_MEAS	0x10000		// Denominador para medidas unipolares -> 2^16
#define		DEN_BIPOLAR_MEAS	0x1000		// Denominador para medidas bipolares  -> 2^15
#define		DEN_UNIPOLAR_MODE	0x10000000	// Denominador para modo unipolar -> 2^28
#define		DEN_BIPOLAR_MODE	0x1000000	// Denominador para modo bipolar  -> 2^27

/*
 * PAC1932 I2C DATA
 */
#define		PAC1932_ADDR		0x10	/*	PAC1932 I2C Address	*/
/*
 * PAC1932 I2C Registers
 */
#define		REFRESH				0x00
#define		CTRL_PAC1932		0x01
#define		ACC_COUNT			0x02
#define		VPOWER1_ACC			0x03
#define		VPOWER2_ACC			0x04
#define		VPOWER3_ACC			0x05
#define		VPOWER4_ACC			0x06
#define		VBUS1				0x07
#define		VBUS2				0x08
#define		VBUS3				0x09
#define		VBUS4				0x0A
#define		VSENSE1				0x0B
#define		VSENSE2				0x0C
#define		VSENSE3				0x0D
#define		VSENSE4				0x0E
#define		VBUS1_AVG			0x0F
#define		VBUS2_AVG			0x10
#define		VBUS3_AVG			0x11
#define		VBUS4_AVG			0x12
#define		VSENSE1_AVG			0x13
#define		VSENSE2_AVG			0x14
#define		VSENSE3_AVG			0x15
#define		VSENSE4_AVG			0x16
#define		VPOWER1				0x17
#define		VPOWER2				0x18
#define		VPOWER3				0x19
#define		VPOWER4				0x1A
#define		CHANNEL_DIS			0x1C
#define		NEG_PWR				0x1D
#define		REFRESH_G			0x1E
#define		REFRESH_V			0x1F
#define		SLOW				0x20
#define		CTRL_ACT			0x21
#define		CHANNEL_DIS_ACT		0x22
#define		NEG_PWR_ACT			0x23
#define		CTRL_LAT			0x24
#define		CHANNEL_DIS_LAT		0x25
#define		NEG_PWR_LAT			0x26
#define		PID					0xFD
#define		MID					0xFE
#define		REV					0xFF
/*
 * PAC1932 I2C Registers Length
 */
#define		REFRESH_LENGTH			1
#define		CTRL_LENGTH				1
#define		ACC_COUNT_LENGTH		3
#define		VPOWER_ACC_LENGTH		6
#define		VBUS_LENGTH				2
#define		VSENSE_LENGTH			2
#define		VBUS_AVG_LENGTH			2
#define		VSENSE_AVG_LENGTH		2
#define		VPOWER_LENGTH			4
#define		CHANNEL_DIS_LENGTH		1
#define		NEG_PWR_LENGTH			1
#define		REFRESH_G_LENGTH		1
#define		REFRESH_V_LENGTH		1
#define		SLOW_LENGTH				1
#define		CTRL_ACT_LENGTH			1
#define		CHANNEL_DIS_ACT_LENGTH	1
#define		NEG_PWR_ACT_LENGTH		1
#define		CTRL_LAT_LENGTH			1
#define		CHANNEL_DIS_LAT_LENGTH	1
#define		NEG_PWR_LAT_LENGTH		1
#define		PID_LENGTH				1
#define		MID_LENGTH				1
#define		REV_LENGTH				1


#endif /* PAC1932_H_ */


