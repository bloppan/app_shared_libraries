/*
 * HTS221.h
 *
 *  Created on: Jul 1, 2021
 *      Author: bernardo
 */

#ifndef HTS221_H_
#define HTS221_H_


#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <dirent.h>
#include <dlfcn.h>
#include "../libI2C/I2C_export.h"
#include "../../app_includes/app_errors.h"
#include "../../app_includes/app_typedef.h"
//#define	DEBUG

/*
 * HTS221 library location
 */
#define		HTS221_LIBRARY_DIRECTORY	"/usr/lib/"
#define		HTS221_LIBRARY_NAME			"libHTS221.so.1"

/*
 * HTS221 I2C DATA
 */
#define		HTS221_ADDR			0x5F	/*	HTS221 I2C Address	*/
/*
 * HTS221 I2C Registers
 */
#define		WHO_AM_I			0x0F	/*	Identifier register	*/
#define		AV_CONF				0x10	/*	Configuration register	*/
#define		CTRL_REG1			0x20	/*	Control register	*/
#define		CTRL_REG2			0x21	/*	Control register	*/
#define		CTRL_REG3			0x22	/*	Control register	*/
#define		STATUS_REG			0x27	/*	Control register	*/
#define		HUMIDITY_OUT_L		0x28	/*	Output register	*/
#define		HUMIDITY_OUT_H		0x29	/*	Output register	*/
#define		TEMP_OUT_L			0x2A	/*	Output register	*/
#define		TEMP_OUT_H			0x2B	/*	Output register	*/
#define		H0_rH_x2			0x30	/*	Calibration register	*/
#define		H1_rH_x2			0x31	/*	Calibration register	*/
#define		T0_degC_x8			0x32	/*	Calibration register	*/
#define		T1_degC_x8			0x33	/*	Calibration register	*/
#define		T1_T0				0x35	/*	Calibration register	*/
#define		H0_T0_OUT_L			0x36	/*	Calibration register	*/
#define		H0_T0_OUT_H			0x37	/*	Calibration register	*/
#define		H1_T0_OUT_L			0x3A	/*	Calibration register	*/
#define		H1_T0_OUT_H			0x3B	/*	Calibration register	*/
#define		T0_OUT_L			0x3C	/*	Calibration register	*/
#define		T0_OUT_H			0x3D	/*	Calibration register	*/
#define		T1_OUT_L			0x3E	/*	Calibration register	*/
#define		T1_OUT_H			0x3F	/*	Calibration register	*/

/*
 * @brief	Estructura que almacena los datos de temperatura, humedad y calibracion
 * 			del sensor HTS221.
 */
typedef struct {

	float			H0_rH;			/*	Calibration data	*/
	float			H1_rH;			/*	Calibration data	*/
	float			T0_degC;		/*	Calibration data	*/
	float			T1_degC;		/*	Calibration data	*/
	uint16_t		H0_T0_OUT;		/*	Calibration data	*/
	uint16_t		H1_T0_OUT;		/*	Calibration data	*/
	uint16_t		T0_OUT;			/*	Calibration data	*/
	uint16_t		T1_OUT;			/*	Calibration data	*/
	float			T_OUT;			/*	Temperature data. A este valor se le aplica una interpolacion lineal para calcular el valor final	*/
	float			H_OUT;			/*  Humidity data. A este valor se le aplica una interpolacion lineal para calcular el valor final	*/

}HTS221_struct;


#endif /* HTS221_H_ */
