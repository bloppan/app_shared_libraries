/*
 * i2c.h
 *
 *  Created on: Jun 30, 2021
 *      Author: bernardo
 */

#ifndef I2C_H_
#define I2C_H_

#include <stdio.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include "../../app_includes/app_typedef.h"

//#define 	DEBUG

#define		I2C_0	"/dev/i2c-0"
#define 	I2C_1	"/dev/i2c-1"

/*
 * I2C library location
 */
#define		I2C_LIBRARY_DIRECTORY	"/usr/local/lib/"
#define		I2C_LIBRARY_NAME		"libI2C.so"

/*
 * I2C Errors
 */
#define		OPENING_I2C_PORT			1
#define		WRITING_IOCTL_FILE			2

/*
 * @brief	Estructura de parametros I2C de un dispositivo
 */
typedef struct {

	char* 		i2c_port;	// Puerto I2C al que esta conectado
	uint16_t 	dev_addr;	// Direccion I2C del dispositivo
	uint8_t 	reg_addr;	// Direccion del registro que se quiere leer/escribir
	uint8_t* 	data;		// Puntero al buffer de datos que se van a leer/escribir
	uint16_t 	length;		// Numero de bytes que se van a leer/escribir

}i2c_device_struct;


#endif /* I2C_H_ */

